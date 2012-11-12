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





#ifndef _NIFTYLINO_USB
#define _NIFTYLINO_USB

/** USB request types */
enum USB_SETUP_BM_REQUEST_TYPE_BITS
{
        RECV_DEVICE = 1,
        RECV_IFACE = 2,
        RECV_EP = 4,
        RECV_OTHER = 8,
        TYPE_STANDARD = (0) << 5,
        TYPE_CLASS = (1) << 5,
        TYPE_VENDOR = (2) << 5,
        DIR_HOST_TO_DEV = (0 << 7),
        DIR_DEV_TO_HOST = (1 << 7),
};

/** niftylino command IDs */
enum NIFTY_CTRL_MSG_ID
{
        NIFTY_LATCH = 10,
        NIFTY_SET_CHAINLENGTH,
        NIFTY_SET_GAIN,
        NIFTY_SET_GAMMA_VALUE,
        NIFTY_SET_INPUT_BITWIDTH,
        NIFTY_SET_GAIN_NO_PROPAGATE,
        NIFTY_GET_STATE,
        NIFTY_GET_VERSION,
        NIFTY_GET_CHAINLENGTH,
        NIFTY_GET_GAIN,
        NIFTY_GET_GAMMA_VALUE,
        NIFTY_GET_INPUT_BITWIDTH
};


#endif /* _NIFTYLINO_USB */
