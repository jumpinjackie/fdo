/*
* Copyright (C) 2009  SL-King d.o.o
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
#include "c_Ora_API2.h"

#define D_FILTER_OPEN_PARENTH L" ( "
#define D_FILTER_CLOSE_PARENTH L" ) "

// For arthmetic operation
#define D_FILTER_ARITHMETIC_PLUS    L" + "
#define D_FILTER_ARITHMETIC_MINUS   L" - "
#define D_FILTER_ARITHMETIC_MULT    L" * "
#define D_FILTER_ARITHMETIC_DIV     L" / "


c_KgOraExpressionProcessor::c_KgOraExpressionProcessor(c_FilterStringBuffer* StrBuff,c_KgOraSchemaDesc *KgOraSchemaDesc,FdoIdentifier* ClassId,const c_KgOraSridDesc& OraSridDesc,int ParamNumberOffset)
{
  m_StringBuff = StrBuff;
  
  m_KgOraSchemaDesc = KgOraSchemaDesc;
  FDO_SAFE_ADDREF(m_KgOraSchemaDesc.p);
  
  m_ClassId = ClassId;
  FDO_SAFE_ADDREF(m_ClassId.p);
  
  m_OraSridDesc = OraSridDesc;
  
  m_DoAsParameters = true;
  
  m_ParamNumberOffset = ParamNumberOffset;
  
  
  
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
void c_KgOraExpressionProcessor::AppendString(const wchar_t *Str)
{
  m_StringBuff->AppendString(Str);
}

//
// Add a string to the biginning of the buffer
void c_KgOraExpressionProcessor::PrependString(const wchar_t *Str)
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
        AppendString( L" ( - ( " );
        ProcessExpresion( uniExpr );
        AppendString( L" ) ) ");
    }
    else
        throw FdoFilterException::Create(L"FdoUnaryExpression supports only the negate operation" );
}

void c_KgOraExpressionProcessor::ProcessFunction(FdoFunction& expr)
{
  FdoStringP fname = expr.GetName();
  
  
  if( FdoCommonOSUtil::wcsicmp(fname,FDO_FUNCTION_SPATIALEXTENTS) == 0)
  {
    FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping;
    FdoPtr<FdoKgOraClassDefinition> phys_class;
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );

    }
    
    if( phys_class && phys_class->GetIsSdeClass() )
    {
      AppendString( L"min(" );
      AppendString( phys_class->GetSdeGeomTableAlias() );
      AppendString( L".eminx)" );
      
      AppendString( L",min(" );
      AppendString( phys_class->GetSdeGeomTableAlias() );
      AppendString( L".eminy)" );
      
      AppendString( L",max(" );
      AppendString( phys_class->GetSdeGeomTableAlias() );
      AppendString( L".emaxx)" );

      AppendString( L",max(" );
      AppendString( phys_class->GetSdeGeomTableAlias() );
      AppendString( L".emaxy) " );
      
      return;
    }
    else
    {    
      if( m_ConstantSpatialExtent.GetLength() > 0 )
      {      
        AppendString( m_ConstantSpatialExtent );      
        return;
      }
      else
      {
        AppendString( L"SDO_AGGR_MBR" );
      }
    }
  }
  else
  {
    AppendString( fname );
  }
  
  
  
  FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
  
  if( FdoCommonOSUtil::wcsicmp(fname,FDO_FUNCTION_COUNT) == 0)
  {
    if( exprCol->GetCount() == 0 )
    {
      AppendString(L"(*) "); 
      return;
    }
  }
    //AppendString( D_FILTER_OPEN_PARENTH ); // MySql tripped on the leading space 
	AppendString(L"( "); 
    
    
    for(int i=0; i<exprCol->GetCount(); i++ )
    {
        if( i!= 0 )
            AppendString( L", " );

        FdoPtr<FdoExpression>exp = exprCol->GetItem( i );
        ProcessExpresion( exp );
    }
    AppendString( D_FILTER_CLOSE_PARENTH );
}



void c_KgOraExpressionProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& Expr)
{
    FdoPtr<FdoExpression>pExpr = Expr.GetExpression();
    if( pExpr == NULL )
        throw FdoFilterException::Create(L"FdoComputedIdentifier is missing the expression");


// Check if it is SpatialExtent function and SDE class
// It is not enough to override in ProcessFunction to create min(eminx),min(eminy),max(emaxx),max(emaxy)
// but need to avoid adding parethness () and also keyword "AS" , so I proccess that separetly
    FdoFunction* funcex = (dynamic_cast<FdoFunction*>(pExpr.p));
    if( funcex )
    {
      if( FdoCommonOSUtil::wcsicmp(funcex->GetName(),FDO_FUNCTION_SPATIALEXTENTS) == 0)
      {
        FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping;
        FdoPtr<FdoKgOraClassDefinition> phys_class;
        if( m_KgOraSchemaDesc.p && m_ClassId.p )
        {
          phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
          phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );

        }

        if( phys_class && phys_class->GetIsSdeClass() )
        {
          AppendString( L"min(" );
          AppendString( phys_class->GetSdeGeomTableAlias() );
          AppendString( L".eminx)" );

          AppendString( L",min(" );
          AppendString( phys_class->GetSdeGeomTableAlias() );
          AppendString( L".eminy)" );

          AppendString( L",max(" );
          AppendString( phys_class->GetSdeGeomTableAlias() );
          AppendString( L".emaxx)" );

          AppendString( L",max(" );
          AppendString( phys_class->GetSdeGeomTableAlias() );
          AppendString( L".emaxy) " );


          return;
        }
      }
    }
    
    AppendString( D_FILTER_OPEN_PARENTH );
    ProcessExpresion( pExpr );
    AppendString( D_FILTER_CLOSE_PARENTH );
    
    AppendString( L" AS " );
    AppendString( FdoStringP(Expr.GetName()) );
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
      AppendString( L"." );
    }
  }
  
  
  
  AppendString( FdoStringP(Expr.GetName()) );
}



void c_KgOraExpressionProcessor::ProcessParameter( FdoParameter& expr )
{
    long size = m_ParamList.size() + m_ParamNumberOffset;
    size++;
  
    FdoStringP chbuff = FdoStringP::Format(L"%ld",size);
    AppendString( L":" );
    AppendString( (FdoString*)chbuff );
    
    
    m_ParamList.push_back( new c_KgOraSqlParamDesc(expr.GetName()) );
    //FdoStringP str = expr.GetName();
    //AppendString( str );
}

void c_KgOraExpressionProcessor::AddAsParameter(FdoDataValue& Value)
{
  long size = m_ParamList.size() + m_ParamNumberOffset;
  size++;
  FdoStringP chbuff = FdoStringP::Format(L":%ld",size);
  AppendString( (FdoString*)chbuff );
    
    
 m_ParamList.push_back( new c_KgOraSqlParamDesc(&Value) );
 
}//end of c_KgOraExpressionProcessor::AddAsParameter

FdoStringP c_KgOraExpressionProcessor::PushParameter(FdoDataValue& Value)
{
  long size = m_ParamList.size() + m_ParamNumberOffset;
  size++;
  
  m_ParamList.push_back( new c_KgOraSqlParamDesc(&Value) );
  return FdoStringP::Format(L":%ld",size);
  
}//end of c_KgOraExpressionProcessor::PushParameter

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
      AppendString( L"NULL" );
     
    }
    else
    {
      if( expr.GetBoolean() )
          AppendString( L"1" );  // Database bool true
      else
          AppendString( L"0" );  // Database bool false
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
      AppendString( L"NULL" );
     
    }
    else
    {
      wchar_t   val[2];
      val[0] = expr.GetByte();
      val[1] = '\0';
      AppendString( L"'" );
      AppendString( val );
      AppendString( L"'" );
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
      AppendString( L"NULL" );
      return;
    }
    FdoDateTime dt = expr.GetDateTime();
    
    if( dt.IsDateTime() )
    {
      
      AppendString( L"TO_DATE(" );
      FdoStringP tmpValue = FdoStringP::Format(L"'%d-%d-%d %d:%d:%d'",dt.month,dt.day,dt.year,dt.hour,dt.minute,dt.seconds);
      
      AppendString( L",'MM-DD-YYYY HH24:MI:SS')" );
      
      return;
    }
    
    if( dt.IsDate() )
    {
      AppendString( L"TO_DATE(" );
      FdoStringP tmpValue = FdoStringP::Format(L"'%d-%d-%d'",dt.month,dt.day,dt.year);
      
      AppendString( L",'MM-DD-YYYY')" );
      
      return;
    }
    
    if( dt.IsTime() )
    {
      
      AppendString( L"TO_DATE(" );
      FdoStringP  tmpValue = FdoStringP::Format(L"'%d:%d:%d'",dt.hour,dt.minute,dt.seconds);
      
      AppendString( L",'HH24:MI:SS')" );
      
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
      AppendString( L"NULL" );
    }
    else
    {
      FdoStringP tmpValue = FdoStringP::Format(L"%.16g",expr.GetDouble());
      AppendString( (FdoString*)tmpValue );
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
      AppendString( L"NULL" );
    }
    else
    {
      FdoStringP tmpValue = FdoStringP::Format(L"%.8f",expr.GetDecimal());
      AppendString( (FdoString*)tmpValue );
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
      AppendString( L"NULL" );
    }
    else
    {
      FdoStringP tmpValue = FdoStringP::Format(L"%d",(int)expr.GetInt16());
      AppendString( (FdoString*)tmpValue );
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
      AppendString( L"NULL" );
    }
    else
    {
      FdoStringP tmpValue = FdoStringP::Format(L"%ld",(long)expr.GetInt32());
      AppendString( (FdoString*)tmpValue );
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
      AppendString( L"NULL" );
      return; 
    }
    
      FdoStringP tmpValue = FdoStringP::Format(L"%lld", (FdoInt64)(dynamic_cast<FdoInt64Value&>(expr)).GetInt64());

      AppendString((FdoString*)tmpValue);
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
      AppendString( L"NULL" );
     
    }
    else
    {
      FdoStringP tmpValue = FdoStringP::Format(L"%.8f",expr.GetSingle());
      AppendString( (FdoString*)tmpValue );
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
      AppendString( L"NULL" );
     
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
      AppendString( L"NULL" );
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
      AppendString( L"NULL" );
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
  
    long size = m_ParamList.size() + m_ParamNumberOffset;
    size++;
    FdoStringP chbuff = FdoStringP::Format(L"%ld",size);
    AppendString( L":" );
    AppendString( (FdoString*)chbuff );
      
      
    m_ParamList.push_back( new c_KgOraSqlParamDesc(fgf) );  
  
  
  
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
  //SDO_GEOMETRY *sdorect = c_Ora_API2::CreateOptimizedRect(m_OraSridDesc.m_OraSrid,minx,miny,maxx,maxy);
        
         
    long size = m_ParamList.size() + m_ParamNumberOffset;
    size++;
    FdoStringP chbuff = FdoStringP::Format(L"%ld",size);
    AppendString( L":" );
    AppendString( (FdoString*)chbuff );
      
      
   m_ParamList.push_back( new c_KgOraSqlParamDesc(m_OraSridDesc.m_OraSrid,minx,miny,maxx,maxy) );  
  
  
}//end of c_KgOraExpressionProcessor::ProcessGeometryValueRect

int c_KgOraExpressionProcessor::GetSqlParametersCount()
{
  return m_ParamList.size();
      
}//end of c_KgOraExpressionProcessor::GetSqlParametersCount

void c_KgOraExpressionProcessor::ApplySqlParameters(c_Oci_Statement* OciStm,bool IsGeodeticCS,long OraSrid,int ParamOffest/*=0*/)
{
  if( m_ParamList.size() > 0 )
  {
    long psize = m_ParamList.size();
    for(long pind=0;pind<psize;pind++)
    {
      FdoStringP paramname = FdoStringP::Format(L"%d",pind+1+ParamOffest);
      //m_ParamList[pind]->ApplySqlParameter(OciStm,IsGeodeticCS,OraSrid,pind+1+ParamOffest);      
      m_ParamList[pind]->ApplySqlParameter(OciStm,IsGeodeticCS,OraSrid,paramname);      
    }
  }
      
}//end of c_KgOraExpressionProcessor::ApplySqlParameters

void c_KgOraExpressionProcessor::SetConstantSpatialExtent( const wchar_t* ConstantSpatialExtent )
{
  m_ConstantSpatialExtent=ConstantSpatialExtent ? ConstantSpatialExtent : L"";
}
