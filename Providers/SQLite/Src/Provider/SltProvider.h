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

#include "SltCapabilities.h"
#include "SQLiteProvider.h"

class SltMetadata;
class SpatialIndex;
class SltReader;
class RowidIterator;
class SltExtendedSelect;
struct NameOrderingPair;
class StringBuffer;
class SpatialIndexDescriptor;
struct DBounds;
class ConnInfoDetails;
class SpatialIndexMaxRecordInfo;

// on read connection only the provider can open (internal) transactions
enum SQLiteActiveTransactionType
{
    SQLiteActiveTransactionType_None = 0,     // no transaction started
    SQLiteActiveTransactionType_Internal = 1, // internal transaction started on write connection
    SQLiteActiveTransactionType_User = 2      // user transaction started on write connection
};

struct string_less
{	// functor for operator<
bool operator()(const char* _Left, const char* _Right) const
	{	// apply operator< to operands
	    return strcmp(_Left, _Right) < 0;
	}
};

struct wstring_less
{	// functor for operator<
bool operator()(FdoString* _Left, FdoString* _Right) const
	{	// apply operator< to operands
	    return wcscmp(_Left, _Right) < 0;
	}
};

struct QueryCacheRec
{
    QueryCacheRec() : stmt(NULL), inUse(false) {}
    QueryCacheRec(sqlite3_stmt* s) : stmt(s), inUse(true) {}

    sqlite3_stmt* stmt;
    bool inUse;
};
typedef std::vector<QueryCacheRec> QueryCacheRecList;
typedef std::map<char*, QueryCacheRecList, string_less> QueryCache;

typedef std::map<char*, SltMetadata*, string_less> MetadataCache;

typedef std::map<FdoString*, FdoUniqueConstraint*, wstring_less> UniqueConstraints;

typedef std::map<char*, SpatialIndexDescriptor*, string_less> SpatialIndexCache;

typedef std::map<char*, SpatialIndexMaxRecordInfo*, string_less> MaxRecordInfoCache;

class SltConnection : public FdoIConnection, 
                      public FdoIConnectionInfo,
                      public FdoIConnectionPropertyDictionary,
                      public FdoIDataStorePropertyDictionary
{
public:

    SltConnection();

protected:
    virtual ~SltConnection();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    SLT_IMPLEMENT_REFCOUNTING

    //-------------------------------------------------------
    // FdoIConnection implementation
    //-------------------------------------------------------

public:
    
    //FdoIConnection functions requiring SQLite specific logic
    //implemented in the .cpp file
    virtual FdoString*          GetConnectionString();
    virtual void                SetConnectionString(FdoString* value);
    virtual FdoConnectionState  Open();
    virtual void                Close();
    virtual FdoICommand*        CreateCommand(FdoInt32 commandType);

    
    //FdoIConnection function implementations
    virtual FdoIConnectionCapabilities* GetConnectionCapabilities() 
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoISchemaCapabilities*     GetSchemaCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoICommandCapabilities*    GetCommandCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIFilterCapabilities*     GetFilterCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIExpressionCapabilities* GetExpressionCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIRasterCapabilities*     GetRasterCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoITopologyCapabilities*   GetTopologyCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIGeometryCapabilities*   GetGeometryCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIConnectionInfo*         GetConnectionInfo()
                                                { return FDO_SAFE_ADDREF(this); }
    virtual FdoConnectionState          GetConnectionState() 
                                                { return m_connState; }


    virtual FdoInt32 GetConnectionTimeout()                     { return 0; }
    virtual void SetConnectionTimeout(FdoInt32 value)           { }
    virtual FdoITransaction* BeginTransaction();
    virtual FdoPhysicalSchemaMapping* CreateSchemaMapping()     { return NULL; }
    virtual void SetConfiguration(FdoIoStream* stream)          { }
    virtual void Flush()                                        { }


    //-------------------------------------------------------
    // FdoIConnectionInfo implementation
    //-------------------------------------------------------

    virtual FdoString* GetProviderName()                { return SQLITE_PROVIDER_NAME; }
    virtual FdoString* GetProviderDisplayName()         { return SQLITE_PROVIDER_DISPLAY_NAME; }
    virtual FdoString* GetProviderDescription()         { return SQLITE_PROVIDER_DESCRIPTION; }
    virtual FdoString* GetProviderVersion()             { return SQLITE_PROVIDER_VERSION; }
    virtual FdoString* GetFeatureDataObjectsVersion()   { return SQLITE_FDO_VERSION; }
    
    virtual FdoIConnectionPropertyDictionary*   GetConnectionProperties()
                                                        { return FDO_SAFE_ADDREF(this); }
    virtual FdoProviderDatastoreType            GetProviderDatastoreType() 
                                                        { return FdoProviderDatastoreType_File; } 
    virtual FdoStringCollection*                GetDependentFileNames()
        { 
            FdoStringCollection* fsc = FdoStringCollection::Create();
            fsc->Add((*m_mProps)[PROP_NAME_FILENAME].c_str());
            return fsc; 
        }

//-------------------------------------------------------
// FdoIConnectionPropertyDictionary declaration
//-------------------------------------------------------

    virtual FdoString**     GetPropertyNames            (FdoInt32& count);
    virtual FdoString*      GetProperty                 (FdoString* name);
    virtual void            SetProperty                 (FdoString* name, FdoString* value);
    virtual FdoString*      GetPropertyDefault          (FdoString* name);
    virtual bool            IsPropertyRequired          (FdoString* name);
    virtual bool            IsPropertyProtected         (FdoString* name);
    virtual bool            IsPropertyEnumerable        (FdoString* name);
    virtual FdoString**     EnumeratePropertyValues     (FdoString* name, FdoInt32& count);
    virtual FdoString*      GetLocalizedName            (FdoString* name);
    virtual bool            IsPropertyFileName          (FdoString* name);
    virtual bool            IsPropertyFilePath          (FdoString* name);
    virtual bool            IsPropertyDatastoreName     (FdoString* name);

//--------------------------------------------------------
//   Command execution functions -- each of these corresponds
//   to an FDO command
//--------------------------------------------------------

    FdoFeatureSchemaCollection* 
                        DescribeSchema          (FdoStringCollection* classNames, bool makeACopy = true);
    
    FdoISpatialContextReader* 
                        GetSpatialContexts      ();
    
    void                CreateDatabase          ();

    SltReader*  Select                         (FdoIdentifier*           fcname, 
                                                FdoFilter*               filter, 
                                                FdoIdentifierCollection* props,
                                                bool                     scrollable,
                                                const std::vector<NameOrderingPair>& ordering,
                                                FdoParameterValueCollection*  parmValues);

    FdoIDataReader*     SelectAggregates       (FdoIdentifier*             fcname, 
                                                FdoIdentifierCollection*   properties,
                                                bool                       bDistinct,
                                                FdoFilter*                  filter,
                                                FdoOrderingOption          eOrderingOption,
                                                FdoIdentifierCollection*   ordering,
                                                FdoFilter*                 grFilter,
                                                FdoIdentifierCollection*   grouping,
                                                FdoParameterValueCollection*  parmValues);

    FdoInt32            Update                 (FdoIdentifier*              fcname, 
                                                FdoFilter*                  filter, 
                                                FdoPropertyValueCollection* propvals,
                                                FdoParameterValueCollection*  parmValues);
    
    FdoInt32            Delete                 (FdoIdentifier*              fcname, 
                                                FdoFilter*                  filter,
                                                FdoParameterValueCollection*  parmValues);


    void                ApplySchema            (FdoFeatureSchema* schema, bool ignoreStates);

    sqlite3*        GetDbConnection() { return m_dbWrite; }
    SpatialIndex*   GetSpatialIndex(const char* table);
    bool            GetExtents(const wchar_t* fcname, double ext[4]);
    SltMetadata*    GetMetadata(const char* table);
    SltReader*      CheckForSpatialExtents(FdoIdentifierCollection* props, FdoFeatureClass* fc, FdoFilter* filter);
    FdoInt64        GetFeatureCount(const char* table);
    
    sqlite3_stmt*   GetCachedParsedStatement(const char* sql);
    void            ReleaseParsedStatement(const char* sql, sqlite3_stmt* stmt);
    void            ClearQueryCache();
    
    bool SupportsDetailedGeomType();
    
    int StartTransaction(bool isUserTrans = false); 
    int CommitTransaction(bool isUserTrans = false);
    int RollbackTransaction(bool isUserTrans = false);
    bool IsTransactionStarted() { return (m_transactionState != SQLiteActiveTransactionType_None); }
    void CacheViewContent(const char* viewName);
    bool HooksEnabled() {return m_updateHookEnabled;}
    void EnableHooks(bool enable = true, bool enforceRollback = false);
    void GetGeometryExtent(const unsigned char* ptr, int len, DBounds* ext);
    bool IsCoordSysLatLong();
    bool IsReadOnlyConnection();
    
    // when SC not found: if valIfNotFound = 0 the default SC will be returned else that value will be returned.
    int FindSpatialContext(const wchar_t* name, int valIfNotFound = 0);
    int GetDefaultSpatialContext();
    SpatialIndexMaxRecordInfo* GetSpatialIndexMaxRecordInfo(const char* table);

private :

    void AddGeomCol(FdoGeometricPropertyDefinition* gpd, const wchar_t* fcname);
    void AddDataCol(FdoDataPropertyDefinition* dpd, const wchar_t* fcname);
    void AddClassToSchema(FdoClassCollection* classes, FdoClassDefinition* fc);
    void DeleteClassFromSchema(FdoClassDefinition* fc);
    void DeleteClassFromSchema(const wchar_t* fcName);
    void UpdateClassFromSchema(FdoClassCollection* classes, FdoClassDefinition* fc, FdoClassDefinition* mainfc);
    bool GetExtentAndCountInfo(FdoFeatureClass* fc, FdoFilter* filter, bool isExtentReq, FdoInt64* countReq, DBounds* extReq);
    void GenerateAutoGenerateTrigger(FdoClassDefinition* fc, bool dropTriggerFirst = false);

    void CollectBaseClassProperties(FdoClassCollection* myclasses, FdoClassDefinition* fc, FdoClassDefinition* mainfc, 
        StringBuffer& sb, int mode, UniqueConstraints& simpleUniqueConstr);
    void AddClassPrimaryKeys(FdoClassDefinition* fc, StringBuffer& sb);
    void AddPropertyConstraintDefaultValue(FdoDataPropertyDefinition* prop, StringBuffer& sb);
    void AddPropertyUniqueConstraint(UniqueConstraints& simpleUniqueConstr, FdoDataPropertyDefinition* prop, StringBuffer& sb);
    void AddComplexUniqueConstraints(FdoUniqueConstraintCollection* uniqueConstr, FdoClassDefinition* fc, StringBuffer& sb);
    std::wstring GenerateValidConstrName(FdoString* name);
    RowidIterator* GetScrollableIterator(SltReader* rdr);
    static int PrepareSpatialDatabase(sqlite3* db, bool useFdoMetadata, bool isInMemory = false);

    static void update_hook(void* caller, int action, char const* database, char const* tablename, sqlite3_int64 id);
    static int commit_hook(void* caller);
    static void rollback_hook(void* caller);

    bool                                    m_updateHookEnabled;
    bool                                    m_changesAvailable;
    bool                                    m_isReadOnlyConnection;

    //sqlite3*                                m_dbRead;
    sqlite3*                                m_dbWrite;

    std::map<std::wstring, std::wstring>*   m_mProps;
    std::wstring                            m_connStr;
    FdoConnectionState                      m_connState;
    FdoFeatureSchemaCollection*             m_pSchema;

    MetadataCache                           m_mNameToMetadata;
    SpatialIndexCache                       m_mNameToSpatialIndex;
    MaxRecordInfoCache                      m_mTableRecInfo;
    QueryCache                              m_mCachedQueries;

    SltCapabilities*                        m_caps;

    bool                                    m_bUseFdoMetadata;
    bool                                    m_bHasFdoMetadata;
    char                                    m_cSupportsDetGeomType;
    SQLiteActiveTransactionType             m_transactionState;
    // geometry conversion buffer
    unsigned char*                          m_wkbBuffer;
    int                                     m_wkbBufferLen;
    ConnInfoDetails*                        m_connDet;
    int                                     m_defSpatialContextId;
};

class ConnInfoDetails
{
public:
    ConnInfoDetails(SltConnection* conn);
    bool IsCoordSysLatLong();

private:
    // don't use add ref here
    SltConnection* m_conn;
    bool m_isInitialized;
    bool m_isCoordSysLatLong; // specify if connection is based on a lat/long CS
    // other parameters needed to pass thru sqlite connection
};
