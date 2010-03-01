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
#include "StringUtil.h"
#include "FdoCommonOSUtil.h"

static FdoDataType ConvertDataType(const char* type)
{
    if (StringContains(type, "INT") >= 0)
        return FdoDataType_Int64;
    else if ((StringContains(type, "CHAR") >= 0) || (StringContains(type, "CLOB") >= 0) || (StringContains(type, "TEXT") >= 0))
        return FdoDataType_String;
    else if (StringContains(type, "BLOB") >= 0)
        return FdoDataType_BLOB;
    else if ((StringContains(type, "REAL") >= 0) || (StringContains(type, "FLOA") >= 0) || (StringContains(type, "DOUB") >= 0))
        return FdoDataType_Double;
    else if (StringContains(type, "NULL") >= 0)
        return (FdoDataType)-1;
    // else should return it as NUMERIC !?

    //If we don't know the type, return -1,
    //which is an invalid data type -- hopefully
    //the client would skip over the column in such a case.
    return (FdoDataType)-1;
}

SltMetadata::SltMetadata(SltConnection* connection, const char* name, bool bUseFdoMetadata)
: m_tablename(name),
  m_bIsView(false),
  m_fc(NULL),
  m_connection(connection), //No addref -- it owns us, and we are private to it
  m_bUseFdoMetadata(bUseFdoMetadata),
  m_geomName(NULL),
  m_idName(NULL),
  m_geomIndex(-1),
  m_idIndex(-1)
{
}


SltMetadata::~SltMetadata()
{
    FDO_SAFE_RELEASE(m_fc);
}


FdoClassDefinition* SltMetadata::ToClass()
{
    if (m_fc)
    {
        return FDO_SAFE_ADDREF(m_fc);
    }

    sqlite3* db = m_connection->GetDbRead();

    char* zErr = NULL;
    Table* pTable = NULL;

    sqlite3_mutex_enter(db->mutex);

    (void)sqlite3SafetyOn(db);
    sqlite3BtreeEnterAll(db);
    int rc = sqlite3Init(db, &zErr);

    /* Locate the table in question */
    if( SQLITE_OK!=rc || (pTable = sqlite3FindTable(db, m_tablename.c_str(), 0)) == NULL) 
    {
        if (zErr) 
            sqlite3DbFree(db, zErr);
        sqlite3BtreeLeaveAll(db);
        (void)sqlite3SafetyOff(db);
        sqlite3_mutex_leave(db->mutex);
        return NULL;
    }

    m_bIsView = (pTable->pSelect != NULL);

    bool supDetGeom = m_connection->SupportsDetailedGeomType();
    //find geometry properties by querying the geometry_columns table
    StringBuffer sb;
    if (supDetGeom)
        sb.Append("SELECT f_geometry_column,coord_dimension,srid,geometry_format,"
                  "geometry_type,geometry_dettype FROM geometry_columns WHERE f_table_name=");
    else
        sb.Append("SELECT f_geometry_column,coord_dimension,srid,geometry_format,"
                  "geometry_type FROM geometry_columns WHERE f_table_name=");
    sb.AppendSQuoted(pTable->zName);
    sb.Append(";");

    std::vector<std::string> gnames;
    std::vector<int> gdims;
    std::vector<int> srids;
    std::vector<std::string> gformats;
    std::vector<int> gtypes;

    sqlite3_stmt* pfdostmt = NULL;
    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    if ((rc = sqlite3_prepare_v2(db, sb.Data(), -1, &pstmt, &pzTail)) == SQLITE_OK)
    {
        while (sqlite3_step(pstmt) == SQLITE_ROW)
        {
            const char* gname = (const char*)sqlite3_column_text(pstmt, 0);
            gnames.push_back(std::string(gname));
            
            int dim = sqlite3_column_int(pstmt, 1);
            gdims.push_back(dim);
            
            int srid = sqlite3_column_int(pstmt, 2);
            srids.push_back(srid);
            
            const char* gformat = (const char*)sqlite3_column_text(pstmt, 3);
            gformats.push_back(gformat ? gformat : "");
            
            int gtype = sqlite3_column_int(pstmt, 4);
            gtypes.push_back(gtype);
            
            int gdettype = supDetGeom ? sqlite3_column_int(pstmt, 5) : 0;
            gtypes.push_back(gdettype);
        }
    }
    else
    {
        // do we have geometry_columns? The file can be a normal data store
        if (SQLITE_ERROR != rc || sqlite3FindTable(db, "geometry_columns", 0) != NULL)
        {
            const char* err = sqlite3_errmsg(db);
            if (err != NULL)
                throw FdoException::Create(A2W_SLOW(err).c_str(), rc);
            else
                throw FdoException::Create(L"Failed to get class information.", rc);
        }
    }

    sqlite3_finalize(pstmt);

    std::wstring fname = A2W_SLOW(pTable->zName);

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
    if (pTable->pSelect != NULL)
    {
        m_fc->SetIsComputed(true);
        caps->SetSupportsWrite(false);
        if (0 == pTable->nCol)
        {
            // enforce generate view columns
            sb.Reset();
            sb.Append("PRAGMA table_info(\"", 19);
            sb.Append(pTable->zName);
            sb.Append("\");", 3);
            sqlite3_stmt* pvstmt = NULL;
            const char* pzvTail = NULL;
            if (sqlite3_prepare_v2(db, sb.Data(), -1, &pvstmt, &pzvTail) == SQLITE_OK)
            {
                while (sqlite3_step(pvstmt) == SQLITE_ROW)
                {
                    break;
                }
                sqlite3_finalize(pvstmt);
            }
        }
    }
    else
        caps->SetSupportsWrite(!m_connection->IsReadOnlyConnection());

    m_fc->SetCapabilities(caps);
    
    FdoPtr<FdoPropertyDefinitionCollection> pdc = m_fc->GetProperties();
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = m_fc->GetIdentityProperties();

    std::vector<SQLiteExpression> tableConstraints;
    if (pTable->pCheck != NULL && !ExtractConstraints(pTable->pCheck, tableConstraints))
        tableConstraints.clear();

    int iGeom = -1;
    for (int i=0; i<pTable->nCol; i++)
    {
        // TODO - handle Expr names (null names replaced by expression value) and names containing "." and ":"
        const char* pname = pTable->aCol[i].zName;

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

        std::wstring wpname = A2W_SLOW(pname);
        // used to make a difference between auto-generated read-only
        bool propMarkedAsReadOnly = true;

        //is it a geometry property
        if (gi != -1)
        {
            FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(wpname.c_str(), NULL);
            gpd->SetReadOnly(false);

            //if it is the first geometry property, make it THE geometry
            //for the feature class
            // pick first from the table but enforce first from f_geometry_column
            if (iGeom == -1 || gi == 0)
            {
                //cache the name of the geometry and its index in the proeprty collection
                m_geomName = gpd->GetName();
                m_geomIndex = pdc->GetCount();

                ((FdoFeatureClass*)m_fc)->SetGeometryProperty(gpd);
                iGeom = gi;
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

            if ((gdims[gi] & 0x01) != 0)
                gpd->SetHasElevation(true);
            if ((gdims[gi] & 0x02) != 0)
                gpd->SetHasMeasure(true);

            int fgtype = gtypes[gi];

            if (fgtype > 0)
                gpd->SetGeometryTypes(fgtype); //unsure... set what we get.
            else
                gpd->SetGeometryTypes(FdoGeometricType_All); //unsure... set all.

            int fgdettype = gtypes[gi + 1];
            if (fgdettype > 0)
            {
                // update geometry subtypes in case we have them defined
                int maxGeomVal = FdoGeometryType_MultiCurvePolygon;
                int* types = (int*)alloca(sizeof(int)*maxGeomVal);
                int idx = 0;
                int type = 0;
                do
                {
                    int geomType = (0x01 << type);
                    if ((fgdettype & geomType) != 0)
                    {
                        types[idx++] = type + 1;
                        fgdettype &= ~geomType;
                    }
                    type++;
                }while(fgdettype != 0 && type < maxGeomVal);
                gpd->SetSpecificGeometryTypes((FdoGeometryType*)types, idx);
            }

            pdc->Add(gpd);
        }
        else
        {
            //It's a Data property.
            FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(wpname.c_str(), NULL);
            
            // if is PK and has NOT NULL constraint then the column is not autogenerated
            if (i == pTable->iPKey && pTable->aCol[i].isPrimKey)
                dpd->SetIsAutoGenerated(!pTable->aCol[i].notNull);
            else
                dpd->SetIsAutoGenerated(false);

            dpd->SetNullable(!pTable->aCol[i].notNull);

            //Now get the data type -- here it gets dicey
            FdoDataType dt = (FdoDataType)-1;

            if (m_bUseFdoMetadata)
            {
                // If there is FDO metadata table, look up the data type hint for this property
                if (pfdostmt == NULL)
                {
                    sb.Reset();
                    sb.Append("SELECT f_column_desc, fdo_data_type, fdo_data_details, fdo_data_length,"
                        "fdo_data_precision, fdo_data_scale FROM fdo_columns WHERE f_table_name=? AND f_column_name=?;");
                    const char* pzTail = NULL;
                    if (sqlite3_prepare_v2(db, sb.Data(), -1, &pfdostmt, &pzTail) == SQLITE_OK)
                    {
                        sqlite3_bind_text(pfdostmt, 1, pTable->zName, -1, SQLITE_STATIC);
                        sqlite3_bind_text(pfdostmt, 2, pname, -1, SQLITE_STATIC);
                    }
                    else
                        pfdostmt = NULL;
                }
                else
                {
                    sqlite3_reset(pfdostmt);
                    sqlite3_bind_text(pfdostmt, 1, pTable->zName, -1, SQLITE_STATIC);
                    sqlite3_bind_text(pfdostmt, 2, pname, -1, SQLITE_STATIC);
                }
                if (pfdostmt)
                {
                    while (sqlite3_step(pfdostmt) == SQLITE_ROW)
                    {
                        dt = (FdoDataType)sqlite3_column_int(pfdostmt, 1);

                        const char* txt = (const char*)sqlite3_column_text(pfdostmt, 0);
                        if (txt != NULL)
                             dpd->SetDescription(A2W_SLOW(txt).c_str());
                         
                        int detail = (int)sqlite3_column_int(pfdostmt, 2);
                        dpd->SetIsSystem((detail & 0x01) != 0);
                        if ((detail & 0x02) != 0)
                            dpd->SetReadOnly(true);
                        else
                            propMarkedAsReadOnly = false;
                        dpd->SetLength((int)sqlite3_column_int(pfdostmt, 3));
                        dpd->SetPrecision((int)sqlite3_column_int(pfdostmt, 4));
                        dpd->SetScale((int)sqlite3_column_int(pfdostmt, 5));
                     }
                }
            }

            if (dt == -1)
            {
                if (NULL == pTable->aCol[i].zType)
                    continue;
                //Easy case -- no FDO metadata, just report the SQLite native type
                dt = ConvertDataType(pTable->aCol[i].zType);

                //Lie about the type of the primary key -- report Int32 for better Map3D compatibility,
                //but only do it if it's an Int64->Int32 downcast in the type -- otherwise, report
                //the correct type.
                if (dpd->GetIsSystem() && (dt == FdoDataType_Int64) && !m_bUseFdoMetadata)
                    dt = FdoDataType_Int32;

                switch(dt)
                {
                case FdoDataType_DateTime:
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                    dpd->SetLength(sqlite3_limit(m_connection->GetDbRead(), SQLITE_LIMIT_LENGTH, -1));
                    break;
                case FdoDataType_Boolean:
                case FdoDataType_Byte:
                case FdoDataType_Int16:
                case FdoDataType_Int32:
                case FdoDataType_Int64:
                    dpd->SetLength(20); // assume max size
                    break;
                case FdoDataType_Decimal:
                case FdoDataType_Double:
                case FdoDataType_Single:
                    dpd->SetLength(30); // assume max size
                    break;
                default:
                    dpd->SetLength(8);
                    break;
                }
            }

            if (dt != -1)
            {
                //successfully mapped the data type -- add it to the FDO class
                dpd->SetDataType(dt);
                
                Expr* defValExp = pTable->aCol[i].pDflt;
                if(defValExp != NULL)
                {
                    std::vector<SQLiteExpression> defExp;
                    defExp.push_back(SQLiteExpression(TK_EQ)); // emulate a condition Prop = val
                    // we support only simple default values
                    // only one value can be processed
                    if (ExtractConstraints(defValExp, defExp) && defExp.back().values.size() == 1)
                    {
                        sb.Reset();
                        sb.Append(defExp.back().values[0].c_str());
                        if (dt == FdoDataType_DateTime)
                        {
                            FdoPtr<FdoDateTimeValue> dtVal = FdoDateTimeValue::Create(DateFromString(sb.Data(), false));
                            dpd->SetDefaultValue(dtVal->ToString());
                        }
                        else if (dt == FdoDataType_Boolean)
                        {
                            FdoPtr<FdoDataValue> dtValBool = GenerateConstraintValue(dt, A2W_SLOW(sb.Data()).c_str());
                            if (dtValBool != NULL)
                                dpd->SetDefaultValue(dtValBool->ToString());
                        }
                        else
                            dpd->SetDefaultValue(A2W_SLOW(sb.Data()).c_str());
                    }
                }
                // since on a class we usually don't have too many constraints we can use normal search
                for(size_t idx = 0; idx < tableConstraints.size(); idx ++)
                {
                    SQLiteExpression& operation = tableConstraints.at(idx);
                    if (operation.name == wpname)
                        GenerateConstraint(dpd, operation);
                }

                pdc->Add(dpd);
                if (pTable->aCol[i].isPrimKey)
                {
                    if (dt == FdoDataType_Int64 && !m_bUseFdoMetadata)
                    {
                        //Lie about the type of the primary key -- report Int32 for better Map3D compatibility,
                        //but only do it if it's an Int64->Int32 downcast in the type -- otherwise, report
                        //the correct type.
                        dpd->SetIsAutoGenerated(i == pTable->iPKey);
                        dt = FdoDataType_Int32;
                        dpd->SetDataType(dt);
                    }
                    if (m_bUseFdoMetadata)
                    {
                        // in case we have a PK and we have a trigger property could be autogenerated
                        // we can "detect" that only when we have FDO metadata
                        if (!dpd->GetIsAutoGenerated() && pTable->pTrigger != NULL &&
                            (dt == FdoDataType_Int16 || dt == FdoDataType_Int32 || dt == FdoDataType_Int64))
                        {
                            dpd->SetIsAutoGenerated(dpd->GetReadOnly());
                        }
                        // in case we use FDO metadata there are cases when SQLite can "mark" a property
                        // as auto-generated property (and SQLite is really generating the values), however
                        // we need cases to expose schema as a non auto-generated and we will base that on
                        // the read-only flag since we cannot have auto-generated without read-only:
                        // in case property is not marked as read-only then is not auto-generated
                        if (!propMarkedAsReadOnly && dpd->GetIsAutoGenerated())
                        {
                            dpd->SetIsAutoGenerated(false);
                            dpd->SetReadOnly(false);
                        }
                    }
                    m_idIndex = pdc->GetCount() - 1; //remember the index of the id property in the property collection
                    idpdc->Add(dpd);
                }
            }
            else if (pTable->pSelect != NULL) // is view
            {
                // in case of a view we can have expressions.
                // we just need to expose the property even we don't know the type
                dpd->SetDataType(FdoDataType_String);
                pdc->Add(dpd);
            }
        }
    }

    // is view?
    if ((pTable->nCol && pTable->pSelect != NULL))
    {
        // views don't have a primary key we need to "generate" one as ROWID
        if (m_fc->GetClassType() == FdoClassType_FeatureClass)
            ProcessViewProperties(pTable);

        if (idpdc->GetCount() == 0)
        {
            std::wstring propAgName = L"rowid";
            FdoPtr<FdoPropertyDefinition> epd = pdc->FindItem(propAgName.c_str());
            if (epd != NULL)
            {
                propAgName = L"_rowid_";
                epd = pdc->FindItem(propAgName.c_str());
                if (epd != NULL)
                {
                    propAgName = L"oid";
                    epd = pdc->FindItem(propAgName.c_str());
                    // if (epd != NULL) - bad luck, not sure what to do in this case... use it...
                }
            }
            if (epd == NULL)
            {
                FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(propAgName.c_str(), NULL);
                dpd->SetDataType(FdoDataType_Int32);
                dpd->SetIsAutoGenerated(true);
                dpd->SetNullable(false);
                dpd->SetReadOnly(true);
                dpd->SetLength(8);
                pdc->Add(dpd);
                idpdc->Add(dpd);
            }
        }
    }

    if (pTable->pSelect == NULL && pTable->pIndex != NULL)
    {
        FdoPtr<FdoUniqueConstraintCollection> uqcc = m_fc->GetUniqueConstraints();
        FdoPtr<FdoPropertyDefinitionCollection> clsProps = m_fc->GetProperties();

        Index* pIndex = pTable->pIndex;
        while (pIndex != NULL)
        {
            if (pIndex->nColumn != 0)
            {
                FdoPtr<FdoUniqueConstraint> uqc = FdoUniqueConstraint::Create();                
                FdoPtr<FdoDataPropertyDefinitionCollection> propsConstr = uqc->GetProperties();
                for (int i = 0; i < pIndex->nColumn; i++)
                {
                    FdoPtr<FdoPropertyDefinition> constrProp = clsProps->FindItem(A2W_SLOW(pTable->aCol[pIndex->aiColumn[i]].zName).c_str());
                    if (constrProp != NULL && constrProp->GetPropertyType() == FdoPropertyType_DataProperty)
                        propsConstr->Add(static_cast<FdoDataPropertyDefinition*>(constrProp.p));
                }
                if (propsConstr->GetCount() != 0)
                    uqcc->Add(uqc);
            }
            pIndex = pIndex->pNext;
        }        
    }

    if (pfdostmt)
        sqlite3_finalize(pfdostmt);

    sqlite3BtreeLeaveAll(db);
    (void)sqlite3SafetyOff(db);
    sqlite3_mutex_leave(db->mutex);

    return FDO_SAFE_ADDREF(m_fc);
}

void SltMetadata::ProcessViewProperties(Table* pTable)
{
    StlMapNamesList sources;
    StlMapPropNamesList properties;
    StlMapNamesList expressions;
    bool extraCheck = ExtractViewDetailsInfo(sources, properties, expressions, pTable);
    size_t propSz = properties.size();
    size_t srcSz = sources.size();
    if (srcSz == 0 || propSz == 0 || m_geomName == NULL)
        return;
    // search the table who has the geometry column
    std::string geomName = W2A_SLOW(m_geomName);
    std::string geomClassName;
    for (size_t i = 0; i < propSz; i++)
    {
        StlPropNamePair& item = properties.at(i);
        if (geomName == item.first)
        {
            geomClassName = item.second.first;
            break;
        }
    }
    FdoPtr<FdoDataPropertyDefinition> idProp;
    if (geomClassName.size() != 0)
    {
        SltMetadata* md = m_connection->GetMetadata(geomClassName.c_str());
        if (md != NULL)
        {
            FdoPtr<FdoClassDefinition> fc = md->ToClass();
            if (fc != NULL)
            {
                FdoPtr<FdoDataPropertyDefinitionCollection> pIdColl = fc->GetIdentityProperties();
                if (pIdColl->GetCount() == 1)
                {
                    idProp = pIdColl->GetItem(0);
                    FdoDataType dtp = idProp->GetDataType();
                    if (dtp != FdoDataType_Int16 && dtp != FdoDataType_Int32 && dtp != FdoDataType_Int64)
                    {
                        idProp = NULL;
                    }
                    else if (extraCheck)
                    {
                        if (m_bUseFdoMetadata)
                        {
                            StringBuffer sb;
                            sqlite3_stmt* pstmt = NULL;
                            const char* pzTail = NULL;
                            sb.Append("SELECT fdo_data_details FROM fdo_columns WHERE f_table_name='", 61);
                            sb.Append(m_fc->GetName());
                            sb.Append("' AND f_column_name='", 21);
                            sb.Append(idProp->GetName());
                            sb.Append("';", 2);
                            if (sqlite3_prepare_v2(m_connection->GetDbRead(), sb.Data(), -1, &pstmt, &pzTail) == SQLITE_OK)
                            {
                                if (sqlite3_step(pstmt) == SQLITE_ROW)
                                {
                                    int detail = (int)sqlite3_column_int(pstmt, 0);
                                    if ((detail & 0x02) == 0)
                                        idProp = NULL; // property is not marked as read-only
                                }
                                else // ID is not registered so we cannot assume we can use this class as main class
                                    idProp = NULL;
                                sqlite3_finalize(pstmt);
                            }
                        }
                        else
                            idProp = NULL;
                    }
                }
            }
        }
    }
    if (idProp != NULL)
    {
        FdoPtr<FdoDataPropertyDefinitionCollection> pIdColl = m_fc->GetIdentityProperties();
        if (pIdColl->GetCount() == 0)
        {
            std::string idPropName = W2A_SLOW(idProp->GetName());
            for (size_t i = 0; i < propSz; i++)
            {
                StlPropNamePair& item = properties.at(i);
                if (_strnicmp(idPropName.c_str(), item.second.second.c_str(), idPropName.size()) == 0)
                {
                    std::wstring wIdName = A2W_SLOW(item.first.c_str());
                    FdoPtr<FdoPropertyDefinitionCollection> pClsColl = m_fc->GetProperties();
                    FdoPtr<FdoPropertyDefinition> pDef = pClsColl->FindItem(wIdName.c_str());
                    if (pDef != NULL && pDef->GetPropertyType() == FdoPropertyType_DataProperty)
                    {
                        FdoDataPropertyDefinition* pdNewId = static_cast<FdoDataPropertyDefinition*>(pDef.p);
                        pdNewId->SetReadOnly(true); // make it read only
                        pIdColl->Add(pdNewId);
                        m_idName = pDef->GetName();
                    }
                    m_tablename = geomClassName;
                    break;
                }
            }
        }
    }
}

bool SltMetadata::ExtractViewDetailsInfo(StlMapNamesList& sources, StlMapPropNamesList& properties, StlMapNamesList& expressions, Table* pTable)
{
    bool retVal = false;
    Select* pSelect = pTable->pSelect;

    if(pSelect == NULL || pSelect->pSrc == NULL || pSelect->pSrc->nSrc == 0 || 
        pSelect->pEList == NULL || pSelect->pEList->nExpr == 0)
        return false;
    switch (pSelect->op)
    {
        // this can involve complicated queries which are not from one table
        // extra checks need to be done to ensure we are not wrongfully 
        // assuming we can use a PK and SI from a main table.
    case TK_ALL:
        retVal = true;
        break;
    case TK_SELECT:
        break;
    default:
        return false;
    }
    
    for(int idx = 0; idx < pSelect->pSrc->nSrc; idx++)
    {
        const char* alias = pSelect->pSrc->a[idx].zAlias;
        const char* name = pSelect->pSrc->a[idx].zName;
        if (name == NULL)
            continue;
        sources.push_back(std::make_pair(name, ((alias == NULL) ? "" : alias)));
    }
    if (sources.size() == 0)
        return false;

    for(int idx = 0; idx < pSelect->pEList->nExpr; idx++)
    {
        const char* name = pSelect->pEList->a[idx].zName;
        std::string value;
        if (pSelect->pEList->a[idx].pExpr->span.n)
            value = std::string((const char*)pSelect->pEList->a[idx].pExpr->span.z, pSelect->pEList->a[idx].pExpr->span.n);
        // TODO study the resulted name for un-named expression 
        // e.g. select at.p1 + bt.p1 from at, bt;
        if (name == NULL && value.size() != 0)
            name = value.c_str();

        if (name != NULL && value.size() != 0)
        {
            if (pSelect->pEList->a[idx].pExpr->op == TK_DOT)
            {
                const char* propSrc = value.c_str();
                int stClsStr = 0, lenClsStr = 0, stPropStr = 0, lenPropStr = 0;

                const char* propToSrc = ExtractDbName(propSrc, stClsStr, lenClsStr);
                if (lenClsStr != 0 && propToSrc != NULL)
                    ExtractDbName(propToSrc, stPropStr, lenPropStr);
                
                if (lenClsStr != 0 && lenPropStr != 0)
                {
                    // property from a table/view
                    std::string alias = std::string(propSrc+stClsStr, lenClsStr);
                    for (StlMapNamesList::iterator it = sources.begin(); it < sources.end(); it++)
                    {
                        if (it->second == alias)
                        {
                            alias = it->first;
                            break;
                        }
                    }
                    std::string realPropName = std::string(propToSrc+stPropStr, lenPropStr);
                    if (pSelect->pEList->a[idx].zName == NULL)
                        name = realPropName.c_str();
                    properties.push_back(std::make_pair(name, std::make_pair(alias, realPropName)));
                }
            }
            else
                expressions.push_back(std::make_pair(name, value)); // it's an expression
        }
    }
    return retVal;
}

FdoDataValue* SltMetadata::GenerateConstraintValue(FdoDataType type, FdoString* value)
{
    FdoPtr<FdoDataValue> retVal;
    switch(type)
    {
    case FdoDataType_Boolean:
        {
            bool bval = false;
            if (sqlite3UpperToLower[*value] == sqlite3UpperToLower['T'])
                bval = true;
            else if(sqlite3UpperToLower[*value] == sqlite3UpperToLower['F'])
                bval = false;
            else
                bval = FdoCommonOSUtil::wtoi(value) != 0;
            retVal = FdoBooleanValue::Create(bval);
        }
        break;
    case FdoDataType_Byte:
        retVal = FdoByteValue::Create((FdoByte)FdoCommonOSUtil::wtoi(value));
        break;
    case FdoDataType_Int16:
        retVal = FdoInt16Value::Create((FdoInt16)FdoCommonOSUtil::wtoi(value));
        break;
    case FdoDataType_Int32:
        retVal = FdoInt32Value::Create((FdoInt32)FdoCommonOSUtil::wtoi(value));
        break;
    case FdoDataType_Int64:
        retVal = FdoInt64Value::Create(
        #ifdef _WIN32
                _wtoi64( value )
        #else
               atoll ( W2A_SLOW(value).c_str() )
        #endif
        );
        break;
    case FdoDataType_Decimal:
        retVal = FdoDecimalValue::Create(FdoCommonOSUtil::wtof(value));
        break;
    case FdoDataType_Double:
        retVal = FdoDoubleValue::Create(FdoCommonOSUtil::wtof(value));
        break;
    case FdoDataType_Single:
        retVal = FdoSingleValue::Create((FdoFloat)FdoCommonOSUtil::wtof(value));
        break;
    case FdoDataType_String:
        retVal = FdoStringValue::Create(value);
        break;
    case FdoDataType_DateTime:
        if (value == NULL || *value == '\0')
            retVal = NULL;
        else
            retVal = FdoDateTimeValue::Create(DateFromString(value, false));
        break;
    }
    return FDO_SAFE_ADDREF(retVal.p);
}

void SltMetadata::GenerateConstraint(FdoDataPropertyDefinition* prop, SQLiteExpression& operation)
{
    FdoPtr<FdoPropertyValueConstraint> constr = prop->GetValueConstraint();
    FdoDataType dt = prop->GetDataType();
    switch(operation.op)
    {
    case TK_BETWEEN:
        if (operation.values.size() == 2)
        {
            if (constr == NULL || constr->GetConstraintType() != FdoPropertyValueConstraintType_Range)
                constr = FdoPropertyValueConstraintRange::Create();
            FdoPropertyValueConstraintRange* rangeConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
            FdoPtr<FdoDataValue> valueMin = SltMetadata::GenerateConstraintValue(dt, operation.values.at(0).c_str());
            FdoPtr<FdoDataValue> valueMax = SltMetadata::GenerateConstraintValue(dt, operation.values.at(1).c_str());
            if (valueMin != NULL)
            {
                rangeConstr->SetMinValue(valueMin);
                rangeConstr->SetMinInclusive(true);
            }
            if (valueMax != NULL)
            {
                rangeConstr->SetMaxValue(valueMax);
                rangeConstr->SetMaxInclusive(true);
            }
        }
        break;
    case TK_IN:
        if (operation.values.size() != 0)
        {
            if (constr == NULL || constr->GetConstraintType() != FdoPropertyValueConstraintType_List)
                constr = FdoPropertyValueConstraintList::Create();
            FdoPropertyValueConstraintList* listConstr = static_cast<FdoPropertyValueConstraintList*>(constr.p);
            FdoPtr<FdoDataValueCollection> valColl = listConstr->GetConstraintList();
            for (size_t idx = 0; idx < operation.values.size(); idx++)
            {
                FdoPtr<FdoDataValue> value = SltMetadata::GenerateConstraintValue(dt, operation.values.at(idx).c_str());
                if (value != NULL)
                    valColl->Add(value);
            }
            // in case we failed to have a list ignore the constraint
            if (valColl->GetCount() == 0)
                constr = NULL;
        }
        break;
    case TK_EQ: // =
        if (operation.values.size() == 1)
        {
            if (constr == NULL || constr->GetConstraintType() != FdoPropertyValueConstraintType_Range)
                constr = FdoPropertyValueConstraintRange::Create();
            FdoPropertyValueConstraintRange* rangeConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
            FdoPtr<FdoDataValue> value = SltMetadata::GenerateConstraintValue(dt, operation.values.at(0).c_str());
            if (value != NULL)
            {
                // we simulate "=" as an operation ">= & <="
                rangeConstr->SetMaxValue(value);
                rangeConstr->SetMaxInclusive(true);
                rangeConstr->SetMinValue(value);
                rangeConstr->SetMinInclusive(true);
            }
        }
        break;
    case TK_GE: // >=
    case TK_GT: // >
        if (operation.values.size() == 1)
        {
            if (constr == NULL || constr->GetConstraintType() != FdoPropertyValueConstraintType_Range)
                constr = FdoPropertyValueConstraintRange::Create();
            FdoPropertyValueConstraintRange* rangeConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
            FdoPtr<FdoDataValue> value = SltMetadata::GenerateConstraintValue(dt, operation.values.at(0).c_str());
            if (value != NULL)
            {
                rangeConstr->SetMinValue(value);
                rangeConstr->SetMinInclusive(operation.op == TK_GE);
            }
        }
        break;
    case TK_LE: // <=
    case TK_LT: // <
        if (operation.values.size() == 1)
        {
            if (constr == NULL || constr->GetConstraintType() != FdoPropertyValueConstraintType_Range)
                constr = FdoPropertyValueConstraintRange::Create();
            FdoPropertyValueConstraintRange* rangeConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
            FdoPtr<FdoDataValue> value = SltMetadata::GenerateConstraintValue(dt, operation.values.at(0).c_str());
            if (value != NULL)
            {
                rangeConstr->SetMaxValue(value);
                rangeConstr->SetMaxInclusive(operation.op == TK_LE);
            }
        }
        break;
    }
    if (constr != NULL)
        prop->SetValueConstraint(constr);
}

// we can support only some kind of operators in Constraints
bool SltMetadata::ExtractConstraints(Expr* node, std::vector<SQLiteExpression>& result)
{
    bool valid = true;
    switch(node->op)
    {
    case TK_AND:
        if(node->pLeft != NULL)
            valid = ExtractConstraints(node->pLeft, result);
        if(valid && node->pRight != NULL)
            valid = ExtractConstraints(node->pRight, result);
        break;
    case TK_BETWEEN:
    case TK_IN:
        result.push_back(SQLiteExpression(node->op));
        if (node->x.pList != 0 && node->x.pList->nExpr != 0)
        {
            for(int idx = 0; idx < node->x.pList->nExpr && valid; idx++)
            {
                valid = ExtractConstraints(node->x.pList->a[idx].pExpr, result);
            }
        }
        if(node->pLeft != NULL)
            valid = ExtractConstraints(node->pLeft, result);
        if(valid && node->pRight != NULL)
            valid = ExtractConstraints(node->pRight, result);
        break;
    case TK_EQ: // =
    case TK_GT: // >
    case TK_LE: // <=
    case TK_LT: // <
    case TK_GE: // >=
        result.push_back(SQLiteExpression(node->op));
        if(node->pLeft != NULL)
            valid = ExtractConstraints(node->pLeft, result);
        if(valid && node->pRight != NULL)
            valid = ExtractConstraints(node->pRight, result);
        break;
    case TK_STRING:
        if(node->token.n != 0)
        {
            StringBuffer sb;
            const char* pValue = (const char*)node->token.z;
            if (*pValue == '\'' && *(pValue + node->token.n - 1) == '\'')
                sb.Append(pValue + 1, node->token.n - 2);
            else
                sb.Append(pValue, node->token.n);
            result.back().values.push_back(A2W_SLOW(sb.Data()));
        }
        break;
    case TK_INTEGER:
    case TK_FLOAT:
        if(node->token.n != 0)
        {
            StringBuffer sb;
            sb.Append((const char*)node->token.z, node->token.n);
            result.back().values.push_back(A2W_SLOW(sb.Data()));
        }
        break;
    case TK_COLUMN:
        if(node->token.n != 0)
        {
            StringBuffer sb;
            const char* pName = (const char*)node->token.z;
            if (*pName == '\"')
                sb.Append(pName + 1, node->token.n - 2);
            else
                sb.Append(pName, node->token.n);
            result.back().name = A2W_SLOW(sb.Data());
        }
        break;
    case TK_UMINUS:
        if(node->pLeft != NULL)
        {
            valid = ExtractConstraints(node->pLeft, result);
            SQLiteExpression& val = result.back();
            if (val.values.size() != 0)
                val.values.back() = L"-" + val.values.back();
            else
                valid = false;
        }
        else
            valid = false;
        break;
    case TK_UPLUS:
        // just get it since +val = val
        if(node->pLeft != NULL)
            valid = ExtractConstraints(node->pLeft, result);
        else
            valid = false;
        break;
    default:
        valid = false;
        break;
    }
    return valid;
}

//Given an SRID of a spatial_ref_sys record, returns
//its sr_name (if it exists) or a string representation of the SRID.
void SltMetadata::FindSpatialContextName(int srid, std::wstring& ret)
{
    ret.clear();
	int defSc = -1;
	if (!srid)
	{
		srid = m_connection->GetDefaultSpatialContext();
		defSc = srid;
	}

	// in case we still have a valid SRID search for it
	if (srid)
	{
		const char* sql = "SELECT sr_name FROM spatial_ref_sys WHERE srid=?";
		int rc;
		sqlite3_stmt* stmt = NULL;
		const char* tail = NULL;
		//NOTE: This should fail around here if the column sr_name
		//does not exist -- we'll deal with that 
		if ((rc = sqlite3_prepare_v2(m_connection->GetDbRead(), sql, -1, &stmt, &tail)) == SQLITE_OK)
		{
			do
			{
				rc = sqlite3_bind_int(stmt, 1, srid);
				if ((rc = sqlite3_step(stmt)) == SQLITE_ROW )
				{
					const char* txt = (const char*)sqlite3_column_text(stmt, 0);
					ret = (txt == NULL || *txt == '\0') ? L"" : A2W_SLOW(txt);
					defSc = 0; // enforce to leave the break
				}
				else if (defSc == -1) // No sr_name -- use the SRID as the name
				{
					// here can be two cases: one is the SRID is not valid for some reason
					// and second when a geometry points to a SC which has been deleted (rare case)
					// requery with the default SRID
					sqlite3_reset(stmt);
					defSc = m_connection->GetDefaultSpatialContext();
					srid = defSc; // try to return the default one.
				}
				else
					defSc = 0; // enforce to leave the break
			}while(defSc == -1);
			sqlite3_finalize(stmt);
		}
	}
	// In case still did not find any CS use the SRID as the name
	if (ret.empty())
	{
		wchar_t tmp[64];
		swprintf(tmp, 64, L"%d", srid);
		ret = tmp;
	}
}
