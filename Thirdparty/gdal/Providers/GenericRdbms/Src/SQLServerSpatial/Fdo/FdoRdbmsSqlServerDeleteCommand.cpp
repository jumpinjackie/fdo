/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
 */

#include "stdafx.h"
#include "FdoRdbmsSqlServerConnection.h"
#include "FdoRdbmsSqlServerDeleteCommand.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include "DbiConnection.h"

FdoRdbmsSqlServerDeleteCommand::FdoRdbmsSqlServerDeleteCommand ()
{
}

FdoRdbmsSqlServerDeleteCommand::FdoRdbmsSqlServerDeleteCommand (FdoIConnection *connection):
FdoRdbmsDeleteCommand( connection )
{
}

FdoRdbmsSqlServerDeleteCommand::~FdoRdbmsSqlServerDeleteCommand()
{
}

void FdoRdbmsSqlServerDeleteCommand::AddLtProperty( const wchar_t *scope, const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection *propertyValues, FdoDataValue *dataValue )
{
	FdoStringP name(scope);
	if( scope[0] != '\0' )
		name += L".";
	FdoPtr<FdoPropertyValue> propertyValue = FdoPropertyValue::Create( (const wchar_t*)(name + L"LtId"), dataValue);
	propertyValues->Add( propertyValue );

	const FdoSmLpPropertyDefinitionCollection *props = classDefinition->RefProperties();
	for( int i=0; i<props->GetCount(); i++ )
	{
		const FdoSmLpPropertyDefinition *propertyDefinition = props->RefItem( i );
		if( propertyDefinition->GetPropertyType() == FdoPropertyType_ObjectProperty )
		{
			const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
			if( objProp->GetObjectType() == FdoObjectType_Value )
				AddLtProperty( (const wchar_t*)(name+objProp->GetName()), objProp->RefTargetClass(), propertyValues, dataValue );
		}
	}
}

FdoInt32 FdoRdbmsSqlServerDeleteCommand::Execute ()
{
    return FdoRdbmsDeleteCommand::Execute();
}

