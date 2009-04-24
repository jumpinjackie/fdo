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
#include "RowidIterator.h"
#include "FdoCommonMiscUtil.h"
#include "vdbeInt.h"


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
    static const Mem nullMem = {{0}, 0.0, 0, "", 0, MEM_Null, SQLITE_NULL, 0, 0, 0 };
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
m_useFastStepping(false),
m_ri(NULL),
m_aPropNames(NULL),
m_fromwhere()
{
	m_connection = FDO_SAFE_ADDREF(connection);

    m_pStmt = m_connection->GetCachedParsedStatement(m_sql.Data());

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
m_useFastStepping(false),
m_ri(NULL),
m_aPropNames(NULL),
m_fromwhere()
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
SltReader::SltReader(SltConnection* connection, FdoIdentifierCollection* props, const char* fcname, const char* where, SpatialIterator* si, bool useFastStepping, RowidIterator* ri)
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
m_bUseTransaction(false),
m_useFastStepping(useFastStepping),
m_ri(ri),
m_aPropNames(NULL),
m_fromwhere()
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
m_ri(NULL),
m_nMaxProps(0),
m_eGeomFormat(eFGF),
m_wkbBuffer(NULL),
m_wkbBufferLen(0),
m_closeDB(false),
m_bUseTransaction(false),
m_useFastStepping(true),
m_aPropNames(NULL),
m_fromwhere()
{
	m_connection = FDO_SAFE_ADDREF(connection);
}


SltReader::~SltReader()
{
	Close();
    FDO_SAFE_RELEASE(m_class);
    delete m_si;
    delete m_ri;
	m_connection->Release();
	delete[] m_sprops;
    delete[] m_wkbBuffer;
    delete[] m_aPropNames;
}


void SltReader::DelayedInit(FdoIdentifierCollection* props, const char* fcname, const char* where)
{
    int rc = 0;

	//first, issue the full statement, and create the FDO feature class from it -- we will present that to the caller
	//so that he sees the properties he asked for. 
	if (props && props->GetCount())
	{
		int nProps = props->GetCount();
        m_reissueProps.Reserve(nProps);
		for (int i=0; i<nProps; i++)
		{
			FdoPtr<FdoIdentifier> id = props->GetItem(i);
            //TODO: dynamic_cast is generally slow -- it's possible to optimize this by using an FdoIExpressionProcessor
            //implementing ProcessIdentifier and ProcessComputedIdentifier to determine what type of identifier we 
            //are working with.
            FdoComputedIdentifier* compId = dynamic_cast<FdoComputedIdentifier*>(id.p);
            if (compId != NULL)
            {
                StringBuffer sb;
                FdoPtr<FdoExpression> exp = compId->GetExpression();
                sb.Append(exp->ToString());
                sb.Append(" AS ", 4);
                sb.AppendDQuoted(compId->GetName());
                m_reissueProps.Add(sb.Data(), sb.Length());
            }
            else
			    m_reissueProps.Add(id->GetName());
		}
	}
    else
    {
        m_reissueProps.Reserve(4);
    }
    

    m_fromwhere.Append(" FROM ", 6);
    m_fromwhere.Append(fcname);

    //construct the where clause and 
    //if necessary add FeatId filter -- in case we know which features we want
    //like when we have a spatial iterator
    if (*where==0)
    {
        if (m_si || m_ri)
            m_fromwhere.Append(" WHERE ROWID=?;", 15);
        else
            m_fromwhere.Append(";", 1);
    }
    else
    {
        m_fromwhere.Append(" WHERE ", 7);
        m_fromwhere.Append(where);

        if (m_si || m_ri)
            m_fromwhere.Append(" AND ROWID=?;", 13);
        else
            m_fromwhere.Append(";", 1);
    }

    //remember the geometry encoding format
    SltMetadata* md = m_connection->GetMetadata(fcname);

    if (!md)
        FdoCommandException::Create(L"Requested feature class does not exist in the database.");

    m_eGeomFormat = md->GetGeomFormat();

    if (m_bUseTransaction)
        rc = sqlite3_exec(m_connection->GetDbRead(), "BEGIN;", NULL, NULL, NULL);

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
    m_reissueProps.Add(idp->GetName());

    if (m_class->GetClassType() == FdoClassType_FeatureClass)
    {
        //add the geom by doing a requery
        FdoPtr<FdoGeometricPropertyDefinition> gpd = ((FdoFeatureClass*)m_class)->GetGeometryProperty();
        m_reissueProps.Add(gpd->GetName());
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
        
        //Note buflen is longer than the column name, but the code we call will terminate when it sees the NULL terminator.
        //We just need to pass in a number that is bigger than the length of the string, and buflen is guaranteed to be.
        int len = 1 + A2W_FAST(dst, buflen, cname, buflen); 

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
        throw FdoException::Create(L"Attempted to access a property which was not listed in the Select command. API misuse by the caller!");

    //make sure the property exists in the feature class
    FdoPtr<FdoPropertyDefinitionCollection> pdc = m_class->GetProperties();
    FdoPtr<FdoPropertyDefinition> pd = pdc->FindItem(name);

    if (pd.p)
    {
        m_reissueProps.Add(pd->GetName());

        Requery2();
        InitPropIndex(m_pStmt);


        //step till the feature we were at with the previous query
        while (cur_id != sqlite3_column_int(m_pStmt, 0))
            ReadNext();

        //return the index of the new property
        return (int)m_reissueProps.Count() - 1;
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
            return L""; //empty string L"" better than NULL, since callers may not be used to NULL :)

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
	    m_sprops[i].EnsureSize(len+1);
	    A2W_FAST(m_sprops[i].data, len+1, text, len);
	    m_sprops[i].valid = 1;
    }

	return m_sprops[i].data;
}

FdoLOBValue* SltReader::GetLOB(FdoString* propertyName)
{
    //TODO: do something similar to GetGeometry
	throw FdoException::Create(L"Not Implemented.");
}

FdoIStreamReader* SltReader::GetLOBStreamReader(FdoString* propertyName )
{
	throw FdoException::Create(L"Not Implemented.");
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

    //use spatial iterator or rowid iterator if any
    //Note that we will not get both a spatial and rowid iterator.
    //The SltConntection::Select() function will pre-process data so that
    //we get either a rowid iterator or a spatial iterator, but not both.
    if (m_si || m_ri)
    {
        while (1)
        {
            //are we at the end of the current spatial iterator batch?
            if (m_si)
            {
                m_curfid ++;
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
                v->aMem[1].u.i = m_curfid;


                //now set the VDBE program counter to the instruction that
                //fetches the row we set above -- this is to skip initialization
                //instructions which are called when we step the statement after
                //a reset -- we are skipping the reset for speed, so we have 
                //to skip the initialization as well (it would lock the table again without
                //it being freed, since we are not going to finish the previous step
                //which would have freed the previous lock.
                v->pc = 5;
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
        if (sqlite3_step(m_pStmt) == SQLITE_ROW)
        {
		    return true;
        }
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
    if (m_closeOpcode != -1)
    {
        Vdbe* v = (Vdbe*)m_pStmt;
        v->pc = m_closeOpcode;
        int rc = sqlite3_step(m_pStmt);

        //must set this back so that a second call to Close() does not
        //reattempt to end the VDBE execution
        m_closeOpcode = -1;
    }

    //remember the underlying ephemeral database, 
    //before killing the statement, so that we can free it,
    //after we commit it... Phew.
    sqlite3* db = sqlite3_db_handle(m_pStmt); 

    //Finalize the statemenet, in case of ephemeral db, or release it to cache otherwise
    //We must do this before committing the transaction, in case we are being Closed()
    //before we were done calling ReadNext()
    if (m_closeDB)
    {
        sqlite3_finalize(m_pStmt);
    }
    else //otherwise just release the cached statement we were using
        m_connection->ReleaseParsedStatement(m_sql.Data(), m_pStmt);

    int rc;
    if (m_bUseTransaction)
        rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

    //Close the database as well, if it was an ephemeral database
    //used to return computed data
    if (m_closeDB)
        sqlite3_close(db);

	m_pStmt = NULL;
}

	//-------------------------------------------------------
	// FdoIFeatureReader implementation
	//-------------------------------------------------------

FdoClassDefinition* SltReader::GetClassDefinition()
{
	if (!m_class)
	{
        std::vector<int> unknownPropsIdx;
		//decide on a name for the class -- just pick the table name
		//for the first column :)
		const char* tableName = sqlite3_column_table_name(m_pStmt, 0);
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
			bool propFound = false;
			//get name of table that is the source for this column
			const char* table = sqlite3_column_table_name(m_pStmt, i);

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

				FdoPtr<FdoPropertyDefinition> srcprop = pdc->FindItem(pname);

                if (!srcprop.p)
                    continue; //TODO: really in this case we need to jump to the else statement that handles generic columns

                FdoPtr<FdoPropertyDefinition> clonedprop = FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition(srcprop);
                propFound = true;
    			dstpdc->Add(clonedprop);

				if (idpdc->Contains(pname))
					dstidpdc->Add((FdoDataPropertyDefinition*)clonedprop.p);

				if (wcscmp(pname, geompd->GetName()) == 0)
					((FdoFeatureClass*)m_class)->SetGeometryProperty((FdoGeometricPropertyDefinition*)clonedprop.p);
			}
            // in case property was not found let's look at the sqlite column
			if (!propFound)
			{
				//case where the reader is a result of arbitrary sql and the
				//resulting columns do not come from any existing table (is this case possible?)
				FdoPtr<FdoDataPropertyDefinition> dpd;

				//NOTE: Unfortunately, the result of calling this function
				//may vary when called on different rows of the result.
				int type = sqlite3_column_type(m_pStmt, i);
				switch (type)
				{
				case SQLITE_INTEGER: 
                    dpd = FdoDataPropertyDefinition::Create(m_propNames[i], NULL);
    				dpd->SetDataType(FdoDataType_Int64);
					break;
				case SQLITE_TEXT:
                    dpd = FdoDataPropertyDefinition::Create(m_propNames[i], NULL);
    				dpd->SetDataType(FdoDataType_String);
					break;
				case SQLITE_FLOAT:
                    dpd = FdoDataPropertyDefinition::Create(m_propNames[i], NULL);
    				dpd->SetDataType(FdoDataType_Double);
					break;
				case SQLITE_BLOB:
                    dpd = FdoDataPropertyDefinition::Create(m_propNames[i], NULL);
    				dpd->SetDataType(FdoDataType_BLOB);
					break;
				case SQLITE_NULL:
                    // we will process them later at the end...
                    unknownPropsIdx.push_back(i);
					break;
				}
                if (dpd != NULL)
				    dstpdc->Add(dpd);
			}
		}
        // do we have unknown calculations !?
        if (unknownPropsIdx.size() != 0)
        {
			FdoPtr<FdoClassDefinition> origfc = mainMd->ToClass();
            // use the original class in case we have one otherwise use the generated class
            FdoClassDefinition* origClass = (origfc == NULL) ? m_class : origfc;
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
                    FdoPtr<FdoExpression> expr = FdoExpression::Parse(expression.c_str());
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
                    FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(m_propNames[unknownPropsIdx.at(idx)], NULL);
				    dpd->SetDataType(FdoDataType_String);
                    pdToAdd = dpd;
                }
                if (pdToAdd != NULL)
				    dstpdc->Add(pdToAdd);
            }
        }
	}

	return FDO_SAFE_ADDREF(m_class);
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

    //is geometry null?
    if (!len) return NULL;

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


	//-------------------------------------------------------
	// FdoIDataReader implementation
	//-------------------------------------------------------

FdoInt32 SltReader::GetPropertyCount()
{
	return sqlite3_column_count(m_pStmt);
}

FdoString* SltReader::GetPropertyName(FdoInt32 index)
{
	return m_propNames[index];
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
// DelayedReader implementation
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

