//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//

#ifdef FDOPOSTGIS_MESSAGE_DEFINE

// Default message catalog filename
#ifdef _WIN32
char const* PostGisMsgCat = "PostGisMessage.dll";
#else
char const* PostGisMsgCat = "PostGisMessage.cat";
#endif // _WIN32

FdoString* NlsMsgGet(int msgNum, char* defaultMsg, ...)
{
    va_list varargs;
    va_start(varargs, defaultMsg);
    FdoString* ret = FdoException::NLSGetMessage(msgNum, defaultMsg,
                     const_cast<char*>(PostGisMsgCat), varargs);
    va_end(varargs);
    return ret;
}

#else // FDOPOSTGIS_MESSAGE_DEFINE

extern char const* PostGisMsgCat;
extern FdoString* NlsMsgGet(int msgNum, char* defaultMsg, ...);

#endif // FDOPOSTGIS_MESSAGE_DEFINE

