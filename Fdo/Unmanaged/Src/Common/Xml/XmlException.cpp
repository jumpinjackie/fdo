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
#include <FdoStd.h>
#include <stdarg.h>

FdoXmlException* FdoXmlException::Create()
{
    return new FdoXmlException();
}

FdoXmlException* FdoXmlException::Create(FdoString* message)
{
    return new FdoXmlException(message);
}

FdoXmlException* FdoXmlException::Create(FdoString* message, FdoInt64 nativeErrorCode)
{
    return new FdoXmlException(message, NULL, nativeErrorCode);
}

FdoXmlException* FdoXmlException::Create(FdoString* message, FdoException* cause)
{
    return new FdoXmlException(message, cause);
}

FdoXmlException* FdoXmlException::Create(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode)
{
    return new FdoXmlException(message, cause, nativeErrorCode);
}

FdoXmlException::FdoXmlException()
{
}

FdoXmlException::FdoXmlException(FdoString* message) : FdoException(message)
{
}

FdoXmlException::FdoXmlException(FdoString* message, FdoException* cause) : FdoException(message, cause)
{
}

FdoXmlException::FdoXmlException(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode) : FdoException(message, cause, nativeErrorCode)
{
}

void FdoXmlException::Dispose()
{
    delete this;
}


