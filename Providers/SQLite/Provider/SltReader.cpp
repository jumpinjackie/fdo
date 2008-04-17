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
#include "DiskSpatialIndex.h"
#include "SltConversionUtils.h"

#include "vdbeInt.h"

using namespace std;


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
  if( pVm && pVm->resOnStack && i<pVm->nResColumn && i>=0 ){
      //we compile SQLite without thread safety...
    //sqlite3_mutex_enter(pVm->db->mutex);
    vals = sqlite3_data_count(pStmt);
    pOut = &pVm->pTos[(1-vals)+i];
  }else{
    static const Mem nullMem = {{0}, 0.0, 0, "", 0, MEM_Null, SQLITE_NULL };
    if( pVm->db ){
        //we compile SQLite without thread safety...
      //sqlite3_mutex_enter(pVm->db->mutex);
      sqlite3Error(pVm->db, SQLITE_RANGE, 0);
    }
    pOut = (Mem*)&nullMem;
  }
  return pOut;
}


//constructor taking a general sql statement, which we will step through
SltReader::SltReader(SltConnection* connection, const char* sql)
: m_refCount(1),
m_sql(sql),
m_class(NULL),
m_sprops(NULL),
m_closeOpcode(-1),
m_si(NULL),
m_nMaxProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(false),
m_bUseTransaction(true),
m_useFastStepping(false)
{
	m_connection = FDO_SAFE_ADDREF(connection);

    m_pStmt = m_connection->GetCachedParsedStatement(m_sql);

    //start the transaction we'll use for this reader
    int rc = sqlite3_exec(sqlite3_db_handle(m_pStmt), "BEGIN;", NULL, NULL, NULL);

	InitPropIndex(m_pStmt);
}

//constructor taking a general sql statement, which we will step through
//Same as above, but this one takes a sqlite3 statement pointer rather than
//a string. This means that it is a statement based on an ephemeral database
//which this reader will close once it is done being read.
SltReader::SltReader(SltConnection* connection, sqlite3_stmt* stmt)
: m_refCount(1),
m_sql(""),
m_class(NULL),
m_sprops(NULL),
m_closeOpcode(-1),
m_si(NULL),
m_nMaxProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(true),
m_bUseTransaction(true),
m_useFastStepping(false)
{
	m_connection = FDO_SAFE_ADDREF(connection);

    m_pStmt = stmt;

    //start the transaction we'll use for this reader
    int rc = sqlite3_exec(sqlite3_db_handle(m_pStmt), "BEGIN;", NULL, NULL, NULL);

	InitPropIndex(m_pStmt);
}


//constructor tailored for an FDO Select command -- in cases where the
//requested columns collection is empty, it will start out with a query
//for just featid and geometry, then redo the query if caller asks for other
//property values
SltReader::SltReader(SltConnection* connection, FdoIdentifierCollection* props, const char* fcname, const char* where, SpatialIterator* si, bool useFastStepping)
: m_refCount(1),
m_pStmt(0),
m_class(NULL),
m_sprops(NULL),
m_closeOpcode(-1),
m_si(si),
m_nMaxProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(false),
m_bUseTransaction(true),
m_useFastStepping(useFastStepping)
{
	m_connection = FDO_SAFE_ADDREF(connection);
    DelayedInit(props, fcname, where);
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
m_nMaxProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(false),
m_bUseTransaction(false),
m_useFastStepping(true)
{
	m_connection = FDO_SAFE_ADDREF(connection);
}


SltReader::~SltReader()
{
	Close();
    FDO_SAFE_RELEASE(m_class);
    delete m_si;
	m_connection->Release();
	delete[] m_sprops;
    delete[] m_wkbBuffer;
}


void SltReader::DelayedInit(FdoIdentifierCollection* props, const char* fcname, const char* where)
{
    int rc = 0;

    if (m_bUseTransaction)
        rc = sqlite3_exec(m_connection->GetDB(), "BEGIN;", NULL, NULL, NULL);

	//first, issue the full statement, and create the FDO feature class from it -- we will present that to the caller
	//so that he sees the properties he asked for. 
	if (props && props->GetCount())
	{
		int nProps = props->GetCount();
		for (int i=0; i<nProps; i++)
		{
			FdoPtr<FdoIdentifier> id = props->GetItem(i);
			string mbnm = W2A_SLOW(id->GetName());
			m_reissueProps.push_back(mbnm);
		}
	}
    
    char* tmpstr = (char*)alloca(strlen(fcname) + strlen(where) + 40);

    //construct the where clause and 
    //if necessary add FeatId filter -- in case we know which features we want
    //like when we have a spatial iterator
    if (*where==0)
    {
        if (m_si)
            sprintf(tmpstr, " FROM %s WHERE ROWID=?;", fcname);
        else
            sprintf(tmpstr, " FROM %s;", fcname);

    }
    else
    {
        if (m_si)
            sprintf(tmpstr, " FROM %s WHERE (%s) AND ROWID=?;", fcname, where);
        else
            sprintf(tmpstr, " FROM %s WHERE (%s);", fcname, where);
    }

    m_fromwhere = tmpstr;

    //remember the geometry encoding format
    SltMetadata* md = m_connection->GetMetadata(fcname);
    m_eGeomFormat = md->GetGeomFormat();

	//if there were properties passed in the identifier collection, assume 
	//the caller knows what they want and we will use the exact query
    //rather than paring down the property list initially
	if (props && props->GetCount() > 0)
	{
        Requery2();
        InitPropIndex(m_pStmt);
        return;
	}
    
    //Caller passed empty property list -- assume the
    //resulting feature class is identical to the one in the schema.
    m_class = md->ToClass();

	//we have created the class definition -- now we can reissue with 
	//only the id and geometry properties to start with, and then reissue
	//with other properties if needed

    //add the id
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = m_class->GetIdentityProperties();
    FdoPtr<FdoDataPropertyDefinition> idp = idpdc->GetItem(0);
    string idname = W2A_SLOW(idp->GetName());
    m_reissueProps.push_back(idname);

    if (m_class->GetClassType() == FdoClassType_FeatureClass)
    {
        //add the geom by doing a requery
        FdoPtr<FdoGeometricPropertyDefinition> gpd = ((FdoFeatureClass*)m_class)->GetGeometryProperty();
        string gpname = W2A_SLOW(gpd->GetName());
        m_reissueProps.push_back(gpname);
    }

    //redo the query with the id and geom props only
    Requery2();    
    InitPropIndex(m_pStmt);
}

void SltReader::InitPropIndex(sqlite3_stmt* pStmt)
{
	m_propNames.clear();
	m_mNameToIndex.Clear();

	int nProps = sqlite3_column_count(pStmt);

	if (nProps > m_nMaxProps)
    {
        delete [] m_sprops;
        m_nMaxProps = nProps;
		m_sprops = new StringRec[m_nMaxProps];
    }

	//cache information about the returned columns
	for (int i=0; i<nProps; i++)
	{
		const char* cname = sqlite3_column_name(pStmt, i);
		std::wstring pname = A2W_SLOW(cname);
		m_propNames.push_back(pname);
	}

	//we cannot do this in the loop above since pointers may change
	//in the m_propNames vector while we are still adding to it --
	//we need the string pointers to remain fixed for use inside this map
	for (int i=0; i<nProps; i++)
        m_mNameToIndex.Add(m_propNames[i].c_str(), i);

    m_mNameToIndex.Prepare();
}

int SltReader::AddColumnToQuery(const wchar_t* name)
{
    //first remember the current feature
    //so that we know to step the new query up to it
    //we know the id is the first property in the query result, since
    //we placed it there
    int cur_id = sqlite3_column_int(m_pStmt, 0);

    //make sure the property exists in the feature class
    FdoPtr<FdoPropertyDefinitionCollection> pdc = m_class->GetProperties();
    FdoPtr<FdoPropertyDefinition> pd = pdc->FindItem(name);

    if (pd.p)
    {
        string mbname = W2A_SLOW(pd->GetName());
        m_reissueProps.push_back(mbname);

        Requery2();
        InitPropIndex(m_pStmt);


        //step till the feature we were at with the previous query
        while (cur_id != sqlite3_column_int(m_pStmt, 0))
            ReadNext();

        //return the index of the new property
        return (int)m_reissueProps.size() - 1;
    }
    else
    {
        throw FdoCommandException::Create(L"Column does not exist in the select query.");
    }

    return -1;
}

void SltReader::Requery2()
{
    if (m_pStmt) 
        m_connection->ReleaseParsedStatement(m_sql, m_pStmt);

    m_sql = "SELECT ";

    if (m_reissueProps.empty())
    {
        m_sql += "*";
    }
    else
    {
        for (size_t i=0; i<m_reissueProps.size(); i++)
        {
		    if (i) m_sql += ",";
		    m_sql += m_reissueProps[i];
        }
    }

    m_sql += m_fromwhere;

    m_curfid = 1;
    m_closeOpcode = -1;

    if (m_si)
    {
        m_siEnd = -1;
        m_si->Reset();
    }

    m_pStmt = m_connection->GetCachedParsedStatement(m_sql);

    //If the reader was constructed for a Select that is known
    //to be safe for fast SQLite reading (no null termination
    //and memcopy for column values is needed), set a flag inidicating
    //that on the SQLite query execution engine
    if (m_useFastStepping)
        ((Vdbe*)m_pStmt)->fdo = 1;
}

	//-------------------------------------------------------
	// FdoIReader implementation
	//-------------------------------------------------------

bool SltReader::GetBoolean(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);
	int res = sqlite3_column_int(m_pStmt, i);
	return res != 0;
}

FdoByte SltReader::GetByte(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);
	int res = sqlite3_column_int(m_pStmt, i);
	return (FdoByte)res;
}

FdoDateTime SltReader::GetDateTime(FdoString* propertyName)
{
    const FdoString* sdt = SltReader::GetString(propertyName);
	return DateFromString(sdt);
}

double SltReader::GetDouble(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);
	double res = sqlite3_column_double(m_pStmt, i);
	return res;
}

FdoInt16 SltReader::GetInt16(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);
	int res = sqlite3_column_int(m_pStmt, i);
	return (FdoInt16)res;
}

FdoInt32 SltReader::GetInt32(FdoString* propertyName)
{
	return GetInt32(NameToIndex(propertyName));
}

FdoInt32 SltReader::GetInt32(int i)
{
	int res = sqlite3_column_int(m_pStmt, i);
	return (FdoInt32)res;
}

FdoInt64 SltReader::GetInt64(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);
	sqlite3_int64 res = sqlite3_column_int64(m_pStmt, i);
	return res;
}

float SltReader::GetSingle(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);
	double res = sqlite3_column_double(m_pStmt, i);
	return (float)res;
}

FdoString* SltReader::GetString(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);

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

        if (!textmem->n) 
            return NULL; //or empty string L"" may be better, since callers may not be used to NULL :)

	    int len = (int)textmem->n;
	    m_sprops[i].EnsureSize(len+1);
	    A2W_FAST(m_sprops[i].data, len + 1, (const char*)textmem->z, len);
	    m_sprops[i].valid = 1;
    }
    else
    {
        const char* text = (const char*)sqlite3_column_text(m_pStmt, i);

        if (!text)
            return NULL;

	    int len = (int)strlen(text);
	    m_sprops[i].EnsureSize(len);
	    A2W_FAST(m_sprops[i].data, len, text);
	    m_sprops[i].valid = 1;
    }

	return m_sprops[i].data;
}

FdoLOBValue* SltReader::GetLOB(FdoString* propertyName)
{
	throw FdoException::Create(L"Call GetGeometry() to get BLOB values.");
}

FdoIStreamReader* SltReader::GetLOBStreamReader(FdoString* propertyName )
{
	throw FdoException::Create(L"Call GetGeometry() to get BLOB values.");
}

bool SltReader::IsNull(FdoString* propertyName)
{
	int i = NameToIndex(propertyName);
	return sqlite3_column_type(m_pStmt, i) == SQLITE_NULL;
}

FdoIRaster* SltReader::GetRaster(FdoString* propertyName)
{
	throw FdoException::Create(L"GetRaster() is not implemented by this provider.");
}

bool SltReader::ReadNext()
{
    //clear the wide string row cache
	for (size_t i=0; i<m_propNames.size(); i++)
		m_sprops[i].valid = 0;

    //yes, we know what we are doing (hopefully)...
    Vdbe* v = (Vdbe*)m_pStmt;

    //use spatial iterator if any
    if (m_si)
    {
        while (1)
        {
            m_curfid ++;

            //are we at the end of the current spatial iterator batch?
            if (m_curfid >= m_siEnd)
            {
                int start;
                bool ret = m_si->NextRange(start, m_siEnd);

                //spatial reader is done, so we are done
                if (!ret)
                    goto done;

                m_curfid = (sqlite3_int64)start;
            }
            

            //we have had at least once successful hit
            if (m_closeOpcode != -1)
            {
                //in order to bind a value to the execution engine
                //it must think that it is not in the middle of executing
                //so we set its program counter to -1 in order to directly
                //set the next row ID we need into the live compiled bytecode.
                v->pc = -1;

                sqlite3_bind_int64(m_pStmt, 1, m_curfid);

                //now set the VDBE program counter to the instruction that
                //fetches the row we set above -- this is to skip initialization
                //instructions which are called when we step the statement after
                //a reset -- we are skipping the reset for speed, so we have 
                //to skip the initialization as well (it would lock the table again without
                //it being freed, since we are not going to finish the previous step
                //which would have freed the previous lock.
                v->pc = 4;
            }
            else
            {
                //situation where we have to reset the statement (SLOWwwwwWWWWwwWWWWw!)
                //Should only happen the first time we execute ReadNext.
                sqlite3_reset(m_pStmt);
                sqlite3_bind_int64(m_pStmt, 1, m_curfid);
            }

            if (sqlite3_step(m_pStmt) == SQLITE_ROW)
            {
                //the opcode that will be execute after a step
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
        if (sqlite3_step(m_pStmt) == SQLITE_ROW)
        {
		    return true;
        }
    }

done:
    //execute the instructions that end the VDBE execution loop
    if (m_closeOpcode != -1)
    {
        v->pc = m_closeOpcode;
        int rc = sqlite3_step(m_pStmt);
    }

	return false;
}

void SltReader::Close()
{
	if (!m_pStmt)
		return;

    sqlite3* db = sqlite3_db_handle(m_pStmt);

    int rc;

    if (m_bUseTransaction)
        rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

    //Close the database as well, if it was an ephemeral database
    //used to return computed data
    if (m_closeDB)
    {
        sqlite3_finalize(m_pStmt);
        sqlite3_close(db);
    }
    else //otherwise just release the cached statement we were using
        m_connection->ReleaseParsedStatement(m_sql, m_pStmt);


	m_pStmt = NULL;
}

	//-------------------------------------------------------
	// FdoIFeatureReader implementation
	//-------------------------------------------------------

FdoClassDefinition* SltReader::GetClassDefinition()
{
	if (!m_class)
	{
		//decide on a name for the class -- just pick the table name
		//for the first column :)
		const char* table = sqlite3_column_table_name(m_pStmt, 0);
		std::wstring wtable = A2W_SLOW(table);

		m_class = FdoFeatureClass::Create(wtable.c_str(), NULL);
		FdoPtr<FdoPropertyDefinitionCollection> dstpdc = m_class->GetProperties();
		FdoPtr<FdoDataPropertyDefinitionCollection> dstidpdc = m_class->GetIdentityProperties();

		int nProps = sqlite3_column_count(m_pStmt);

		//cache information about the returned columns
		for (int i=0; i<nProps; i++)
		{
			//get name of table that is the source for this column
			const char* table = sqlite3_column_table_name(m_pStmt, i);

			//find source feature class metadata
			SltMetadata* md = m_connection->GetMetadata(table);
			
			if (md)
			{
				FdoPtr<FdoClassDefinition> origfc = md->ToClass();
				FdoPtr<FdoPropertyDefinitionCollection> pdc = origfc->GetProperties();
				FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = origfc->GetIdentityProperties();
				FdoPtr<FdoGeometricPropertyDefinition> geompd = (origfc->GetClassType() == FdoClassType_FeatureClass) 
					? ((FdoFeatureClass*)origfc.p)->GetGeometryProperty() : NULL;

				const std::wstring& pname = m_propNames[i];

				FdoPtr<FdoPropertyDefinition> srcprop = pdc->FindItem(pname.c_str());

                if (!srcprop.p)
                    continue; //TODO: really in this case we need to jump to the else statement that handles generic columns

                FdoPtr<FdoPropertyDefinition> clonedprop = FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition(srcprop);
                
    			dstpdc->Add(clonedprop);

				if (idpdc->Contains(pname.c_str()))
					dstidpdc->Add((FdoDataPropertyDefinition*)clonedprop.p);

				if (wcscmp(pname.c_str(), geompd->GetName()) == 0)
					((FdoFeatureClass*)m_class)->SetGeometryProperty((FdoGeometricPropertyDefinition*)clonedprop.p);
			}
			else
			{
				//case where the reader is a result of arbitrary sql and the
				//resulting columns do not come from any existing table (is this case possible?)
				FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(m_propNames[i].c_str(), NULL);

				//NOTE: Unfortunately, the result of calling this function
				//may vary when called on different rows of the result.
				int type = sqlite3_column_type(m_pStmt, i);
				FdoDataType fdoType = (FdoDataType)-1;

				switch (type)
				{
				case SQLITE_INTEGER: 
					fdoType = FdoDataType_Int64;
					break;
				case SQLITE_TEXT:
					fdoType = FdoDataType_String;
					break;
				case SQLITE_FLOAT:
					fdoType = FdoDataType_Double;
					break;
				case SQLITE_BLOB:
					fdoType = FdoDataType_BLOB;
					break;
				case SQLITE_NULL:
					fdoType = FdoDataType_String; //TODO: what to do here?
					break;
				}

				dpd->SetDataType(fdoType);

				dstpdc->Add(dpd);
			}
		}
	}

	return FDO_SAFE_ADDREF(m_class);
}

FdoInt32 SltReader::GetDepth()
{
	throw FdoException::Create(L"GetDepth() is not implemented by this provider.");
}

FdoIFeatureReader* SltReader::GetFeatureObject(FdoString* propertyName)
{
	throw FdoException::Create(L"GetFeatureObject() is not implemented by this provider.");
}

FdoByteArray* SltReader::GetGeometry(FdoString* propertyName)
{
	int len = 0;
    const void* ptr = SltReader::GetGeometry(propertyName, &len);
	return FdoByteArray::Create((unsigned char*)ptr, len);
}

const FdoByte* SltReader::GetGeometry(FdoString* propertyName, FdoInt32* len)
{
    return SltReader::GetGeometry(NameToIndex(propertyName), len);
}

const FdoByte* SltReader::GetGeometry(int i, FdoInt32* len)
{
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
    else
    {
        throw FdoException::Create(L"Unsupported geometry format.");
    }

    //return NULL;//can't get here.
}


	//-------------------------------------------------------
	// FdoIDataReader implementation
	//-------------------------------------------------------

FdoInt32 SltReader::GetPropertyCount()
{
	return sqlite3_column_count(m_pStmt);
}

FdoString* SltReader::GetPropertyName(FdoInt32 index)
{
	return m_propNames[index].c_str();
}

FdoDataType SltReader::GetDataType(FdoString* propertyName)
{
	FdoPtr<FdoFeatureClass> fc = (FdoFeatureClass*)GetClassDefinition(); //guaranteed to be FeatureClass, see the implementation
	FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();

	FdoPtr<FdoPropertyDefinition> pd = pdc->FindItem(propertyName);

	return ((FdoDataPropertyDefinition*)pd.p)->GetDataType();
}

FdoPropertyType SltReader::GetPropertyType(FdoString* propertyName)
{
	FdoPtr<FdoFeatureClass> fc = (FdoFeatureClass*)GetClassDefinition(); //guaranteed to be FeatureClass, see the implementation
	
	FdoPtr<FdoGeometricPropertyDefinition> gpd = fc->GetGeometryProperty();
		
	if (gpd && wcscmp(propertyName, gpd->GetName()) == 0)
		return FdoPropertyType_GeometricProperty;
	else
		return FdoPropertyType_DataProperty;
}


	//-------------------------------------------------------
	// FdoISQLDataReader implementation
	//-------------------------------------------------------

FdoInt32 SltReader::GetColumnCount()
{
	return GetPropertyCount();
}

FdoString* SltReader::GetColumnName(FdoInt32 index)
{
	return GetPropertyName(index);
}

FdoDataType SltReader::GetColumnType(FdoString* columnName)
{
	return GetDataType(columnName);
}


//-------------------------------------------------------
// FdoISQLDataReader implementation
//-------------------------------------------------------


DelayedInitReader::DelayedInitReader(   SltConnection*              connection, 
                                        FdoIdentifierCollection*    props, 
                                        const char*                 fcname, 
                                        const char*                 where)
: SltReader(connection),
m_bInit(false),
m_fcname(fcname),
m_where(where)
{
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
        DelayedInit(m_props, m_fcname.c_str(), m_where.c_str());
        m_bInit = true;
    }

    return SltReader::ReadNext();
}

FdoClassDefinition* DelayedInitReader::GetClassDefinition()
{
    if (!m_bInit)
    {
        DelayedInit(m_props, m_fcname.c_str(), m_where.c_str());
        m_bInit = true;
    }

    return SltReader::GetClassDefinition();
}
