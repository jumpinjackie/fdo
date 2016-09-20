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
#include <stdlib.h>
#include <Fdo/Connections/ConnectionException.h>

FdoConnectionException* FdoConnectionException::Create()
{
	return new FdoConnectionException();
}

FdoConnectionException* FdoConnectionException::Create(FdoString* message)
{
	return new FdoConnectionException(message);
}

FdoConnectionException* FdoConnectionException::Create(FdoString* message, FdoInt64 nativeErrorCode)
{
    return new FdoConnectionException(message, NULL, nativeErrorCode);
}

FdoConnectionException* FdoConnectionException::Create(FdoString* message, FdoException* cause)
{
	return new FdoConnectionException(message, cause);
}

FdoConnectionException* FdoConnectionException::Create(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode)
{
    return new FdoConnectionException(message, cause, nativeErrorCode);
}

FdoConnectionException::FdoConnectionException() :
  FdoException()
{
}

FdoConnectionException::FdoConnectionException(FdoString* message) :
  FdoException(message)
{
}

FdoConnectionException::FdoConnectionException(FdoString* message, FdoException* cause) :
  FdoException(message, cause)
{
}

FdoConnectionException::FdoConnectionException(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode) :
  FdoException(message, cause, nativeErrorCode)
{
}

FdoConnectionException::~FdoConnectionException()
{
}

void FdoConnectionException::Dispose()
{
    delete this;
}

