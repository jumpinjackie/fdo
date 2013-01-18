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
#include <Fdo/Schema/AutogenerationException.h>

FdoAutogenerationException* FdoAutogenerationException::Create()
{
	return new FdoAutogenerationException();
}

FdoAutogenerationException* FdoAutogenerationException::Create(FdoString* message)
{
	return new FdoAutogenerationException(message);
}

FdoAutogenerationException* FdoAutogenerationException::Create(FdoString* message, FdoInt64 nativeErrorCode)
{
    return new FdoAutogenerationException(message, NULL, nativeErrorCode);
}

FdoAutogenerationException* FdoAutogenerationException::Create(FdoString* message, FdoException* cause)
{
	return new FdoAutogenerationException(message, cause);
}

FdoAutogenerationException* FdoAutogenerationException::Create(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode)
{
    return new FdoAutogenerationException(message, cause, nativeErrorCode);
}

FdoAutogenerationException::FdoAutogenerationException() :
  FdoSchemaException()
{
}

FdoAutogenerationException::FdoAutogenerationException(FdoString* message) :
  FdoSchemaException(message)
{
}

FdoAutogenerationException::FdoAutogenerationException(FdoString* message, FdoException* cause) :
  FdoSchemaException(message, cause)
{
}

FdoAutogenerationException::FdoAutogenerationException(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode) :
  FdoSchemaException(message, cause, nativeErrorCode)
{
}

FdoAutogenerationException::~FdoAutogenerationException()
{
}

void FdoAutogenerationException::Dispose()
{
    delete this;
}

