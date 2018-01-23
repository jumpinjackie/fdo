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

#include "stdafx.h"
#include <Sm/Ph/Rb/Column.h>

FdoSmPhRbColumn::FdoSmPhRbColumn(
		FdoString* name, 
		FdoSmSchemaElement* parent,
		FdoSchemaElementState elementState
): 
	FdoSmSchemaElement( name, 
							 L"", 
						     parent 
						    )
{
	SetElementState(elementState);
}

FdoSmPhRbColumn::~FdoSmPhRbColumn(void)
{
}

void FdoSmPhRbColumn::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<column name=\"%s\" >\n",
			(const char*) FdoStringP(GetName())
	);

	FdoSmSchemaElement::XMLSerialize( xmlFp, ref );

	fprintf( xmlFp, "</column>\n" );
}

