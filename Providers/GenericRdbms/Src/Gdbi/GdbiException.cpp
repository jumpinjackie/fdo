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
#include "stdafx.h"
#include "GdbiException.h"
#ifdef USE_NONRDBMS_HEADER
#include <inc/ut.h>
#else
#include <Inc/ut.h>
#endif

wchar_t*  Gdbi_nls_msg_get_0( int msg_num, char* default_msg, char* catalog )
{
	return (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, catalog );
}

wchar_t*  Gdbi_nls_msg_get_S( int msg_num, char* default_msg, char* catalog, char* arg1 )
{
	wchar_t *warg1 = new wchar_t[ strlen( arg1) + 1 ];
	FdoStringP::Utf8ToUnicode( arg1, warg1, (int)strlen( arg1) + 1 );
	wchar_t* ret = (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, catalog, warg1 );
	delete[] warg1;

	return ret;
}

GdbiException* GdbiException::Create()
{
    return new GdbiException();
}

GdbiException* GdbiException::Create(const wchar_t* message)
{
    return new GdbiException(message);
}

GdbiException* GdbiException::Create(const char* message)
{
    GdbiException   *exp;
    exp =  new GdbiException(FdoStringP(message));

    return exp;
}

GdbiException* GdbiException::Create(const wchar_t* message, FdoException* cause)
{
    return new GdbiException(message, cause);
}


GdbiException::GdbiException() :
  FdoException()
{
}

GdbiException::GdbiException(const wchar_t* message) :
  FdoException(message)
{
}

GdbiException::GdbiException(const wchar_t* message, FdoException* cause) :
  FdoException(message, cause)
{
}

GdbiException::~GdbiException()
{
}

void GdbiException::Dispose()
{
    delete this;
}
