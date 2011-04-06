#ifndef _GDBISTATEMENT_
#define _GDBISTATEMENT_
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
 *
 */
#ifdef _WIN32
#pragma once
#endif

#include "GdbiQueryResult.h"
#include "Inc/Geometry/IGeometry.h"
#include "GdbiQueryIdentifier.h"

class GdbiStatement
{
    friend class GdbiConnection;
private:
    GdbiCommands*     m_pGdbiCommands;
    mutable char      buffer[16];  // Used to convert integer indexes to a string indexes
	GdbiQueryIdentifier	*m_QueryId;


    GdbiStatement(GdbiCommands* command, int qid);

public:

     ~GdbiStatement(void);
    //
    // Bind overrides
    int Bind( int parmIndex, int size, const char* szValue, GDBI_NI_TYPE *nullInd = NULL);

    int Bind( int parmIndex, int size, const wchar_t* szValue, GDBI_NI_TYPE *nullInd = NULL );

	int Bind( int parmIndex, short *sValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, int *nValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, double *dValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, float *fValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, FdoInt64 *i64Value, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, FdoIGeometry* gValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, FdoDateTime *dValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind(
        int parmIndex,       /* column/expression position               */
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE  *null_ind   /* pointer to null indicator variables      */
            );

    int Bind(
        const char* parmName,       /* bind variable name               */
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE  *null_ind   /* pointer to null indicator variables      */
            );

    int BindNull( int parmIndex );
    // End bind functions

    int ParmCount( );

    int ExecuteNonQuery( );
	int Free();

    GdbiQueryResult*  ExecuteQuery( );

	bool SupportsInt64Binding();

	int set_array_size(int arraySize);

	int geom_srid_set(
			int				parmIndex,
			long			srid );

    char * GetGeomInfoFromFgf( long srid, FdoByteArray * fgf );

};

class GdbiCacheParam
{
    union
    {
        FdoInt16    vInt16;
        FdoInt32    vInt32;
        FdoInt64    vInt64;
	    float       vFloat;
	    double      vDouble;
	    char*       vChar;
	    FdoString*  vString;
    }Data;
    FdoDataType mType;
    FdoInt64    mNullInd;

public:
    GdbiCacheParam()
    {
        mType = FdoDataType_Int32;
        Data.vInt64 = 0;
        mNullInd = -1;
    }
    GdbiCacheParam(FdoDataValue* pData, GdbiStatement* statement, int idx)
    {
        BindParameter(pData, statement, idx);
    }
    void BindParameter(FdoDataValue* pData, GdbiStatement* statement, int idx)
    {
        mType = pData->GetDataType();
        mNullInd = -1;
        switch(mType)
        {
        case FdoDataType_BLOB:
            {
            //FdoLOBValue* v = static_cast<FdoLOBValue*>(pData);
            //FdoPtr<FdoByteArray> ba = v->GetData();
            }
            break;
        case FdoDataType_DateTime:
            {
                FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(pData);
                FdoDateTime dtm = v->GetDateTime();
                // convert
            }
            break;
        case FdoDataType_Boolean:
            if (!pData->IsNull())
            {
                FdoBooleanValue* v = static_cast<FdoBooleanValue*>(pData);
                Data.vInt32 = v->GetBoolean() ? 1 : 0;
                statement->Bind(idx, &Data.vInt32);
            }
            else
                statement->Bind(idx, (int*)NULL, &mNullInd);
            break;
        case FdoDataType_Byte:
            if (!pData->IsNull())
            {
                FdoByteValue* v = static_cast<FdoByteValue*>(pData);
                Data.vInt16 = v->GetByte();
                statement->Bind(idx, &Data.vInt16);
            }
            else
                statement->Bind(idx, (short*)NULL, &mNullInd);
            break;
        case FdoDataType_Decimal:
            if (!pData->IsNull())
            {
                FdoDecimalValue* v = static_cast<FdoDecimalValue*>(pData);
                Data.vDouble = v->GetDecimal();
                statement->Bind(idx, &Data.vDouble);
            }
            else
                statement->Bind(idx, (double*)NULL, &mNullInd);
            break;
        case FdoDataType_Double:
            if (!pData->IsNull())
            {
                FdoDoubleValue* v = static_cast<FdoDoubleValue*>(pData);
                Data.vDouble = v->GetDouble();
                statement->Bind(idx, &Data.vDouble);
            }
            else
                statement->Bind(idx, (double*)NULL, &mNullInd);
            break;
        case FdoDataType_Int16:
            if (!pData->IsNull())
            {
                FdoInt16Value* v = static_cast<FdoInt16Value*>(pData);
                Data.vInt16 = v->GetInt16();
                statement->Bind(idx, &Data.vInt16);
            }
            else
                statement->Bind(idx, (short*)NULL, &mNullInd);
            break;
        case FdoDataType_Int32:
            if (!pData->IsNull())
            {
                FdoInt32Value* v = static_cast<FdoInt32Value*>(pData);
                Data.vInt32 = v->GetInt32();
                statement->Bind(idx, &Data.vInt32);
            }
            else
                statement->Bind(idx, (int*)NULL, &mNullInd);
            break;
        case FdoDataType_Int64:
            if (!pData->IsNull())
            {
                FdoInt64Value* v = static_cast<FdoInt64Value*>(pData);
                Data.vInt64 = v->GetInt64();
                statement->Bind(idx, &Data.vInt64);
            }
            else
                statement->Bind(idx, (FdoInt64*)NULL, &mNullInd);
            break;
        case FdoDataType_Single:
            if (!pData->IsNull())
            {
                FdoSingleValue* v = static_cast<FdoSingleValue*>(pData);
                Data.vFloat = v->GetSingle();
                statement->Bind(idx, &Data.vFloat);
            }
            else
                statement->Bind(idx, (float*)NULL, &mNullInd);
            break;
        case FdoDataType_String:
            if (!pData->IsNull())
            {
                FdoStringValue* v = static_cast<FdoStringValue*>(pData);
                Data.vString = v->GetString();
                if (Data.vString != NULL)
                {
                    int sz = sizeof(FdoString) * (wcslen(Data.vString) + 1);
                    statement->Bind(idx, sz, Data.vString);
                }
                else
                    statement->Bind(idx, 0, (FdoString*)NULL, &mNullInd);
            }
            else
                statement->Bind(idx, 0, (FdoString*)NULL, &mNullInd);
            break;
        }
    }
    ~GdbiCacheParam()
    {
    }
};


#endif //_GDBISTATEMENT_
