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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <niftyled.h>
#include "config.h"
#include "lpd8806-spi.h"




/** private info of our "hardware" */
struct priv
{
        /* LedHardware descriptor for this plugin */
        LedHardware *hw;
        /* id - for our plugin, path to tty */
        char id[1024];
        /* amount of LEDs connected to SPI port */
        LedCount ledcount;
        /* file descriptor for SPI device */
        int fd;
        /* SPI buffers */
        uint8_t spiMode;
        uint8_t spiBPW;
        uint16_t spiDelay;
        uint32_t spiSpeed;
        uint8_t *txBuffer;

};




/** send/receive SPI data */
NftResult spiTxData(int fd, uint8_t * tx, uint32_t len)
{
        /* SPI transfer descriptor */
        struct spi_ioc_transfer tr;

        /* zero initialize */
        memset(&tr, 0, sizeof(struct spi_ioc_transfer));

        tr.tx_buf = (unsigned long) tx;
        tr.len = len;
        tr.cs_change = false;

        if(ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1)
        {
                NFT_LOG_PERROR("Failed to send SPI message:");
                return NFT_FAILURE;
        }

        return NFT_SUCCESS;
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
static NftResult _init(void **privdata, LedHardware * h)
{
        NFT_LOG(L_DEBUG, "Initializing LDP8806 plugin...");


        /* allocate private structure */
        struct priv *p;
        if(!(p = calloc(1, sizeof(struct priv))))
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
        p->spiMode = SPI_MODE_0;
        p->spiBPW = 8;
        p->spiDelay = 0;
        p->spiSpeed = 500000;

        /* 
         * register some dynamic properties for this plugin - those will be
         * set/read in the _get/set_handler() from this plugin
         */
        if(!led_hardware_plugin_prop_register
           (h, "spi_speed", LED_HW_CUSTOM_PROP_INT))
                return NFT_FAILURE;
        if(!led_hardware_plugin_prop_register
           (h, "spi_delay", LED_HW_CUSTOM_PROP_INT))
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
        NFT_LOG(L_DEBUG, "Deinitializing LDP8806 plugin...");

        struct priv *p = privdata;

        /* unregister or settings-handlers */
        led_hardware_plugin_prop_unregister(p->hw, "spi_speed");
        led_hardware_plugin_prop_unregister(p->hw, "spi_delay");

        /* free structure we allocated in _init() */
        free(privdata);
}



/**
 * initialize hardware
 */
static NftResult _spi_init(void *privdata, const char *id)
{
        NFT_LOG(L_DEBUG, "Initializing LDP8806 hardware");

        struct priv *p = privdata;


        /* pixelformat supported? */
        LedPixelFormat *format =
                led_chain_get_format(led_hardware_get_chain(p->hw));
        const char *fmtstring = led_pixel_format_to_string(format);

        int bytes_per_pixel = led_pixel_format_get_bytes_per_pixel(format);
        int components_per_pixel = led_pixel_format_get_n_components(format);
        int bytes_per_component = bytes_per_pixel / components_per_pixel;

        if(bytes_per_component != 1)
        {
                NFT_LOG(L_ERROR,
                        "We need a format with 8 bits per pixel-component. Format %s has %d bytes-per-pixel and %d components-per-pixel.",
                        fmtstring, bytes_per_pixel, components_per_pixel);
                return NFT_FAILURE;
        }


        NFT_LOG(L_DEBUG, "Using \"%s\" as pixel-format", fmtstring);


        /* 
         * check if id = "*" in this case we should try to automagically find our device,
         * we'll just use a default in this case 
         * @todo cycle through all devices 
         */
        if(strcmp(id, "*") == 0)
        {
                strncpy(p->id, "/dev/spidev0.0", sizeof(p->id));
        }
        else
        {
                /* copy our id (and/or change it) */
                strncpy(p->id, id, sizeof(p->id));
        }

        /* open SPI port */
        if((p->fd = open(p->id, O_RDWR)) == -1)
        {
                NFT_LOG(L_ERROR, "Failed to open port \"%s\"", p->id);
                NFT_LOG_PERROR("open()");
                return NFT_FAILURE;
        }


        /* Set SPI parameters */
        if(ioctl(p->fd, SPI_IOC_WR_MODE, &p->spiMode) < 0)
                return NFT_FAILURE;

        if(ioctl(p->fd, SPI_IOC_RD_MODE, &p->spiMode) < 0)
                return NFT_FAILURE;

        if(ioctl(p->fd, SPI_IOC_WR_BITS_PER_WORD, &p->spiBPW) < 0)
                return NFT_FAILURE;

        if(ioctl(p->fd, SPI_IOC_RD_BITS_PER_WORD, &p->spiBPW) < 0)
                return NFT_FAILURE;

        if(ioctl(p->fd, SPI_IOC_WR_MAX_SPEED_HZ, &p->spiSpeed) < 0)
                return NFT_FAILURE;

        if(ioctl(p->fd, SPI_IOC_RD_MAX_SPEED_HZ, &p->spiSpeed) < 0)
                return NFT_FAILURE;

        NFT_LOG(L_DEBUG,
                "SPI \"%d\" initialized (mode: %d, bits-per-word: %d, speed-hz: %d)",
                p->spiMode, p->spiBPW, p->spiSpeed);

        return NFT_SUCCESS;
}


/**
 * deinitialize hardware
 */
static void _spi_deinit(void *privdata)
{
        NFT_LOG(L_DEBUG, "Deinitializing LDP8806 hardware");

        struct priv *p = privdata;

        close(p->fd);

        /* free buffer */
        // free(p->txBuffer);
}


/**
 * plugin getter - this will be called if core wants to get stuff from the plugin
 * @note you don't need to implement a getter for every single LedPluginParam
 */
NftResult _get_handler(void *privdata, LedPluginParam o,
                       LedPluginParamData * data)
{
        struct priv *p = privdata;

        /** decide about object to give back to the core (s. hardware.h) */
        switch (o)
        {
                case LED_HW_ID:
                {
                        NFT_LOG(L_DEBUG,
                                "Getting id of LDP8806 hardware (%s)", p->id);

                        data->id = p->id;
                        return NFT_SUCCESS;
                }

                case LED_HW_LEDCOUNT:
                {
                        NFT_LOG(L_DEBUG,
                                "Getting ledcount of LDP8806 hardware (%s): %d LEDs",
                                p->id, data->ledcount);

                        data->ledcount = p->ledcount;
                        return NFT_SUCCESS;
                }

                case LED_HW_GAIN:
                {
                        NFT_LOG(L_INFO,
                                "Getting gain of LED %d from LDP8806 hardware (%s)",
                                data->gain.pos, p->id);

                        /* @todo */
                        data->gain.value = 0;
                        return NFT_SUCCESS;
                }

                        /* handle dynamic custom properties - we have to fill
                         * in data->custom.value.[s|i|f] and
                         * data->custom.valuesize */
                case LED_HW_CUSTOM_PROP:
                {
                        /* @todo bugs ahead, will show with debug @todo fix
                         * chain != config output */
                        if(strcmp(data->custom.name, "spi_speed") == 0)
                        {
                                data->custom.value.i = (uint32_t) p->spiSpeed;
                                data->custom.valuesize = sizeof(uint32_t);
                                return NFT_SUCCESS;
                        }
                        else if(strcmp(data->custom.name, "spi_delay") == 0)
                        {
                                data->custom.value.i = (uint16_t) p->spiDelay;
                                data->custom.valuesize = sizeof(uint16_t);
                                return NFT_SUCCESS;
                        }
                        else
                        {
                                NFT_LOG(L_ERROR,
                                        "Unhandled custom property \"%s\"",
                                        data->custom.name);
                                return NFT_FAILURE;
                        }
                }

                default:
                {
                        NFT_LOG(L_ERROR,
                                "Request to get unhandled object \"%d\" from plugin",
                                o);
                        return NFT_FAILURE;
                }
        }

        return NFT_FAILURE;
}


/**
 * plugin setter - this will be called if core wants to set stuff
 * @note you don't need to implement a setter for every LedPluginParam
 */
NftResult _set_handler(void *privdata, LedPluginParam o,
                       LedPluginParamData * data)
{
        struct priv *p = privdata;

        /** decide about type of data (s. hardware.h) */
        switch (o)
        {
                case LED_HW_ID:
                {
                        strncpy(p->id, data->id, sizeof(p->id));
                        return NFT_SUCCESS;
                }

                case LED_HW_LEDCOUNT:
                {
                        /* resize buffer */
                        /* if(!(p->txBuffer = realloc(p->txBuffer,
                         * data->ledcount))) { NFT_LOG_PERROR("realloc");
                         * return NFT_FAILURE; } */

                        /* save ledcount */
                        p->ledcount = data->ledcount;

                        return NFT_SUCCESS;
                }

                case LED_HW_GAIN:
                {
                        NFT_TODO();
                        return NFT_SUCCESS;
                }

                        /* handle dynamic custom properties - we can read out
                         * data->custom.value.[s|i|f] and
                         * data->custom.valuesize */
                case LED_HW_CUSTOM_PROP:
                {
                        if(strcmp(data->custom.name, "spi_speed") == 0)
                        {
                                /* set new value */
                                p->spiSpeed = (uint32_t) data->custom.value.i;

                                NFT_LOG(L_DEBUG,
                                        "Setting \"spi_speed\" of \"%s\" to %u",
                                        p->id, p->spiSpeed);
                                return NFT_SUCCESS;
                        }
                        else if(strcmp(data->custom.name, "spi_delay") == 0)
                        {
                                /* set new value */
                                p->spiDelay = (uint16_t) data->custom.value.i;
                                NFT_LOG(L_DEBUG,
                                        "Setting \"spi_delay\" of \"%s\" to %hu",
                                        p->id, p->spiDelay);
                                return NFT_SUCCESS;
                        }
                        else
                        {
                                NFT_LOG(L_ERROR,
                                        "Unhandled custom property \"%s\"",
                                        data->custom.name);
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
NftResult _send(void *privdata, LedChain * c, LedCount count, LedCount offset)
{
        NFT_LOG(L_DEBUG, "Sending LDP8806 data");

        struct priv *p = privdata;
        uint8_t *buf = led_chain_get_buffer(c);

        /* seek to offset */
        int bytes_per_component =
                led_chain_get_buffer_size(c) / led_chain_get_ledcount(c);
        buf += offset * bytes_per_component;

        /* send buffer */
        return spiTxData(p->fd, buf, bytes_per_component * count);
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
        NFT_LOG(L_DEBUG, "Showing LDP8806 data");

        struct priv *p = privdata;

        /* send zero byte to latch */
        uint8_t latch = 0;

        return spiTxData(p->fd, &latch, 1);
}





/** descriptor of hardware-plugin passed to the library */
LedHardwarePlugin hardware_descriptor = {
        /** family name of the plugin (lib{family}-hardware.so) */
        .family = "lpd8806-spi",
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
        .author = "Daniel Hiepler <daniel@niftylight.de> (c) 2011-2013",
        .description =
                "Plugin to control LDP8806 and compatible connected to SPI port",
        .url = PACKAGE_URL,
        .id_example = "/dev/spidev0.0",
        .plugin_init = _init,
        .plugin_deinit = _deinit,
        .hw_init = _spi_init,
        .hw_deinit = _spi_deinit,
        .get = _get_handler,
        .set = _set_handler,
        .show = _show,
        .send = _send,
};
