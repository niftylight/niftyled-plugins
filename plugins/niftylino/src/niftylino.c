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
#include <string.h>
#include <stdint.h>
#include <usb.h>
#include <niftyled.h>
#include "config.h"
#include "niftylino.h"
#include "niftylino_usb.h"




/** send message + payload to adapter */
static NftResult _adapter_usb_send(Niftylino * n, uint message, char *payload,
                                   size_t payload_size)
{
        if(!n->usb_handle)
                NFT_LOG_NULL(NFT_FAILURE);

        if(usb_control_msg(n->usb_handle, RECV_EP, message, 0, 0,
                           payload, payload_size, n->usb_timeout) < 0)
                return NFT_FAILURE;

        return NFT_SUCCESS;
}


/** receive payload from adapter */
/*static NftResult _adapter_usb_rcv(Niftylino *n, uint message, char *payload, size_t payload_size)
{
        if(!n->usb_handle)
                NFT_LOG_NULL(NFT_FAILURE);

        if(usb_control_msg(n->usb_handle,
                       USB_TYPE_CLASS | DIR_DEV_TO_HOST | USB_RECIP_INTERFACE,
                       message, 0, 0, payload, payload_size, n->usb_timeout) < 0)
                return NFT_FAILURE;

        return NFT_SUCCESS;
}*/




/** send gain value of LED */
static NftResult _set_gain(Niftylino * n, LedCount led, LedGain gain)
{
        if(!n)
                NFT_LOG_NULL(NFT_FAILURE);;

        if(led % LEDS_PER_CHIP != 0)
                return NFT_SUCCESS;

        /* used to set the gain of one chip in a chain */
        struct
        {
                uint32_t chip;
                uint8_t gain;
        } _Gain =
        {
                .chip = led / LEDS_PER_CHIP,
                        /* scale gain from (0x0 - 0xffff) to (0x0 - 0xff) */
        .gain = (uint8_t) (gain * 255 / LED_GAIN_MAX),};

        return _adapter_usb_send(n, NIFTY_SET_GAIN_NO_PROPAGATE,
                                 (char *) &_Gain, sizeof(_Gain));

}


/** set amount of LEDs connected to niftylino */
static NftResult _set_ledcount(Niftylino * n, LedCount ledcount)
{
        if(!n)
                NFT_LOG_NULL(NFT_FAILURE);

        struct
        {
                uint32_t leds;
        } _Chainlength;

        _Chainlength.leds = ledcount;

        return _adapter_usb_send(n, NIFTY_SET_CHAINLENGTH,
                                 (char *) &_Chainlength,
                                 sizeof(_Chainlength));
}

/** set width of brightness-values */
static NftResult _set_format(void *privdata, NiftylinoValueWidth w)
{
        Niftylino *n = privdata;

        if(!n)
                NFT_LOG_NULL(NFT_FAILURE);


        /* used to set input-data fieldwidth */
        static struct
        {
                uint32_t width;
        } _Bitwidth;

        /* insert value-width */
        _Bitwidth.width = w;

        /* send bitwidth */
        return _adapter_usb_send(n, NIFTY_SET_INPUT_BITWIDTH,
                                 (char *) &_Bitwidth, sizeof(_Bitwidth));
}

/*******************************************************************************
 *******************************************************************************
 ******************************************************************************/

/**
 * initialize this plugin
 */
static NftResult _init(void **privdata, LedHardware * hw)
{
        /** create niftylino descriptor */
        Niftylino *n;
        if(!(n = calloc(1, sizeof(Niftylino))))
        {
                NFT_LOG_PERROR("calloc");
                return NFT_FAILURE;
        }

        /* save Niftylino descriptor as private-data */
        *privdata = n;

        /* initialize default usb timeout */
        n->usb_timeout = 2500;

        /* save our hardware descriptor for later */
        n->hw = hw;

        /* initialize usb subsystem */
        usb_init();

        /* enable debugging */
        int level = 0;
        if(nft_log_level_get() >= L_DEBUG)
                level = 3;
        else if(nft_log_level_get() >= L_VERBOSE)
                level = 2;
        else if(nft_log_level_get() >= L_WARNING)
                level = 1;

        usb_set_debug(level);

        return NFT_SUCCESS;
}


/**
 * deinitialize plugin
 */
static void _deinit(void *privdata)
{

        /* deinitialize hardware */
        Niftylino *n = privdata;
        if(n)
                free(n);

}



/**
 * initialize hardware
 */
static NftResult _usb_init(void *privdata, const char *id)
{
        Niftylino *n = privdata;

        struct usb_bus *bus;
        struct usb_device *dev;
        struct usb_dev_handle *h;

        /* save id */
        strncpy(n->id, id, sizeof(n->id));

        /* get our chain */
        LedChain *chain = led_hardware_get_chain(n->hw);

        /* pixel-format of our chain */
        LedPixelFormat *format = led_chain_get_format(chain);

        /* pixelformat supported? */
        NiftylinoValueWidth vw;
        switch (led_pixel_format_get_bytes_per_pixel(format) /
                led_pixel_format_get_n_components(format))
        {
                        /* 8 bit values */
                case 1:
                {
                        vw = NIFTYLINO_8BIT_VALUES;
                        break;
                }

                        /* 16 bit values */
                case 2:
                {
                        vw = NIFTYLINO_16BIT_VALUES;
                        break;
                }

                        /* unsupported format */
                default:
                {
                        NFT_LOG(L_ERROR,
                                "Unsupported format requested: %d bytes-per-pixel, %d components-per-pixel",
                                led_pixel_format_get_bytes_per_pixel(format),
                                led_pixel_format_get_n_components(format));

                        return NFT_FAILURE;
                }
        }


        /* find (new) busses */
        usb_find_busses();

        /* find (new) devices */
        usb_find_devices();



        /* open niftylino usb-device */
        char serial[255];

        /* walk all busses */
        for(bus = usb_get_busses(); bus; bus = bus->next)
        {
                /* walk all devices on bus */
                for(dev = bus->devices; dev; dev = dev->next)
                {
                        /* found niftylino? */
                        if((dev->descriptor.idVendor != VENDOR_ID) ||
                           (dev->descriptor.idProduct != PRODUCT_ID))
                        {
                                continue;
                        }


                        /* try to open */
                        if(!(h = usb_open(dev)))
                                /* device allready open or other error */
                                continue;

                        /* interface already claimed by driver? */
                        char driver[1024];
                        if(!(usb_get_driver_np(h, 0, driver, sizeof(driver))))
                        {
                                // NFT_LOG(L_ERROR, "Device already claimed by
                                // \"%s\"", driver);
                                continue;
                        }

                        /* reset device */
                        usb_reset(h);
                        usb_close(h);
                        // ~ if(usb_reset(h) < 0)
                        // ~ {
                        // ~ /* reset failed */
                        // ~ usb_close(h);
                        // ~ continue;
                        // ~ }

                        /* re-open */
                        if(!(h = usb_open(dev)))
                                /* device allready open or other error */
                                continue;

                        /* clear any previous halt status */
                        // usb_clear_halt(h, 0);

                        /* claim interface */
                        if(usb_claim_interface(h, 0) < 0)
                        {
                                /* device claim failed */
                                usb_close(h);
                                continue;
                        }

                        /* receive string-descriptor (serial number) */
                        if(usb_get_string_simple(h, 3, serial, sizeof(serial))
                           < 0)
                        {
                                usb_release_interface(h, 0);
                                usb_close(h);
                                continue;
                        }


                        /* device id == requested id? (or wildcard id
                         * requested? */
                        if(strlen(n->id) == 0 ||
                           strncmp(n->id, serial, sizeof(serial)) == 0 ||
                           strncmp(n->id, "*", 1) == 0)
                        {
                                /* serial-number... */
                                strncpy(n->id, serial, sizeof(n->id));
                                /* usb device handle */
                                n->usb_handle = h;

                                /* set format */
                                NFT_LOG(L_DEBUG, "Setting bitwidth to %d bit",
                                        (vw ==
                                         NIFTYLINO_8BIT_VALUES ? 8 : 16));

                                if(!_set_format(privdata, vw))
                                        return NFT_FAILURE;

                                return NFT_SUCCESS;
                        }

                        /* close this adapter */
                        usb_release_interface(h, 0);
                        usb_close(h);


                }
        }

        return NFT_FAILURE;
}


/**
 * deinitialize hardware
 */
static void _usb_deinit(void *privdata)
{
        Niftylino *n = privdata;

        if(!(n->usb_handle))
                return;

        usb_release_interface(n->usb_handle, 0);
        usb_close(n->usb_handle);
        n->usb_handle = NULL;
}

/**
 * plugin getter - this will be called if core wants to get stuff
 */
NftResult _get_handler(void *privdata, LedPluginParam o,
                       LedPluginParamData * data)
{
        Niftylino *n = privdata;

        /** decide about object to give back to the core (s. hardware.h) */
        switch (o)
        {
                case LED_HW_ID:
                {
                        data->id = n->id;
                        return NFT_SUCCESS;
                }

                case LED_HW_LEDCOUNT:
                {
                        data->ledcount = n->ledcount;
                        return NFT_SUCCESS;
                }

                default:
                {
                        NFT_LOG(L_ERROR,
                                "Request to get unhandled object from plugin");
                        return NFT_FAILURE;
                }
        }

        return NFT_FAILURE;
}


/**
 * plugin setter - this will be called if core wants to set stuff
 */
NftResult _set_handler(void *privdata, LedPluginParam o,
                       LedPluginParamData * data)
{
        Niftylino *n = privdata;

        /** decide about type of data (s. hardware.h) */
        switch (o)
        {
                case LED_HW_GAIN:
                {
                        return _set_gain(n, data->gain.pos, data->gain.value);
                }

                case LED_HW_ID:
                {
                        NFT_LOG(L_DEBUG, "Setting \"%s\" ID: %s",
                                led_hardware_get_name(n->hw), data->id);
                        strncpy(n->id, data->id, sizeof(n->id));
                        return NFT_SUCCESS;
                }

                case LED_HW_LEDCOUNT:
                {
                        NFT_LOG(L_DEBUG, "Setting \"%s\" to ledcount: %d",
                                led_hardware_get_name(n->hw), data->ledcount);
                        if(!_set_ledcount(n, data->ledcount))
                                return NFT_FAILURE;

                        n->ledcount = data->ledcount;
                        return NFT_SUCCESS;
                }

                default:
                {
                        return NFT_SUCCESS;
                }
        }

        return NFT_FAILURE;
}


/**
 * trigger hardware to show data
 */
NftResult _show(void *privdata)
{
        Niftylino *n = privdata;

        if(!n)
                NFT_LOG_NULL(NFT_FAILURE);

        /* latch hardware */
        return _adapter_usb_send(n, NIFTY_LATCH, NULL, 0);
}


/**
 * send data in chain to hardware (only use this if hardware doesn't show data right away
 * to avoid blanking)
 */
NftResult _send(void *privdata, LedChain * c, LedCount count, LedCount offset)
{

        NFT_LOG(L_NOISY, "Sending %d LEDs (Offset: %d)", count, offset);

        Niftylino *n = privdata;

        if(!n || !n->usb_handle)
                NFT_LOG_NULL(NFT_FAILURE);


        char *buffer = led_chain_get_buffer(c);

        if(usb_bulk_write(n->usb_handle, 1, buffer,
                          led_chain_get_buffer_size(c), n->usb_timeout) < 0)
        {
                return NFT_FAILURE;
        }

        return NFT_SUCCESS;

}










/** descriptor of hardware-plugin passed to the library */
LedHardwarePlugin hardware_descriptor = {
        /** family name of the plugin (lib{family}-hardware.so) */
        .family = "niftylino",
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
        .author = "Daniel Hiepler <daniel@niftylight.de> (c) 2006-201",
        .description = "Hardware plugin for niftylino LED-Controllers",
        .url = PACKAGE_URL,
        .id_example = "\"112441352785892847210780677\" or \"*\"",
        .plugin_init = _init,
        .plugin_deinit = _deinit,
        .hw_init = _usb_init,
        .hw_deinit = _usb_deinit,
        .get = _get_handler,
        .set = _set_handler,
        .show = _show,
        .send = _send,
};
