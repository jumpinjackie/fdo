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

#ifndef _c_KgOraFilterProcessor_h
#define _c_KgOraFilterProcessor_h

#include "c_FilterStringBuffer.h"
#include "c_KgOraExpressionProcessor.h"



/// \brief
/// The FdoIFilterProcessor interface can be used to process the nodes in a Filter
/// tree. It declares a process operation for each concrete class in the Filter
/// hierarchy. Providers or client applications can create classes that realize
/// this interface to do something meaningful with a Filter hierarchy. For
/// example, a RDBMS feature provider can implement a processor class to convert a
/// Filter hierarchy to the SQL equivalent syntax.
class c_KgOraFilterProcessor : public virtual FdoIFilterProcessor 
{
public:
  c_KgOraFilterProcessor(c_KgOraSchemaDesc *KgOraSchemaDesc,FdoIdentifier* ClassId,const c_KgOraSridDesc& OraSrid);
public:
  ~c_KgOraFilterProcessor(void);
  virtual void Dispose() { delete this; }  

protected:
  FdoPtr<c_KgOraSchemaDesc> m_KgOraSchemaDesc;
  FdoPtr<FdoIdentifier> m_ClassId;  
  
  FdoPtr<FdoKgOraClassDefinition> m_ClassDef;  
  c_KgOraSridDesc m_OraSridDesc;
public:
    /// \brief
    /// Processes the FdoBinaryLogicalOperator passed in as an argument.
    /// 
    /// \param Filter 
    /// Input the FdoBinaryLogicalOperator
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& Filter);

    /// \brief
    /// Processes the FdoUnaryLogicalOperator passed in as an argument.
    /// 
    /// \param Filter 
    /// Input the FdoUnaryLogicalOperator
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& Filter);

    /// \brief
    /// Processes the FdoComparisonCondition passed in as an argument.
    /// 
    /// \param Filter 
    /// Input the FdoComparisonCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessComparisonCondition(FdoComparisonCondition& Filter);

    /// \brief
    /// Processes the FdoInCondition passed in as an argument.
    /// 
    /// \param Filter 
    /// Input the FdoInCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessInCondition(FdoInCondition& Filter);

    /// \brief
    /// Processes the FdoNullCondition passed in as an argument.
    /// 
    /// \param Filter 
    /// Input the FdoNullCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessNullCondition(FdoNullCondition& Filter);

    /// \brief
    /// Processes the FdoSpatialCondition passed in as an argument.
    /// 
    /// \param Filter 
    /// Input the FdoSpatialCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessSpatialCondition(FdoSpatialCondition& Filter);

    /// \brief
    /// Processes the FdoDistanceCondition passed in as an argument.
    /// 
    /// \param Filter 
    /// Input the FdoDistanceCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessDistanceCondition(FdoDistanceCondition& Filter);  
    
    
public:
  const char* GetFilterText() { return m_StringBuff.GetString(); };   
  
  c_KgOraExpressionProcessor& GetExpressionProcessor() { return m_ExpressionProcessor; };   
    
protected:
    c_KgOraExpressionProcessor m_ExpressionProcessor;
    c_FilterStringBuffer m_StringBuff;
    
    void ProcessFilter(FdoFilter* Filter); 
    void ProcessExpresion( FdoExpression* Expr,bool IsSpatialCondition=false,bool IsEnvelopeIntersect=false);
    
    void AppendString(const char *Str);

    void PrependString(const char *Str);
	  
};

#endif
