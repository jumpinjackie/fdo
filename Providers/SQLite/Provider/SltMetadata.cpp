// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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
#include "SltMetadata.h"
#include "SltConversionUtils.h"
#include "SltProvider.h"

using namespace std;

static FdoDataType ConvertDataType(const char* type)
{
    if (sqlite3StrICmp("BLOB", type) == 0)
        return FdoDataType_BLOB;
    else if (sqlite3StrICmp("TEXT", type) == 0)
        return FdoDataType_String;
    else if (sqlite3StrICmp("INTEGER", type) == 0)
        return FdoDataType_Int64;
    else if (sqlite3StrICmp("INT", type) == 0)
        return FdoDataType_Int64;
    else if (sqlite3StrICmp("REAL", type) == 0)
        return FdoDataType_Double;
    else if (sqlite3StrICmp("FLOAT", type) == 0)
        return FdoDataType_Double;
    else if (sqlite3StrICmp("DOUBLE", type) == 0)
        return FdoDataType_Double;
    else if (sqlite3StrICmp("VARCHAR", type) == 0)
        return FdoDataType_String;
    else if (sqlite3StrICmp("CLOB", type) == 0)
        return FdoDataType_String;

    //If we don't know the type, return -1,
    //which is an invalid data type -- hopefully
    //the client would skip over the column in such a case.
    return (FdoDataType)-1;
}


SltMetadata::SltMetadata(SltConnection* connection, const char* name)
: m_table(NULL),
  m_mdtable(NULL),
  m_fc(NULL),
  m_connection(connection), //No addref -- it owns us.
  m_iGeom(-1)
{
    sqlite3* db = m_connection->GetDB();

    /* Ensure the database schema has been loaded */
    if( sqlite3SafetyOn(db) )
    {
        return;
    }

    //we compile SQLite without thread safety...
    //sqlite3_mutex_enter(db->mutex);

    char* zErrMsg = NULL;
    int rc = sqlite3Init(db, &zErrMsg);

    if( SQLITE_OK !=rc )
        goto error_out;

    /* Locate the table in question */
    m_table = sqlite3FindTable(db, name, 0);
    if( !m_table || m_table->pSelect )
    {
        m_table = 0;
        goto error_out;
    }

error_out:
    if( sqlite3SafetyOff(db) )
    {
        rc = SQLITE_MISUSE;
    }

    //we compile SQLite without thread safety...
    //sqlite3_mutex_leave(db->mutex);
}


SltMetadata::~SltMetadata()
{
    FDO_SAFE_RELEASE(m_fc);
}


FdoClassDefinition* SltMetadata::ToClass()
{
    if (m_fc)
        return FDO_SAFE_ADDREF(m_fc);

    if (!m_table)
        return NULL;

    sqlite3* db = m_connection->GetDB();

    //find geometry properties by querying the geometry_columns table
    string gsql = "SELECT f_geometry_column,coord_dimension,srid,geometry_format,geometry_type FROM geometry_columns WHERE f_table_name='";
    gsql += m_table->zName;
    gsql += "';";

    vector<string> gnames;
    vector<int> gdims;
    vector<int> srids;
    vector<string> gformats;
    vector<int> gtypes;

    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    if (sqlite3_prepare_v2(db, gsql.c_str(), -1, &pstmt, &pzTail) == SQLITE_OK)
    {
        while (sqlite3_step(pstmt) == SQLITE_ROW)
        {
            const char* gname = (const char*)sqlite3_column_text(pstmt, 0);
            gnames.push_back(string(gname));
            
            int dim = sqlite3_column_int(pstmt, 1);
            gdims.push_back(dim);
            
            int srid = sqlite3_column_int(pstmt, 2);
            srids.push_back(srid);
            
            const char* gformat = (const char*)sqlite3_column_text(pstmt, 3);
            gformats.push_back(gformat ? gformat : "");
            
            int gtype = sqlite3_column_int(pstmt, 4);
            gtypes.push_back(gtype);
        }
    }
    else
    {
        wstring err = A2W_SLOW(pzTail);
        throw FdoException::Create(err.c_str());
    }

    sqlite3_finalize(pstmt);

    wstring fname = A2W_SLOW(m_table->zName);

    if (gnames.size() > 0)
    {       
        m_fc = FdoFeatureClass::Create(fname.c_str(), NULL);
    }
    else
    {
        m_fc = FdoClass::Create(fname.c_str(), NULL);
    }

    FdoPtr<FdoClassCapabilities> caps = FdoClassCapabilities::Create(*m_fc);
    caps->SetSupportsLocking(false);
    caps->SetSupportsLongTransactions(false);
    caps->SetSupportsWrite(true);

    m_fc->SetCapabilities(caps);
    
    FdoPtr<FdoPropertyDefinitionCollection> pdc = m_fc->GetProperties();
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = m_fc->GetIdentityProperties();

    for (int i=0; i<m_table->nCol; i++)
    {
        const char* pname = m_table->aCol[i].zName;

        //check if this is a geometry property
        int gi = -1;
        for (size_t j=0; j<gnames.size(); j++)
        {
            if (strcmp(gnames[j].c_str(), pname) == 0)
            {
                gi = (int)j;
                break;
            }
        }

        bool theGeom = (gi != -1 && m_iGeom == -1);

        std::wstring wpname = A2W_SLOW(pname);

        //is it a geometry property
        if (gi != -1)
        {
            FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(wpname.c_str(), NULL);
            gpd->SetReadOnly(false);

            //if it is the first geometry property, make it THE geometry
            //for the feature class
            if (theGeom)
            {
                ((FdoFeatureClass*)m_fc)->SetGeometryProperty(gpd);
                m_iGeom = gi;
                const char* gf = gformats[gi].c_str();
                if (strcmp(gf, "WKB") == 0)
                    m_geomFormat = eWKB;
                else if (strcmp(gf, "WKT") == 0)
                    m_geomFormat = eWKT;
                else if (strcmp(gf, "FGF") == 0)
                    m_geomFormat = eFGF;
            }

            int srid = srids[gi];
            std::wstring scname;
            FindSpatialContextName(srid, scname);            
            gpd->SetSpatialContextAssociation(scname.c_str());

            if (gdims[gi] > 2)
                gpd->SetHasElevation(true);
            if (gdims[gi] > 3)
                gpd->SetHasMeasure(true);

            int fgtype = (FdoGeometryType)gtypes[gi];

            if (fgtype > 0)
                gpd->SetSpecificGeometryTypes((FdoGeometryType*)&fgtype, 1);
            else
                gpd->SetGeometryTypes(FdoGeometricType_All); //unsure... set all.

            pdc->Add(gpd);
            continue;
        }

        FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(wpname.c_str(), NULL);
        
        dpd->SetIsAutoGenerated(i == m_table->iPKey);
        dpd->SetNullable(!m_table->aCol[i].notNull);
        dpd->SetIsSystem(m_table->aCol[i].isPrimKey != 0);
        dpd->SetReadOnly(dpd->GetIsSystem() && dpd->GetIsAutoGenerated());

        //Use Int32 for id to make some FDO clients which expect it happy
        dpd->SetDataType(dpd->GetIsSystem() ? FdoDataType_Int32 : ConvertDataType(m_table->aCol[i].zType));

        //TODO: default value is available, but we need to figure out how to eval a sqlite Expr* to a string
        //dpd->SetDefaultValue(pTab->aCol[i].pDflt);

        pdc->Add(dpd);

        if (m_table->aCol[i].isPrimKey)
            idpdc->Add(dpd);
    }

    return FDO_SAFE_ADDREF(m_fc);
}


//Given an SRID of a spatial_ref_sys record, returns
//its sr_name (if it exists) or a string representation of the SRID.
void SltMetadata::FindSpatialContextName(int srid, std::wstring& ret)
{
    ret.clear();

    std::string sql = "SELECT sr_name FROM spatial_ref_sys WHERE srid=?";

    int rc;
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
   
    //NOTE: This should fail around here if the column sr_name
    //does not exist -- we'll deal with that 
    if ((rc = sqlite3_prepare_v2(m_connection->GetDB(), sql.c_str(), -1, &stmt, &tail)) == SQLITE_OK)
    {
        rc = sqlite3_bind_int(stmt, 1, srid);
        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW )
        {
            const char* txt = (const char*)sqlite3_column_text(stmt, 0);
            ret = A2W_SLOW(txt);
        }

        sqlite3_finalize(stmt);
    }

    //No sr_name -- use the SRID as the name
    if (ret.empty())
    {
        wchar_t tmp[64];
        ret = _itow(srid, tmp, 10);
    }
}
