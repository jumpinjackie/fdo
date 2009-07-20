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
#include <Fdo/Commands/CommandException.h>

FdoCommandException* FdoCommandException::Create()
{
	return new FdoCommandException();
}

FdoCommandException* FdoCommandException::Create(FdoString* message)
{
	return new FdoCommandException(message);
}

FdoCommandException* FdoCommandException::Create(FdoString* message, FdoInt64 nativeErrroCode)
{
    return new FdoCommandException(message, NULL, nativeErrroCode);
}

FdoCommandException* FdoCommandException::Create(FdoString* message, FdoException* cause)
{
	return new FdoCommandException(message, cause);
}

FdoCommandException* FdoCommandException::Create(FdoString* message, FdoException* cause, FdoInt64 nativeErrroCode)
{
    return new FdoCommandException(message, cause, nativeErrroCode);
}

FdoCommandException::FdoCommandException() :
  FdoException()
{
}

FdoCommandException::FdoCommandException(FdoString* message) :
  FdoException(message)
{
}

FdoCommandException::FdoCommandException(FdoString* message, FdoException* cause) :
  FdoException(message, cause)
{
}

FdoCommandException::FdoCommandException(FdoString* message, FdoException* cause,  FdoInt64 nativeErrroCode) :
    FdoException(message, cause, nativeErrroCode)
{
}

FdoCommandException::~FdoCommandException()
{
}

void FdoCommandException::Dispose()
{
    delete this;
}


