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
#include <Fdo/DestroyClass.h>

// Constructs a default instance of a DestroyClass command.
FdoDestroyClass::FdoDestroyClass()
{
}

// Constructs an instance of a DestroyClass command using the 
// specified arguments.
FdoDestroyClass::FdoDestroyClass(FdoIConnection* connection)
{
}

// Default destructor for DestroyClass command.
FdoDestroyClass::~FdoDestroyClass()
{
}

// Gets the name of the class to destroy as an Identifier. 
// If the feature provider supports multiple schemas the name
// of the schema must be included in the identifier in the form 
// "<SchemaName>.<ClassName>".
FdoIdentifier* FdoDestroyClass::GetClassName()
{
    return NULL;
}

// Sets the name of the class to destroy as an Identifier. If
// the feature provider supports multiple schemas the name of
// the schema must be included in the identifier in the form 
// "<SchemaName>.<ClassName>".
void FdoDestroyClass::SetClassName(FdoIdentifier* value)
{
}

// Sets the name of the class to destroy as a String. If the 
// feature provider supports multiple schemas the name of the
// schema must be included in the string in the form 
// "<SchemaName>.<ClassName>".
void FdoDestroyClass::SetClassName(const wchar_t* value)
{
}

// Executes the destroy class command removing the class definition
// and all instance data from the data store. If other schema 
// elements refer to the class to be destroyed then an exception 
// will be thrown.
void FdoDestroyClass::Execute()
{
}

