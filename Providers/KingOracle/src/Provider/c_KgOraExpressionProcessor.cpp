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
#include "c_KgOraExpressionProcessor.h"
#include "c_FilterStringBuffer.h"
#include "c_FgfToSdoGeom.h"
#include "c_Ora_API.h"

#define D_FILTER_OPEN_PARENTH " ( "
#define D_FILTER_CLOSE_PARENTH " ) "

// For arthmetic operation
#define D_FILTER_ARITHMETIC_PLUS    " + "
#define D_FILTER_ARITHMETIC_MINUS   " - "
#define D_FILTER_ARITHMETIC_MULT    " * "
#define D_FILTER_ARITHMETIC_DIV     " / "


c_KgOraExpressionProcessor::c_KgOraExpressionProcessor(c_FilterStringBuffer* StrBuff,c_KgOraSchemaDesc *KgOraSchemaDesc,FdoIdentifier* ClassId,const c_KgOraSridDesc& OraSridDesc)
{
  m_StringBuff = StrBuff;
  
  m_KgOraSchemaDesc = KgOraSchemaDesc;
  FDO_SAFE_ADDREF(m_KgOraSchemaDesc.p);
  
  m_ClassId = ClassId;
  FDO_SAFE_ADDREF(m_ClassId.p);
  
  m_OraSridDesc = OraSridDesc;
  
  m_DoAsParameters = true;
  
}

c_KgOraExpressionProcessor::~c_KgOraExpressionProcessor(void)
{
  long size = m_ParamList.size();
  for(long ind=0;ind<size;ind++)
  {
    delete m_ParamList[ind];
  }
}



void c_KgOraExpressionProcessor::ProcessExpresion( FdoExpression* Expr)
{
  Expr->Process( this );
  
}

//
// Add a string to the end of the buffer
void c_KgOraExpressionProcessor::AppendString(const char *Str)
{
  m_StringBuff->AppendString(Str);
}

//
// Add a string to the biginning of the buffer
void c_KgOraExpressionProcessor::PrependString(const char *Str)
{
  m_StringBuff->PrependString(Str);
}

void c_KgOraExpressionProcessor::ProcessBinaryExpression(FdoBinaryExpression& expr)
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

void c_KgOraExpressionProcessor::ProcessUnaryExpression(FdoUnaryExpression& expr)
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

void c_KgOraExpressionProcessor::ProcessFunction(FdoFunction& expr)
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



void c_KgOraExpressionProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    FdoPtr<FdoExpression>pExpr = expr.GetExpression();
    if( pExpr == NULL )
        throw FdoFilterException::Create(L"FdoComputedIdentifier is missing the expression");

    AppendString( D_FILTER_OPEN_PARENTH );
    ProcessExpresion( pExpr );
    AppendString( D_FILTER_CLOSE_PARENTH );
}


void c_KgOraExpressionProcessor::ProcessIdentifier( FdoIdentifier& Expr)
{
  
  
  if( m_KgOraSchemaDesc.p && m_ClassId.p )
  {
    FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
    FdoPtr<FdoKgOraClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
    
    if( phys_class && !phys_class->GetIsPointGeometry() )
    {      
      AppendString( phys_class->GetOraTableAlias() );
      AppendString( "." );
    }
  }
  
  
  
  AppendString( FdoStringP(Expr.GetName()) );
}



void c_KgOraExpressionProcessor::ProcessParameter( FdoParameter& expr )
{
  long size = m_ParamList.size();
  size++;
  char chbuff[16];
  sprintf(chbuff,"%ld",size);
    AppendString( ":" );
    AppendString( chbuff );
    
    
    m_ParamList.push_back( new c_KgOraSqlParamDesc(expr.GetName()) );
    //FdoStringP str = expr.GetName();
    //AppendString( str );
}

void c_KgOraExpressionProcessor::AddAsParameter(FdoDataValue& Value)
{
  long size = m_ParamList.size();
  size++;
  char chbuff[16];
  sprintf(chbuff,":%ld",size);
  AppendString( chbuff );
    
    
 m_ParamList.push_back( new c_KgOraSqlParamDesc(&Value) );
 
}//end of c_KgOraExpressionProcessor::AddAsParameter

void c_KgOraExpressionProcessor::ProcessBooleanValue(FdoBooleanValue& expr)
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

void c_KgOraExpressionProcessor::ProcessByteValue(FdoByteValue& expr)
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

void c_KgOraExpressionProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
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

void c_KgOraExpressionProcessor::ProcessDoubleValue(FdoDoubleValue& expr)
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

void c_KgOraExpressionProcessor::ProcessDecimalValue(FdoDecimalValue& expr)
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

void c_KgOraExpressionProcessor::ProcessInt16Value(FdoInt16Value& expr)
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

void c_KgOraExpressionProcessor::ProcessInt32Value(FdoInt32Value& expr)
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

void c_KgOraExpressionProcessor::ProcessInt64Value(FdoInt64Value& expr)
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

void c_KgOraExpressionProcessor::ProcessSingleValue(FdoSingleValue& expr)
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

void c_KgOraExpressionProcessor::ProcessStringValue(FdoStringValue& expr)
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

void c_KgOraExpressionProcessor::ProcessBLOBValue(FdoBLOBValue& Expr)
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

void c_KgOraExpressionProcessor::ProcessCLOBValue(FdoCLOBValue& Expr)
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

void c_KgOraExpressionProcessor::ProcessGeometryValue(FdoGeometryValue& Expr)
{
  if( Expr.IsNull() )
  {  
    c_KgOraSqlParamDesc* pdesc = new c_KgOraSqlParamDesc();
    pdesc->SetGeometry(NULL);
    m_ParamList.push_back( pdesc );  
    
    return; 
  }
 
  FdoPtr<FdoByteArray> fgf = Expr.GetGeometry();    
  SDO_GEOMETRY * sdogeom = new SDO_GEOMETRY;
  
  c_FgfToSdoGeom fgftosdo;
  
  if( fgftosdo.ToSdoGeom((int*)fgf->GetData(),m_OraSridDesc.m_OraSrid,sdogeom) == c_FgfToSdoGeom::e_Ok )
  {
    long size = m_ParamList.size();
    size++;
    char chbuff[16];
    sprintf(chbuff,"%ld",size);
    AppendString( ":" );
    AppendString( chbuff );
      
      
   m_ParamList.push_back( new c_KgOraSqlParamDesc(sdogeom) );  
  }
  else
  {
    delete sdogeom;
    c_KgOraSqlParamDesc* pdesc = new c_KgOraSqlParamDesc();
    pdesc->SetGeometry(NULL);
    m_ParamList.push_back( pdesc );  
  }
  
  
}//end of c_KgOraExpressionProcessor::ProcessGeometryValue

// c_KgOraExpressionProcessor::ProcessGeometryValueRect function is King.Oracle specific.
// It is created to be used by spatial filter with EnvelopeIntersect.
// I needed this special case to creaet optimised rectangle from geometry ( not polygon )
// beacuse Oracle will not return features if polygon is to huge for geodetic systems.
// If geometry used in spatial filter is Rectangle then query will return correct features
void c_KgOraExpressionProcessor::ProcessGeometryValueRect(FdoGeometryValue& Expr)
{
  if( Expr.IsNull() )
  {  
    c_KgOraSqlParamDesc* pdesc = new c_KgOraSqlParamDesc();
    pdesc->SetGeometry(NULL);
    m_ParamList.push_back( pdesc );  
    
    return; 
  }
 
  FdoPtr<FdoByteArray> fgf = Expr.GetGeometry();
  FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
  FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
  FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

  double minx = envelope->GetMinX();
  double miny = envelope->GetMinY();

  double maxx = envelope->GetMaxX();
  double maxy = envelope->GetMaxY();

  if( m_OraSridDesc.m_IsGeodetic )
  {
  // if it is geodetic data I need to check boundaries
  // that will not go over 180 and 90
  // MapGuide will send bigger and than oracle wan't return geometries
  // so this is workarround for it
    if( minx < - 180 ) minx = -180.0;
    if( maxx > 180 ) maxx = 180.0;
    
    if( maxx < minx ) { minx=-180; maxx=180; }
    
    if( miny < -90.0 ) miny = -90.0;
    if( maxy > 90.0 ) maxy = 90;
    
    if( maxy < miny ) { miny=-90; maxy=90; }
    if( maxx < minx ) { minx=-180; maxx=180; }

  }

  // create optimize rect
  SDO_GEOMETRY *sdorect = c_Ora_API::CreateOptimizedRect(m_OraSridDesc.m_OraSrid,minx,miny,maxx,maxy);
        
         
    long size = m_ParamList.size();
    size++;
    char chbuff[16];
    sprintf(chbuff,"%ld",size);
    AppendString( ":" );
    AppendString( chbuff );
      
      
   m_ParamList.push_back( new c_KgOraSqlParamDesc(sdorect) );  
  
  
}//end of c_KgOraExpressionProcessor::ProcessGeometryValueRect

int c_KgOraExpressionProcessor::GetSqlParametersCount()
{
  return m_ParamList.size();
      
}//end of c_KgOraExpressionProcessor::GetSqlParametersCount

void c_KgOraExpressionProcessor::ApplySqlParameters(oracle::occi::Environment*Env,oracle::occi::Statement* OraStm,int ParamOffest/*=0*/)
{
  if( m_ParamList.size() > 0 )
  {
    long psize = m_ParamList.size();
    for(long pind=0;pind<psize;pind++)
    {
      m_ParamList[pind]->ApplySqlParameter(Env,OraStm,pind+1+ParamOffest);      
    }
  }
      
}//end of c_KgOraExpressionProcessor::ApplySqlParameters