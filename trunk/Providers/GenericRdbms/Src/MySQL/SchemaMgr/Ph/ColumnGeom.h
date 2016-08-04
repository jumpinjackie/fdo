#ifndef FDOSMPHMYSQLCOLUMNGEOM_H
#define FDOSMPHMYSQLCOLUMNGEOM_H      1
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

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnGeom.h>
#include "Rd/ColumnReader.h"
#include "Column.h"
#include "ColTypeMapper.h"

// Represents a MySql geometric type column.
class FdoSmPhMySqlColumnGeom :
    public FdoSmPhColumnGeom, public FdoSmPhMySqlColumn
{
public:
    FdoSmPhMySqlColumnGeom(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        FdoSmPhScInfoP AssociatedSCInfo,
        bool bNullable = true,
        bool bHasElevation = false,
        bool bHasMeasure = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhMySqlColumn (reader),
        FdoSmPhColumn    ( columnName, L"geometry", elementState, parentObject, bNullable, rootColumnName),
        FdoSmPhColumnGeom( AssociatedSCInfo, bHasElevation, bHasMeasure )
    {
        m_FdoGeomType = FdoSmPhMySqlColTypeMapper::GetColFdoGeometricType(L"geometry");
        m_FdoGeometryType = FdoSmPhMySqlColTypeMapper::GetColFdoGeometryType(L"geometry");
        mSRID = -1;

        if (NULL != reader)
        {
            try
            {
                FdoSmPhRdMySqlColumnReader* pReader = dynamic_cast<FdoSmPhRdMySqlColumnReader*>(reader);
                if (pReader != NULL)
                {
                    m_FdoGeomType = pReader->GetFdoGeometricType();
                    m_FdoGeometryType = pReader->GetFdoGeometryType();
                }
            }
            catch ( FdoException* e ){e->Release();}
            catch ( ... ){}
        }
    }

    virtual ~FdoSmPhMySqlColumnGeom(void) {}

    virtual int GetRdbType()
    {
        return RDBI_GEOMETRY;
    }

    virtual FdoStringP GetBestFdoType()
    {
        return FdoStringP::Format(L"%d", m_FdoGeomType);
    }

    virtual FdoStringP GetBestFdoGeometryType()
    {
        return FdoStringP::Format(L"%d", m_FdoGeometryType);
    }
	virtual FdoInt64 GetSRID();

    virtual FdoInt64 GetDbBinarySize()
    {
        return 12;
    }

protected:
    long m_FdoGeomType;
    long m_FdoGeometryType;
	FdoInt64 mSRID;

    virtual void PostFinalize()
    {
        SetHasElevation(false);
        SetHasMeasure(false);
    }
};

typedef FdoPtr<FdoSmPhMySqlColumnGeom> FdoSmPhMySqlColumnGeomP;

#endif
