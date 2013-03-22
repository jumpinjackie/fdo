/*
* Copyright (C) 2007  Haris Kurtagic
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

#include "StdAfx.h"
#include "c_KgMssExpressionProcessor.h"
#include "c_FilterStringBuffer.h"

#define D_FILTER_OPEN_PARENTH " ( "
#define D_FILTER_CLOSE_PARENTH " ) "

// For arthmetic operation
#define D_FILTER_ARITHMETIC_PLUS    " + "
#define D_FILTER_ARITHMETIC_MINUS   " - "
#define D_FILTER_ARITHMETIC_MULT    " * "
#define D_FILTER_ARITHMETIC_DIV     " / "


c_KgMssExpressionProcessor::c_KgMssExpressionProcessor(c_FilterStringBuffer* StrBuff,c_KgMssSchemaDesc *KgOraSchemaDesc
                  ,FdoIdentifier* ClassId,const c_KgMssSridDesc& OraSridDesc,int ParamSqlOffset)
{
  m_StringBuff = StrBuff;
  
  m_KgOraSchemaDesc = KgOraSchemaDesc;
  FDO_SAFE_ADDREF(m_KgOraSchemaDesc.p);
  
  m_ClassId = ClassId;
  FDO_SAFE_ADDREF(m_ClassId.p);
  
  m_OraSridDesc = OraSridDesc;
  
  m_DoAsParameters = true;
  
  m_ParamSqlOffset = ParamSqlOffset;
  
  m_UseTableAllias = true;
  
}

c_KgMssExpressionProcessor::~c_KgMssExpressionProcessor(void)
{
  
  long size = m_ParamList.size();
  for(long ind=0;ind<size;ind++)
  {
    delete m_ParamList[ind];
  }
}



void c_KgMssExpressionProcessor::ProcessExpresion( FdoExpression* Expr)
{
  Expr->Process( this );
  
}

//
// Add a string to the end of the buffer
void c_KgMssExpressionProcessor::AppendString(const char *Str)
{
  m_StringBuff->AppendString(Str);
}

//
// Add a string to the biginning of the buffer
void c_KgMssExpressionProcessor::PrependString(const char *Str)
{
  m_StringBuff->PrependString(Str);
}

void c_KgMssExpressionProcessor::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    FdoPtr<FdoExpression>lftExpr = expr.GetLeftExpression();
    FdoPtr<FdoExpression>rgtExpr = expr.GetRightExpression();
    if( lftExpr == NULL  )
        throw FdoFilterException::Create(L"FdoBinaryExpression is missing the left expression");
    if( rgtExpr == NULL )
        throw FdoFilterException::Create(L"FdoBinaryExpression is missing the right expression");
    AppendString( D_FILTER_OPEN_PARENTH );
    ProcessExpresion( lftExpr );
    switch( expr.GetOperation() )
    {
        case FdoBinaryOperations_Add: AppendString( D_FILTER_ARITHMETIC_PLUS );
            break;
        case FdoBinaryOperations_Subtract: AppendString( D_FILTER_ARITHMETIC_MINUS );
            break;

        case FdoBinaryOperations_Multiply: AppendString( D_FILTER_ARITHMETIC_MULT );
            break;

        case FdoBinaryOperations_Divide: AppendString( D_FILTER_ARITHMETIC_DIV );
            break;

        default:
            throw FdoFilterException::Create(L"ProcessBinaryExpression unsupported binary expression");
    }
    ProcessExpresion( rgtExpr );
    AppendString( D_FILTER_CLOSE_PARENTH );
}

void c_KgMssExpressionProcessor::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    FdoPtr<FdoExpression>uniExpr = expr.GetExpression();
    if( uniExpr == NULL )
        throw FdoFilterException::Create(L"FdoUnaryExpression is missing the expression");

    if( expr.GetOperation() == FdoUnaryOperations_Negate )
    {
        AppendString( " ( - ( " );
        ProcessExpresion( uniExpr );
        AppendString( " ) ) ");
    }
    else
        throw FdoFilterException::Create(L"FdoUnaryExpression supports only the negate operation" );
}

void c_KgMssExpressionProcessor::ProcessFunction(FdoFunction& expr)
{
  FdoStringP fname = expr.GetName();
  AppendString( fname );
  
  
    //AppendString( D_FILTER_OPEN_PARENTH ); // MySql tripped on the leading space 
	AppendString("( "); 
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for(int i=0; i<exprCol->GetCount(); i++ )
    {
        if( i!= 0 )
            AppendString( ", " );

        FdoPtr<FdoExpression>exp = exprCol->GetItem( i );
        ProcessExpresion( exp );
    }
    AppendString( D_FILTER_CLOSE_PARENTH );
}



void c_KgMssExpressionProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    FdoPtr<FdoExpression>pExpr = expr.GetExpression();
    if( pExpr == NULL )
        throw FdoFilterException::Create(L"FdoComputedIdentifier is missing the expression");

    AppendString( D_FILTER_OPEN_PARENTH );
    ProcessExpresion( pExpr );
    AppendString( D_FILTER_CLOSE_PARENTH );
}


void c_KgMssExpressionProcessor::ProcessIdentifier( FdoIdentifier& Expr)
{
  
  
  if( m_KgOraSchemaDesc.p && m_ClassId.p )
  {
    FdoPtr<c_KgMssOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
    FdoPtr<c_KgMssOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
    
    if( phys_class && m_UseTableAllias )
    {      
      AppendString( phys_class->GetOraTableAlias() );
      AppendString( "." );
    }
  }
  
  
  
  AppendString( FdoStringP(Expr.GetName()) );
}



void c_KgMssExpressionProcessor::ProcessParameter( FdoParameter& expr )
{
  long size = m_ParamList.size();
  size = size + 1 + m_ParamSqlOffset;
  char chbuff[16];
  sprintf(chbuff,"%ld",size);
    AppendString( ":" );
    AppendString( chbuff );
    
    
    m_ParamList.push_back( new c_KgMssSqlParamDesc(expr.GetName()) );
    //FdoStringP str = expr.GetName();
    //AppendString( str );
}

void c_KgMssExpressionProcessor::AddAsParameter(FdoDataValue& Value)
{
  long size = m_ParamList.size();
  size = size + 1 + m_ParamSqlOffset;
  char chbuff[64];
  switch ( Value.GetDataType() )
{
    /// Represents a Boolean value of true or false.
    case FdoDataType_Boolean:
      sprintf(chbuff,":%ld<char(1)>",size);
    break;

    /// Represents unsigned 8-bit integers with values between 0 and 255.
    case FdoDataType_Byte:
      sprintf(chbuff,":%ld<char(1)>",size);
    break;

    /// Represents a date and time value.
    case FdoDataType_DateTime:
      sprintf(chbuff,":%ld<timestamp>",size);
    break;

    /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
    /// with 28-29 significant digits.
    case FdoDataType_Decimal:
      sprintf(chbuff,":%ld<double>",size);
    break;

    /// Represents a floating point value ranging from approximately 5.0 
    /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
    case FdoDataType_Double:
      sprintf(chbuff,":%ld<double>",size);
    break;

    /// Represents signed 16-bit integers with values between -32768 and 32767.
    case FdoDataType_Int16:
      sprintf(chbuff,":%ld<short>",size);
    break;

    /// Represents signed 32-bit integers with values between -2147483648 
    /// and 2147483647.
    case FdoDataType_Int32:
      sprintf(chbuff,":%ld<int>",size);
    break;

    /// Represents signed 64-bit integers with values 
    /// between -9223372036854775808 and 9223372036854775807.
    case FdoDataType_Int64:
      sprintf(chbuff,":%ld<bigint>",size);
    break;

    /// Represents floating point values ranging from approximately 1.5 x 10^-45
    /// to 3.4 x 10^38, with a precision of 7 digits. 
    case FdoDataType_Single:
      sprintf(chbuff,":%ld<float>",size);
    break;

    /// Represents a Unicode character strings.
    case FdoDataType_String:
      sprintf(chbuff,":%ld<char(4000)>",size);
    break;

    /// Represents a binary large object stored as a collection of bytes.
    case FdoDataType_BLOB:
      sprintf(chbuff,":%ld<raw_long>",size);
    break;

    /// Represents a character large object stored as a collection of
    /// characters.
    case FdoDataType_CLOB:
      sprintf(chbuff,":%ld<varchar_long>",size);
    break;
}
  
  AppendString( chbuff );
    
    
 m_ParamList.push_back( new c_KgMssSqlParamDesc(&Value) );
 
}//end of c_KgMssExpressionProcessor::AddAsParameter

void c_KgMssExpressionProcessor::ProcessBooleanValue(FdoBooleanValue& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {  
      AppendString( "NULL" );
     
    }
    else
    {
      if( expr.GetBoolean() )
          AppendString( "1" );  // Database bool true
      else
          AppendString( "0" );  // Database bool false
    }
  }
}

void c_KgMssExpressionProcessor::ProcessByteValue(FdoByteValue& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {  
      AppendString( "NULL" );
     
    }
    else
    {
      char   val[2];
      val[0] = expr.GetByte();
      val[1] = '\0';
      AppendString( "'" );
      AppendString( val );
      AppendString( "'" );
    }
  }
}

void c_KgMssExpressionProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {  
    if( expr.IsNull() )
    {  
      AppendString( "NULL" );
      return;
    }
    FdoDateTime dt = expr.GetDateTime();
    
    if( dt.IsDateTime() )
    {
      
      AppendString( "TO_DATE(" );
      char    tmpValue[124];
      sprintf( tmpValue,"'%d-%d-%d %d:%d:%d'",dt.month,dt.day,dt.year,dt.hour,dt.minute,dt.seconds);
      
      AppendString( ",'MM-DD-YYYY HH24:MI:SS')" );
      
      return;
    }
    
    if( dt.IsDate() )
    {
      AppendString( "TO_DATE(" );
      char    tmpValue[124];
      sprintf( tmpValue,"'%d-%d-%d'",dt.month,dt.day,dt.year);
      
      AppendString( ",'MM-DD-YYYY')" );
      
      return;
    }
    
    if( dt.IsTime() )
    {
      
      AppendString( "TO_DATE(" );
      char    tmpValue[124];
      sprintf( tmpValue,"'%d:%d:%d'",dt.hour,dt.minute,dt.seconds);
      
      AppendString( ",'HH24:MI:SS')" );
      
      return;
    }
    
   } 
}

void c_KgMssExpressionProcessor::ProcessDoubleValue(FdoDoubleValue& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {
      AppendString( "NULL" );
    }
    else
    {
      char    tmpValue[124];
      sprintf(tmpValue,"%.16g",expr.GetDouble());
      AppendString( tmpValue );
    }
  }
}

void c_KgMssExpressionProcessor::ProcessDecimalValue(FdoDecimalValue& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {
      AppendString( "NULL" );
    }
    else
    {
      char    tmpValue[124];
      sprintf(tmpValue,"%.8f",expr.GetDecimal());
      AppendString( tmpValue );
    }
  }
}

void c_KgMssExpressionProcessor::ProcessInt16Value(FdoInt16Value& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {
      AppendString( "NULL" );
    }
    else
    {
      char    tmpValue[124];
      sprintf(tmpValue,"%d",(int)expr.GetInt16());
      AppendString( tmpValue );
    }
  }
}

void c_KgMssExpressionProcessor::ProcessInt32Value(FdoInt32Value& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {
      AppendString( "NULL" );
    }
    else
    {
      char    tmpValue[124];
      sprintf(tmpValue,"%ld",(long)expr.GetInt32());
      AppendString( tmpValue );
    }
  }
}

void c_KgMssExpressionProcessor::ProcessInt64Value(FdoInt64Value& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {  
      AppendString( "NULL" );
      return; 
    }
    
      char    tmpValue[124];

  #ifdef _WIN32
	  strncpy( tmpValue, _i64toa( (FdoInt64)(dynamic_cast<FdoInt64Value&>(expr)).GetInt64(),tmpValue, 10 ), 123 );
	  tmpValue[123]='\0';
  #else

	  sprintf(tmpValue, "%lld", (FdoInt64)(dynamic_cast<FdoInt64Value&>(expr)).GetInt64());
  #endif
      AppendString(tmpValue);
    }

}

void c_KgMssExpressionProcessor::ProcessSingleValue(FdoSingleValue& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {  
      AppendString( "NULL" );
     
    }
    else
    {
      char    tmpValue[124];
      sprintf(tmpValue,"%.8f",expr.GetSingle());
      AppendString( tmpValue );
    }
  }
}

void c_KgMssExpressionProcessor::ProcessStringValue(FdoStringValue& expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(expr);
  }
  else
  {
    if( expr.IsNull() )
    {  
      AppendString( "NULL" );
     
    }
    else
    {
      FdoStringP fstr = expr.ToString();
      AppendString( fstr );
    }
  }
}

void c_KgMssExpressionProcessor::ProcessBLOBValue(FdoBLOBValue& Expr)
{
  if( m_DoAsParameters )
  {
    AddAsParameter(Expr);
  }
  else
  {
    if( Expr.IsNull() )
    {  
      AppendString( "NULL" );
      return; 
    }
    throw FdoFilterException::Create(L"Unsupported FDO type in expression");
  }
}

void c_KgMssExpressionProcessor::ProcessCLOBValue(FdoCLOBValue& Expr)
{
   if( m_DoAsParameters )
  {
    AddAsParameter(Expr);
  }
  else
  {
    if( Expr.IsNull() )
    {  
      AppendString( "NULL" );
      return; 
    }
    throw FdoFilterException::Create(L"Unsupported FDO type in expression");
  }
}

void c_KgMssExpressionProcessor::ProcessGeometryValue(FdoGeometryValue& Expr)
{
  
  //FdoStringP gstr = Expr.ToString();
  
  if( Expr.IsNull() )
  {  
    c_KgMssSqlParamDesc* pdesc = new c_KgMssSqlParamDesc();
    pdesc->SetGeometryWkb(NULL);
    m_ParamList.push_back( pdesc );  
    
    return; 
  }
 
  FdoPtr<FdoByteArray> fgf = Expr.GetGeometry();    
  
  FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
  FdoPtr<FdoIGeometry>  fgeom = gf->CreateGeometryFromFgf(fgf);
  FdoPtr<FdoByteArray> wkb = gf->GetWkb(fgeom);
  
   long size = m_ParamList.size();
    size = size + 1 + m_ParamSqlOffset;
    char chbuff[16];
    sprintf(chbuff,"%ld",size);
    AppendString( ":" );
    AppendString( chbuff );
    AppendString( "<raw_long>" );  
      
   m_ParamList.push_back( new c_KgMssSqlParamDesc(wkb) );  
   
   
  
  /*
  if( fgftosdo.ToSdoGeom((int*)fgf->GetData(),m_OraSridDesc.m_OraSrid,sdogeom) == c_FgfToSdoGeom::e_Ok )
  {
    long size = m_ParamList.size();
    size = size + 1 + m_ParamSqlOffset;
    char chbuff[16];
    sprintf(chbuff,"%ld",size);
    AppendString( ":" );
    AppendString( chbuff );
      
      
   m_ParamList.push_back( new c_KgMssSqlParamDesc(sdogeom) );  
  }
  else
  {
    delete sdogeom;
    c_KgMssSqlParamDesc* pdesc = new c_KgMssSqlParamDesc();
    pdesc->SetGeometry(NULL);
    m_ParamList.push_back( pdesc );  
  }
  */
  
  
}//end of c_KgMssExpressionProcessor::ProcessGeometryValue


int c_KgMssExpressionProcessor::GetSqlParametersCount()
{
  return m_ParamList.size();
      
}//end of c_KgMssExpressionProcessor::GetSqlParametersCount



void c_KgMssExpressionProcessor::ApplySqlParameters(otl_stream* OtlStream,int ParamOffest/*=0*/)
{
  if( m_ParamList.size() > 0 )
  {
    long psize = m_ParamList.size();
    for(long pind=0;pind<psize;pind++)
    {
      m_ParamList[pind]->ApplySqlParameter(OtlStream,pind+1+ParamOffest);      
    }
  }
      
}//end of c_KgMssExpressionProcessor::ApplySqlParameters