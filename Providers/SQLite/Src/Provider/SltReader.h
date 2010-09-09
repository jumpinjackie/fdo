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

#ifndef SLT_READER_H
#define SLT_READER_H

//Include our custom string map/list data structures
#include "PropertyNameIndex.h"
#include "StringUtil.h"

class SltConnection;
class SpatialIterator;
class RowidIterator;
class SltIdReader;

struct StringRec
{
	int valid;
	int len;
	wchar_t* data;

	StringRec()
		: valid(0), len(0), data(0)
	{}

	~StringRec()
	{
		delete[] data;
	}

	void EnsureSize(int wcCount)
	{
		if (len < wcCount)
		{
			delete [] data;
			data = new wchar_t[wcCount];
			len = wcCount;
		}
	}
};

enum ReaderCloseType
{
    ReaderCloseType_None,        // no close
    ReaderCloseType_CloseDb,     // close statement and database 
    ReaderCloseType_CloseStmtOnly // close statement only
};

//feature reader -- returned when executing a select command
class SltReader :   public FdoIScrollableFeatureReader, 
                    public FdoIDataReader, 
                    public FdoISQLDataReader
{
    public:
        SltReader(  SltConnection*              connection, 
                    const char*                 sql,
                    FdoParameterValueCollection*  parmValues);

        SltReader(  SltConnection*              connection, 
                    sqlite3_stmt*               stmt,
                    ReaderCloseType closeDB,
                    FdoClassDefinition* cls,
                    FdoParameterValueCollection*  parmValues);

		SltReader(  SltConnection*              connection, 
                    FdoIdentifierCollection*    props, 
                    const char*                 fcname, 
                    const char*                 strWhere, 
                    SpatialIterator*            si,
                    bool                        useFastStepping,
                    RowidIterator*              ri,
                    FdoParameterValueCollection*  parmValues,
                    const char*                 strOrderBy = "");

        virtual ~SltReader();

        void SetInternalFilter(FdoFilter* filter);

protected:
        //for internal use only!
		SltReader(  SltConnection*              connection);


    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------
    
    SLT_IMPLEMENT_REFCOUNTING
        
    //-------------------------------------------------------
    // FdoIReader implementation
    //-------------------------------------------------------

    public:

         virtual bool                GetBoolean      (int index);
         virtual bool                GetBoolean      (FdoString* propertyName);

         virtual FdoByte             GetByte         (int index);
         virtual FdoByte             GetByte         (FdoString* propertyName);

         virtual FdoDateTime         GetDateTime     (int index);
         virtual FdoDateTime         GetDateTime     (FdoString* propertyName);

         virtual double              GetDouble       (int index);
         virtual double              GetDouble       (FdoString* propertyName);

         virtual FdoInt16            GetInt16        (int index);
         virtual FdoInt16            GetInt16        (FdoString* propertyName);

         virtual FdoInt32            GetInt32        (int index);
         virtual FdoInt32            GetInt32        (FdoString* propertyName);

         virtual FdoInt64            GetInt64        (int index);
         virtual FdoInt64            GetInt64        (FdoString* propertyName);

         virtual float               GetSingle       (int index);
         virtual float               GetSingle       (FdoString* propertyName);

         virtual FdoString*          GetString       (int index);
         virtual FdoString*          GetString       (FdoString* propertyName);

         virtual FdoLOBValue*        GetLOB          (int index);
         virtual FdoLOBValue*        GetLOB          (FdoString* propertyName);

         virtual FdoIStreamReader*   GetLOBStreamReader(int index);
         virtual FdoIStreamReader*   GetLOBStreamReader(FdoString* propertyName );

         virtual bool                IsNull          (FdoInt32 index);
         virtual bool                IsNull          (FdoString* propertyName);

         virtual FdoIRaster*         GetRaster       (int index);
         virtual FdoIRaster*         GetRaster       (FdoString* propertyName);

         virtual bool                ReadNext();
         virtual void                Close();

    //-------------------------------------------------------
    // FdoIFeatureReader implementation
    //-------------------------------------------------------
         virtual FdoClassDefinition* GetClassDefinition();
         virtual FdoInt32            GetDepth();

         virtual FdoIFeatureReader*  GetFeatureObject(int index);
         virtual FdoIFeatureReader*  GetFeatureObject(FdoString* propertyName);

         virtual FdoByteArray*       GetGeometry     (int index);
         virtual FdoByteArray*       GetGeometry     (FdoString* propertyName);

         virtual const FdoByte*      GetGeometry     (int index, FdoInt32* len);
         virtual const FdoByte*      GetGeometry     (FdoString* propertyName, FdoInt32* len);

    //-------------------------------------------------------
    // FdoIDataReader implementation
    //-------------------------------------------------------

         virtual FdoInt32            GetPropertyCount();

         virtual FdoString*          GetPropertyName (int index);
         virtual FdoInt32            GetPropertyIndex(FdoString* propertyName);

         virtual FdoDataType         GetDataType     (int index);
         virtual FdoDataType         GetDataType     (FdoString* propertyName);

         virtual FdoPropertyType     GetPropertyType (int index);
         virtual FdoPropertyType     GetPropertyType (FdoString* propertyName);

    //-------------------------------------------------------
    // FdoISQLDataReader implementation
    //-------------------------------------------------------

         virtual FdoInt32            GetColumnCount();
         virtual FdoInt32            GetColumnIndex  (FdoString* columnName);
         virtual FdoString*          GetColumnName   (FdoInt32 index);

         virtual FdoDataType         GetColumnType   (int index);
         virtual FdoDataType         GetColumnType   (FdoString* columnName);

    //-------------------------------------------------------
    // FdoIScrollableFeatureReader implementation
    //-------------------------------------------------------
        virtual int                  Count();
        virtual bool                 ReadFirst();
        virtual bool                 ReadLast();
        virtual bool                 ReadPrevious();
        virtual bool                 ReadAt(FdoPropertyValueCollection* key);
        virtual bool                 ReadAtIndex( unsigned int recordIndex );
        virtual unsigned int         IndexOf(FdoPropertyValueCollection* key);

    protected:

        void DelayedInit(FdoIdentifierCollection* props, const char* fcname, const char* strWhere, const char* strOrderBy = "", bool addPkOnly = false);

    private:
        bool ReadNextOnView();
        int GenerateUniqueName(const wchar_t* pname, FdoPropertyDefinition* prop, FdoPropertyDefinitionCollection* pcol);
        void ValidateIndex(sqlite3_stmt *pStmt, int index);
        std::wstring ExtractExpression(const wchar_t* exp, const wchar_t* propName);
		void InitPropIndex(sqlite3_stmt* pStmt);
        int AddColumnToQuery(const wchar_t* name);
        void Requery2();
        FdoPropertyDefinition* GetFdoProperty(int index);

        bool PositionScrollable(__int64 index);

        int NameToIndex(const wchar_t* name) 
        { 
            int ret = m_mNameToIndex.GetIndex(name);

            if (ret >= 0)
                return ret;

            AddColumnToQuery(name);
            return m_mNameToIndex.GetIndex(name);
        }

        SltConnection* m_connection;
        
		StringRec* m_sprops; //cache of unicode string values of columns in the current row
        int m_nMaxProps; //maximum number of columns that are currently queried
        int m_nTotalProps; //number of properties in the feature class (total that can be queried after Requery())
		
        int             m_eGeomFormat; //indicates encoding of geometry
        unsigned char*  m_wkbBuffer; //geometry conversion buffer
        int             m_wkbBufferLen;

        StringBuffer  m_sql;  //the SQL query corresponding to this reader
        sqlite3_stmt* m_pStmt; //the SQL statement corresponding to this reader
        int m_closeOpcode; //we need this to cleanly exit the SQL engine after messing with its bytecodes
        ReaderCloseType m_closeDB;  //indicates the statement is based on a memory backed temporary database that we should close when done
        bool m_useFastStepping;
        bool m_isViewSelect;
        bool m_canAddSelectProps;

        //things cached/precomputed for speed
        FdoClassDefinition*         m_class;
        // name used only in cases where select is a simple select 
        // and we do not have m_class, e.g.: "select (A+B) as C from TN;"
        // witout this we cannot identify the type of a calculation
        std::string                 m_mainClassName;
        std::vector<const wchar_t*> m_propNames;
        wchar_t*                    m_aPropNames;
        PropertyNameIndex           m_mNameToIndex;

        //things we need for reissuing the sql query while incrementally
        //building a list of properties that the caller accesses, in case
        //they requested all properties in the select command
        StringList m_reissueProps;
        StringBuffer m_fromwhere;

        //stuff related to spatial filters
        SpatialIterator*    m_si;
        int                 m_siEnd;
        sqlite3_int64       m_curfid;
        // kept here only for special cases when provider needs it alive
        // due some optimizations to avoid copying a geometry
        FdoFilter*          m_filter;

        FdoParameterValueCollection*  m_parmValues;
protected:
        //stuff related to scrollable readers
        RowidIterator*      m_ri;
};


//Overloaded reader which delays its initialization
//until ReadNext is called. Returned by Insert commands
//in order to speed them up in the common case where
//the reader returned by IInsert is discarded.
class DelayedInitReader : public SltReader
{
public:

    DelayedInitReader(      SltConnection*              connection, 
                            FdoIdentifierCollection*    props, 
                            const char*                 fcname, 
                            const char*                 where,
                            RowidIterator* ri);

    virtual ~DelayedInitReader();

    virtual bool ReadNext();

    virtual FdoClassDefinition* GetClassDefinition();

private:

    FdoIdentifierCollection*    m_props;
    std::string                 m_fcname;
    std::string                 m_where;
    bool                        m_bInit;
};

// can be used RowidIterator* here but since is not needed let's avoid alloc new memory
// do not use this reader for other things, since is not "smart"
class SltIdReader : public FdoIFeatureReader, 
                    public FdoIDataReader, 
                    public FdoISQLDataReader
{
	FdoDataPropertyDefinition*	m_idProp;
	sqlite3_int64				m_id;
	bool						m_rdrEnd;
	FdoClassDefinition*			m_cls;
public:

	SltIdReader (FdoDataPropertyDefinition* idProp, sqlite3_int64 id);
	virtual ~SltIdReader();

	//-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------
    
    SLT_IMPLEMENT_REFCOUNTING

	bool ReadNext();
    FdoClassDefinition* GetClassDefinition();
	FdoInt32 GetDepth();
	const FdoByte * GetGeometry(FdoString* /*propertyName*/, FdoInt32* /*count*/);
    FdoByteArray* GetGeometry(FdoString* /*propertyName*/);
    FdoIFeatureReader* GetFeatureObject(FdoString* /*propertyName*/);
    const FdoByte * GetGeometry(FdoInt32 /*index*/, FdoInt32* /*count*/);
    FdoByteArray* GetGeometry(FdoInt32 /*index*/);
    FdoIFeatureReader* GetFeatureObject(FdoInt32 /*index*/);
	void CheckProperty(int idx);
	void CheckProperty(FdoString* propertyName);
	FdoInt32 GetColumnCount();
	FdoInt32 GetColumnIndex(FdoString* columnName);
    FdoString* GetColumnName(FdoInt32 index);
	FdoDataType GetColumnType(FdoString* columnName);
	FdoDataType GetColumnType(FdoInt32 index);

	//-------------------------------------------------------
    // FdoIDataReader implementation
    //-------------------------------------------------------
	FdoInt32 GetPropertyCount();
	FdoDataType GetDataType(FdoString* propertyName);
    FdoPropertyType GetPropertyType(FdoString* propertyName);
    FdoDataType GetDataType(FdoInt32 index);
    FdoPropertyType GetPropertyType(FdoInt32 index);
	FdoString* GetPropertyName(FdoInt32 index);
    FdoInt32 GetPropertyIndex(FdoString* propertyName);
    bool GetBoolean(FdoString* /*propertyName*/);
    FdoByte GetByte(FdoString* propertyName);
    FdoDateTime GetDateTime(FdoString* /*propertyName*/);
    double GetDouble(FdoString* /*propertyName*/);
    FdoInt16 GetInt16(FdoString* propertyName);
    FdoInt32 GetInt32(FdoString* propertyName);
    FdoInt64 GetInt64(FdoString* propertyName);
    float GetSingle(FdoString* /*propertyName*/);
    FdoString* GetString(FdoString* /*propertyName*/);
    FdoLOBValue* GetLOB(FdoString* /*propertyName*/);
    FdoIStreamReader* GetLOBStreamReader(FdoString* /*propertyName*/);
    bool IsNull(FdoString* propertyName);
    FdoIRaster* GetRaster(FdoString* /*propertyName*/);
    FdoBoolean GetBoolean(FdoInt32 /*index*/);
    FdoByte GetByte(FdoInt32 index);
    FdoDateTime GetDateTime(FdoInt32 /*index*/);
    FdoDouble GetDouble(FdoInt32 /*index*/);
    FdoInt16 GetInt16(FdoInt32 index);
    FdoInt32 GetInt32(FdoInt32 index);
    FdoInt64 GetInt64(FdoInt32 index);
    FdoFloat GetSingle(FdoInt32 /*index*/);
    FdoString* GetString(FdoInt32 /*index*/);
    FdoLOBValue* GetLOB(FdoInt32 /*index*/);
    FdoIStreamReader* GetLOBStreamReader(FdoInt32 /*index*/);
    FdoBoolean IsNull(FdoInt32 index);
    FdoIRaster* GetRaster(FdoInt32 /*index*/);
    void Close();
};

#endif
