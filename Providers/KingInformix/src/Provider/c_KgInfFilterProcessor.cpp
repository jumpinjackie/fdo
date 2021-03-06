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
#include "c_KgInfFilterProcessor.h"


#define D_FILTER_OPEN_PARENTH " ( "
#define D_FILTER_CLOSE_PARENTH " ) "

#define D_FILTER_COMMA " , "
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





c_KgInfFilterProcessor::c_KgInfFilterProcessor(c_KgInfSchemaDesc *KgOraSchemaDesc,FdoIdentifier* ClassId
      ,const c_KgInfSridDesc& OraSridDesc,int ParamSqlOffset)
  : m_ExpressionProcessor( &m_StringBuff,KgOraSchemaDesc,ClassId,OraSridDesc,ParamSqlOffset ) // they will share same string buffer

{
  m_KgOraSchemaDesc = KgOraSchemaDesc;
  FDO_SAFE_ADDREF(m_KgOraSchemaDesc.p);
  
  m_ClassId = ClassId;
  FDO_SAFE_ADDREF(m_ClassId.p);
  
  m_OraSridDesc = OraSridDesc;
  
}

c_KgInfFilterProcessor::~c_KgInfFilterProcessor(void)
{
  
}

void c_KgInfFilterProcessor::ProcessFilter(FdoFilter* Filter)
{
  Filter->Process(this);
}//end of c_KgInfFilterProcessor::ProcessFilter

void c_KgInfFilterProcessor::ProcessExpresion( FdoExpression* Expr,bool IsSpatialCondition,bool IsEnvelopeIntersect)
{  
  Expr->Process( &m_ExpressionProcessor );   
}

void c_KgInfFilterProcessor::ProcessExpresionGeomFilter( FdoExpression* Expr,bool IsSpatialCondition,bool IsEnvelopeIntersect)
{  
  c_FilterStringBuffer* oldbuff = m_ExpressionProcessor.GetStringBuff();
  
  //m_ExpressionProcessor.SetStringBuff(&m_StringGeomFilter);
  Expr->Process( &m_ExpressionProcessor );   
  //m_ExpressionProcessor.SetStringBuff(oldbuff);
}

//
// Add a string to the end of the buffer
void c_KgInfFilterProcessor::AppendString(const char *Str)
{
  m_StringBuff.AppendString(Str);
}

//
// Add a string to the biginning of the buffer
void c_KgInfFilterProcessor::PrependString(const char *Str)
{
  m_StringBuff.PrependString(Str);
}

//
// Add a string to the end of the buffer
void c_KgInfFilterProcessor::AppendStringGeomFilter(const char *Str)
{
  //m_StringGeomFilter.AppendString(Str);
  m_StringBuff.AppendString(Str);
}

//
// Add a string to the biginning of the buffer
void c_KgInfFilterProcessor::PrependStringGeomFilter(const char *Str)
{
  //m_StringGeomFilter.PrependString(Str);
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
void c_KgInfFilterProcessor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& Filter)
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
  
}//end of c_KgInfFilterProcessor::ProcessBinaryLogicalOperator

void c_KgInfFilterProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& Filter)
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
}//end of c_KgInfFilterProcessor::ProcessUnaryLogicalOperator

void c_KgInfFilterProcessor::ProcessComparisonCondition(FdoComparisonCondition& Filter)
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
}//end of c_KgInfFilterProcessor::ProcessComparisonCondition


void c_KgInfFilterProcessor::ProcessInCondition(FdoInCondition& Filter)
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
}//end of c_KgInfFilterProcessor::ProcessInCondition

void c_KgInfFilterProcessor::ProcessNullCondition(FdoNullCondition& Filter)
{
    FdoPtr<FdoIdentifier>id = Filter.GetPropertyName();
    if( id == NULL )
        throw FdoFilterException::Create(L"FdoNullCondition is missing the property name");

    AppendString(D_FILTER_OPEN_PARENTH);
    ProcessExpresion( id );
    AppendString( D_FILTER_IS_NULL );
    AppendString(D_FILTER_CLOSE_PARENTH);
    
}//end of c_KgInfFilterProcessor::ProcessNullCondition


void c_KgInfFilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& Filter)
{
FdoPtr<FdoIdentifier> geomprop = Filter.GetPropertyName();

FdoPtr<FdoExpression> geomexp = Filter.GetGeometry();

switch( Filter.GetOperation() )
{
  case FdoSpatialOperations_EnvelopeIntersects:
  {
    // sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
    
    //FROM ST.FilterQuery('GEOTABLE', 'GEOCOLUMN', ST.MakeEnvelope(1000,1000,2000,2000,-1)) AS q
    //INNER JOIN GEOTABLE AS t ON q.oid = t.oid


    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {   
        //simon 
        AppendStringGeomFilter("SE_EnvelopesIntersect(('");
        //AppendStringGeomFilter("ST_Intersects(");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }    
  }
  break;
  case FdoSpatialOperations_Intersects:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Intersects(");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }    

  }
  break;
  
  
  /// Test to see if the geometric property value spatially contains the
  /// literal geometric value.
  case FdoSpatialOperations_Contains:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Contains(");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }    
  }
  break;

  /// Test to see if the geometric property value spatially crosses the given
  /// geometry.
  case FdoSpatialOperations_Crosses:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Crosses(");
        AppendStringGeomFilter(FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }    
  }
  break;

  case FdoSpatialOperations_Disjoint:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Disjoint(");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }   
  }
  break;
  /// Test to see if the geometric property value spatially overlaps the given
  /// geometry
  case FdoSpatialOperations_Overlaps:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Overlaps(");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }   
  }
  break;

    /// Test to see if the geometric property value spatially touches the given
    /// geometry.
  case FdoSpatialOperations_Touches:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Touches(");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }  
  }
  break;

    /// Test to see if the geometric property value is spatially within the
    /// given geometry.
  case FdoSpatialOperations_Within:
  {
     if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Within(");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(",");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(")");
      }
    }  
  }
  break;

    /// Test to see if the geometric property value is covered by the interior
    /// and boundary of the given geometry.
  case FdoSpatialOperations_CoveredBy:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Covers(");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(",");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter("')");
      }
    }  
  }
  break;

  /// Test to see if the geometric property value is inside the interior
  /// of the given geometry, not touching the boundary.
  case FdoSpatialOperations_Inside:
  {
    if( m_KgOraSchemaDesc.p && m_ClassId.p )
    {
      FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
      FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
      
      if( phys_class )
      {      
        AppendStringGeomFilter("ST_Contains(");
        ProcessExpresionGeomFilter( geomexp,true );
        AppendStringGeomFilter(",");
        AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
        AppendStringGeomFilter(")");
      }
    }
  }
  break;

  

    /// Test to see if the geometric property value is spatially equal to the
    /// given geometry.
    case FdoSpatialOperations_Equals:
    {
      if( m_KgOraSchemaDesc.p && m_ClassId.p )
      {
        FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = m_KgOraSchemaDesc->GetPhysicalSchemaMapping();
        FdoPtr<c_KgInfOverClassDefinition> phys_class = phschemamapping->FindByClassName( m_ClassId->GetName() );
        
        if( phys_class )
        {      
          AppendStringGeomFilter("ST_Equals(");
          AppendStringGeomFilter( FdoStringP(geomprop->GetName()) );
          AppendStringGeomFilter(",");
          ProcessExpresionGeomFilter( geomexp,true );
          AppendStringGeomFilter(")");
        }
      }
    }
    break;
    
    
  default:
    throw FdoFilterException::Create(L"c_KgInfFilterProcessor::ProcessSpatialCondition Unsupported Spatial Condition Operation");
  break;  
}
}//end of c_KgInfFilterProcessor::ProcessSpatialCondition

void c_KgInfFilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& Filter)
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
    throw FdoFilterException::Create(L"c_KgInfFilterProcessor::ProcessDistanceCondition Unsupported Spatial Condition Operation");
  break;  
}
}//end of c_KgInfFilterProcessor::ProcessDistanceCondition

