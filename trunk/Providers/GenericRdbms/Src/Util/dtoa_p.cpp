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
#include <locale.h>
#include <Inc/ut.h>

char * ut_dtoa_p(
        double d,
        int p,
        char   *ptr
        )
{
    int             left;                   /*  digits to left of decimal   */
    int             right;                  /*  digits to right of decimal  */
    register char   *end;                   /*  end of formatted string     */
    char            radix = '\0';                   /*  radix character             */
    struct lconv    *nls;

    
    nls = localeconv();
    if (nls)
        radix = nls->decimal_point[0];
    if (radix == '\0')
        radix = '.';

    if (d > 0)                              /*  determine digits to left    */
        left = (int)(log10(d) + 1.0);               /*      of decimal point        */
    else if (d < 0)                         /*  note: add 1.0 before int cnv*/
        left = (int)(log10(-d) + 1.0);              /*      so that: 1.n=>1, 0.n=>0 */
    else                                    /*          not 0.n=>1          */
        left = 0;
    if (left < 0)                           /*  treat 0 like .nnnn ...      */
        left = 0;

    right = p - left;                       /*  determine digits allowed    */
    if (right < 0)                          /*      on right, within        */
        right = 0;                          /*          precision           */

                                            /*  format with appropriate     */
    sprintf (ptr, "%.*f", right, d);        /*      decimals                */
    end = ptr + strlen(ptr) -1;

                                            /*  note: sprintf rounds dec    */
    if (right > 0) {
        while (*end == '0')                 /*  remove trailing '0's if     */
            end--;                          /*      there is a decimal      */
    }
    if (*end == radix)
        *end = '\0';                        /*  remove radix if no decimal  */
    else
        *(end+1) = '\0';                    /*  keep last non-zero          */

    if (strcmp(ptr, "-0") == 0)
        strcpy(ptr, "0");


    return (ptr);
}
