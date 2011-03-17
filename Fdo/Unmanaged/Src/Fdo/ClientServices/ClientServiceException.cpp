/***************************************************************************
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
 ***************************************************************************/

#include <Fdo/ClientServices/ClientServiceException.h>

FdoClientServiceException* FdoClientServiceException::Create()
{
	return new FdoClientServiceException();
}

FdoClientServiceException* FdoClientServiceException::Create(FdoString* message)
{
	return new FdoClientServiceException(message);
}

FdoClientServiceException* FdoClientServiceException::Create(FdoString* message, FdoInt64 nativeErrorCode)
{
    return new FdoClientServiceException(message, nativeErrorCode);
}

FdoClientServiceException::FdoClientServiceException() :
    FdoException ()
{
}

FdoClientServiceException::FdoClientServiceException(FdoString* message) :
    FdoException (message)
{
}

FdoClientServiceException::FdoClientServiceException(FdoString* message, FdoInt64 nativeErrorCode) :
FdoException (message, NULL, nativeErrorCode)
{
}

FdoClientServiceException::~FdoClientServiceException()
{
}

void FdoClientServiceException::Dispose()
{
    delete this;
}

FdoString* FdoClientServiceException::ToString()
{
    return m_message;
}

