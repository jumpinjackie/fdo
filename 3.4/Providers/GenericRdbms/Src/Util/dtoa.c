/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include <stdlib.h>
#include <Inc/ut.h>


#define DBL_PRECISION   12
#define FLT_PRECISION   6

char * ut_dtoa(
        double n,
        char   *ptr 
        )
{
    return ut_dtoa_p(n,DBL_PRECISION, ptr);
}
char * ut_ftoa(
        double n, 
        char   *ptr 
        )
{
    return ut_dtoa_p(n,FLT_PRECISION, ptr);
}

