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


class SltConnection;


class SltTransaction : public FdoITransaction
{
public: 
    SltTransaction(SltConnection* conn)
        : m_conn(conn),
          m_needsRollback(true)
    {
        FDO_SAFE_ADDREF(m_conn);
        m_savepoints = FdoStringCollection::Create();
    }
protected:
    virtual ~SltTransaction()
    {
        if (m_needsRollback)
            m_conn->RollbackTransaction(true);
        FDO_SAFE_RELEASE(m_conn);
    }

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------
    void Dispose() { delete this; }

    //-------------------------------------------------------
    // FdoITransaction declaration
    //-------------------------------------------------------
    virtual FdoIConnection* GetConnection()     { return FDO_SAFE_ADDREF(m_conn); }
    virtual void Commit()                       { m_needsRollback = false; m_conn->CommitTransaction(true); m_savepoints->Clear();}
    virtual void Rollback()                     { m_needsRollback = false; m_conn->RollbackTransaction(true); m_savepoints->Clear();}
    virtual FdoString* AddSavePoint(FdoString* suggestName);
    virtual void ReleaseSavePoint(FdoString* savePointName);
    virtual void Rollback(FdoString* savePointName);
private:
    SltConnection*  m_conn;
	FdoStringsP		m_savepoints;
    bool            m_needsRollback;
};

