// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
/************************************************************************
*                                                                       *
* Name                                                                  *
*   msg.h - External header file for "msg" package                      *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/msg.h>                                                *
*                                                                       *
* Description                                                           *
*       Define  standard message stuff (constants and functions).       *
*                                                                       *
************************************************************************/

#ifndef MSG_H_DEFINED
#define MSG_H_DEFINED

#if defined(FULLPROTO) || (defined(VISION_NUTC_INC) && !defined(VISION_NUTC_INC_VARARGS))
#include <stdio.h>
#endif
#include <Inc/os.h>


#define MSGBUFSIZE 256

#define MSG_FLAG_MAX_STR_LEN    3

#define MSG_LEFT_JUSTIFY        32
#define MSG_BACKWARDS_DISPLAY   64

#define MSG_FLAG_C              'c'
#define MSG_FLAG_O              'o'
#define MSG_FLAG_I              'i'
#define MSG_FLAG_W              'w'
#define MSG_FLAG_E              'e'
#define MSG_FLAG_S              's'
#define MSG_FLAG_CL             'c'
#define MSG_FLAG_OL             'o'
#define MSG_FLAG_IL             'i'
#define MSG_FLAG_WL             'w'
#define MSG_FLAG_EL             'e'
#define MSG_FLAG_SL             's'
#define MSG_FLAG_CR             ('c'^MSG_LEFT_JUSTIFY)
#define MSG_FLAG_OR             ('o'^MSG_LEFT_JUSTIFY)
#define MSG_FLAG_IR             ('i'^MSG_LEFT_JUSTIFY)
#define MSG_FLAG_WR             ('w'^MSG_LEFT_JUSTIFY)
#define MSG_FLAG_ER             ('e'^MSG_LEFT_JUSTIFY)
#define MSG_FLAG_SR             ('s'^MSG_LEFT_JUSTIFY)
#define MSG_FLAG_CB             ('c'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_OB             ('o'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_IB             ('i'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_WB             ('w'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_EB             ('e'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_SB             ('s'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_CLB            ('c'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_OLB            ('o'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_ILB            ('i'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_WLB            ('w'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_ELB            ('e'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_SLB            ('s'^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_CRB            ('c'^MSG_LEFT_JUSTIFY^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_ORB            ('o'^MSG_LEFT_JUSTIFY^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_IRB            ('i'^MSG_LEFT_JUSTIFY^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_WRB            ('w'^MSG_LEFT_JUSTIFY^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_ERB            ('e'^MSG_LEFT_JUSTIFY^MSG_BACKWARDS_DISPLAY)
#define MSG_FLAG_SRB            ('s'^MSG_LEFT_JUSTIFY^MSG_BACKWARDS_DISPLAY)

#define MSG_TRAP_ENABLE "msg_custom_enable"
#define MSG_TRAP_CMD    "msg_custom_cmd"
#define MSG_TRAP_FILTER "msg_custom_filter"
#define MSG_TRAP_MSG    "msg_message"
#define MSG_TRAP_SEV    "msg_severity"
#define MSG_TRAP_COUNT  "msg_count"

#ifdef C_PLUSPLUS
extern "C"
{
#endif

#if defined(FULLPROTO) || defined(c_plusplus) || defined(__cplusplus)

typedef int (* msg_handlr_type)(
              char * message,
              char   msg_flag
              );
void msg_delay(
        char * message,
        char msg_flag
        );
void msg_display(
        void
        );
void msg_flag_cnvrt(
        char * flag_string,
        char * msg_flag
        );
void msg_flag_lkup(
        char msg_flag,
        char * flag_string
        );
void msg_get_handlr(
        msg_handlr_type * func
        );
void msg_issue_s(
        char * format,
        char * string,
        char severity
        );
int msg_issue(
        char * text,
        char severity
        );
void msg_issue_d(
        char * format,
        int number,
        char severity
        );
void msg_issue_sd(
        char * format,
        char * string,
        int number,
        char severity
        );
void msg_issue_ds(
        char * format,
        int number,
        char * string,
        char severity
        );
void msg_issue_ss(
        char * format,
        char * string1,
        char * string2,
        char severity
        );
void msg_issue_dd(
        char * format,
        int number1,
        int number2,
        char severity
        );
void msg_issue_cprt(
        void
        );
void msg_save(
        char * text,
        char severity
        );
void msg_set_handlr(
        msg_handlr_type func
        );
void msg_sys_err(
        char * user_text
        );
        


#else /* FULLPROTO */

typedef int (* msg_handlr_type)();
extern void msg_delay();
extern void msg_display();
extern void msg_flag_cnvrt();
extern void msg_flag_lkup();
extern void msg_get_handlr();
extern void msg_issue_s();
extern void msg_issue();
extern void msg_issue_d();
extern void msg_issue_sd();
extern void msg_issue_ds();
extern void msg_issue_ss();
extern void msg_issue_dd();
extern void msg_issue_cprt();
extern void msg_save();
extern void msg_set_handlr();
extern void msg_sys_err();


#endif /* FULLPROTO */

#ifdef C_PLUSPLUS
}
#endif

#endif
