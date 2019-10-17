/* $XConsortium: ddxLEDs.c /main/3 1996/01/14 16:45:52 kaleb $ */
/************************************************************
Copyright (c) 1993 by Silicon Graphics Computer Systems, Inc.

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of Silicon Graphics not be 
used in advertising or publicity pertaining to distribution 
of the software without specific prior written permission.
Silicon Graphics makes no representation about the suitability 
of this software for any purpose. It is provided "as is"
without any express or implied warranty.

SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#include <stdio.h>
#define	NEED_EVENTS 1
#include <X11/X.h>
#include <X11/Xproto.h>
#include <X11/keysym.h>
#include "inputstr.h"
#include "scrnintstr.h"
#include "windowstr.h"
#include "XKBsrv.h"
#include "XI.h"

void
#if NeedFunctionPrototypes
XkbDDXUpdateIndicators(DeviceIntPtr dev,CARD32 new)
#else
XkbDDXUpdateIndicators(dev,new)
    DeviceIntPtr  dev;
    CARD32 new;
#endif
{
    dev->kbdfeed->ctrl.leds= new;
    (*dev->kbdfeed->CtrlProc)(dev,&dev->kbdfeed->ctrl);
    return;
}

void
#if NeedFunctionPrototypes
XkbDDXUpdateDeviceIndicators(	DeviceIntPtr		dev,
				XkbSrvLedInfoPtr 	sli,
				CARD32 			new)
#else
XkbDDXUpdateDeviceIndicators(dev,sli,new)
    DeviceIntPtr  	dev;
    XkbSrvLedInfoPtr	sli;
    CARD32 		new;
#endif
{
    if (sli->fb.kf==dev->kbdfeed)
	XkbDDXUpdateIndicators(dev,new);
    else if (sli->class==KbdFeedbackClass) {
	KbdFeedbackPtr	kf;
	kf= sli->fb.kf;
	if (kf && kf->CtrlProc) {
	    (*kf->CtrlProc)(dev,&kf->ctrl);
	}
    }
    else if (sli->class==LedFeedbackClass) {
	LedFeedbackPtr	lf;
	lf= sli->fb.lf;
	if (lf && lf->CtrlProc) {
	    (*lf->CtrlProc)(dev,&lf->ctrl);
	}
    }
    return;
}
