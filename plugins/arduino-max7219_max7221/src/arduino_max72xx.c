/*
 * libniftyled - Interface library for LED interfaces
 * Copyright (C) 2010 Daniel Hiepler <daniel@niftylight.de>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <niftyled.h>
#include "config.h"
#include "arduino_max72xx.h"




/** private info of our "hardware" */
struct priv
{
	/* LedHardware descriptor for this plugin */
        LedHardware *            hw;
	/* id - for our plugin, path to tty */
        char                     id[1024];
	/* amount of LEDs connected to arduino */
        LedCount                 ledcount;
	/* buffer to contain our monochrome buffer */
	unsigned char *		 buffer;
        /* threshold to use for greyscale -> monochrome conversion */
        int			 threshold;
	/* scan limit for MAX72xx multiplexing */
	int			 scan_limit;
	/* file descriptor for serial port */
	int			 fd;
	/* place to save current termios of serial port */
	struct termios		 oldtio;
};



/** send data packet to arduino */
NftResult ad_txPacket(struct priv *p, char opcode, char *data, char size)
{

	/* send opcode */
	if(write(p->fd, &opcode, 1) == -1)
	{
		NFT_LOG_PERROR("write()");
		return NFT_FAILURE;
	}
	
	/* send datasize */
	if(write(p->fd, &size, 1) == -1)
	{
		NFT_LOG_PERROR("write()");
		return NFT_FAILURE;
	}
	
	ssize_t sent = 0;
	ssize_t send = (ssize_t) size;
	
	while(send > 0)
	{
		if((sent = write(p->fd, &data[sent], send)) == -1)
		{
			NFT_LOG_PERROR("write()");
			return NFT_FAILURE;
		}
		
		send -= sent;
	}
	
	return NFT_SUCCESS;
}


/** send buffer to arduino */
NftResult ad_sendBuffer(struct priv *p, char *buf, char size)
{
	NFT_LOG(L_NOISY, "Uploading to arduino: %d bytes", size);
	return ad_txPacket(p, OP_UPLOAD, buf, size);
}


/** latch previously sent data to LEDs */
NftResult ad_latch(struct priv *p)
{
	return ad_txPacket(p, OP_LATCH, NULL, 0);
}


/** set scan limit */
NftResult ad_setScanLimit(struct priv *p, char scan_limit)
{
	return ad_txPacket(p, OP_SET_SCANLIMIT, &scan_limit, 1);
}


/** set amount of MAX72xx chips connected to arduino */
NftResult ad_setChipcount(struct priv *p, char chipcount)
{
	return ad_txPacket(p, OP_SET_CHIPCOUNT, &chipcount, 1);
}

/** set intensity */
NftResult ad_setIntensity(struct priv *p, char intensity)
{
	return ad_txPacket(p, OP_SET_GAIN, &intensity, 1);
}



/******************************************************************************/

/**
 * called upon plugin load
 *
 * You should:
 * - do any non-hardware initialization/checking here 
 * - register plugin properties
 * - fill in "privdata" if you want to use a private pointer that's passed to
 *   the plugin in subsequent calls.
 *
 * @param privdata space for a private pointer.  
 * @param h LedHardware descriptor belonging to this plugin
 * @result NFT_SUCCESS or NFT_FAILURE upon error
 */
static NftResult _init(void **privdata, LedHardware *h)
{
        NFT_LOG(L_DEBUG, "Initializing arduino-max72xx plugin...");

	
        /* allocate private structure */
        struct priv *p;
        if(!(p = calloc(1,sizeof(struct priv))))
        {
                NFT_LOG_PERROR("calloc");
                return NFT_FAILURE;
        }

	
        /* register our config-structure */
        *privdata = p;
        
        /* save our hardware descriptor */
        p->hw = h;

	
        /* defaults */
        p->ledcount = 0;
	p->threshold = 128;
	
	/* 
         * register some dynamic properties for this plugin - those will be
         * set/read in the _get/set_handler() from this plugin
         */
    	if(!led_hardware_plugin_prop_register(h, "threshold", LED_HW_CUSTOM_PROP_INT))
                return NFT_FAILURE;
	if(!led_hardware_plugin_prop_register(h, "scan_limit", LED_HW_CUSTOM_PROP_INT))
                return NFT_FAILURE;
        
        return NFT_SUCCESS;
}


/**
 * called upon plugin unload. Hardware will be deinitialized before that.
 * 
 * You should:
 * - unregister all properties you registered before 
 * - finally free all resources you needed 
 *
 */
static void _deinit(void *privdata)
{
        NFT_LOG(L_DEBUG, "Deinitializing arduino-max72xx plugin...");   

        struct priv *p = privdata;
        
        /* unregister or settings-handlers */
        led_hardware_plugin_prop_unregister(p->hw, "threshold");
	led_hardware_plugin_prop_unregister(p->hw, "scan_limit");
	
	/* free buffer */
	free(p->buffer);
	
        /* free structure we allocated in _init() */
        free(privdata);
}



/**
 * initialize hardware
 */
static NftResult _hw_init(void *privdata, const char *id)
{
        NFT_LOG(L_DEBUG, "Initializing arduino-max72xx hardware");

        struct priv *p = privdata;

        /* ... do checks ... */
        
        /* pixelformat supported? */
        LedPixelFormat *format = led_chain_get_format(led_hardware_get_chain(p->hw));
        if(led_pixel_format_get_bytes_per_pixel(format) != 1)
	{
		NFT_LOG(L_ERROR, "This hardware only supports 1 bpp formats (e.g. \"Y u8\")");
		return NFT_FAILURE;
	}
	
        if(led_pixel_format_get_n_components(format) != 1)
	{
		NFT_LOG(L_ERROR, "This hardware only supports 1 component per pixel (e.g. \"Y u8\")");
		return NFT_FAILURE;
	}

        const char *fmtstring = led_pixel_format_to_string(led_chain_get_format(
                                                led_hardware_get_chain(p->hw)));
        NFT_LOG(L_DEBUG, "Using \"%s\" as pixel-format", fmtstring);
        
        /* 
	 * check if id = "*" in this case we should try to automagically find our device,
	 * we'll just use a default in this case 
	 */
	if(strcmp(id, "*") == 0)
	{
		strncpy(p->id, "/dev/ttyUSB0", sizeof(p->id));
	}
	else
	{
	    	/* copy our id (and/or change it) */
	    	strncpy(p->id, id, sizeof(p->id));
	}

	/* open serial port */
	if((p->fd = open(p->id, O_RDWR | O_NOCTTY | O_NONBLOCK)) == -1)
	{
		NFT_LOG(L_ERROR, "Failed to open port \"%s\"", p->id);
		NFT_LOG_PERROR("open()");
		return NFT_FAILURE;
	}

	/* save current port settings */
	tcgetattr(p->fd,&p->oldtio); 
	
      	/* set new port settings */
	struct termios newtio;
	newtio.c_cflag = B115200 | CS8 | CSTOPB | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;       //ICANON;
        newtio.c_cc[VMIN]=1;
        newtio.c_cc[VTIME]=0;
        tcflush(p->fd, TCIFLUSH);
        tcsetattr(p->fd,TCSANOW,&newtio);

	
        return NFT_SUCCESS;
}


/**
 * deinitialize hardware
 */
static void _hw_deinit(void *privdata)
{
        NFT_LOG(L_DEBUG, "Deinitializing arduino-max72xx hardware");

	struct priv *p = privdata;
	
	/* restore old serial port settings */
	tcflush(p->fd, TCIFLUSH);
        tcsetattr(p->fd,TCSANOW,&p->oldtio);
	
	/* close serial port */
	close(p->fd);
}


/**
 * plugin getter - this will be called if core wants to get stuff from the plugin
 * @note you don't need to implement a getter for every single LedPluginParam
 */
NftResult _get_handler(void *privdata, LedPluginParam o, LedPluginParamData *data)
{
        struct priv *p = privdata;
        
        /** decide about object to give back to the core (s. hardware.h) */
        switch(o)
        {
                case LED_HW_ID:
                {
		    	NFT_LOG(L_DEBUG, "Getting id of arduino-max72xx hardware (%s)",
			            p->id);
		    
                        data->id = p->id;
                        return NFT_SUCCESS;
                }

                case LED_HW_LEDCOUNT:
                {
		    	NFT_LOG(L_DEBUG, "Getting arduino-max72xx hardware (%s) ledcount (%d LEDs)",
			            p->id, data->ledcount);
		    
                        data->ledcount = p->ledcount;
                        return NFT_SUCCESS;
                }

		case LED_HW_GAIN:
	    	{
			NFT_LOG(L_INFO, "Getting gain of LED %d from arduino-max72xx hardware (%s)",
			        data->gain.pos, p->id);
			
			data->gain.value = 0;
			return NFT_SUCCESS;
		}

                /* handle dynamic custom properties - 
                   we have to fill in data->custom.value.[s|i|f] and
                   data->custom.valuesize */
                case LED_HW_CUSTOM_PROP:
                {
                        if(strcmp(data->custom.name, "threshold") == 0)
                        {
                                data->custom.value.i = p->threshold;
                                data->custom.valuesize = sizeof(p->threshold);
				return NFT_SUCCESS;
                        }
			else if(strcmp(data->custom.name, "scan_limit") == 0)
                        {				
                                data->custom.value.i = p->scan_limit;
                                data->custom.valuesize = sizeof(p->threshold);
				return NFT_SUCCESS;
                        }
                        else
                        {
                                NFT_LOG(L_ERROR, "Unhandled custom property \"%s\"", data->custom.name);
                                return NFT_FAILURE;
                        }
                }
                        
                default:
                {
                        NFT_LOG(L_ERROR, "Request to get unhandled object \"%d\" from plugin", o);
                        return NFT_FAILURE;
                }
        }

        return NFT_FAILURE;
}


/**
 * plugin setter - this will be called if core wants to set stuff
 * @note you don't need to implement a setter for every LedPluginParam
 */
NftResult _set_handler(void *privdata, LedPluginParam o, LedPluginParamData *data)
{
        struct priv *p = privdata;
        
        /** decide about type of data (s. hardware.h) */
        switch(o)
        {
                case LED_HW_ID:
                {
                        strncpy(p->id, data->id, sizeof(p->id));
                        return NFT_SUCCESS;
                }

                case LED_HW_LEDCOUNT:
                {
			/* validate range */
			if(data->ledcount < 0 || data->ledcount > 512)
			{
				NFT_LOG(L_ERROR, "This hardware can't control less than 0 or more than 512 LEDs");
				return NFT_SUCCESS;
			}

			/* ledcount must be a multiple of 8 */
			/*if(data->ledcount % 8 != 0)
			{
				p->ledcount = (data->ledcount/8+1)*8;
				NFT_LOG(L_WARNING, "Ledcount (%d) is no multiple of 8, using %d.", data->ledcount, p->ledcount);
			}
			else
			{*/
                        	p->ledcount = data->ledcount;
			/*}*/

			/* set chipcount at arduino */
			char chipcount = (char) (p->ledcount%64 == 0 ? p->ledcount/64 : p->ledcount/8+1);
			NFT_LOG(L_DEBUG, "Setting chipcount to %d", chipcount);
			
			return ad_setChipcount(p, chipcount);
                }

		case LED_HW_GAIN:
		{
			NFT_LOG_TODO();
			return NFT_SUCCESS;
		}
			
                 /* handle dynamic custom properties - 
                   we can read out data->custom.value.[s|i|f] and
                   data->custom.valuesize */
                case LED_HW_CUSTOM_PROP:
                {
                        if(strcmp(data->custom.name, "threshold") == 0)
                        {
                                /* set new value */
                                p->threshold = data->custom.value.i;
				
                                NFT_LOG(L_DEBUG, "Setting \"threshold\" of \"%s\" to %d", p->id, p->threshold);
				return NFT_SUCCESS;
                        }
			else if(strcmp(data->custom.name, "scan_limit") == 0)
                        {
				if(data->custom.value.i < 0 || data->custom.value.i >= 8)
				{
					NFT_LOG(L_ERROR, "Scan limit %d outside range (0-7)", data->custom.value.i);
					return NFT_FAILURE;
				}
				
                                /* set new value */
                                p->scan_limit = data->custom.value.i;
                                NFT_LOG(L_DEBUG, "Setting \"scan_limit\" of \"%s\" to %d", p->id, p->threshold);
				ad_setScanLimit(p, p->scan_limit);
				
				return NFT_SUCCESS;
                        }
                        else
                        {
                                NFT_LOG(L_ERROR, "Unhandled custom property \"%s\"", data->custom.name);
                                return NFT_FAILURE;
                        }
                }
                        
                default:
                {
                        return NFT_SUCCESS;
                }
        }
        
        return NFT_FAILURE;
}


/**
 * send data in chain to hardware (only use this if hardware doesn't show data right after
 * data is received to avoid blanking. If the data is shown immediately, you have
 * to transmit it in _show() 
 */
NftResult _send(void *privdata, LedChain *c, LedCount count, LedCount offset)
{
        NFT_LOG(L_DEBUG, "Sending arduino-max72xx data");       

	struct priv *p = privdata;
	
	/* 8 bits-per-pixel buffer as given by niftyled */
        char *buffer = led_chain_get_buffer(c);
        /* 1 bit-per-pixel buffer as needed by arduino */
	char packed[64];

	/* clear buffer */
	memset(packed, 0, sizeof(packed));
		
	/* convert to 8bpp to 1bpp */
	unsigned int i;
	for(i=0; i < p->ledcount; i++)
	{
		if(buffer[i] >= p->threshold)
			packed[i/8] |= 1;

		packed[i/8] = packed[i/8] << 1;
		NFT_LOG(L_DEBUG, "0x%x ", buffer[i]);
	}

	NFT_LOG(L_DEBUG, "Packed: %x %x %x %x %x %x %x %x", packed[0], packed[1], packed[2], packed[3],
  packed[4], packed[5], packed[6], packed[7]); 

	packed[0] = 0xff;
	packed[1] = 0xff;
	packed[2] = 0xff;
	packed[3] = 0xff;
	packed[4] = 0xff;
	packed[5] = 0xff;
	packed[6] = 0xff;
	packed[7] = 0xff;
	packed[8] = 0xff;

	/* send buffer */
	ad_sendBuffer(p, packed, (p->ledcount%8 == 0 ? p->ledcount/8 : p->ledcount/8+1));
                
        return NFT_SUCCESS;
}


/**
 * trigger hardware to show data
 *
 * - if you previously uploaded your data to your hardware, output it now to LEDs
 * - if you can't do this, try to send all data except last bit/value/... in 
 *   _send() so that sent data is not immediately visible. Then send last portion here
 * - if you can't do this, send all data here as quick as possible :)
 */
NftResult _show(void *privdata)
{
        NFT_LOG(L_DEBUG, "Showing arduino-max72xx data");

	struct priv *p = privdata;
	
	ad_latch(p);
	
        return NFT_SUCCESS;
}





/** descriptor of hardware-plugin passed to the library */
LedHardwarePlugin hardware_descriptor =
{
        /** family name of the plugin (lib{family}-hardware.so) */
        .family = "arduino-max72xx",
	/** api major version */
        .api_major = HW_PLUGIN_API_MAJOR_VERSION,
    	/** api minor version */
        .api_minor = HW_PLUGIN_API_MINOR_VERSION,
    	/** api micro version */
        .api_micro = HW_PLUGIN_API_MICRO_VERSION,
        /** plugin version major */
        .major_version = 0,
        /** plugin version minor */
        .minor_version = 0,
        /** plugin version micro */
        .micro_version = 1,
        .license = "GPL",
        .author = "Daniel Hiepler <daniel@niftylight.de> (c) 2011-2012",
        .description = "Plugin to control MAX7219/MAX7221 and compatible connected to an arduino through a virtual serial port",
        .url = PACKAGE_URL,
        .id_example = "/dev/ttyUSB0",
        .plugin_init = _init,
        .plugin_deinit = _deinit,
        .hw_init = _hw_init,
        .hw_deinit = _hw_deinit,
        .get = _get_handler,
        .set = _set_handler,
        .show = _show,
        .send = _send,
};
