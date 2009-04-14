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

#define PROP_NAME_FILENAME L"File"
#define PROP_NAME_FDOMETADATA L"UseFdoMetadata"

#include "SltCapabilities.h"


class SltMetadata;
class SpatialIndex;
class SltReader;
class SltExtendedSelect;
struct NameOrderingPair;
class StringBuffer;


struct string_less
{	// functor for operator<
bool operator()(const char* _Left, const char* _Right) const
	{	// apply operator< to operands
	    return strcmp(_Left, _Right) < 0;
	}
};
typedef std::map<char*, sqlite3_stmt*, string_less> QueryCache;
typedef std::map<char*, SltMetadata*, string_less> MetadataCache;
typedef std::map<char*, SpatialIndex*, string_less> SpatialIndexCache;


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
    virtual FdoITransaction* BeginTransaction()                 { return NULL; }
    virtual FdoPhysicalSchemaMapping* CreateSchemaMapping()     { return NULL; }
    virtual void SetConfiguration(FdoIoStream* stream)          { }
    virtual void Flush()                                        { }


    //-------------------------------------------------------
    // FdoIConnectionInfo implementation
    //-------------------------------------------------------

    virtual FdoString* GetProviderName()                { return L"OSGeo.SQLite.3.4"; }
    virtual FdoString* GetProviderDisplayName()         { return L"OSGeo FDO Provider for SQLite (Spatial)"; }
    virtual FdoString* GetProviderDescription()         { return L"FDO Access to SQLite Data Sources"; }
    virtual FdoString* GetProviderVersion()             { return L"3.4.0.0"; }
    virtual FdoString* GetFeatureDataObjectsVersion()   { return L"3.4.0.0"; }
    
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
                        DescribeSchema          (FdoStringCollection* classNames);
    
    FdoISpatialContextReader* 
                        GetSpatialContexts      ();
    
    void                CreateDatabase          ();

    SltReader*  Select                         (FdoIdentifier*           fcname, 
                                                FdoFilter*               filter, 
                                                FdoIdentifierCollection* props,
                                                bool                     scrollable,
                                                const std::vector<NameOrderingPair>& ordering);

    FdoIDataReader*     SelectAggregates       (FdoIdentifier*             fcname, 
                                                FdoIdentifierCollection*   properties,
                                                bool                       bDistinct,
                                                FdoOrderingOption          eOrderingOption,
                                                FdoIdentifierCollection*   ordering,
                                                FdoFilter*                 filter,
                                                FdoIdentifierCollection*   grouping);

    FdoInt32            ExecuteNonQuery        (FdoString* sql);
    FdoISQLDataReader*  ExecuteReader          (FdoString* sql);

    FdoInt32            Update                 (FdoIdentifier*              fcname, 
                                                FdoFilter*                  filter, 
                                                FdoPropertyValueCollection* propvals);
    
    FdoInt32            Delete                 (FdoIdentifier*              fcname, 
                                                FdoFilter*                  filter);


    void                ApplySchema            (FdoFeatureSchema* schema);

    sqlite3*        GetDbRead() { return m_dbRead; }
    sqlite3*        GetDbWrite() { return m_dbWrite; }
    SpatialIndex*   GetSpatialIndex(const char* table);
    void            GetExtents(const wchar_t* fcname, double ext[4]);
    SltMetadata*    GetMetadata(const char* table);
    SltReader*      CheckForSpatialExtents(FdoIdentifierCollection* props, FdoFeatureClass* fc);
    int             GetFeatureCount(const char* table);
    
    sqlite3_stmt*   GetCachedParsedStatement(const char* sql);
    void            ReleaseParsedStatement(const char* sql, sqlite3_stmt* stmt);
    void            ClearQueryCache();
    
    void UpdateClassesWithInvalidSC();

private :

    void AddGeomCol(FdoGeometricPropertyDefinition* gpd, const wchar_t* fcname);
    void AddDataCol(FdoDataPropertyDefinition* dpd, const wchar_t* fcname);
    bool IsClassEqual(FdoClassDefinition* fc1, FdoClassDefinition* fc2);
    bool IsPropertyEqual(FdoPropertyDefinition* prop1, FdoPropertyDefinition* prop2);
    bool IsTableEmpty(FdoString* name);

    int FindSpatialContext(const wchar_t* name);
    int GetDefaultSpatialContext();

    void CollectBaseClassProperties(FdoClassCollection* myclasses, FdoClassDefinition* fc, FdoClassDefinition* mainfc, StringBuffer& sb, int mode);
    void AddPropertyConstraintDefaultValue(FdoDataPropertyDefinition* prop, StringBuffer& sb);

    sqlite3*                                m_dbRead;
    sqlite3*                                m_dbWrite;

    std::map<std::wstring, std::wstring>*   m_mProps;
    std::wstring                            m_connStr;
    FdoConnectionState                      m_connState;
    FdoFeatureSchemaCollection*             m_pSchema;

    MetadataCache                           m_mNameToMetadata;
    SpatialIndexCache                       m_mNameToSpatialIndex;
    QueryCache                              m_mCachedQueries;

    SltCapabilities*                        m_caps;

    bool                                    m_bUseFdoMetadata;
    bool                                    m_bHasFdoMetadata;
};
