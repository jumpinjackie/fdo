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

#define SQL_CLEANUP \
        if ( qid != -1) \
            (void) m_DbiConnection->dbi_free(qid, INULP);



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

    try
    {
        const wchar_t* lastPos = NULL;
        GdbiConnection* gdbiConn = m_DbiConnection->GetGdbiConnection();

        if ((SQLStartsWith(m_SqlString, L"CREATE", &lastPos) && SQLStartsWith(lastPos, L"DATABASE")) || 
            (SQLStartsWith(m_SqlString, L"DROP", &lastPos) && SQLStartsWith(lastPos, L"DATABASE")) ||
            (SQLStartsWith(m_SqlString, L"ALTER", &lastPos) && SQLStartsWith(lastPos, L"DATABASE")))
        {
            // for now we cannot have parameters for this kind of select
            numberOfRows = mFdoConnection->ExecuteDdlNonQuery(m_SqlString);
        }
        else
        {
            FdoString* sqlToExecute = NULL;
            std::wstring resultSQL;
            if (m_params->GetCount() != 0)
            {
                GdbiStatement* statement = NULL;
                try
                {
                    std::vector< std::pair< FdoLiteralValue*, FdoInt64 > > paramsUsed;
                    if (HandleBindValues(paramsUsed, resultSQL))
                        sqlToExecute = resultSQL.c_str();
                    else
                        sqlToExecute = m_SqlString;

                    statement = m_DbiConnection->GetGdbiConnection()->Prepare(sqlToExecute);
                    
                    if (m_bindHelper == NULL)
                        m_bindHelper = new FdoRdbmsPropBindHelper(mFdoConnection);

                    m_bindHelper->BindParameters(statement, &paramsUsed);

                    numberOfRows = statement->ExecuteNonQuery();
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
    }
    catch (FdoException *ex)
    {
        FdoCommandException *exp = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
        ex->Release();
        throw exp;
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

    GdbiQueryResult *query = NULL;
    FdoString* sqlToExecute = NULL;
    std::wstring resultSQL;
    if (m_params->GetCount() != 0)
    {
        GdbiStatement* statement = NULL;
        try
        {
            std::vector< std::pair< FdoLiteralValue*, FdoInt64 > > paramsUsed;
            if (HandleBindValues(paramsUsed, resultSQL))
                sqlToExecute = resultSQL.c_str();
            else
                sqlToExecute = m_SqlString;

            statement = m_DbiConnection->GetGdbiConnection()->Prepare(sqlToExecute);
            
            if (m_bindHelper == NULL)
                m_bindHelper = new FdoRdbmsPropBindHelper(mFdoConnection);

            m_bindHelper->BindParameters(statement, &paramsUsed);

            query = statement->ExecuteQuery();
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
        query = m_DbiConnection->GetGdbiConnection()->ExecuteQuery(m_SqlString);

    return new FdoRdbmsSQLDataReader(mFdoConnection , query );
}

bool FdoRdbmsSQLCommand::HandleBindValues(std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >& usedParameterValues, std::wstring& resultSQL)
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
                    for (FdoInt32 idx = 0; idx < m_params->GetCount(); idx++)
                    {
                        FdoPtr<FdoParameterValue> param = m_params->GetItem(idx);
                        const wchar_t* lastPos = NULL;
                        if (SQLStartsWith(tmp, param->GetName(), &lastPos))
                        {
                            if (!IsSpecialChar(*lastPos))
                                continue;
                            resultSQL.append(pStrPos, tmp-pStrPos-1);
                            FdoPtr<FdoLiteralValue> lv = param->GetValue();
                            // for geometries we will need SRID otherwise zero will be used and statements will fail
                            usedParameterValues.push_back(std::make_pair(lv.p, 0));
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
