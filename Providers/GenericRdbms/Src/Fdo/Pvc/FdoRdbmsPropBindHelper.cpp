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
#include "Fdo/Pvc/FdoRdbmsPropBindHelper.h"

#ifndef _WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define _SQL_PARAM_INPUT 1
#define _SQL_MAX_VAR_LEN_BIND 8000
#define _SQL_MAX_TXT_LEN_BIND 5000

class FdoRdbmsPvdBindDef
{
    std::vector<FdoRdbmsPvcBindDef*> params;
    size_t usedCnt;
public:
    FdoRdbmsPvdBindDef()
    {
        usedCnt = 0;
    }
    FdoRdbmsPvdBindDef(size_t cnt)
    {
        params.reserve(cnt);
        usedCnt = 0;
    }
    ~FdoRdbmsPvdBindDef()
    {
        Clear();
        std::vector<FdoRdbmsPvcBindDef*>::iterator it;
        for (it = params.begin(); it < params.end(); it++)
            delete *it;
        params.clear();
    }
    void Clear()
    {
        for (size_t idx = 0; idx < usedCnt; idx++)
        {
            FdoRdbmsPvcBindDef* pBind = params.at(idx);
            if (pBind->null_ind)
                free(pBind->null_ind);

            // the BLOB value was not allocated
            if (NULL != pBind->value.strvalue)
            {
                if (pBind->type != FdoDataType_BLOB)
                {
                    if (pBind->type == FdoRdbmsDataType_Geometry)
                        ((FdoIGeometry*)pBind->value.strvalue)->Release ();
                    else if( pBind->valueNeedsFree )
				        delete[] (char*)pBind->value.strvalue;
                }
            }
            if (pBind->barray != NULL && pBind->type == FdoDataType_BLOB)
                ((FdoByteArray*)pBind->barray)->Release ();

            memset (pBind, sizeof(FdoRdbmsPvcBindDef), 0x00);
        }
        usedCnt = 0;
    }
    size_t size()
    {
        return usedCnt;
    }
    FdoRdbmsPvcBindDef* at(size_t idx)
    {
        return params[idx];
    }
    FdoRdbmsPvcBindDef* push_back()
    {
        if (usedCnt < params.size())
            return params.at(usedCnt++);

        FdoRdbmsPvcBindDef* item = new FdoRdbmsPvcBindDef();
        memset (item, sizeof(FdoRdbmsPvcBindDef), 0x00);
        params.push_back(item);
        usedCnt++;
        return item;
    }
};

FdoRdbmsPropBindHelper::FdoRdbmsPropBindHelper(FdoRdbmsConnection *connection) : 
    mFdoConnection (connection), // no add ref
    mBindParams (NULL),
    mHasOutParams (false)
{
}

void FdoRdbmsPropBindHelper::Clear()
{
    if (mBindParams != NULL)
        mBindParams->Clear();
}

void FdoRdbmsPropBindHelper::BindParameters(GdbiStatement* statement, std::vector< std::pair< FdoParameterValue*, FdoInt64 > >* params)
{
    size_t cntParams = (params != NULL) ? params->size() : 0;
    if (cntParams == 0)
        return;
    
    mHasOutParams = false;
    if (mBindParams == NULL)
        mBindParams = new FdoRdbmsPvdBindDef(cntParams);
    else
        mBindParams->Clear();

    char temp[32];
    GdbiCommands* cmds = statement->GetGdbiCommands();
    for (size_t idx = 0; idx < cntParams; idx++)
    {
        FdoRdbmsPvcBindDef* bind = mBindParams->push_back();
        if (bind->valueNeedsFree)
        {
            delete[] (char*)bind->value.strvalue;
            bind->value.strvalue = NULL;
            bind->valueNeedsFree = false;
        }

        cmds->alcnullind(1, &(bind->null_ind));
        cmds->set_null(bind->null_ind, 0, 0 );

        std::vector< std::pair< FdoParameterValue*, FdoInt64 > >::const_reference it = params->at(idx);
        FdoParameterValue* parval = it.first;
        int parVal = (int)parval->GetDirection();
        int paramOutType = (parVal < 2) ? (3*parVal + 1) : (parVal + (parVal-1) * (parVal-2));
        FdoPtr<FdoLiteralValue> val = parval->GetValue();
        switch (val->GetExpressionType())
        {
        case FdoExpressionItemType_DataValue:
            {
                FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
                FdoDataType dataType = dval->GetDataType();
                // Initialize the bind item
                *bind->name = L'\0';
                *bind->propertyName = L'\0';
                bind->type = dataType;
                bind->len = 64;     // Set the length to be 64
                bind->reader = NULL;
                bind->barray = NULL;
                bind->value.strvalue = NULL;
				bind->valueNeedsFree = false;
                sprintf(temp, "%d", idx+1); // Parm name are one based
                bool isNull = dval->IsNull();

                switch(dataType)
                {
                case FdoDataType_BLOB:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
				        bind->valueNeedsFree = true;
                        // this is used only when we call stored procedures
                        // I could not find a way to get/rebind out blob size values
                        // for now we just use a fixed value
                        bind->value.strvalue = new char[_SQL_MAX_VAR_LEN_BIND];
                        *((wchar_t*)bind->value.strvalue) = '\0';
                        bind->len = _SQL_MAX_VAR_LEN_BIND;
                        statement->Bind((int)(idx+1), RDBI_BLOB_ULEN, bind->len, (char *)bind->value.strvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoBLOBValue* v = static_cast<FdoBLOBValue*>(dval);
                            bind->barray = v->GetData();
                            bind->value.strvalue = (void*)bind->barray->GetData();
                            statement->Bind((int)(idx+1), RDBI_BLOB, bind->barray->GetCount(), (char*)bind->value.strvalue, NULL);
                        }
                        else
                            statement->Bind((int)(idx+1), RDBI_BLOB, 0, (char*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_DateTime:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
				        bind->valueNeedsFree = true;
                        bind->value.strvalue = new wchar_t[100];
                        *((wchar_t*)bind->value.strvalue) = '\0';
                        bind->len = 100;
                        statement->Bind((int)(idx+1), bind->len, (FdoString*)bind->value.strvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                            bind->value.strvalue = (void*)mFdoConnection->FdoToDbiTime(v->GetDateTime());
                            statement->Bind((int)(idx+1), (int)strlen((char*)bind->value.strvalue), (char*)bind->value.strvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), 0, (char*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Boolean:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.lvalue = 0;
                        statement->Bind((int)(idx+1), &bind->value.lvalue, NULL, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                            bind->value.lvalue = (int)v->GetBoolean() ? 1 : 0;
                            statement->Bind((int)(idx+1), &bind->value.lvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (int*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Byte:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.svalue = 0;
                        statement->Bind((int)(idx+1), &bind->value.svalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                            bind->value.svalue = (short)v->GetByte();
                            statement->Bind((int)(idx+1), &bind->value.svalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (short*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Decimal:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.gvalue = 0.0;
                        statement->Bind((int)(idx+1), &bind->value.gvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoDecimalValue* v = static_cast<FdoDecimalValue*>(dval);
                            bind->value.gvalue = v->GetDecimal();
                            statement->Bind((int)(idx+1), &bind->value.gvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (double*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Double:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.gvalue = 0.0;
                        statement->Bind((int)(idx+1), &bind->value.gvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                            bind->value.gvalue = v->GetDouble();
                            statement->Bind((int)(idx+1), &bind->value.gvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (double*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Int16:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.svalue = 0;
                        statement->Bind((int)(idx+1), &bind->value.svalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                            bind->value.svalue = v->GetInt16();
                            statement->Bind((int)(idx+1), &bind->value.svalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (short*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Int32:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.lvalue = 0;
                        statement->Bind((int)(idx+1), &bind->value.lvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                            bind->value.lvalue = v->GetInt32();
                            statement->Bind((int)(idx+1), &bind->value.lvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (int*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Int64:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.llvalue = 0;
                        statement->Bind((int)(idx+1), &bind->value.llvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                            bind->value.llvalue = v->GetInt64();
                            statement->Bind((int)(idx+1), &bind->value.llvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (FdoInt64*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Single:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        bind->value.fvalue = 0.0;
                        statement->Bind((int)(idx+1), &bind->value.fvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                            bind->value.fvalue = v->GetSingle();
                            statement->Bind((int)(idx+1), &bind->value.fvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), (float*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_String:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
				        bind->valueNeedsFree = true;
                        // this is used only when we call stored procedures
                        // I could not find a way to get/rebind out blob size values
                        // for now we just use a fixed value
                        bind->value.strvalue = new wchar_t[_SQL_MAX_TXT_LEN_BIND];
                        *((wchar_t*)bind->value.strvalue) = '\0';
                        bind->len = _SQL_MAX_TXT_LEN_BIND*sizeof(wchar_t);
                        statement->Bind((int)(idx+1), bind->len, (FdoString*)bind->value.strvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                            bind->value.strvalue = (void*)v->GetString();
                            bind->len = (int)(sizeof(FdoString) * (wcslen((FdoString*)bind->value.strvalue) + 1));
                            statement->Bind((int)(idx+1), bind->len, (FdoString*)bind->value.strvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), 0, (FdoString*)NULL, bind->null_ind);
                    }
                    break;
                }
            }
            break;
        case FdoExpressionItemType_GeometryValue:
            {
                if (paramOutType != _SQL_PARAM_INPUT)
                {
                    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                }
                else
                {
                    FdoGeometryValue* gval = static_cast<FdoGeometryValue*>(val.p);
                    if (!gval->IsNull())
                    {
                        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                        sprintf(temp, "%d", idx+1); // Parm name are one based
                        cmds->geom_srid_set(statement->GetQueryId(), temp, (long)it.second);
                        cmds->geom_version_set(statement->GetQueryId(), temp, mFdoConnection->GetSpatialGeometryVersion());
                        cmds->set_nnull(bind->null_ind, 0, 0 );
                        FdoPtr<FdoByteArray> ba = gval->GetGeometry();
                        FdoIGeometry* g = gf->CreateGeometryFromFgf(ba);
                        bind->type = FdoRdbmsDataType_Geometry;
                        bind->value.strvalue = (char*)g;
                        bind->len = sizeof(gval); // 4 / 8 depends of OS
                        statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), bind->len, (char*)&bind->value.strvalue, NULL);
                    }
                    else
                    {
                        bind->type = FdoRdbmsDataType_Geometry;
                        bind->value.strvalue = NULL;
                        bind->len = 0;
                        statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), (int)(sizeof(FdoIGeometry*)), (char*)&bind->value.strvalue, bind->null_ind);
                    }
                }
            }
            break;
        default:
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        }
    }
}

void FdoRdbmsPropBindHelper::BindParameters(GdbiStatement* statement, std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* params)
{
    size_t cntParams = (params != NULL) ? params->size() : 0;
    if (cntParams == 0)
        return;
    
    if (mBindParams == NULL)
        mBindParams = new FdoRdbmsPvdBindDef(cntParams);
    else
        mBindParams->Clear();

    char temp[32];
    GdbiCommands* cmds = statement->GetGdbiCommands();
    for (size_t idx = 0; idx < cntParams; idx++)
    {
        FdoRdbmsPvcBindDef* bind = mBindParams->push_back();
        cmds->alcnullind(1, &(bind->null_ind));
        cmds->set_null(bind->null_ind, 0, 0 );

        std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >::const_reference it = params->at(idx);
        FdoLiteralValue* val = it.first;
        switch (val->GetExpressionType())
        {
        case FdoExpressionItemType_DataValue:
            {
                FdoDataValue* dval = static_cast<FdoDataValue*>(val);
                FdoDataType dataType = dval->GetDataType();
                // Initialize the bind item
                *bind->name = L'\0';
                *bind->propertyName = L'\0';
                bind->type = dataType;
                bind->len = 64;     // Set the length to be 64
                bind->reader = NULL;
                bind->barray = NULL;
                bind->value.strvalue = NULL;
				bind->valueNeedsFree = false;
                sprintf(temp, "%d", idx+1); // Parm name are one based
                bool isNull = dval->IsNull();

                switch(dataType)
                {
                case FdoDataType_BLOB:
                    if (!isNull)
                    {
                        FdoBLOBValue* v = static_cast<FdoBLOBValue*>(dval);
                        bind->barray = v->GetData();
                        bind->value.strvalue = (void*)bind->barray->GetData();
                        statement->Bind((int)(idx+1), RDBI_BLOB, bind->barray->GetCount(), (char*)bind->value.strvalue, NULL);
                    }
                    else
                        statement->Bind((int)(idx+1), RDBI_BLOB, 0, (char*)NULL, bind->null_ind);
                    break;
                case FdoDataType_DateTime:
                    if (!isNull)
                    {
                        FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                        bind->value.strvalue = (void*)mFdoConnection->FdoToDbiTime(v->GetDateTime());
                        statement->Bind((int)(idx+1), (int)strlen((char*)bind->value.strvalue), (char*)bind->value.strvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), 0, (char*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Boolean:
                    if (!isNull)
                    {
                        FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                        bind->value.lvalue = (int)v->GetBoolean() ? 1 : 0;
                        statement->Bind((int)(idx+1), &bind->value.lvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (int*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Byte:
                    if (!isNull)
                    {
                        FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                        bind->value.svalue = (short)v->GetByte();
                        statement->Bind((int)(idx+1), &bind->value.svalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (short*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Decimal:
                    if (!isNull)
                    {
                        FdoDecimalValue* v = static_cast<FdoDecimalValue*>(dval);
                        bind->value.gvalue = v->GetDecimal();
                        statement->Bind((int)(idx+1), &bind->value.gvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (double*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Double:
                    if (!isNull)
                    {
                        FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                        bind->value.gvalue = v->GetDouble();
                        statement->Bind((int)(idx+1), &bind->value.gvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (double*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Int16:
                    if (!isNull)
                    {
                        FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                        bind->value.svalue = v->GetInt16();
                        statement->Bind((int)(idx+1), &bind->value.svalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (short*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Int32:
                    if (!isNull)
                    {
                        FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                        bind->value.lvalue = v->GetInt32();
                        statement->Bind((int)(idx+1), &bind->value.lvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (int*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Int64:
                    if (!isNull)
                    {
                        FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                        bind->value.llvalue = v->GetInt64();
                        statement->Bind((int)(idx+1), &bind->value.llvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (FdoInt64*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Single:
                    if (!isNull)
                    {
                        FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                        bind->value.fvalue = v->GetSingle();
                        statement->Bind((int)(idx+1), &bind->value.fvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), (float*)NULL, bind->null_ind);
                    break;
                case FdoDataType_String:
                    if (!isNull)
                    {
                        FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                        bind->value.strvalue = (void*)v->GetString();
                        bind->len = (int)(sizeof(FdoString) * (wcslen((FdoString*)bind->value.strvalue) + 1));
                        statement->Bind((int)(idx+1), bind->len, (FdoString*)bind->value.strvalue);
                    }
                    else
                        statement->Bind((int)(idx+1), 0, (FdoString*)NULL, bind->null_ind);
                    break;
                }
            }
            break;
        case FdoExpressionItemType_GeometryValue:
            {
                FdoGeometryValue* gval = static_cast<FdoGeometryValue*>(val);
                if (!gval->IsNull())
                {
                    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                    sprintf(temp, "%d", idx+1); // Parm name are one based
                    cmds->geom_srid_set(statement->GetQueryId(), temp, (long)it.second);
                    cmds->geom_version_set(statement->GetQueryId(), temp, mFdoConnection->GetSpatialGeometryVersion());
                    cmds->set_nnull(bind->null_ind, 0, 0 );
                    FdoPtr<FdoByteArray> ba = gval->GetGeometry();
                    FdoIGeometry* g = gf->CreateGeometryFromFgf(ba);
                    bind->type = FdoRdbmsDataType_Geometry;
                    bind->value.strvalue = (char*)g;
                    bind->len = sizeof(gval); // 4 / 8 depends of OS
                    statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), bind->len, (char*)&bind->value.strvalue, NULL);
                }
                else
                {
                    bind->type = FdoRdbmsDataType_Geometry;
                    bind->value.strvalue = NULL;
                    bind->len = 0;
                    statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), (int)(sizeof(FdoIGeometry*)), (char*)&bind->value.strvalue, bind->null_ind);
                }
            }
            break;
        default:
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        }
    }
}

FdoInt32 FdoRdbmsPropBindHelper::GetIntValueToRet(FdoLiteralValue* pVal)
{
    switch (pVal->GetExpressionType())
    {
    case FdoExpressionItemType_DataValue:
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(pVal);
            FdoDataType dataType = dval->GetDataType();

            switch(dataType)
            {
            case FdoDataType_BLOB:
                {
                    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                }
                break;
            case FdoDataType_DateTime:
                {
                    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                }
                break;
            case FdoDataType_Boolean:
                {
                    FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetBoolean();
                }
                break;
            case FdoDataType_Byte:
                {
                    FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetByte();
                }
                break;
            case FdoDataType_Decimal:
                {
                    FdoDecimalValue* v = static_cast<FdoDecimalValue*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetDecimal();
                }
                break;
            case FdoDataType_Double:
                {
                    FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetDouble();
                }
                break;
            case FdoDataType_Int16:
                {
                    FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetInt16();
                }
                break;
            case FdoDataType_Int32:
                {
                    FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetInt32();
                }
                break;
            case FdoDataType_Int64:
                {
                    FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetInt64();
                }
                break;
            case FdoDataType_Single:
                {
                    FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                    return v->IsNull() ? 0 : (FdoInt32)v->GetSingle();
                }
                break;
            case FdoDataType_String:
                {
                throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                }
                break;
            default:
                throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
            }
        }
        break;
    case FdoExpressionItemType_GeometryValue:
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    default:
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
}

void FdoRdbmsPropBindHelper::BindBack(size_t idx, FdoLiteralValue* pVal)
{
    size_t cntParams = (mBindParams != NULL) ? mBindParams->size() : 0;
    if (idx >= cntParams)
        return;
    FdoRdbmsPvcBindDef* bind = mBindParams->at(idx);
    GdbiCommands* cmds = mFdoConnection->GetDbiConnection()->GetGdbiCommands();

    switch (pVal->GetExpressionType())
    {
    case FdoExpressionItemType_DataValue:
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(pVal);
            FdoDataType dataType = dval->GetDataType();

            switch(dataType)
            {
            case FdoDataType_BLOB:
                {
                    FdoBLOBValue* v = static_cast<FdoBLOBValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                    {
                        FdoPtr<FdoByteArray> arr = FdoByteArray::Create((FdoByte*)bind->value.strvalue, min(_SQL_MAX_VAR_LEN_BIND, *(int*)bind->null_ind));
                        v->SetData(arr);
                    }
                }
                break;
            case FdoDataType_DateTime:
                {
                    FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetDateTime(mFdoConnection->DbiToFdoTime((wchar_t*)bind->value.strvalue));
                }
                break;
            case FdoDataType_Boolean:
                {
                    FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetBoolean(bind->value.lvalue == 1 || bind->value.lvalue == '1');
                }
                break;
            case FdoDataType_Byte:
                {
                    FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetByte((FdoByte)bind->value.svalue);
                }
                break;
            case FdoDataType_Decimal:
                {
                    FdoDecimalValue* v = static_cast<FdoDecimalValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetDecimal(bind->value.gvalue);
                }
                break;
            case FdoDataType_Double:
                {
                    FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetDouble(bind->value.gvalue);
                }
                break;
            case FdoDataType_Int16:
                {
                    FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetInt16(bind->value.svalue);
                }
                break;
            case FdoDataType_Int32:
                {
                    FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetInt32(bind->value.lvalue);
                }
                break;
            case FdoDataType_Int64:
                {
                    FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetInt64(bind->value.llvalue);
                }
                break;
            case FdoDataType_Single:
                {
                    FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetSingle(bind->value.fvalue);
                }
                break;
            case FdoDataType_String:
                {
                    FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                    if (cmds->is_null(bind->null_ind, 0))
                        v->SetNull();
                    else
                        v->SetString((wchar_t*)bind->value.strvalue);
                }
                break;
            }
        }
        break;
    case FdoExpressionItemType_GeometryValue:
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    default:
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
}