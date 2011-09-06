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
#define _MAX_STR_INT64 32

typedef struct {
        int     type; // FdoDataType
		union {
			void		*strvalue;
			double		gvalue;
			float		fvalue;
			FdoInt64    llvalue;
			int			lvalue;
			short		svalue;
			char		cvalue;
		} value;
		bool			 valueNeedsFree;
        int				 len;
        GDBI_NI_TYPE	 *null_ind;
        FdoByteArray     *barray;
} FdoRdbmsBindStrDef;

class FdoRdbmsPvdBindDef
{
    std::vector<FdoRdbmsBindStrDef*> params;
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
        std::vector<FdoRdbmsBindStrDef*>::iterator it;
        for (it = params.begin(); it < params.end(); it++)
        {
            FdoRdbmsBindStrDef* pBind = *it;
            if (pBind->null_ind)
                free(pBind->null_ind);

            if (NULL != pBind->value.strvalue && pBind->type != FdoDataType_BLOB)
            {
                if (pBind->type == FdoRdbmsDataType_Geometry)
                    ((FdoIGeometry*)pBind->value.strvalue)->Release ();
                else if( pBind->valueNeedsFree )
			        delete[] (char*)pBind->value.strvalue;
            }
            if (pBind->barray != NULL && pBind->type == FdoDataType_BLOB)
                ((FdoByteArray*)pBind->barray)->Release ();

            delete pBind;
        }
        params.clear();
    }
    void Clear()
    {
        for (size_t idx = 0; idx < usedCnt; idx++)
        {
            FdoRdbmsBindStrDef* pBind = params.at(idx);
            // the BLOB value was not allocated
            if (NULL != pBind->value.strvalue && pBind->type == FdoRdbmsDataType_Geometry)
            {
                ((FdoIGeometry*)pBind->value.strvalue)->Release ();
                pBind->value.strvalue = NULL;
                pBind->len = 0;
            }
            if (pBind->barray != NULL && pBind->type == FdoDataType_BLOB)
            {
                ((FdoByteArray*)pBind->barray)->Release ();
                pBind->barray = NULL;
            }
        }
        usedCnt = 0;
    }
    size_t size()
    {
        return usedCnt;
    }
    FdoRdbmsBindStrDef* at(size_t idx)
    {
        return params[idx];
    }
    FdoRdbmsBindStrDef* push_back()
    {
        if (usedCnt < params.size())
            return params.at(usedCnt++);

        FdoRdbmsBindStrDef* item = new FdoRdbmsBindStrDef();
        memset (item, sizeof(FdoRdbmsBindStrDef), 0x00);
        item->type = -2;
        params.push_back(item);
        usedCnt++;
        return item;
    }
    bool EnsureSize(FdoRdbmsBindStrDef* item, size_t sz)
    {
        if (item->type != FdoDataType_String)
            FreeResources(item);

        if ((size_t)item->len < sz)
        {
            char* ptr = (char*)item->value.strvalue;
            delete[] ptr;
            item->len = 2*sz;
            item->value.strvalue = new char[2*sz];
            item->valueNeedsFree = true;
            return true;
        }
        return false;
    }
    void FreeResources(FdoRdbmsBindStrDef* item)
    {
        if (NULL != item->value.strvalue && item->type != FdoDataType_BLOB)
        {
            if (item->type == FdoRdbmsDataType_Geometry)
                ((FdoIGeometry*)item->value.strvalue)->Release ();
            else if (item->valueNeedsFree)
		        delete[] (char*)item->value.strvalue;
        }
        if (item->barray != NULL && item->type == FdoDataType_BLOB)
            ((FdoByteArray*)item->barray)->Release ();

        item->len = 0;
        item->barray = NULL;
        item->value.strvalue = NULL;
        item->valueNeedsFree = false;
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

    GdbiCommands* cmds = statement->GetGdbiCommands();
    for (size_t idx = 0; idx < cntParams; idx++)
    {
        FdoRdbmsBindStrDef* bind = mBindParams->push_back();
        if (!bind->null_ind)
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
                bool isNull = dval->IsNull();
                switch(dataType)
                {
                case FdoDataType_BLOB:
                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        if (bind->type != FdoDataType_String)
                            mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_String;
                        
                        // this is used only when we call stored procedures
                        // I could not find a way to get/rebind out blob size values
                        // for now we just use a fixed value
                        mBindParams->EnsureSize(bind, _SQL_MAX_VAR_LEN_BIND);
                        statement->Bind((int)(idx+1), RDBI_BLOB_ULEN, bind->len, (char*)bind->value.strvalue, bind->null_ind, paramOutType);
                    }
                    else
                    {
                        mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_BLOB;

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
                    if (bind->type != FdoDataType_String)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_String;

                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        if (!cmds->SupportsUnicode())
                        {
                            mBindParams->EnsureSize(bind, 100);
                            statement->Bind((int)(idx+1), bind->len, (const char*)bind->value.strvalue, bind->null_ind, paramOutType);
                        }
                        else
                        {
                            mBindParams->EnsureSize(bind, 100* sizeof(wchar_t));
                            statement->Bind((int)(idx+1), bind->len, (FdoString*)bind->value.strvalue, bind->null_ind, paramOutType);
                        }
                    }
                    else
                    {
                        if (!isNull)
                        {
                            FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                            const char* dateval = mFdoConnection->FdoToDbiTime(v->GetDateTime());
                            size_t ln = strlen(dateval)+1;
                            mBindParams->EnsureSize(bind, ln);
                            strcpy((char*)bind->value.strvalue, dateval);
                            statement->Bind((int)(idx+1), (int)ln, (char*)bind->value.strvalue);
                        }
                        else
                            statement->Bind((int)(idx+1), 0, (char*)NULL, bind->null_ind);
                    }
                    break;
                case FdoDataType_Boolean:
                    if (bind->type != FdoDataType_Boolean)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Boolean;

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
                    if (bind->type != FdoDataType_Byte)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Byte;

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
                    if (bind->type != FdoDataType_Decimal)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Decimal;

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
                    if (bind->type != FdoDataType_Double)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Double;

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
                    if (bind->type != FdoDataType_Int16)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Int16;

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
                    if (bind->type != FdoDataType_Int32)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Int32;

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
                    if (cmds->SupportsInt64Binding())
                    {
                        if (bind->type != FdoDataType_Int64)
                            mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_Int64;
                    }
                    else
                    {
                        if (bind->type != FdoDataType_String)
                            mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_String;
                    }

                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        if (!cmds->SupportsInt64Binding())
                        {
                            mBindParams->EnsureSize(bind, _MAX_STR_INT64*sizeof(wchar_t));
                            statement->Bind((int)(idx+1), bind->len, (FdoString*)bind->value.strvalue, bind->null_ind, paramOutType);
                        }
                        else
                        {
                            bind->value.llvalue = 0;
                            statement->Bind((int)(idx+1), &bind->value.llvalue, bind->null_ind, paramOutType);
                        }
                    }
                    else
                    {
                        if (!cmds->SupportsInt64Binding())
                        {
                            if (!isNull)
                            {
                                FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
				                if (!cmds->SupportsUnicode())
                                {
                                    mBindParams->EnsureSize(bind, _MAX_STR_INT64);
#ifdef _WIN32
                        			_i64toa_s(v->GetInt64(), (char*)bind->value.strvalue, _MAX_STR_INT64, 10);
#else
		                            snprintf((char*)bind->value.strvalue, _MAX_STR_INT64, "%lld", (long long int)v->GetInt64());
#endif
                                    statement->Bind((int)(idx+1), strlen((char*)bind->value.strvalue) + sizeof(char), (char*)bind->value.strvalue);
                                }
                                else
                                {
                                    mBindParams->EnsureSize(bind, _MAX_STR_INT64*sizeof(wchar_t));
#ifdef _WIN32
                        			_i64tow_s(v->GetInt64(), (wchar_t*)bind->value.strvalue, _MAX_STR_INT64, 10);
#else
		                            swprintf((wchar_t*)bind->value.strvalue, _MAX_STR_INT64, L"%lld", (long long int)v->GetInt64());
#endif
                                    statement->Bind((int)(idx+1), wcslen((wchar_t*)bind->value.strvalue) + sizeof(wchar_t), (wchar_t*)bind->value.strvalue);
                                }
                            }
                            else
                                statement->Bind((int)(idx+1), (FdoInt32*)NULL, bind->null_ind);
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
                    }
                    break;
                case FdoDataType_Single:
                    if (bind->type != FdoDataType_Single)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Single;

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
                    if (bind->type != FdoDataType_String)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_String;

                    if (paramOutType != _SQL_PARAM_INPUT)
                    {
                        mHasOutParams = true;
                        // this is used only when we call stored procedures
                        // I could not find a way to get/rebind out blob size values
                        // for now we just use a fixed value
                        if (!cmds->SupportsUnicode())
                        {
                            mBindParams->EnsureSize(bind, _SQL_MAX_VAR_LEN_BIND);
                            statement->Bind((int)(idx+1), bind->len, (const char*)bind->value.strvalue, bind->null_ind, paramOutType);
                        }
                        else
                        {
                            mBindParams->EnsureSize(bind, _SQL_MAX_VAR_LEN_BIND*sizeof(wchar_t));
                            statement->Bind((int)(idx+1), bind->len, (FdoString*)bind->value.strvalue, bind->null_ind, paramOutType);
                        }
                    }
                    else
                    {
                        if (!cmds->SupportsUnicode())
                        {
                            if (!isNull)
                            {
                                FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                                FdoString* pStrVal = v->GetString();
                                size_t wlen = wcslen(pStrVal);
                                int mbslen = (int) wlen * 4 + 1;
                                mBindParams->EnsureSize(bind, mbslen);
                                _ut_utf8_from_unicode(pStrVal, wlen, (char*)bind->value.strvalue, mbslen);
                                statement->Bind((int)(idx+1), mbslen, (const char*)bind->value.strvalue);
                            }
                            else
                                statement->Bind((int)(idx+1), 0, (const char*)NULL, bind->null_ind);
                        }
                        else
                        {
                            if (!isNull)
                            {
                                FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                                FdoString* strval = v->GetString();
                                size_t wlen = (sizeof(FdoString) * (wcslen(strval) + 1));
                                mBindParams->EnsureSize(bind, wlen);
                                wcscpy((wchar_t*)bind->value.strvalue, strval);
                                statement->Bind((int)(idx+1), wlen, (FdoString*)bind->value.strvalue);
                            }
                            else
                                statement->Bind((int)(idx+1), 0, (FdoString*)NULL, bind->null_ind);
                        }
                    }
                    break;
                }
            }
            break;
        case FdoExpressionItemType_GeometryValue:
            {
                mBindParams->FreeResources(bind);
                if (paramOutType != _SQL_PARAM_INPUT)
                {
                    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                }
                else
                {
                    FdoGeometryValue* gval = static_cast<FdoGeometryValue*>(val.p);
                    if (!gval->IsNull())
                    {
                        char temp[32];
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
    GdbiCommands* cmds = statement->GetGdbiCommands();
    BindParameters(cmds, statement->GetQueryId(), params);
}

void FdoRdbmsPropBindHelper::BindParameters(GdbiCommands* cmds, int id, std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* params)
{
    size_t cntParams = (params != NULL) ? params->size() : 0;
    if (cntParams == 0)
        return;
    
    if (mBindParams == NULL)
        mBindParams = new FdoRdbmsPvdBindDef(cntParams);
    else
        mBindParams->Clear();

    char temp[32];
    for (size_t idx = 0; idx < cntParams; idx++)
    {
        FdoRdbmsBindStrDef* bind = mBindParams->push_back();
        if (!bind->null_ind)
            cmds->alcnullind(1, &(bind->null_ind));
        cmds->set_nnull(bind->null_ind, 0, 0 );        

        std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >::const_reference it = params->at(idx);
        FdoLiteralValue* val = it.first;
        switch (val->GetExpressionType())
        {
        case FdoExpressionItemType_DataValue:
            {
                FdoDataValue* dval = static_cast<FdoDataValue*>(val);
                FdoDataType dataType = dval->GetDataType();
                sprintf(temp, "%d", idx+1); // Parm name are one based
                bool isNull = dval->IsNull();
                if (isNull)
                    cmds->set_null(bind->null_ind, 0, 0 );
                switch(dataType)
                {
                case FdoDataType_BLOB:
                    mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_BLOB;

                    if (!isNull)
                    {
                        FdoBLOBValue* v = static_cast<FdoBLOBValue*>(dval);
                        bind->barray = v->GetData();
                        bind->value.strvalue = (void*)bind->barray->GetData();
                    }
                    cmds->bind(id, temp, RDBI_BLOB, sizeof(void*), (char*)bind->value.strvalue, bind->null_ind);
                    break;
                case FdoDataType_DateTime:
                    if (bind->type != FdoDataType_String)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_String;

                    mBindParams->EnsureSize(bind, 100);
                    if (!isNull)
                    {
                        FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                        strcpy((char*)bind->value.strvalue, mFdoConnection->FdoToDbiTime(v->GetDateTime()));
                    }
                    cmds->bind(id, temp, RDBI_STRING, bind->len, (char*)bind->value.strvalue, bind->null_ind);
                    break;
                case FdoDataType_Boolean:
                    if (bind->type != FdoDataType_Boolean)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Boolean;

                    if (!isNull)
                    {
                        FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                        bind->value.lvalue = (int)v->GetBoolean() ? 1 : 0;
                    }
                    cmds->bind(id, temp, RDBI_INT, sizeof(int), (char*)&bind->value.lvalue, bind->null_ind);
                    break;
                case FdoDataType_Byte:
                        if (bind->type != FdoDataType_Byte)
                            mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_Byte;

                    if (!isNull)
                    {
                        FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                        bind->value.svalue = (short)v->GetByte();
                    }
                    cmds->bind(id, temp, RDBI_SHORT, sizeof(short), (char*)&bind->value.svalue, bind->null_ind);
                    break;
                case FdoDataType_Decimal:
                    if (bind->type != FdoDataType_Decimal)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Decimal;

                    if (!isNull)
                    {
                        FdoDecimalValue* v = static_cast<FdoDecimalValue*>(dval);
                        bind->value.gvalue = v->GetDecimal();
                    }
                    cmds->bind(id, temp, RDBI_DOUBLE, sizeof(double), (char*)&bind->value.gvalue, bind->null_ind);
                    break;
                case FdoDataType_Double:
                    if (bind->type != FdoDataType_Double)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Double;

                    if (!isNull)
                    {
                        FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                        bind->value.gvalue = v->GetDouble();
                    }
                    cmds->bind(id, temp, RDBI_DOUBLE, sizeof(double), (char*)&bind->value.gvalue, bind->null_ind);
                    break;
                case FdoDataType_Int16:
                    if (bind->type != FdoDataType_Int16)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Int16;

                    if (!isNull)
                    {
                        FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                        bind->value.svalue = v->GetInt16();
                    }
                    cmds->bind(id, temp, RDBI_SHORT, sizeof(short), (char*)&bind->value.svalue, bind->null_ind);
                    break;
                case FdoDataType_Int32:
                    if (bind->type != FdoDataType_Int32)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Int32;

                    if (!isNull)
                    {
                        FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                        bind->value.lvalue = v->GetInt32();
                    }
                    cmds->bind(id, temp, RDBI_INT, sizeof(int), (char*)&bind->value.lvalue, bind->null_ind);
                    break;
                case FdoDataType_Int64:
                    if (!cmds->SupportsInt64Binding())
                    {
                        if (bind->type != FdoDataType_String)
                            mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_String;

                        FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
		                if (!cmds->SupportsUnicode())
                        {
                            mBindParams->EnsureSize(bind, _MAX_STR_INT64);
                            if (!isNull)
                            {
#ifdef _WIN32
                    			_i64toa_s(v->GetInt64(), (char*)bind->value.strvalue, _MAX_STR_INT64, 10);
#else
	                            sprintf((char*)bind->value.strvalue, _MAX_STR_INT64, "%lld", (long long int)v->GetInt64());
#endif
                            }
                            cmds->bind(id, temp, RDBI_STRING, bind->len, (char*)bind->value.strvalue, bind->null_ind);
                        }
                        else
                        {
                            mBindParams->EnsureSize(bind, _MAX_STR_INT64*sizeof(wchar_t));
                            if (!isNull)
                            {
#ifdef _WIN32
                			    _i64tow_s(v->GetInt64(), (wchar_t*)bind->value.strvalue, _MAX_STR_INT64, 10);
#else
                                swprintf((wchar_t*)bind->value.strvalue, _MAX_STR_INT64, L"%lld", (long long int)v->GetInt64());
#endif
                            }
                            cmds->bind(id, temp, RDBI_WSTRING, bind->len, (char*)bind->value.strvalue, bind->null_ind);
                        }
                    }
                    else
                    {
                        if (bind->type != FdoDataType_Int64)
                            mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_Int64;

                        if (!isNull)
                        {
                            FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                            bind->value.llvalue = v->GetInt64();
                        }
                        cmds->bind(id, temp, RDBI_LONGLONG, sizeof(FdoInt64), (char*)&bind->value.llvalue, bind->null_ind);
                    }
                    break;
                case FdoDataType_Single:
                    if (bind->type != FdoDataType_Single)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_Single;

                    if (!isNull)
                    {
                        FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                        bind->value.fvalue = v->GetSingle();
                    }
                    cmds->bind(id, temp, RDBI_FLOAT, sizeof(float), (char*)&bind->value.fvalue, bind->null_ind);
                    break;
                case FdoDataType_String:
                    if (bind->type != FdoDataType_String)
                        mBindParams->FreeResources(bind);
                    bind->type = FdoDataType_String;

                    if (!cmds->SupportsUnicode())
                    {
                        mBindParams->EnsureSize(bind, _SQL_MAX_TXT_LEN_BIND);
                        if (!isNull)
                        {
                            FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                            FdoString* pStrVal = v->GetString();
                            size_t wlen = wcslen(pStrVal);
                            int mbslen = (int) wlen * 4 + 1;
                            mBindParams->EnsureSize(bind, mbslen);
                            _ut_utf8_from_unicode(pStrVal, wlen, (char*)bind->value.strvalue, mbslen);
                        }
                        cmds->bind(id, temp, RDBI_STRING, bind->len, (char*)bind->value.strvalue, bind->null_ind);
                    }
                    else
                    {
                        mBindParams->EnsureSize(bind, _SQL_MAX_TXT_LEN_BIND*sizeof(wchar_t));
                        if (!isNull)
                        {
                            FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                            FdoString* strval = v->GetString();
                            size_t wlen = sizeof(FdoString)*(wcslen(strval) + 1);
                            mBindParams->EnsureSize(bind, wlen);
                            wcscpy((wchar_t*)bind->value.strvalue, strval);
                        }
                        cmds->bind(id, temp, RDBI_WSTRING, bind->len, (char*)bind->value.strvalue, bind->null_ind);
                    }
                    break;
                }
            }
            break;
        case FdoExpressionItemType_GeometryValue:
            {
                mBindParams->FreeResources(bind);
                bind->type = FdoRdbmsDataType_Geometry;
                bind->value.strvalue = NULL;
                FdoGeometryValue* gval = static_cast<FdoGeometryValue*>(val);
                if (!gval->IsNull())
                {
                    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                    sprintf(temp, "%d", idx+1); // Parm name are one based
                    cmds->geom_srid_set(id, temp, (long)it.second);
                    cmds->geom_version_set(id, temp, mFdoConnection->GetSpatialGeometryVersion());
                    cmds->set_nnull(bind->null_ind, 0, 0 );
                    FdoPtr<FdoByteArray> ba = gval->GetGeometry();
                    FdoIGeometry* g = gf->CreateGeometryFromFgf(ba);
                    bind->type = FdoRdbmsDataType_Geometry;
                    bind->value.strvalue = (char*)g;
                    bind->len = sizeof(gval); // 4 / 8 depends of OS
                }
                cmds->bind(id, temp, (int)(RDBI_GEOMETRY), (int)(sizeof(FdoIGeometry*)), (char*)&bind->value.strvalue, bind->null_ind);
            }
            break;
        default:
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        }
    }
}

void FdoRdbmsPropBindHelper::BindValues(GdbiCommands* cmds, int id, std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* params)
{
    size_t cntParams = (params != NULL) ? params->size() : 0;
    if (cntParams == 0)
        return;
    
    if (mBindParams == NULL)
        mBindParams = new FdoRdbmsPvdBindDef(cntParams);
    else
        mBindParams->Clear();

    char temp[32];
    for (size_t idx = 0; idx < cntParams; idx++)
    {
        FdoRdbmsBindStrDef* bind = mBindParams->at(idx);
        std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >::const_reference it = params->at(idx);
        FdoLiteralValue* val = it.first;
        cmds->set_null(bind->null_ind, 0, 0 );
        switch (val->GetExpressionType())
        {
        case FdoExpressionItemType_DataValue:
            {
                FdoDataValue* dval = static_cast<FdoDataValue*>(val);
                FdoDataType dataType = dval->GetDataType();
                if (dval->IsNull())
                    continue;
                cmds->set_nnull(bind->null_ind, 0, 0 );

                switch(dataType)
                {
                case FdoDataType_BLOB:
                    {
                        mBindParams->FreeResources(bind);
                        bind->type = FdoDataType_BLOB;
                        sprintf(temp, "%d", idx+1);
                        FdoBLOBValue* v = static_cast<FdoBLOBValue*>(dval);
                        bind->barray = v->GetData();
                        bind->value.strvalue = (void*)bind->barray->GetData();
                        cmds->bind(id, temp, RDBI_BLOB, bind->barray->GetCount(), (char*)bind->value.strvalue, bind->null_ind);
                    }
                    break;
                case FdoDataType_DateTime:
                    {
                        FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                        strcpy((char*)bind->value.strvalue, mFdoConnection->FdoToDbiTime(v->GetDateTime()));
                    }
                    break;
                case FdoDataType_Boolean:
                    {
                        FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                        bind->value.lvalue = (int)v->GetBoolean() ? 1 : 0;
                    }
                    break;
                case FdoDataType_Byte:
                    {
                        FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                        bind->value.svalue = (short)v->GetByte();
                    }
                    break;
                case FdoDataType_Decimal:
                    {
                        FdoDecimalValue* v = static_cast<FdoDecimalValue*>(dval);
                        bind->value.gvalue = v->GetDecimal();
                    }
                    break;
                case FdoDataType_Double:
                    {
                        FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                        bind->value.gvalue = v->GetDouble();
                    }
                    break;
                case FdoDataType_Int16:
                    {
                        FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                        bind->value.svalue = v->GetInt16();
                    }
                    break;
                case FdoDataType_Int32:
                    {
                        FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                        bind->value.lvalue = v->GetInt32();
                    }
                    break;
                case FdoDataType_Int64:
                    {
                        FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                        if (!cmds->SupportsInt64Binding())
                        {
		                    if (!cmds->SupportsUnicode())
                            {
#ifdef _WIN32
                			    _i64toa_s(v->GetInt64(), (char*)bind->value.strvalue, _MAX_STR_INT64, 10);
#else
                                sprintf((char*)bind->value.strvalue, _MAX_STR_INT64, "%lld", (long long int)v->GetInt64());
#endif
                            }
                            else
                            {
#ifdef _WIN32
                    			_i64tow_s(v->GetInt64(), (wchar_t*)bind->value.strvalue, _MAX_STR_INT64, 10);
#else
	                            swprintf((wchar_t*)bind->value.strvalue, _MAX_STR_INT64, L"%lld", (long long int)v->GetInt64());
#endif
                            }
                        }
                        else
                            bind->value.llvalue = v->GetInt64();
                    }
                    break;
                case FdoDataType_Single:
                    {
                        FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                        bind->value.fvalue = v->GetSingle();
                    }
                    break;
                case FdoDataType_String:
                    if (!cmds->SupportsUnicode())
                    {
                        FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                        FdoString* pStrVal = v->GetString();
                        size_t wlen = wcslen(pStrVal);
                        int mbslen = (int) wlen * 4 + 1;
                        if(mBindParams->EnsureSize(bind, mbslen))
                        {
                            // we re-allocated the memory, so we have to re-bind
                            sprintf(temp, "%d", idx+1);
                            cmds->bind(id, temp, RDBI_STRING, mbslen, (char*)bind->value.strvalue, bind->null_ind);
                        }
                        _ut_utf8_from_unicode(pStrVal, wlen, (char*)bind->value.strvalue, mbslen);
                    }
                    else
                    {
                        FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                        FdoString* strval = v->GetString();
                        size_t wlen = sizeof(FdoString)*(wcslen(strval) + 1);
                        if (mBindParams->EnsureSize(bind, wlen))
                        {
                            // we re-allocated the memory, so we have to re-bind
                            sprintf(temp, "%d", idx+1);
                            cmds->bind(id, temp, RDBI_WSTRING, wlen, (char*)bind->value.strvalue, bind->null_ind);
                        }
                        wcscpy((wchar_t*)bind->value.strvalue, strval);                        
                    }
                    break;
                }
            }
            break;
        case FdoExpressionItemType_GeometryValue:
            {
                mBindParams->FreeResources(bind);
                FdoGeometryValue* gval = static_cast<FdoGeometryValue*>(val);
                if (!gval->IsNull())
                {
                    cmds->set_nnull(bind->null_ind, 0, 0 );
                    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                    FdoPtr<FdoByteArray> ba = gval->GetGeometry();
                    FdoIGeometry* g = gf->CreateGeometryFromFgf(ba);
                    bind->type = FdoRdbmsDataType_Geometry;
                    bind->value.strvalue = (char*)g;
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
    FdoRdbmsBindStrDef* bind = mBindParams->at(idx);
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
                    if (!cmds->is_null(bind->null_ind, 0))
                    {
                        if (!cmds->SupportsUnicode())
                            v->SetDateTime(mFdoConnection->DbiToFdoTime((char*)bind->value.strvalue));
                        else
                            v->SetDateTime(mFdoConnection->DbiToFdoTime((wchar_t*)bind->value.strvalue));
                    }
                    else
                        v->SetNull();
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
