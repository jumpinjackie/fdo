/*
 * Copyright (C) 2006 Refractions Research, Inc. 
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

#ifndef POSTGIS_DIGITS_H
#define POSTGIS_DIGITS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <limits.h>

/*
 * Compile time estimation of (log(10) / log(2)) used as precision for
 * calculation of number of digits in numeric type.
 */
#define LOG10PERLOG2    301L / 1000

/*
 * For the sake of safety, overestimation should not hurt us.
 */
#define OVERESTIMATOR   3

/*
 * A shortcut macros for calculating number of digits in integral type.
 */
#define NUMBER_OF_DIGITS10(x) \
    (((CHAR_BIT * sizeof(x) - 1) * LOG10PERLOG2) + OVERESTIMATOR)

#define NUMBER_OF_CHAR_DIGITS10 \
    ((CHAR_BIT - (CHAR_MIN != 0 ? 1 : 0)) * LOG10PERLOG2) + OVERESTIMATOR)

#define NUMBER_OF_UCHAR_DIGITS10 \
    ((CHAR_BIT - 1) * LOG10PERLOG2) + OVERESTIMATOR)

/*
 * Constants of number of digits for float types
 */
#define NUMBER_OF_FLOAT_DIGITS      100
#define NUMBER_OF_DOUBLE_DIGITS     100

#endif /* POSTGIS_DIGITS_H */