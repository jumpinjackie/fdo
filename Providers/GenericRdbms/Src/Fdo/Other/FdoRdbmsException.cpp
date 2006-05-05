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
 */

#include "stdafx.h"

#include "FdoRdbmsException.h"


FdoRdbmsException* FdoRdbmsException::Create()
{
    return new FdoRdbmsException();
}

FdoRdbmsException* FdoRdbmsException::Create(const wchar_t* message)
{
    return new FdoRdbmsException(message);
}

FdoRdbmsException* FdoRdbmsException::Create(const wchar_t* message, FdoException* cause)
{
    return new FdoRdbmsException(message, cause);
}


FdoRdbmsException::FdoRdbmsException() :
  FdoException()
{
}

FdoRdbmsException::FdoRdbmsException(const wchar_t* message) :
  FdoException(message)
{
}

FdoRdbmsException::FdoRdbmsException(const wchar_t* message, FdoException* cause) :
  FdoException(message, cause)
{
}

FdoRdbmsException::~FdoRdbmsException()
{
}

void FdoRdbmsException::Dispose()
{
    delete this;
}
