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
#include "SltProvider.h"
#include "SltTransaction.h"


FdoString* SltTransaction::AddSavePoint(FdoString* suggestName)
{
    if(suggestName == NULL || wcslen(suggestName) == 0)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_14_NULLSTRING)));
    }

    FdoStringP realName = suggestName;
    int postfixCount = 0;
    while(true)
    {
        postfixCount++;
        if(m_savepoints->IndexOf(realName, false) == -1)
        {
            break;
        }
        realName = FdoStringP::Format(L"%ls%d", (FdoString*)realName, postfixCount);
    }

    FdoStringP sql = FdoStringP::Format(L"SAVEPOINT %ls", (FdoString*)realName);
    FdoPtr<FdoISQLCommand> savepointSQL = (FdoISQLCommand*) m_conn->CreateCommand(FdoCommandType_SQLCommand);
    savepointSQL->SetSQLStatement(sql);
    savepointSQL->ExecuteNonQuery();

    m_savepoints->Add(realName);
    return  (FdoString*)realName;
}

void SltTransaction::ReleaseSavePoint(FdoString* savePointName)
{
    if(savePointName == NULL || wcslen(savePointName) == 0)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_14_NULLSTRING)));
    }

    FdoInt32 index = m_savepoints->IndexOf(savePointName);
    if(index == -1)
    {	
        FdoStringP errorMessage = FdoException::NLSGetMessage(FDO_NLSID(FDO_187_SAVEPOINT_NOT_EXIST), savePointName);
        throw  FdoException::Create(errorMessage);
    }

    FdoStringP sql = FdoStringP::Format(L"RELEASE %ls", savePointName);
    FdoPtr<FdoISQLCommand> savepointSQL = (FdoISQLCommand*) m_conn->CreateCommand(FdoCommandType_SQLCommand);
    savepointSQL->SetSQLStatement(sql);
    savepointSQL->ExecuteNonQuery();

    //Remove all save points defined after it.
    for(int i = m_savepoints->GetCount() - index - 1; i >= 0 ; i--)
    {
        m_savepoints->RemoveAt(index + i);
    }
}

void SltTransaction::Rollback(FdoString* savePointName)
{
    if(savePointName == NULL || wcslen(savePointName) == 0)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_14_NULLSTRING)));
    }

    FdoInt32 index = m_savepoints->IndexOf(savePointName);
    if(index == -1)
    {	
        FdoStringP errorMessage = FdoException::NLSGetMessage(FDO_NLSID(FDO_187_SAVEPOINT_NOT_EXIST), savePointName);
        throw  FdoException::Create(errorMessage);
    }
    
    FdoStringP sql = FdoStringP::Format(L"ROLLBACK TO %ls", savePointName);
    FdoPtr<FdoISQLCommand> savepointSQL = (FdoISQLCommand*) m_conn->CreateCommand(FdoCommandType_SQLCommand);
    savepointSQL->SetSQLStatement(sql);
    savepointSQL->ExecuteNonQuery();

    //Remove all save points defined after it.
    for(int i = m_savepoints->GetCount() - index - 1; i > 0 ; i--)
    {
        m_savepoints->RemoveAt(index + i);
    }
}
