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


SltMetadata::SltMetadata(SltConnection* connection, const char* name, bool bUseFdoMetadata)
: m_table(NULL),
  m_mdtable(NULL),
  m_fc(NULL),
  m_connection(connection), //No addref -- it owns us, and we are private to it
  m_bUseFdoMetadata(bUseFdoMetadata)
{
    sqlite3* db = m_connection->GetDbRead();

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

    sqlite3* db = m_connection->GetDbRead();

    bool supDetGeom = m_connection->SupportsDetailedGeomType();
    //find geometry properties by querying the geometry_columns table
    StringBuffer sb;
    if (supDetGeom)
        sb.Append("SELECT f_geometry_column,coord_dimension,srid,geometry_format,\
                  geometry_type,geometry_dettype FROM geometry_columns WHERE f_table_name=");
    else
        sb.Append("SELECT f_geometry_column,coord_dimension,srid,geometry_format,\
                  geometry_type FROM geometry_columns WHERE f_table_name=");
    sb.AppendSQuoted(m_table->zName);
    sb.Append(";");

    std::vector<std::string> gnames;
    std::vector<int> gdims;
    std::vector<int> srids;
    std::vector<std::string> gformats;
    std::vector<int> gtypes;

    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    if (sqlite3_prepare_v2(db, sb.Data(), -1, &pstmt, &pzTail) == SQLITE_OK)
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
        std::wstring err = A2W_SLOW(pzTail);
        throw FdoException::Create(err.c_str());
    }

    sqlite3_finalize(pstmt);

    std::wstring fname = A2W_SLOW(m_table->zName);

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

    std::vector<SQLiteExpression> tableConstraints;
    if (m_table->pCheck != NULL && !ExtractConstraints(m_table->pCheck, tableConstraints))
        tableConstraints.clear();

    int iGeom = -1;
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

        std::wstring wpname = A2W_SLOW(pname);

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
            
            dpd->SetIsAutoGenerated(i == m_table->iPKey);
            dpd->SetNullable(!m_table->aCol[i].notNull);
            dpd->SetIsSystem(m_table->aCol[i].isPrimKey != 0);
            dpd->SetReadOnly(dpd->GetIsSystem() && dpd->GetIsAutoGenerated());
            
            //Now get the data type -- here it gets dicey
            FdoDataType dt = (FdoDataType)-1;

            if (m_bUseFdoMetadata)
            {
                // TODO column by column is slow, select by table name and process properties is faster...
                // If there is FDO metadata table, look up the data type hint for this property
                sb.Reset();
                sb.Append("SELECT f_column_desc, fdo_data_type, fdo_data_details, fdo_data_length, "
                    "fdo_data_precision, fdo_data_scale FROM fdo_columns WHERE f_table_name=");
                sb.AppendSQuoted(m_table->zName);
                sb.Append(" AND f_column_name=");
                sb.AppendSQuoted(pname);
                sb.Append(";");
                
                sqlite3_stmt* pstmt = NULL;
                const char* pzTail = NULL;
                if (sqlite3_prepare_v2(db, sb.Data(), -1, &pstmt, &pzTail) == SQLITE_OK)
                {
                    while (sqlite3_step(pstmt) == SQLITE_ROW)
                    {
                        dt = (FdoDataType)sqlite3_column_int(pstmt, 1);

                        const char* txt = (const char*)sqlite3_column_text(pstmt, 0);
                        if (txt != NULL)
                            dpd->SetDescription(A2W_SLOW(txt).c_str());
                        
                        int detail = (int)sqlite3_column_int(pstmt, 2);
                        dpd->SetIsSystem((detail & 0x01) != 0);
                        dpd->SetNullable((detail & 0x02) != 0);
                        dpd->SetLength((int)sqlite3_column_int(pstmt, 3));
                        dpd->SetPrecision((int)sqlite3_column_int(pstmt, 4));
                        dpd->SetScale((int)sqlite3_column_int(pstmt, 5));
                    }
                }

                sqlite3_finalize(pstmt);
            }

            if (dt == -1)
            {
                //Easy case -- no FDO metadata, just report the SQLite native type
                dt = ConvertDataType(m_table->aCol[i].zType);

                //Lie about the type of the primary key -- report Int32 for better Map3D compatibility
                if (dpd->GetIsSystem())
                    dt = FdoDataType_Int32;

                switch(dt)
                {
                case FdoDataType_DateTime:
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                    dpd->SetLength(sqlite3_limit(m_connection->GetDbRead(), SQLITE_LIMIT_LENGTH, -1));
                    break;
                default: // set len = 8 for all other cases
                    dpd->SetLength(8);
                    break;
                }
            }

            if (dt != -1)
            {
                //successfully mapped the data type -- add it to the FDO class
                dpd->SetDataType(dt);
                
                Expr* defValExp = m_table->aCol[i].pDflt;
                // we support only simple default values
                if(defValExp != NULL && defValExp->token.n != 0)
                {
                    sb.Reset();
                    sb.Append((const char*)defValExp->token.z, defValExp->token.n);
                    dpd->SetDefaultValue(A2W_SLOW(sb.Data()).c_str());
                }
                // since on a class we usually don't have too many constraints we can use normal search
                for(size_t idx = 0; idx < tableConstraints.size(); idx ++)
                {
                    SQLiteExpression& operation = tableConstraints.at(idx);
                    if (operation.name == wpname)
                        GenerateConstraint(dpd, operation);
                }

                pdc->Add(dpd);
                if (m_table->aCol[i].isPrimKey)
                    idpdc->Add(dpd);
            }
        }
    }

    return FDO_SAFE_ADDREF(m_fc);
}

FdoDataValue* SltMetadata::GenerateConstraintValue(FdoDataType type, FdoString* value)
{
    FdoPtr<FdoDataValue> retVal;
    switch(type)
    {
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
        // TODO
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
        if (node->pList != 0 && node->pList->nExpr != 0)
        {
            for(int idx = 0; idx < node->pList->nExpr && valid; idx++)
            {
                valid = ExtractConstraints(node->pList->a[idx].pExpr, result);
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

    char* sql = "SELECT sr_name FROM spatial_ref_sys WHERE srid=?";

    int rc;
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
   
    //NOTE: This should fail around here if the column sr_name
    //does not exist -- we'll deal with that 
    if ((rc = sqlite3_prepare_v2(m_connection->GetDbRead(), sql, -1, &stmt, &tail)) == SQLITE_OK)
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
        swprintf(tmp, 64, L"%d", srid);
        ret = tmp;
    }
}
