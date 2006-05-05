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
#include <Sm/Ph/Mt/PropertyReader.h>

FdoSmPhMtPropertyReader::FdoSmPhMtPropertyReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr ) : 
    FdoSmPhReader( MakeReader(froms, schemaName, mgr) )
{
}

FdoSmPhMtPropertyReader::~FdoSmPhMtPropertyReader(void)
{
}

FdoSmPhReaderP FdoSmPhMtPropertyReader::MakeReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr )
{
    FdoSmPhRowP from = froms->GetItem(0);
    FdoSmPhOwner* owner = static_cast<FdoSmPhOwner*>((FdoSmSchemaElement*)(from->GetDbObject()->GetParent()));
    double schemaVersion = owner->GetSchemaVersion();
    FdoStringP classidRow;

    if ( schemaVersion > 0.0 && schemaVersion < 3.0 ) {
        // This block will only be executed for the Oracle Provider, since MetaSchema
        // versions 1 and 2 were Oracle Provider only.

        // Version 3 added a property for ClassId and the Rdbms providers will not 
        // find classid columns if this property is not present. In Versions 1 and 2
        // of the Oracle Provider, classid column name was hard-coded; there was
        // no corresponding property in the MetaSchema.

        // The following adds a fake f_attributedefinition row for classid to make
        // a pre-version 3 MetaSchema look more like version 3. This allows more recent
        // versions of the Oracle Provider to work with these older schemas.

        // The fields for the fake row must be in the same order as the fields setup
        // by FdoSmPhPropertyReader::MakeRows().
        classidRow = FdoStringP::Format(
            L" union select classid, tablename, 'ClassId', 0, 'CLASSID', null, 'int64', 'number', 20, 0, 0, 0, 0, 1, '%ls', null, null, 1, 1, 0, 0, 0, 0, null, 'Feature' from %ls.f_classdefinition where schemaname = 'F_MetaClass' and classname = 'Feature' ",
            owner->GetName(),
            owner->GetName()
        );
    }

    // Generate the where clause
    FdoStringP where = FdoStringP::Format( 
        L"where f_attributedefinition.classid = f_classdefinition.classid and schemaname = %ls %ls order by classname, attributename", 
    	(FdoString*) mgr->FormatSQLVal(schemaName, FdoSmPhColType_String),
        (FdoString*) classidRow
    );

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP pSubReader = mgr->CreateQueryReader( froms, where );

    return FDO_SAFE_ADDREF( (FdoSmPhRdQueryReader*) pSubReader );
}

