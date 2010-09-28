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

//
// This header file is shared by multiple projects including GRFPProvider and GRFPOverrides
//

#ifndef FDOGRFPDEFINES_H
#define FDOGRFPDEFINES_H

#ifdef _WIN32
#pragma once
#endif

// the default message catalog filename
extern char *fdorfp_cat;   

// Convenient NLS macros
inline FdoString* RFPNLSGetMessage(FdoInt32 msgNum, const char* defMsg, const char* file, int line, const char* catalog, ...)
{
    va_list arguments;

    va_start(arguments, catalog);
    FdoString*  result = FdoException::NLSGetMessage(msgNum, defMsg, file, line, catalog, arguments);
    va_end(arguments);
    return result;
}
inline FdoString* RFPNLSGetMessage(FdoInt32 msgNum, const char* defMsg, const char* catalog, ...)
{
    va_list arguments;

    va_start(arguments, catalog);
    FdoString*  result = FdoException::NLSGetMessage(msgNum, defMsg, catalog, arguments);
    va_end(arguments);
    return result;
}
#ifdef  _DEBUG
    #define NlsMsgGet(msg_num,default_msg)  RFPNLSGetMessage(msg_num, default_msg, __FILE__, __LINE__, fdorfp_cat, NULL)
    #define NlsMsgGet1(msg_num,default_msg,arg1) RFPNLSGetMessage(msg_num, default_msg, __FILE__, __LINE__, fdorfp_cat, arg1)
    #define NlsMsgGet2(msg_num,default_msg,arg1,arg2) RFPNLSGetMessage(msg_num, default_msg, __FILE__, __LINE__, fdorfp_cat, arg1, arg2)
    #define NlsMsgGet3(msg_num,default_msg,arg1,arg2,arg3) RFPNLSGetMessage(msg_num, default_msg, __FILE__, __LINE__, fdorfp_cat, arg1, arg2, arg3)
    #define NlsMsgGet4(msg_num,default_msg,arg1,arg2,arg3,arg4) RFPNLSGetMessage(msg_num, default_msg, __FILE__, __LINE__, fdorfp_cat, arg1, arg2, arg3, arg4)
    #define NlsMsgGet5(msg_num,default_msg,arg1,arg2,arg3,arg4,arg5) RFPNLSGetMessage(msg_num, default_msg, __FILE__, __LINE__, fdorfp_cat, arg1, arg2, arg3, arg4, arg5)
#else
    #define NlsMsgGet(msg_num,default_msg)  RFPNLSGetMessage(msg_num, default_msg, fdorfp_cat, NULL)
    #define NlsMsgGet1(msg_num,default_msg,arg1) RFPNLSGetMessage(msg_num, default_msg, fdorfp_cat, arg1)
    #define NlsMsgGet2(msg_num,default_msg,arg1,arg2) RFPNLSGetMessage(msg_num, default_msg, fdorfp_cat, arg1, arg2)
    #define NlsMsgGet3(msg_num,default_msg,arg1,arg2,arg3) RFPNLSGetMessage(msg_num, default_msg, fdorfp_cat, arg1, arg2, arg3)
    #define NlsMsgGet4(msg_num,default_msg,arg1,arg2,arg3,arg4) RFPNLSGetMessage(msg_num, default_msg, fdorfp_cat, arg1, arg2, arg3, arg4)
    #define NlsMsgGet5(msg_num,default_msg,arg1,arg2,arg3,arg4,arg5) RFPNLSGetMessage(msg_num, default_msg, fdorfp_cat, arg1, arg2, arg3, arg4, arg5)
#endif


// case insensitive string comparison
#ifdef _WIN32
#define STREQUAL(s1, s2)	(_wcsicmp(s1, s2) == 0)
#else
#define STREQUAL(s1, s2)	(wcscasecmp(s1, s2) == 0)
#endif
// case sensitive string comparison
#define STRCASEEQ(s1, s2)	(wcscmp(s1, s2) == 0)

#define FILE_PATH_DELIMITER L"/"

#define SP_STATIC_CAST(T, sp)  FDO_SAFE_ADDREF(static_cast<T*>(sp.p))
#define SP_DYNAMIC_CAST(T, sp) FDO_SAFE_ADDREF(dynamic_cast<T*>(sp.p))

enum PropertyType
{
	PropertyType_ID = 0,
	PropertyType_Raster,
};

// there are three kinds of raster currently supported by RFP :
// SFSB - single file single band(frame)
// SFMB - single file multi bands(frames)
// MFMB - multi files multi bands
// MFSB - not supported by RFP currently
enum FdoRasterType { RasterType_SFSB, RasterType_SFMB, RasterType_MFMB, RasterType_MFSB } ;

#endif // FDORFPDEFINES_H
