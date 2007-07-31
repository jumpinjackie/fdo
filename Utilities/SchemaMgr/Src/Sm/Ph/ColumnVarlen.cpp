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
#include <Sm/Ph/ColumnVarlen.h>
#include <Sm/Error.h>

FdoSmPhColumnVarlen::FdoSmPhColumnVarlen(
    int length
) :
	mLength(length)
{

	if ( mLength < 0 ) 
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_207), 
				mLength, 
				(FdoString*) GetTypeName()
            )
		);

}

int FdoSmPhColumnVarlen::GetMinLength() const
{
    return 1;
}

bool FdoSmPhColumnVarlen::DefinitionEquals( FdoSmPhColumnP otherColumn )
{
    bool equals = FdoSmPhColumn::DefinitionEquals( otherColumn );

    if ( equals ) {
        equals = false;

        FdoSmPhColumnVarlenP otherVarColumn = otherColumn->SmartCast<FdoSmPhColumnVarlen>();

        if ( otherVarColumn && (GetLength() == otherVarColumn->GetLength()) ) 
            equals = true;
    }

    return equals;
}

