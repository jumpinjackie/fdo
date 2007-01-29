#ifndef FDORDBMSFEATUREREADER_H
#define FDORDBMSFEATUREREADER_H     1
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
 */

#include "FdoRdbms.h"
#include "FdoRdbmsUtil.h"
#include "../../Gdbi/GdbiQueryResult.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include "../../Gdbi/GdbiStatement.h"
#include <Sm/Lp/PropertyMappingConcrete.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include "FdoRdbmsFilterProcessor.h"

class FdoRdbmsConnection;
class ISchemaHandler;
class DbiConnection;
class FdoRdbmsDataReader;

//#include "../../../src/fdo/Rdbms/SchemaMgr/LogicalPhysical/AssociationPropertyDefinition.h"
#include <map>



#define     QUERY_CACHE_SIZE    10   // The number of cached cursors to handle the attributes.
                                     // TODO: externilize the caching parmameter. May be make them a
                                     // them a conenction string optional parameter.

typedef  struct _col_cache_ {
      char      col_name[GDBI_COLUMN_NAME_SIZE];
      int       type;
      int       size;
  } ColDef;

  typedef  struct _query_cache_ {
      char      class_name[GDBI_SCHEMA_ELEMENT_NAME_SIZE];
      GdbiQueryResult *query;
      GdbiStatement *statement;
      int       mColCount;
      ColDef    *mColList;
  } AttributeQueryDef;

typedef struct {
    wchar_t     *value;
    size_t      len;
} ValueDef;

typedef struct {
	wchar_t           propertyName[GDBI_SCHEMA_ELEMENT_NAME_SIZE];
    char              columnQName[GDBI_SCHEMA_ELEMENT_NAME_SIZE*2 + 1];
	wchar_t           columnNameW[GDBI_SCHEMA_ELEMENT_NAME_SIZE * sizeof(wchar_t) + 1]; // Not table qualified
	wchar_t           columnPosition[3 * sizeof(wchar_t) + 1];  // 1- based
    FdoPropertyType   propertyType;
} FdoRdbmsPropertyInfoDef;

typedef std::map<std::string, ValueDef *> StrMap;
typedef std::pair<std::string, ValueDef *> ModulePair;
typedef StrMap::iterator StringMapIterator;


class StringMap
{
public:
    ~StringMap()
    {
        StringMapIterator   mapIterator;

        mapIterator = mStringMap.begin();
        while (mapIterator != mStringMap.end())
        {
            ValueDef *prop = mapIterator->second;
            delete [] prop->value;
            delete prop;
            mapIterator++;
        }
    }

//
// Add the string to the map
// If the column alreadys exists, then replace the value
//
    wchar_t *AddtoMap(char *colName, const wchar_t *value, FdoRdbmsUtil* util)
    {
        wchar_t *colValue = NULL;
        ValueDef *val = NULL;
        StringMapIterator   mapIterator;
        mapIterator = mStringMap.find(colName);
        if (mapIterator == mStringMap.end())
        {
            val = new ValueDef();
            val->len = wcslen(value)+1;
            val->value = new wchar_t[val->len];
            mStringMap.insert(ModulePair(colName, val));
        }
        else
        {
            val = mapIterator->second;
            if (wcslen(value) >= val->len)
            {
                delete [] val->value;
                val->len = wcslen(value) + 1;
                val->value = new wchar_t[val->len];
            }
        }
        wcscpy(val->value, value );
        colValue = val->value;
        return colValue;
    }
private:
    StrMap         mStringMap;

};

class FdoRdbmsFeatureReader: public FdoIFeatureReader
{
  friend class FdoRdbmsSelectCommand;
  friend class FdoRdbmsDataReader;

  protected:
      FdoRdbmsFeatureReader()
      {
      }
      FdoRdbmsFeatureReader( FdoIConnection *connection, GdbiQueryResult *queryResult, bool isFeatureQuery, const FdoSmLpClassDefinition *classDef, FdoFeatureSchemaCollection *schmCol, FdoIdentifierCollection *properties = NULL, int level=0, FdoRdbmsSecondarySpatialFilterCollection * secondarySpatialFilters=NULL);

      FdoRdbmsFeatureReader(const FdoRdbmsFeatureReader &right);


      FdoRdbmsFeatureReader & operator=(const FdoRdbmsFeatureReader &right);

      const char*	 Property2ColName( const wchar_t *propName, FdoPropertyType *type, bool *found = NULL, int *index = NULL );
      const wchar_t* Property2ColNameW( const wchar_t *propName, FdoPropertyType *type, bool *found = NULL, int *index = NULL );
      const char*	 Property2ColNameChar( const wchar_t *propName, FdoPropertyType *type, bool *found = NULL, int *index = NULL );

      const char* GetDbAliasName( const wchar_t *propName, FdoPropertyType *type = NULL );

      int GetAttributeQuery( wchar_t* className );

      bool GetIfFeatId( const wchar_t *propertyName, long *featId );

      void FetchProperties ( );

      void ThrowPropertyNotFoundExp( const wchar_t* propertyName, FdoException* exc = NULL);

      void AddToList(FdoPropertyDefinitionCollection *propertyDefinitions, FdoPropertyDefinition *propertyDefinition);

      FdoIFeatureReader* GetAssociatedObject( const FdoSmLpAssociationPropertyDefinition *propertyDefinition );

	  // Derive the given expression's property type and data type:
	  void GetExpressionType(FdoIConnection* connection, FdoClassDefinition* classDef, const char* colName, FdoExpression* expr, FdoPropertyType &propType, FdoDataType &dataType);

protected:
    virtual ~FdoRdbmsFeatureReader();
    virtual void Dispose();

private:
    // This is an internal method to support the DataReader
    FdoInt32 GetPropertyCount();
    // This is an internal method to support the DataReader
    FdoString* GetPropertyName(FdoInt32 index);
    // This is an internal method to support the DataReader
    FdoDataType GetDataType(FdoString* propertyName);
    // This is an internal method to support the DataReader
    FdoPropertyType GetPropertyType(FdoString* propertyName);

    // Methods in aid of overriding the Active spatial context
    void ChangeActiveSpatialContext();
    void RestoreActiveSpatialContext();
    // Method in aid of GetGeometry()
    FdoByteArray* GetGeometry(const wchar_t* propertyName, bool checkIsNullOnly);
    FdoByteArray* GetGeometry(const wchar_t* propertyName, bool checkIsNullOnly, GdbiQueryResult *query);

    FdoClassDefinition* FilterClassDefinition( FdoClassDefinition* classDef, bool isBaseClass = false );

public:

        virtual FdoClassDefinition* GetClassDefinition();
        virtual int GetDepth();
        virtual bool     GetBoolean( const wchar_t *propertyName );
        virtual FdoByte  GetByte( const wchar_t *propertyName );
        virtual double   GetDouble(const wchar_t* propertyName);
        virtual short    GetInt16( const wchar_t *propertyName );
        virtual int      GetInt32( const wchar_t *propertyName );
        virtual FdoInt64 GetInt64( const wchar_t *propertyName );
        virtual float    GetSingle( const wchar_t *propertyName );
        virtual const wchar_t* GetString( const wchar_t *propertyName );

        virtual FdoLOBValue* GetLOB(const wchar_t* propertyName );
        virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

        virtual bool     IsNull( const wchar_t *propertyName );
        virtual FdoIFeatureReader* GetFeatureObject(const wchar_t* propertyName);
        virtual FdoByteArray* GetGeometry(const wchar_t* propertyName);
        virtual const FdoByte * GetGeometry(const wchar_t* propertyName, FdoInt32 * count);
        virtual FdoIRaster* GetRaster(const wchar_t* propertyName);
        virtual bool     ReadNext( );
        virtual void     Close();


        virtual FdoDateTime GetDateTime(const wchar_t *propertyName );


  protected:

      virtual bool       ReadNextWithLocalFilter();

      // Returns true if column should be skipped during column-to-property mapping.
      // This is an internal method to support the DataReader.
      virtual bool       SkipColumnForProperty(FdoInt32 index);

      int                mQid;
      GdbiQueryResult   *mQueryResult;
      int                mNextQidToFree;
      int                mAttrsQidIdx;
      bool               mPropertiesFetched;

      FdoPropertyValueCollection    *mCurrentFeatId; // Used to maintain the feature id name/value pairs
      bool               mHasMoreFeatures;
      bool               mIsFeatureQuery;   // Used to indicate that this reader is for a feature query
      wchar_t            mCurrentClassName[GDBI_SCHEMA_ELEMENT_NAME_SIZE]; // Contains the class name of the current record(feature)
      const FdoSmLpClassDefinition *mClassDefinition;
      int               mCurrentRevisionNumber;


      // The next three member variable are used to manage the attribute query cache.
      AttributeQueryDef  mAttrQueryCache[ QUERY_CACHE_SIZE ];
      wchar_t            mLastClassName[GDBI_SCHEMA_ELEMENT_NAME_SIZE]; // used to handle the case where many successive features of the same type are returned
      int                mLastAttrQueryIdx;

      DbiConnection     *mConnection; // The dbi connection that should be used for any dbi lever access
      FdoRdbmsConnection *mFdoConnection;

      FdoFeatureSchemaCollection     *mSchemaCollection; // Used to cach the schema definition
      bool              mCurrentRevisionNumberValid;
      FdoIdentifierCollection *mProperties; // list of properties to be returned
      int               mLevel; //
      FdoClassDefinition *mFdoClassDefinition;

      FdoByteArray *    mGeometryCache; // Last fetched geometry (in aid of FdoByte * GetGeometry())

      // The next 3 member variable are mostly needed to support the DataReader
      int               mColCount;
      int               mUnskippedColCount;
      GdbiColumnDesc    *mColList;

      FdoStringP        mFeatIdColName;
      FdoStringP        mClassIdColName;
      FdoStringP        mRevNumColName;

      StringMap         mStringMap;

      char              mTmpStringValue[GDBI_MAXIMUM_STRING_SIZE+1];
      int               mFeatNum;

      dbi_pn_id_t       mOldActiveSC;
      dbi_pn_id_t       mNewActiveSC;

    // List of secondary spatial filters that apply to this filter.  The current way of storing 
    // them does not include structure of binary conditions, so it is assumed that these
    // are all applied equally ("and" of all conditions must be satisfied in any 
    // secondary spatial filtering).
    FdoRdbmsSecondarySpatialFilters     mSecondarySpatialFilters;

	// A cache of property definition names to avoid expensive string conversions
	FdoRdbmsPropertyInfoDef   *mPropertyInfoDefs;
	int                       mNumPropertyInfoDefs;
	int                       mLastPropertyInfoDef;
	int						  m_cacheHits;
	int						  m_cacheMissed1;
	int						  m_cacheMissed2;
};

#endif // FDORDBMSFEATUREREADER_H

