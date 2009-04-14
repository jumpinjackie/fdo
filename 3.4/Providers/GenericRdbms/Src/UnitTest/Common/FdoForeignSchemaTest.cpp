
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
 *
 */ 

#include "Pch.h"
#include "FdoForeignSchemaTest.h"
#include "UnitTestUtil.h"


FdoForeignSchemaTest::FdoForeignSchemaTest(void)
{
    mSuffix[0] = '\0';
}

FdoForeignSchemaTest::FdoForeignSchemaTest(char *suffix)
{
    strncpy(mSuffix, suffix, 11 );
    mSuffix[11] = '\0';
}

void FdoForeignSchemaTest::setUp ()
{
    set_provider();
	// always create new foreign database
	create_foreign_datastore();
}

FdoForeignSchemaTest::~FdoForeignSchemaTest(void)
{
}

FdoPropertyValue* FdoForeignSchemaTest::AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = NULL;

    propertyValue = propertyValues->FindItem( name );
    if ( !propertyValue ) 
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}



		  

