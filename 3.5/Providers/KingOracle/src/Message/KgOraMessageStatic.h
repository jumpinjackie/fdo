/*
* Copyright (C) 2009  SL-King d.o.o
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
*/

#ifdef KGORA_MESSAGE_DEFINE

#ifdef _WIN32
char *g_KgOraMsgCat = "KingOracleMessage.dll";
#else
char *g_KgOraMsgCat = "KingOracleMessage.cat";
#endif
FdoString* NlsMsgGet(int msg_num, char* default_msg, ...)
{
  va_list varargs;
  va_start(varargs, default_msg);
#ifdef _FDO_3_1
  FdoString* ret = GisException::NLSGetMessage(msg_num, default_msg, g_KgOraMsgCat, varargs);
#else
  FdoString* ret = FdoException::NLSGetMessage(msg_num, default_msg, g_KgOraMsgCat, varargs);
#endif
  va_end(varargs);

  return ret;
}
#else // KGORA_MESSAGE_DEFINE
extern char *g_KgOraMsgCat;
extern FdoString* NlsMsgGet(int msg_num, char* default_msg, ...);
#endif // KGORA_MESSAGE_DEFINE
