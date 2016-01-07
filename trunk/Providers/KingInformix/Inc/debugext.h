// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
*                                                                       *
* Name                                                                  *
*   debugext - Declare pointers, flags required for debug statements.   *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/debugext.h>                                           *
*                                                                       *
* Description                                                           *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#ifndef DEBUGEXT_H
#define DEBUGEXT_H  1

#include <ctype.h>
#include <stdlib.h>

int debug_trace(
    char    *msg,
	wchar_t *msgW,
    int     *line_num
    );

#define debug_area()                                        \
    if ( 0 )

#define debug_init(p1, p2)
#define debug_main(ac, av) debug_init(ac, av)
#define debug_on(func)
#define debug_on1(func, fmt, p1)
#define debug_on2(func, fmt, p1, p2)
#define debug_on3(func, fmt, p1, p2, p3)
#define debug_on4(func, fmt, p1, p2, p3, p4)
#define debug_on5(func, fmt, p1, p2, p3, p4, p5)
#define debug_on6(func, fmt, p1, p2, p3, p4, p5, p6)
#define debug_on7(func, fmt, p1, p2, p3, p4, p5, p6, p7)
#define debug_on8(func, fmt, p1, p2, p3, p4, p5, p6, p7, p8)
#define debug_on9(func, fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)
#define debug_off(p1)
#define debug_return(fmt, p1)                               \
    { return(p1); }
#define debug_return_void(p1)                               \
    { return; }
#define debug0(fmt)
#define debug1(fmt, p1)
#define debug2(fmt, p1, p2)
#define debug3(fmt, p1, p2, p3)
#define debug4(fmt, p1, p2, p3, p4)
#define debug5(fmt, p1, p2, p3, p4, p5)
#define debug6(fmt, p1, p2, p3, p4, p5, p6)
#define debug7(fmt, p1, p2, p3, p4, p5, p6, p7)
#define debug8(fmt, p1, p2, p3, p4, p5, p6, p7, p8)
#define debug9(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)
#define debug_out0(fmt)
#define debug_out1(fmt, p1)
#define debug_out2(fmt, p1, p2)
#define debug_out3(fmt, p1, p2, p3)
#define debug_out4(fmt, p1, p2, p3, p4)
#define debug_out5(fmt, p1, p2, p3, p4, p5)
#define debug_out6(fmt, p1, p2, p3, p4, p5, p6)
#define debug_out7(fmt, p1, p2, p3, p4, p5, p6, p7)
#define debug_out8(fmt, p1, p2, p3, p4, p5, p6, p7, p8)
#define debug_out9(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)

#endif

