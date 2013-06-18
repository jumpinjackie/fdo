// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#include "DataDb.h"
#include "DataIO.h"


static FdoDataPropertyDefinitionCollection* FindIDs(FdoClassDefinition* fc)
{
    FdoPtr <FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
    FdoPtr<FdoClassDefinition> base = FDO_SAFE_ADDREF(fc);

    //go up class hierarchy to find base class (it has the identity properties)
    while ((base = base->GetBaseClass()) != NULL)
        idpdc = base->GetIdentityProperties();

    //if (idpdc->GetCount() == 0)
    //    throw FdoException::Create(
    //        NlsMsgGetMain(
    //            FDO_NLSID(SDFPROVIDER_15_NO_IDENTITY_PROPS),
    //            (FdoString*) fc->GetQualifiedName()
    //        )
    //    );

    return (FDO_SAFE_ADDREF (idpdc.p));
}

DataDb::DataDb(SQLiteDataBase* env, const char* filename, FdoString* dbname, bool bReadOnly, 
			   FdoClassDefinition* fc, PropertyIndex* pi, SdfCompareHandler* cmpHandler)
: m_wrtData(256),m_lastRec( 0 ),m_Fc(fc), m_Pi(pi), m_dbname( L"DATA:", dbname, true ) 

{
    m_db = new SQLiteTable(env);

	m_orderingOptions = NULL;
	m_CompareHandler = NULL;
	if( cmpHandler != NULL )
	{
		m_CompareHandler = cmpHandler;
		m_db->SetCompareHandler( this );
	}

    int res;

    int readOnlyFlag = bReadOnly ? SQLiteDB_RDONLY : 0;

	if( ! fc->GetIsAbstract() )
		m_Ids = FindIDs( fc );

    //try to open a database that already exists
	// Note that the nested PhysName call is necessary to reproduce the behavior of the previous versions of the SDF provider.
    if (res = m_db->open(0, filename, (const char*)PhysName("DATA:", (const char*)PhysName(L"",dbname,false),false), (const char*) m_dbname, readOnlyFlag, 0,(cmpHandler!= NULL) ) != 0)
    {
        //must close even if open failed
        m_db->close(0);
        delete m_db;

        m_db = new SQLiteTable(env);
		if( cmpHandler != NULL )
			m_db->SetCompareHandler( this );
        if (bReadOnly)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_4_CONNECTION_IS_READONLY)));

        //if that fails, create one
        if (res = m_db->open(0, filename, (const char*)PhysName("DATA:", (const char*)PhysName(L"",dbname,false),false), (const char*) m_dbname, SQLiteDB_CREATE, 0, (cmpHandler!= NULL) ) != 0)
        {
            //printf("%s\n", env->strerror(res));
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
        }
    }

	m_Offsets1 = m_Offsets2 = NULL;
	if( m_Ids )
	{
		m_Offsets1 = new int[m_Ids->GetCount()];
		m_Offsets2 = new int[m_Ids->GetCount()];
	}
}

DataDb::~DataDb()
{
    m_db->close(0);
    delete m_db;

	if( NULL != m_orderingOptions )
		delete[] m_orderingOptions;

	if( m_Offsets1 )
		delete[] m_Offsets1;
	if( m_Offsets2 )
		delete[] m_Offsets2;
}



REC_NO DataDb::InsertFeature(FdoClassDefinition* clas, PropertyIndex* pi, FdoPropertyValueCollection* pvc)
{
    m_wrtData.Reset();

    DataIO::MakeDataRecord(clas, pi, pvc, m_wrtData);

    //insert feature into database
    SQLiteData keyfeat;
    SQLiteData datafeat(m_wrtData.GetData(), m_wrtData.GetDataLen());

    if (m_db->put(0, &keyfeat, &datafeat, SQLiteDB_APPEND) != 0)
        return (REC_NO)0;

    //endian
    return *(REC_NO*)keyfeat.get_data();
}

//overwrites a feature record with the given data
void DataDb::UpdateFeature(REC_NO recno, SQLiteData* data)
{
    SQLiteData key(&recno, sizeof(REC_NO));

    //overwrite feature record
    if (m_db->put(0, &key, data, 0) != 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_11_ERROR_INSERTING_FEATURE)));
}


//removes the feature with the given record number
void DataDb::DeleteFeature(REC_NO recno)
{    
    SQLiteData key(&recno, sizeof(REC_NO));
    if (m_db->del(0, &key, 0) != 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_12_ERROR_DELETING_FEATURE)));
}


int DataDb::Cursor(SQLiteCursor** cursor, bool write)
{
	return m_db->cursor(0, cursor, write);
}

void DataDb::CloseCursor()
{
	m_db->close_cursor();
    m_lastRec = 0;
}

bool DataDb::CurrentDataIsValid()
{
	return m_db->cursor_is_valid();
}


// The DataDb can be shared by many reader. The tag is used to identify the
// last reader that used the DataDb. 
void DataDb::SetTag( void* tag )
{
	m_tag = tag;
}

void* DataDb::GetTag()
{
	return m_tag;
}

int DataDb::GetFirstFeature( SQLiteData* key, SQLiteData* data )
{
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	if( cur->first() == SQLiteDB_OK )
	{
		int size;
		char *dat;
		if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
		{
			data->set_size(size);
			data->set_data(dat);
			if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
				return SQLiteDB_ERROR;

			key->set_size(size);
			key->set_data(dat);
			m_lastRec = *(REC_NO*)(dat);
			return SQLiteDB_OK;
		}
	}

	return SQLiteDB_NOTFOUND;
}

int DataDb::GetNextFeature( SQLiteData* key, SQLiteData* data )
{
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	if( m_lastRec != *(REC_NO*)(key->get_data() ) )
	{
		bool found;
		// Another reader moved the cursor since the previous call or since the call to first.
		// Need to set the cursor to the previous position
		if( cur->move_to( key->get_size(), (unsigned char*)key->get_data(), found ) != SQLiteDB_OK || ! found )
			return SQLiteDB_ERROR;
	}

	if( cur->next() == SQLiteDB_OK )
	{
		int size;
		char *dat;
		if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
		{
			data->set_size(size);
			data->set_data(dat);
			if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
				return SQLiteDB_ERROR;

			key->set_size(size);
			key->set_data(dat);
			m_lastRec = *(REC_NO*)(dat);

			return SQLiteDB_OK;
		}
	}

	return SQLiteDB_NOTFOUND;
}

int DataDb::GetLastFeature( SQLiteData* key, SQLiteData* data )
{
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	bool empty = false;
	if( cur->last( empty ) == SQLiteDB_OK && !empty )
	{
		int size;
		char *dat;
		if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
		{
			data->set_size(size);
			data->set_data(dat);
			if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
				return SQLiteDB_ERROR;

			key->set_size(size);
			key->set_data(dat);
			m_lastRec = *(REC_NO*)(dat);
			return SQLiteDB_OK;
		}
	}

	return SQLiteDB_NOTFOUND;
}

int DataDb::GetPreviousFeature( SQLiteData* key, SQLiteData* data )
{
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	if( m_lastRec != *(REC_NO*)(key->get_data() ) )
	{
		bool found;
		// Another reader moved the cursor since the previous call or since the call to first.
		// Need to set the cursor to the previous position
		if( cur->move_to( key->get_size(), (unsigned char*)key->get_data(), found ) != SQLiteDB_OK || ! found )
			return SQLiteDB_ERROR;
	}

	if( cur->prev() == SQLiteDB_OK )
	{
		int size;
		char *dat;
		if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
		{
			data->set_size(size);
			data->set_data(dat);
			if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
				return SQLiteDB_ERROR;

			key->set_size(size);
			key->set_data(dat);
			m_lastRec = *(REC_NO*)(dat);

			return SQLiteDB_OK;
		}
	}

	return SQLiteDB_NOTFOUND;
}

// Not the best performing method as it make a table scan to find a particular record
int DataDb::FindFeatureAt(SQLiteData* key, SQLiteData* data, FdoPropertyValueCollection* pvc)
{
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	if( cur->first() == SQLiteDB_OK )
	{
		do {
			int size;
			char *dat;
			if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
					return SQLiteDB_ERROR;
			if( compare( size, dat, pvc ) == 0 )
			{
				key->set_size(size);
				key->set_data(dat);
				m_lastRec = *(REC_NO*)(dat);
				if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
				{
					data->set_size(size);
					data->set_data(dat);
					return SQLiteDB_OK;
				}
			}
		}while( cur->next() == SQLiteDB_OK );
	}

	return SQLiteDB_NOTFOUND;
}

int DataDb::GetFeatureAt(SQLiteData* key, SQLiteData* data, FdoPropertyValueCollection* pvc)
{	
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;

    if (NULL != m_Ids)
    {
	    if( pvc->GetCount() != m_Ids->GetCount() )
		    return FindFeatureAt( key, data, pvc );  // Performs a table scan

	    for(int i=0; i<pvc->GetCount(); i++ )
	    {
		    const wchar_t* name1 = FdoPtr<FdoIdentifier>(FdoPtr<FdoPropertyValue>(pvc->GetItem(i))->GetName())->GetName();
		    const wchar_t* name2 = FdoPtr<FdoDataPropertyDefinition>(m_Ids->GetItem(i))->GetName();
		    if( wcscmp(name1,name2) )
			    return FindFeatureAt( key, data, pvc );  // Performs a table scan
	    }
    }
	BinaryWriter wrtkey(64);
	DataIO::MakeKey(m_Fc, m_Pi, pvc, wrtkey, 1);

	key->set_data(wrtkey.GetData());
	key->set_size(wrtkey.GetDataLen());
	
	bool found;
		
	if( cur->move_to( key->get_size(), (unsigned char*)key->get_data(), found ) != SQLiteDB_OK || ! found )
		return SQLiteDB_ERROR;

	int size;
	char *dat;
	if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
	{
		data->set_size(size);
		data->set_data(dat);
		if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
			return SQLiteDB_ERROR;

		key->set_size(size);
		key->set_data(dat);
		m_lastRec = *(REC_NO*)(dat);

		return SQLiteDB_OK;
	}

	return SQLiteDB_NOTFOUND;
}

int DataDb::GetFeatureAt( SQLiteData* key, SQLiteData* data )
{	
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	bool found;
		
	if( cur->move_to( key->get_size(), (unsigned char*)key->get_data(), found ) != SQLiteDB_OK || ! found )
		return SQLiteDB_ERROR;

	int size;
	char *dat;
	if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
	{
		data->set_size(size);
		data->set_data(dat);
		
		m_lastRec = *(REC_NO*)(key->get_data());

		return SQLiteDB_OK;
	}

	return SQLiteDB_NOTFOUND;
}

REC_NO DataDb::InsertFeatureExclusive(FdoClassDefinition* clas, PropertyIndex* pi, FdoIFeatureReader* reader, FdoPropertyValueCollection* defaultPvc)
{
    m_wrtData.Reset();

    DataIO::MakeDataRecord(clas, pi, reader, defaultPvc, m_wrtData);

    //insert feature into database
    SQLiteData keyfeat;
//
	BinaryWriter wrtkey(64);
	if( m_Ids && m_CompareHandler!= NULL )
	{
		DataIO::MakeKey(clas, reader, wrtkey);
		keyfeat.set_data(wrtkey.GetData());
		keyfeat.set_size(wrtkey.GetDataLen());
	}
//
    SQLiteData datafeat(m_wrtData.GetData(), m_wrtData.GetDataLen());

    if (m_db->put_exclusive(0, &keyfeat, &datafeat, SQLiteDB_APPEND) != 0)
        return (REC_NO)0;

    //endian
    return *(REC_NO*)keyfeat.get_data();
}

void DataDb::SetOrderingOptions( std::map<std::wstring, int> *opts ) 
{ 
	//m_orderingOptions = opts; 
	if( NULL != m_orderingOptions )
		delete[] m_orderingOptions;
    m_orderingOptions = NULL;
    if (NULL != m_Ids)
    {
	    m_orderingOptions = new int[m_Ids->GetCount()];
	    for( int i=0; i<m_Ids->GetCount(); i++ )
	    {
		    FdoPtr<FdoDataPropertyDefinition>id = m_Ids->GetItem(i);
		    std::map<std::wstring, int>::iterator iter = opts->find( id->GetName() );
		    if( iter != opts->end() )
			    m_orderingOptions[i] = (FdoOrderingOption)iter->second;
		    else
			    m_orderingOptions[i] = FdoOrderingOption_Ascending;
	    }
    }
}

void DataDb::Drop()
{

    if ( m_db->Drop() != 0 ) {
        throw FdoException::Create(
            NlsMsgGetMain(
                FDO_NLSID(SDFPROVIDER_81_DROP_TABLE),
                L"Data",
                (FdoString*) m_dbname
            )
        );
    }
}

FdoString* DataDb::GetDbName()
{
    return (FdoString*) m_dbname;
}

void DataDb::Flush() 
{ 
    m_lastRec = 0;
    m_db->flush(); 
}

int DataDb::compare(int size1,const void* data1,int size2,const void* data2)
{
  if( m_CompareHandler == NULL )
  {
	  int c;
	  c = memcmp(data1, data2, size1<size2 ? size1 : size2);
	  if( c==0 ){
		c = size1 - size2;
	  }
	  return c;
  }

  int ret = 0; // Equals
  m_Reader1.Reset((unsigned char*)data1, size1);
  m_Reader2.Reset((unsigned char*)data2, size2);
  
  if (NULL == m_Ids)
    return -1;
  if( m_Ids->GetCount() > 1 )
  {
	for( int i=0; i<m_Ids->GetCount(); i++ )
	{
		m_Offsets1[i] = m_Reader1.ReadInt32();
		m_Offsets2[i] = m_Reader2.ReadInt32();
	}
  }
  for( int i=0; i<m_Ids->GetCount() && ret == 0; i++ )
  {
	  FdoPtr<FdoDataPropertyDefinition>id = m_Ids->GetItem(i);
	  switch (id->GetDataType())
	  {
	  case FdoDataType_Boolean : 
	  case FdoDataType_Byte : 
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadByte(), m_Reader2.ReadByte());
		  break;

	  case FdoDataType_DateTime :
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadDateTime(), m_Reader2.ReadDateTime());
		  break;

	  case FdoDataType_Decimal :		  
	  case FdoDataType_Double :
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadDouble(), m_Reader2.ReadDouble());
		  break;

	  case FdoDataType_Int16 : 
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadInt16(), m_Reader2.ReadInt16() );
		  break;

	  case FdoDataType_Int32 : 
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadInt32(), m_Reader2.ReadInt32() );
		  break;

	  case FdoDataType_Int64 : 
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadInt64(), m_Reader2.ReadInt64() );
		  break;

	  case FdoDataType_Single : 
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadSingle(), m_Reader2.ReadSingle() );
		  break;

	  case FdoDataType_String : 
		  ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadRawStringNoCache(m_Offsets1[i+1]-m_Offsets1[i]),
										   m_Reader2.ReadRawStringNoCache(m_Offsets2[i+1]-m_Offsets2[i]) );
		  break;

	  default:
		  throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_14_UNKNOWN_DATA_TYPE)));
		  break;
	  }
	  if( m_orderingOptions[i] == FdoOrderingOption_Descending )
		ret *= -1; 
  }

  return ret;
}

int DataDb::compare(int size1, const void* data1, FdoPropertyValueCollection* pvc)
{
  if( m_CompareHandler == NULL || m_Ids == NULL)
	return -1; // requires a m_CompareHandler

  int ret = 0; // Equals
  m_Reader1.Reset((unsigned char*)data1, size1);
  
  if( m_Ids->GetCount() > 1 )
  {
	for( int i=0; i<m_Ids->GetCount(); i++ )
		m_Offsets1[i] = m_Reader1.ReadInt32();
  }
  for( int i=0; i<m_Ids->GetCount() && ret == 0; i++ )
  {
	  FdoPtr<FdoDataPropertyDefinition>id = m_Ids->GetItem(i);
	  FdoPtr<FdoPropertyValue>val = pvc->FindItem( id->GetName() );
	  if( val == NULL )
		  continue;

	  FdoDataValue* dv = (FdoDataValue*)(FdoPtr<FdoExpression>(val->GetValue()).p);
      if(dv == NULL || dv->IsNull())
            continue;

	  m_Reader1.SetPosition(m_Offsets1[i]);
	  switch (id->GetDataType())
	  {
	  case FdoDataType_Boolean : 
	  case FdoDataType_Byte : 
		  if( dv->GetDataType() == FdoDataType_Boolean )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadByte(), (FdoByte)(((FdoBooleanValue*)(dv))->GetBoolean() ? 1 : 0));
		  else if( dv->GetDataType() == FdoDataType_Byte )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadByte(), ((FdoByteValue*)(dv))->GetByte());
		  break;

	  case FdoDataType_DateTime :
		  if( dv->GetDataType() == FdoDataType_DateTime )
			ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadDateTime(), ((FdoDateTimeValue*)(dv))->GetDateTime());
		  break;

	  case FdoDataType_Decimal :		  
	  case FdoDataType_Double :
		  if( dv->GetDataType() == FdoDataType_Decimal )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadDouble(), (double)((FdoDecimalValue*)(dv))->GetDecimal());
		  else if( dv->GetDataType() == FdoDataType_Double )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadDouble(), ((FdoDoubleValue*)(dv))->GetDouble());
		  break;

	  case FdoDataType_Int16 : 
		  if( dv->GetDataType() == FdoDataType_Int16 )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadInt16(), ((FdoInt16Value*)(dv))->GetInt16() );
		  break;

	  case FdoDataType_Int32 : 
		  if( dv->GetDataType() == FdoDataType_Int32 )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadInt32(), ((FdoInt32Value*)(dv))->GetInt32() );
		  break;

	  case FdoDataType_Int64 : 
		  if( dv->GetDataType() == FdoDataType_Int64 )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadInt64(), ((FdoInt64Value*)(dv))->GetInt64() );
		  break;

	  case FdoDataType_Single : 
		  if( dv->GetDataType() == FdoDataType_Single )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadSingle(), ((FdoSingleValue*)(dv))->GetSingle() );
		  break;

	  case FdoDataType_String : 
		  if( dv->GetDataType() == FdoDataType_String )
				ret = m_CompareHandler->Compare(id->GetName(), m_Reader1.ReadRawStringNoCache(m_Offsets1[i+1]-m_Offsets1[i]),
										   ((FdoStringValue*)(dv))->GetString() );
		  break;

	  default:
		  throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_14_UNKNOWN_DATA_TYPE)));
		  break;
	  }
  }

  return ret;
}
