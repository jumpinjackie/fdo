// 
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
//  
#include "stdafx.h"
#include "ExInfoDb.h"
#include "SdfConnection.h"
#include <FdoCommonSchemaUtil.h>


char* DB_EXINFO_NAME = "EXTENDEDINFO"; //NOXLATE
const REC_NO DB_EXSCHEMAINFO_RECNO = 1;

enum ExSchemaInfoType
{
    ExSchemaInfoType_GeometricTypes = 1
};


//----------------------------------------------------------------------------------
// ExInfoDb database organization
// ==============================
//
// We store any data that doesn't conform to the older SDF file format in the ExInfoDb database.
// This allows for forward-compatibility, since older SDF Providers will ignore this table
// and newer providers will ignore entries in this table they do not understand.
//
// Currently, only Record 1 is used to store extended info; the remaining records are reserved for future use and must be ignored.
//----------------------------------------------------------------------------------

ExInfoDb::ExInfoDb(SQLiteDataBase* env, const char* filename, bool bReadOnly) :
    m_env( env ),
    m_bReadOnly(bReadOnly)
{
    m_db = new SQLiteTable(env);

    int res;

    int readOnlyFlag = bReadOnly ? SQLiteDB_RDONLY : 0;

    //try to open a database that already exists
    if (res = m_db->open(0, filename, DB_EXINFO_NAME, DB_EXINFO_NAME, readOnlyFlag, 0) == 0)
    {
        // ok
    }
    else
    {
        //must close even if open failed
        m_db->close(0);
        delete m_db;
        m_db = NULL;

        if (!bReadOnly)
        {
            m_db = new SQLiteTable(env);

            // Since we failed to open an existing SDF file, create a new one instead:
            if (res = m_db->open(0, filename, DB_EXINFO_NAME, DB_EXINFO_NAME, SQLiteDB_CREATE, 0) != 0)
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
        }
    }
}


ExInfoDb::~ExInfoDb()
{
    if (m_db)
    {
        m_db->close(0);
        delete m_db;
    }
}


void ExInfoDb::ReadExtendedInfo(FdoFeatureSchema* schema)
{
    // retrieve extended schema info record:
    REC_NO rootRecno = DB_EXSCHEMAINFO_RECNO;
    SQLiteData keyExSchemaInfo(&rootRecno, sizeof(REC_NO));
    SQLiteData data;

    //this can fail if there is no extended info in the database, or no schema info in the database:
    if ((schema==NULL) || (m_db==NULL) || (m_db->get(0, &keyExSchemaInfo, &data, 0) != 0) || data.get_size()==0)
    {
        CloseCursor();
        return;
    }

    // Check that we have something in there
    BinaryReader rdr((unsigned char*)data.get_data(), data.get_size());

    // Read each extended schema info element:
    while (rdr.GetPosition() < rdr.GetDataLen())
    {
        ExSchemaInfoType exSchemaInfoType = (ExSchemaInfoType)rdr.ReadInt32();
        FdoInt32 exSchemaInfoLength = rdr.ReadInt32();  // NOTE: length does not include these 2 int32's we just read
        switch (exSchemaInfoType)
        {
            case ExSchemaInfoType_GeometricTypes:
            {
                // Read all the data for this :
                FdoStringP schemaName = rdr.ReadString();
                FdoStringP className = rdr.ReadString();
                FdoStringP propName = rdr.ReadString();
                FdoInt32 numGeomTypes = rdr.ReadInt32();
                FdoGeometryType* geomTypes = (FdoGeometryType*)alloca(sizeof(FdoGeometryType) * numGeomTypes);
                FdoInt32 numGeomTypesUnderstood = 0;
                for (int i=0; i<numGeomTypes; i++)
                {
                    FdoGeometryType geomType = (FdoGeometryType)rdr.ReadInt32();
                    // for forward compatibility, ignore anything we dont understand:
                    if (geomType >= 0 && geomType <= FdoGeometryType_MultiCurvePolygon)
                        geomTypes[numGeomTypesUnderstood++] = geomType;
                }

                // Find the property in the logical schema;
                // NOTE we only have one schema right now but it might change in the future, so check that the name matches:
                if (0==wcscmp(schema->GetName(), schemaName))
                {
                    FdoPtr<FdoClassCollection> classDefs = schema->GetClasses();
                    FdoPtr<FdoClassDefinition> classDef = classDefs->GetItem(className);
                    FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
                    FdoPtr<FdoPropertyDefinition> prop = props->FindItem(propName);
                    // NOTE: dont bother looking in BaseProperties since we dont write the BaseProperties attributes of a class
                    // (that's the job of the parent class(es))

                    // Apply the specific geometry types to the select geometry property:
                    if (NULL != prop)
                    {
                        FdoGeometricPropertyDefinition* geomProp = static_cast<FdoGeometricPropertyDefinition*>(prop.p);
                        geomProp->SetSpecificGeometryTypes(geomTypes, numGeomTypesUnderstood);
                    }
                }
            }
            break;

            default:
                // we purposefully ignore (and skip over) any types we dont understand without throwing any exceptions.
                // This allows for forward-compatibility in the future.
                rdr.SetPosition(rdr.GetPosition() + exSchemaInfoLength);
            break;
        }
    }

    schema->AcceptChanges();

	// The following closes any open cursors so inserting features would work. 
	CloseCursor();
}


void ExInfoDb::CloseCursor()
{
    if (m_db)
    	m_db->close_cursor();
}


void ExInfoDb::WriteExtendedInfo(FdoFeatureSchema* schema)
{
    WriteExtendedSchemaInfo(schema);
}

void ExInfoDb::WriteExtendedSchemaInfo(FdoFeatureSchema* schema)
{
    //this can fail if there is no extended info in the database:
    if (m_db==NULL)
    {
        CloseCursor();
        return;
    }

    BinaryWriter wrt(256);
    BinaryWriter wrtTemp(256);

    // Write each extended schema info element:
    FdoPtr<FdoClassCollection> classDefs = schema->GetClasses();
    for (int c=0; c<classDefs->GetCount(); c++)
    {
        FdoPtr<FdoClassDefinition> classDef = classDefs->GetItem(c);
        FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();  // NOTE: dont bother writing BaseProperties, they will be written by the base class

        for (int p=0; p<props->GetCount(); p++)
        {
            FdoPtr<FdoPropertyDefinition> prop = props->GetItem(p);
            if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
            {
                // Write this geometry propertie's SpecificGeometryypes:
                FdoGeometricPropertyDefinition* geomProp = static_cast<FdoGeometricPropertyDefinition*>(prop.p);
                FdoInt32 numGeomValues = 0;
                FdoGeometryType* geomValues = geomProp->GetSpecificGeometryTypes(numGeomValues);

                wrtTemp.Reset();
                wrtTemp.WriteString(schema->GetName());
                wrtTemp.WriteString(classDef->GetName());
                wrtTemp.WriteString(geomProp->GetName());
                wrtTemp.WriteInt32(numGeomValues);
                for (int i=0; i<numGeomValues; i++)
                    wrtTemp.WriteInt32(geomValues[i]);

                wrt.WriteInt32(ExSchemaInfoType_GeometricTypes);
                wrt.WriteInt32(wrtTemp.GetDataLen());  // NOTE: length does not include these 2 int32's we just wrote
                wrt.WriteBytes(wrtTemp.GetData(), wrtTemp.GetDataLen());
            }
        }
    }

    // save extended schema info:
    REC_NO rootRecno = DB_EXSCHEMAINFO_RECNO;
    SQLiteData keyExSchemaInfo(&rootRecno, sizeof(REC_NO));
    SQLiteData dataExSchemaInfo(wrt.GetData(), wrt.GetDataLen());
    if (m_db->put(0, &keyExSchemaInfo, &dataExSchemaInfo, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_94_EXINFO_STORAGE_ERROR)));

	// The following closes any open cursors so inserting features would work. 
	CloseCursor();
}
