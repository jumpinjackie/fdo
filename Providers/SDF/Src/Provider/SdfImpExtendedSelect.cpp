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
#include <iostream>
#include <stdlib.h> 
#include <algorithm>
#include "stdafx.h"
#include "SdfImpExtendedSelect.h"
#include "SdfConnection.h"
#include "SdfQueryOptimizer.h"
#include "SdfIndexedScrollableFeatureReader.h"
#include "DataDb.h"
#include "BinaryReader.h"
#include "FilterExecutor.h"
#include "FdoCommonSchemaUtil.h"
#include "SdfImpScrollableFeatureReader.h"


// Some local types used for sorting
typedef struct _DataProperty_ {
	FdoDataType		type;
	union {
		FdoInt32	intVal;
		FdoInt64	int64Val;
		double		dblVal;
		FdoDateTime* dateVal;
		wchar_t*	strVal;
	} value;
} DataPropertyDef;

typedef struct _SortContext_ {
		REC_NO				*table;
		int					propCount;
		FdoOrderingOption	*options;
		FdoString			**names;
		SdfCompareHandler	*compareHandler;
		DataPropertyDef		**propCache;
} SortContextDef;

// Only needed for Linux to pass the context to qsort compare function.
// Win32 has a qsort_s that takes an extra parameter for the context.
#ifndef _WIN32
typedef struct _SortElement_ {
		REC_NO			index;
		SortContextDef	*ctx;
} SortElementDef;
#endif



static int PositionReader(int recordIndex, BinaryReader*  dataReader, PropertyIndex* pi, int datalength )
{
    //position where offset to property value is stored
    dataReader->SetPosition(sizeof(FCID_STORAGE) + recordIndex * sizeof(int));

    //get offset to property value and position there
    int offset = dataReader->ReadInt32();

    int endoffset = 0;
    if (recordIndex < pi->GetNumProps() - 1)
        endoffset = dataReader->ReadInt32();
    else
        endoffset = datalength;

    //skip to the beginning of the data value
    dataReader->SetPosition(offset);

    //the length of the data
    int len = endoffset - offset;

    return len;
}

// qsort compare function
static int local_compare( void *handler, const void * lhs, const void * rhs )
{
	int retcode = 0;

	SortContextDef  *ctx = (SortContextDef*)handler;
    int idx1 = *(int*)lhs;
	int idx2 = *(int*)rhs;
	for(int i=0; i<ctx->propCount && retcode==0; i++ )
	{
		if( ctx->propCache[idx1][i].type == -1 || ctx->propCache[idx2][i].type == -1 )
        {
            if( ctx->propCache[idx1][i].type == ctx->propCache[idx2][i].type )
                continue;

            if( ctx->propCache[idx1][i].type == -1 )
                retcode = -1;
            else 
                retcode = 1;

			continue;
        }
		switch( ctx->propCache[idx2][i].type )
		{
		  case FdoDataType_DateTime :
			 retcode = ctx->compareHandler->Compare( ctx->names[i], *ctx->propCache[idx1][i].value.dateVal, *ctx->propCache[idx2][i].value.dateVal);
			  break;
		  
		  case FdoDataType_Decimal :
		  case FdoDataType_Single : 
		  case FdoDataType_Double :
			 retcode = ctx->compareHandler->Compare( ctx->names[i], ctx->propCache[idx1][i].value.dblVal, ctx->propCache[idx2][i].value.dblVal);
			  break;

		  case FdoDataType_Boolean : 
		  case FdoDataType_Byte :
		  case FdoDataType_Int16 : 
		  case FdoDataType_Int32 : 
			 retcode = ctx->compareHandler->Compare( ctx->names[i], ctx->propCache[idx1][i].value.intVal, ctx->propCache[idx2][i].value.intVal);
			  break;

		  case FdoDataType_Int64 : 
			 retcode = ctx->compareHandler->Compare( ctx->names[i], ctx->propCache[idx1][i].value.int64Val, ctx->propCache[idx2][i].value.int64Val);
			  break;

		  case FdoDataType_String : 
			  retcode = ctx->compareHandler->Compare( ctx->names[i], ctx->propCache[idx1][i].value.strVal, ctx->propCache[idx2][i].value.strVal);
			  break;

		  default:
			  throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_14_UNKNOWN_DATA_TYPE)));
			  break;
		}
		if( ctx->options[i] == FdoOrderingOption_Descending )
			retcode *= -1; 
	}

    return retcode;
}

#ifndef _WIN32
// Used for Linux only
static int compare( const void * lhs, const void * rhs )
{
	SortElementDef  *elm1 = (SortElementDef*)lhs;
	SortElementDef  *elm2 = (SortElementDef*)rhs;
	return local_compare( (void*)elm1->ctx, (const void*) &elm1->index, (const void*) &elm2->index );
}
#endif


//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------
// constructs a Select command using the specified connection
SdfImpExtendedSelect::SdfImpExtendedSelect(SdfConnection* connection)
: SdfSelect(connection)
{
    m_orderingOption = FdoOrderingOption_Ascending;
	m_orderingProperties = FdoIdentifierCollection::Create();
	m_compareHandler = SdfCompareHandler::Create();
	m_orderingOptions = new std::map<std::wstring, int>();
}

// default constructor
SdfImpExtendedSelect::SdfImpExtendedSelect()
: SdfSelect(NULL)
{
	m_orderingOption = FdoOrderingOption_Ascending;
	m_orderingOptions = new std::map<std::wstring, int>();
}

// default destructor
SdfImpExtendedSelect::~SdfImpExtendedSelect()
{
	if( m_orderingOptions )
		delete m_orderingOptions;
}

void SdfImpExtendedSelect::SetOrderingOption( FdoString* propertyName, FdoOrderingOption  option )
{
	(*m_orderingOptions)[propertyName] = (int)option;
}

FdoOrderingOption SdfImpExtendedSelect::GetOrderingOption( FdoString* propertyName )
{
	std::map<std::wstring, int>::iterator iter = m_orderingOptions->find( propertyName );
	if( iter == m_orderingOptions->end() )
		return FdoOrderingOption_Ascending; // default
	return (FdoOrderingOption) iter->second;
}

void SdfImpExtendedSelect::ClearOrderingOptions()
{
	m_orderingOptions->clear();
}

FdoIdentifierCollection* SdfImpExtendedSelect::GetOrdering()
{
	return FDO_SAFE_ADDREF(m_orderingProperties.p);
}

void SdfImpExtendedSelect::SetCompareHandler( SdfCompareHandler*  handler )
{
	m_compareHandler = FDO_SAFE_ADDREF(handler);
}

SdfConnection* SdfImpExtendedSelect::CreateCacheFile( FdoClassDefinition* fdoclass, const wchar_t* sdfFile )
{
	bool  inMemory = false;
	bool  fileExits = false;
	wchar_t fullpath[1024];
	SdfConnection*  conn = SdfConnection::Create();
	if( wcscmp(L":memory:",sdfFile)==0)
	{
		wcscpy(fullpath,sdfFile);
		inMemory = true;
	}
	else
	{
#ifdef _WIN32
		_wfullpath(fullpath, sdfFile, 1024);
		FILE *fp = _wfopen(fullpath, L"r");
		if( fp )
		{
			fclose(fp);
			fileExits = true;
		}
	//	_wremove(fullpath)
#else
		char cpath[1024];
		char cfullpath[1024];
		wcstombs(cpath, sdfFile, 1024);
		realpath(cpath, cfullpath);
	    
		mbstowcs(fullpath, cfullpath, 1024);
		FILE *fp = fopen((const char*)FdoString(fullpath), "r");
		if( fp )
		{
			fclose(fp);
			fileExits = true;
		}
	//	unlink( fullpath );
#endif
	}
	std::wstring connStr = std::wstring(L"File=") + std::wstring(fullpath);
	conn->SetConnectionString(connStr.c_str());
    
	if( ! fileExits )
	{
		FdoPtr<FdoICreateDataStore>	pCreateCmd = (FdoICreateDataStore*) conn->CreateCommand(FdoCommandType_CreateDataStore);
	        
		FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pCreateCmd->GetDataStoreProperties();
		int	count;
		FdoString **names = dictionary->GetPropertyNames(count);
		dictionary->SetProperty( names[0], fullpath );
		pCreateCmd->Execute();	

		conn->Open( m_compareHandler );

		FdoPtr<FdoIApplySchema> applyschema = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);
		FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"SDF Cache",L"SDF temporary Cache Schema");
		FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(fdoclass);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();
		if( ! inMemory )
		{
			conn->Close();
			conn->Open( m_compareHandler );
		}
	}
	else
	{
		conn->Open( m_compareHandler );

		// Let's make sure the class exist and has the same definition
		FdoPtr<FdoIDescribeSchema> desc = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
		FdoPtr<FdoFeatureSchemaCollection> schemas = desc->Execute();
		if( schemas->GetCount() == 0 )
			throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_20_SCHEMA_STORAGE_ERROR)));
		FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0); // Should only be one
		FdoPtr<FdoClassCollection> clscol = schema->GetClasses();
		FdoPtr<FdoClassDefinition> clsdef = clscol->FindItem( fdoclass->GetName() );
		if( clsdef  == NULL )
			throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_75_CLASS_NOTFOUND), fdoclass->GetName()));

		bool isSameClass = true;
		// Same base properties
		FdoPtr<FdoReadOnlyPropertyDefinitionCollection> fileBaseProps = clsdef->GetBaseProperties();
		FdoPtr<FdoReadOnlyPropertyDefinitionCollection> newBaseProps = fdoclass->GetBaseProperties();
		isSameClass = ( fileBaseProps->GetCount() == newBaseProps->GetCount() );
		if( isSameClass )
		{
			// Same name and type
			for(int i=0; i<fileBaseProps->GetCount() && isSameClass; i++ )
			{
				FdoPtr<FdoPropertyDefinition> fileProp = fileBaseProps->GetItem(i);
				FdoPtr<FdoPropertyDefinition> newProp = newBaseProps->GetItem(i);
				isSameClass = ( wcscmp(fileProp->GetName(),newProp->GetName()) == 0 && fileProp->GetPropertyType() == newProp->GetPropertyType() );
			}
		}

		// Check the class properties
		FdoPtr<FdoPropertyDefinitionCollection> fileProps = clsdef->GetProperties();
		FdoPtr<FdoPropertyDefinitionCollection> newProps = fdoclass->GetProperties();
		isSameClass = ( fileProps->GetCount() == newProps->GetCount() );
		if( isSameClass )
		{
			// Same name and type
			for(int i=0; i<fileProps->GetCount() && isSameClass; i++ )
			{
				FdoPtr<FdoPropertyDefinition> fileProp = fileProps->GetItem(i);
				FdoPtr<FdoPropertyDefinition> newProp = newProps->GetItem(i);
				isSameClass = ( wcscmp(fileProp->GetName(),newProp->GetName()) == 0 && fileProp->GetPropertyType() == newProp->GetPropertyType() );
			}
		}
	
		if( ! isSameClass )
			throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_76_EXISTING_CLASS_DIFFERENT), fdoclass->GetName()));

	}
	return conn;
}

void SdfImpExtendedSelect::BuildCacheFile( SdfConnection* conn, FdoClassDefinition* clas, FdoIFeatureReader* reader, FdoPropertyValueCollection* defaultPvc )
{
    PropertyIndex* pi = conn->GetPropertyIndex( clas );
	DataDb  *dataDb = conn->GetDataDb( clas );
	dataDb->SetOrderingOptions( m_orderingOptions );
	do
	{
		dataDb->InsertFeatureExclusive(clas, pi, reader, defaultPvc );
	} while( reader->ReadNext() );

	conn->FlushAll(clas);
}
SdfIScrollableFeatureReader* SdfImpExtendedSelect::ExecuteFastScrollable( )
{
    SQLiteData		key;
    SQLiteData		data;
    FdoPtr<FdoClassDefinition> cls = FdoPtr<FdoClassCollection>(
        m_connection->GetSchema()->GetClasses())->GetItem(m_className->GetName());
	KeyDb  *keyDb = this->m_connection->GetKeyDb( cls );
	int ret = keyDb->GetLast( &key, &data );
	if( ret != SQLiteDB_OK )
		return NULL; // Exception here

	int maxsize = *((int*)data.get_data());
	REC_NO *indexTable = NULL;
	indexTable = new REC_NO[maxsize];

	bool found = true;
	SQLiteData		*pkey;
    SQLiteData		*pdata;
	
	found = ( keyDb->GetFirst( &key, &data ) == SQLiteDB_OK );
    if( ! found )
        return NULL;

	pkey = &key;
	pdata = &data;
	
	int i = 0;
	while( found && i <maxsize )
	{
        REC_NO recno = *((REC_NO*)pdata->get_data());
        indexTable[i] = recno;
        i++;
		found = ( keyDb->GetNext( &key, &data ) == SQLiteDB_OK );
		pkey = &key;
		pdata = &data;
	}
	maxsize = i;
	return new SdfImpScrollableFeatureReader<SdfIndexedScrollableFeatureReader>(new SdfIndexedScrollableFeatureReader( 
                        m_connection,cls,indexTable, maxsize) );
}

SdfIScrollableFeatureReader* SdfImpExtendedSelect::ExecuteScrollable()
{
	const wchar_t*	tmpStr;
	BinaryReader	rdr1;
	FdoPtr<SdfSimpleFeatureReader> reader;

	SQLiteData		key;
    SQLiteData		data;
    FdoPtr<FdoClassDefinition> cls = FdoPtr<FdoClassCollection>(
        m_connection->GetSchema()->GetClasses())->GetItem(m_className->GetName());

    FdoPtr<FdoClassDefinition> base = cls->GetBaseClass();
    if( this->m_filter == NULL && m_orderingProperties->GetCount() == 0 && base == NULL )
        return ExecuteFastScrollable();

	
	DataDb  *dataDb = this->m_connection->GetDataDb( cls );
	int ret = dataDb->GetLastFeature( &key, &data );
	if( ret != SQLiteDB_OK )
		return NULL; // Exception here

	int maxsize = *((int*)key.get_data());

	bool getAll = ( this->m_filter == NULL );

	if( ! getAll )
	{
		reader = (SdfSimpleFeatureReader*)SdfSelect::Execute();
		if( reader == NULL || ! reader->ReadNext() )
			return NULL;
	}
	

	SortContextDef		ctx;
	PropertyIndex* pi = m_connection->GetPropertyIndex(cls);
	ctx.compareHandler = m_compareHandler;
	ctx.propCount = m_orderingProperties->GetCount();
    PropertyStub **propStubs = NULL;
    bool isSortingRequired = (ctx.propCount != 0);

    if( isSortingRequired )
    {
        propStubs = new PropertyStub*[ctx.propCount];
	    ctx.options = new FdoOrderingOption[ctx.propCount];
	    ctx.names = new FdoString*[ctx.propCount];

        // initialize the ordering property cache; only needed if we need to order the data
        ctx.propCache = new DataPropertyDef*[maxsize];
    }
    else
    {
        ctx.options = NULL;
        ctx.names = NULL;
        ctx.propCache = NULL;
    }

	int i = 0;
	for(i=0; i<ctx.propCount; i++ )
	{
		FdoPtr<FdoIdentifier>id = m_orderingProperties->GetItem( i );
		propStubs[i] = pi->GetPropInfo(id->GetName());
		ctx.options[i] = GetOrderingOption( id->GetName() );
		ctx.names[i] = new wchar_t[wcslen(id->GetName())+1];
		wcscpy((wchar_t*)ctx.names[i],id->GetName());
	}
	
	ctx.table = new REC_NO[maxsize];
#ifndef _WIN32
	SortElementDef  *indexArray = NULL;
    if( isSortingRequired )
        indexArray = new SortElementDef[maxsize];
#else
	REC_NO *indexTable = NULL;
    if( isSortingRequired )
        indexTable = new REC_NO[maxsize];
#endif
	
        
	bool found = true;
	SQLiteData		*pkey;
    SQLiteData		*pdata;
	if( getAll )
	{
		found = ( dataDb->GetFirstFeature( &key, &data ) == SQLiteDB_OK );
		pkey = &key;
		pdata = &data;
	}
	else
	{
		// ReadNext is called above
		pkey = reader->GetRawKey();
		pdata = reader->GetRawData();
	}
	i = 0;
	while( found && i <maxsize )
	{
        if( pi->GetFCID() == *((unsigned short*)pdata->get_data())  )
        {
		    REC_NO recno = *((REC_NO*)pkey->get_data());
#ifndef _WIN32
            if( isSortingRequired )
            {
		        indexArray[i].index = i;
		        indexArray[i].ctx = &ctx;
            }
#else
            if( isSortingRequired )
		        indexTable[i] = i;
#endif
		    ctx.table[i]= recno;

            // Just get the ids if no ordering is required.
            if( isSortingRequired )
            {
		        ctx.propCache[i] = new DataPropertyDef[ctx.propCount];
		        rdr1.Reset((unsigned char*)pdata->get_data(), pdata->get_size());
		        for(int j=0; j<ctx.propCount; j++ )
		        {
			        PropertyStub  *ps = propStubs[j];
			        int len1 = PositionReader( ps->m_recordIndex, &rdr1, pi, pdata->get_size() );
			        if( len1 == 0 )
			        {
				        if( ps->m_isAutoGen )
				        {
					        ctx.propCache[i][j].type = ps->m_dataType;
					        if( getAll )
						        ctx.propCache[i][j].value.intVal = recno;
					        else
						        ctx.propCache[i][j].value.intVal = reader->GetInt32(ctx.names[j]);
					        continue;
				        }
				        ctx.propCache[i][j].type = (FdoDataType)-1;
				        continue;
			        }
			        ctx.propCache[i][j].type = ps->m_dataType;
			        switch( ps->m_dataType )
			        {
			          case FdoDataType_Boolean : 
			          case FdoDataType_Byte : 
				          ctx.propCache[i][j].value.intVal = (int)rdr1.ReadByte();
				          break;

			          case FdoDataType_DateTime :
				          ctx.propCache[i][j].value.dateVal = new FdoDateTime();
				          *ctx.propCache[i][j].value.dateVal = rdr1.ReadDateTime();
				          break;

			          case FdoDataType_Decimal :		  
			          case FdoDataType_Double :
				          ctx.propCache[i][j].value.dblVal = rdr1.ReadDouble();
				          break;

			          case FdoDataType_Int16 : 
				          ctx.propCache[i][j].value.intVal = rdr1.ReadInt16();
				          break;

			          case FdoDataType_Int32 : 
				          ctx.propCache[i][j].value.intVal = rdr1.ReadInt32();
				          break;

			          case FdoDataType_Int64 : 
				          ctx.propCache[i][j].value.int64Val = rdr1.ReadInt64();
				          break;

			          case FdoDataType_Single :
				          ctx.propCache[i][j].value.dblVal = rdr1.ReadSingle();
				          break;

			          case FdoDataType_String : 
				          tmpStr = rdr1.ReadRawStringNoCache(len1);
				          ctx.propCache[i][j].value.strVal = new wchar_t[wcslen(tmpStr)+1];
				          wcscpy( ctx.propCache[i][j].value.strVal , tmpStr );
				          break;

			          default:
				          throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_14_UNKNOWN_DATA_TYPE)));
				          break;
			        }
		        }
            }
            i++;
        }
		// get the next record
		if( getAll )
		{
			found = ( dataDb->GetNextFeature( &key, &data ) == SQLiteDB_OK );
			pkey = &key;
			pdata = &data;
		}
		else
		{
			found = reader->ReadNext();
			if( found )
			{
				pkey = reader->GetRawKey();
				pdata = reader->GetRawData();
			}
		}
		
	}
	maxsize = i;

	// Build the sorted list
	REC_NO *sortedTable = NULL;
    if( ! isSortingRequired )
        sortedTable = ctx.table;
    else
    {
        sortedTable = new REC_NO[maxsize];
#ifndef _WIN32
	    qsort( (void*)indexArray, maxsize, sizeof(SortElementDef), compare );
	    for(i=0;i<maxsize;i++) sortedTable[i] = ctx.table[indexArray[i].index];
	    delete[] indexArray;
#else
	    qsort_s( indexTable, maxsize, sizeof(REC_NO), local_compare, (void*)&ctx );
	    for(i=0;i<maxsize;i++) sortedTable[i] = ctx.table[indexTable[i]];
	    delete[] indexTable;
#endif
    }
	for( i=0; i<ctx.propCount;i++)
		delete[] ctx.names[i];

    if( ctx.names )
	    delete[] ctx.names;

    if( ctx.table && isSortingRequired )
	    delete[] ctx.table;

    if( ctx.options )
	    delete[] ctx.options;

    if( propStubs )
	    delete[] propStubs;

	for(i=0; i<maxsize && ctx.propCount != 0;i++)
	{
		for(int j=0; j<ctx.propCount; j++ )
		{
			if( ctx.propCache[i][j].type == FdoDataType_String && ctx.propCache[i][j].value.strVal != NULL )
				delete[] ctx.propCache[i][j].value.strVal;
			else if ( ctx.propCache[i][j].type == FdoDataType_DateTime && ctx.propCache[i][j].value.dateVal != NULL )
				delete ctx.propCache[i][j].value.dateVal;
		}
		delete[] ctx.propCache[i];
	}
    if( ctx.propCache != NULL )
	    delete[] ctx.propCache;
	return new SdfImpScrollableFeatureReader<SdfIndexedScrollableFeatureReader>(new SdfIndexedScrollableFeatureReader( m_connection,cls,sortedTable, maxsize) );
}

// Executes the select command and returns a reference to an SdfIScrollableFeatureReader.
 SdfIScrollableFeatureReader* SdfImpExtendedSelect::ExecuteScrollable( const wchar_t* sdfCacheFile, 
					FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults )
{
	FdoPtr<FdoIFeatureReader> reader = SdfSelect::Execute();
	if( reader == NULL )
		return NULL;

	return ExecuteScrollable( reader, sdfCacheFile, extendedProps, extendedDefaults );
}

 SdfIScrollableFeatureReader* SdfImpExtendedSelect::ExecuteScrollable( FdoIFeatureReader* reader, const wchar_t* sdfCacheFile, 
									FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults )
{
	if( ! reader->ReadNext() )
		return NULL;

	FdoPtr<FdoClassDefinition> fdoclass = reader->GetClassDefinition();

	fdoclass = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(fdoclass);
	//Add the extended properties
	if( extendedProps != NULL && extendedProps->GetCount() != 0 )
	{
		for(int i=0; i<extendedProps->GetCount(); i++ )
		{
			FdoPtr<FdoDataPropertyDefinition>prop = extendedProps->GetItem(i);
			FdoPtr<FdoPropertyDefinitionCollection>(fdoclass->GetProperties())->Add( prop );
		}
	}
	FdoPtr<FdoDataPropertyDefinitionCollection> properties = fdoclass->GetIdentityProperties();
	for(int i=m_orderingProperties->GetCount()-1; i>=0; i-- )
	{
		FdoPtr<FdoIdentifier>id = m_orderingProperties->GetItem( i );
		FdoPtr<FdoPropertyDefinition>prop = FdoPtr<FdoPropertyDefinitionCollection>(fdoclass->GetProperties())->FindItem( id->GetName() );
		if( prop == NULL || prop->GetPropertyType() != FdoPropertyType_DataProperty )
			throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_77_ORDERING_PROPERTY), id->GetName()));

		// Remove it and insert it in the right order
		if( properties->Contains( id->GetName() ) )
			properties->Remove((const FdoDataPropertyDefinition*) prop.p );
		
		properties->Insert( 0, (FdoDataPropertyDefinition*)(prop.p) );
	}
	

	// Override any autogen id as we want to preserve the original id values.
	for(int i=0; i<properties->GetCount(); i++ )
		FdoPtr<FdoDataPropertyDefinition>(properties->GetItem(i))->SetIsAutoGenerated(false);

	FdoPtr<SdfConnection>conn = CreateCacheFile( fdoclass, sdfCacheFile );

	FdoPtr<FdoClassDefinition> clas = FdoPtr<FdoClassCollection>(
        conn->GetSchema()->GetClasses())->GetItem(fdoclass->GetName());

	BuildCacheFile( conn, clas, reader, extendedDefaults );

	return new SdfImpScrollableFeatureReader<SdfScrollableFeatureReader>(new SdfScrollableFeatureReader( conn, clas ));
}

