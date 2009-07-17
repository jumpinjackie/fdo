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

//feature reader -- returned when executing a select command
class SltReader :   public FdoIScrollableFeatureReader, 
                    public FdoIDataReader, 
                    public FdoISQLDataReader
{
    public:
        SltReader(  SltConnection*              connection, 
                    const char*                 sql);

        SltReader(  SltConnection*              connection, 
                    sqlite3_stmt*               stmt,
                    bool closeDB);

		SltReader(  SltConnection*              connection, 
                    FdoIdentifierCollection*    props, 
                    const char*                 fcname, 
                    const char*                 where, 
                    SpatialIterator*            si,
                    bool                        useFastStepping,
                    RowidIterator*              ri);

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
         virtual bool                GetBoolean      (FdoString* propertyName);
         virtual FdoByte             GetByte         (FdoString* propertyName);
         virtual FdoDateTime         GetDateTime     (FdoString* propertyName);
         virtual double              GetDouble       (FdoString* propertyName);
         virtual FdoInt16            GetInt16        (FdoString* propertyName);
         virtual FdoInt32            GetInt32        (FdoString* propertyName);
         virtual FdoInt64            GetInt64        (FdoString* propertyName);
         virtual float               GetSingle       (FdoString* propertyName);
         virtual FdoString*          GetString       (FdoString* propertyName);
         virtual FdoLOBValue*        GetLOB          (FdoString* propertyName);
         virtual FdoIStreamReader*   GetLOBStreamReader(FdoString* propertyName );
         virtual bool                IsNull          (FdoString* propertyName);
         virtual FdoIRaster*         GetRaster       (FdoString* propertyName);
         virtual bool                ReadNext();
         virtual void                Close();

    //-------------------------------------------------------
    // FdoIFeatureReader implementation
    //-------------------------------------------------------
         virtual FdoClassDefinition* GetClassDefinition();
         virtual FdoInt32            GetDepth();
         virtual FdoIFeatureReader*  GetFeatureObject(FdoString* propertyName);
         virtual FdoByteArray*       GetGeometry     (FdoString* propertyName);
         virtual const FdoByte*      GetGeometry     (FdoString* propertyName, FdoInt32* len);

    //-------------------------------------------------------
    // FdoIDataReader implementation
    //-------------------------------------------------------

         virtual FdoInt32            GetPropertyCount();
         virtual FdoString*          GetPropertyName (FdoInt32 index);
         virtual FdoDataType         GetDataType     (FdoString* propertyName);
         virtual FdoPropertyType     GetPropertyType (FdoString* propertyName);

    //-------------------------------------------------------
    // FdoISQLDataReader implementation
    //-------------------------------------------------------

         virtual FdoInt32            GetColumnCount();
         virtual FdoString*          GetColumnName   (FdoInt32 index);
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

    //-------------------------------------------------------
    // Access values by index
    //-------------------------------------------------------

         const FdoByte*  GetGeometry(int i, FdoInt32* len);
         FdoInt32        GetInt32(int i);

    protected:

        void DelayedInit(FdoIdentifierCollection* props, const char* fcname, const char* where);

    private:

        std::wstring ExtractExpression(const wchar_t* exp, const wchar_t* propName);
		void InitPropIndex(sqlite3_stmt* pStmt);
        int AddColumnToQuery(const wchar_t* name);
        void Requery2();

        bool PositionScrollable(__int64 index);

        int NameToIndex(const wchar_t* name) 
        { 
            int ret = m_mNameToIndex.GetIndex(name);

            if (ret >= 0)
                return ret;

            return AddColumnToQuery(name);
        }

        SltConnection* m_connection;
        
		StringRec* m_sprops; //cache of unicode string values of columns in the current row
        int m_nMaxProps; //maximum number of columns that could be returned
		
        int             m_eGeomFormat; //indicates encoding of geometry
        unsigned char*  m_wkbBuffer; //geometry conversion buffer
        int             m_wkbBufferLen;

        StringBuffer  m_sql;  //the SQL query corresponding to this reader
        sqlite3_stmt* m_pStmt; //the SQL statement corresponding to this reader
        int m_closeOpcode; //we need this to cleanly exit the SQL engine after messing with its bytecodes
        bool m_closeDB; //indicates the statement is based on a memory backed temporary database that we should close when done
        bool m_useFastStepping;

        //things cached/precomputed for speed
        FdoClassDefinition*         m_class;
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

        //stuff related to scrollable readers
        RowidIterator*      m_ri;
        // kept here only for special cases when provider needs it alive
        // due some optimizations to avoid copying a geometry
        FdoFilter*          m_filter;
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
                            const char*                 where);

    virtual ~DelayedInitReader();

    virtual bool ReadNext();

    virtual FdoClassDefinition* GetClassDefinition();

private:

    FdoIdentifierCollection*    m_props;
    std::string                 m_fcname;
    std::string                 m_where;
    bool                        m_bInit;
};

#endif
