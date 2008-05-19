/*
 * 
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
/*NOTUSED*/

#include <Inc/tfn.h>
#include <Inc/nls.h>
#include <Inc/Nls/nls_msg.h>
#include "func.h"


#define GMLP_IS_NLS_ENABLED 0


#ifndef MS_WINDOWS
char *nls_cat = "nls.cat";
#else
char *nls_cat = "coremsg.dll";
#endif


static struct {
    int category;
    char *variable;
} locale[] = {
    { LC_CTYPE,     "LC_CTYPE" },
    { LC_COLLATE,   "LC_COLLATE" },
    { LC_TIME,      "LC_TIME" },
    { LC_NUMERIC,   "LC_NUMERIC" },
    { LC_MONETARY,  "LC_MONETARY" },
#if ! defined(ultrix)
    { LC_MESSAGES,  "LC_MESSAGES" },
#endif
    { 0,            NULL },     /* sentinel must be NULL */
};


/*
 *  nls_locale_set() -- Initialise the program's locale.
 *  
 *  This  function initialises the program's locale by calling
 *  setlocale(LC_ALL, "").
 *  Returns boolean TRUE if successful, FALSE otherwise.
 */
int nls_locale_set(void)
{
    register int i;

    /*
     * Fix for windows if LANG not set.
     */
#if defined(VISION_NUTC)
    if (getenv("LANG") == NULL)
    {
        if (!putenv("LANG", "C"))
            return(FALSE);
    }
#endif

    /*
     *  If we can't set the locale...
     */
    if (setlocale(LC_ALL, "") == NULL) {
        /*
         *  Issue message and ...
         */
#if 0
        nls_msg_issue('e', MF_NLS, S_NLS_1, NLS_6,
                            "cannot set locale because ...");
#endif
        /*
         *  Reset locale to something sane.
         */
        setlocale(LC_ALL, "C");
        /*
         *  Then determine which locale environment variable is invalid.
         */
        for (i = 0; locale[i].variable; i++) {
#if defined(sun)
            /*
             *  SunOS bug: because some locale data is missing
             *  skip over LC_CTYPE and LC_COLLATE categories
             */
            if (locale[i].category == LC_CTYPE
            || locale[i].category == LC_COLLATE)
                i++;
#endif
            /*
             *  If we can't set locale category and the corresponding
             *  locale environment variable is set...
             */
            if (setlocale(locale[i].category, "") == NULL
            && getenv(locale[i].variable) != NULL) {
                /*
                 *  Then that locale environment variable's value is invalid.
                 */
#if 0
                nls_msg_issue('i', MF_NLS, S_NLS_1, NLS_1,
                    "environment variable `%s' is invalid", locale[i].variable);
#endif
                return (FALSE);
            }
        }
        /*
         *  Otherwise LANG environment variable is undefined or invalid.
         */
#if 0
        if (getenv("LANG") == NULL)
            nls_msg_issue('i', MF_NLS, S_NLS_1, NLS_2,
                "environment variable `LANG' is undefined.");
        else
            nls_msg_issue('i', MF_NLS, S_NLS_1, NLS_3,
                "environment variable `LANG' is invalid.");
#endif
        return (FALSE);
    }
#if ! GMLP_IS_NLS_ENABLED 
    /*
     *  Reset numeric punctuations to C.
     */
    (void) setlocale(LC_NUMERIC, "C");
    /*
     *  Set environment variable LC_NUMERIC so that
     *  Oracle clients will inherit it and use '.' as
     *  the radix character.
     */
    putenv("LC_NUMERIC=C");
#endif
    return (TRUE);
}


#ifdef STANDALONE
/*
 * $Compile: lint -uwP -I/victor -I. %f
 */

#include <stdio.h>
#include <stdlib.h>

main(argc, argv)
    int argc;
    char *argv[];
{
    char datestring[20];
    char timestring[20];
#if defined(sun)
    struct dtconv *localdtconv();
    struct dtconv *dt;
#endif
    debug_init (&argc, argv);
    if (!nls_locale_set())
        printf("Locale is incomplete.\n");
    if (setlocale(LC_CTYPE, "") == NULL)
        printf("cannot set LC_CTYPE\n");
    if (setlocale(LC_NUMERIC, "") == NULL)
        printf("cannot set LC_NUMERIC\n");
    if (setlocale(LC_TIME, "") == NULL)
        printf("cannot set LC_TIME\n");
    if (setlocale(LC_MONETARY, "") == NULL)
        printf("cannot set LC_MONETARY\n");
    if (setlocale(LC_COLLATE, "") == NULL)
        printf("cannot set LC_COLLATE\n");
#if ! defined(ultrix)
    if (setlocale(LC_MESSAGES, "") == NULL)
        printf("cannot set LC_MESSAGES\n");
#endif
    ut_date_time(datestring, timestring);
    printf("date=`%s %s'\n", datestring, timestring);

#if ! defined(sun)
    printf("Date=`%s'\n", nl_langinfo(D_T_FMT));
    printf("Sun=`%s'\n", nl_langinfo(ABDAY_1));
    printf("Sunday=`%s'\n", nl_langinfo(DAY_1));
    printf("Jan=`%s'\n", nl_langinfo(ABMON_1));
    printf("January=`%s'\n", nl_langinfo(MON_1));
    printf("Apr=`%s'\n", nl_langinfo(ABMON_4));
    printf("Jun=`%s'\n", nl_langinfo(ABMON_6));
    printf("Jul=`%s'\n", nl_langinfo(ABMON_7));
    printf("June=`%s'\n", nl_langinfo(MON_6));
    printf("currency=`%s'\n", nl_langinfo(CRNCYSTR));
    printf("radix=`%s'\n", nl_langinfo(RADIXCHAR));
    printf("thousandth=`%s'\n", nl_langinfo(THOUSEP));
    printf("null=`%s'\n", nl_langinfo(""));
#else
    if ((dt = localdtconv()) == (struct dtconv *) NULL)
        exit(1);
    printf("Date=`%s'\n", dt->dtime_format);
    printf("Sdate=`%s'\n", dt->sdate_format);
    printf("Ldate=`%s'\n", dt->ldate_format);
    printf("Sun=`%s'\n", dt->abbrev_weekday_names[0]);
    printf("Sunday=`%s'\n", dt->weekday_names[0]);
    printf("Jan=`%s'\n", dt->abbrev_month_names[0]);
    printf("January=`%s'\n", dt->month_names[0]);
    printf("Apr=`%s'\n", dt->abbrev_month_names[3]);
    printf("Jun=`%s'\n", dt->abbrev_month_names[5]);
    printf("Jul=`%s'\n", dt->abbrev_month_names[6]);
    printf("June=`%s'\n", dt->month_names[5]);
#endif
    exit(EXIT_SUCCESS);
}

#endif /* STANDALONE */

