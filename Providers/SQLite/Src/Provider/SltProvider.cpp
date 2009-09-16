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
#include "slt.h"
#include "SltProvider.h"
#include "SltCapabilities.h"
#include "SltCommands.h"
#include "SltSpatialContextReader.h"
#include "SltReader.h"
#include "SltConversionUtils.h"
#include "SltMetadata.h"
#include "SltExprExtensions.h"
#include "SltGeomUtils.h"
#include "SltQueryTranslator.h"
#include "RowidIterator.h"
#include "StringUtil.h"
#include "SltQueryTranslator.h"
#include "FdoCommonSchemaUtil.h"
#include "SQLiteSchemaMergeContext.h"
#include "SltTransaction.h"

#ifndef _MSC_VER
#include "SpatialIndex.h"
#else
#include "DiskSpatialIndex.h"
#endif

#include "SpatialIndexDescriptor.h"

//FDO entry point
extern "C"
SLT_API FdoIConnection* CreateConnection()
{
    return new SltConnection();
}


static bool IsMetadataTable(const char* table)
{
    if (sqlite3StrICmp(table, "sqlite_master") == 0)
        return true;

    if (sqlite3StrICmp(table, "geometry_columns") == 0)
        return true;

    if (sqlite3StrICmp(table, "spatial_ref_sys") == 0)
        return true;

    if (sqlite3StrICmp(table, "fdo_columns") == 0)
        return true;

    return false;
}

std::map<int, std::string> g_fdo2sql_map;

SltConnection::SltConnection() : m_refCount(1)
{
    if (g_fdo2sql_map.empty())
    {
        g_fdo2sql_map[FdoDataType_BLOB] =       "BLOB";
        g_fdo2sql_map[FdoDataType_CLOB] =       "TEXT";
        g_fdo2sql_map[FdoDataType_Boolean] =    "INTEGER";
        g_fdo2sql_map[FdoDataType_Byte] =       "INTEGER";
        g_fdo2sql_map[FdoDataType_DateTime] =   "TEXT";
        g_fdo2sql_map[FdoDataType_Decimal] =    "REAL";
        g_fdo2sql_map[FdoDataType_Double] =     "REAL";
        g_fdo2sql_map[FdoDataType_Int16] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Int32] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Int64] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Single] =     "REAL";
        g_fdo2sql_map[FdoDataType_String] =     "TEXT";
    }

    m_dbRead = NULL;
    m_dbWrite = NULL;
    m_pSchema = NULL;
    m_mProps = new std::map<std::wstring, std::wstring>();
    m_connState = FdoConnectionState_Closed;

    m_caps = new SltCapabilities();

    m_bUseFdoMetadata = false;
    m_bHasFdoMetadata = false;
    m_transactionState = SQLiteActiveTransactionType_None;
    m_cSupportsDetGeomType = -1;
    m_wkbBuffer = NULL;
    m_wkbBufferLen = 0;
    m_updateHookEnabled = false;
    m_changesAvailable = false;
}

SltConnection::~SltConnection()
{
    Close();
    delete m_mProps;
    delete m_caps;
}

//----------------------------------------------------------------
//
//                          FdoIConnection
//
//----------------------------------------------------------------

FdoString* SltConnection::GetConnectionString()
{
    m_connStr = L"";

    std::map<std::wstring, std::wstring>::iterator iter;
    
    for (iter = m_mProps->begin(); iter != m_mProps->end(); iter++)
    {
        m_connStr += iter->first;
        m_connStr += L"=";
        m_connStr += iter->second;
        m_connStr += L";";
    }

    return m_connStr.c_str();
}

void SltConnection::SetConnectionString(FdoString* value)
{
    m_mProps->clear();
    
    //parse the connection properties from the connection string
    size_t len = wcslen(value);
    wchar_t* valcpy = (wchar_t*)alloca(sizeof(wchar_t) * (len + 1));
    wcscpy(valcpy, value);

#ifdef _WIN32
    wchar_t* token = wcstok(valcpy, L";");
#else
    wchar_t* ptr = NULL; //for Linux wcstok
    wchar_t* token = wcstok(valcpy, L";", &ptr);
#endif

    //tokenize input string into separate connection properties
    while (token) 
    {
        //token is in the form "<prop_name>=<prop_value>"
        //look for the = sign
        wchar_t* eq = wcschr(token, L'=');

        if (eq)
        {
            *eq = L'\0';
    
                //pass empty string instead of null. This means the null prop value
                //exception is delayed up to until the user attempts to open the 
                //connection. This gives the opportunity to fix the connection
                //string before opening the connection.
            if (*(eq+1) == L'\0')
                SetProperty(token, L"");
            else
                SetProperty(token, eq+1);
        }
    
    #ifdef _WIN32
            token = wcstok(NULL, L";");
    #else
            token = wcstok(NULL, L";", &ptr);
    #endif
    }
}

//creates a new database, based on the filename stored in the
//connection string and connection property dictionary.
void SltConnection::CreateDatabase()
{
    if (m_dbRead || m_connState != FdoConnectionState_Closed)
    {
        throw FdoCommandException::Create(L"Cannot create data store while connection is in open state.");
    }

    const wchar_t* dsw = GetProperty(PROP_NAME_FILENAME);
    
    std::string file = W2A_SLOW(dsw);

    sqlite3* tmpdb = NULL;

    //create the database
    //TODO: this will also work if the database exists -- in which 
    //case it will open it.
    int rc;
    if((rc = sqlite3_open(file.c_str(), &tmpdb)) != SQLITE_OK )
    {
        std::wstring err = std::wstring(L"Failed to open or create: ") + dsw;
        throw FdoCommandException::Create(err.c_str(), rc);
    }

    //first things first -- set big page size for better
    //performance. Must be done on an empty db to have effect.
    rc = sqlite3_exec(tmpdb, "PRAGMA page_size=32768;", NULL, NULL, NULL);
    
    //create the spatial_ref_sys table
    //Note the sr_name field is not in the spec, we are adding it in order to 
    //match fdo feature class geometry properties to rows in the spatial_ref_sys table.
    //This is because geometry properties use a string spatial context association.
    const char* srs_sql = "CREATE TABLE spatial_ref_sys"
                          "(srid INTEGER PRIMARY KEY,"
                          "sr_name TEXT, "
                          "auth_name TEXT,"
                          "auth_srid INTEGER,"
                          "srtext TEXT "
                          ");";

    char* zerr = NULL;
    rc = sqlite3_exec(tmpdb, srs_sql, NULL, NULL, &zerr);

    //create the geometry_columns table
    const char* gc_sql = "CREATE TABLE geometry_columns "
                         "(f_table_name TEXT,"
                         "f_geometry_column TEXT,"
                         "geometry_type INTEGER,"
                         "geometry_dettype INTEGER,"
                         "coord_dimension INTEGER,"
                         "srid INTEGER,"
                         "geometry_format TEXT);";  

    int rc2 = sqlite3_exec(tmpdb, gc_sql, NULL, NULL, &zerr);

    //Check if we should create the FDO metadata table as well
    dsw = GetProperty(PROP_NAME_FDOMETADATA);
    int rc3 = 0;

    if (dsw && _wcsicmp(dsw, L"true") == 0)
    {
        const char* fc_sql = "CREATE TABLE fdo_columns "
                             "(f_table_name TEXT,"
                             "f_column_name TEXT,"
                             "f_column_desc TEXT,"
                             "fdo_data_type INTEGER,"
                             "fdo_data_details INTEGER," // flag usage nullable = 0x02, system = 0x01
                             "fdo_data_length INTEGER,"
                             "fdo_data_precision INTEGER,"
                             "fdo_data_scale INTEGER);";  

        rc3 = sqlite3_exec(tmpdb, fc_sql, NULL, NULL, &zerr);
    }

    //close the database -- CreateDataStore itself does not 
    //open the FDO connection, subsequent call to Open() does.
    sqlite3_close(tmpdb);

    if (rc || rc2 || rc3)
    {
        throw FdoCommandException::Create(L"Failed to create SQLite database.", rc ? rc : rc2 ? rc2 : rc3);
    }
}

FdoConnectionState SltConnection::Open()
{
    const wchar_t* dsw = GetProperty(PROP_NAME_FILENAME);
    
    std::string file = W2A_SLOW(dsw);

#ifdef _WIN32
    if (_waccess(dsw, 0) == -1)
        throw FdoConnectionException::Create(L"File does not exist!");
#else 
    if (access(file.c_str(), 0) == -1)
        throw FdoConnectionException::Create(L"File does not exist!");
#endif

    const wchar_t* sUseMeta = GetProperty(PROP_NAME_FDOMETADATA);

    if (sUseMeta != NULL && _wcsicmp(sUseMeta, L"true") == 0)
        m_bUseFdoMetadata = true;

    sqlite3_initialize();

    //We will use two connections to the database -- one for reading and one for writing.
    //This will help us with concurrent reads and writes (to different tables).
    //If we use the same SQLite connection, we will get problems with transaction nesting
    //due to interleaved reads and writes.

    //Allow sharing of memory caches between the reading and writing connections
    int rc = sqlite3_enable_shared_cache(1);

    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to enable shared cache.\n");
    //Open the Read connection
    if( (rc = sqlite3_open(file.c_str(), &m_dbRead)) != SQLITE_OK )
    {
        m_dbRead = NULL;
        std::wstring err = std::wstring(L"Failed to open ") + dsw;
        throw FdoConnectionException::Create(err.c_str(), rc);
    }

    rc = sqlite3_exec(m_dbRead, "PRAGMA read_uncommitted=1;", NULL, NULL, NULL);
    
    //Open the Write connection
    if( (rc = sqlite3_open(file.c_str(), &m_dbWrite)) != SQLITE_OK )
    {
        sqlite3_close(m_dbRead);
        m_dbRead = m_dbWrite = NULL;
        std::wstring err = std::wstring(L"Failed to open ") + dsw;
        throw FdoConnectionException::Create(err.c_str(), rc);
    }

    //Register the extra SQL functions we would like to support
    RegisterExtensions(m_dbRead);
    RegisterExtensions(m_dbWrite);

    //check if an FDO metadata table existsm, in case the caller asked
    //for us to use it.
    if (m_bUseFdoMetadata)
    {
        const char* tables_sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='fdo_columns';";
        sqlite3_stmt* pstmt = NULL;
        const char* pzTail = NULL;
        if (sqlite3_prepare_v2(m_dbRead, tables_sql, -1, &pstmt, &pzTail) == SQLITE_OK)
        {
            while (sqlite3_step(pstmt) == SQLITE_ROW)
            {
                m_bHasFdoMetadata = true;
                break;
            }
        }

        sqlite3_finalize(pstmt);
    }

    m_connState = FdoConnectionState_Open;

    return m_connState;
}

void SltConnection::Close()
{
    //free the spatial indexes
    for (SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.begin();
         iter != m_mNameToSpatialIndex.end(); iter++)
    {
        delete iter->second;
        free(iter->first); //was allocated using strdup
    }

    m_mNameToSpatialIndex.clear();

    //clear the cached schema metadata
    for (MetadataCache::iterator iter = m_mNameToMetadata.begin();
         iter != m_mNameToMetadata.end(); iter++)
    {
         delete iter->second;
         free(iter->first); //it was created via strdup, must use free()
    }

    m_mNameToMetadata.clear();
    
    switch(m_transactionState)
    {
    case SQLiteActiveTransactionType_Internal:
        CommitTransaction();
        break;
    case SQLiteActiveTransactionType_User:
        RollbackTransaction(true); // enforce a rollback
        break;
    }

    ClearQueryCache();
    
    delete[] m_wkbBuffer;
    m_wkbBuffer = NULL;
    m_wkbBufferLen = 0;

    if (m_dbRead && sqlite3_close(m_dbRead) != SQLITE_BUSY)
        m_dbRead = NULL;

    if (m_dbWrite && sqlite3_close(m_dbWrite) != SQLITE_BUSY)
        m_dbWrite = NULL;
    
    FDO_SAFE_RELEASE(m_pSchema);

    m_connState = FdoConnectionState_Closed;

    m_updateHookEnabled = false;
    m_changesAvailable = false;
}

FdoICommand* SltConnection::CreateCommand(FdoInt32 commandType)
{
    switch(commandType)
    {
        case FdoCommandType_DescribeSchema :        return new SltDescribeSchema(this);
        case FdoCommandType_GetSpatialContexts :    return new SltGetSpatialContexts(this);
        case FdoCommandType_Select :                return new SltExtendedSelect(this);
        case FdoCommandType_SelectAggregates :      return new SltSelectAggregates(this);
        case FdoCommandType_Update:                 return new SltUpdate(this);
        case FdoCommandType_Delete:                 return new SltDelete(this);
        case FdoCommandType_Insert:                 return new SltInsert(this);
        case FdoCommandType_SQLCommand:             return new SltSql(this);
        case FdoCommandType_CreateDataStore:        return new SltCreateDataStore(this);
        case FdoCommandType_CreateSpatialContext:   return new SltCreateSpatialContext(this);
        case FdoCommandType_ApplySchema:            return new SltApplySchema(this);
        case FdoCommandType_ExtendedSelect:         return new SltExtendedSelect(this);
        default: break;
    }
    
    return NULL;
}

//-------------------------------------------------------
// FdoIConnectionPropertyDictionary implementation
//-------------------------------------------------------

FdoString** SltConnection::GetPropertyNames(FdoInt32& count)
{
    //NOTE: We advertise (at least for testing purposes) the FDO metadata flag
    static const wchar_t* PROP_NAMES[] = {PROP_NAME_FILENAME, PROP_NAME_FDOMETADATA};
    count = 2;
    return (const wchar_t**)PROP_NAMES;
}

FdoString* SltConnection::GetProperty(FdoString* name)
{
    std::map<std::wstring, std::wstring>::iterator val = m_mProps->find(name);

    if (val == m_mProps->end())
        return NULL;

    return val->second.c_str();
}

void SltConnection::SetProperty(FdoString* name, FdoString* value)
{
    // check the connection
    if (GetConnectionState() != FdoConnectionState_Closed)
        throw FdoConnectionException::Create(L"Attempt to set property on open connection");

    if (value)
        (*m_mProps)[name] = value;
    else
        (*m_mProps)[name] = L"";
}

FdoString* SltConnection::GetPropertyDefault(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
        return VAL_FALSE;
    return L"";
}

bool SltConnection::IsPropertyRequired(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILENAME) == 0)
        return true;

    return false;
}

bool SltConnection::IsPropertyProtected(FdoString* name)
{
    return false;
}

bool SltConnection::IsPropertyFileName(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILENAME) == 0)
        return true;

    return false;
}

bool SltConnection::IsPropertyFilePath(FdoString* name)
{
    return false;
}

bool SltConnection::IsPropertyDatastoreName(FdoString* name)
{
    return false;
}

bool SltConnection::IsPropertyEnumerable(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
        return true;
    return false;
}

FdoString** SltConnection::EnumeratePropertyValues(FdoString* name, FdoInt32& count)
{
    static const wchar_t* PROP_VALUES[] = {VAL_TRUE, VAL_FALSE};
    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
    {
        count = 2;
        return PROP_VALUES;
    }
    return NULL;
}

FdoString* SltConnection::GetLocalizedName(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILENAME) == 0)
        return PROP_NAME_FILENAME;

    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
        return L"Fdo Enabled";

    return NULL;
}


//--------------------------------------------------------------------------
//
//        Command helpers -- implementation of command functionality
//
//--------------------------------------------------------------------------



//Maps sqlite table descriptions to an FDO schema
FdoFeatureSchemaCollection* SltConnection::DescribeSchema(FdoStringCollection* classNames, bool makeACopy)
{
    //TODO: Take into account the classNames collection -- not required, but
    //can potentially speed things up in some use cases

    if (m_pSchema)
    {
        //We need to clone the schema, because the caller may modify it and mess us up.
        return makeACopy ? FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(m_pSchema, NULL) : FDO_SAFE_ADDREF(m_pSchema);
    }

    if (!m_dbRead)
        return NULL;

    m_pSchema = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"Default", L"");
    m_pSchema->Add(schema.p);
    
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    
    //first, make a list of all tables that can be FDO feature classes
    std::vector<std::string> tables;

    const char* tables_sql = "SELECT name FROM sqlite_master WHERE type=? ORDER BY name;";
    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    int rc;
    if ((rc = sqlite3_prepare_v2(m_dbRead, tables_sql, -1, &pstmt, &pzTail)) == SQLITE_OK)
    {
        sqlite3_bind_text(pstmt, 1, "table", 5, SQLITE_STATIC);
        while (sqlite3_step(pstmt) == SQLITE_ROW)
            tables.push_back((const char*)sqlite3_column_text(pstmt, 0));
    }
    else
    {
        const char* err = sqlite3_errmsg(m_dbRead);
        if (err != NULL)
            throw FdoException::Create(A2W_SLOW(err).c_str(), rc);
        else
            throw FdoException::Create(L"Failed to get all tables that can be FDO feature classes.", rc);
    }

    sqlite3_reset(pstmt);
    sqlite3_bind_text(pstmt, 1, "view", 4, SQLITE_STATIC);
    while (sqlite3_step(pstmt) == SQLITE_ROW)
        tables.push_back((const char*)sqlite3_column_text(pstmt, 0));

    sqlite3_finalize(pstmt);

    //now for each table, map to an FDO class
    for (size_t i=0; i<tables.size(); i++)
    {
        //first check if table is an FDO metadata table and skip it
        if (IsMetadataTable(tables[i].c_str()))
            continue;

        SltMetadata* md = GetMetadata(tables[i].c_str());
        
        if (md)
        {
            FdoPtr<FdoClassDefinition> fc = md->ToClass();
            classes->Add(fc);
        }
    }

    //We need to clone the schema, because the caller may modify it and mess us up.
    return makeACopy ? FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(m_pSchema, NULL) : FDO_SAFE_ADDREF(m_pSchema);
}

FdoISpatialContextReader* SltConnection::GetSpatialContexts()
{
    return new SltSpatialContextReader(this);
}

SltReader* SltConnection::Select(FdoIdentifier* fcname, 
                                 FdoFilter* filter, 
                                 FdoIdentifierCollection* props, 
                                 bool scrollable, 
                                 const std::vector<NameOrderingPair>& ordering)
{
    if (m_connState != FdoConnectionState_Open)
        throw FdoCommandException::Create(L"Connection must be open in order to Select.");


    const wchar_t* wfc = fcname->GetName();
    size_t wlen = wcslen(wfc);
    size_t clen = 4 * wlen+1;
    char* mbfc = (char*)alloca(4*wlen+1);
    W2A_FAST(mbfc, clen, wfc, wlen);
    StringBuffer sbfcn;
    char* mbfcname = mbfc;

    DBounds bbox;
    std::vector<__int64>* rowids = NULL;
    StringBuffer strWhere((size_t)0);
    bool canFastStep = true;
    bool mustKeepFilterAlive = false;

    SltMetadata* md = GetMetadata(mbfc);
    FdoPtr<FdoClassDefinition> fc = md->ToClass();
    if (md->IsView())
    {
        CacheViewContent(mbfc);
        sbfcn.Reset();
        sbfcn.Append("$view");
        sbfcn.Append(wfc);
        mbfc = sbfcn.Data();
    }

    //Translate the filter from FDO to SQLite where clause.
    //Also detect if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {
        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) 
            strWhere.Append(txtFilter);
        qt.GetBBOX(bbox);
        rowids = qt.DetachIDList();
        canFastStep = qt.CanUseFastStepping();
        mustKeepFilterAlive = qt.MustKeepFilterAlive();
    }

    SpatialIterator* siter = NULL;
    RowidIterator* ri = NULL;


    //if we have a query by specific ID, it will take precedence over spatial query
    if (rowids && ordering.empty())
    {
        ri = new RowidIterator(-1, rowids);
    }
    else if (!bbox.IsEmpty())
    {
        //if we have a BBOX filter, we need to get the spatial index
        SpatialIndex* si = GetSpatialIndex(mbfcname);

        DBounds total_ext;
        si->GetTotalExtent(total_ext);

        if (bbox.Contains(total_ext))
        {
            //only use spatial iterator if the search bounds does not
            //fully contain the data bounds
        }
        else if (bbox.Intersects(total_ext))
        {
            siter = new SpatialIterator(bbox, si);
        }
        else
        {
            // enforce an empty result since result will be empty
            rowids = new std::vector<__int64>();
            rowids->push_back(-1);
            ri = new RowidIterator(-1, rowids);
        }
    }

    //Now process any ordering options .
    //Our strategy here is to perform any ordering without evaluating the where
    //clause. Then we will pass the ordered list of IDs to the feature reader
    //which will process the where clause (filter) in the same way regardless of ordering,
    //or at least we hope it does.
    //This approach may or may not be slower, it needs some experimentation to see if
    //it's better to also perform the where clause here.

    if (!ordering.empty())
    {
        StringBuffer sb;
        sb.Append("SELECT ROWID FROM ");
        sb.AppendDQuoted(mbfc);
        sb.Append(" ORDER BY ");

        SltExtractExpressionTranslator exTrans(props);
        for (size_t i=0; i<ordering.size(); i++)
        {
            if (i)
                sb.Append(",");

            // identifiers for order are provided as identifiers even are calculations.
            // so in order to get the "expression" we need to look for them in the select list
            FdoIdentifier* idfto = ordering[i].name;
            FdoPtr<FdoIdentifier> idfadd;
            if (props != NULL)
            {
                idfadd = props->FindItem(idfto->GetName());
                if (idfadd != NULL)
                {
                    idfadd->Process(&exTrans);
                    StringBuffer* exp = exTrans.GetExpression();
                    sb.Append(exp->Data(), exp->Length());
                    exTrans.Reset();
                }
            }
            // in case it's a property not present in selection list just add it
            if (idfadd == NULL)
                sb.Append(idfto->ToString());

            if (ordering[i].option == FdoOrderingOption_Ascending)
                sb.Append(" ASC");
            else
                sb.Append(" DESC");
        }

        sb.Append(";");

        sqlite3_stmt* stmt = NULL;
        const char* tail = NULL;
        std::vector<__int64>* rows = new std::vector<__int64>(); //accumulate ordered row ids here

        int rc = sqlite3_prepare_v2(m_dbRead, sb.Data(), -1, &stmt, &tail);

        //If there is also a spatial filter, we need to compute the intersection
        //of the ordering with the spatial filter -- the SltReader cannot handle both
        //a row id iterator and a spatial iterator, so we have to precompute this
        //and pass it the intersected row id iterator.
        
        //First, read the spatial iterator fully -- it will return results in sorted order
        if (siter)
        {
            std::vector<int> srows;
            int start = -1;
            int end = -1;

            while (siter->NextRange(start, end))
            {
                for (int i=start; i<=end; i++)
                    srows.push_back(i);
            }

            //Second, we will check for each result of the ordering query, if
            //it exists in the spatial query results, and add it to a new list 
            //if it does

            // we need to get all rows
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
            {
                __int64 id = sqlite3_column_int64(stmt, 0);
                //TODO: resolve rowid type issue (__int64 vs. int)
                if (std::binary_search(srows.begin(), srows.end(), (int)id))
                    rows->push_back(id);
            }

            //get rid of the spatial iterator -- we are done with it here
            delete siter;
            siter = NULL;
        }
        else
        {
            // we need to get all rows
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
                rows->push_back(sqlite3_column_int64(stmt, 0));
        }
        rc = sqlite3_finalize(stmt);

        ri = new RowidIterator(-1, rows);

        if (strWhere.Length()>0 && scrollable)
        {
            FdoPtr<FdoIdentifierCollection> collidf = FdoIdentifierCollection::Create();
            FdoPtr<FdoIdentifier> rowIdIdf = FdoIdentifier::Create(L"rowid");
            collidf->Add(rowIdIdf);
            SltReader* rdrSc = new SltReader(this, collidf, mbfcname, strWhere.Data(), siter, canFastStep, ri);
            ri = GetScrollableIterator(rdrSc);
            delete rdrSc;
            siter = NULL;
            strWhere.Reset();
            canFastStep = true;
        }
    }
    else if (scrollable) //if we want the reader to be scrollable without ordering, we also need a rowid iterator
    {
        if (strWhere.Length()>0 && scrollable)
        {
            FdoPtr<FdoIdentifierCollection> collidf = FdoIdentifierCollection::Create();
            FdoPtr<FdoIdentifier> rowIdIdf = FdoIdentifier::Create(L"rowid");
            collidf->Add(rowIdIdf);
            SltReader* rdrSc = new SltReader(this, collidf, mbfcname, strWhere.Data(), siter, canFastStep, ri);
            ri = GetScrollableIterator(rdrSc);
            delete rdrSc;
            siter = NULL;
            strWhere.Reset();
            canFastStep = true;
        }
        else
        {
            //again, if we also have a spatial iterator, we need
            //to eliminate it from the picture by intersecting the 
            //row list with the spatial result list
            if (siter)
            {
                std::vector<__int64>* rows = new std::vector<__int64>;
                int start = -1;
                int end = -1;

                while (siter->NextRange(start, end))
                {
                    for (int i=start; i<=end; i++)
                        rows->push_back(i);
                }

                delete siter;
                siter = NULL;

                ri = new RowidIterator(-1, rows);
            }
            else if (!ri)
            {
                ri = new RowidIterator(GetFeatureCount(mbfc), NULL);
            }
        }
    }
    SltReader* rdr = new SltReader(this, props, mbfcname, strWhere.Data(), siter, canFastStep, ri);
    if (mustKeepFilterAlive)
        rdr->SetInternalFilter(filter);

    return rdr;
}

RowidIterator* SltConnection::GetScrollableIterator(SltReader* rdr)
{
    std::vector<__int64>* list = new std::vector<__int64>;
    while (rdr->ReadNext())
    {
        //With a default SltReader we know that rowID will
        //be returned in column 0 and geometry in column 1
        //Accessing by index is significantly faster than
        //accessing by name
        FdoInt64 id = rdr->GetInt64((int)0);
        list->push_back(id);
    }
    rdr->Close();
    if (list->size() == 0)
        list->push_back(-1);
    return new RowidIterator(-1, list);
}


FdoIDataReader* SltConnection::SelectAggregates(FdoIdentifier*              fcname, 
                                                FdoIdentifierCollection*    properties,
                                                bool                        bDistinct,
                                                FdoOrderingOption           eOrderingOption,
                                                FdoIdentifierCollection*    ordering,
                                                FdoFilter*                  filter,
                                                FdoIdentifierCollection*    grouping)
{
    const wchar_t* wfc = fcname->GetName();
    StringBuffer sbfcn;
    sbfcn.Append(wfc);
    char* mbfc = sbfcn.Data();
    SltMetadata* md = GetMetadata(mbfc);
    FdoPtr<FdoClassDefinition> fc = md->ToClass();
    if (md->IsView())
    {
        CacheViewContent(mbfc);
        sbfcn.Reset();
        sbfcn.Append("$view");
        sbfcn.Append(wfc);
        mbfc = sbfcn.Data();
    }
    
    StringBuffer sb;
    SltExpressionTranslator exTrans(properties);
    
    if (bDistinct)
    {
        //make SQL for distict values 
        
        FdoPtr<FdoIdentifier> id = properties->GetItem(0);
        
        sb.Append("SELECT DISTINCT ");
        id->Process(&exTrans);
        StringBuffer* exp = exTrans.GetExpression();
        sb.Append(exp->Data(), exp->Length());
        sb.Append(" FROM ");
        sb.AppendDQuoted(mbfc);
    }
    else
    {
        if (!filter && fc->GetClassType() == FdoClassType_FeatureClass)
        {
            SltReader* rdr = CheckForSpatialExtents(properties, (FdoFeatureClass*)fc.p);

            if (rdr)
                return rdr;
        }

        //select aggregate -- only one computed identifier expected!
        FdoPtr<FdoIdentifier> id = properties->GetItem(0);
        id->Process(&exTrans);

        sb.Append("SELECT ");
        StringBuffer* exp = exTrans.GetExpression();
        sb.Append(exp->Data(), exp->Length());
        sb.Append(" FROM ");
        sb.AppendDQuoted(mbfc);
    }

    bool mustKeepFilterAlive = false;
    if(filter) {
        FdoPtr<FdoClassDefinition> fc = GetMetadata(mbfc)->ToClass();
        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        mustKeepFilterAlive = qt.MustKeepFilterAlive();
        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) {
            sb.Append(" WHERE ");
            sb.Append(txtFilter);
        }
    }

    sb.Append(";");

    SltReader* rdr = new SltReader(this, sb.Data());
    if (mustKeepFilterAlive)
        rdr->SetInternalFilter(filter);

    return rdr;
}


FdoInt32 SltConnection::Update(FdoIdentifier* fcname, FdoFilter* filter, FdoPropertyValueCollection* propvals)
{
    StringBuffer sb;
    bool geomPropIsUpdated = false;

    sb.Append("UPDATE ", 7);
    sb.AppendDQuoted(fcname->GetName());
    sb.Append(" SET ", 5);

    const wchar_t* wfc = fcname->GetName();
    size_t wlen = wcslen(wfc);
    size_t clen = 4 * wlen+1;
    char* mbfc = (char*)alloca(4*wlen+1);
    W2A_FAST(mbfc, clen, wfc, wlen);
    SltMetadata* md = GetMetadata(mbfc);
    FdoPtr<FdoClassDefinition> fc = md->ToClass();
    // don't update views
    if (md->IsView())
        throw FdoException::Create(L"Views cannot be updated");

    FdoString* geomPropName = NULL;
    if (fc->GetClassType() == FdoClassType_FeatureClass)
    {
        FdoFeatureClass* fcl = static_cast<FdoFeatureClass*>(fc.p);
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = fcl->GetGeometryProperty();
        geomPropName = geomProp->GetName();
    }

    for (int i=0; i<propvals->GetCount(); i++)
    {
        if (i) sb.Append(",", 1); 

        FdoPtr<FdoPropertyValue> pv = propvals->GetItem(i);
        FdoPtr<FdoIdentifier> id = pv->GetName();
        FdoString* propName = id->GetName();
        
        if (geomPropName != NULL && wcscmp(geomPropName, propName) == 0)
            geomPropIsUpdated = true;

        sb.Append(propName);
        sb.Append("=?", 2);
    }

    std::vector<__int64>* rowids = NULL;
    StringBuffer strWhere((size_t)0);
    DBounds bbox;
    //Translate the filter from FDO to SQLite where clause.
    //Also detect if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {
        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) 
            strWhere.Append(txtFilter);
        qt.GetBBOX(bbox);
        rowids = qt.DetachIDList();
    }

    RowidIterator* ri = NULL;
    //if we have a query by specific ID, it will take precedence over spatial query
    if (rowids)
    {
        ri = new RowidIterator(-1, rowids);
    }
    else if (!bbox.IsEmpty())
    {
        //if we have a BBOX filter, we need to get the spatial index
        SpatialIndex* si = GetSpatialIndex(mbfc);

        DBounds total_ext;
        si->GetTotalExtent(total_ext);

        if (bbox.Contains(total_ext))
        {
            //only use spatial iterator if the search bounds does not
            //fully contain the data bounds
        }
        else if (bbox.Intersects(total_ext))
        {
            SpatialIterator* siter = new SpatialIterator(bbox, si);
            rowids = new std::vector<__int64>();
            int start = -1;
            int end = -1;

            while (siter->NextRange(start, end))
            {
                for (int i=start; i<=end; i++)
                    rowids->push_back(i);
            }
            if (rowids->size() == 0)
            {
                delete rowids;
                return 0;
            }
            else
                ri = new RowidIterator(-1, rowids);
        }
        else
            return 0; // enforce an empty result since result will be empty
    }

    if (!strWhere.Length())
    {
        if (ri)
            sb.Append(" WHERE ROWID=?;", 15);
        else
            sb.Append(";", 1);
    }
    else
    {
        sb.Append(" WHERE ", 7);

        if (ri)
            sb.Append("ROWID=? AND ", 12);

        sb.Append("(", 1);
        sb.Append(strWhere.Data(), strWhere.Length());
        sb.Append(");", 2);
    }

    const char* tail = NULL;
    sqlite3_stmt* stmt = NULL;
    
    FdoInt64 ret = 0;
    int rc = sqlite3_prepare_v2(m_dbWrite, sb.Data(), -1, &stmt, &tail);
    if (rc == SQLITE_OK)
    {
        if (geomPropIsUpdated)
            EnableHooks(true);

        if (!ri)
        {
            BindPropVals(propvals, stmt);
            if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
            {
                sqlite3_finalize(stmt);
                if (geomPropIsUpdated)
                    EnableHooks(false, true);

                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                else
                    throw FdoCommandException::Create(L"Failed to execute update statement.", rc);                
            }
            ret += sqlite3_changes(m_dbWrite);
        }
        else
        {
            ri->MoveToIndex(0);
            int cntProps = propvals->GetCount();
            while(ri->Next())
            {
                BindPropVals(propvals, stmt);
                sqlite3_bind_int64(stmt, cntProps+1, ri->CurrentRowid());
                
                if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
                {
                    sqlite3_finalize(stmt);
                    if (ri)
                        delete ri;
                    if (geomPropIsUpdated)
                        EnableHooks(false, true);

                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                else                    
                    throw FdoCommandException::Create(L"Failed to execute update statement.", rc);
                }
                ret += sqlite3_changes(m_dbWrite);
                sqlite3_reset(stmt);
            }
        }
        if (ri)
            delete ri;
        sqlite3_finalize(stmt);
        if (geomPropIsUpdated)
            EnableHooks(false);
    }
    else
    {
        if (ri)
            delete ri;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
        else
        {
            std::wstring err = L"Failed to parse: " + A2W_SLOW(sb.Data());
            throw FdoCommandException::Create(err.c_str(), rc);
        }
    }
    return (FdoInt32)ret;
}

FdoInt32 SltConnection::Delete(FdoIdentifier* fcname, FdoFilter* filter)
{
    StringBuffer sb;

    sb.Append("DELETE FROM ", 12);
    sb.AppendDQuoted(fcname->GetName());

    std::vector<__int64>* rowids = NULL;
    StringBuffer strWhere((size_t)0);
    DBounds bbox;
    char* mbfc = NULL;
    //Translate the filter from FDO to SQLite where clause.
    //Also detect if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {
        const wchar_t* wfc = fcname->GetName();
        size_t wlen = wcslen(wfc);
        size_t clen = 4 * wlen+1;
        mbfc = (char*)alloca(4*wlen+1);
        W2A_FAST(mbfc, clen, wfc, wlen);
        SltMetadata* md = GetMetadata(mbfc);
        FdoPtr<FdoClassDefinition> fc = md->ToClass();
        // don't update views
        if (md->IsView())
            throw FdoException::Create(L"Views cannot be updated");

        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) 
            strWhere.Append(txtFilter);
        qt.GetBBOX(bbox);
        rowids = qt.DetachIDList();
    }

    RowidIterator* ri = NULL;
    //if we have a query by specific ID, it will take precedence over spatial query
    if (rowids)
    {
        ri = new RowidIterator(-1, rowids);
    }
    else if (!bbox.IsEmpty())
    {
        //if we have a BBOX filter, we need to get the spatial index
        SpatialIndex* si = GetSpatialIndex(mbfc);

        DBounds total_ext;
        si->GetTotalExtent(total_ext);

        if (bbox.Contains(total_ext))
        {
            //only use spatial iterator if the search bounds does not
            //fully contain the data bounds
        }
        else if (bbox.Intersects(total_ext))
        {
            SpatialIterator* siter = new SpatialIterator(bbox, si);
            rowids = new std::vector<__int64>();
            int start = -1;
            int end = -1;

            while (siter->NextRange(start, end))
            {
                for (int i=start; i<=end; i++)
                    rowids->push_back(i);
            }
            if (rowids->size() == 0)
            {
                delete rowids;
                return 0;
            }
            else
                ri = new RowidIterator(-1, rowids);
        }
        else
            return 0; // enforce an empty result since result will be empty
    }

    if (!strWhere.Length())
    {
        if (ri)
            sb.Append(" WHERE ROWID=?;", 15);
        else
            sb.Append(";", 1);
    }
    else
    {
        sb.Append(" WHERE ", 7);

        if (ri)
            sb.Append("ROWID=? AND ", 12);

        sb.Append("(", 1);
        sb.Append(strWhere.Data(), strWhere.Length());
        sb.Append(");", 2);
    }

    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
    int rc;

    FdoInt64 ret = 0;
    if ((rc = sqlite3_prepare_v2(m_dbWrite, sb.Data(), -1, &stmt, &tail)) == SQLITE_OK)
    {
        EnableHooks(true);
        if (!ri)
        {
            if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
            {
                const char* err = sqlite3_errmsg(m_dbWrite);
                FdoException* excThr = NULL;
                if (err != NULL)
                    excThr = FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                else
                    excThr = FdoCommandException::Create(L"Failed to execute delete statement.", rc);

                sqlite3_finalize(stmt);
                EnableHooks(false, true);
                throw excThr;
            }
            ret += sqlite3_changes(m_dbWrite);
        }
        else
        {
            ri->MoveToIndex(0);
            while(ri->Next())
            {
                sqlite3_bind_int64(stmt, 1, ri->CurrentRowid());
                if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
                {
                    const char* err = sqlite3_errmsg(m_dbWrite);
                    FdoException* excThr = NULL;
                    if (err != NULL)
                        excThr = FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                    else
                        excThr = FdoCommandException::Create(L"Failed to execute delete statement.", rc);
                    sqlite3_finalize(stmt);
                    if (ri)
                        delete ri;

                    EnableHooks(false, true);
                    throw excThr;
                }
                ret += sqlite3_changes(m_dbWrite);
                sqlite3_reset(stmt);
            }
        }
        if (ri)
            delete ri;
        sqlite3_finalize(stmt);
        EnableHooks(false);
    }
    else
    {
        if (ri)
            delete ri;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
        else
        {
            std::wstring err = L"Failed to parse: " + A2W_SLOW(sb.Data());
            throw FdoCommandException::Create(err.c_str(), rc);
        }
    }
    return (FdoInt32)ret;
}

SltMetadata* SltConnection::GetMetadata(const char* table)
{
    SltMetadata* ret = NULL;

    MetadataCache::iterator iter = m_mNameToMetadata.find((char*)table);
    
    if (iter == m_mNameToMetadata.end())
    {
        if (!IsMetadataTable(table))
        {
            ret = new SltMetadata(this, table, m_bUseFdoMetadata && m_bHasFdoMetadata);
            FdoPtr<FdoClassDefinition> fc = ret->ToClass();

            //Check if it failed to find the corresponding database table
            if (fc == NULL)
            {
                delete ret;
                ret = NULL;
            }
        }

        m_mNameToMetadata[_strdup(table)] = ret; //Note the memory allocation here
    }
    else 
    {
        ret = iter->second;
    }

    return ret;
}

void SltConnection::GetGeometryExtent(const unsigned char* ptr, int len, DBounds* ext)
{
    if (ptr != NULL && len != 0)
    {
        if (*ptr == 1 && *(ptr+1) != 0) // is WKB
        {
            int estFGFLen = len * 2; //should be sufficient, heh
            if (m_wkbBufferLen < estFGFLen)
            {
                delete[] m_wkbBuffer;
                m_wkbBufferLen = estFGFLen;
                m_wkbBuffer = new unsigned char[m_wkbBufferLen];
            }
            len = Wkb2Fgf(ptr, m_wkbBuffer);

            GetFgfExtents(m_wkbBuffer, len, (double*)ext);
        }
        else if (*ptr != 0 && *(ptr+1) == 0) // is FGF
        {
            GetFgfExtents(ptr, len, (double*)ext);
        }
    }
}

SpatialIndex* SltConnection::GetSpatialIndex(const char* table)
{
    SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.find((char*)table);

    SpatialIndex* si = NULL;
    SpatialIndexDescriptor* spDesc = NULL;
    SltReader* rdr = NULL;
    DBounds ext;

    if (iter != m_mNameToSpatialIndex.end())
    {
        spDesc = iter->second;
        si = spDesc->GetSpatialIndex();
        // don't auto-update the SI for views we will need to find a different way to do that
        if (spDesc->IsView())
            return si;
        FdoInt64 id = si->GetLastInsertedIdx();
        FdoInt64 lastCnt = spDesc->GetFeatureCount();
        if (id < lastCnt)
        {
            // in this case is faster to not use a reader SltReader since we need limited
            // functionality to get the id and the geometry
            sqlite3_stmt* stmt = spDesc->GetNewFeatures(id);
            if (stmt != NULL)
            {
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoIGeometry> fg;
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    FdoInt64 idToAdd = sqlite3_column_int64(stmt, 0);
	                const unsigned char* ptr = (const unsigned char*)sqlite3_column_blob(stmt, 1);
	                int len = sqlite3_column_bytes(stmt, 1);
                    GetGeometryExtent(ptr, len, &ext);
                    if (!ext.IsEmpty())
                        si->Insert((unsigned int)idToAdd, ext);
                }
            }
        }
        // in case we have outstanding changes commit them
        if (spDesc->ChangesAvailable())
            spDesc->CommitChanges();
        return si;
    }

#if 0
    clock_t t0 = clock();
    for (int i=0; i<1000; i++)
    {
    delete si;
#endif
    //build the spatial index, if this is the first time it
    //is needed
    si = new SpatialIndex(GetProperty(PROP_NAME_FILENAME));
    // lets create the description in case we get an exception we don't leak
    SltMetadata* md = GetMetadata(table);
    if(md == NULL || md->IsView())
        spDesc = new SpatialIndexDescriptor(si);
    else
        spDesc = new SpatialIndexDescriptor(this, table, si);
    m_mNameToSpatialIndex[_strdup(table)] = spDesc; //Note the memory allocation

    rdr = new SltReader(this, NULL, table, "", NULL, true, NULL);

    while (rdr->ReadNext())
    {
        int len = 0;

        //With a default SltReader we know that rowID will
        //be returned in column 0 and geometry in column 1
        //Accessing by index is significantly faster than
        //accessing by name
        FdoInt32 id = rdr->GetInt32(0);
        const FdoByte* geom = rdr->GetGeometry(1, &len);

        if (len)
        {
            //TODO: assumes DBounds is 2D since GetFgfExtents is 2D
            GetFgfExtents((unsigned char*)geom, len, (double*)&ext);

            si->Insert(id, ext);
        }
    }

    rdr->Close();
    delete rdr;
    si->ReOpenForRead();
#if 0
    }
    clock_t t1 = clock();
    printf("Spatial index build time: %d\n", t1 - t0);
#endif

    return si;
}

void SltConnection::AddGeomCol(FdoGeometricPropertyDefinition* gpd, const wchar_t* fcname)
{
    StringBuffer sb;

    //add this geometry property to the geometry_columns table
    
    bool supDetGeom = SupportsDetailedGeomType();
    if(supDetGeom)
        sb.Append("INSERT INTO geometry_columns(f_table_name,f_geometry_column,geometry_format,\
            geometry_type,geometry_dettype,coord_dimension,srid)VALUES(");
    else
        sb.Append("INSERT INTO geometry_columns(f_table_name,f_geometry_column,geometry_format,\
            geometry_type,coord_dimension,srid)VALUES(");
    
    sb.AppendSQuoted(fcname);//f_table_name
    sb.Append(","); 
    sb.AppendSQuoted(gpd->GetName());//f_geometry_column
    sb.Append(",'FGF',"); 

    int gtype = gpd->GetGeometryTypes();
    //if gdettype remains 0 at this points, it will be treated as "All" types
    //of geometry
    sb.Append(gtype);
    sb.Append(",");

    int len = 0;
    int gdettype = 0;

    if (supDetGeom)
    {
        FdoGeometryType* gtypes = gpd->GetSpecificGeometryTypes(len);
        for (int idx = 0; idx < len; idx++)
        {
            if (*(gtypes + idx) == FdoGeometryType_None)
                continue;
            gdettype |= (0x01 << ((*(gtypes + idx)) - 1));
        }
        sb.Append(gdettype);
        sb.Append(",");
    }

    int dim = 0x00;
    if (gpd->GetHasElevation())
        dim = 0x01;
    if (gpd->GetHasMeasure())
        dim |= 0x02;
    
    sb.Append(dim); //coord_dimension
    sb.Append(",");
    
    //find a record in spatial_ref_sys whose sr_name matches
    //the name of the spatial context association.
    //Then, assign that record's SRID as the SRID for this
    //geometry table. This way we remove the FDO-idiosyncratic
    //spatial context name from the picture.
    sb.Append((int) FindSpatialContext(gpd->GetSpatialContextAssociation()));
    sb.Append(");");

    int rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
}


//Adds an entry for the given property definition to the FDO metadata table.
//Only done if the connection was opened with the "Use FDO metadata" flag set.
void SltConnection::AddDataCol(FdoDataPropertyDefinition* dpd, const wchar_t* fcname)
{
    if (!m_bUseFdoMetadata || !m_bHasFdoMetadata)
        return;

    StringBuffer sb;
    sb.Append(  "INSERT INTO fdo_columns (f_table_name, f_column_name, f_column_desc, "
        "fdo_data_type, fdo_data_details, fdo_data_length, fdo_data_precision, fdo_data_scale) VALUES("  );
    sb.AppendSQuoted(fcname);
    sb.Append(",");
    sb.AppendSQuoted(dpd->GetName());
    sb.Append(",");
    sb.AppendSQuotedHandleNull(dpd->GetDescription());
    sb.Append(",");
    sb.Append((int)dpd->GetDataType());
    sb.Append(",");
    //nullable = 0x02, system = 0x01
    sb.Append((int)((dpd->GetNullable() ? 0x02 : 0x00) |(dpd->GetIsSystem() ? 0x01 : 0x00)));
    sb.Append(",");
    sb.Append((int)dpd->GetLength());
    sb.Append(",");
    sb.Append((int)dpd->GetPrecision());
    sb.Append(",");
    sb.Append((int)dpd->GetScale());
    sb.Append(");");

    int rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
}

//Returns the SRID of an entry in the spatial_ref_sys table,
//whose sr_name column matches the input.
int SltConnection::FindSpatialContext(const wchar_t* name)
{
    // After apply schema a class can have a garbage CS name
    // in case we do not find it return the first CS SRID
    int retVal = 0; // invalid CS
    if (name)
    {
        std::string mbname = W2A_SLOW(name);

        //We will attempt two ways to get the SRID -- first
        //in case there is an sr_name column, we will get it by name.
        //Otherwise, we will interpret the given name as an SRID integer
        //and make sure it exists in the spatial_ref_sys table
        std::string sql1 = "SELECT srid FROM spatial_ref_sys WHERE sr_name='" + mbname + "';";
        std::string sql2 = "SELECT srid FROM spatial_ref_sys WHERE srid=" + mbname + ";";

        int rc;
        int ret = 0;
        sqlite3_stmt* stmt = NULL;
        const char* tail = NULL;
       
        if ((rc = sqlite3_prepare_v2(m_dbRead, sql1.c_str(), -1, &stmt, &tail)) != SQLITE_OK)
            if ((rc = sqlite3_prepare_v2(m_dbRead, sql2.c_str(), -1, &stmt, &tail)) != SQLITE_OK)
                return 0;

        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
            ret = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }
    // most of FDO providers have only one CS per schema
    // in case we have invalid CS name try getting the first one
    if (!retVal)
        retVal = GetDefaultSpatialContext();
    
    return retVal; //returns 0 if not found
}

int SltConnection::GetDefaultSpatialContext()
{
    int retVal = 0; // invalid CS
    std::string sql = "SELECT srid FROM spatial_ref_sys;";
    int rc;
    int ret = 0;
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
   
    if ((rc = sqlite3_prepare_v2(m_dbRead, sql.c_str(), -1, &stmt, &tail)) != SQLITE_OK)
        return 0;

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        retVal = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return retVal; //returns 0 if not found
}

void SltConnection::UpdateClassesWithInvalidSC()
{
    int defSC = GetDefaultSpatialContext();
    if(defSC != 0)
    {
        StringBuffer sb;
        sb.Append("UPDATE geometry_columns SET srid=");
        sb.Append(defSC);
        sb.Append(" WHERE srid=0;");
        int rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
    }
}

void SltConnection::DeleteClassFromSchema(FdoClassDefinition* fc)
{
    DeleteClassFromSchema(fc->GetName());
}

void SltConnection::DeleteClassFromSchema(const wchar_t* fcName)
{
    std::string table = W2A_SLOW(fcName);

    StringBuffer sb;
    sb.Append("DROP TABLE IF EXISTS ");
    sb.AppendDQuoted(table.c_str());
    sb.Append(";");
    int rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        FdoException* baseExc = NULL;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
        std::wstring errorMsg = std::wstring(L"Failed to delete class \'") + fcName + L"\'"; 
        throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
    }
    sb.Reset();
    sb.Append("DELETE FROM geometry_columns WHERE f_table_name=");
    sb.AppendSQuoted(table.c_str());
    sb.Append(";");
    rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);

    if (m_bUseFdoMetadata)
    {
        sb.Reset();
        sb.Append("DELETE FROM fdo_columns WHERE f_table_name=");
        sb.AppendSQuoted(table.c_str());
        sb.Append(";");
        rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
    }
    SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.find((char*)table.c_str());
    if (iter != m_mNameToSpatialIndex.end())
    {
         delete iter->second;
         free(iter->first); //was allocated using strdup
         m_mNameToSpatialIndex.erase(iter);
    }
}

void SltConnection::AddClassToSchema(FdoClassCollection* classes, FdoClassDefinition* fc)
{
    std::string fcname = W2A_SLOW(fc->GetName());
    StringBuffer sb;
    sb.Append("CREATE TABLE ");
    sb.AppendDQuoted(fcname.c_str());
    sb.Append(" (");

    FdoPtr<FdoClassDefinition> fctmp = FDO_SAFE_ADDREF(fc);
    UniqueConstraints simpleUqc;
    FdoPtr<FdoUniqueConstraintCollection> complexUqc = FdoUniqueConstraintCollection::Create();
    
    int cntip = 0;
    while (fctmp != NULL)
    {
        FdoPtr<FdoDataPropertyDefinitionCollection> pdi = fctmp->GetIdentityProperties();
        if (pdi != NULL)
            cntip += pdi->GetCount();

        FdoPtr<FdoUniqueConstraintCollection> uqc = fctmp->GetUniqueConstraints();
        int cntConstr = uqc->GetCount();
        if (cntConstr != 0)
        {
            for (int i = 0; i < cntConstr; i++)
            {
                FdoPtr<FdoUniqueConstraint> uc = uqc->GetItem(i);
                FdoPtr<FdoDataPropertyDefinitionCollection> propsConstr = uc->GetProperties();
                int cntProps = propsConstr->GetCount();
                if (cntProps == 1)
                {
                    FdoPtr<FdoDataPropertyDefinition> propCu = propsConstr->GetItem(0);
                    simpleUqc[propCu->GetName()] = uc.p;
                }
                else if (cntProps > 1)
                    complexUqc->Add(uc);
            }
        }
        fctmp = fctmp->GetBaseClass();
    }

    CollectBaseClassProperties(classes, fc, fc, sb, (cntip > 1) ? 0 : 1, simpleUqc);
    CollectBaseClassProperties(classes, fc, fc, sb, 2, simpleUqc);
    CollectBaseClassProperties(classes, fc, fc, sb, 3, simpleUqc);
    
    if (complexUqc->GetCount() != 0)
        AddComplexUniqueConstraints(complexUqc, fc,sb);

    if (cntip > 1)
        AddClassPrimaryKeys(fc, sb);

    //HACK -- remove a trailing comma and space added by CollectBaseClassProps
    //and replace by a closing of the SQL command
    sb.Data()[sb.Length()-2] = ')';
    sb.Data()[sb.Length()-1] = ';';

    //create the database table for this feature class
    int rc;
    if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
    {
        FdoException* baseExc = NULL;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
        std::wstring errorMsg = std::wstring(L"Failed to create class \'") + fc->GetName() + L"\'"; 
        throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
    }
}

void SltConnection::UpdateClassFromSchema(FdoClassCollection* classes, FdoClassDefinition* fc, FdoClassDefinition* mainfc)
{
    FdoPtr<FdoClassDefinition> baseFc = fc->GetBaseClass();
    if (NULL != baseFc)
        UpdateClassFromSchema(classes, baseFc, mainfc);

    StringBuffer sb;
    // since we allow only to add fields when we have data
    // we can check only for new added properties.
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
    for (int i = 0; i < idpdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
        if (pd->GetElementState() != FdoSchemaElementState_Unchanged)
            throw FdoException::Create(L"PRIMARY KEYs or UNIQUE constraints cannot be added when class contains data");
    }
    // check if this altering is purely additive
    bool pureAdditive = true;
    for (int i = 0; pureAdditive && i < pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
        pureAdditive = (pd->GetElementState() == FdoSchemaElementState_Added || pd->GetElementState() == FdoSchemaElementState_Unchanged);
    }

    if(!pureAdditive) 
    {
        const std::wstring originalClassName = fc->GetName();    
        const std::wstring tempClassName     = originalClassName + L"_temp_copy";
        fc->SetName(tempClassName.c_str());

        // create temporary class (and table)
        AddClassToSchema(classes, fc);
        
        // copy relevant content of old into temporary table
        sb.Reset();
        sb.Append("INSERT INTO ");
        sb.AppendDQuoted(tempClassName.c_str());
        sb.Append(" (");

        bool first = true;
        for (int i = 0, iEnd = pdc->GetCount(); i<iEnd; i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            if( pd->GetElementState() != FdoSchemaElementState_Detached &&
                pd->GetElementState() != FdoSchemaElementState_Deleted
            ) {
                if(!first) {
                    sb.Append(", ");            
                }
                sb.AppendDQuoted(pd->GetName());
                first = false;
            }
        }
        sb.Append(") SELECT ");
        first = true;
        for (int i = 0, iEnd = pdc->GetCount(); i<iEnd; i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            if( pd->GetElementState() == FdoSchemaElementState_Unchanged ||
                pd->GetElementState() == FdoSchemaElementState_Modified // only renaming is allowed so far
            ) {
                if(!first) {
                    sb.Append(", ");            
                }
                sb.AppendDQuoted(pd->GetName());
                first = false;                          
            }         
        }
        sb.Append(" FROM ");
        sb.Append(originalClassName.c_str());
        sb.Append(";");

        int rc;
        if (!first && (rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
        {
            FdoException* baseExc = NULL;
            const char* err = sqlite3_errmsg(m_dbWrite);
            if (err != NULL)
                baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
            std::wstring errorMsg = L"Failed to copy table content for class \'" + originalClassName + L"\'"; 
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }
        // drop old table
        DeleteClassFromSchema(originalClassName.c_str());
        // rename temporary table
        sb.Reset();
        sb.Append("ALTER TABLE ");
        sb.Append(tempClassName.c_str());
        sb.Append(" RENAME TO ");
        sb.Append(originalClassName.c_str());
        sb.Append(";");
        
        if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
        {
            FdoException* baseExc = NULL;
            const char* err = sqlite3_errmsg(m_dbWrite);
            if (err != NULL)
                baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
            std::wstring errorMsg = L"Failed to rename temporary table for class \'" + originalClassName + L"\'"; 
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }
        // update table name column in meta tables 
        sb.Reset();
        sb.Append("UPDATE geometry_columns SET f_table_name=");
        sb.AppendSQuoted(originalClassName.c_str());
        sb.Append(" WHERE f_table_name=");
        sb.AppendSQuoted(tempClassName.c_str());
        sb.Append(";");
        if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
        {
            FdoException* baseExc = NULL;
            const char* err = sqlite3_errmsg(m_dbWrite);
            if (err != NULL)
                baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
            std::wstring errorMsg = L"Failed to update geometry meta data table for class \'" + originalClassName + L"\'"; 
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }

        if (m_bUseFdoMetadata)
        {
            sb.Reset();
            sb.Append("UPDATE fdo_columns SET f_table_name=");
            sb.AppendSQuoted(originalClassName.c_str());
            sb.Append(" WHERE f_table_name=");
            sb.AppendSQuoted(tempClassName.c_str());
            sb.Append(";");
            if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
            {
                FdoException* baseExc = NULL;
                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
                std::wstring errorMsg = L"Failed to update meta data table for class \'" + originalClassName + L"\'"; 
                throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
            }
        }
        // update spatial index
        std::string tempClassNameA = W2A_SLOW(tempClassName.c_str());
        std::string originalClassNameA = W2A_SLOW(originalClassName.c_str());
        SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.find((char*) tempClassNameA.c_str());
        if (iter != m_mNameToSpatialIndex.end())
        {
             m_mNameToSpatialIndex.erase(iter);
             SpatialIndexDescriptor* desc = new SpatialIndexDescriptor(this, originalClassNameA.c_str(), iter->second->DetachSpatialIndex());
             free(iter->first); //was allocated using strdup
             m_mNameToSpatialIndex[_strdup(originalClassNameA.c_str())] = desc; //Note the memory allocation
        }
        // reset class name
        fc->SetName(originalClassName.c_str());

    } 
    else 
    {
        for (int i = 0; i < pdc->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            // only new properties are processed
            if (pd->GetElementState() != FdoSchemaElementState_Added)
                continue;
            
            FdoPropertyType ptype = pd->GetPropertyType();
            sb.Reset();
            sb.Append("ALTER TABLE ");
            sb.AppendDQuoted(fc->GetName());
            sb.Append(" ADD COLUMN ");
            if (ptype == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)pd.p;
                
                sb.AppendDQuoted(dpd->GetName());
                sb.Append(" ", 1);
                sb.Append(g_fdo2sql_map[dpd->GetDataType()].c_str());

                AddPropertyConstraintDefaultValue(dpd, sb);
                sb.Append(";", 1);
                AddDataCol(dpd, mainfc->GetName());
            }
            else if (ptype == FdoPropertyType_GeometricProperty)
            {
                sb.AppendDQuoted(pd->GetName());
                sb.Append(" BLOB, ", 7);

                AddGeomCol((FdoGeometricPropertyDefinition*)pd.p, mainfc->GetName());
            }
            else
                throw FdoException::Create(L"Invalid property type");
           
            int rc;
            if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
            {
                FdoException* baseExc = NULL;
                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
                std::wstring errorMsg = std::wstring(L"Failed to apply schema for class \'") + fc->GetName() + L"\'"; 
                throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
            }
        }
    }
}

void SltConnection::ApplySchema(FdoFeatureSchema* schema, bool ignoreStates)
{
    FdoPtr<FdoFeatureSchemaCollection> myschemac = DescribeSchema(NULL);
    FdoPtr<FdoFeatureSchema> myschema = myschemac->GetItem(0);
    FdoPtr<FdoClassCollection> oldclasses = myschema->GetClasses();
	FdoPtr<FdoFeatureSchema> userschema = FDO_SAFE_ADDREF(schema);

    FdoPtr<FdoFeatureSchema> mergedSchema;
    FdoPtr<SQLiteSchemaMergeContext> context;
    if (oldclasses->GetCount() != 0)
    {
		// an error will be thrown when user will take the current schema and change it (change the name)
		// and apply schema uning ignoreStates=true (SQLite does not support multimple schema).
		if (ignoreStates && _wcsicmp(schema->GetName(), L"Default") != 0)
		{
			// in case ignoreStates = true and schema name is different, make a copy, change the name
			// and run merge schema. We need to copy it since we cannot change user data!
			userschema = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchema(schema, NULL);
		}
        context = SQLiteSchemaMergeContext::Create(this, myschemac, schema, ignoreStates);
        context->CommitSchemas();
        FdoFeatureSchemasP mergedSchemas = context->GetSchemas();
        mergedSchema = mergedSchemas->GetItem( myschema->GetName() );
    }
    else
        mergedSchema = userschema;

    FdoPtr<FdoClassCollection> classes = mergedSchema->GetClasses();

    int rc = StartTransaction();

    try
    {
        for (int i = 0; i < classes->GetCount(); i++)
        {
            bool changeMade = false;
            FdoPtr<FdoClassDefinition> fc = classes->GetItem(i);
            std::string table;
            if (!ignoreStates)
            {
                switch (fc->GetElementState())
                {
                case FdoSchemaElementState_Added:
                    AddClassToSchema(classes, fc);
                    changeMade = true;
                    break;
                case FdoSchemaElementState_Deleted:
                    DeleteClassFromSchema(fc);
                    changeMade = true;
                    break;
                case FdoSchemaElementState_Modified:
                    if ((NULL == context) || !context->TableHasObjects(fc->GetName()))
                    {
                        DeleteClassFromSchema(fc);
                        AddClassToSchema(classes, fc);
                    }
                    else
                        UpdateClassFromSchema(classes, fc, fc);
                    changeMade = true;
                    break;
                }
            }
            else
            {
                FdoPtr<FdoClassDefinition> oldfc = oldclasses->FindItem(fc->GetName());
                if (oldfc == NULL)
                    AddClassToSchema(classes, fc);
                else
                {
                    if (fc->GetElementState() == FdoSchemaElementState_Unchanged)
                        continue;
                    table = W2A_SLOW(fc->GetName());
                    if (GetFeatureCount(table.c_str()) <= 0)
                    {
                        DeleteClassFromSchema(fc);
                        AddClassToSchema(classes, fc);
                    }
                    else
                        UpdateClassFromSchema(classes, fc, fc);
                }
                changeMade = true;
            }
            if (changeMade)
            {
                // we need to erase the metadata to force a refresh
                if (table.size() == 0)
                    table = W2A_SLOW(fc->GetName());
                MetadataCache::iterator iter = m_mNameToMetadata.find((char*)table.c_str());
                if (iter != m_mNameToMetadata.end())
                {
                    delete iter->second;
                    free(iter->first); //it was created via strdup, must use free()
                    m_mNameToMetadata.erase(iter);
                }
            }
        }
    }
    catch(...)
    {
        RollbackTransaction();
        // The cached FDO schema will need to be refreshed
        FDO_SAFE_RELEASE(m_pSchema);
        m_pSchema = NULL;
        throw;
    }

    rc = CommitTransaction();
    if (rc == SQLITE_OK)
    {
        schema->AcceptChanges();
        // We need to add this call since we have so many cases when users apply schema followed by create 
        // spatial context or the applied schema has invalid CS names (not added into the data store).
        // We may modify this later in case we don't like to enforce classes with an invalid CS to get the default CS
        UpdateClassesWithInvalidSC();
    }
    else // not sure we need to do that yet
        RollbackTransaction();

    // The cached FDO schema will need to be refreshed
    FDO_SAFE_RELEASE(m_pSchema);
    m_pSchema = NULL;
}

std::wstring SltConnection::GenerateValidConstrName(FdoString* name)
{
    std::wstring ret(name);
    int cnt = ret.size();
    for (int idx = 0; idx < cnt; idx++)
    {
        if (iswalnum(ret[idx]) == 0)
            ret[idx] = L'_';
    }
    return ret;
}

void SltConnection::AddPropertyUniqueConstraint(UniqueConstraints& simpleUniqueConstr, FdoDataPropertyDefinition* prop, StringBuffer& sb)
{
    UniqueConstraints::iterator it = simpleUniqueConstr.find(prop->GetName());
    if (it == simpleUniqueConstr.end())
        return;
    FdoUniqueConstraint* uqc = it->second;
    sb.Append(" CONSTRAINT UNQ_", 16);
    sb.Append(GenerateValidConstrName(prop->GetName()).c_str());
    sb.Append(" UNIQUE", 7);
    // remove it to avoid another search
    simpleUniqueConstr.erase(it);
}

void SltConnection::AddPropertyConstraintDefaultValue(FdoDataPropertyDefinition* prop, StringBuffer& sb)
{
    // add the default value
    FdoString* defVal = prop->GetDefaultValue();
    FdoDataType dt = prop->GetDataType();
    if (defVal != NULL && *defVal != '\0' && (dt != FdoDataType_String && _wcsicmp(defVal, L"null") == 0))
        defVal = NULL;

    FdoPtr<FdoPropertyValueConstraint> constr = prop->GetValueConstraint();
    if (!prop->GetNullable() && !prop->GetIsAutoGenerated())
        sb.Append(" NOT NULL ", 10);

    char dateBuff[31];
    *dateBuff = '\0';
    if(defVal != NULL && defVal[0] != '\0')
    {
        switch(dt)
        {
            case FdoDataType_String:
                if (constr == NULL || constr->Contains(FdoPtr<FdoStringValue>(FdoStringValue::Create(defVal))))
                {
                    sb.Append(" DEFAULT(", 9);
                    std::wstring strRez;
                    while(*defVal != L'\0')
                    {
                        if (*defVal == L'\'')
                            strRez.append(L"''");
                        else
                            strRez.append(defVal, 1);
                        defVal++;
                    }
                    sb.AppendSQuoted(strRez.c_str());
                    sb.Append(")", 1);
                }
            break;
            case FdoDataType_Boolean:
                {
                    FdoPtr<FdoDataValue> dv = SltMetadata::GenerateConstraintValue(dt, defVal);
                    if (constr == NULL || constr->Contains(dv))
                    {
                        FdoBooleanValue* pboolVal = static_cast<FdoBooleanValue*>(dv.p);
                        if (!pboolVal->IsNull())
                        {
                            sb.Append(" DEFAULT(", 9);
                            sb.Append(pboolVal->GetBoolean() ? "1" : "0", 1);
                            sb.Append(")", 1);
                        }
                    }
                }
            case FdoDataType_BLOB:
            case FdoDataType_CLOB:
                // nothing
            break;
            case FdoDataType_DateTime:
                {
                    int len = wcslen(defVal);
                    const wchar_t* defValtmp = defVal;
                    std::wstring wsVal;
                    if (len > 2)
                    {
                        if (*defVal == '\'' && *(defVal+len-1) == '\'')
                        {
                            wsVal = std::wstring(defVal+1, len-2);
                        }
                        else if (sqlite3UpperToLower[*defVal] == sqlite3UpperToLower['D'] || 
                            sqlite3UpperToLower[*defVal] == sqlite3UpperToLower['T'] && *(defVal+len-1) == '\'')
                        {
                            // eliminate "DATE '" or "TIME '" or "TIMESTAMP '" from the default value
                            while (*defValtmp != '\'' && *defValtmp != '\0' && (defValtmp-defVal)<15) defValtmp++;
                            if (*defValtmp == '\'')
                                wsVal = std::wstring(defValtmp+1, len-(defValtmp-defVal)-2);
                            else
                                wsVal = defVal;
                        }
                        else
                            wsVal = defVal;

                        FdoPtr<FdoDataValue> dVal = SltMetadata::GenerateConstraintValue(dt, wsVal.c_str());
                        // check if default values are ok in case we have a constraint
                        if((dVal != NULL && !dVal->IsNull()) && (constr == NULL || constr->Contains(dVal)))
                        {
                            sb.Append(" DEFAULT(", 9);
                            FdoDateTimeValue* dataValue = static_cast<FdoDateTimeValue*>(dVal.p);
                            FdoDateTime dtRet = dataValue->GetDateTime();
                            DateToString(&dtRet, dateBuff, 31);
                            sb.AppendSQuoted(dateBuff);
                            sb.Append(")", 1);
                        }
                    }
                }
            break;
            //FdoDataType_Byte FdoDataType_Decimal FdoDataType_Double FdoDataType_Int16 FdoDataType_Int32 FdoDataType_Int64 FdoDataType_Single
            default:
                // check if default values are ok in case we have a constraint
                if(constr == NULL || constr->Contains(FdoPtr<FdoDataValue>(SltMetadata::GenerateConstraintValue(dt, defVal))))
                {
                    sb.Append(" DEFAULT(", 9);
                    sb.Append(defVal);
                    sb.Append(")", 1);
                }
            break;
        }
    }

    if (constr == NULL)
        return;

    FdoString* propName = prop->GetName();

    if (dt == FdoDataType_DateTime)
    {
        *dateBuff = '\0';
        FdoDateTime dtRet;
        FdoDateTimeValue* dataValue;
        if (constr->GetConstraintType() == FdoPropertyValueConstraintType_Range)
        {
            FdoPropertyValueConstraintRange* rgConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
            FdoPtr<FdoDataValue> dataMin = rgConstr->GetMinValue();
            FdoPtr<FdoDataValue> dataMax = rgConstr->GetMaxValue();
            if((dataMin != NULL && dataMin->GetDataType() == FdoDataType_DateTime && !dataMin->IsNull()) ||
                (dataMax != NULL  && dataMax->GetDataType() == FdoDataType_DateTime && !dataMax->IsNull()))
            {
                sb.Append(" CONSTRAINT CHK_", 16);
                sb.Append(GenerateValidConstrName(propName).c_str());
                sb.Append(" CHECK(", 7);
                if ((dataMin != NULL && !dataMin->IsNull()) && (dataMax != NULL  && !dataMax->IsNull())
                    && rgConstr->GetMinInclusive() && rgConstr->GetMaxInclusive())
                {
                    sb.AppendDQuoted(propName);
                    sb.Append(" BETWEEN ", 9);
                    
                    dataValue = static_cast<FdoDateTimeValue*>(dataMin.p);
                    dtRet = dataValue->GetDateTime();
                    DateToString(&dtRet, dateBuff, 31);
                    sb.AppendSQuoted(dateBuff);
                    sb.Append(" AND ", 5);
                    dataValue = static_cast<FdoDateTimeValue*>(dataMax.p);
                    dtRet = dataValue->GetDateTime();
                    DateToString(&dtRet, dateBuff, 31);
                    sb.AppendSQuoted(dateBuff);
                }
                else
                {
                    if (dataMin != NULL && !dataMin->IsNull())
                    {
                        sb.AppendDQuoted(propName);
                        if(rgConstr->GetMinInclusive())
                            sb.Append(">=", 2);
                        else
                            sb.Append(">", 1);
                        dataValue = static_cast<FdoDateTimeValue*>(dataMin.p);
                        dtRet = dataValue->GetDateTime();
                        DateToString(&dtRet, dateBuff, 31);
                        sb.AppendSQuoted(dateBuff);
                    }
                    if (dataMax != NULL && !dataMax->IsNull())
                    {
                        if(dataMin != NULL && !dataMin->IsNull())
                            sb.Append(" AND ", 5);
                        
                        sb.AppendDQuoted(propName);
                        if(rgConstr->GetMinInclusive())
                            sb.Append("<=", 2);
                        else
                            sb.Append("<", 1);
                        dataValue = static_cast<FdoDateTimeValue*>(dataMax.p);
                        dtRet = dataValue->GetDateTime();
                        DateToString(&dtRet, dateBuff, 31);
                        sb.AppendSQuoted(dateBuff);
                    }
                }

                sb.Append(")", 1);
            }
        }
        else
        {
            FdoPropertyValueConstraintList* lsConstr = static_cast<FdoPropertyValueConstraintList*>(constr.p);
            FdoPtr<FdoDataValueCollection> dataColl = lsConstr->GetConstraintList();
            int cnt = (dataColl != NULL) ? dataColl->GetCount() : 0;
            if (cnt != 0)
            {
                sb.Append(" CONSTRAINT CHK_", 16);
                sb.Append(GenerateValidConstrName(propName).c_str());
                sb.Append(" CHECK(", 7);
                sb.AppendDQuoted(propName);
                sb.Append(" IN(", 4);
                for(int idx = 0; idx < cnt; idx++)
                {
                    FdoPtr<FdoDataValue> dataItem = dataColl->GetItem(idx);
                    if(dataItem != NULL && dataItem->GetDataType() == FdoDataType_DateTime && !dataItem->IsNull())
                    {
                        dataValue = static_cast<FdoDateTimeValue*>(dataItem.p);
                        dtRet = dataValue->GetDateTime();
                        DateToString(&dtRet, dateBuff, 31);
                        sb.AppendSQuoted(dateBuff);
                    }
                    if (idx != (cnt-1))
                        sb.Append(",", 1);
                }
                sb.Append("))", 2);
            }
        }
    }
    else if (constr->GetConstraintType() == FdoPropertyValueConstraintType_Range)
    {
        FdoPropertyValueConstraintRange* rgConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
        FdoPtr<FdoDataValue> dataMin = rgConstr->GetMinValue();
        FdoPtr<FdoDataValue> dataMax = rgConstr->GetMaxValue();
        if(dataMin != NULL || dataMax != NULL)
        {
            sb.Append(" CONSTRAINT CHK_");
            sb.Append(GenerateValidConstrName(propName).c_str());
            sb.Append(" CHECK(", 7);
            if(dataMin != NULL && dataMax != NULL && rgConstr->GetMinInclusive() && rgConstr->GetMaxInclusive())
            {
                sb.AppendDQuoted(propName);
                sb.Append(" BETWEEN ", 9);
                sb.Append(dataMin->ToString());
                sb.Append(" AND ", 5);
                sb.Append(dataMax->ToString());
            }
            else
            {
                if(dataMin != NULL)
                {
                    sb.AppendDQuoted(propName);
                    if(rgConstr->GetMinInclusive())
                        sb.Append(">=", 2);
                    else
                        sb.Append(">", 1);
                    sb.Append(dataMin->ToString());
                }
                if (dataMax != NULL)
                {
                    if(dataMin != NULL)
                        sb.Append(" AND ", 5);

                    sb.AppendDQuoted(propName);
                    if(rgConstr->GetMaxInclusive())
                        sb.Append("<=", 2);
                    else
                        sb.Append("<", 1);
                    sb.Append(dataMax->ToString());
                }
            }
            sb.Append(")", 1);
        }
    }
    else
    {
        FdoPropertyValueConstraintList* lsConstr = static_cast<FdoPropertyValueConstraintList*>(constr.p);
        FdoPtr<FdoDataValueCollection> dataColl = lsConstr->GetConstraintList();
        bool needsSQ = (prop->GetDataType() == FdoDataType_String);
        int cnt = (dataColl != NULL) ? dataColl->GetCount() : 0;
        if (cnt != 0)
        {
            sb.Append(" CONSTRAINT CHK_");
            sb.Append(GenerateValidConstrName(propName).c_str());
            sb.Append(" CHECK(", 7);
            sb.AppendDQuoted(propName);
            sb.Append(" IN(", 4);
            for(int idx = 0; idx < cnt; idx++)
            {
                FdoPtr<FdoDataValue> dataItem = dataColl->GetItem(idx);
                sb.Append(dataItem->ToString());
                if (idx != (cnt-1))
                    sb.Append(",", 1);
            }
            sb.Append("))", 2);
        }
    }
}

void SltConnection::AddComplexUniqueConstraints(FdoUniqueConstraintCollection* uniqueConstr, FdoClassDefinition* fc, StringBuffer& sb)
{
    int cntConstr = uniqueConstr->GetCount();
    for (int i = 0; i < cntConstr; i++)
    {
        FdoPtr<FdoUniqueConstraint> uqc = uniqueConstr->GetItem(i);

        sb.Append("CONSTRAINT UNQ_", 15);
        sb.Append(GenerateValidConstrName(fc->GetName()).c_str());
        sb.Append(i+1);
        sb.Append(" UNIQUE (", 9);
        FdoPtr<FdoDataPropertyDefinitionCollection> pdc = uqc->GetProperties();
        int cnt = pdc->GetCount();
        for(int idx = 0; idx < cnt; idx++)
        {
            FdoPtr<FdoDataPropertyDefinition> item = pdc->GetItem(idx);
            sb.AppendDQuoted(item->GetName());
            sb.Append(",", 1);
        }
        sb.Data()[sb.Length()-1] = ')';
        // those values will be replaced at the end with ");"
        sb.Append(", ", 2);
    }
}

void SltConnection::AddClassPrimaryKeys(FdoClassDefinition* fc, StringBuffer& sb)
{
    FdoPtr<FdoClassDefinition> fctmp = FDO_SAFE_ADDREF(fc);
    sb.Append("PRIMARY KEY(", 12);
    while (fctmp != NULL)
    {
        FdoPtr<FdoDataPropertyDefinitionCollection> pdic = fctmp->GetIdentityProperties();
        int cnt = pdic->GetCount();
        for(int idx = 0; idx < cnt; idx++)
        {
            FdoPtr<FdoDataPropertyDefinition> item = pdic->GetItem(idx);
            sb.AppendDQuoted(item->GetName());
            sb.Append(",", 1);
        }
        fctmp = fctmp->GetBaseClass();
    }
    sb.Data()[sb.Length()-1] = ')';
    // those values will be replaced at the end with ");"
    sb.Append(", ", 2);
}

void SltConnection::CollectBaseClassProperties(FdoClassCollection* myclasses, FdoClassDefinition* fc, FdoClassDefinition* mainfc, 
                                               StringBuffer& sb, int mode, UniqueConstraints& simpleUniqueConstr)
{
    FdoPtr<FdoClassDefinition> baseFc = fc->GetBaseClass();
    if (NULL != baseFc)
        CollectBaseClassProperties(myclasses, baseFc, mainfc, sb, mode, simpleUniqueConstr);

    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
    FdoPtr<FdoGeometricPropertyDefinition> gpd;
    
    if (fc->GetClassType() == FdoClassType_FeatureClass)
        gpd = ((FdoFeatureClass*)fc)->GetGeometryProperty();

    if (mode == 0)
    {
        int cntprops = idpdc->GetCount();
        for (int i=0; i<cntprops; i++)
        {
            FdoPtr<FdoDataPropertyDefinition> dpd = idpdc->GetItem(i);
            sb.AppendDQuoted(dpd->GetName());
            sb.Append(" ", 1);
            sb.Append(g_fdo2sql_map[dpd->GetDataType()].c_str());

            AddPropertyConstraintDefaultValue(dpd, sb);
            if (!simpleUniqueConstr.empty())
                AddPropertyUniqueConstraint(simpleUniqueConstr, dpd, sb);
            sb.Append(", ", 2);

            //Add an entry to the metadata type table, if needed
            AddDataCol(dpd, mainfc->GetName());
        }
    }
    else if (mode == 1)
    {
        if (idpdc->GetCount() > 0)
        {
            if (idpdc->GetCount() > 1)
            {
                return;
            }
            else
            {
                bool canAddUnique = true;
                FdoPtr<FdoDataPropertyDefinition> idp = idpdc->GetItem(0);
                FdoDataType dt = idp->GetDataType();
                if (idp->GetIsAutoGenerated() 
                    || dt == FdoDataType_Int32 
                    || dt == FdoDataType_Int64)
                {
                    //autogenerated ID -- we will have SQLite generate a new one for us
                    sb.AppendDQuoted(idp->GetName());
                    sb.Append(" INTEGER PRIMARY KEY");
                    canAddUnique = false;
                }
                else
                {
                    if (simpleUniqueConstr.empty())
                    {
                        sb.AppendDQuoted(idp->GetName());
                        sb.Append(" ", 1);
                        // TODO should we !?
                        // in case it's a decimal with no digits to the right of the decimal point, take
                        // it as int64 since it's faster and it will create a closer copy of original schema
                        //if (dt == FdoDataType_Decimal && idp->GetScale() == 0 && idp->GetPrecision() != 0)
                        //    sb.Append(g_fdo2sql_map[FdoDataType_Int64].c_str());
                        //else
                        sb.Append(g_fdo2sql_map[idp->GetDataType()].c_str());
                        sb.Append(" PRIMARY KEY", 12);
                        canAddUnique = false;
                    }
                }

                AddPropertyConstraintDefaultValue(idp, sb);
                if (canAddUnique && !simpleUniqueConstr.empty())
                    AddPropertyUniqueConstraint(simpleUniqueConstr, idp, sb);
                sb.Append(", ", 2);

                //Add to the metadata table, if necessary
                AddDataCol(idp, mainfc->GetName());
            }
        }
    }
    else if (mode == 2)
    {
        //now do the geometry property -- we want that to be near the 
        //beginning of the list for better performance when reading
        if (gpd.p && gpd->GetElementState() != FdoSchemaElementState_Deleted && gpd->GetElementState() != FdoSchemaElementState_Detached )
        {
            sb.AppendDQuoted(gpd->GetName());
            sb.Append(" BLOB, ", 7);

            //also add to the geometry_columns table
            AddGeomCol(gpd.p, mainfc->GetName());
        }
    }
    else
    {
        //add the remaining properties
        for (int i=0; i<pdc->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            if( pd->GetElementState() == FdoSchemaElementState_Deleted || pd->GetElementState() == FdoSchemaElementState_Detached )
                continue;

            FdoPropertyType ptype = pd->GetPropertyType();

            if (ptype == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)pd.p;

                //make sure it is not the id property, we did that already
                if (idpdc->Contains(dpd->GetName()))
                    continue;

                sb.AppendDQuoted(dpd->GetName());
                sb.Append(" ", 1);
                sb.Append(g_fdo2sql_map[dpd->GetDataType()].c_str());

                AddPropertyConstraintDefaultValue(dpd, sb);
                if (!simpleUniqueConstr.empty())
                    AddPropertyUniqueConstraint(simpleUniqueConstr, dpd, sb);
                sb.Append(", ", 2);

                //Add an entry to the metadata type table, if needed
                AddDataCol(dpd, mainfc->GetName());
            }
            else if (ptype == FdoPropertyType_GeometricProperty)
            {
                //make sure it is not THE geometry property which we already did
                if (gpd != NULL && wcscmp(gpd->GetName(), pd->GetName()) == 0)
                    continue;

                sb.AppendDQuoted(pd->GetName());
                sb.Append(" BLOB, ", 7);

                AddGeomCol((FdoGeometricPropertyDefinition*)pd.p, mainfc->GetName());
            }
        }
    }
}


SltReader* SltConnection::CheckForSpatialExtents(FdoIdentifierCollection* props, FdoFeatureClass* fc)
{
    //Checks for a SpatialExtents or Count request
    std::string fcname = W2A_SLOW(fc->GetName());
    std::string gname;
    std::string extname = ""; //if this remains empty, we know extents is not needed
    std::string countname = "";

    int propsCount = props->GetCount();
    if (propsCount != 1 && propsCount != 2)
        return NULL;
    
    SltScCHelperTranslator expTrans(fc);
    for (int i=0; i<propsCount; i++)
    {
        FdoPtr<FdoIdentifier> identifier = props->GetItem(i);
        identifier->Process(&expTrans);
        if (expTrans.IsError())
            return NULL;
    }
    extname = W2A_SLOW(expTrans.GetSContextName());
    countname = W2A_SLOW(expTrans.GetCountName());

    //ok this is a spatial extents or count computed property.
    //Look up the extents and count and return a reader with them.
    //We will always return count, but not extents, if they are not needed.

    //create a temporary table to hold return result
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);

    sqlite3_stmt* stmt;
    const char* tail = NULL;
    std::string sql;

    //get the count
    //we always use the count, whether we compute extents or not
    FdoInt64 count = countname.empty()? 0 : GetFeatureCount(fcname.c_str());

    //case where we only need count
    if (extname.empty())
    {
        sql = "CREATE TABLE SpatialExtentsResult (\"" + countname + "\" INTEGER);";

        char* err;
        rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &err);

        if (rc)
            sqlite3_free(err);

        //insert into the temporary table
        //TODO: some day we should check the error codes...
        //but really, failure is not an option here.
        rc = sqlite3_prepare_v2(db, "INSERT INTO SpatialExtentsResult VALUES(?);", -1, &stmt, &tail);
        rc = sqlite3_bind_int64(stmt, 1, count);
        rc = sqlite3_step(stmt);
        rc = sqlite3_finalize(stmt);
    }
    else
    {
        //get the extents and convert to blob
        SpatialIndex* si = GetSpatialIndex(fcname.c_str());
        DBounds ext;
        si->GetTotalExtent(ext);

        //convert to FGF byte array
        FgfPolygon poly;
        poly.np = 5;

        poly.p[0] = ext.min[0];   poly.p[1] = ext.min[1];
        poly.p[2] = ext.max[0];   poly.p[3] = ext.min[1];
        poly.p[4] = ext.max[0];   poly.p[5] = ext.max[1];
        poly.p[6] = ext.min[0];   poly.p[7] = ext.max[1];
        poly.p[8] = ext.min[0];   poly.p[9] = ext.min[1];


        StringBuffer sql;
        sql.Append("CREATE TABLE SpatialExtentsResult (\"", 36);
        sql.Append(extname.c_str(), extname.size());
        if (!countname.empty())
        {
            sql.Append("\" BLOB,\"", 8);
            sql.Append(countname.c_str(), countname.size());
            sql.Append("\" INTEGER);", 11);
        }
        else
            sql.Append("\" BLOB);", 8);
        
        char* err;
        rc = sqlite3_exec(db, sql.Data(), NULL, NULL, &err);

        if (rc)
            sqlite3_free(err);

        //insert into the temporary table
        //TODO: some day we should check the error codes...
        //but really, failure is not an option here.
        if (!countname.empty())
            rc = sqlite3_prepare_v2(db, "INSERT INTO SpatialExtentsResult VALUES(?,?);", -1, &stmt, &tail);
        else
            rc = sqlite3_prepare_v2(db, "INSERT INTO SpatialExtentsResult VALUES(?);", -1, &stmt, &tail);
        rc = sqlite3_bind_blob(stmt, 1, &poly, sizeof(FgfPolygon), SQLITE_TRANSIENT);
        if (countname.size() != 0)
            rc = sqlite3_bind_int64(stmt, 2, count);
        rc = sqlite3_step(stmt);
        rc = sqlite3_finalize(stmt);
    }

    stmt = NULL;
    tail = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT * FROM SpatialExtentsResult;", -1, &stmt, &tail);

    if (rc == SQLITE_OK)
    {
        FdoPtr<FdoFeatureClass> cls = FdoFeatureClass::Create(L"GenSpatialExtents", NULL);
        FdoPtr<FdoPropertyDefinitionCollection> props = cls->GetProperties();

        // add count
        if (!countname.empty())
        {
            FdoPtr<FdoDataPropertyDefinition> dpdcnt = FdoDataPropertyDefinition::Create(expTrans.GetCountName(), NULL);
		    dpdcnt->SetDataType(FdoDataType_Int64);
            props->Add(dpdcnt);
        }

        if (!extname.empty())
        {            
            // we need extent
            FdoPtr<FdoGeometricPropertyDefinition> geom = FdoGeometricPropertyDefinition::Create(expTrans.GetSContextName(), NULL);
            props->Add(geom);
            cls->SetGeometryProperty(geom);
        }
        return new SltReader(this, stmt, true, cls);        
    }
    else
    {
        const char* err = sqlite3_errmsg(db);
        if (err != NULL)
            throw FdoException::Create(A2W_SLOW(err).c_str(), rc);
        else
            throw FdoException::Create(L"Failed to generate Count() or SpatialExtents() reader.", rc);
    }
}

FdoInt64 SltConnection::GetFeatureCount(const char* table)
{
    //get the feature count -- this is approximate since
    //a real count would take a long time (table scan)

    sqlite3_stmt* stmt;
    const char* tail = NULL;

    StringBuffer sb;
    sb.Append("SELECT MAX(ROWID) FROM ");
    sb.AppendDQuoted(table);
    sb.Append(";");

    if (sqlite3_prepare_v2(m_dbRead, sb.Data(), -1, &stmt, &tail) == SQLITE_OK)
    {
        int rc = sqlite3_step(stmt);
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return count;
    }

    return -1;
}

// Do not use it in other functions than GetCachedParsedStatement
// Statement is not cached -- make one, and also add an entry for it
// into the cache table
#define SQL_PREPARE_CACHEPARSEDSTM(db) {                                           \
    const char* tail = NULL;                                                       \
    int rc = sqlite3_prepare_v2(((NULL==db)?m_dbRead:db), sql, -1, &ret, &tail);   \
    if (rc != SQLITE_OK || ret == NULL){                                           \
        const char* err = sqlite3_errmsg(((NULL==db)?m_dbRead:db));                \
        if (err != NULL)                                                           \
            throw FdoException::Create(A2W_SLOW(err).c_str(), rc);                 \
        else                                                                       \
            throw FdoException::Create(L"Failed to parse SQL statement", rc);      \
        }                                                                          \
}                                                                                  \


sqlite3_stmt* SltConnection::GetCachedParsedStatement(const char* sql, sqlite3* db)
{
    //Don't let too many queries get cached.
    //There are legitimate cases where lots of different
    //queries can be issued on the same connection.
    if (m_mCachedQueries.size() > 1000)
    {
        ClearQueryCache();
    }

    sqlite3_stmt* ret = NULL;

    QueryCache::iterator iter = m_mCachedQueries.find((char*)sql);
    
    if (iter != m_mCachedQueries.end())
    {
        //found a cached statement -- take it from the cache
        //and return it

        QueryCacheRecList& lst = iter->second;
        for(size_t i=0; i<lst.size(); i++)
        {
            QueryCacheRec& rec = lst[i];
            if (!rec.inUse)
            {
                if (db == NULL || (db == sqlite3_db_handle(rec.stmt)))
                {
                    rec.inUse = true;
                    ret = rec.stmt;
                    break;
                }
            }
        }
        if (ret == NULL)
        {
            // to avoid a m_mCachedQueries.find() we will clone this small part
            SQL_PREPARE_CACHEPARSEDSTM(db);
            lst.push_back(QueryCacheRec(ret));
        }
    }
    else
    {
        // to avoid a m_mCachedQueries.find() we will clone this small part
        SQL_PREPARE_CACHEPARSEDSTM(db);
        m_mCachedQueries[_strdup(sql)].push_back(QueryCacheRec(ret));
    }
    if (ret == NULL)
        throw FdoException::Create(L"Failed to create SQL statement");

    return ret;
}


void SltConnection::ReleaseParsedStatement(const char* sql, sqlite3_stmt* stmt)
{
    QueryCache::iterator iter = m_mCachedQueries.find((char*)sql);

    if (iter != m_mCachedQueries.end())
    {
        QueryCacheRecList& lst = iter->second;
        for(size_t i=0; i<lst.size(); i++)
        {
            QueryCacheRec& rec = lst[i];

            if (rec.stmt == stmt)
            {
                sqlite3_reset(stmt);
                rec.inUse = false;
                return;
            }
        }
    }
    sqlite3_finalize(stmt);
}

void SltConnection::ClearQueryCache()
{
    //We have to keep all cached statements that are still in use, 
    //we can only free the ones that are not in use
    QueryCache newCache;

    for (QueryCache::iterator iter = m_mCachedQueries.begin(); 
        iter != m_mCachedQueries.end(); iter++)
    {
        QueryCacheRecList& lst = iter->second;

        bool stringInUse = false;

        for (size_t i=0; i<lst.size(); i++)
        {
            QueryCacheRec& rec = lst[i];
            
            if (rec.inUse)
            {
                //If the query is in use, carry it over to the new cache map

                stringInUse = true;
                newCache[iter->first].push_back(rec);
            }
            else
            {
                //otherwise free the query and the query string

                sqlite3_finalize(rec.stmt);
            }
        }

        if (!stringInUse)
            free(iter->first); //it was created via strdup, must use free()
    }

    m_mCachedQueries = newCache;
}

bool SltConnection::GetExtents(const wchar_t* fcname, double ext[4])
{
    std::string table = W2A_SLOW(fcname);

    SpatialIndex* si = GetSpatialIndex(table.c_str());

    DBounds dext;
    if (si)
    {
        si->GetTotalExtent(dext);

        ext[0] = dext.min[0];
        ext[1] = dext.min[1];
        ext[2] = dext.max[0];
        ext[3] = dext.max[1];
    }
    return !dext.IsEmpty();
}

// function added to handle old files already created 
// without support for geometry_dettype
bool SltConnection::SupportsDetailedGeomType()
{
    if (m_cSupportsDetGeomType == -1)
    {
        m_cSupportsDetGeomType = 0;
        Table* table = sqlite3FindTable(m_dbRead, "geometry_columns", 0);
        if( table )
        {
            for(int idx = 0; idx < table->nCol; idx++)
            {
                if (sqlite3StrICmp((table->aCol+idx)->zName, "geometry_dettype") == 0)
                {
                    m_cSupportsDetGeomType = 1;
                    break;
                }
            }
        }
    }
    return (m_cSupportsDetGeomType != 0);
}

FdoITransaction* SltConnection::BeginTransaction()
{
    if (m_dbWrite == NULL)
        throw FdoException::Create(L"Connection is not opened");
    StartTransaction(true);
    return new SltTransaction(this);
}

// save points keep data store locked!
// all calls return SQLITE_LOCKED in case of rollback
// #define SAVEPOINTS

int SltConnection::StartTransaction(bool isUserTrans)
{
    if (m_dbWrite == NULL)
        return SQLITE_MISUSE;

    int rc = SQLITE_OK;
    if (!isUserTrans)
    {
        if (m_transactionState == SQLiteActiveTransactionType_None)
        {
#ifdef SAVEPOINTS
            rc = sqlite3_exec(m_dbWrite, "SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "BEGIN;", NULL, NULL, NULL);
#endif
            if (rc == SQLITE_OK) // do we need it!?
                m_transactionState = SQLiteActiveTransactionType_Internal;
        }
    }
    else
    {
        if (m_transactionState == SQLiteActiveTransactionType_User)
        {
            // transaction already started exception
            throw FdoException::Create(L"Transactions is already started");
        }
        else if (m_transactionState == SQLiteActiveTransactionType_Internal)
        {
            // commit internal transaction and open an user transaction
#ifdef SAVEPOINTS
            rc = sqlite3_exec(m_dbWrite, "RELEASE SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "COMMIT;", NULL, NULL, NULL);
#endif
            m_transactionState = SQLiteActiveTransactionType_None;
        }

#ifdef SAVEPOINTS
        rc = sqlite3_exec(m_dbWrite, "SAVEPOINT sp;", NULL, NULL, NULL);
#else
        rc = sqlite3_exec(m_dbWrite, "BEGIN;", NULL, NULL, NULL);
#endif
        if (rc == SQLITE_OK)
            m_transactionState = SQLiteActiveTransactionType_User;
        else
            throw FdoException::Create(L"SQLite begin transaction failed!", rc);
    }
    return rc;
}

int SltConnection::CommitTransaction(bool isUserTrans)
{
    if (m_dbWrite == NULL)
        return SQLITE_MISUSE;

    int rc = SQLITE_OK;
    if (!isUserTrans)
    {
        if (m_transactionState == SQLiteActiveTransactionType_Internal)
        {
#ifdef SAVEPOINTS
            rc = sqlite3_exec(m_dbWrite, "RELEASE SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "COMMIT;", NULL, NULL, NULL);
#endif
            if (rc == SQLITE_OK)
                m_transactionState = SQLiteActiveTransactionType_None;
            // else we need to handle this internally
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::commit_hook(this);
        }
    }
    else
    {
        if (m_transactionState == SQLiteActiveTransactionType_User)
        {
            // commit internal transaction and open an user transaction
#ifdef SAVEPOINTS
            rc = sqlite3_exec(m_dbWrite, "RELEASE SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "COMMIT;", NULL, NULL, NULL);
#endif
            if (rc == SQLITE_OK)
                m_transactionState = SQLiteActiveTransactionType_None;
            else
                throw FdoException::Create(L"SQLite commit transaction failed!", rc);
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::commit_hook(this);
        }
        else
            throw FdoException::Create(L"No active transaction to commit");
    }
    return rc;
}

int SltConnection::RollbackTransaction(bool isUserTrans)
{
    if (m_dbWrite == NULL)
        return SQLITE_MISUSE;

    int rc = SQLITE_OK;
    if (!isUserTrans)
    {
        if (m_transactionState == SQLiteActiveTransactionType_Internal)
        {
#ifdef SAVEPOINTS
            rc = sqlite3_exec(m_dbWrite, "ROLLBACK TO SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "ROLLBACK;", NULL, NULL, NULL);
#endif
            m_transactionState = SQLiteActiveTransactionType_None;
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::rollback_hook(this);
        }
    }
    else
    {
        if (m_transactionState == SQLiteActiveTransactionType_User)
        {
            // commit internal transaction and open an user transaction
#ifdef SAVEPOINTS
            rc = sqlite3_exec(m_dbWrite, "ROLLBACK TO SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "ROLLBACK;", NULL, NULL, NULL);
#endif
            m_transactionState = SQLiteActiveTransactionType_None;
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::rollback_hook(this);
        }
        else
            throw FdoException::Create(L"No active transaction to rollback");
    }
    return rc;
}

void SltConnection::CacheViewContent(const char* viewName)
{
    StringBuffer sb;
    sb.Append("$view");
    sb.Append(viewName);
    Table* table = sqlite3FindTable(m_dbRead, sb.Data(), 0);
    if (table == NULL)
    {
        sb.Reset();
        sb.Append("CREATE TEMP TABLE IF NOT EXISTS ", 32);
        sb.Append("\"$view", 6);
        sb.Append(viewName);
        sb.Append("\" AS SELECT * FROM ", 19);
        sb.AppendDQuoted(viewName);
        sb.Append(";");
        sqlite3_exec(m_dbRead, sb.Data(), NULL, NULL, NULL);
    }
}

// below section is a pain but is no other way to "see" the changes on update & delete
void SltConnection::update_hook(void* caller, int action, char const* database,
                                char const* tablename, sqlite3_int64 id)
{
    switch (action)
    {
    case SQLITE_DELETE:
        {
            SltConnection* conn = static_cast<SltConnection*>(caller);
            SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.find((char*)tablename);
            if (iter != conn->m_mNameToSpatialIndex.end())
            {
                iter->second->AddRowIdToDeleteList(id);
                conn->m_changesAvailable = true;
            }
        }
        break;
    case SQLITE_UPDATE:
        {
            SltConnection* conn = static_cast<SltConnection*>(caller);
            SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.find((char*)tablename);
            if (iter != conn->m_mNameToSpatialIndex.end())
            {
                iter->second->AddRowIdToUpdateList(id);
                conn->m_changesAvailable = true;
            }
        }
        break;
    //case SQLITE_INSERT:
    default:
        // in this case we do not do anything since
        // insert is handled in a different way performance reasons
        break;
    }
}

int SltConnection::commit_hook(void* caller)
{
    SltConnection* conn = static_cast<SltConnection*>(caller);
    if (conn->m_changesAvailable)
    {
        for (SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.begin();
             iter != conn->m_mNameToSpatialIndex.end(); iter++)
        {
            iter->second->CommitChanges();
        }
        conn->m_changesAvailable = false;
    }
    return 0;
}

void SltConnection::rollback_hook(void* caller)
{
    // since rollback can be hard to maintain just force a SI 
    // rebuild for those who contain changes
    SltConnection* conn = static_cast<SltConnection*>(caller);
    if (conn->m_changesAvailable)
    {
        for (SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.begin();
             iter != conn->m_mNameToSpatialIndex.end(); iter++)
        {
            if (iter->second->ChangesAvailable())
            {
                delete iter->second;
                free(iter->first); //was allocated using strdup
                conn->m_mNameToSpatialIndex.erase(iter);
                // we will get a crash in case we do not set it back
                iter = conn->m_mNameToSpatialIndex.begin();
            }
        }
        conn->m_changesAvailable = false;
    }
}

void SltConnection::EnableHooks(bool enable, bool enforceRollback)
{
    if (enable)
    {
        if (m_updateHookEnabled)
            return;
        sqlite3_update_hook(m_dbWrite, SltConnection::update_hook, this);
        sqlite3_commit_hook(m_dbWrite, SltConnection::commit_hook, this);
        sqlite3_rollback_hook(m_dbWrite, SltConnection::rollback_hook, this);
        m_updateHookEnabled = true;
    }
    else
    {
        sqlite3_update_hook(m_dbWrite, NULL, NULL);
        sqlite3_commit_hook(m_dbWrite, NULL, NULL);
        sqlite3_rollback_hook(m_dbWrite, NULL, NULL);
        m_updateHookEnabled = false;

         // we need to commit since user wants to "see" his change during a transaction
        if (!enforceRollback)
            SltConnection::commit_hook(this);
        else
            SltConnection::rollback_hook(this);
    }
}
