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
#ifndef SDFDATAVALIDATOR_H
#define SDFDATAVALIDATOR_H
#include "Fdo.h"

class SdfConnection;

typedef enum SdfDataValidationType
{
	SdfDataValidationType_None		      = 0x00,
    SdfDataValidationType_Association     = 0x01,
    SdfDataValidationType_Constraint      = 0x02
};

// This class implements insert and update common functionality for data validation.
// It primarly scan the user property value collection and make sure the data is as expected.
class SdfDataValidator
{
public:

	//
	// This method computes a validation flag. This flag indicates the various types of validation that
	// is required by a given class.
    static int ValidationFlag( FdoClassDefinition* fc );

	//
	// This method computes a validation flag. This flag indicates the various types of validation that
	// is required by a given property.
    static int ValidationFlag( FdoPropertyDefinition* prop );


	//
	// This method scans the property value collection and validate its content. Exception will be thrown
	// if the data contain invalid values.
    static void Validate( SdfConnection* connection, FdoClassDefinition* fc, FdoPropertyValueCollection* pvc, int vFlag, bool forUpdate );

private:

	//
	// This method validate the association property.
    static void ValidateAssociationProperty( SdfConnection* connection, FdoAssociationPropertyDefinition* aprop, FdoPropertyValueCollection* pvc, bool forUpdate );

	//
	// This method validate the data property.
    static void ValidateDataProperty( SdfConnection* connection, FdoDataPropertyDefinition* dprop, FdoPropertyValueCollection* pvc, bool forUpdate );
};

#endif

