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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <niftyled.h>




int main(int argc, char *argv[])
{
        nft_log_level_set(L_DEBUG);

        /* register new dummy hardware called "nlo01" */
        LedHardware *h;
        if(!(h = led_hardware_new("nlo01", "dummy")))
        {
                NFT_LOG(L_ERROR, "Hardware creation FAILED");
                return -1;
        }

        /* initialize any hardware that can be found (id="*"), define 12
         * connected LEDs (4 BGR pixels). Greyscale data should be provided in
         * unsigned 8-bit format */
        if(!led_hardware_init(h, "*", 12, "BGR u8"))
        {
                NFT_LOG(L_ERROR, "failed to initialize hardware");
                return -1;
        }


                /**********************************************************************
		 * niftyled basically provides 2 ways to send data to LED hardware:
		 * 
		 * 1.  set "raw" greyscale value of a LED in a chain connected to a
		 *     correctly initialized LED hardware
		 * 
		 * 1.1 use the API to set each greyscale value
		 * 
		 * 1.2 directly write to the RAW buffer
		 *
		 *
		 * 2. send a complete pixelframe and have niftyled translate that to 
		 *    mapped LEDs connected to one or more LED hardware devices 
		 *    according to the current setup configuration (where each LED
		 *    has been assigned a X/Y coordinate and a "Component" that refers 
		 *    to a component of a pixel at this position in the pixelframe)
		 */


                /****** METHOD 1.1 *******/
        /* get chain of this hardware */
        LedChain *c = led_hardware_get_chain(h);

        /* set all LEDs to half brightness */
        LedCount l;
        for(l = 0; l < led_chain_get_ledcount(c); l++)
        {
                led_chain_set_greyscale(c, l, 128);
        }

        /* send data to hardware */
        led_hardware_send(h);

        /* show previously sent hardware (latch to LEDs) */
        led_hardware_show(h);



                /****** METHOD 1.2 *******/
        /* ...or access raw pixelbuffer */
        uint8_t *pixels = led_chain_get_buffer(c);

        /* walk all pixels to turn on all LEDs */
        int i;
        for(i = 0; i < 16; i++)
                pixels[i] = 0xff;

        /* send data to hardware */
        led_hardware_send(h);

        /* show previously sent hardware (latch to LEDs) */
        led_hardware_show(h);


                /****** METHOD 2 - s. ledcat sources *******/


        /* deinitialize hardware */
        led_hardware_deinit(h);

        return 0;

}
