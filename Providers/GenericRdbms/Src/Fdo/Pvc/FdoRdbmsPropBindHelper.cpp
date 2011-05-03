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
            if (NULL != pBind->value.strvalue && pBind->type != FdoDataType_BLOB)
            {
                if (pBind->type == FdoRdbmsDataType_Geometry)
			    {
                    ((FdoIGeometry*)pBind->value.strvalue)->Release ();
				    pBind->value.strvalue = NULL;
			    }
                else if( pBind->valueNeedsFree )
			    {
				    delete[] (char*)pBind->value.strvalue;
				    pBind->value.strvalue = NULL;
				    pBind->valueNeedsFree = false;
			    }
            }
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

FdoRdbmsPropBindHelper::FdoRdbmsPropBindHelper(FdoRdbmsConnection *connection)
    : mFdoConnection (connection), // no add ref
    mBindParams (NULL)
{
}

void FdoRdbmsPropBindHelper::Clear()
{
    if (mBindParams != NULL)
        mBindParams->Clear();
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
                bind->value.strvalue = NULL;
				bind->valueNeedsFree = false;
                sprintf(temp, "%d", idx+1); // Parm name are one based
                bool isNull = dval->IsNull();

                switch(dataType)
                {
                case FdoDataType_BLOB:
                    if (!isNull)
                    {
                        // TODO
                    }
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
                    statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), (int)(sizeof(FdoIGeometry)), (char*)&bind->value.strvalue, bind->null_ind);
                }
                else
                {
                    bind->type = FdoRdbmsDataType_Geometry;
                    bind->value.strvalue = NULL;
                    bind->len = 0;
                    statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), (int)(sizeof(FdoIGeometry)), (char*)&bind->value.strvalue, bind->null_ind);
                }
            }
            break;
        default:
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        }
    }
}
