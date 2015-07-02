//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "Class.h"

FdoSmLpPostGisClass::FdoSmLpPostGisClass(FdoSmPhClassReaderP classReader,
    FdoSmLpSchemaElement* parent) : FdoSmLpClass(classReader, parent),
    FdoSmLpPostGisClassDefinition(classReader, parent),
    FdoSmLpClassBase(classReader, parent)
{
    // idle
}

FdoSmLpPostGisClass::FdoSmLpPostGisClass(FdoClass* fdoClass, bool ignoreStates,
    FdoSmLpSchemaElement* parent)
    : FdoSmLpClass(fdoClass, ignoreStates, parent),
        FdoSmLpPostGisClassDefinition(fdoClass, ignoreStates, parent),
        FdoSmLpClassBase(fdoClass, ignoreStates, parent)
{
    // idle
}

FdoSmLpPostGisClass::~FdoSmLpPostGisClass()
{
    // idle
}

void FdoSmLpPostGisClass::Update(FdoClassDefinition* fdoClass,
                                 FdoSchemaElementState state,
                                 FdoPhysicalClassMapping* classOverrides,
                                 bool ignoreStates)
{
    // Apply the schema overrides first since these are needed to update properties
    FdoSmLpPostGisClassDefinition::Update(fdoClass, state,
        classOverrides, ignoreStates);

    // Do Generic feature class updates next
    FdoSmLpClass::Update(fdoClass, state, classOverrides, ignoreStates);
}

FdoSmPhClassWriterP FdoSmLpPostGisClass::GetPhysicalAddWriter()
{
    FdoSmPhClassWriterP writer(FdoSmLpClassDefinition::GetPhysicalAddWriter());

    FdoSmLpPostGisClassDefinition::SetPhysicalAddWriter(writer);

    return writer;
}

void FdoSmLpPostGisClass::PostFinalize()
{
    FdoSmLpClass::PostFinalize();
}

