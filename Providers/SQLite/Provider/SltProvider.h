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

#include "SltCapabilities.h"

class SltMetadata;
class SpatialIndex;
class SltReader;

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

    virtual FdoString* GetProviderName()                { return L"OSGeo.SQLite.1.0"; }
    virtual FdoString* GetProviderDisplayName()         { return L"OSGeo FDO Provider for SQLite (spatial)"; }
    virtual FdoString* GetProviderDescription()         { return L"FDO Access to SQLite Data Sources"; }
    virtual FdoString* GetProviderVersion()             { return L"1.0.0.0"; }
    virtual FdoString* GetFeatureDataObjectsVersion()   { return L"3.3.0.0"; }
    
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
                        DescribeSchema          ();
    
    FdoISpatialContextReader* 
                        GetSpatialContexts      ();
    
    void                CreateDatabase          ();

    FdoIFeatureReader*  Select                 (FdoIdentifier*          fcname, 
                                                FdoFilter*              filter, 
                                                FdoIdentifierCollection* props);

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

    void                AddGeomCol             (FdoGeometricPropertyDefinition* gpd, 
                                                const wchar_t*                  fcname);


    sqlite3*        GetDB() { return m_db; }
    SpatialIndex*   GetSpatialIndex(const char* table);
    SltMetadata*    GetMetadata(const char* table);
    SltReader*      CheckForSpatialExtents(FdoIdentifierCollection* props, FdoFeatureClass* fc);
    
    sqlite3_stmt*   GetCachedParsedStatement(const std::string& sql);
    void            ReleaseParsedStatement(const std::string& sql, sqlite3_stmt* stmt);
    void            ClearQueryCache();
    

private :
    
    sqlite3*                                m_db;

    std::map<std::wstring, std::wstring>*   m_mProps;
    std::wstring                            m_connStr;
    FdoConnectionState                      m_connState;
    FdoFeatureSchemaCollection*             m_pSchema;

    std::map<std::string, SltMetadata*>     m_mNameToMetadata;
    std::map<std::string, SpatialIndex*>    m_mNameToSpatialIndex;
    std::map<std::string, sqlite3_stmt*>    m_mCachedQueries;

    SltCapabilities*                         m_caps;
};
