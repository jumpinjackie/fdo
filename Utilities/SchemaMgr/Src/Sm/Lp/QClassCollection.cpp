 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "stdafx.h"
#include <Sm/Lp/QClassCollection.h>

FdoSmLpQClassDefinition::FdoSmLpQClassDefinition( const FdoSmLpClassDefinition* classDef ) :
    mClassDef(classDef)
{
}

FdoSmLpQClassDefinition::~FdoSmLpQClassDefinition()
{
}

FdoString* FdoSmLpQClassDefinition::GetName() const
{
    if ( mQName == L"" )
        ((FdoSmLpQClassDefinition*) this)->mQName = mClassDef->GetQName();

    return mQName;
}

const FdoSmLpClassDefinition* FdoSmLpQClassDefinition::RefClassDefinition() const
{
    return (FdoSmLpClassDefinition*) mClassDef;
}

const FdoSmLpClassDefinition* FdoSmLpQClassCollection::RefClassDefinition(FdoInt32 item) const
{
    FdoSmLpQClassDefinitionP qClassDef = ((FdoSmLpQClassCollection*) this)->GetItem(item);

    return qClassDef->RefClassDefinition();
}

const FdoSmLpClassDefinition* FdoSmLpQClassCollection::RefClassDefinition(FdoString* name) const
{
    const FdoSmLpClassDefinition* classDef = NULL;
    FdoSmLpQClassDefinitionP qClassDef = ((FdoSmLpQClassCollection*) this)->FindItem(name);

    if ( qClassDef ) 
        classDef = qClassDef->RefClassDefinition();

    return classDef;
}

FdoInt32 FdoSmLpQClassCollection::AddClassDefinition( FdoSmLpClassDefinition* value)
{
    FdoSmLpQClassDefinitionP qClassDef = new FdoSmLpQClassDefinition( value );
    return Add(qClassDef);
}

bool FdoSmLpQClassCollection::ContainsClassDefinition( const FdoSmLpClassDefinition* value)
{
    FdoSmLpQClassDefinitionP qClassDef = FindItem( value->GetQName() );

    return (qClassDef != NULL);
}
