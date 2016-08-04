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
#include "IndexPropertyReader.h"
#include "PhysicalSchema.h"

FdoRdbmsPhClassIndexReader::FdoRdbmsPhClassIndexReader( 
	FdoStringP schemaName, 
	FdoStringP className, 
	FdoRdbmsPhIndexReader* pIndexReader
) :
	FdoRdbmsPhGroupReader( className, pIndexReader),
	mSchemaName(schemaName),
	mClassName(className),
	mpIndexReader(pIndexReader)
{
}

FdoRdbmsPhClassIndexReader::~FdoRdbmsPhClassIndexReader(void)
{
}

FdoStringP FdoRdbmsPhClassIndexReader::GetGroupName()
{
	return( mpIndexReader->GetFeatureClassName() );
}

FdoStringP FdoRdbmsPhClassIndexReader::GetIndexName()
{
	// Make sure there is a current row.
	CheckGet();

	return(mpIndexReader->GetIndexName());
}

bool FdoRdbmsPhClassIndexReader::GetIsUnique()
{
	// Make sure there is a current row.
	CheckGet();

	return(mpIndexReader->GetIsUnique());
}

FdoStringP FdoRdbmsPhClassIndexReader::GetFeatureClassName()
{
	// Make sure there is a current row.
	CheckGet();

	return(mpIndexReader->GetFeatureClassName());
}

FdoStringP FdoRdbmsPhClassIndexReader::GetAttributeName()
{
	// Make sure there is a current row.
	CheckGet();

	return(mpIndexReader->GetAttributeName());
}

int FdoRdbmsPhClassIndexReader::GetAttributePosition()
{
	// Make sure there is a current row.
	CheckGet();

	return(mpIndexReader->GetAttributePosition());
}

FdoRdbmsPhIndexPropertyReader* FdoRdbmsPhClassIndexReader::GetPropertyReader()
{
    // Need to prime this reader if no read has yet been done.
    if ( IsBOF() )
        ReadNext();

    FdoRdbmsPhIndexPropertyReader* pReader = 
        new FdoRdbmsPhIndexPropertyReader( 
            mSchemaName, 
            mClassName, 
            IsEOF() ? L"" : GetIndexName(), // Set property reader to read nothing
                                            // when this reader is at EOF.
            this 
        );

    return pReader;
}
