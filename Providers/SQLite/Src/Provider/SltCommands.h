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

#include "SltCommandTemplates.h"
#include "SltReader.h"
#include "SltConversionUtils.h"
#include "StringUtil.h"

//When performing bulk inserts or updates, we commit the transaction 
//once every so many features
const int BULK_OP_SIZE = 10000;

///Now featuring lasagna comments!

///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                                   DESCRIBE SCHEMA
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltDescribeSchema : public SltCommand<FdoIDescribeSchema>
{
    public:
        SltDescribeSchema(SltConnection* connection) 
            : SltCommand<FdoIDescribeSchema>(connection), m_classNames(NULL) 
                                                                { }

    protected:
        virtual ~SltDescribeSchema()                            { }

    //-------------------------------------------------------
    // FdoIDescribeSchema implementation
    //-------------------------------------------------------

    public:
        virtual FdoString*              GetSchemaName()                             { return L"Default"; }
        virtual void                    SetSchemaName(FdoString* value)             { }
        virtual FdoStringCollection*    GetClassNames()                             { return FDO_SAFE_ADDREF(m_classNames); }
        virtual void                    SetClassNames(FdoStringCollection* value)   
        { 
            FDO_SAFE_RELEASE(m_classNames); 
            m_classNames = FDO_SAFE_ADDREF(value); 
        }

        virtual FdoFeatureSchemaCollection* Execute()   
        { 
            return m_connection->DescribeSchema(m_classNames); 
        }

    private:

        FdoStringCollection* m_classNames;
};

///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                                   SELECT (Regular and Extended)
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

//helper used for passing ordering options to the connection
struct NameOrderingPair
{
    NameOrderingPair(FdoIdentifier* nm, FdoOrderingOption opt)
        : name(nm), option(opt)
    {}

    FdoIdentifier* name;
    FdoOrderingOption option;
};


class SltExtendedSelect: public SltFeatureCommand<FdoIExtendedSelect>
{
    public:

        SltExtendedSelect(SltConnection* connection)
            : SltFeatureCommand<FdoIExtendedSelect>(connection),
              m_orderingProps(NULL)
        {
            m_properties = FdoIdentifierCollection::Create();
        }

    protected:
        virtual ~SltExtendedSelect()
        {
            m_properties->Release();
            FDO_SAFE_RELEASE(m_orderingProps);
        }

    //-------------------------------------------------------
    // FdoISelect implementation
    //-------------------------------------------------------

    public:
        virtual FdoIdentifierCollection*    GetPropertyNames()              { return FDO_SAFE_ADDREF(m_properties); }
        
        //Regular select -- not scrollable
        virtual FdoIFeatureReader*          Execute() 
        {
            std::vector<NameOrderingPair> ordering;

            if (m_orderingProps)
            {
                for (int i=0; i<m_orderingProps->GetCount(); i++)
                {
                    FdoPtr<FdoIdentifier> id = m_orderingProps->GetItem(i);
                    ordering.push_back(NameOrderingPair(id.p, m_orderingOptions[id->GetName()])); 
                }
            }

            return m_connection->Select(m_className, m_filter, m_properties, false, ordering);
        }
               
        virtual FdoIdentifierCollection*    GetOrdering()       
        { 
            if (!m_orderingProps)
                m_orderingProps = FdoIdentifierCollection::Create(); 

            return FDO_SAFE_ADDREF(m_orderingProps);
        }

        //irrelevant stuff
        virtual FdoLockType                 GetLockType()                   { return FdoLockType_Exclusive;};
        virtual void                        SetLockType(FdoLockType value)  { };
        virtual FdoLockStrategy             GetLockStrategy()               { return FdoLockStrategy_All;};
        virtual void                        SetLockStrategy(FdoLockStrategy value) { };
        virtual FdoIFeatureReader*          ExecuteWithLock()               { return NULL; }
        virtual FdoILockConflictReader*     GetLockConflicts()              { return NULL; }
        virtual void                        SetOrderingOption(FdoOrderingOption option) {}
        virtual FdoOrderingOption           GetOrderingOption()             { return FdoOrderingOption_Ascending; }

    public:

        //-------------------------------------------------------
        // FdoIExtendedSelect implementation
        //-------------------------------------------------------

        virtual void SetOrderingOption(FdoString* propertyName, FdoOrderingOption option)  
        {
            if (m_orderingProps->Contains(propertyName))
                m_orderingOptions[propertyName] = option;
        }

        virtual FdoOrderingOption GetOrderingOption(FdoString* propertyName)                
        { 
            if (m_orderingProps->Contains(propertyName))
                return m_orderingOptions[propertyName];

            throw FdoCommandException::Create(L"Property is not in the order list.");
        }

        virtual void ClearOrderingOptions() 
        { 
            m_orderingProps->Clear();
            m_orderingOptions.clear();
        }

        virtual FdoIScrollableFeatureReader* ExecuteScrollable()
        {
            std::vector<NameOrderingPair> ordering;

            if (m_orderingProps)
            {
                for (int i=0; i<m_orderingProps->GetCount(); i++)
                {
                    FdoPtr<FdoIdentifier> id = m_orderingProps->GetItem(i);
                    ordering.push_back(NameOrderingPair(id.p, m_orderingOptions[id->GetName()])); 
                }
            }

            return m_connection->Select(m_className, m_filter, m_properties, true, ordering);
        }

        //-------------------------------------------------------
        // Variables
        //-------------------------------------------------------

        private:
            FdoIdentifierCollection* m_orderingProps;
            FdoIdentifierCollection* m_properties;
            std::map<std::wstring, FdoOrderingOption> m_orderingOptions;
};



///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                            SELECT (Aggregate)
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltSelectAggregates : public SltFeatureCommand<FdoISelectAggregates>
{

    public:
        SltSelectAggregates(SltConnection *connection) :
            SltFeatureCommand<FdoISelectAggregates>(connection),
        m_bDistinct(false), 
        m_eOrderingOption(FdoOrderingOption_Ascending), 
        m_grfilter(NULL)
        {
            m_grouping = FdoIdentifierCollection::Create();
            m_ordering = FdoIdentifierCollection::Create();
            m_properties = FdoIdentifierCollection::Create();
        }

    protected:
        virtual ~SltSelectAggregates(void)
        {
            m_grouping->Release();
            m_ordering->Release();
            m_properties->Release();
        }
        
    //-------------------------------------------------------
    // FdoISelectAggregates implementation
    //-------------------------------------------------------
        
    public:
        virtual FdoIdentifierCollection* GetPropertyNames () { return FDO_SAFE_ADDREF(m_properties);}
        virtual FdoIDataReader* Execute () 
        { 
            return m_connection->SelectAggregates(  m_className, 
                                                    m_properties, 
                                                    m_bDistinct, 
                                                    m_eOrderingOption, 
                                                    m_ordering, 
                                                    m_grfilter, 
                                                    m_grouping); 
        }
        virtual void                     SetDistinct( bool value )              { m_bDistinct = value; }
        virtual bool                     GetDistinct( )                         { return m_bDistinct; }
        virtual FdoIdentifierCollection* GetGrouping()                          { return FDO_SAFE_ADDREF(m_grouping); }
        virtual void                     SetGroupingFilter( FdoFilter* filter ) 
        {
            FDO_SAFE_RELEASE(m_grfilter);
            m_grfilter = FDO_SAFE_ADDREF(filter);
        } 
        virtual FdoFilter*               GetGroupingFilter( )                   { return FDO_SAFE_ADDREF(m_filter); }
        virtual FdoIdentifierCollection* GetOrdering()                          { return FDO_SAFE_ADDREF(m_ordering); }
        virtual void                     SetOrderingOption( FdoOrderingOption option) { m_eOrderingOption = option; }
        virtual FdoOrderingOption        GetOrderingOption( )                   { return m_eOrderingOption; }

    private:
        FdoIdentifierCollection*    m_properties;
        bool                        m_bDistinct;
        FdoOrderingOption           m_eOrderingOption;
        FdoIdentifierCollection*    m_ordering;
        FdoFilter*                  m_grfilter;
        FdoIdentifierCollection*    m_grouping;
};


///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                                   UPDATE
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltUpdate : public SltFeatureCommand<FdoIUpdate>
{
    public:
        SltUpdate(SltConnection* connection) 
            : SltFeatureCommand<FdoIUpdate>(connection),
              m_updateCount(0),
              m_bInTransaction(false)
        {
            m_properties = FdoPropertyValueCollection::Create();
            m_db = m_connection->GetDbWrite();
        }

    protected:
        virtual ~SltUpdate()
        {
            if (m_bInTransaction)
                m_connection->CommitTransaction();

            FDO_SAFE_RELEASE(m_properties);
        }

    //-------------------------------------------------------
    // FdoIUpdate implementation
    //-------------------------------------------------------

    public:
        virtual FdoPropertyValueCollection* GetPropertyValues() { return FDO_SAFE_ADDREF(m_properties); }
        virtual FdoInt32 Execute()
        {
            //Commit a bulk update if we reached the limit
            if (m_updateCount == BULK_OP_SIZE)
            {
                if (m_bInTransaction)
                {
                    m_connection->CommitTransaction();
                    m_bInTransaction = false;
                }

                m_updateCount = 0;
            }

            //Begin a bulk update transaction
            if (m_updateCount == 0)
            {
                if (m_connection->StartTransaction() == SQLITE_OK)
                    m_bInTransaction = true;                    
            }

            m_updateCount++;

            return m_connection->Update(m_className, 
                                        m_filter, 
                                        m_properties);
        }
        virtual FdoILockConflictReader* GetLockConflicts()      { return NULL; }

    private:
        FdoPropertyValueCollection* m_properties;
        sqlite3*                    m_db;
        int                         m_updateCount;
        bool                        m_bInTransaction;
};


///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                                   DELETE
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltDelete : public SltFeatureCommand<FdoIDelete>
{
    public:
        SltDelete(SltConnection* connection) 
            : SltFeatureCommand<FdoIDelete>(connection)
                                                                        { }

    protected:
        virtual ~SltDelete()                                            { }

    //-------------------------------------------------------
    // FdoIDelete implementation
    //-------------------------------------------------------

    public:
        virtual FdoInt32 Execute()
        {
            return m_connection->Delete(m_className, m_filter);
        }
        virtual FdoILockConflictReader* GetLockConflicts()              { return NULL; }
};


///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                                   INSERT
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

//Insert is special. We attempt to speed up inserts if the caller is cooperating.
//The contract is as follows -- as long as the caller reuses the SltInsert object,
//he is working with a compiled query and within a single transaction. Whenever
//the command object is freed, the transaction is committed.
class SltInsert : public SltCommand<FdoIInsert>
{
    public:
        SltInsert(SltConnection* connection) 
            : SltCommand<FdoIInsert>(connection)
        {
            m_className = NULL;
            m_properties = FdoPropertyValueCollection::Create();
            m_pCompiledSQL = NULL;
            m_db = m_connection->GetDbWrite();
        }

    protected:
        virtual ~SltInsert()
        {
            FlushSQL();

            //TODO: here also notify the connection that portions of the
            //spatial index are dirty and need to be reindexed

            FDO_SAFE_RELEASE(m_className);
            FDO_SAFE_RELEASE(m_properties);
        }

    //-------------------------------------------------------
    // FdoIInsert implementation
    //-------------------------------------------------------

    public:
        virtual FdoIdentifier* GetFeatureClassName()                        { return FDO_SAFE_ADDREF(m_className); }
        virtual void SetFeatureClassName(FdoIdentifier* value)
        {
            FDO_SAFE_RELEASE(m_className);
            m_className = FDO_SAFE_ADDREF(value);
            m_fcname = W2A_SLOW(m_className->GetName());

            //if the feature class changes, any precompiled SQL is no longer valid
            FlushSQL();
        }
        virtual void SetFeatureClassName(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_className);
            m_className = NULL;
            if (value)
            {
                m_className = FdoIdentifier::Create(value);
                m_fcname = W2A_SLOW(m_className->GetName());
            }

            //if the feature class changes, any precompiled SQL is no longer valid
            FlushSQL();
        }
        virtual FdoPropertyValueCollection* GetPropertyValues()             { return FDO_SAFE_ADDREF(m_properties); }
        virtual FdoBatchParameterValueCollection* GetBatchParameterValues() { return NULL; }
        virtual FdoIFeatureReader* Execute()
        {
            if (!m_pCompiledSQL)
            {
                PrepareSQL();
                m_execCount = 0;
            }
            else
            {
                size_t count = (size_t)m_properties->GetCount();

                //detect changes to the property value collection that may have been
                //done between calls to Execute(). Not recommended to do that, but it happens...
                if (count != m_propNames.size())
                {
                    FlushSQL();
                    return Execute();
                }

                for (size_t i=0; i<count; i++)
                {
                    FdoPtr<FdoPropertyValue> pv = m_properties->GetItem(i);
                    FdoPtr<FdoIdentifier> id = pv->GetName();
                    if (wcscmp(id->GetName(), m_propNames[i].c_str()) != 0)
                    {
                        FlushSQL();
                        return Execute();
                    }
                }
            }

            // in case active transaction was closed by the user reopen it
            if (!m_connection->IsTransactionStarted())
                m_connection->StartTransaction();

            sqlite3_reset(m_pCompiledSQL);

            BindPropVals(m_properties, m_pCompiledSQL);

            int rc = sqlite3_step(m_pCompiledSQL);

            if (rc != SQLITE_DONE)
            {
                //uh-oh the insert failed.
                //What should we do?
                sqlite3_finalize(m_pCompiledSQL);
                //commit the transaction we had started
                //it makes more sense to commit than rollback
                //since we probably want to keep all the inserts that we
                //did using this command before it failed.
                if (m_connection->CommitTransaction() != SQLITE_OK)
                    m_connection->RollbackTransaction();
                m_pCompiledSQL = NULL;
                throw FdoCommandException::Create(L"SQLite insert failed!");
            }

            if (++m_execCount == BULK_OP_SIZE)
            {
                rc = m_connection->CommitTransaction();
                
                if (rc == SQLITE_OK && m_connection->StartTransaction() != SQLITE_OK)
                    throw FdoCommandException::Create(L"SQLite begin transaction failed!");

                //We will accept commit failures at this point, since they are
                //not critical. It is important that the last COMMIT completes,
                //and there we do throw an exception (see FlushSQL())
                if (rc != SQLITE_OK && rc != SQLITE_BUSY)
                //    throw FdoCommandException::Create(L"SQLite commit failed!");
                    fprintf(stderr, "%ls\n", L"Transient commit SQLite failure during Insert.");

                //TODO: here also notify the connection that portions of the
                //spatial index are dirty and need to be reindexed

                m_execCount = 0;
            }

            //get the ID of the last inserted feature
            sqlite3_int64 id = sqlite3_last_insert_rowid(m_db);
            char sid[64];
            sprintf(sid, "rowid=%lld", id);

            //return the new feature
            //IMPORTANT: use a transaction-less reader so that
            //it does not commit the insert when disposed. SQLite 
            //does not do nested transactions!
            SltReader* ret = new DelayedInitReader(m_connection, NULL, m_fcname.c_str(), sid);
            return ret;
        }

    private:

        void FlushSQL()
        {
            if (m_pCompiledSQL)
            {
                int rc = m_connection->CommitTransaction();
                int rc2 = sqlite3_finalize(m_pCompiledSQL);

                if ((rc != SQLITE_OK && rc != SQLITE_BUSY) || rc2 != SQLITE_OK)
                //    throw FdoCommandException::Create(L"SQLite commit failed!");
                    fprintf(stderr, "%ls\n", L"Transient commit SQLite failure during Insert.");
            }

            m_pCompiledSQL = NULL;
            m_propNames.clear();
        }


        void PrepareSQL()
        {
            StringBuffer sb;
            sb.Append("INSERT INTO ");
            sb.AppendDQuoted(m_fcname.c_str());
            sb.Append(" (");

            for (int i=0; i<m_properties->GetCount(); i++)
            {
                if (i)
                    sb.Append(",");
                
                FdoPtr<FdoPropertyValue> pv = m_properties->GetItem(i);
                FdoPtr<FdoIdentifier> id = pv->GetName();

                m_propNames.push_back(id->GetName()); //build up a list of the property names (see Execute() for why this is needed)
                sb.AppendDQuoted(id->GetName());
            }

            //set up parametrized insert values
            sb.Append(") VALUES(");
            for (int i=0; i<m_properties->GetCount(); i++)
                (i) ? sb.Append(",?") : sb.Append("?");

            sb.Append(");");

            //begin the insert transaction
            m_connection->StartTransaction();

            //parse the SQL statement
            const char* tail = NULL;
            int rc = sqlite3_prepare_v2(m_db, sb.Data(), -1, &m_pCompiledSQL, &tail);

            if (rc != SQLITE_OK)
                throw FdoCommandException::Create(L"Failed to parse insert sql.");
        }

        FdoIdentifier*              m_className;
        FdoPropertyValueCollection* m_properties;

        std::string                 m_fcname;
        sqlite3*                    m_db;
        sqlite3_stmt*               m_pCompiledSQL;
        int                         m_execCount;
        std::vector<std::wstring>   m_propNames;
};



///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                             GET SPATIAL CONTEXTS
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltGetSpatialContexts : public SltCommand<FdoIGetSpatialContexts>
{
    public:
        SltGetSpatialContexts(SltConnection* connection)
            : SltCommand<FdoIGetSpatialContexts>(connection)
                                                                { }

    protected:
        virtual ~SltGetSpatialContexts()                        { }


    //-------------------------------------------------------
    // FdoIGetSpatialContexts implementation
    //-------------------------------------------------------
    
    public:
        virtual const bool                  GetActiveOnly()                 { return m_bActiveOnly; }
        virtual void                        SetActiveOnly(const bool value) { m_bActiveOnly = value; }
        virtual FdoISpatialContextReader*   Execute() 
        { 
            return m_connection->GetSpatialContexts(); 
        }

    private:
        bool m_bActiveOnly;
};


///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                                   SQL
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// since this command can delete/update/insert rows we need to make it in sync with the 
// transaction
// user transactions can be started only on write connection so we need to be sure this SQL
// will be executed against write connection, otherwise in case user calls rollback
// changes made will not be discarded
class SltSql : public SltCommand<FdoISQLCommand>
{
    public:
        SltSql(SltConnection* connection) 
            : SltCommand<FdoISQLCommand>(connection) 
        {
            m_pCompiledSQL = NULL;
            m_pSqlParmeterValues = NULL;
            m_db = m_connection->GetDbWrite();
        }

    protected:

        virtual ~SltSql()
        {
            FlushSQL();
            FDO_SAFE_RELEASE(m_pSqlParmeterValues);
        }
        virtual FdoString* GetSQLStatement()
        {
            if (m_sql.size() == 0)
                m_sql = A2W_SLOW(m_sb.Data());
            return m_sql.c_str();
        }
        virtual void SetSQLStatement(FdoString* value)          
        {
            m_sb.Reset();
            m_sb.Append(value ? value : L"");
            FlushSQL();
        }
        // in this case we can keep the stmt in case user repeats the operation
        virtual FdoInt32 ExecuteNonQuery()
        {
            if (m_sb.Length() == 0)
                throw FdoCommandException::Create(L"Invalid empty SQL statement.");

            int count = 0;
            int rc = SQLITE_OK;

            sqlite3_stmt* pStmt = m_pCompiledSQL;
            if (NULL != pStmt)
            {
                sqlite3_reset(pStmt);
                BindPropVals(m_pSqlParmeterValues, pStmt);
            }
            else
            {
                if (m_pSqlParmeterValues != NULL && m_pSqlParmeterValues->GetCount() != 0)
                {
                    //parse the SQL statement
                    const char* tail = NULL;
                    if (sqlite3_prepare_v2(m_db, m_sb.Data(), -1, &m_pCompiledSQL, &tail) != SQLITE_OK)
                        throw FdoCommandException::Create(L"Failed to parse SQL statement.");

                    pStmt = m_pCompiledSQL;
                    BindPropVals(m_pSqlParmeterValues, m_pCompiledSQL);
                }
                else
                    pStmt = m_connection->GetCachedParsedStatement(m_sb.Data(), m_db);
            }

            while ((rc = sqlite3_step(pStmt)) == SQLITE_ROW);
            if( rc == SQLITE_DONE )
                count = sqlite3_changes(m_db);

            if (NULL == m_pCompiledSQL)
                m_connection->ReleaseParsedStatement(m_sb.Data(), pStmt);

            if (rc == SQLITE_DONE)
                return count;
            else 
                throw FdoCommandException::Create(L"Failed to execute sql command.");
        }

        // in this case we cannot keep the stmt since is used by the reader
        virtual FdoISQLDataReader* ExecuteReader()
        {
            if (m_sb.Length() == 0)
                throw FdoCommandException::Create(L"Invalid empty SQL statement.");
            
            sqlite3_stmt* pStmt = m_connection->GetCachedParsedStatement(m_sb.Data(), m_db);
            if( m_pSqlParmeterValues != NULL && m_pSqlParmeterValues->GetCount() != 0 )
                BindPropVals(m_pSqlParmeterValues, pStmt );
            return new SltReader(m_connection, pStmt, false);
        }

        virtual FdoParameterValueCollection* GetParameterValues()
        { 
            if( m_pSqlParmeterValues == NULL )
                m_pSqlParmeterValues = FdoParameterValueCollection::Create();

            return FDO_SAFE_ADDREF(m_pSqlParmeterValues);
        }

    private:
        void FlushSQL()
        {
            if (m_pCompiledSQL)
            {
                if (sqlite3_finalize(m_pCompiledSQL) != SQLITE_OK)
                    fprintf(stderr, "%ls\n", L"Transient commit SQLite failure during execute.");
                m_pCompiledSQL = NULL;
            }
            m_sql = L"";
        }
    private:
        sqlite3_stmt*                    m_pCompiledSQL;
        sqlite3*                         m_db;
        // use this for speed performance
        StringBuffer                     m_sb;
        // this will be empty most of the time except when user calls getSQL
        std::wstring                     m_sql;
        FdoParameterValueCollection*     m_pSqlParmeterValues;
};

///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                              CREATE SPATIAL CONTEXT
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltCreateSpatialContext : public SltCommand<FdoICreateSpatialContext>
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:

    SltCreateSpatialContext(SltConnection* connection)
        : SltCommand<FdoICreateSpatialContext>(connection)
    {
    }

protected:

    virtual ~SltCreateSpatialContext()
    {
    }


    //-------------------------------------------------------
    // FdoICreateSpatialContext implementation
    //-------------------------------------------------------

public:
    virtual FdoString*      GetName()                               { return m_scName.c_str(); }
    virtual void            SetName(FdoString* value)               { m_scName = value; }
    virtual FdoString*      GetDescription()                        { return m_description.c_str(); }
    virtual void            SetDescription(FdoString* value)        { m_description = value; }
    virtual void            SetGeometricProperty(FdoPropertyDefinition *value) { }
    virtual void            SetDimensionality(int value)            { }
    virtual int             GetDimensionality()                     { return 0; }
    virtual FdoString*      GetCoordinateSystem()                   { return m_coordSysName.c_str(); }
    virtual void            SetCoordinateSystem(FdoString* value)   { m_coordSysName = value; }
    virtual FdoString*      GetCoordinateSystemWkt()                { return m_coordSysWkt.c_str(); }
    virtual void            SetCoordinateSystemWkt(FdoString* value){ m_coordSysWkt = value; }
    virtual FdoSpatialContextExtentType GetExtentType()             { return FdoSpatialContextExtentType_Dynamic; }
    virtual void            SetExtentType(FdoSpatialContextExtentType value) { }
    virtual FdoByteArray*   GetExtent()                             { return NULL; }
    virtual void            SetExtent(FdoByteArray* value)          { }
    virtual const double    GetXYTolerance()                        { return 0; }
    virtual void            SetXYTolerance(const double value)      { }
    virtual const double    GetZTolerance()                         { return 0; }
    virtual void            SetZTolerance(const double value)       { }
    virtual const bool      GetUpdateExisting()                     { return m_updateExisting; }
    virtual void            SetUpdateExisting(const bool value)     { m_updateExisting = value; }
    virtual void            Execute()
    {
        StringBuffer sb;
        sb.Append("INSERT INTO spatial_ref_sys (sr_name,auth_name,srtext) VALUES(");

        if (m_scName.empty())
            sb.Append("NULL");
        else
            sb.AppendSQuoted(m_scName.c_str());

        sb.Append(",");

        if (m_coordSysName.empty())
            sb.Append("NULL");
        else
            sb.AppendSQuoted(m_coordSysName.c_str());

        sb.Append(",");

        if (m_coordSysWkt.empty())
            sb.Append("NULL");
        else
            sb.AppendSQuoted(m_coordSysWkt.c_str());

        sb.Append(");");

        char* zerr = NULL;
        int rc = sqlite3_exec(m_connection->GetDbWrite(), sb.Data(), NULL, NULL, &zerr);

        if (rc)
        {
            FdoCommandException::Create(L"Failed to create spatial context.");
        }

        m_connection->UpdateClassesWithInvalidSC();
    }

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

private:
    std::wstring                    m_scName;
    std::wstring                    m_description;
    std::wstring                    m_coordSysName;
    std::wstring                    m_coordSysWkt;
    
    FdoByteArray*                   m_extent;
    FdoInt32                        m_extentLength;

    bool                            m_updateExisting;
};



///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                              CREATE DATA STORE
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltCreateDataStore: public SltCommand<FdoICreateDataStore>
{
public:

    SltCreateDataStore(SltConnection* connection)
        : SltCommand<FdoICreateDataStore>(connection)
    {
    }

public:

    //-------------------------------------------------------
    // FdoICreateDataStore implementation
    //-------------------------------------------------------

    virtual FdoIDataStorePropertyDictionary* GetDataStoreProperties() 
    { 
        return (FdoIDataStorePropertyDictionary *)(FDO_SAFE_ADDREF(m_connection));
    }

    virtual void Execute()
    {
        m_connection->CreateDatabase();
    }
};


///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///                              APPLY SCHEMA
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class SltApplySchema : public SltCommand<FdoIApplySchema>
{

public:
    SltApplySchema(SltConnection* connection)
        : SltCommand<FdoIApplySchema>(connection),
          m_schema(NULL),
          m_ignoreStates(false)                                             { }

    virtual ~SltApplySchema()                                               { FDO_SAFE_RELEASE(m_schema); }
   
    //-------------------------------------------------------------------------
    // FdoIApplySchema
    //-------------------------------------------------------------------------

    virtual FdoFeatureSchema*           GetFeatureSchema()                  { return FDO_SAFE_ADDREF(m_schema); }
    virtual void                        SetFeatureSchema(FdoFeatureSchema* value) 
    {
        FDO_SAFE_RELEASE(m_schema);
        m_schema = FDO_SAFE_ADDREF(value); 
    }

    virtual FdoPhysicalSchemaMapping*   GetPhysicalMapping()                { return NULL; }
    
    virtual void                        SetPhysicalMapping(FdoPhysicalSchemaMapping* value) 
                                                                            { }
    
    virtual FdoBoolean                  GetIgnoreStates()                   { return m_ignoreStates; } 
    
    virtual void                        SetIgnoreStates( FdoBoolean ignoreStates ) 
                                                                            { m_ignoreStates = ignoreStates;}
    
    virtual void Execute()
    {
        m_connection->ApplySchema(m_schema, m_ignoreStates);
    }

private:
    FdoFeatureSchema* m_schema;
    bool m_ignoreStates;
};

