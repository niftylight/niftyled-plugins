/*
 * libniftyled - Interface library for LED interfaces
 * Copyright (C) 2010-2014 Daniel Hiepler <daniel@niftylight.de>
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

#include <niftyled.h>
#include <artnet/artnet.h>


/** private info of our "hardware" */
struct priv
{
        /* our LedHardware instance */
        LedHardware *hw;
        /* our artnet node instance */
        artnet_node *node;
        /* artnet address */
        char address[1024];
        /* artnet port */
        int port;
        /* amount of LEDs controlled by this plugin */
        LedCount leds;
};





/**
 * called upon plugin load
 */
static NftResult _init(void **privdata, LedHardware * h)
{
        NFT_LOG(L_INFO, "Initializing plugin...");


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





        // ~ /* register dynamic property for artnet address */
        // ~ if(!led_hardware_plugin_prop_register(h, "address",
        // LED_HW_CUSTOM_PROP_STRING))
        // ~ return NFT_FAILURE;
        // ~ /* register dynamic property for artnet port / universe */
        // ~ if(!led_hardware_plugin_prop_register(h, "port",
        // LED_HW_CUSTOM_PROP_INT))
        // ~ return NFT_FAILURE;

        return NFT_SUCCESS;
}


/**
 * called upon plugin unload. Hardware will be deinitialized before that.
 */
static void _deinit(void *privdata)
{
        NFT_LOG(L_INFO, "Deinitializing plugin...");

        // struct priv *p = privdata;

        /* unregister dynamic properties */
        // ~ led_hardware_plugin_prop_unregister(p->hw, "address");
        // ~ led_hardware_plugin_prop_unregister(p->hw, "port");

        /** free structure we allocated in _init() */
        free(privdata);
}



/**
 * initialize hardware
 */
static NftResult _hw_init(void *privdata, const char *id)
{
        NFT_LOG(L_DEBUG, "Initializing dummy hardware");

        // ~ struct priv *p = privdata;

        // ~ /* ... do checks ... */



        // ~ /* copy our id (and/or change it; check for "*" wildcard) */
        // ~ strncpy(p->id, id, sizeof(p->id));

        return NFT_SUCCESS;
}


/**
 * deinitialize hardware
 */
static void _hw_deinit(void *privdata)
{
        NFT_LOG(L_DEBUG, "Deinitializing dummy hardware");
}


/**
 * plugin getter - this will be called if core wants to get stuff from the plugin
 * @note you don't need to implement a getter for every single LedPluginParam
 */
NftResult _get_handler(void *privdata, LedPluginParam o,
                       LedPluginParamData * data)
{
        // ~ struct priv *p = privdata;

        // ~ /** decide about object to give back to the core (s. hardware.h)
        // */
        // ~ switch(o)
        // ~ {
        // ~ case LED_HW_ID:
        // ~ {
        // ~ NFT_LOG(L_INFO, "Getting id of dummy hardware (%s)",
        // ~ p->id);

        // ~ data->id = p->id;

        // ~ return NFT_SUCCESS;
        // ~ }

        // ~ case LED_HW_LEDCOUNT:
        // ~ {
        // ~ NFT_LOG(L_INFO, "Getting dummy hardware ledcount (%d LEDs)",
        // ~ data->ledcount);

        // ~ data->ledcount = p->ledcount;

        // ~ return NFT_SUCCESS;
        // ~ }

        // ~ case LED_HW_GAIN:
        // ~ {
        // ~ NFT_LOG(L_INFO, "Getting gain of LED %d (0)",
        // ~ data->gain.pos);

        // ~ data->gain.value = 0;

        // ~ return NFT_SUCCESS;
        // ~ }

        // ~ /* handle dynamic custom properties - 
        // ~ we have to fill in data->custom.value.[s|i|f] and
        // ~ data->custom.valuesize */
        // ~ case LED_HW_CUSTOM_PROP:
        // ~ {
        // ~ /** foo? */
        // ~ if(strcmp(data->custom.name, "foo") == 0)
        // ~ {
        // ~ data->custom.value.s = p->foo;
        // ~ data->custom.valuesize = sizeof(p->foo);

        // ~ return NFT_SUCCESS;
        // ~ }
        // ~ else if(strcmp(data->custom.name, "bar") == 0)
        // ~ {
        // ~ data->custom.value.i = p->bar;
        // ~ data->custom.valuesize = sizeof(p->bar);
        // ~ }
        // ~ else
        // ~ {
        // ~ NFT_LOG(L_ERROR, "Unhandled custom property \"%s\"",
        // data->custom.name);
        // ~ return NFT_FAILURE;
        // ~ }
        // ~ }

        // ~ default:
        // ~ {
        // ~ NFT_LOG(L_ERROR, "Request to get unhandled object \"%d\" from
        // plugin",
        // ~ o);
        // ~ return NFT_FAILURE;
        // ~ }
        // ~ }

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
                        strncpy(p->address, data->id, sizeof(p->address));
                        return NFT_SUCCESS;
                }

                case LED_HW_LEDCOUNT:
                {
                        p->leds = data->ledcount;
                        return NFT_SUCCESS;
                }

                        /* handle dynamic custom properties - we can read out
                         * data->custom.value.[s|i|f] and
                         * data->custom.valuesize */
                case LED_HW_CUSTOM_PROP:
                {
                        // ~ /** address? */
                        // ~ if(strcmp(data->custom.name, "foo") == 0)
                        // ~ {
                        // ~ /* check valuesize */
                        // ~ if(data->custom.valuesize > sizeof(p->foo))
                        // ~ {
                        // ~ NFT_LOG(L_WARNING, "value of \"foo\" truncated.
                        // Continuing.");
                        // ~ }

                        // ~ /* do some sanity checks. Return NFT_FAILURE if
                        // they fail */
                        // ~ /* ... */

                        // ~ /* copy new string to our buffer */
                        // ~ strncpy(p->foo, data->custom.value.s,
                        // sizeof(p->foo)-1);
                        // ~ p->foo[sizeof(p->foo)-1] = '\0';

                        // ~ NFT_LOG(L_INFO, "Set \"foo\" to \"%s\"", p->foo);

                        // ~ return NFT_SUCCESS;
                        // ~ }
                        // ~ else if(strcmp(data->custom.name, "bar") == 0)
                        // ~ {
                        // ~ /* set new value */
                        // ~ p->bar = data->custom.value.i;

                        // ~ NFT_LOG(L_INFO, "Set \"bar\" to %d", p->bar);
                        // ~ }
                        // ~ else
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
        NFT_LOG(L_VERBOSE, "Sending dummy data");

        // ~ /* do nothing if no greyscale-values are not printed */
        // ~ if(nft_log_level_get() > L_DEBUG)
        // ~ return NFT_SUCCESS;

        // ~ /* print all greyscale-values */
        // ~ NFT_LOG(L_DEBUG, "Greyscale-Buffer:");

        // ~ char *buffer = led_chain_get_buffer(c);
        // ~ int i,a ;
        // ~ size_t pos;
        // ~ static char string[512];
        // ~ char *b = buffer;

        // ~ for(a=offset; a<count/10; a++)
        // ~ {
        // ~ char *tmp = string;
        // ~ size_t size = sizeof(string);

        // ~ for(i=0; i<10; i++)
        // ~ {
        // ~ //if(tmp >= string+count)
        // ~ //return NFT_SUCCESS;

        // ~ if((pos = snprintf(tmp, size, "0x%.2hhx ",*b++)) < 0)
        // ~ {
        // ~ NFT_LOG_PERROR("snprintf()");
        // ~ return NFT_FAILURE;
        // ~ }

        // ~ tmp += pos;
        // ~ size -= pos;
        // ~ }

        // ~ NFT_LOG(L_DEBUG, "%s", string);
        // ~ }

        // ~ /* send chainbuffer to hardware */
        // ~ NFT_LOG(L_DEBUG, "Sent %d LED values to dummy hardware",
        // led_chain_get_ledcount(c));

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
        NFT_LOG(L_DEBUG, "Showing dummy data");
        return NFT_SUCCESS;
}





/** descriptor of hardware-plugin passed to the library */
LedHardwarePlugin hardware_descriptor = {
        /** family name of the plugin (lib{family}-hardware.so) */
        .family = "udp_artnet",
        /** api major version */
        .api_major = LED_HW_PLUGIN_API_MAJOR_VERSION,
        /** api minor version */
        .api_minor = LED_HW_PLUGIN_API_MINOR_VERSION,
        /** api micro version */
        .api_micro = LED_HW_PLUGIN_API_MICRO_VERSION,
        /** plugin version major */
        .major_version = 0,
        /** plugin version minor */
        .minor_version = 0,
        /** plugin version micro */
        .micro_version = 1,
        .license = "GPL",
        .author = "Daniel Hiepler <daniel@niftylight.de> (c) 2012-2014",
        .description = "libartnet hardware plugin",
        .url = PACKAGE_URL,
        .id_example = "127.0.0.1",
        .plugin_init = _init,
        .plugin_deinit = _deinit,
        .hw_init = _hw_init,
        .hw_deinit = _hw_deinit,
        .get = _get_handler,
        .set = _set_handler,
        .show = _show,
        .send = _send,
};
