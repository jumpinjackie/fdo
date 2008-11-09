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
//


#include "Fdo/PhysicalSchemaMappingCasts.h"

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalSchemaMappingToFdoXmlSchemaMapping
// Description:   Casts an FdoPhysicalSchemaMapping to an  
//                FdoXmlSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalSchemaMapping Object
//
//
// Returns:       A FdoXmlSchemaMapping
// ------------------------------------------------------------------------
FdoXmlSchemaMapping* FdoPhysicalSchemaMappingToFdoXmlSchemaMapping( FdoPhysicalSchemaMapping* mapping ) {
	FdoXmlSchemaMapping* xmlMapping = static_cast< FdoXmlSchemaMapping*>(mapping);
	return xmlMapping;
}
