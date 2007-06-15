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

#include "StdAfx.h"
#include "c_KgOraFilterProcessor.h"
#include "c_FgfToSdoGeom.h"
#include "c_Ora_API.h"


#define D_FILTER_OPEN_PARENTH " ( "
#define D_FILTER_CLOSE_PARENTH " ) "

#define D_FILTER_COMMA " ( "
#define D_FILTER_LOGICAL_AND " AND "
#define D_FILTER_LOGICAL_OR " OR "
#define D_FILTER_LOGICAL_NOT " NOT "
#define D_FILTER_IN " IN "
#define D_FILTER_IS_NULL " IS NULL "

// For comparison operation
#define D_FILTER_EQUAL_OP               " = "
#define D_FILTER_NOT_EQUAL_OP           " <> "
#define D_FILTER_GREATER_THAN_OP        " > "
#define D_FILTER_GREATER_OR_EQUAL_OP    " >= "
#define D_FILTER_LESS_THAN_OP           " < "
#define D_FILTER_LESS_OR_EQUAL_OP       " <= "
#define D_FILTER_LIKE_OP                " LIKE "





c_KgOraFilterProcessor::c_KgOraFilterProcessor(c_KgOraSchemaDesc *KgOraSchemaDesc,FdoIdentifier* ClassId,const c_KgOraSridDesc& OraSridDesc)
  : m_ExpressionProcessor( &m_StringBuff,KgOraSchemaDesc,ClassId,OraSridDesc ) // they will share same string buffer
{
  m_KgOraSchemaDesc = KgOraSchemaDesc;
  FDO_SAFE_ADDREF(m_KgOraSchemaDesc.p);
  
  m_ClassId = ClassId;
  FDO_SAFE_ADDREF(m_ClassId.p);
  
  if( m_KgOraSchemaDesc.p && m_ClassId.p )
  {
    FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
    m_ClassDef = phschemamapping->FindByClassName( m_ClassId->GetName() );
    
    /*
    if( phys_class && phys_class->GetIsPointGeometry() )
    {      
      AppendString( phys_class->GetOraTableAlias() );
      AppendString( "." );
    }
    */
  }
  
  m_OraSridDesc = OraSridDesc;
  
}

c_KgOraFilterProcessor::~c_KgOraFilterProcessor(void)
{
  
}

void c_KgOraFilterProcessor::ProcessFilter(FdoFilter* Filter)
{
  Filter->Process(this);
}//end of c_KgOraFilterProcessor::ProcessFilter

void c_KgOraFilterProcessor::ProcessExpresion( FdoExpression* Expr,bool IsSpatialCondition,bool IsEnvelopeIntersect)
{  
  Expr->Process( &m_ExpressionProcessor );
   
}


//
// Add a string to the end of the buffer
void c_KgOraFilterProcessor::AppendString(const char *Str)
{
  m_StringBuff.AppendString(Str);
}

//
// Add a string to the biginning of the buffer
void c_KgOraFilterProcessor::PrependString(const char *Str)
{
  m_StringBuff.PrependString(Str);
}




/// \brief
/// Processes the FdoBinaryLogicalOperator passed in as an argument.
/// 
/// \param Filter 
/// Input the FdoBinaryLogicalOperator
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraFilterProcessor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& Filter)
{
  FdoPtr<FdoFilter>leftop = Filter.GetLeftOperand();
  FdoPtr<FdoFilter>rightop = Filter.GetRightOperand();
  AppendString(D_FILTER_OPEN_PARENTH);
  if( Filter.GetOperation() == FdoBinaryLogicalOperations_And )
  {
    
    ProcessFilter( leftop );  
    AppendString( D_FILTER_LOGICAL_AND );
    ProcessFilter( rightop );
  }
  else
  {   
    ProcessFilter( leftop );
    AppendString( D_FILTER_LOGICAL_OR );
    ProcessFilter( rightop );
  }

  AppendString(D_FILTER_CLOSE_PARENTH);
  
}//end of c_KgOraFilterProcessor::ProcessBinaryLogicalOperator

void c_KgOraFilterProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& Filter)
{
    FdoPtr<FdoFilter>unaryop = Filter.GetOperand();
    if( unaryop == NULL ) throw FdoFilterException::Create(L"FdoUnaryLogicalOperator is missing the operand");

    AppendString(D_FILTER_OPEN_PARENTH);
    if ( Filter.GetOperation() == FdoUnaryLogicalOperations_Not )
    {
        AppendString( D_FILTER_LOGICAL_NOT );
    }
    else
        throw FdoFilterException::Create(L"FdoUnaryLogicalOperator supports only the 'Not' operation");

    ProcessFilter( unaryop );
    AppendString(D_FILTER_CLOSE_PARENTH);
}//end of c_KgOraFilterProcessor::ProcessUnaryLogicalOperator

void c_KgOraFilterProcessor::ProcessComparisonCondition(FdoComparisonCondition& Filter)
{
    FdoPtr<FdoExpression> leftexp = Filter.GetLeftExpression();
    FdoPtr<FdoExpression> rightexp = Filter.GetRightExpression();
    if( leftexp == NULL  ) throw FdoFilterException::Create(L"FdoComparisonCondition is missing the left expression");
    if( rightexp == NULL ) throw FdoFilterException::Create(L"FdoComparisonCondition is missing the right expression");

    AppendString( D_FILTER_OPEN_PARENTH );
    ProcessExpresion( leftexp );

    switch ( Filter.GetOperation() )
    {
        case FdoComparisonOperations_EqualTo: 
          AppendString( D_FILTER_EQUAL_OP );
        break;

        case FdoComparisonOperations_NotEqualTo: 
          AppendString( D_FILTER_NOT_EQUAL_OP );
        break;

        case FdoComparisonOperations_GreaterThan: 
          AppendString( D_FILTER_GREATER_THAN_OP );
        break;

        case FdoComparisonOperations_GreaterThanOrEqualTo: 
          AppendString( D_FILTER_GREATER_OR_EQUAL_OP );
        break;

        case FdoComparisonOperations_LessThan: 
          AppendString( D_FILTER_LESS_THAN_OP );
        break;

        case FdoComparisonOperations_LessThanOrEqualTo: 
          AppendString( D_FILTER_LESS_OR_EQUAL_OP );
        break;

        case FdoComparisonOperations_Like: 
          AppendString( D_FILTER_LIKE_OP );
        break;
        default:
          throw FdoFilterException::Create(L"FdoComparisonCondition unkown comparison operation");
        break;
    }
    ProcessExpresion( rightexp );
    AppendString( D_FILTER_CLOSE_PARENTH );
}//end of c_KgOraFilterProcessor::ProcessComparisonCondition


void c_KgOraFilterProcessor::ProcessInCondition(FdoInCondition& Filter)
{
    int i;
    FdoPtr<FdoExpression> exp;
    FdoPtr<FdoIdentifier> id = Filter.GetPropertyName();
    if( id == NULL )
        throw FdoFilterException::Create(L"FdoInCondition is missing the property name");

    FdoPtr<FdoValueExpressionCollection>expressions = Filter.GetValues();
    if( expressions == NULL || expressions->GetCount() <= 0 )
        throw FdoFilterException::Create(L"FdoInCondition has an empty value list");

    AppendString(D_FILTER_OPEN_PARENTH);
    ProcessExpresion( id );
    AppendString( D_FILTER_IN );
    AppendString(D_FILTER_OPEN_PARENTH);
    for(i=0; i < expressions->GetCount()-1; i++ )
    {
        exp = expressions->GetItem(i);
        ProcessExpresion( exp );
        AppendString( D_FILTER_COMMA );
    }
    exp = expressions->GetItem( i );
    ProcessExpresion(exp);
    AppendString(D_FILTER_CLOSE_PARENTH);
    AppendString(D_FILTER_CLOSE_PARENTH);
}//end of c_KgOraFilterProcessor::ProcessInCondition

void c_KgOraFilterProcessor::ProcessNullCondition(FdoNullCondition& Filter)
{
    FdoPtr<FdoIdentifier>id = Filter.GetPropertyName();
    if( id == NULL )
        throw FdoFilterException::Create(L"FdoNullCondition is missing the property name");

    AppendString(D_FILTER_OPEN_PARENTH);
    ProcessExpresion( id );
    AppendString( D_FILTER_IS_NULL );
    AppendString(D_FILTER_CLOSE_PARENTH);
    
}//end of c_KgOraFilterProcessor::ProcessNullCondition


void c_KgOraFilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& Filter)
{
FdoPtr<FdoIdentifier> geomprop = Filter.GetPropertyName();

FdoPtr<FdoExpression> geomexp = Filter.GetGeometry();

switch( Filter.GetOperation() )
{
  case FdoSpatialOperations_EnvelopeIntersects:
  {
    if( m_ClassDef.p &&  m_ClassDef->GetIsPointGeometry() )
    {
      FdoStringP str_xcol =  m_ClassDef->GetPointXOraColumn();
      FdoStringP str_ycol =  m_ClassDef->GetPointYOraColumn();
      
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
        FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

        double minx = envelope->GetMinX();
        double miny = envelope->GetMinY();
        
        double maxx = envelope->GetMaxX();
        double maxy = envelope->GetMaxY();

        char buff[512];
        
        AppendString(D_FILTER_OPEN_PARENTH);
        
        AppendString(str_xcol);
        AppendString(">=");       
        sprintf(buff,"%.8lf",minx);
        AppendString(buff);
        
        AppendString(" and ");
        
        AppendString(str_xcol);
        AppendString("<=");        
        sprintf(buff,"%.8lf",maxx);
        AppendString(buff);
        
        AppendString(" and ");
        
        AppendString(str_ycol);
        AppendString(">=");        
        sprintf(buff,"%.8lf",miny);
        AppendString(buff);
        
        AppendString(" and ");
        
        AppendString(str_ycol);
        AppendString("<=");        
        sprintf(buff,"%.8lf",maxy);
        AppendString(buff);
        
        AppendString(D_FILTER_CLOSE_PARENTH);        
      }      
    }
    else
    {
     
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString("SDO_FILTER(");
      ProcessExpresion( geomprop );
      AppendString(",");
      
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        GetExpressionProcessor().ProcessGeometryValueRect(*geomval);
      }
      else
      {
        ProcessExpresion( geomexp,true );
      }
         
      
      AppendString(")='TRUE'");
      AppendString(D_FILTER_CLOSE_PARENTH);
    
    
    /*
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString("SDO_FILTER(");
      ProcessExpresion( geomprop );
      AppendString(",");
      
      // Here I need spatial case of processing becase i want to create
      // optimezed rect (not polygons) for  this query - needed for geodetic data 
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
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
        char * buff = c_Ora_API::SdoGeomToString(sdorect);
      
        if( buff )
        {
          AppendString( buff );
          delete buff;
        }
        
        delete sdorect;
      }
      else
      {
        ProcessExpresion( geomexp,true );
      }
      
      
      AppendString(")='TRUE'");
      AppendString(D_FILTER_CLOSE_PARENTH);
    
    */
    }    
  }
  break;
  case FdoSpatialOperations_Intersects:
  {
    if( m_ClassDef.p &&  m_ClassDef->GetIsPointGeometry() )
    {
      FdoStringP str_xcol =  m_ClassDef->GetPointXOraColumn();
      FdoStringP str_ycol =  m_ClassDef->GetPointYOraColumn();
      
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
        FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

        double minx = envelope->GetMinX();
        double miny = envelope->GetMinY();
        
        double maxx = envelope->GetMaxX();
        double maxy = envelope->GetMaxY();    

        char buff[512];
        
        AppendString(D_FILTER_OPEN_PARENTH);
        
        AppendString(str_xcol);
        AppendString(">=");       
        sprintf(buff,"%.8lf",minx);
        AppendString(buff);
        
        AppendString(" and ");
        
        AppendString(str_xcol);
        AppendString("<=");        
        sprintf(buff,"%.8lf",maxx);
        AppendString(buff);
        
        AppendString(" and ");
        
        AppendString(str_ycol);
        AppendString(">=");        
        sprintf(buff,"%.8lf",miny);
        AppendString(buff);
        
        AppendString(" and ");
        
        AppendString(str_ycol);
        AppendString("<=");        
        sprintf(buff,"%.8lf",maxy);
        AppendString(buff);
        
        AppendString(D_FILTER_CLOSE_PARENTH);        
      }
    }
    else
    {    
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString("SDO_ANYINTERACT(");
      ProcessExpresion( geomprop );
      AppendString(",");
      
      ProcessExpresion( geomexp,true );
      /*
      // Here I need spatial case of processing becase i want to create
      // optimezed rect (not polygons) for  this query - needed for geodetic data 
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
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
          
          if( miny < -90.0 ) miny = -90.0;
          if( maxy > 90.0 ) maxy = 90;
        }

      // create optimize rect
        SDO_GEOMETRY *sdorect = c_Ora_API::CreateOptimizedRect(m_OraSridDesc.m_OraSrid,minx,miny,maxx,maxy);
        char * buff = c_Ora_API::SdoGeomToString(sdorect);
      
        if( buff )
        {
          AppendString( buff );
          delete buff;
        }
        
        delete sdorect;
        
      }
      else
      {
        ProcessExpresion( geomexp,true );
      }
      */
      
      
      AppendString(")='TRUE'");
      AppendString(D_FILTER_CLOSE_PARENTH);
    }
  }
  break;
  
  
  /// Test to see if the geometric property value spatially contains the
  /// literal geometric value.
  case FdoSpatialOperations_Contains:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_RELATE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomexp,true );
    AppendString(",'mask=CONTAINS')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

  /// Test to see if the geometric property value spatially crosses the given
  /// geometry.
  case FdoSpatialOperations_Crosses:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_RELATE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomexp,true );
    AppendString(",'mask=OVERLAPBDYDISJOINT')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

  case FdoSpatialOperations_Disjoint:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_RELATE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomexp,true );
    AppendString(",'mask=DISJOINT')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;
  /// Test to see if the geometric property value spatially overlaps the given
  /// geometry
  case FdoSpatialOperations_Overlaps:
    {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_RELATE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomexp,true );
    AppendString(",'mask=OVERLAPBDYINTERSECT')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

    /// Test to see if the geometric property value spatially touches the given
    /// geometry.
  case FdoSpatialOperations_Touches:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_RELATE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomexp,true );
    AppendString(",'mask=TOUCH')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

    /// Test to see if the geometric property value is spatially within the
    /// given geometry.
  case FdoSpatialOperations_Within:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_RELATE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomexp,true );
    AppendString(",'mask=COVERS')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

    /// Test to see if the geometric property value is covered by the interior
    /// and boundary of the given geometry.
  case FdoSpatialOperations_CoveredBy:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_RELATE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomexp,true );
    AppendString(",'mask=COVERDBY')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

  /// Test to see if the geometric property value is inside the interior
  /// of the given geometry, not touching the boundary.
  case FdoSpatialOperations_Inside:
  {
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString("SDO_RELATE(");
      ProcessExpresion( geomprop );
      AppendString(",");
      ProcessExpresion( geomexp,true );
      AppendString(",'mask=INSIDE')='TRUE'");
      AppendString(D_FILTER_CLOSE_PARENTH);
    }
  break;

  

    /// Test to see if the geometric property value is spatially equal to the
    /// given geometry.
    case FdoSpatialOperations_Equals:
    {
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString("SDO_RELATE(");
      ProcessExpresion( geomprop );
      AppendString(",");
      ProcessExpresion( geomexp,true );
      AppendString(",'mask=EQUAL')='TRUE'");
      AppendString(D_FILTER_CLOSE_PARENTH);
    }
    break;
    
    
  default:
    throw FdoFilterException::Create(L"c_KgOraFilterProcessor::ProcessSpatialCondition Unsupported Spatial Condition Operation");
  break;  
}
}//end of c_KgOraFilterProcessor::ProcessSpatialCondition

void c_KgOraFilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& Filter)
{
FdoPtr<FdoIdentifier> geomprop = Filter.GetPropertyName();

FdoPtr<FdoExpression> geomval = Filter.GetGeometry();
double dist = Filter.GetDistance();
switch( Filter.GetOperation() )
{
  case FdoDistanceOperations_Within:
  {
    // sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
    
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_WITHIN_DISTANCE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomval );
    char tmpbuff[64];
    sprintf(tmpbuff,",'distance=%.6lf'",dist);
    AppendString(tmpbuff);
    AppendString(")='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;
  case FdoDistanceOperations_Beyond:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString("SDO_WITHIN_DISTANCE(");
    ProcessExpresion( geomprop );
    AppendString(",");
    ProcessExpresion( geomval );
    char tmpbuff[64];
    sprintf(tmpbuff,",'distance=%.6lf'",dist);
    AppendString(tmpbuff);
    AppendString(")='FALSE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;
  default:
    throw FdoFilterException::Create(L"c_KgOraFilterProcessor::ProcessDistanceCondition Unsupported Spatial Condition Operation");
  break;  
}
}//end of c_KgOraFilterProcessor::ProcessDistanceCondition

