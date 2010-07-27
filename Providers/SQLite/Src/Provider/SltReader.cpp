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
#include "SltReader.h"
#include "SltProvider.h"
#include "SltMetadata.h"
#include "FdoCommonSchemaUtil.h"
#ifndef _MSC_VER
#include "SpatialIndex.h"
#else
#include "DiskSpatialIndex.h"
#endif
#include "SltConversionUtils.h"
#include "RowidIterator.h"
#include "FdoCommonMiscUtil.h"
#include "vdbeInt.h"
#include "SltQueryTranslator.h"
#include "SltBLOBStreamReader.h"

/*
** Check to see if column iCol of the given statement is valid.  If
** it is, return a pointer to the Mem for the value of that column.
** If iCol is not valid, return a pointer to a Mem which has a value
** of NULL.
*/
static Mem *columnMem(sqlite3_stmt *pStmt, int i){
  Vdbe *pVm;
  int vals;
  Mem *pOut;

  pVm = (Vdbe *)pStmt;
  if( pVm && pVm->pResultSet!=0 && i<pVm->nResColumn && i>=0 ){
      //We compile without thread safety anyway...
    //sqlite3_mutex_enter(pVm->db->mutex);
    vals = sqlite3_data_count(pStmt);
    pOut = &pVm->pResultSet[i];
  }else{
    static const Mem nullMem = {{0}, 0.0, 0, (char*)"", 0, MEM_Null, SQLITE_NULL, 0, 0, 0 };
    if( pVm->db ){
        //We compile without thread safety anyway...
      //sqlite3_mutex_enter(pVm->db->mutex);
      sqlite3Error(pVm->db, SQLITE_RANGE, 0);
    }
    pOut = (Mem*)&nullMem;
  }
  return pOut;
}



//constructor taking a general sql statement, which we will step through
SltReader::SltReader(SltConnection* connection, const char* sql, FdoParameterValueCollection*  parmValues)
: m_refCount(1),
m_sql(sql),
m_class(NULL),
m_sprops(NULL),
m_closeOpcode(-1),
m_si(NULL),
m_nMaxProps(0),
m_nTotalProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(ReaderCloseType_None),
m_useFastStepping(false),
m_ri(NULL),
m_filter(NULL),
m_aPropNames(NULL),
m_fromwhere(),
m_isViewSelect(false)
{
	m_connection = FDO_SAFE_ADDREF(connection);
    m_parmValues  = FDO_SAFE_ADDREF(parmValues);

    m_pStmt = m_connection->GetCachedParsedStatement(m_sql.Data());

	InitPropIndex(m_pStmt);
    m_nTotalProps = sqlite3_column_count(m_pStmt);
}

//constructor taking a general sql statement, which we will step through
//Same as above, but this one takes a sqlite3 statement pointer rather than
//a string. This means that it is a statement based on an ephemeral database
//which this reader will close once it is done being read.
SltReader::SltReader(SltConnection* connection, sqlite3_stmt* stmt, ReaderCloseType closeDB, FdoClassDefinition* cls, FdoParameterValueCollection*  parmValues)
: m_refCount(1),
m_sql(""),
m_sprops(NULL),
m_closeOpcode(-1),
m_si(NULL),
m_nMaxProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(closeDB),
m_useFastStepping(false),
m_ri(NULL),
m_aPropNames(NULL),
m_filter(NULL),
m_fromwhere(),
m_isViewSelect(false)
{
	m_connection = FDO_SAFE_ADDREF(connection);
    m_class = FDO_SAFE_ADDREF(cls);
    m_parmValues  = FDO_SAFE_ADDREF(parmValues);
    m_nTotalProps = sqlite3_column_count(stmt);
    m_pStmt = stmt;
	InitPropIndex(m_pStmt);
}


//constructor tailored for an FDO Select command -- in cases where the
//requested columns collection is empty, it will start out with a query
//for just featid and geometry, then redo the query if caller asks for other
//property values
SltReader::SltReader(SltConnection* connection, FdoIdentifierCollection* props, const char* fcname, const char* strWhere, SpatialIterator* si, bool useFastStepping, RowidIterator* ri, FdoParameterValueCollection*  parmValues)
: m_refCount(1),
m_pStmt(0),
m_class(NULL),
m_sprops(NULL),
m_closeOpcode(-1),
m_si(si),
m_nMaxProps(0),
m_nTotalProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(ReaderCloseType_None),
m_useFastStepping(useFastStepping),
m_ri(ri),
m_aPropNames(NULL),
m_filter(NULL),
m_fromwhere(),
m_isViewSelect(false)
{
	m_connection = FDO_SAFE_ADDREF(connection);
    m_parmValues  = FDO_SAFE_ADDREF(parmValues);
    DelayedInit(props, fcname, strWhere);
}

//Same as above but does not initialize the reader.
//Used by the DelayedInitReader, which is returned 
//by the Insert command. It makes insert faster in the
//case where we don't care to read the reader returned
//when executing the insert command
SltReader::SltReader(SltConnection* connection)
: m_refCount(1),
m_pStmt(0),
m_class(NULL),
m_sprops(NULL),
m_closeOpcode(-1),
m_si(NULL),
m_ri(NULL),
m_nMaxProps(0),
m_nTotalProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(ReaderCloseType_None),
m_useFastStepping(true),
m_aPropNames(NULL),
m_filter(NULL),
m_fromwhere(),
m_parmValues(NULL),
m_isViewSelect(false)
{
	m_connection = FDO_SAFE_ADDREF(connection);
}


SltReader::~SltReader()
{
	Close();
    FDO_SAFE_RELEASE(m_filter);
    FDO_SAFE_RELEASE(m_class);
    FDO_SAFE_RELEASE(m_parmValues);
    delete m_si;
    delete m_ri;
	m_connection->Release();
	delete[] m_sprops;
    delete[] m_wkbBuffer;
    delete[] m_aPropNames;
}

void SltReader::SetInternalFilter(FdoFilter* filter)
{
    FDO_SAFE_RELEASE(m_filter);
    m_filter = FDO_SAFE_ADDREF(filter);
}

void SltReader::DelayedInit(FdoIdentifierCollection* props, const char* fcname, const char* strWhere, bool addPkOnly)
{
    int rc = 0;

    SltMetadata* md = m_connection->GetMetadata(fcname);

    if (!md)
        throw FdoCommandException::Create(L"Requested feature class does not exist in the database.");

	//first, issue the full statement, and create the FDO feature class from it -- we will present that to the caller
	//so that he sees the properties he asked for. 
	if (props && props->GetCount())
	{
        FdoPtr<FdoClassDefinition> clsDef = md->ToClass();
        SltExpressionTranslator exTrans(props, clsDef);
		int nProps = props->GetCount();
        m_reissueProps.Reserve(nProps);
		for (int i=0; i<nProps; i++)
		{
			FdoPtr<FdoIdentifier> id = props->GetItem(i);
            exTrans.Reset();
            id->Process(&exTrans);
            StringBuffer* exp = exTrans.GetExpression();
            m_reissueProps.Add(exp->Data(), exp->Length());
        }
        m_nTotalProps = nProps;
	}
    else
    {
        m_reissueProps.Reserve(4);
    }
    
    const wchar_t* idClassProp = L"rowid";

    m_fromwhere.Append(" FROM ", 6);
    if (!md->IsView())
        m_fromwhere.AppendDQuoted(fcname);
    else
    {
        if (md->GetIdName() == NULL)
        {
            m_connection->CacheViewContent(fcname);
            m_fromwhere.Append("\"$view");
            m_fromwhere.Append(fcname);
            m_fromwhere.Append("\"");
        }
        else
        {
            m_isViewSelect = true;
            m_useFastStepping = false;
            idClassProp = md->GetIdName();
            m_fromwhere.AppendDQuoted(fcname);
        }
    }

    //construct the where clause and 
    //if necessary add FeatId filter -- in case we know which features we want
    //like when we have a spatial iterator
    if (*strWhere==0)
    {
        if (m_si || m_ri)
        {
            m_fromwhere.Append(" WHERE ", 7);
            m_fromwhere.AppendDQuoted(idClassProp);
            m_fromwhere.Append("=?;", 3);
        }
        else
            m_fromwhere.Append(";", 1);
    }
    else
    {
        m_fromwhere.Append(" WHERE ", 7);

        if (m_si || m_ri)
        {
            m_fromwhere.AppendDQuoted(idClassProp);
            m_fromwhere.Append("=? AND ", 7);
        }

        m_fromwhere.Append("(", 1);
        m_fromwhere.Append(strWhere);
        m_fromwhere.Append(");", 2);
    }

    //remember the geometry encoding format
    m_eGeomFormat = md->GetGeomFormat();

    //if there were properties passed in the identifier collection, assume 
	//the caller knows what they want and we will use the exact query
    //rather than paring down the property list initially
	if (props && props->GetCount() > 0)
	{
        m_mainClassName = fcname;
        Requery2();
        InitPropIndex(m_pStmt);
        return;
	}
    
    //Caller passed empty property list -- assume the
    //resulting feature class is identical to the one in the schema.
    m_class = md->ToClass();
    
	//we have created the class definition -- now we can add the
    //columns from the feature class in the same order as they appear in it
    //We will speculatively add only the columns up to the geometry,
    //since a lot of the time callers who do "select * ..." actually only
    //use the geometry and the id, which most of the time are first and second in the table.
    //We will add all the other properties after that if the caller asks for any property
    //other than the geometry and id.
    FdoPtr<FdoPropertyDefinitionCollection> pdc = m_class->GetProperties();
    int maxIndex = md->GetGeomIndex();
    if (maxIndex < md->GetIDIndex())
        maxIndex = md->GetIDIndex();

    m_nTotalProps = pdc->GetCount();
    
    FdoPtr<FdoDataPropertyDefinitionCollection> pdic = m_class->GetIdentityProperties();
    bool addRowId = true;
    if (pdic->GetCount() == 1)
    {
        FdoPtr<FdoDataPropertyDefinition> pdi = pdic->GetItem(0);
        FdoDataType dtdpi = pdi->GetDataType();
        if (dtdpi == FdoDataType_Byte || dtdpi == FdoDataType_Int16 || 
            dtdpi == FdoDataType_Int32 || dtdpi == FdoDataType_Int64)
            addRowId = false;
    }

    // in case we have non-integer PK add RowId
    if (addRowId)
    {
        m_reissueProps.Add("\"rowid\"", 7);
        maxIndex = -1; // composite, non-int or no PK force add all
    }

    StringBuffer propName(30);
    if (addPkOnly && !m_isViewSelect)
    {
        for (int i = 0; i < pdic->GetCount(); i++)
        {
            FdoPtr<FdoDataPropertyDefinition> pdi = pdic->GetItem(i);
            propName.Reset();
            propName.AppendDQuoted(pdi->GetName());
            m_reissueProps.Add(propName.Data(), propName.Length());
        }
    }
    else
    {
        //If the query is for a single feature, we will directly add all the properties
        //since in this case it is more likely for the caller to need them
        if (maxIndex == -1 || (m_ri && m_ri->Count() == 1) || m_isViewSelect)
            maxIndex = pdc->GetCount() - 1;

        for (int i=0; i<=maxIndex; i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            propName.Reset();
            propName.AppendDQuoted(pd->GetName());
            m_reissueProps.Add(propName.Data(), propName.Length());
        }
    }

    //redo the query with the id and geom props only
    Requery2();    
    InitPropIndex(m_pStmt);
}

void SltReader::InitPropIndex(sqlite3_stmt* pStmt)
{
	m_propNames.clear();
	m_mNameToIndex.Clear();

    if( m_parmValues != NULL )
        BindPropVals(m_parmValues, m_pStmt, true, m_eGeomFormat);

	int nProps = sqlite3_column_count(pStmt);

	if (nProps > m_nMaxProps)
    {
        delete [] m_sprops;
        m_nMaxProps = nProps;
		m_sprops = new StringRec[m_nMaxProps];
    }
    
    //cache information about the returned columns
    //We will store all the property names converted to wide char
    //into a single string buffer (to save on allocations) and
    //use pointers into that buffer 
    delete[] m_aPropNames;
    size_t buflen = 0;

	//find out how big a buffer we need
	for (int i=0; i<nProps; i++)
	{
		const char* cname = sqlite3_column_name(pStmt, i);
        buflen += strlen(cname) + 1;
	}

    m_aPropNames = new wchar_t[buflen];
    wchar_t* dst = m_aPropNames;
    m_propNames.reserve(nProps);

	//convert column names to wchar and store in our buffer
	for (int i=0; i<nProps; i++)
	{
		const char* cname = sqlite3_column_name(pStmt, i);
        int stProp = 0, lenProp = 0;
        ExtractDbName(cname, stProp, lenProp);
        
        //Note buflen is longer than the column name, but the code we call will terminate when it sees the NULL terminator.
        //We just need to pass in a number that is bigger than the length of the string, and buflen is guaranteed to be.
        int len = 1 + A2W_FAST(dst, buflen, cname+stProp, lenProp); 

        m_propNames.push_back(dst);
        m_mNameToIndex.Add(dst, i);

        dst += len;
        buflen -= len;
	}

    m_mNameToIndex.Prepare();
}

int SltReader::AddColumnToQuery(const wchar_t* name)
{
    //first remember the current feature
    //so that we know to step the new query up to it
    //we know the id is the first property in the query result, since
    //we placed it there
    int cur_id = sqlite3_column_int(m_pStmt, 0);

    if (!m_class)
        throw FdoCommandException::Create((std::wstring(L"The property \'") + name + L"\' was not found.").c_str());
        //throw FdoException::Create(L"Attempted to access a property which was not listed in the Select command. API misuse by the caller!");

    //make sure the property exists in the feature class
    FdoPtr<FdoPropertyDefinitionCollection> pdc = m_class->GetProperties();
    int index = pdc->IndexOf(name);

    //Add all the properties to the query.
    //If we reach here, it means we speculatively added only the id and the geometry
    //to the query for a "select * " query, but the caller needs more than just those
    if (index != -1)
    {
        StringBuffer propName(30);
        for (int i=m_reissueProps.Count(), iEnd = pdc->GetCount(); i<iEnd; i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            propName.Reset();
            propName.AppendDQuoted(pd->GetName());
            m_reissueProps.Add(propName.Data(), propName.Length());
        }

        Requery2();
        InitPropIndex(m_pStmt);

        //step till the feature we were at with the previous query
        while (cur_id != sqlite3_column_int(m_pStmt, 0))
            ReadNext();

        //return the index of the new property
        return index; // this is not accurate since other properties can be already selected...
    }
    else
        throw FdoCommandException::Create((std::wstring(L"The property \'") + name + L"\' was not found.").c_str());

    return -1;
}

void SltReader::Requery2()
{
    if (m_pStmt) 
        m_connection->ReleaseParsedStatement(m_sql.Data(), m_pStmt);

    m_sql.Reset();
    m_sql.Append("SELECT ", 7);

    if (m_reissueProps.Count() == 0)
    {
        m_sql.Append("*", 1);
    }
    else
    {
        for (int i=0; i<m_reissueProps.Count(); i++)
        {
		    if (i) m_sql.Append(",", 1);
		    m_sql.Append(m_reissueProps.Get(i));
        }
    }

    m_sql.Append(m_fromwhere.Data(), m_fromwhere.Length());

    m_curfid = 0; //position prior to first record 
    m_closeOpcode = -1;

    //reset the spatial iterator, if any
    if (m_si)
    {
        m_siEnd = -1;
        m_si->Reset();
    }

    //reset the rowid iterator, if any
    //TODO: reading forward can be really slow in the case when someone 
    //is doing reverse scrolling through the result set. In such a case 
    //it is better to reset the rowid iterator to its last item and iterate 
    //backwards. However, it is not easy to detect when someone is reading 
    //backwards or indeed just reading randomly.
    if (m_ri)
    {
        m_ri->MoveToIndex(0);
    }

    m_pStmt = m_connection->GetCachedParsedStatement(m_sql.Data());

    //If the reader was constructed for a Select that is known
    //to be safe for fast SQLite reading (no null termination
    //and memcopy for column values is needed), set a flag inidicating
    //that on the SQLite query execution engine
    if (m_useFastStepping && !m_isViewSelect)
        ((Vdbe*)m_pStmt)->fdo = 1;
}

	//-------------------------------------------------------
	// FdoIReader implementation
	//-------------------------------------------------------
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
bool SltReader::GetBoolean(int index)
{
    ValidateIndex(m_pStmt, index);
	int res = sqlite3_column_int(m_pStmt, index);
	return res != 0;
}
bool SltReader::GetBoolean(FdoString* propertyName)
{
    return SltReader::GetBoolean(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoByte SltReader::GetByte(int index)
{
    ValidateIndex(m_pStmt, index);
	int res = sqlite3_column_int(m_pStmt, index);
	return (FdoByte)res;
}
FdoByte SltReader::GetByte(FdoString* propertyName)
{
    return SltReader::GetByte(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoDateTime SltReader::GetDateTime(int index)
{
    ValidateIndex(m_pStmt, index);
    const FdoString* sdt = SltReader::GetString(index);
	return DateFromString(sdt);
}
FdoDateTime SltReader::GetDateTime(FdoString* propertyName)
{
    return SltReader::GetDateTime(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
double SltReader::GetDouble(int index)
{
    ValidateIndex(m_pStmt, index);
	double res = sqlite3_column_double(m_pStmt, index);
	return res;
}
double SltReader::GetDouble(FdoString* propertyName)
{
    return SltReader::GetDouble(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoInt16 SltReader::GetInt16(int index)
{
    ValidateIndex(m_pStmt, index);
	int res = sqlite3_column_int(m_pStmt, index);
	return (FdoInt16)res;
}
FdoInt16 SltReader::GetInt16(FdoString* propertyName)
{
    return SltReader::GetInt16(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoInt32 SltReader::GetInt32(int index)
{
    ValidateIndex(m_pStmt, index);
	int res = sqlite3_column_int(m_pStmt, index);
	return (FdoInt32)res;
}
FdoInt32 SltReader::GetInt32(FdoString* propertyName)
{
	return GetInt32(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoInt64 SltReader::GetInt64(int index)
{
    ValidateIndex(m_pStmt, index);
    FdoInt64 res = sqlite3_column_int64(m_pStmt, index);
	return res;
}
FdoInt64 SltReader::GetInt64(FdoString* propertyName)
{
    return SltReader::GetInt64(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
float SltReader::GetSingle(int index)
{
    ValidateIndex(m_pStmt, index);
	double res = sqlite3_column_double(m_pStmt, index);
	return (float)res;
}
float SltReader::GetSingle(FdoString* propertyName)
{
    return SltReader::GetSingle(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoString* SltReader::GetString(int index)
{
    ValidateIndex(m_pStmt, index);
    int i = index;

	if (m_sprops[i].valid)
		return m_sprops[i].data;

    //If the fdo flag is set on the VDBE, it means
    //the SQLite is not allocating memory to copy row data into.
    //Therefore we cannot ask it for strings using the regular API,
    //since it will attempt to null terminate them, with disastrous
    //results. We will fetch the raw string directly from row cache memory.
    if (((Vdbe*)m_pStmt)->fdo)
    {
	    Mem* textmem = columnMem(m_pStmt, i);

		if (textmem->type == SQLITE_INTEGER)
		{
			m_sprops[i].EnsureSize(32);
#ifdef _WIN32
			_i64tow_s(textmem->u.i, m_sprops[i].data, 32, 10);
#else
		    swprintf(m_sprops[i].data, 256, L"%lld", (long long int)textmem->u.i);
#endif
			m_sprops[i].valid = 1;
		}
		else if(textmem->type == SQLITE_FLOAT)
		{
			m_sprops[i].EnsureSize(256);
			swprintf(m_sprops[i].data, 256, L"%.16g", textmem->r);
			m_sprops[i].valid = 1;
		}
		else if(textmem->type == SQLITE_NULL)
		{
			return L"";
		}
		else
		{
			if (!textmem->n) 
				return L""; //empty string L"" better than NULL, since callers may not be used to NULL :)

			int len = (int)textmem->n;
			m_sprops[i].EnsureSize(len+1);
			A2W_FAST(m_sprops[i].data, len + 1, (const char*)textmem->z, len);
			m_sprops[i].valid = 1;
		}
    }
    else
    {
        const char* text = (const char*)sqlite3_column_text(m_pStmt, i);

        if (!text)
            return NULL;

	    int len = (int)strlen(text);
	    m_sprops[i].EnsureSize(len+1);
	    A2W_FAST(m_sprops[i].data, len+1, text, len);
	    m_sprops[i].valid = 1;
    }

	return m_sprops[i].data;

}
FdoString* SltReader::GetString(FdoString* propertyName)
{
    return SltReader::GetString(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoLOBValue* SltReader::GetLOB(int index)
{
    ValidateIndex(m_pStmt, index);
    FdoByte* vblob = NULL;
    int len = 0;
    if (((Vdbe*)m_pStmt)->fdo)
    {
        Mem* blob = columnMem(m_pStmt, index);
        len = blob->n;
        vblob = (unsigned char*)blob->z;
    }
    else
    {
	    const void* ptr = sqlite3_column_blob(m_pStmt, index);
	    len = sqlite3_column_bytes(m_pStmt, index);
    	vblob = (unsigned char*)ptr;    
    }
    return (vblob) ? (FdoLOBValue*)FdoDataValue::Create(vblob, len, FdoDataType_BLOB) : NULL;
}
FdoLOBValue* SltReader::GetLOB(FdoString* propertyName)
{
    return SltReader::GetLOB(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoIStreamReader* SltReader::GetLOBStreamReader(int index)
{
    FdoPtr<FdoLOBValue> blob = GetLOB(index);
    return new SltBLOBStreamReader(blob); 
}
FdoIStreamReader* SltReader::GetLOBStreamReader(FdoString* propertyName )
{
	return SltReader::GetLOBStreamReader(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
bool SltReader::IsNull(int index)
{
    ValidateIndex(m_pStmt, index);
	return sqlite3_column_type(m_pStmt, index) == SQLITE_NULL;
}
bool SltReader::IsNull(FdoString* propertyName)
{
    return SltReader::IsNull(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoIRaster* SltReader::GetRaster(int index)
{
	throw FdoException::Create(L"GetRaster() is not implemented by this provider.");
}
FdoIRaster* SltReader::GetRaster(FdoString* propertyName)
{
    return SltReader::GetRaster(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoIFeatureReader* SltReader::GetFeatureObject(int index)
{
	throw FdoException::Create(L"GetFeatureObject() is not implemented by this provider.");
}
FdoIFeatureReader* SltReader::GetFeatureObject(FdoString* propertyName)
{
    return SltReader::GetFeatureObject(NameToIndex(propertyName));
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

FdoByteArray* SltReader::GetGeometry(int index)
{
	int len = 0;
    const void* ptr = SltReader::GetGeometry(index, &len);
    return (len) ? FdoByteArray::Create((unsigned char*)ptr, len) : NULL;
}
FdoByteArray* SltReader::GetGeometry(FdoString* propertyName)
{
    return SltReader::GetGeometry(NameToIndex(propertyName));
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

const FdoByte* SltReader::GetGeometry(FdoString* propertyName, FdoInt32* len)
{
    return SltReader::GetGeometry(NameToIndex(propertyName), len);
}

const FdoByte* SltReader::GetGeometry(int i, FdoInt32* len)
{
    ValidateIndex(m_pStmt, i);

    //If the FDO flag is set, we can be pretty sure the underlying
    //column is a geometry blob and we can fetch it directly from
    //sqlite row cache memory
    unsigned char* geom;

    if (((Vdbe*)m_pStmt)->fdo)
    {
        Mem* blob = columnMem(m_pStmt, i);
        *len = blob->n;
        geom = (unsigned char*)blob->z;
    }
    else
    {
	    const void* ptr = sqlite3_column_blob(m_pStmt, i);
	    *len = sqlite3_column_bytes(m_pStmt, i);
    	geom = (unsigned char*)ptr;    
    }

    //is geometry null?
    if (!*len)
        return NULL;

    if (m_eGeomFormat == eFGF)
        return geom;
    else if (m_eGeomFormat == eWKB)
    {
        int estFGFLen = (*len) * 2; //should be sufficient, heh
        if (m_wkbBufferLen < estFGFLen)
        {
            delete[] m_wkbBuffer;
            m_wkbBufferLen = estFGFLen;
            m_wkbBuffer = new unsigned char[m_wkbBufferLen];
        }

        *len = Wkb2Fgf(geom, m_wkbBuffer);
        return m_wkbBuffer;
    }
    else if (m_eGeomFormat == eWKT)
    {
        wchar_t* tmp = (wchar_t*)alloca(sizeof(wchar_t)*(*len+1));
        A2W_FAST(tmp, *len+1, (const char*)geom, *len);

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        FdoPtr<FdoIGeometry> geom = gf->CreateGeometry((FdoString*)tmp);
        FdoPtr<FdoByteArray> fgf = gf->GetFgf(geom);
        
        *len = fgf->GetCount();
        if (m_wkbBufferLen < *len)
        {
            delete[] m_wkbBuffer;
            m_wkbBufferLen = *len;
            m_wkbBuffer = new unsigned char[m_wkbBufferLen];
        }

        memcpy(m_wkbBuffer, fgf->GetData(), *len);
        return m_wkbBuffer;
    }
    else
    {
        throw FdoException::Create(L"Unsupported geometry format.");
    }

    //return NULL;//can't get here from there.
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

#pragma warning (disable: 4127)

bool SltReader::ReadNextOnView()
{
    // we will use m_closeOpcode for read more than once from a view where ID=val
    bool retVal = false;
    do
    {
        if (m_curfid == 0 || m_closeOpcode == -1)
        {
            while (1)
            {
                //are we at the end of the current spatial iterator batch?
                if (m_si)
                {
                    m_curfid++;
                    if (m_curfid >= m_siEnd)
                    {
                        int start;
                        bool ret = m_si->NextRange(start, m_siEnd);

                        //spatial reader is done, so we are done
                        if (!ret)
                            return false;

                        m_curfid = (sqlite3_int64)(start ? start : 1); //make sure we skip fid=0, which is not valid
                    }
                }
                else if (m_ri) //or are we using a rowid iterator?
                {
                    bool res = m_ri->Next();

                    if (res)
                        m_curfid = m_ri->CurrentRowid();
                    else
                        return false; //scrolled past the end of the data
                }
                sqlite3_reset(m_pStmt);
                sqlite3_bind_int64(m_pStmt, 1, ((m_si == NULL) ? m_curfid : (*m_si)[(int)m_curfid]));
                if (sqlite3_step(m_pStmt) == SQLITE_ROW)
                {
                    m_closeOpcode = 0;
                    return true;
                }
            }
        }
        retVal = (sqlite3_step(m_pStmt) == SQLITE_ROW);
        if (!retVal)
            m_closeOpcode = -1;
    }while(!retVal);

    return true;
}

bool SltReader::ReadNext()
{
    //clear the wide string row cache
 	for (size_t i=0; i<m_propNames.size(); i++)
		m_sprops[i].valid = 0;

    //yes, we know what we are doing (hopefully)...
    Vdbe* v = (Vdbe*)m_pStmt;

    //use spatial iterator or rowid iterator if any
    //Note that we will not get both a spatial and rowid iterator.
    //The SltConntection::Select() function will pre-process data so that
    //we get either a rowid iterator or a spatial iterator, but not both.
    if (m_si || m_ri)
    {
        if (m_isViewSelect)
            return ReadNextOnView();
        while (1)
        {
            //are we at the end of the current spatial iterator batch?
            if (m_si)
            {
                m_curfid++;
                if (m_curfid >= m_siEnd)
                {
                    int start;
                    bool ret = m_si->NextRange(start, m_siEnd);

                    //spatial reader is done, so we are done
                    if (!ret)
                        return false;

                    m_curfid = (sqlite3_int64)(start ? start : 1); //make sure we skip fid=0, which is not valid
                }
            }
            else if (m_ri) //or are we using a rowid iterator?
            {
                bool res = m_ri->Next();

                if (res)
                    m_curfid = m_ri->CurrentRowid();
                else
                    return false; //scrolled past the end of the data
            }
            
            //we have had at least once successful hit
            if (m_closeOpcode != -1)
            {
                //Set the next row ID we need into the live compiled bytecode.
                //Note that this is not the same as sqlite_bind_int64, because
                //the execution engine copies the variables from the statement into
                //internal memory, which we are setting directly here.
                v->aMem[1].u.i = ((m_si == NULL) ? m_curfid : (*m_si)[(int)m_curfid]);


                //now set the VDBE program counter to the instruction that
                //fetches the row we set above -- this is to skip initialization
                //instructions which are called when we step the statement after
                //a reset -- we are skipping the reset for speed, so we have 
                //to skip the initialization as well (it would lock the table again without
                //it being freed, since we are not going to finish the previous step
                //which would have freed the previous lock.
                //Here is sample bytecode generated for the type of select statement we use
                //for spatial queries:
                /*
                sqlite> explain select rowid,geometry from Legal_Parcel where rowid=?;
                addr  opcode         p1    p2    p3    p4             p5  comment
                ----  -------------  ----  ----  ----  -------------  --  -------------
                0     Trace          0     0     0                    00
                1     Variable       1     1     1                    00
                2     Goto           0     11    0                    00
                3     OpenRead       0     61    0     2              00
                4     MustBeInt      1     9     0                    00
                5     NotExists      0     9     1                    00
                6     Rowid          0     3     0                    00
                7     Column         0     1     4                    00
                8     ResultRow      3     2     0                    00
                9     Close          0     0     0                    00
                10    Halt           0     0     0                    00
                11    Transaction    0     0     0                    00
                12    VerifyCookie   0     9     0                    00
                13    TableLock      0     61    0     Legal_Parcel   00
                14    Goto           0     3     0                    00
                */
                //Note that we compile without Trace, so the instruction addresses in our case
                //are less by 1 each. We want to set the bytecode execution to repeat itself for 
                //each next ID starting at operation NotExists, up to operation ResultRow.
                //Hence we need to set the instruction pointer back to 5, or in our case 4, since
                //we omit the initial trace instruction.
                v->pc = 4;
            }
            else
            {
                //situation where we have to reset the statement (SLOWwwwwWWWWwwWWWWw!)
                //Should only happen the first time we execute ReadNext.
                sqlite3_reset(m_pStmt);
                sqlite3_bind_int64(m_pStmt, 1, ((m_si == NULL) ? m_curfid : (*m_si)[(int)m_curfid]));
            }

            if (sqlite3_step(m_pStmt) == SQLITE_ROW)
            {
                //the opcode that will be executed after a step
                //closes off the table locks, etc. We want to skip
                //that for speed -- here we remember the program counter
                //necessary for closing off the query, and we will
                //execute this only once at the end
                m_closeOpcode = v->pc;
		        return true;
            }
            else
            {
                //TODO: here if the return value is not SQLITE_ROW
                //the statement will close the cursors -- we need to reset it
                //This is bad because resetting the statement is slow!
                //This can happen if a feature matches the bbox query but fails
                //the rest of the WHERE clause.
                //setting close opcode to -1 will reset the statement the 
                //next time we call ReadNext().
                m_closeOpcode = -1;
            }
        }
    }
    else
    {
        //case where we are not using a spatial index
        //here we can simply step the statement and let
        //SQLite handle the work
        return (sqlite3_step(m_pStmt) == SQLITE_ROW);
    }

    return false;
}

void SltReader::Close()
{
	if (!m_pStmt)
		return;

    //Execute the instructions that end the VDBE execution loop.
    //This puts VDBE back into a sane state after us having messed with
    //its bytecode in ReadNext().
    //NOTE: If Close() does not get called and the reader is left
    //dangling in this state, it spells ***CERTAIN DOOM*** for anybody trying to 
    //use the database connection thereafter! Consider yourself warned.
    if (m_closeOpcode != -1 && !m_isViewSelect)
    {
        Vdbe* v = (Vdbe*)m_pStmt;
        v->pc = m_closeOpcode;
        int rc = sqlite3_step(m_pStmt);

        //must set this back so that a second call to Close() does not
        //reattempt to end the VDBE execution
        m_closeOpcode = -1;
    }

    sqlite3* db = sqlite3_db_handle(m_pStmt); 

    //Finalize the statemenet, in case of ephemeral db, or release it to cache otherwise
    //We must do this before committing the transaction, in case we are being Closed()
    //before we were done calling ReadNext()
    if (m_closeDB != ReaderCloseType_None)
    {
        sqlite3_finalize(m_pStmt);
    }
    else //otherwise just release the cached statement we were using
        m_connection->ReleaseParsedStatement(m_sql.Data(), m_pStmt);

    //Close the database as well, if it was an ephemeral database
    //used to return computed data
    if (m_closeDB == ReaderCloseType_CloseDb)
        sqlite3_close(db);

	m_pStmt = NULL;
}


	//-------------------------------------------------------
	// FdoIFeatureReader implementation (except GetGeometry, that's done above)
	//-------------------------------------------------------

FdoClassDefinition* SltReader::GetClassDefinition()
{
	if (!m_class)
	{
        int additionalSize = 0;
        std::vector<int> idxProperties;
        std::vector<int> unknownPropsIdx;
		//decide on a name for the class -- just pick the table name
		//for the first column :)
		const char* tableName = sqlite3_column_table_name(m_pStmt, 0);
        //in case is a temp table filled with a view content use the view name
        if (NULL == tableName)
            tableName = "GeneratedClass";
        else
            tableName = DecodeTableName(tableName);

        std::wstring wtable = A2W_SLOW(tableName);
		//find source feature class metadata
        SltMetadata* mainMd = m_connection->GetMetadata(tableName);

		m_class = FdoFeatureClass::Create(wtable.c_str(), NULL);
		FdoPtr<FdoPropertyDefinitionCollection> dstpdc = m_class->GetProperties();
		FdoPtr<FdoDataPropertyDefinitionCollection> dstidpdc = m_class->GetIdentityProperties();

		int nProps = sqlite3_column_count(m_pStmt);

		//cache information about the returned columns
		for (int i=0; i<nProps; i++)
		{
            idxProperties.push_back(-1);
			bool propFound = false;
			//get name of table that is the source for this column
			const char* table = sqlite3_column_table_name(m_pStmt, i);
            table = DecodeTableName(table);

			//find source feature class metadata
            SltMetadata* md = (table == NULL)? NULL : m_connection->GetMetadata(table);

			if (md)
			{
				FdoPtr<FdoClassDefinition> origfc = md->ToClass();
				FdoPtr<FdoPropertyDefinitionCollection> pdc = origfc->GetProperties();
				FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = origfc->GetIdentityProperties();
				FdoPtr<FdoGeometricPropertyDefinition> geompd = (origfc->GetClassType() == FdoClassType_FeatureClass) 
					? ((FdoFeatureClass*)origfc.p)->GetGeometryProperty() : NULL;

				const wchar_t* pname = m_propNames[i];
                const char* origin_name = NULL;

				FdoPtr<FdoPropertyDefinition> srcprop = pdc->FindItem(pname);
                if (!srcprop.p)
                {
                    origin_name = sqlite3_column_origin_name(m_pStmt, i);
                    if (origin_name)
                    {
                        std::wstring worigin_name = A2W_SLOW(origin_name);
                        srcprop = pdc->FindItem(worigin_name.c_str());
                    }
                }

                if (srcprop.p)
                {
                    FdoPtr<FdoPropertyDefinition> clonedprop = FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition(srcprop);
                    if (origin_name)
                        clonedprop->SetName(pname);

                    propFound = true;
                    if (dstpdc->Contains(pname))
                    {
                        additionalSize += GenerateUniqueName(pname, clonedprop, dstpdc);
                        idxProperties[i] = dstpdc->GetCount();
                    }
                    dstpdc->Add(clonedprop);

                    if (idpdc->Contains(pname))
					    dstidpdc->Add((FdoDataPropertyDefinition*)clonedprop.p);

				    if (NULL != geompd && wcscmp(pname, geompd->GetName()) == 0)
					    ((FdoFeatureClass*)m_class)->SetGeometryProperty((FdoGeometricPropertyDefinition*)clonedprop.p);
                }
			}
            // in case property was not found let's look at the sqlite column
			if (!propFound)
			{
				//case where the reader is a result of arbitrary sql and the
				//resulting columns do not come from any existing table. This a case of an ad-hoc query 
                //with a select list that contains an expression withiout an alias: select (max(f.id))+1 from...
				FdoPtr<FdoDataPropertyDefinition> dpd;
                
                // Replace FDO special characters with an underscore otherwise the FdoDataPropertyDefinition::Create will
                // fail for expression with "." similar to the above example.
                FdoStringP propName = m_propNames[i];
                propName = propName.Replace(L":",L"_");
                propName = propName.Replace(L".",L"_");
				//NOTE: Unfortunately, the result of calling this function
				//may vary when called on different rows of the result.

                // A ReadNext is required before the type of an expression can be known. Otherwise, the result
                // will default to a string.
				int type = sqlite3_column_type(m_pStmt, i);
				switch (type)
				{
				case SQLITE_INTEGER: 
                    dpd = FdoDataPropertyDefinition::Create(propName, NULL);
    				dpd->SetDataType(FdoDataType_Int64);
					break;
				case SQLITE_TEXT:
                    dpd = FdoDataPropertyDefinition::Create(propName, NULL);
    				dpd->SetDataType(FdoDataType_String);
					break;
				case SQLITE_FLOAT:
                    dpd = FdoDataPropertyDefinition::Create(propName, NULL);
    				dpd->SetDataType(FdoDataType_Double);
					break;
				case SQLITE_BLOB:
                    dpd = FdoDataPropertyDefinition::Create(propName, NULL);
    				dpd->SetDataType(FdoDataType_BLOB);
					break;
				case SQLITE_NULL:
                    // Create a data property with a default type and
                    // we will process them later at the end...
                    dpd = FdoDataPropertyDefinition::Create(propName, NULL);
    				dpd->SetDataType(FdoDataType_String);
                    unknownPropsIdx.push_back(i);
					break;
				}
                if (dpd != NULL)
                {
                    const wchar_t* pname = dpd->GetName();
                    if (dstpdc->Contains(pname))
                    {
                        additionalSize += GenerateUniqueName(pname, dpd, dstpdc);
                        idxProperties[i] = dstpdc->GetCount();
                    }
				    dstpdc->Add(dpd);
                }
			}
		}
        // do we have unknown calculations !?
        if (unknownPropsIdx.size() != 0 && m_reissueProps.Count() != 0)
        {
            if (NULL == mainMd && m_mainClassName.size() != 0)
                mainMd = m_connection->GetMetadata(m_mainClassName.c_str());

            FdoPtr<FdoClassDefinition> origfc = (NULL == mainMd) ? NULL : mainMd->ToClass();
            // use the original class in case we have one otherwise use the generated class
            FdoClassDefinition* origClass = (origfc == NULL) ? m_class : origfc.p;
            // the expression may be based on provider functions
            FdoPtr<FdoIExpressionCapabilities> expCap = m_connection->GetExpressionCapabilities();
            FdoPtr<FdoFunctionDefinitionCollection> functionDefinitions = expCap->GetFunctions();
            for(size_t idx = 0; idx < unknownPropsIdx.size(); idx++)
            {
                FdoPtr<FdoPropertyDefinition> pdToAdd;
                try
                {
                    FdoPropertyType retPropType;
                    FdoDataType retDataType;
                    const char* propDef = m_reissueProps.Get(unknownPropsIdx.at(idx));
                    std::wstring expUni = A2W_SLOW(propDef);
                    std::wstring expression = ExtractExpression(expUni.c_str(), m_propNames[unknownPropsIdx.at(idx)]);

                    // Replace the internal functions. Since they are not standard getting the definitions will fail below.
                    FdoStringP  origExpr = FdoStringP(expression.c_str());
                    origExpr = origExpr.Replace(L"booleantostring", L"");
                    origExpr = origExpr.Replace(L"floattostring", L"");
                    origExpr = origExpr.Replace(L"doubletostring", L"");
                    origExpr = origExpr.Replace(L"datetostring", L"");

                    FdoPtr<FdoExpression> expr = FdoExpression::Parse(origExpr);
                    FdoCommonMiscUtil::GetExpressionType(functionDefinitions, origClass, expr, retPropType, retDataType);
                    switch(retPropType)
                    {
                        case FdoPropertyType_DataProperty:
                        {
                            FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(m_propNames[unknownPropsIdx.at(idx)], NULL);
				            dpd->SetDataType(retDataType);
                            pdToAdd = dpd;
                        }
                            break;
                        case FdoPropertyType_GeometricProperty:
                            pdToAdd = FdoGeometricPropertyDefinition::Create(m_propNames[unknownPropsIdx.at(idx)], NULL);
                            break;
                        default:
                        {
                            // we cannot do much about this case
                            FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(m_propNames[unknownPropsIdx.at(idx)], NULL);
				            dpd->SetDataType(FdoDataType_String);
                            pdToAdd = dpd;
                        }
                            break;
                    }
                }
                catch(FdoException* exc)
                {
                    // we failed to detect the type use the default type
                    exc->Release();
                }
                if (pdToAdd != NULL)
                {
                    FdoPtr<FdoPropertyDefinition> dpd = dstpdc->FindItem(pdToAdd->GetName());
                    if( dpd != NULL )
                    {
                        int index = dstpdc->IndexOf(dpd);
                        dstpdc->RemoveAt( index );
                        dstpdc->Insert(index,pdToAdd);
                    }
                    else
				        dstpdc->Add(pdToAdd);
                }
            }
        }
        if (additionalSize != 0)
        {
            m_mNameToIndex.Clear();
            int buflen = (int)(m_propNames.back() - m_propNames[0]) + wcslen(m_propNames.back()) + 1 + additionalSize;
            wchar_t* aPropNames = new wchar_t[buflen];
            wchar_t* dst = aPropNames;

	        //convert column names to wchar and store in our buffer
	        for (int i=0; i<nProps; i++)
	        {
                int len = 0;
                if (idxProperties[i] == -1)
                {
                    // property name did not changed
                    wcscpy(dst, m_propNames[i]);
                    len = wcslen(dst) + 1;
                }
                else
                {
                    FdoPtr<FdoPropertyDefinition> propDefChg = dstpdc->GetItem(idxProperties[i]);
                    FdoString* propNameChg = propDefChg->GetName();
                    wcscpy(dst, propNameChg);
                    len = wcslen(dst) + 1;
                }
                m_propNames.push_back(dst);
                dst += len;
	        }
            delete[] m_aPropNames;
            m_aPropNames = aPropNames;
            m_propNames.erase(m_propNames.begin(), m_propNames.begin()+nProps);
            for (int i=0; i<nProps; i++)
            {
                m_mNameToIndex.Add(m_propNames[i], i);
            }

            m_mNameToIndex.Prepare();
        }
	}

	return FDO_SAFE_ADDREF(m_class);
}

// returns the additional size needs to be allocated
int SltReader::GenerateUniqueName(const wchar_t* pname, FdoPropertyDefinition* prop, FdoPropertyDefinitionCollection* pcol)
{
    int idxProp = 1;
    int idx = 0;
    wchar_t buffer[5];
    
    int propsize = wcslen(pname);
    wchar_t* pnewName = new wchar_t[propsize + 1 + 4]; // from 1 to max 999
    memcpy(pnewName, pname, propsize * sizeof(wchar_t));
    *(pnewName+propsize) = L'$';
    do
    {
        swprintf(buffer, 5, L"%d", idxProp);
        idx = 0;
        while(buffer[idx] != L'\0')
            *(pnewName+propsize+1+idx) = buffer[idx++];
        *(pnewName+propsize+1+idx) = L'\0';
        idxProp++;
    }
    while(pcol->Contains(pnewName));
    
    prop->SetName(pnewName);
    delete[] pnewName;

    return idx + 1; // '$' + len(no)
}

FdoInt32 SltReader::GetDepth()
{
	throw FdoException::Create(L"GetDepth() is not implemented by this provider.");
}


	//-------------------------------------------------------
	// FdoIDataReader implementation
	//-------------------------------------------------------
//simple utility function we use a few times below
FdoPropertyDefinition* SltReader::GetFdoProperty(int index)
{
    FdoPtr<FdoClassDefinition> fc = GetClassDefinition(); 
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
    return pdc->GetItem(index);
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoInt32 SltReader::GetPropertyCount()
{
    FdoPtr<FdoClassDefinition> fc = GetClassDefinition(); 
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
	return pdc->GetCount();
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoString* SltReader::GetPropertyName(FdoInt32 index)
{
    FdoPtr<FdoPropertyDefinition> pd = GetFdoProperty(index);
    return pd->GetName();
}
int SltReader::GetPropertyIndex(FdoString* propertyName)
{
	return NameToIndex(propertyName);
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoDataType SltReader::GetDataType(int index)
{
	FdoPtr<FdoPropertyDefinition> pd = GetFdoProperty(index);

	return ((FdoDataPropertyDefinition*)pd.p)->GetDataType();
}
FdoDataType SltReader::GetDataType(FdoString* propertyName)
{
    return SltReader::GetDataType(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoPropertyType SltReader::GetPropertyType(int index)
{
	FdoPtr<FdoPropertyDefinition> pd = GetFdoProperty(index);

    return pd->GetPropertyType();
}
FdoPropertyType SltReader::GetPropertyType(FdoString* propertyName)
{
    return SltReader::GetPropertyType(NameToIndex(propertyName));
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

//-------------------------------------------------------
// FdoISQLDataReader implementation
// This is implemented in terms of the FdoIDataReader functions
//-------------------------------------------------------

FdoInt32 SltReader::GetColumnCount()
{
	return GetPropertyCount();
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
FdoString* SltReader::GetColumnName(FdoInt32 index)
{
	return GetPropertyName(index);
}
 int SltReader::GetColumnIndex(FdoString* columnName)
{
	return GetPropertyIndex(columnName);
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

FdoDataType SltReader::GetColumnType(int index)
{
    return GetDataType(index);
}
FdoDataType SltReader::GetColumnType(FdoString* columnName)
{
	return GetDataType(columnName);
}
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

//-------------------------------------------------------
// FdoIScrollableFeatureReader implementation
//-------------------------------------------------------

//Helper that moves the scrollable reader to the next requested
//record ID. It returns true if the record exists and false if it
//has been deleted.
//This helper is used for positioning the reader for all of the
//"scrollable" functions and its general strategy is to position
//us at one item before the one we want and then call ReadNext(),
//which does all the hard SQLite work.
bool SltReader::PositionScrollable(__int64 index)
{
    if (m_ri->Count() < index)
        return false;
    //remember which rowid we are looking for,
    //we will later check if that's what we got when
    //we stepped sqlite forward.
    m_ri->MoveToIndex(index);
    __int64 tmp = m_ri->CurrentRowid(); 

    //move to one rowid before the one we need
    m_ri->MoveToIndex(index-1);

    //move forward to the record we want by simply calling ReadNext() 
    // -- it will sort out the mess.
    bool res = ReadNext();

    //If the record exists, m_curfid is set to it,
    //otherwise it will get incremented to the next available.
    //This way we can infer whether the record we wanted was empty.
    if (m_curfid == tmp)
        return true;

    m_curfid = 0;
    return false;
}


int SltReader::Count()
{
    //BUG //TODO: We are forced to return an int, since the FDO API is wrong here
    return (int)m_ri->Count();
}

bool SltReader::ReadFirst()
{
    return PositionScrollable(1);
}

bool SltReader::ReadLast()
{
    return PositionScrollable(m_ri->Count());
}

bool SltReader::ReadPrevious()
{
    if (m_ri->Previous())
    {
        return PositionScrollable(m_ri->CurrentIndex());
    }
    else
    {
        //scrolled past the beginning
        return false;
    }
}

bool SltReader::ReadAtIndex(unsigned int recordIndex)
{
    return PositionScrollable(recordIndex);
}

bool SltReader::ReadAt(FdoPropertyValueCollection* key)
{
    //Assumes a single integer ID comes inside the input prop val collection
    FdoPtr<FdoPropertyValue> pv = key->GetItem(0);
    FdoPtr<FdoValueExpression> expr = pv->GetValue();
    FdoLiteralValue* lv = (FdoLiteralValue*)expr.p;

    if (lv->GetLiteralValueType() == FdoLiteralValueType_Data)
    {
        FdoDataValue* dv = (FdoDataValue*)lv;
        sqlite_int64 want_fid = 0;

        if (dv->GetDataType() == FdoDataType_Int64)
            want_fid = ((FdoInt64Value*)dv)->GetInt64();
        else if (dv->GetDataType() == FdoDataType_Int32)
            want_fid = ((FdoInt32Value*)dv)->GetInt32();

        if (want_fid == 0)
            return false;

        //map the ID to an index in our ID iterator
        __int64 index = m_ri->FindIndex(want_fid);
        if (index == -1)
            return false;

        return PositionScrollable(index);
    }

    return false;
}

unsigned int SltReader::IndexOf(FdoPropertyValueCollection* key)
{
    //Assumes a single integer ID and simply returns the number back
    //as equal to the requested record index
    FdoPtr<FdoPropertyValue> pv = key->GetItem(0);
    FdoPtr<FdoValueExpression> expr = pv->GetValue();
    FdoLiteralValue* lv = (FdoLiteralValue*)expr.p;

    if (lv->GetLiteralValueType() == FdoLiteralValueType_Data)
    {
        FdoDataValue* dv = (FdoDataValue*)lv;

        __int64 want_fid = 0;

        if (dv->GetDataType() == FdoDataType_Int64)
            want_fid = ((FdoInt64Value*)dv)->GetInt64();
        else if (dv->GetDataType() == FdoDataType_Int32)
            want_fid = ((FdoInt32Value*)dv)->GetInt32();

        //map the ID to an index in our ID iterator
        __int64 index = m_ri->FindIndex(want_fid);

        if (index < 1)
            return 0;

        return (unsigned int)index;
    }
    
    //TODO: also return 0 if record is empty -- need to move the reader to 
    //this record to check, and then move it back ...
    return 0;
}


//-------------------------------------------------------
// Some utlity functions
//-------------------------------------------------------

void SltReader::ValidateIndex(sqlite3_stmt *pStmt, int index)
{    
    if (index < 0 || index >= m_nTotalProps)
    {
		wchar_t tmp[15];
		swprintf(tmp, 15, L"%d", index);
        throw FdoCommandException::Create((std::wstring(L"Property index \'") + tmp + L"\' is out of bounds.").c_str());
    }

    //Trying to access a property that is theoretically available
    //but was not queried in the initial query (which only assumes ID and geom)
    int count = sqlite3_column_count(pStmt);
    if (index >= count)
    {
        FdoPtr<FdoPropertyDefinitionCollection> pdc = m_class->GetProperties();
        for (int i=count; i<index; i++)
        {
            FdoPtr<FdoPropertyDefinition> pv = pdc->GetItem(i);
            AddColumnToQuery(pv->GetName());
        }
    }

}

const char* SltReader::DecodeTableName(const char* name)
{
    if (name != NULL)
    {
        int pos = StringContains(name, "$view");
        return (pos == 0) ? (name+5) : name;
    }
    return name;
}

// function supports following formats (note a+b is just a sample - it can be any expression)
//  a + b aS name
//  a + b name
//  a + b AS "na me"
//  a + b "na me"
std::wstring SltReader::ExtractExpression(const wchar_t* exp, const wchar_t* propName)
{
    std::wstring retVal;
    wchar_t asValue[3];
    size_t szExp = wcslen(exp);
    size_t szName = wcslen(propName);
    int posEnd = szExp - szName;
    if (*(exp + szExp - 1) == '\"' && *propName != '\"')
        posEnd -= 2; // eliminate "" in case we have them

    int posAs = posEnd;
    int posEndExp = posEnd;
    if (posEnd > 0)
    {
        for(int idx = posEnd-1; idx > 0; idx--)
        {
            if (*(exp+idx) != ' ')
            {
                posAs = 0;
                int idx2 = 0;
                for(idx2 = idx; idx2 > 0; idx2--)
                {
                    if (*(exp+idx2) == ' ' || *(exp+idx2) == '\"')
                    {
                        posAs = idx2 + 1;
                        break;
                    }
                }
                posEndExp = posEnd; // assume could not find 'AS' take it as an exprerssion
                if ((posEnd - posAs) == 2)
                {
                    memcpy(asValue, posAs + exp, (posEnd - posAs)*sizeof(wchar_t));
                    asValue[2] = '\0';
                    if(_wcsicmp(asValue, L"AS") == 0)
                        posEndExp = posAs; // we found 'AS'
                }
                break;
            }
            else
                posEnd--;
        }
    }
    if (posEndExp > 0)
        retVal = std::wstring(exp, posEndExp);
    else
        retVal = exp;
    return retVal;
}


//-------------------------------------------------------
// DelayedReader implementation
//-------------------------------------------------------


DelayedInitReader::DelayedInitReader(   SltConnection*              connection, 
                                        FdoIdentifierCollection*    props, 
                                        const char*                 fcname, 
                                        const char*                 strWhere,
                                        RowidIterator* ri)
: SltReader(connection),
m_bInit(false),
m_fcname(fcname),
m_where(strWhere)
{
    m_ri = ri;
    m_props = FDO_SAFE_ADDREF(props);   
}

DelayedInitReader::~DelayedInitReader()
{
    FDO_SAFE_RELEASE(m_props);
}

bool DelayedInitReader::ReadNext()
{
    if (!m_bInit)
    {
        DelayedInit(m_props, m_fcname.c_str(), m_where.c_str(), true);
        m_bInit = true;
    }

    return SltReader::ReadNext();
}

FdoClassDefinition* DelayedInitReader::GetClassDefinition()
{
    if (!m_bInit)
    {
        DelayedInit(m_props, m_fcname.c_str(), m_where.c_str(), true);
        m_bInit = true;
    }

    return SltReader::GetClassDefinition();
}

SltIdReader::SltIdReader (FdoDataPropertyDefinition* idProp, sqlite3_int64 id)
  : m_refCount(1), m_id(id), m_rdrEnd(false)
{
	m_idProp = FDO_SAFE_ADDREF(idProp);
	m_cls = NULL;
}

SltIdReader::~SltIdReader()
{
	FDO_SAFE_RELEASE(m_idProp);
	FDO_SAFE_RELEASE(m_cls);
}

bool SltIdReader::ReadNext()
{
	bool retVal = !m_rdrEnd;
	m_rdrEnd = true;
	return retVal;
}

FdoClassDefinition* SltIdReader::GetClassDefinition()
{
	if (!m_cls)
	{
		m_cls = FdoFeatureClass::Create(L"GenClass", L"Id class");
		FdoPtr<FdoPropertyDefinitionCollection> props = m_cls->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> pidtcoll = m_cls->GetIdentityProperties();
		FdoPtr<FdoDataPropertyDefinition> idProp = FdoDataPropertyDefinition::Create(m_idProp->GetName(), L"Id");
		idProp->SetDataType(m_idProp->GetDataType());
        pidtcoll->Add(idProp);
		props->Add(idProp);
	}
    return FDO_SAFE_ADDREF(m_cls);
}
FdoInt32 SltIdReader::GetDepth()
{
    return 1;
}

const FdoByte* SltIdReader::GetGeometry(FdoString* /*propertyName*/, FdoInt32* /*count*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoByteArray* SltIdReader::GetGeometry(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoIFeatureReader* SltIdReader::GetFeatureObject(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

const FdoByte* SltIdReader::GetGeometry(FdoInt32 /*index*/, FdoInt32* /*count*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoByteArray* SltIdReader::GetGeometry(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoIFeatureReader* SltIdReader::GetFeatureObject(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

void SltIdReader::CheckProperty(int idx)
{
	if (idx != 0)
    {
		wchar_t tmp[15];
		swprintf(tmp, 15, L"%d", idx);
        throw FdoCommandException::Create((std::wstring(L"Property index \'") + tmp + L"\' is out of bounds.").c_str());
    }
}

void SltIdReader::CheckProperty(FdoString* propertyName)
{
	if (propertyName == NULL || *propertyName != *m_idProp->GetName())
        throw FdoCommandException::Create((std::wstring(L"The property \'") + propertyName + L"\' was not found.").c_str());
}

FdoInt32 SltIdReader::GetColumnCount()
{
    return 1;
}

FdoInt32 SltIdReader::GetColumnIndex(FdoString* columnName)
{
	CheckProperty(columnName);
	return 0;
}

FdoString* SltIdReader::GetColumnName(FdoInt32 index)
{
	CheckProperty(index);
	return m_idProp->GetName();
}

FdoDataType SltIdReader::GetColumnType(FdoString* columnName)
{
	CheckProperty(columnName);
	return m_idProp->GetDataType();
}

FdoDataType SltIdReader::GetColumnType(FdoInt32 index)
{
	CheckProperty(index);
	return m_idProp->GetDataType();
}

FdoInt32 SltIdReader::GetPropertyCount()
{
    return 1;
}

FdoDataType SltIdReader::GetDataType(FdoString* propertyName)
{
	CheckProperty(propertyName);
	return m_idProp->GetDataType();
}

FdoPropertyType SltIdReader::GetPropertyType(FdoString* propertyName)
{
	CheckProperty(propertyName);
	return m_idProp->GetPropertyType();
}

FdoDataType SltIdReader::GetDataType(FdoInt32 index)
{
	CheckProperty(index);
	return m_idProp->GetDataType();
}

FdoPropertyType SltIdReader::GetPropertyType(FdoInt32 index)
{
	CheckProperty(index);
	return m_idProp->GetPropertyType();
}

FdoString* SltIdReader::GetPropertyName(FdoInt32 index)
{
	CheckProperty(index);
	return m_idProp->GetName();
}

FdoInt32 SltIdReader::GetPropertyIndex(FdoString* propertyName)
{
	CheckProperty(propertyName);
	return 0;
}

bool SltIdReader::GetBoolean(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoByte SltIdReader::GetByte(FdoString* propertyName)
{
	return (FdoByte)m_id;
}

FdoDateTime SltIdReader::GetDateTime(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

double SltIdReader::GetDouble(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoInt16 SltIdReader::GetInt16(FdoString* propertyName)
{
	return (FdoInt16)m_id;
}

FdoInt32 SltIdReader::GetInt32(FdoString* propertyName)
{
	return (FdoInt32)m_id;
}

FdoInt64 SltIdReader::GetInt64(FdoString* propertyName)
{
	return (FdoInt32)m_id;
}

float SltIdReader::GetSingle(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoString* SltIdReader::GetString(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoLOBValue* SltIdReader::GetLOB(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoIStreamReader* SltIdReader::GetLOBStreamReader(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

bool SltIdReader::IsNull(FdoString* propertyName)
{
	CheckProperty(propertyName);
	return false;
}

FdoIRaster* SltIdReader::GetRaster(FdoString* /*propertyName*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoBoolean SltIdReader::GetBoolean(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoByte SltIdReader::GetByte(FdoInt32 index)
{
	return (FdoByte)m_id;
}

FdoDateTime SltIdReader::GetDateTime(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoDouble SltIdReader::GetDouble(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoInt16 SltIdReader::GetInt16(FdoInt32 index)
{
	return (FdoInt16)m_id;
}

FdoInt32 SltIdReader::GetInt32(FdoInt32 index)
{
	return (FdoInt32)m_id;
}

FdoInt64 SltIdReader::GetInt64(FdoInt32 index)
{
	return m_id;
}

FdoFloat SltIdReader::GetSingle(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoString* SltIdReader::GetString(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoLOBValue* SltIdReader::GetLOB(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoIStreamReader* SltIdReader::GetLOBStreamReader(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

FdoBoolean SltIdReader::IsNull(FdoInt32 index)
{
	CheckProperty(index);
    return false;
}

FdoIRaster* SltIdReader::GetRaster(FdoInt32 /*index*/)
{
	throw FdoCommandException::Create(L"Invalid data type.");
}

void SltIdReader::Close()
{
	m_rdrEnd = true;
}
