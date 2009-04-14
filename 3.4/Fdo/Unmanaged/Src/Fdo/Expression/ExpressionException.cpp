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
#include <Fdo/Expression/ExpressionException.h>

// Constructs a default instance of a ExpressionException.
FdoExpressionException* FdoExpressionException::Create()
{
	return new FdoExpressionException();
}

// Constructs an instance of a ExpressionException using the specified
// arguments.
FdoExpressionException* FdoExpressionException::Create(FdoString* message)
{
	return new FdoExpressionException(message);
}

// Constructs an instance of a ExpressionException using the specified
// arguments.
FdoExpressionException* FdoExpressionException::Create(FdoString* message, FdoException* cause)
{
	return new FdoExpressionException(message, cause);
}

// Constructs a default instance of a ExpressionException.
FdoExpressionException::FdoExpressionException() :
  FdoException()
{
}

// Constructs an instance of a ExpressionException using the specified
// arguments.
FdoExpressionException::FdoExpressionException(FdoString* message) :
  FdoException(message)
{
}

// Constructs an instance of a ExpressionException using the specified
// arguments.
FdoExpressionException::FdoExpressionException(FdoString* message, FdoException* cause) :
  FdoException(message, cause)
{
}

FdoExpressionException::~FdoExpressionException()
{
}

void FdoExpressionException::Dispose()
{
    delete this;
}


