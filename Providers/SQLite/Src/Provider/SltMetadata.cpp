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

static std::string GetExprValue(Expr *pExpr)
{
    if (pExpr->flags & EP_IntValue)
    {
        char tmp[30];
#ifdef _WIN32
        _itoa(pExpr->u.iValue, tmp, 10);
#else
        _snprintf(tmp, 30, "%d", pExpr->u.iValue);
#endif
        return tmp;
    }
    else if (pExpr->u.zToken != NULL)
        return pExpr->u.zToken;
    return "";
}

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
  m_bIsMSelectView(false),
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

class SltColumnDefinition
{
public:
    std::wstring column_name;
    GeomFormat geometry_format;
    int coord_dimension;
    int srid;
    int geom_detType;
    int geom_type;
public:
    SltColumnDefinition()
    {
        coord_dimension = srid = geom_detType = geom_type = 0;
        geometry_format = eFGF;
    }
    ~SltColumnDefinition(){}
};

typedef std::map<const wchar_t*, SltColumnDefinition*, wstring_less> SltGeomColumnDefinitions;
typedef std::map<const wchar_t*, FdoPropertyDefinition*, wstring_less> SltFdoColumnDefinitions;
typedef std::vector< std::pair<FdoDataPropertyDefinition*, int> > SltPrimaryKeys;
typedef std::vector< std::pair<int, std::wstring> > SltCacheSpatialContext;
typedef std::vector<FdoPropertyDefinition*> SltOrderedProperties;

class SltTableDefinition
{
public:
    SltGeomColumnDefinitions geomColumns;

    SltFdoColumnDefinitions fdoColumns;
    SltOrderedProperties fdoOrdColumns; // no add ref

    SltCacheSpatialContext cachedSpContext;

    FdoGeometricPropertyDefinition* mainGeom; // no add ref
    SltPrimaryKeys primaryKeys; // no add ref

public:
    SltTableDefinition()
    {
        mainGeom = NULL;
    }
    ~SltTableDefinition()
    {
        ClearCache();
    }

    void Add(SltColumnDefinition* pcolDef)
    {
        SltGeomColumnDefinitions::iterator it = geomColumns.find(pcolDef->column_name.c_str());
        if (it != geomColumns.end())
        {
            delete it->second;
            geomColumns.erase(it);
        }
        geomColumns[pcolDef->column_name.c_str()] = pcolDef;
    }
    
    SltColumnDefinition* FindGeometric(FdoString* name)
    {
        SltGeomColumnDefinitions::iterator it = geomColumns.find(name);
        return (it != geomColumns.end()) ? it->second : NULL;
    }

    void AddPrimaryKey(FdoDataPropertyDefinition* pcolDef, int index)
    {
        primaryKeys.push_back(std::make_pair(pcolDef, index));
    }

    void Add(FdoPropertyDefinition* pcolDef)
    {
        fdoColumns[pcolDef->GetName()] = FDO_SAFE_ADDREF(pcolDef);
        fdoOrdColumns.push_back(pcolDef);
    }
    
    FdoPropertyDefinition* Find(FdoString* name)
    {
        SltFdoColumnDefinitions::iterator it = fdoColumns.find(name);
        return (it != fdoColumns.end()) ? FDO_SAFE_ADDREF(it->second) : NULL;
    }

    int GetIndexIfPrimaryKey(FdoDataPropertyDefinition* prop)
    {
        for (SltPrimaryKeys::iterator it = primaryKeys.begin(); it != primaryKeys.end(); it++)
        {
            if (it->first == prop)
                return it->second;
        }
        return -1;
    }

    void ClearCache()
    {
        for (SltGeomColumnDefinitions::iterator it = geomColumns.begin(); it != geomColumns.end(); it++)
        {
            delete it->second;
        }
        for (SltFdoColumnDefinitions::iterator it = fdoColumns.begin(); it != fdoColumns.end(); it++)
        {
            FDO_SAFE_RELEASE(it->second);
        }
        fdoColumns.clear();
        geomColumns.clear();
        primaryKeys.clear();
        mainGeom = NULL;
    }
};

void SltMetadata::BuildMetadataInfo(SltConnection* conn, SltStringList* lst)
{
    sqlite3_stmt* pstmtView = NULL;
    sqlite3_stmt* pstmtGeom = NULL;
    sqlite3_stmt* pstmtFdoCol = NULL;
    StringBuffer sb;
    sqlite3* db = conn->GetDbConnection();
    char* zErr = NULL;
    const char* pzvTail = NULL;
    sqlite3_mutex_enter(db->mutex);

    sqlite3BtreeEnterAll(db);
    int rc = sqlite3Init(db, &zErr);

    std::vector<Table*> rtables;
    if (lst != NULL && lst->size() != 0)
    {
        for(size_t i = 0; i < lst->size(); i++)
        {
            Table* table = sqlite3FindTable(db, lst->at(i).c_str(), 0);
            if (table != NULL)
                rtables.push_back(table);
        }
    }
    else
    {
        for(HashElem *i = sqliteHashFirst(&db->aDb[0].pSchema->tblHash);i; i = sqliteHashNext(i))
        {
            rtables.push_back((Table*)sqliteHashData(i));
        }
    }

    bool supDetGeom = conn->SupportsDetailedGeomType();

    for(std::vector<Table*>::iterator itt = rtables.begin(); itt != rtables.end(); itt++)
    {
        Table* pTable = *itt;
        if (!conn->NeedsMetadataLoaded(pTable->zName))
            continue;

        std::wstring fname = A2W_SLOW(pTable->zName);

        if (pTable->pSelect != NULL && 0 == pTable->nCol)
        {
            // enforce generate view columns
            sb.Reset();
            sb.Append("PRAGMA table_info(\"", 19);
            sb.Append(pTable->zName);
            sb.Append("\");", 3);
            sqlite3_stmt* pstmtPragma = NULL;
            if (sqlite3_prepare_v2(db, sb.Data(), -1, &pstmtPragma, &pzvTail) != SQLITE_OK)
                continue;
            sqlite3_step(pstmtPragma);
            sqlite3_finalize(pstmtPragma);
        }
        std::vector<SQLiteExpression> tableConstraints;
        if (pTable->pCheck != NULL && !ExtractConstraints(pTable->pCheck, tableConstraints))
            tableConstraints.clear();

        SltMetadata* metadata = new SltMetadata(conn, pTable->zName, conn->CanUseFdoMetadata());
        metadata->m_bIsView = (pTable->pSelect != NULL);

        bool geomsLoaded = false;
        SltTableDefinition tb_def;
        for (int i=0; i<pTable->nCol; i++)
        {
            const char* pname = pTable->aCol[i].zName;
            std::wstring wpname = A2W_SLOW(pname);
            FdoDataType dt = (FdoDataType)-1;

            if (NULL != pTable->aCol[i].zType)
                dt = ConvertDataType(pTable->aCol[i].zType);

            // in case we do not have a type set it to text and later if we have it defined in FDO metadata 
            // we can update the type, otherwise we use text and provider will do conversion
            if ((int)dt == -1)
                dt = FdoDataType_String;

            FdoPropertyDefinition* propDefProc = NULL;
            if (dt == FdoDataType_BLOB) // this can be a geom col
            {
                if (!geomsLoaded)
                {
                    if (pstmtGeom == NULL)
                    {
                        sb.Reset();
                        if (supDetGeom)
                            sb.Append("SELECT f_geometry_column,coord_dimension,srid,geometry_format,geometry_type,geometry_dettype FROM geometry_columns WHERE f_table_name=?;", 136);
                        else
                            sb.Append("SELECT f_geometry_column,coord_dimension,srid,geometry_format,geometry_type FROM geometry_columns WHERE f_table_name=?;", 119);
                        sqlite3_prepare_v2(db, sb.Data(), -1, &pstmtGeom, &pzvTail); // in case we fail pstmtGeom will be null; not much we can do
                    }
                    else
                        sqlite3_reset(pstmtGeom);

                    if (pstmtGeom != NULL)
                    {
                        sqlite3_bind_text(pstmtGeom, 1, pTable->zName, -1, SQLITE_STATIC);
                        while (sqlite3_step(pstmtGeom) == SQLITE_ROW)
                        {
                            const char* colName = (const char*)sqlite3_column_text(pstmtGeom, 0);
                            if (colName != NULL && colName != '\0')
                            {
                                SltColumnDefinition* pcolDef = new SltColumnDefinition();
                                pcolDef->column_name = A2W_SLOW(colName);
                                pcolDef->coord_dimension = sqlite3_column_int(pstmtGeom, 1);
                                pcolDef->srid = sqlite3_column_int(pstmtGeom, 2);
                                const char* gformat = (const char*)sqlite3_column_text(pstmtGeom, 3);
                                if (gformat != NULL && *gformat != '\0')
                                {
                                    if (strcmp(gformat, "WKB") == 0)
                                        pcolDef->geometry_format = eWKB;
                                    else if (strcmp(gformat, "WKT") == 0)
                                        pcolDef->geometry_format = eWKT;
                                    else if (strcmp(gformat, "FGF") == 0)
                                        pcolDef->geometry_format = eFGF;
                                }
                                else
                                    pcolDef->geometry_format = eFGF;
                                pcolDef->geom_type = sqlite3_column_int(pstmtGeom, 4);
                                pcolDef->geom_detType = supDetGeom ? sqlite3_column_int(pstmtGeom, 5) : 0;
                                tb_def.Add (pcolDef);
                            }
                        }
                    }
                    geomsLoaded = true;
                }
                SltColumnDefinition* pcolDef = tb_def.FindGeometric(wpname.c_str());
                if (pcolDef != NULL) // we do have a geom property
                {
                    FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(wpname.c_str(), NULL);
                    gpd->SetReadOnly(false);
                    if (tb_def.mainGeom == NULL)
                    {
                        metadata->m_geomName = gpd->GetName();
                        metadata->m_geomIndex = i;
                        metadata->m_geomFormat = pcolDef->geometry_format;
                        tb_def.mainGeom = gpd.p;
                    }
                    std::wstring scName(L"");
                    bool scFound = false;
                    for (SltCacheSpatialContext::iterator ispc = tb_def.cachedSpContext.begin(); ispc != tb_def.cachedSpContext.end(); ispc++)
                    {
                        if (pcolDef->srid == ispc->first)
                        {
                            scName = ispc->second;
                            scFound = true;
                            break;
                        }
                    }
                    if (!scFound)
                    {
                        metadata->FindSpatialContextName(pcolDef->srid, scName);
                        tb_def.cachedSpContext.push_back(std::make_pair(pcolDef->srid, scName));
                    }
                    gpd->SetSpatialContextAssociation(scName.c_str());

                    if (pcolDef->coord_dimension > 2) // XYZ
                        gpd->SetHasElevation(true);
                    if (pcolDef->coord_dimension > 3) // XYZ or XYZM
                        gpd->SetHasMeasure(true);
                    if (pcolDef->coord_dimension == 5) // XYM
                        gpd->SetHasElevation(false);

                    int fgtype = pcolDef->geom_type;
                    int fgdettype = pcolDef->geom_detType;
                    if (fgdettype <= 0)
                    {
                        if (fgtype != 0 && fgtype != 8 && fgtype != 9 && fgtype <= (int)FdoGeometryType_MultiCurvePolygon)
                            gpd->SetSpecificGeometryTypes((FdoGeometryType*)&fgtype, 1);
                        else
                            gpd->SetGeometryTypes(FdoGeometricType_All); //0 = set all.
                    }
                    else
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
                    tb_def.Add(gpd);
                    propDefProc = gpd.p;
                }
            }
            if (propDefProc == NULL)
            {
                //It's a Data property.
                FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(wpname.c_str(), NULL);
                
                // if is PK and has NOT NULL constraint then the column is not autogenerated
                if (i == pTable->iPKey && pTable->aCol[i].isPrimKey)
                    dpd->SetIsAutoGenerated(!pTable->aCol[i].notNull);
                else
                    dpd->SetIsAutoGenerated(false);

                dpd->SetNullable(!pTable->aCol[i].notNull);

                switch(dt)
                {
                case FdoDataType_DateTime:
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                    dpd->SetLength(sqlite3_limit(db, SQLITE_LIMIT_LENGTH, -1));
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
                if (pTable->aCol[i].isPrimKey)
                {
                    if (i == pTable->iPKey)
                        metadata->m_idIndex = i; //remember the index of the id property in the property collection
                    tb_def.AddPrimaryKey(dpd.p, i);
                }
                tb_def.Add(dpd);
            }
        }
        std::wstring enforcedPK;

        FdoFeatureClass* fcls = NULL;
        if (tb_def.mainGeom != NULL)
        {
            fcls = FdoFeatureClass::Create(fname.c_str(), NULL);
            metadata->m_fc = fcls;
        }
        else
            metadata->m_fc = FdoClass::Create(fname.c_str(), NULL);

        FdoPtr<FdoClassCapabilities> caps = FdoClassCapabilities::Create(*metadata->m_fc);
        caps->SetSupportsLocking(false);
        caps->SetSupportsLongTransactions(false);
        if (pTable->pSelect != NULL)
        {
            metadata->m_fc->SetIsComputed(true);
            caps->SetSupportsWrite(false);
        }
        else
            caps->SetSupportsWrite(!conn->IsReadOnlyConnection());

        metadata->m_fc->SetCapabilities(caps);
        
        FdoPtr<FdoPropertyDefinitionCollection> pdc = metadata->m_fc->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = metadata->m_fc->GetIdentityProperties();

        for (SltOrderedProperties::iterator it = tb_def.fdoOrdColumns.begin(); it != tb_def.fdoOrdColumns.end(); it++)
        {
            FdoPropertyDefinition* pd = *it;
            FdoPropertyType ptype = pd->GetPropertyType();
            if (ptype == FdoPropertyType_GeometricProperty)
            {
                // Set vertex order and strictness rule for geometry property
                caps->SetPolygonVertexOrderRule(pd->GetName(), FdoPolygonVertexOrderRule_None);
                caps->SetPolygonVertexOrderStrictness(pd->GetName(), false);
            }
            pdc->Add(pd);
        }

        if (fcls != NULL)
            fcls->SetGeometryProperty(tb_def.mainGeom);

        for (SltPrimaryKeys::iterator it = tb_def.primaryKeys.begin(); it != tb_def.primaryKeys.end(); it++)
        {
            idpdc->Add(it->first);
        }

        if (conn->CanUseFdoMetadata())
        {
            // used to make a difference between auto-generated read-only
            bool propMarkedAsReadOnly = true;
            if (pstmtFdoCol == NULL)
            {
                sb.Reset();
                sb.Append("SELECT f_column_name, f_column_desc, fdo_data_type, fdo_data_details, fdo_data_length,fdo_data_precision, fdo_data_scale FROM fdo_columns WHERE f_table_name=?;", 159);
                if (sqlite3_prepare_v2(db, sb.Data(), -1, &pstmtFdoCol, &pzvTail) != SQLITE_OK)
                    pstmtFdoCol = NULL;
            }
            else
                sqlite3_reset(pstmtFdoCol);

            if (pstmtFdoCol != NULL)
            {
                sqlite3_bind_text(pstmtFdoCol, 1, pTable->zName, -1, SQLITE_STATIC);
                while (sqlite3_step(pstmtFdoCol) == SQLITE_ROW)
                {
                    const char* colName = (const char*)sqlite3_column_text(pstmtFdoCol, 0);
                    if (colName == NULL || *colName == '\0')
                        continue;

                    std::wstring wcolName = A2W_SLOW(colName);
                    FdoPtr<FdoPropertyDefinition> pd = tb_def.Find(wcolName.c_str());
                    if (pd == NULL || pd->GetPropertyType() != FdoPropertyType_DataProperty)
                        continue;
                    FdoDataPropertyDefinition* dpd = static_cast<FdoDataPropertyDefinition*>(pd.p);

                    const char* txt = (const char*)sqlite3_column_text(pstmtFdoCol, 1);
                    if (txt != NULL)
                         dpd->SetDescription(A2W_SLOW(txt).c_str());

                    FdoDataType dt = (FdoDataType)sqlite3_column_int(pstmtFdoCol, 2);
                    dpd->SetDataType(dt);
                    if (dt == FdoDataType_DateTime)
                    {
                        FdoString* defVal = dpd->GetDefaultValue();
                        if (defVal != NULL && *defVal != '\0') // fix date time
                        {
                            FdoPtr<FdoDateTimeValue> dtVal = FdoDateTimeValue::Create(DateFromString(defVal, false));
                            dpd->SetDefaultValue(dtVal->ToString());
                        }
                    }

                    if (pTable->pSelect == NULL) // avoid this for views
                        FixConstraintTypeValues(dpd);

                    int detail = (int)sqlite3_column_int(pstmtFdoCol, 3);
                    dpd->SetIsSystem((detail & 0x01) != 0);
                    if ((detail & 0x02) != 0)
                    {
                        // if we have a view we might have an enforced PK 
                        if (pTable->pSelect != NULL && (dt == FdoDataType_Int16 || dt == FdoDataType_Int32 || dt == FdoDataType_Int64))
                        {
                            // in case there are more properties marked as read only avoid set a PK
                            if (!enforcedPK.size())
                                enforcedPK = wcolName;
                            else
                                enforcedPK = L" ";
                        }
                        dpd->SetReadOnly(true);
                    }
                    else
                        propMarkedAsReadOnly = false;
                    dpd->SetLength((int)sqlite3_column_int(pstmtFdoCol, 4));
                    dpd->SetPrecision((int)sqlite3_column_int(pstmtFdoCol, 5));
                    dpd->SetScale((int)sqlite3_column_int(pstmtFdoCol, 6));
                    
                    int idx = tb_def.GetIndexIfPrimaryKey(dpd);
                    if (idx != -1)
                    {
                        // in case we have a PK and we have a trigger property could be autogenerated
                        // we can "detect" that only when we have FDO metadata
                        if (!dpd->GetIsAutoGenerated() && pTable->pTrigger != NULL &&
                            (dt == FdoDataType_Int16 || dt == FdoDataType_Int32 || dt == FdoDataType_Int64))
                        {
                            if (dpd->GetReadOnly() && propMarkedAsReadOnly)
                                dpd->SetIsAutoGenerated(true);
                            else
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
                 }                
            }
        }

        // is view?
        if ((pTable->nCol && pTable->pSelect != NULL))
        {
            if (metadata->m_fc->GetClassType() == FdoClassType_FeatureClass)
                metadata->ProcessViewProperties(pTable, &pstmtView);

            if (idpdc->GetCount() == 1)
            {
                FdoPtr<FdoDataPropertyDefinition> pd = idpdc->GetItem(0);
                FdoDataType dtView = pd->GetDataType();
                caps->SetSupportsWrite(dtView == FdoDataType_Int16 || dtView == FdoDataType_Int32 || dtView == FdoDataType_Int64);
            }
            else if (enforcedPK.size() != 0 && enforcedPK[0] != ' ')
            {
                FdoPtr<FdoPropertyDefinition> pdef = pdc->FindItem(enforcedPK.c_str());
                if (pdef != NULL && pdef->GetPropertyType() == FdoPropertyType_DataProperty)
                {
                    idpdc->Add(static_cast<FdoDataPropertyDefinition*>(pdef.p));
                    metadata->m_idName = pdef->GetName();
                    metadata->m_bIsMSelectView = true;
                }
            }
            else
            {
                caps->SetSupportsWrite(false);
            }
        }

        if (pTable->pSelect == NULL && pTable->pIndex != NULL)
        {
            FdoPtr<FdoUniqueConstraintCollection> uqcc = metadata->m_fc->GetUniqueConstraints();
            FdoPtr<FdoPropertyDefinitionCollection> clsProps = metadata->m_fc->GetProperties();

            Index* pIndex = pTable->pIndex;
            while (pIndex != NULL)
            {
                if (pIndex->nColumn != 0 && pIndex->autoIndex)
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
        conn->AddMetadata(pTable->zName, metadata);
    }

    if (pstmtView != NULL)
        sqlite3_finalize(pstmtView);

    if (pstmtGeom != NULL)
        sqlite3_finalize(pstmtGeom);

    if (pstmtFdoCol != NULL)
        sqlite3_finalize(pstmtFdoCol);

    if (zErr) 
        sqlite3DbFree(db, zErr);
    sqlite3BtreeLeaveAll(db);
    sqlite3_mutex_leave(db->mutex);
}

FdoClassDefinition* SltMetadata::ToClass()
{
    return FDO_SAFE_ADDREF(m_fc);
}

void SltMetadata::ProcessViewProperties(Table* pTable, sqlite3_stmt** pstmtView)
{
    StlMapNamesList sources;
    StlMapPropNamesList properties;
    StlMapNamesList expressions;
    ExtractViewDetailsInfo(sources, properties, expressions, pTable);
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
    FdoPtr<FdoClassDefinition> mainfc;
    FdoPtr<FdoDataPropertyDefinition> idProp;
    if (geomClassName.size() != 0 && m_bUseFdoMetadata)
    {
        SltMetadata* md = m_connection->GetMetadata(geomClassName.c_str());
        if (md != NULL)
        {
            mainfc = md->ToClass();
            if (mainfc != NULL)
            {
                FdoPtr<FdoDataPropertyDefinitionCollection> pIdColl = mainfc->GetIdentityProperties();
                if (pIdColl->GetCount() == 1)
                {
                    idProp = pIdColl->GetItem(0);
                    FdoDataType dtp = idProp->GetDataType();
                    if (dtp != FdoDataType_Int16 && dtp != FdoDataType_Int32 && dtp != FdoDataType_Int64)
                    {
                        idProp = NULL;
                    }
                    else
                    {
                        StringBuffer sb;
                        if (*pstmtView == NULL)
                        {
                            const char* pzTail = NULL;
                            sb.Append("SELECT fdo_data_details FROM fdo_columns WHERE f_table_name=? AND f_column_name=?;", 82);
                            if (sqlite3_prepare_v2(m_connection->GetDbConnection(), sb.Data(), -1, pstmtView, &pzTail) != SQLITE_OK)
                                *pstmtView = NULL;
                        }
                        else
                            sqlite3_reset(*pstmtView);

                        if (*pstmtView != NULL)
                        {
                            sb.Reset();
                            sb.Append(mainfc->GetName());
                            sqlite3_bind_text(*pstmtView, 1, sb.Data(), sb.Length(), SQLITE_TRANSIENT);
                            
                            sb.Reset();
                            sb.Append(idProp->GetName());
                            sqlite3_bind_text(*pstmtView, 2, sb.Data(), sb.Length(), SQLITE_TRANSIENT);

                            if (sqlite3_step(*pstmtView) == SQLITE_ROW)
                            {
                                int detail = (int)sqlite3_column_int(*pstmtView, 0);
                                if ((detail & 0x02) == 0)
                                    idProp = NULL; // property is not marked as read-only
                            }
                            else // ID is not registered so we cannot assume we can use this class as main class
                                idProp = NULL;
                        }
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
                if (idPropName == item.second.second && item.second.first == geomClassName)
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
            if (mainfc != NULL)
            {
                // we have a well defined view let's mark read-only properties.
                FdoPtr<FdoPropertyDefinitionCollection> propsColl = m_fc->GetProperties();
                FdoPtr<FdoPropertyDefinitionCollection> propsMainColl = mainfc->GetProperties();
                for (int i = 0; i < propsColl->GetCount(); i++)
                {
                    FdoPtr<FdoPropertyDefinition> propCls = propsColl->GetItem(i);
                    FdoPtr<FdoPropertyDefinition> propMainCls = propsMainColl->FindItem(propCls->GetName());
                    if (propMainCls == NULL) // is sec prop
                    {
                        FdoPropertyType ptype = propCls->GetPropertyType();
                        if (FdoPropertyType_DataProperty == ptype)
                        {
                            FdoDataPropertyDefinition* pDataCls = static_cast<FdoDataPropertyDefinition*>(propCls.p);
                            pDataCls->SetReadOnly(true);
                        }
                        else if (FdoPropertyType_GeometricProperty == ptype)
                        {
                            FdoGeometricPropertyDefinition* pGeomCls = static_cast<FdoGeometricPropertyDefinition*>(propCls.p);
                            pGeomCls->SetReadOnly(true);
                        }
                    }
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
        const char* exprVal = pSelect->pEList->a[idx].zSpan;
        std::string value = (exprVal != NULL) ? exprVal : "";
        // TODO study the resulted name for un-named expression 
        // e.g. select at.p1 + bt.p1 from at, bt;
        if (name == NULL && value.size() != 0)
            name = value.c_str();
        else if (value.size() == 0 && name != NULL)
            value = name;

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
                    // do we have select * ?
                    if (*name == '*' && realPropName.size() == 1)
                    {
                        SltMetadata* md = m_connection->GetMetadata(alias.c_str());
                        if (md != NULL)
                        {
                            FdoPtr<FdoClassDefinition> fc = md->ToClass();
                            if (fc != NULL)
                            {
                                FdoPtr<FdoPropertyDefinitionCollection> props = fc->GetProperties();
                                for(int y = 0; y < props->GetCount(); y++)
                                {
                                    FdoPtr<FdoPropertyDefinition> pdata = props->GetItem(y);
                                    std::string pdataName = W2A_SLOW(pdata->GetName());
                                    properties.push_back(std::make_pair(pdataName, std::make_pair(alias, pdataName)));
                                }
                            }
                        }
                    }
                    else
                        properties.push_back(std::make_pair(name, std::make_pair(alias, realPropName)));
                }
            }
            else if (pSelect->pEList->a[idx].pExpr->op == TK_ID)
            {
                properties.push_back(std::make_pair(name, std::make_pair(sources[0].first, value)));
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

FdoDataValue* SltMetadata::ConvertDataValue(FdoDataType dt, FdoDataValue* val)
{
    if (dt == FdoDataType_DateTime)
    {
        FdoString* strVal = val->ToString();
        if (*strVal == L'\'')
            strVal++; // skip ' character
        return FdoDateTimeValue::Create(DateFromString(strVal, false));
    }
    else
        return FdoDataValue::Create(dt, val);
}

void SltMetadata::FixConstraintTypeValues(FdoDataPropertyDefinition* prop)
{
    FdoPtr<FdoPropertyValueConstraint> constr = prop->GetValueConstraint();
    if (constr == NULL)
        return;

    FdoDataType dt = prop->GetDataType();
    if (constr->GetConstraintType() == FdoPropertyValueConstraintType_Range)
    {
        FdoPropertyValueConstraintRange* rangeConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
        FdoPtr<FdoDataValue> valueMin = rangeConstr->GetMinValue();
        if (valueMin != NULL && dt != valueMin->GetDataType() && !valueMin->IsNull())
        {
            FdoPtr<FdoDataValue> nv = ConvertDataValue(dt, valueMin);
            rangeConstr->SetMinValue(nv);
        }

        FdoPtr<FdoDataValue> valueMax = rangeConstr->GetMaxValue();
        if (valueMax != NULL && dt != valueMax->GetDataType() && !valueMax->IsNull())
        {
            FdoPtr<FdoDataValue> nv = ConvertDataValue(dt, valueMax); 
            rangeConstr->SetMaxValue(nv);
        }
    }
    else
    {
        FdoPropertyValueConstraintList* listConstr = static_cast<FdoPropertyValueConstraintList*>(constr.p);
        FdoPtr<FdoDataValueCollection> valColl = listConstr->GetConstraintList();
        int cnt = (valColl != NULL) ? valColl->GetCount() : 0;
        for (int i = 0; i < cnt; i++)
        {
            FdoPtr<FdoDataValue> value = valColl->GetItem(i);
            if (value != NULL && dt != value->GetDataType() && !value->IsNull())
            {
                FdoPtr<FdoDataValue> nv = ConvertDataValue(dt, value);
                valColl->SetItem(i, nv);
            }
        }
    }
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
        {
            std::string val = GetExprValue(node);
            StringBuffer sb;
            const char* pValue = val.c_str();
            if (*pValue == '\'' && *(pValue + val.size() - 1) == '\'')
                sb.Append(pValue + 1, val.size() - 2);
            else
                sb.Append(pValue, val.size());
            result.back().values.push_back(A2W_SLOW(sb.Data()));
        }
        break;
    case TK_INTEGER:
    case TK_FLOAT:
        {
            std::string val = GetExprValue(node);
            StringBuffer sb;
            sb.Append(val.c_str(), val.size());
            result.back().values.push_back(A2W_SLOW(sb.Data()));
        }
        break;
    case TK_COLUMN:
        {
            std::string val = GetExprValue(node);
            StringBuffer sb;
            const char* pName = val.c_str();
            if (*pName == '\"')
                sb.Append(pName + 1, val.size() - 2);
            else
                sb.Append(pName, val.size());
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
	if (srid == -1)
	{
		srid = m_connection->GetDefaultSpatialContext();
		defSc = srid;
	}

	// in case we still have a valid SRID search for it
	if (srid != -1)
	{
		const char* sql = "SELECT sr_name FROM spatial_ref_sys WHERE srid=?";
		int rc;
		sqlite3_stmt* stmt = NULL;
		const char* tail = NULL;
		//NOTE: This should fail around here if the column sr_name
		//does not exist -- we'll deal with that 
		if ((rc = sqlite3_prepare_v2(m_connection->GetDbConnection(), sql, -1, &stmt, &tail)) == SQLITE_OK)
		{
			do
			{
				rc = sqlite3_bind_int(stmt, 1, srid);
				if ((rc = sqlite3_step(stmt)) == SQLITE_ROW )
				{
					const char* txt = (const char*)sqlite3_column_text(stmt, 0);
					ret = (txt == NULL || *txt == '\0') ? L"" : A2W_SLOW(txt);
					defSc = 0; // enforce to leave the do-while
				}
				else if (defSc == -1) // No sr_name -- use the SRID as the name
				{
					// here can be two cases: one is the SRID is not valid for some reason
					// and second when a geometry points to a SC which has been deleted (rare case)
					// requery with the default SRID
					sqlite3_reset(stmt);
					defSc = m_connection->GetDefaultSpatialContext();
                    if (defSc == -1)
                        srid = defSc = 0; // No default SC, leave the do-while.
                    else
                        srid = defSc; // try to return the default one.
				}
				else
					defSc = 0; // enforce to leave the do-while
			}while(defSc == -1);
			sqlite3_finalize(stmt);
		}
	}
	// In case still did not find any CS use the SRID as the name
	if (ret.empty())
	{
		wchar_t tmp[64];
        swprintf(tmp, 64, L"%d", ((srid==-1)?0:srid));
		ret = tmp;
	}
}
