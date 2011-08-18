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

#include "FdoRdbmsSQLCommand.h"
#include "FdoRdbmsSQLDataReader.h"
#include "FdoRdbmsUtil.h"
#include "Fdo/Pvc/FdoRdbmsPropBindHelper.h"
#include "FdoRdbmsSchemaUtil.h"

#define SQL_CLEANUP \
        if ( qid != -1) \
            (void) m_DbiConnection->dbi_free(qid, INULP);


class FdoOutParamSQLDataReader: public FdoISQLDataReader
{
    std::vector<FdoParameterValue*> mParams;
    bool readerDepleted;
public:
    FdoOutParamSQLDataReader(std::vector<FdoParameterValue*> vParams)
    {
        mParams = vParams;
        readerDepleted = false;
    }
protected:
    virtual ~FdoOutParamSQLDataReader()
    {
        Close();
    }

    virtual void Dispose() { delete this; }

public:
    virtual FdoInt32 GetColumnCount()
    {
        return mParams.size();
    }
    virtual FdoString* GetColumnName(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");
        return mParams[index]->GetName();
    }
    virtual FdoInt32 GetColumnIndex(FdoString* columnName)
    {
        for (size_t idx = 0; idx < mParams.size(); idx++)
        {
            FdoParameterValue* pParVal = mParams[idx];
            if (wcscmp(mParams[idx]->GetName(), columnName) == 0)
                return idx;            
        }
        std::wstring excMsg (L"Invalid property name: ");
        excMsg.append(columnName);
        throw FdoCommandException::Create(excMsg.c_str());
    }
    virtual FdoDataType GetColumnType(FdoString* columnName)
    {
        return GetColumnType(GetColumnIndex(columnName));
    }
    virtual FdoDataType GetColumnType(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            return dval->GetDataType();
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }

    virtual FdoPropertyType GetPropertyType(FdoString* columnName)
    {
        return GetPropertyType(GetColumnIndex(columnName));
    }
    virtual FdoPropertyType GetPropertyType(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            return FdoPropertyType_DataProperty;
        }
        else if (FdoExpressionItemType_GeometryValue == val->GetExpressionType())
            return FdoPropertyType_GeometricProperty;

        throw FdoCommandException::Create(L"Invalid data type.");
    }

    virtual bool GetBoolean(FdoString* columnName)
    {
        return GetBoolean(GetColumnIndex(columnName));
    }
    virtual FdoBoolean GetBoolean(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_Boolean)
            {
                FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                return v->GetBoolean();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }

    virtual FdoByte GetByte(FdoString* columnName)
    {
        return GetByte(GetColumnIndex(columnName));
    }
    virtual FdoByte GetByte(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_Byte)
            {
                FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                return v->GetByte();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoDateTime GetDateTime(FdoString* columnName)
    {
        return GetDateTime(GetColumnIndex(columnName));
    }
    virtual FdoDateTime GetDateTime(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_DateTime)
            {
                FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                return v->GetDateTime();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual double GetDouble(FdoString* columnName)
    {
        return GetDouble(GetColumnIndex(columnName));
    }
    virtual FdoDouble GetDouble(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_Double)
            {
                FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                return v->GetDouble();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoInt16 GetInt16(FdoString* columnName)
    {
        return GetInt16(GetColumnIndex(columnName));
    }
    virtual FdoInt16 GetInt16(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_Int16)
            {
                FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                return v->GetInt16();
            }
            else if (dval->GetDataType() == FdoDataType_Int32)
            {
                FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                return (FdoInt16)v->GetInt32();
            }
            else if (dval->GetDataType() == FdoDataType_Int64)
            {
                FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                return (FdoInt16)v->GetInt64();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoInt32 GetInt32(FdoString* columnName)
    {
        return GetInt32(GetColumnIndex(columnName));
    }
    virtual FdoInt32 GetInt32(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_Int32)
            {
                FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                return v->GetInt32();
            }
            else if (dval->GetDataType() == FdoDataType_Int64)
            {
                FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                return (FdoInt32)v->GetInt64();
            }
            else if (dval->GetDataType() == FdoDataType_Int16)
            {
                FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                return v->GetInt16();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoInt64 GetInt64(FdoString* columnName)
    {
        return GetInt64(GetColumnIndex(columnName));
    }
    virtual FdoInt64 GetInt64(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_Int64)
            {
                FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                return v->GetInt64();
            }
            else if (dval->GetDataType() == FdoDataType_Int32)
            {
                FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                return v->GetInt32();
            }
            else if (dval->GetDataType() == FdoDataType_Int16)
            {
                FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                return v->GetInt16();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual float GetSingle(FdoString* columnName)
    {
        return GetSingle(GetColumnIndex(columnName));
    }
    virtual FdoFloat GetSingle(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_Single)
            {
                FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                return v->GetSingle();
            }
            else if (dval->GetDataType() == FdoDataType_Double)
            {
                FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                return (FdoFloat)v->GetDouble();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoString* GetString(FdoString* columnName)
    {
        return GetString(GetColumnIndex(columnName));
    }
    virtual FdoString* GetString(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            if (dval->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                return v->GetString();
            }
        }
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoLOBValue* GetLOB(FdoString* propertyName)
    {
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoLOBValue* GetLOB(FdoInt32 index)
    {
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName )
    {
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index)
    {
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoByteArray* GetGeometry(FdoString* columnName)
    {
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual FdoByteArray* GetGeometry(FdoInt32 index)
    {
        throw FdoCommandException::Create(L"Invalid data type.");
    }
    virtual bool IsNull(FdoString* columnName)
    {
        return IsNull(GetColumnIndex(columnName));
    }
    virtual FdoBoolean IsNull(FdoInt32 index)
    {
        if(index < 0 || index >= (FdoInt32)mParams.size())
            throw FdoCommandException::Create(L"Index out of range.");

        FdoParameterValue* pParVal = mParams[index];
        FdoPtr<FdoLiteralValue> val = pParVal->GetValue();
        if (FdoExpressionItemType_DataValue == val->GetExpressionType())
        {
            FdoDataValue* dval = static_cast<FdoDataValue*>(val.p);
            return dval->IsNull();
        }
        // we do not support geometry results yet!
        return true;
    }
    virtual bool ReadNext()
    {
        if (!readerDepleted)
        {
            readerDepleted = true;
            return true;
        }
        return false;
    }
    virtual void Close()
    {
        for (size_t idx = 0; idx < mParams.size(); idx++)
        {
            FdoParameterValue* pParVal = mParams[idx];
            FDO_SAFE_RELEASE(pParVal);
        }
        mParams.clear();
    }
};

FdoRdbmsSQLCommand::FdoRdbmsSQLCommand():
m_SqlString(NULL),
m_DbiConnection (NULL),
m_params (NULL),
m_bindHelper(NULL)
{
}

// Constructs an instance of a SQLCommand using the specified arguments.
FdoRdbmsSQLCommand::FdoRdbmsSQLCommand(FdoIConnection* connection):
m_SqlString( NULL ),
m_DbiConnection (NULL),
m_bindHelper(NULL)
{
  mFdoConnection = static_cast<FdoRdbmsConnection*>(connection);
  if( mFdoConnection )
  {
      mFdoConnection->AddRef();
      m_DbiConnection = mFdoConnection->GetDbiConnection();
  }
  m_params = FdoParameterValueCollection::Create();
}

// Default destructor for the SQLCommand command.
FdoRdbmsSQLCommand::~FdoRdbmsSQLCommand()
{
    if( m_SqlString )
        delete[] m_SqlString;

    FDO_SAFE_RELEASE(mFdoConnection);
    FDO_SAFE_RELEASE(m_params);

    delete m_bindHelper;
}

// Gets the SQL statement to be executed as a string.
const wchar_t* FdoRdbmsSQLCommand::GetSQLStatement()
{
    return m_SqlString;
}

// Sets the SQL statement to be executed as a string.
void FdoRdbmsSQLCommand::SetSQLStatement(const wchar_t* value)
{
    if( m_SqlString )
    {
        delete[] m_SqlString;
        m_SqlString = NULL;
    }

    if( value != NULL )
    {
        m_SqlString = new wchar_t[ wcslen( value ) + 1];
        wcscpy( m_SqlString, value );
    }
}

bool FdoRdbmsSQLCommand::SQLStartsWith(const wchar_t* str, const wchar_t* val, const wchar_t** lastPos)
{
    const wchar_t* strTmp = str;
    while (*strTmp == L' ' && *strTmp != L'\0') strTmp++;
    const wchar_t* valTmp = val;
    while(towupper(*strTmp++) == towupper(*valTmp++))
    {
        if (*valTmp == L'\0')
        {
            if (lastPos != NULL)
                *lastPos = strTmp;
            return true;
        }
    }
    return false;
}

// Executes the SQL statement against the connection object and returns
// the number of rows affected.
FdoInt32 FdoRdbmsSQLCommand::ExecuteNonQuery()
{
    bool                res = false;
    FdoInt32            numberOfRows = 0;


    if( m_DbiConnection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if( m_SqlString == NULL )
       throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_41, "SQL string not initialized"));

    const wchar_t* lastPos = NULL;
    GdbiConnection* gdbiConn = m_DbiConnection->GetGdbiConnection();

    bool clearSchema = false;
    bool isNotDdlStmt = true;
    if (SQLStartsWith(m_SqlString, L"CREATE", &lastPos) || SQLStartsWith(m_SqlString, L"DROP", &lastPos) || SQLStartsWith(m_SqlString, L"ALTER", &lastPos))
    {
        if (SQLStartsWith(lastPos, L"DATABASE")) // for now we cannot have parameters for this kind of select
        {
            // we create a new database
            numberOfRows = mFdoConnection->ExecuteDdlNonQuery(m_SqlString);
            isNotDdlStmt = false;
            clearSchema = true;
        }
        else if (SQLStartsWith(lastPos, L"TABLE") || SQLStartsWith(lastPos, L"VIEW"))
            clearSchema = true;
    }
    // in case schema do not change...
    if (isNotDdlStmt)
    {
        FdoString* sqlToExecute = NULL;
        std::wstring resultSQL;
        std::wstring resultSQL2;
        if (m_params->GetCount() != 0)
        {
            GdbiStatement* statement = NULL;
            try
            {
                std::vector< std::pair< FdoParameterValue*, FdoInt64 > > paramsUsed;
                if (HandleBindValues(paramsUsed, resultSQL))
                    sqlToExecute = resultSQL.c_str();
                else
                    sqlToExecute = m_SqlString;

                FdoString* sqlToExecute2 = sqlToExecute;
                FdoParameterValue* pOutPar = HandleStoredProcedureFormat(sqlToExecute, resultSQL2);
                if (pOutPar != NULL)
                {
                    // avoid adding same parameter twice
                    bool addParam = true;
                    for (size_t idx = 0; idx < paramsUsed.size(); idx++)
                    {
                        if (paramsUsed[idx].first == pOutPar)
                        {
                            addParam = false;
                            break;
                        }
                    }
                    if (addParam)
                        paramsUsed.insert(paramsUsed.begin(), std::make_pair(pOutPar, 0));

                    // in case SQL was not changed use the old value
                    if (resultSQL2.size())
                        sqlToExecute = resultSQL2.c_str();

                    mFdoConnection->StartStoredProcedure();
                }
                else
                    sqlToExecute = sqlToExecute2;

                statement = gdbiConn->Prepare(sqlToExecute);
                
                if (m_bindHelper == NULL)
                    m_bindHelper = new FdoRdbmsPropBindHelper(mFdoConnection);

                m_bindHelper->BindParameters(statement, &paramsUsed);

                numberOfRows = statement->ExecuteNonQuery();
                
                if (pOutPar != NULL && m_bindHelper->HasOuParams())
                {
                    mFdoConnection->EndStoredProcedure();
                    std::vector<FdoParameterValue*> vParams;
                    for (size_t idx = 0; idx < paramsUsed.size(); idx++)
                    {
                        FdoParameterValue* pParVal = paramsUsed.at(idx).first;
                        if (pParVal->GetDirection() != FdoParameterDirection_Input)
                        {
                            FdoPtr<FdoLiteralValue> pVal = pParVal->GetValue();
                            m_bindHelper->BindBack(idx, pVal);
                            FDO_SAFE_ADDREF(pParVal);
                            vParams.push_back(pParVal);
                        }
                    }
                    // if we have at least one output parameter just process the value
                    if (vParams.size() != 0)
                    {
                        FdoPtr<FdoLiteralValue> pVal = pOutPar->GetValue();
                        delete statement;
                        m_bindHelper->Clear();
                        if (clearSchema) // clear cached schema
                        {
                            FdoSchemaManagerP pschemaManager = m_DbiConnection->GetSchemaUtil()->GetSchemaManager();
                            pschemaManager->Clear();
                        }
                        else // we ran a stored procedure, in case caller will call Flush we need to release the schema
                            mFdoConnection->SetEnforceClearSchAtFlush(true);

                        return m_bindHelper->GetIntValueToRet(pVal);
                    }
                }
                delete statement;

                m_bindHelper->Clear();
            }
            catch(...)
            {
                delete statement;
                throw;
            }
        }
        else
            numberOfRows = gdbiConn->ExecuteNonQuery(m_SqlString);
    }

    // Do we have to clear cached schema?
    if (clearSchema)
    {
        FdoSchemaManagerP pschemaManager = m_DbiConnection->GetSchemaUtil()->GetSchemaManager();
        pschemaManager->Clear();
    }
    return numberOfRows;
}

// Executes the SQL statement against the connection object and returns
// an ISQLDataReader.
FdoISQLDataReader* FdoRdbmsSQLCommand::ExecuteReader()
{
    int                 qid = -1;
    bool                res = false;
    FdoInt32            numberOfRows = 0;

    if( m_DbiConnection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if( m_SqlString == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_41, "SQL string not initialized"));

    GdbiConnection* gdbiConn = m_DbiConnection->GetGdbiConnection();

    GdbiQueryResult *query = NULL;
    FdoString* sqlToExecute = NULL;
    std::wstring resultSQL;
    std::wstring resultSQL2;
    if (m_params->GetCount() != 0)
    {
        GdbiStatement* statement = NULL;
        try
        {
            std::vector< std::pair< FdoParameterValue*, FdoInt64 > > paramsUsed;
            if (HandleBindValues(paramsUsed, resultSQL))
                sqlToExecute = resultSQL.c_str();
            else
                sqlToExecute = m_SqlString;
            FdoString* sqlToExecute2 = sqlToExecute;

            FdoParameterValue* pOutPar = HandleStoredProcedureFormat(sqlToExecute, resultSQL2);
            if (pOutPar != NULL)
            {
                // avoid adding same parameter twice
                bool addParam = true;
                for (size_t idx = 0; idx < paramsUsed.size(); idx++)
                {
                    if (paramsUsed[idx].first == pOutPar)
                    {
                        addParam = false;
                        break;
                    }
                }
                if (addParam)
                    paramsUsed.insert(paramsUsed.begin(), std::make_pair(pOutPar, 0));

                // in case SQL was not changed use the old value
                if (resultSQL2.size())
                    sqlToExecute = resultSQL2.c_str();

                mFdoConnection->StartStoredProcedure();
            }
            else
                sqlToExecute = sqlToExecute2;

            statement = gdbiConn->Prepare(sqlToExecute);
            
            if (m_bindHelper == NULL)
                m_bindHelper = new FdoRdbmsPropBindHelper(mFdoConnection);

            m_bindHelper->BindParameters(statement, &paramsUsed);

            query = statement->ExecuteQuery();
            delete statement;

            if (m_bindHelper->HasOuParams())
            {
                mFdoConnection->EndStoredProcedure();
                std::vector<FdoParameterValue*> vParams;
                for (size_t idx = 0; idx < paramsUsed.size(); idx++)
                {
                    FdoParameterValue* pParVal = paramsUsed.at(idx).first;
                    if (pParVal->GetDirection() != FdoParameterDirection_Input)
                    {
                        FdoPtr<FdoLiteralValue> pVal = pParVal->GetValue();
                        m_bindHelper->BindBack(idx, pVal);
                        FDO_SAFE_ADDREF(pParVal);
                        vParams.push_back(pParVal);
                    }
                }
                if (vParams.size() != 0)
                {
                    // we ran a stored procedure, in case caller will call Flush we need to release the schema
                    mFdoConnection->SetEnforceClearSchAtFlush(true);
                    return new FdoOutParamSQLDataReader(vParams);
                }
            }
            m_bindHelper->Clear();
        }
        catch(...)
        {
            delete statement;
            throw;
        }
    }
    else
        query = gdbiConn->ExecuteQuery(m_SqlString);

    return new FdoRdbmsSQLDataReader(mFdoConnection , query );
}

FdoParameterValue* FdoRdbmsSQLCommand::HandleStoredProcedureFormat(const wchar_t* sql, std::wstring& resultSQL)
{
    FdoParameterValue* ret = NULL;
    for (FdoInt32 idx = 0; idx < m_params->GetCount(); idx++)
    {
        FdoPtr<FdoParameterValue> param = m_params->GetItem(idx);
        if (param->GetDirection() != FdoParameterDirection_Input)
        {
            ret = param.p;
            break;
        }
    }
    if (!ret || SQLStartsWith(sql, L"{"))
        return ret;

    resultSQL = L"{ ?= CALL ";
    resultSQL.append(sql);
    resultSQL.append(L" }");

    return ret;
}

bool FdoRdbmsSQLCommand::HandleBindValues(std::vector< std::pair< FdoParameterValue*, FdoInt64 > >& usedParameterValues, std::wstring& resultSQL)
{
    bool sqlChanged = false;
    short qState = 0x00;
    wchar_t pchar = '\0';
    const wchar_t* tmp = m_SqlString;
    const wchar_t* pStrPos = m_SqlString;
    while (*tmp != '\0')
    {
        switch(*tmp)
        {
        case '\"':
            qState = qState^0x01;
                break;
        case '\'':
            qState = qState^0x10;
                break;
        case ':':
            {
                // can this be a parameter?
                if (qState) // not in case we are inside a string/property name
                    break;
                if (IsSpecialChar(pchar))
                {
                    tmp++;
                    int cntParams = m_params->GetCount();
                    for (FdoInt32 idx = 0; idx < cntParams; idx++)
                    {
                        FdoPtr<FdoParameterValue> param = m_params->GetItem(idx);
                        const wchar_t* lastPos = NULL;
                        if (SQLStartsWith(tmp, param->GetName(), &lastPos))
                        {
                            if (!IsSpecialChar(*lastPos))
                                continue;
                            resultSQL.append(pStrPos, tmp-pStrPos-1);
                            FdoPtr<FdoLiteralValue> valParam = param->GetValue();
                            FdoInt64 srId = 0;
                            if (FdoExpressionItemType_GeometryValue == valParam->GetExpressionType())
                            {
                                if ((idx + 1) < cntParams)
                                {
                                    FdoPtr<FdoParameterValue> paramSrId = m_params->GetItem(idx+1);
                                    FdoPtr<FdoLiteralValue> paramSrIdVal = paramSrId->GetValue();
                                    if (FdoExpressionItemType_DataValue == paramSrIdVal->GetExpressionType())
                                    {
                                        FdoDataValue* dataVal = static_cast<FdoDataValue*>(paramSrIdVal.p);
                                        FdoDataType dtValType = dataVal->GetDataType();
                                        if (dtValType == FdoDataType_Int64)
                                            srId = (static_cast<FdoInt64Value*>(dataVal))->GetInt64();
                                        else if (dtValType == FdoDataType_Int32)
                                            srId = (static_cast<FdoInt32Value*>(dataVal))->GetInt32();
                                        else if (dtValType == FdoDataType_Int16)
                                            srId = (static_cast<FdoInt16Value*>(dataVal))->GetInt16();
                                    }                                    
                                }
                            }
                            usedParameterValues.push_back(std::make_pair(param.p, srId));
                            tmp = lastPos;
                            pStrPos = tmp;
                            resultSQL.append(L"?");
                            sqlChanged = true;
                            break;
                        }
                    }
                    tmp--;
                }
            }
                break;
        }
        pchar = *tmp;
        tmp++;
    }
    if (*pStrPos != '\0' && sqlChanged)
        resultSQL.append(pStrPos);

    // we should use changed SQL only in case it has been changed
    return sqlChanged;
}
