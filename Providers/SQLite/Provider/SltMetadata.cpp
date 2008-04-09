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

using namespace std;

static FdoDataType ConvertDataType(const char* type)
{
    if (sqlite3StrICmp("BLOB", type) == 0)
        return FdoDataType_BLOB;
    else if (sqlite3StrICmp("TEXT", type) == 0)
        return FdoDataType_String;
    else if (sqlite3StrICmp("INTEGER", type) == 0)
        return FdoDataType_Int64;
    else if (sqlite3StrICmp("REAL", type) == 0)
        return FdoDataType_Double;

    return (FdoDataType)-1;
}


SltMetadata::SltMetadata(sqlite3* db, const char* name)
: m_table(NULL),
  m_mdtable(NULL),
  m_fc(NULL),
  m_db(db),
  m_iGeom(-1)
{

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
    if (sqlite3_prepare_v2(m_db, gsql.c_str(), -1, &pstmt, &pzTail) == SQLITE_OK)
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
            FdoGeometricPropertyDefinition* gpd = FdoGeometricPropertyDefinition::Create(wpname.c_str(), NULL);
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

            wchar_t wsrid[16];
            swprintf(wsrid, 16, L"%d", srids[gi]); 
            gpd->SetSpatialContextAssociation(wsrid);

            if (gdims[gi] > 2)
                gpd->SetHasElevation(true);
            if (gdims[gi] > 3)
                gpd->SetHasMeasure(true);

            int fgtype = (FdoGeometryType)gtypes[gi];

            if (fgtype != -1)
                gpd->SetSpecificGeometryTypes((FdoGeometryType*)&fgtype, 1);
            else
                gpd->SetGeometryTypes(FdoGeometricType_All); //unsure... set all.

            pdc->Add(gpd);
            continue;
        }

        FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(wpname.c_str(), NULL);
        
        dpd->SetIsAutoGenerated(i == m_table->iPKey && m_table->autoInc);
        dpd->SetNullable(!m_table->aCol[i].notNull);
        dpd->SetIsSystem(sqlite3IsRowid(m_table->aCol[i].zName)!=0);
        dpd->SetReadOnly(dpd->GetIsSystem() && dpd->GetIsAutoGenerated());
        dpd->SetDataType(dpd->GetIsSystem() ? FdoDataType_Int32 : ConvertDataType(m_table->aCol[i].zType));

        //TODO: default value is available, but we need to figure out how to eval a sqlite Expr* to a string
        //dpd->SetDefaultValue(pTab->aCol[i].pDflt);

        pdc->Add(dpd);

        if (m_table->aCol[i].isPrimKey)
            idpdc->Add(dpd);
    }

    return FDO_SAFE_ADDREF(m_fc);
}


