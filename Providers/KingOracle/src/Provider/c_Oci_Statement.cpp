/*
* Copyright (C) 2006  SL-King d.o.o
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

#include "stdafx.h"



//#include "RegisterMappings.h"
#include "c_OCI_API.h"
#include "c_Oci_Statement.h"


//#include "c_LogAPI.h"






c_Oci_Statement::c_Oci_Statement( c_Oci_Connection* OciConn )
{
  
  m_FetchArraySize = 1;
  
  m_ColumnDataAllocated = 0;
  m_ColumnDataSize = 0;
  m_ColumnDataPtrArray=NULL;
  
  m_HasMoreData=true;
  m_RowsProcessed=0;
  m_RowsFetched=0;
  
  m_OciConn = OciConn;
  m_OciHpStm=NULL;
  OciConn->OciCheckError( OCIHandleAlloc((dvoid *)OciConn->m_OciHpEnvironment, (dvoid **)&m_OciHpStm, (ub4)OCI_HTYPE_STMT, (size_t)0, (dvoid **)0));
}

c_Oci_Statement::~c_Oci_Statement()
{
  if (m_OciHpStm)
    OCIHandleFree((dvoid *) m_OciHpStm, OCI_HTYPE_STMT);
    
    
  DeleteColumnData();    
  
  DeleteBindValues();
}

void c_Oci_Statement::DeleteBindValues()
{
  std::vector<c_BindValueBuffer*>::iterator iter;  
  
  for(iter =  m_VectorBindValue.begin();iter != m_VectorBindValue.end(); iter++  )
  {
    c_BindValueBuffer* buff = *iter;
    delete buff;
  }
  m_VectorBindValue.clear();
}

void c_Oci_Statement::Prepare(const wchar_t* Sql,ub4 Prefetch/*=256*/)
{
  m_HasMoreData=true;
  m_RowsProcessed=0;
  m_RowsFetched=0;
  ClearColumnData();
  DeleteBindValues();
    
  /* parse query */
  m_OciConn->OciCheckError( OCIStmtPrepare(m_OciHpStm, m_OciConn->m_OciHpError, 
    (OraText *)Sql, (ub4)wcslen(Sql)*sizeof(wchar_t), 
    (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT));
    
  m_OciConn->OciCheckError(OCIAttrSet (m_OciHpStm,OCI_HTYPE_STMT,&Prefetch,sizeof(Prefetch),OCI_ATTR_PREFETCH_ROWS,m_OciConn->m_OciHpError));
 
}

/*
Mode: 
    OCI_BATCH_ERRORS - See "Batch Error Mode", for information about this mode.

    OCI_COMMIT_ON_SUCCESS - When a statement is executed in this mode, the current transaction is committed after execution, if execution completes successfully.

    OCI_DEFAULT - Calling OCIStmtExecute() in this mode executes the statement. It also implicitly returns describe information about the select-list.

    OCI_DESCRIBE_ONLY - This mode is for users who wish to describe a query prior to execution. Calling OCIStmtExecute() in this mode does not execute the statement, but it does return the select-list description. To maximize performance, it is recommended that applications execute the statement in default mode and use the implicit describe which accompanies the execution.

    OCI_EXACT_FETCH - Used when the application knows in advance exactly how many rows it will be fetching. This mode turns prefetching off for Oracle release 8 or later mode, and requires that defines be done before the execute call. Using this mode cancels the cursor after the desired rows are fetched and may result in reduced server-side resource usage.

    OCI_PARSE_ONLY - This mode allows the user to parse the query prior to execution. Executing in this mode parses the query and returns parse errors in the SQL, if any. Users must note that this will involve an additional round trip to the server. To maximize performance, it is recommended that the user execute the statement in the default mode which, as part of a bundled operation, parses the statement.

    OCI_STMT_SCROLLABLE_READONLY - Required for the result set to be scrollable. The result set cannot be updated. See "Fetching Results". Cannot be used with any other mode.

*/
int c_Oci_Statement::ExecuteNonQuery(int Mode)
{
  /* execute */
  int status = OCIStmtExecute(m_OciConn->m_OciHpServiceContext, m_OciHpStm, m_OciConn->m_OciHpError, (ub4)1, (ub4)0, 
    (OCISnapshot *)NULL, (OCISnapshot *)NULL, 
    (ub4)Mode);
    
  if( status == OCI_SUCCESS)    
  {
    // get the number of rows processed
    int rows;      
    m_OciConn->OciCheckError( OCIAttrGet((dvoid *)m_OciHpStm, (ub4)OCI_HTYPE_STMT,
      (dvoid *)&rows, (ub4 *)0, 
      (ub4)OCI_ATTR_ROW_COUNT, m_OciConn->m_OciHpError));
    return rows;      
  }
  
  if( status == OCI_NO_DATA)  return 0;  
  
  m_OciConn->OciCheckError( status);
  
  return 0;
  
  
  
}

int c_Oci_Statement::GetColumnsSize()
{
  int size;
  // Get the number of columns in the query 
m_OciConn->OciCheckError(OCIAttrGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, (dvoid *)&size,
                      (ub4 *)0, OCI_ATTR_PARAM_COUNT, m_OciConn->m_OciHpError));
                      
  return size;                      
}
wchar_t* c_Oci_Statement::GetColumnName(int ColumnNumber)
{
  
  
  OCIParam     *param = (OCIParam *) 0;
  m_OciConn->OciCheckError(OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
               (dvoid **)&param, (ub4) ColumnNumber));
               
  
  // Retrieve the column name attribute 
  ub4 col_name_len = 0;
  wchar_t* col_name;
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
           (dvoid**) &col_name, (ub4 *) &col_name_len, (ub4) OCI_ATTR_NAME,
           (OCIError *) m_OciConn->m_OciHpError ));
                      
  return col_name;                      
}
int c_Oci_Statement::GetColumnOciType(int ColumnNumber)
{
  
  
  OCIParam     *param = (OCIParam *) 0;
  m_OciConn->OciCheckError(OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
               (dvoid **)&param, (ub4) ColumnNumber));
               
  
  // Retrieve the column name attribute 
  ub2          dtype;
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
  (dvoid*) &dtype,(ub4 *) 0, (ub4) OCI_ATTR_DATA_TYPE,
  (OCIError *) m_OciConn->m_OciHpError  ));
                      
  return dtype;                      
}
wchar_t* c_Oci_Statement::GetColumnTypeName(int ColumnNumber)
{
  
  
  OCIParam     *param = (OCIParam *) 0;
  m_OciConn->OciCheckError(OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
               (dvoid **)&param, (ub4) ColumnNumber));
               
  
  // Retrieve the column name attribute 
 // Retrieve the column type name attribute 
  wchar_t* col_type_name;
  int col_type_name_len = 0;
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
           (dvoid**) &col_type_name, (ub4 *) &col_type_name_len, (ub4) OCI_ATTR_TYPE_NAME,
           (OCIError *) m_OciConn->m_OciHpError ));
                      
  return col_type_name;                      
}
int c_Oci_Statement::GetColumnWidth(int ColumnNumber)
{
  
  
  OCIParam     *param = (OCIParam *) 0;
  m_OciConn->OciCheckError(OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
               (dvoid **)&param, (ub4) ColumnNumber));
               
  
   // Retrieve the length semantics for the column 
  ub4 char_semantics = 0;
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
           (dvoid*) &char_semantics,(ub4 *) 0, (ub4) OCI_ATTR_CHAR_USED,
           (OCIError *) m_OciConn->m_OciHpError  ));
  ub4 col_width = 0;
  if (char_semantics)
  {
       // Retrieve the column width in characters 
    m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
               (dvoid*) &col_width, (ub4 *) 0, (ub4) OCI_ATTR_CHAR_SIZE,
               (OCIError *) m_OciConn->m_OciHpError  ));
  }
  else
  {
       /// Retrieve the column width in bytes 
    m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
               (dvoid*) &col_width,(ub4 *) 0, (ub4) OCI_ATTR_DATA_SIZE,
               (OCIError *) m_OciConn->m_OciHpError  ));                
  }
                      
  return col_width;                      
}
int c_Oci_Statement::GetColumnPrecision(int ColumnNumber)
{
  
  
  OCIParam     *param = (OCIParam *) 0;
  m_OciConn->OciCheckError(OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
               (dvoid **)&param, (ub4) ColumnNumber));
               
  
     /// Retrieve the column width in bytes 
  ub4 col_precision;     
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
             (dvoid*) &col_precision,(ub4 *) 0, (ub4) OCI_ATTR_PRECISION,
             (OCIError *) m_OciConn->m_OciHpError  ));    
                      
  return col_precision;                      
}
int c_Oci_Statement::GetColumnScale(int ColumnNumber)
{
  
  
  OCIParam     *param = (OCIParam *) 0;
  m_OciConn->OciCheckError(OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
               (dvoid **)&param, (ub4) ColumnNumber));
               
  
     /// Retrieve the column width in bytes 
  ub4 col_scale;     
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) param, (ub4) OCI_DTYPE_PARAM,
             (dvoid*) &col_scale,(ub4 *) 0, (ub4) OCI_ATTR_SCALE,
             (OCIError *) m_OciConn->m_OciHpError  ));    
                      
  return col_scale;                      
}

int c_Oci_Statement::ExecuteSelectAndDefine( int FetchSize, int ExecMode /*= OCI_DEFAULT*/ )
{

m_FetchArraySize = FetchSize;
sword execstatus = OCIStmtExecute(m_OciConn->m_OciHpServiceContext, m_OciHpStm, m_OciConn->m_OciHpError, 0, 0, (OCISnapshot *)0, (OCISnapshot *)0, OCI_DEFAULT);
if( execstatus != OCI_SUCCESS && execstatus!=OCI_NO_DATA )
{
  m_OciConn->OciCheckError(execstatus);
}

OCIParam     *mypard = (OCIParam *) 0;
sb4 parm_status;
ub2          dtype;
wchar_t         *col_name,*col_type_name;
ub4          counter, col_name_len, char_semantics,col_type_name_len;
ub2          col_width;




// Request a parameter descriptor for position 1 in the select-list 
counter = 1;
parm_status = OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
               (dvoid **)&mypard, (ub4) counter);

// Loop only if a descriptor was successfully retrieved for
//   current position, starting at 1 

while (parm_status == OCI_SUCCESS) 
{
   // Retrieve the datatype attribute 
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
           (dvoid*) &dtype,(ub4 *) 0, (ub4) OCI_ATTR_DATA_TYPE,
           (OCIError *) m_OciConn->m_OciHpError  ));

   // Retrieve the column name attribute 
  col_name_len = 0;
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
           (dvoid**) &col_name, (ub4 *) &col_name_len, (ub4) OCI_ATTR_NAME,
           (OCIError *) m_OciConn->m_OciHpError ));
           
             
  // Retrieve the column type name attribute 
  col_type_name_len = 0;
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
           (dvoid**) &col_type_name, (ub4 *) &col_type_name_len, (ub4) OCI_ATTR_TYPE_NAME,
           (OCIError *) m_OciConn->m_OciHpError ));

   // Retrieve the length semantics for the column 
  char_semantics = 0;
  m_OciConn->OciCheckError(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
           (dvoid*) &char_semantics,(ub4 *) 0, (ub4) OCI_ATTR_CHAR_USED,
           (OCIError *) m_OciConn->m_OciHpError  ));
  col_width = 0;
  if (char_semantics)
  {
       // Retrieve the column width in characters 
    m_OciConn->OciCheckError(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
               (dvoid*) &col_width, (ub4 *) 0, (ub4) OCI_ATTR_CHAR_SIZE,
               (OCIError *) m_OciConn->m_OciHpError  ));
  }
  else
  {
       /// Retrieve the column width in bytes 
    m_OciConn->OciCheckError(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM,
               (dvoid*) &col_width,(ub4 *) 0, (ub4) OCI_ATTR_DATA_SIZE,
               (OCIError *) m_OciConn->m_OciHpError  ));                
  }

  DefineColumn(counter,dtype,col_type_name,col_width,FetchSize);
  
  

   // increment counter and get next descriptor, if there is one 
  counter++;
  parm_status = OCIParamGet((dvoid *)m_OciHpStm, OCI_HTYPE_STMT, m_OciConn->m_OciHpError,
          (dvoid **)&mypard, (ub4) counter);
                                
} 

if( execstatus == OCI_NO_DATA ) return 0;


int fetched;
m_OciConn->OciCheckError( OCIAttrGet((dvoid *)m_OciHpStm, (ub4)OCI_HTYPE_STMT,
      (dvoid *)&fetched, (ub4 *)0, 
      (ub4)OCI_ATTR_ROW_COUNT, m_OciConn->m_OciHpError));

return fetched;
  
}//end of c_Oci_Statement::ExecuteSelectAndDefine


int c_Oci_Statement::ExecuteSelect( int FetchSize, int ExecMode /*= OCI_DEFAULT*/ )
{
  m_HasMoreData=true;
  m_RowsProcessed=0;
  m_RowsFetched=0;
  c_Oci_ColumnData*coldata;
  for(int ind=0;ind<m_ColumnDataSize;ind++)
  {
    coldata = m_ColumnDataPtrArray[ind];
    coldata->StartRow();
  }
  
m_FetchArraySize = FetchSize;
sword execstatus = OCIStmtExecute(m_OciConn->m_OciHpServiceContext, m_OciHpStm, m_OciConn->m_OciHpError, 0, 0, (OCISnapshot *)0, (OCISnapshot *)0, OCI_DEFAULT);
if( execstatus != OCI_SUCCESS && execstatus!=OCI_NO_DATA )
{
  m_OciConn->OciCheckError(execstatus);
}


if( execstatus == OCI_NO_DATA ) return 0;


int fetched;
m_OciConn->OciCheckError( OCIAttrGet((dvoid *)m_OciHpStm, (ub4)OCI_HTYPE_STMT,
      (dvoid *)&fetched, (ub4 *)0, 
      (ub4)OCI_ATTR_ROW_COUNT, m_OciConn->m_OciHpError));

return fetched;
  
}//end of c_Oci_Statement::ExecuteSelect




void c_Oci_Statement::DefineColumn( int ColNumber,int DataType,const wchar_t*TypeName,int ColSize,int FetchSize )
{
  if( m_ColumnDataSize >= m_ColumnDataAllocated )
  {
    ReallocColumnData(m_ColumnDataAllocated+32);
  }
  
  c_Oci_ColumnData* coldata = new c_Oci_ColumnData();
  AddColumnData(coldata);
  coldata->Set(m_OciConn,ColNumber,DataType,TypeName,ColSize,FetchSize);
  
  
  OCIDefine *defn1p;
  
    

  
  
    if( coldata->GetDataDefineType() == SQLT_NTY )
    {
      m_OciConn->OciCheckError( OCIDefineByPos(m_OciHpStm, &defn1p, m_OciConn->m_OciHpError, (ub4)ColNumber, 
      (dvoid *)0, 
      0,SQLT_NTY,
      (dvoid *)0, (ub2 *)0, (ub2 *)0, 
      (ub4)OCI_DEFAULT));
      
      m_OciConn->OciCheckError( OCIDefineObject(defn1p, m_OciConn->m_OciHpError, coldata->GetDataOciType(), 
          (dvoid **)coldata->GetDataDefineBuffer(), (ub4 *)0, 
          (dvoid **)coldata->GetDataIndDefineBuffer(), (ub4 *)0));    
          
    }
    else
    {
      m_OciConn->OciCheckError( OCIDefineByPos(m_OciHpStm, &defn1p, m_OciConn->m_OciHpError, (ub4)ColNumber, 
      (dvoid *)coldata->GetDataDefineBuffer(), 
      //(sb4)sizeof(OCINumber), SQLT_VNU,
      coldata->GetDataDefineSize(),coldata->GetDataDefineType(),
      (dvoid *)coldata->GetDataIndDefineBuffer(), (ub2 *)coldata->GetDataRealLengthBuffer(), (ub2 *)0, 
      (ub4)OCI_DEFAULT));
    }
  
    
    
}

bool c_Oci_Statement::IsColumnNull( int ColNumber )
{
  if( (ColNumber<1) || (ColNumber > m_ColumnDataSize) ) throw new c_Oci_Exception(0,0,L"Invalid Column Number");
  
  c_Oci_ColumnData* coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  return coldata->IsNull();
}

bool c_Oci_Statement::ReadNext()
{
  
  
  if( m_RowsProcessed >= m_RowsFetched )  
  {
    
    if( !m_HasMoreData ) return false;
    
    
    int status = OCIStmtFetch2(m_OciHpStm, m_OciConn->m_OciHpError, (ub4)m_FetchArraySize, 
        (ub2)OCI_FETCH_NEXT, 1,(ub4)OCI_DEFAULT);
      //status = OCIStmtFetch(stmthp, ociconn->m_OciHpError, (ub4)D_OCI_ARRAY_SIZE, 
      //  (ub2)OCI_FETCH_NEXT, (ub4)OCI_DEFAULT);
    
    if( status == OCI_NO_DATA ) 
    {
      m_HasMoreData = FALSE; // no more to fetch
    }
    else
    {
      if( status != OCI_SUCCESS )
      {
        m_OciConn->OciCheckError( status);
      }
    }
    m_OciConn->OciCheckError( OCIAttrGet((dvoid *)m_OciHpStm, (ub4)OCI_HTYPE_STMT,
        (dvoid *)&m_RowsFetched, (ub4 *)0, 
        (ub4)OCI_ATTR_ROW_COUNT, m_OciConn->m_OciHpError));      

    //if ((status != OCI_SUCCESS) || m_RowsFetched<=m_RowsProcessed)
    if( m_RowsFetched<=m_RowsProcessed )
    {        
      return false;
    }
  }
  
  c_Oci_ColumnData*coldata;
  for(int ind=0;ind<m_ColumnDataSize;ind++)
  {
    coldata = m_ColumnDataPtrArray[ind];
    coldata->NextRow();
  }
  
  m_RowsProcessed++;
  
  return true;
}//end of 

int c_Oci_Statement::GetInteger( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  int data;
  
  m_OciConn->OciCheckError( OCINumberToInt(m_OciConn->m_OciHpError, coldata->GetOciNumber(), 
        (uword)sizeof(int),OCI_NUMBER_UNSIGNED,(dvoid *)&data));
  
  return data;
}
long c_Oci_Statement::GetLong( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  long data;
  
  m_OciConn->OciCheckError( OCINumberToInt(m_OciConn->m_OciHpError, coldata->GetOciNumber(), 
        (uword)sizeof(long),OCI_NUMBER_UNSIGNED,(dvoid *)&data));
  
  return data;
}
double c_Oci_Statement::GetDouble( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  double data;
  m_OciConn->OciCheckError( OCINumberToReal(m_OciConn->m_OciHpError, coldata->GetOciNumber(), 
        (uword)sizeof(double),(dvoid *)&data));
  
  return data;
}
OCIDate* c_Oci_Statement::GetOciDate( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  return coldata->GetOciDate();
  
  
}
const wchar_t* c_Oci_Statement::GetString( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  
  return coldata->GetString();  
}

long c_Oci_Statement::GetLongRawLength( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  return coldata->GetLongRawLength();
}
unsigned char* c_Oci_Statement::GetLongRaw( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];

  return coldata->GetLongRaw();
}

c_SDO_GEOMETRY* c_Oci_Statement::GetSdoGeom( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  SDO_GEOMETRY_TYPE* geom;
  SDO_GEOMETRY_ind* geom_ind;
  geom = coldata->GetSdoGeom(&geom_ind);
  
  return new c_SDO_GEOMETRY(m_OciConn->m_OciHpEnvironment,m_OciConn->m_OciHpError,geom,geom_ind);  
}
c_SDO_DIM_ARRAY c_Oci_Statement::GetSdoDimArray( int ColNumber )
{
  c_Oci_ColumnData*coldata;
  if( ColNumber<=0 || ColNumber>m_ColumnDataSize ) throw new c_Oci_Exception(0,0,L"c_Oci_Statement:: Invalid ColumnNumber");
  coldata = m_ColumnDataPtrArray[ColNumber-1];
  
  
  
  return c_SDO_DIM_ARRAY(m_OciConn->m_OciHpEnvironment,m_OciConn->m_OciHpError,coldata->GetSdoDimArray());  
}

void c_Oci_Statement::Bind( int ColNumber,dvoid* ValuePtr, sb4 ValueSize,ub2 DataType )
{
  OCIBind  *bnd1p; 
  if( ValuePtr )
  {
    m_OciConn->OciCheckError( OCIBindByPos(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError, 
      (ub4)ColNumber, (dvoid *) ValuePtr,
      ValueSize, DataType, (dvoid *) 0,
      (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
  }            
  else
  { 
    OCIInd ind=OCI_IND_NULL;
    m_OciConn->OciCheckError( OCIBindByPos(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError, 
      (ub4)ColNumber, (dvoid *) 0,
      (sword) 0, DataType, (dvoid *) &ind,
      (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
  }
}

void c_Oci_Statement::Bind( const wchar_t* Name,dvoid* ValuePtr, sb4 ValueSize,ub2 DataType )
{
  OCIBind  *bnd1p; 
  if( ValuePtr )
  {
    m_OciConn->OciCheckError( OCIBindByName(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError
      ,(OraText*)Name,wcslen(Name)*sizeof(wchar_t), (dvoid *) ValuePtr
      ,ValueSize, DataType, (dvoid *) 0,
      (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
  }            
  else
  { 
    OCIInd ind=OCI_IND_NULL;
    m_OciConn->OciCheckError( OCIBindByName(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError 
      ,(OraText*)Name,wcslen(Name), (dvoid *) 0,
      (sword) 0, DataType, (dvoid *) &ind,
      (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
  }
}

void c_Oci_Statement::BindInt( int ColNumber,int* ValuePtr )
{
  Bind(ColNumber,ValuePtr,sizeof(int),SQLT_INT);
}
void c_Oci_Statement::BindIntValue( int ColNumber,int  Value )
{
   c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
    m_VectorBindValue.push_back(newbuffer);
    
    BindInt(ColNumber,&newbuffer->m_Int);  
 

}//end of c_Oci_Statement::BindIntValue
void c_Oci_Statement::BindInt( const wchar_t* Name,int* ValuePtr )
{
  Bind(Name,ValuePtr,sizeof(int),SQLT_INT);
}
void c_Oci_Statement::BindIntValue( const wchar_t* Name,int  Value )
{
  c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
  m_VectorBindValue.push_back(newbuffer);

  BindInt(Name,&newbuffer->m_Int);  


}//end of c_Oci_Statement::BindIntValue

void c_Oci_Statement::BindLong( int ColNumber,long* ValuePtr )
{
  Bind(ColNumber,ValuePtr,sizeof(long),SQLT_INT);
}
void c_Oci_Statement::BindLongValue( int ColNumber,long  Value )
{
   c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
    m_VectorBindValue.push_back(newbuffer);
    
    BindLong(ColNumber,&newbuffer->m_Long);  
 

}//end of c_Oci_Statement::BindLongValue

void c_Oci_Statement::BindLong( const wchar_t* Name,long* ValuePtr )
{
  Bind(Name,ValuePtr,sizeof(long),SQLT_INT);
}
void c_Oci_Statement::BindLongValue( const wchar_t* Name,long  Value )
{
  c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
  m_VectorBindValue.push_back(newbuffer);

  BindLong(Name,&newbuffer->m_Long);  


}//end of c_Oci_Statement::BindLongValue




void c_Oci_Statement::BindOciNumber( int ColNumber,OCINumber* ValuePtr )
{
  Bind(ColNumber,ValuePtr,sizeof(OCINumber),SQLT_VNU);    
}

void c_Oci_Statement::BindOciNumber( const wchar_t* Name,OCINumber* ValuePtr )
{
  Bind(Name,ValuePtr,sizeof(OCINumber),SQLT_VNU);
 
}




void c_Oci_Statement::BindDouble( int ColNumber,double* ValuePtr )
{
  if( ValuePtr )
  {
    BindDoubleValue( ColNumber,*ValuePtr );    
  }            
  else
  {     
    Bind(ColNumber,NULL,0,SQLT_VNU);               
  }
}
void c_Oci_Statement::BindDoubleValue( int ColNumber,double  Value )
{
  OCINumber oci_number;

  m_OciConn->OciCheckError( OCINumberFromReal(m_OciConn->m_OciHpError, (dvoid *)&Value,(uword)sizeof(double), &oci_number));
    
  c_BindValueBuffer* newbuffer = new c_BindValueBuffer(oci_number);
  m_VectorBindValue.push_back(newbuffer);
    
  BindOciNumber(ColNumber,&newbuffer->m_OciNumber);  
 
}//end of c_Oci_Statement::BindDoubleValue

void c_Oci_Statement::BindDouble( const wchar_t* Name,double* ValuePtr )
{
  if( ValuePtr )
  {
    BindDoubleValue( Name,*ValuePtr );    
  }            
  else
  {     
    Bind(Name,NULL,0,SQLT_VNU);               
  }
}
void c_Oci_Statement::BindDoubleValue( const wchar_t* Name,double  Value )
{
  OCINumber oci_number;

  m_OciConn->OciCheckError( OCINumberFromReal(m_OciConn->m_OciHpError, (dvoid *)&Value,(uword)sizeof(double), &oci_number));

  c_BindValueBuffer* newbuffer = new c_BindValueBuffer(oci_number);
  m_VectorBindValue.push_back(newbuffer);

  BindOciNumber(Name,&newbuffer->m_OciNumber);  

}//end of c_Oci_Statement::BindDoubleValue




void c_Oci_Statement::BindString( int ColNumber,const wchar_t* ValuePtr )
{
  if( ValuePtr )
  {
    int bytesize = wcslen(ValuePtr)*sizeof(wchar_t)+sizeof(wchar_t); // + one more for zero value
    Bind(ColNumber,(void*)ValuePtr,bytesize,SQLT_STR);
    
  }
  else
  {
    Bind(ColNumber,NULL,0,SQLT_STR);    
  }

}//end of c_Oci_Statement::BindString
void c_Oci_Statement::BindStringValue( int ColNumber,const wchar_t* ValuePtr )
{
  if( ValuePtr )
  {
    c_BindValueBuffer* newbuffer = new c_BindValueBuffer(ValuePtr);
    m_VectorBindValue.push_back(newbuffer);
    
    BindString(ColNumber,newbuffer->m_String);  
  }
  else
  {
    BindString(ColNumber,NULL);  
  }

}//end of c_Oci_Statement::BindStringValue
void c_Oci_Statement::BindString( const wchar_t* Name,const wchar_t* ValuePtr )
{
  if( ValuePtr )
  {
    int bytesize = wcslen(ValuePtr)*sizeof(wchar_t)+sizeof(wchar_t); // + one more for zero value
    Bind(Name,(void*)ValuePtr,bytesize,SQLT_STR);

  }
  else
  {
    Bind(Name,NULL,0,SQLT_STR);    
  }

}//end of c_Oci_Statement::BindString
void c_Oci_Statement::BindStringValue( const wchar_t* Name,const wchar_t* ValuePtr )
{
  if( ValuePtr )
  {
    c_BindValueBuffer* newbuffer = new c_BindValueBuffer(ValuePtr);
    m_VectorBindValue.push_back(newbuffer);

    BindString(Name,newbuffer->m_String);  
  }
  else
  {
    BindString(Name,NULL);  
  }

}//end of c_Oci_Statement::BindStringValue

void c_Oci_Statement::BindDate( int ColNumber,OCIDate* ValuePtr )
{
  Bind(ColNumber,ValuePtr,sizeof(OCIDate),SQLT_ODT);
}
void c_Oci_Statement::BindDateValue( int ColNumber,OCIDate Value )
{
   c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
   m_VectorBindValue.push_back(newbuffer);
    
   BindDate(ColNumber,&newbuffer->m_Date);  
 

}//end of c_Oci_Statement::BindDateValue

void c_Oci_Statement::BindDate( const wchar_t* Name,OCIDate* ValuePtr )
{
  Bind(Name,ValuePtr,sizeof(OCIDate),SQLT_ODT);
}
void c_Oci_Statement::BindDateValue( const wchar_t* Name,OCIDate Value )
{
  c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
  m_VectorBindValue.push_back(newbuffer);

  BindDate(Name,&newbuffer->m_Date);  


}//end of c_Oci_Statement::BindDateValue

void c_Oci_Statement::BindSdoGeom( int ColNumber,c_SDO_GEOMETRY* ValuePtr )
{
  OCIBind  *bnd1p; 
  //if( ValuePtr )
  {
    
    m_OciConn->OciCheckError( OCIBindByPos(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError, 
              (ub4)ColNumber, (dvoid *) 0,
              (sword) 0, SQLT_NTY, (dvoid *) 0,
               (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
    m_OciConn->OciCheckError( OCIBindObject(bnd1p, m_OciConn->m_OciHpError, 
              m_OciConn->m_OciType_SdoGeometry,  (void**)&ValuePtr->m_SdoGeom,0,(void**)&ValuePtr->m_SdoGeomInd,0));               
  }
  
}
void c_Oci_Statement::BindSdoGeom( const wchar_t* Name,c_SDO_GEOMETRY* ValuePtr )
{
  OCIBind  *bnd1p; 
  //if( ValuePtr )
  {

    m_OciConn->OciCheckError( OCIBindByName(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError 
      ,(OraText*)Name,wcslen(Name)*sizeof(wchar_t), (dvoid *) 0,
      (sword) 0, SQLT_NTY, (dvoid *) 0,
      (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
    m_OciConn->OciCheckError( OCIBindObject(bnd1p, m_OciConn->m_OciHpError, 
      m_OciConn->m_OciType_SdoGeometry,  (void**)&ValuePtr->m_SdoGeom,0,(void**)&ValuePtr->m_SdoGeomInd,0));               
  }

}

void c_Oci_Statement::BindSdoGeomValue( int ColNumber,c_SDO_GEOMETRY* Value )
{
  c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
  m_VectorBindValue.push_back(newbuffer);  
  BindSdoGeom(ColNumber,newbuffer->m_SdoGeom);  
}
void c_Oci_Statement::BindSdoGeomValue( const wchar_t* Name,c_SDO_GEOMETRY* Value )
{
  c_BindValueBuffer* newbuffer = new c_BindValueBuffer(Value);
  m_VectorBindValue.push_back(newbuffer);  
  BindSdoGeom(Name,newbuffer->m_SdoGeom);  
}

void c_Oci_Statement::BindSdoDimElement( int ColNumber,c_SDO_DIM_ELEMENT* ValuePtr )
{
  OCIBind  *bnd1p; 
  //if( ValuePtr )
  {
    
    m_OciConn->OciCheckError( OCIBindByPos(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError, 
              (ub4)ColNumber, (dvoid *) 0,
              (sword) 0, SQLT_NTY, (dvoid *) 0,
               (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
    m_OciConn->OciCheckError( OCIBindObject(bnd1p, m_OciConn->m_OciHpError, 
              m_OciConn->m_OciType_SdoDimElement,  (void**)&ValuePtr->m_Dim_Element,0,(void**)&ValuePtr->m_Dim_Element_Ind,0));               
  }
  
}
void c_Oci_Statement::BindSdoDimElement( const wchar_t* Name,c_SDO_DIM_ELEMENT* ValuePtr )
{
  OCIBind  *bnd1p; 
  //if( ValuePtr )
  {

    m_OciConn->OciCheckError( OCIBindByName(m_OciHpStm, &bnd1p, m_OciConn->m_OciHpError 
      ,(OraText*)Name,wcslen(Name)*sizeof(wchar_t), (dvoid *) 0,
      (sword) 0, SQLT_NTY, (dvoid *) 0,
      (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT));
    m_OciConn->OciCheckError( OCIBindObject(bnd1p, m_OciConn->m_OciHpError, 
      m_OciConn->m_OciType_SdoDimElement,  (void**)&ValuePtr->m_Dim_Element,0,(void**)&ValuePtr->m_Dim_Element_Ind,0));               
  }

}

void c_Oci_Statement::BindBlob( int ColNumber,const char* ValuePtr,long Size )
{
  Bind(ColNumber,(dvoid*)ValuePtr,Size,SQLT_BLOB);
 
}//end of c_Oci_Statement::BindBlob

void c_Oci_Statement::BindBlob( const wchar_t* Name,const char* ValuePtr,long Size )
{
  Bind(Name,(dvoid*)ValuePtr,Size,SQLT_BLOB);

}//end of c_Oci_Statement::BindBlob

void c_Oci_Statement::BindClob( int ColNumber,const char* ValuePtr,long Size )
{
  Bind(ColNumber,(dvoid*)ValuePtr,Size,SQLT_CLOB);
  
  
}//end of c_Oci_Statement::BindClob

void c_Oci_Statement::BindClob( const wchar_t* Name,const char* ValuePtr,long Size )
{
  Bind(Name,(dvoid*)ValuePtr,Size,SQLT_CLOB);

}//end of c_Oci_Statement::BindClob
