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

#ifndef _c_KgOraExpressionProcessor_h
#define _c_KgOraExpressionProcessor_h

#include "c_FilterStringBuffer.h"
#include "c_KgOraSqlParamDesc.h"


/// \brief
/// The FdoIExpressionProcessor interface can be used to process the nodes in an
/// Expression tree. It declares an process operation for each concrete class in
/// the FdoExpression hierarchy. Providers or client applications can create
/// classes that realize this interface to do something meaningful with an
/// FdoExpression hierarchy. For example, an RDBMS feature provider implements a
/// processor class to convert an FdoExpression hierarchy to the SQL equivalent
/// syntax.
class c_KgOraExpressionProcessor : public virtual FdoIExpressionProcessor
{
public:
  c_KgOraExpressionProcessor(c_FilterStringBuffer* StringBuff,c_KgOraSchemaDesc *KgOraSchemaDesc,FdoIdentifier* ClassDef,const c_KgOraSridDesc& OraSrid,int ParamNumberOffset=0);
public:
  ~c_KgOraExpressionProcessor(void);
  virtual void Dispose() { delete this; }
  
protected:
  FdoPtr<c_KgOraSchemaDesc> m_KgOraSchemaDesc;
  FdoPtr<FdoIdentifier> m_ClassId;    
  c_FilterStringBuffer* m_StringBuff; // pointer to buffer set in constructor; so it can share buffer with expresion processor
  c_KgOraSridDesc m_OraSridDesc; // when converting geomerty need to have this one
  
  FdoStringP m_ConstantSpatialExtent;
  
  int m_ParamNumberOffset;
  bool m_DoAsParameters;
  
  int m_ParamCount; 
  
  std::vector<c_KgOraSqlParamDesc*> m_ParamList; // how many parameters was created inside string ( geometry + parameters expressions)
  
  public:

    /// \brief
    /// Processes the FdoBinaryExpression passed in as an argument.
    /// 
    /// \param expr 
    /// Input binary expression
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

    /// \brief
    /// Processes the FdoUnaryExpression passed in as an argument.
    /// 
    /// \param expr 
    /// Input unary expression
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    /// \brief
    /// Processes the FdoFunction passed in as an argument.
    /// 
    /// \param expr 
    /// Input function
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessFunction(FdoFunction& expr);

    /// \brief
    /// Processes the FdoIdentifier passed in as an argument.
    /// 
    /// \param expr 
    /// Input indentifier
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessIdentifier(FdoIdentifier& expr);

    /// \brief
    /// Processes the FdoComputedIdentifier passed in as an argument.
    /// 
    /// \param expr 
    /// Input indentifier
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

    /// \brief
    /// Processes the FdoParameter passed in as an argument.
    /// 
    /// \param expr 
    /// Input parameter
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessParameter(FdoParameter& expr);

    /// \brief
    /// Processes the FdoBooleanValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessBooleanValue(FdoBooleanValue& expr);

    /// \brief
    /// Processes the FdoByteValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessByteValue(FdoByteValue& expr);

    /// \brief
    /// Processes the FdoDateTimeValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    /// \brief
    /// Processes the FdoDecimalValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessDecimalValue(FdoDecimalValue& expr);

    /// \brief
    /// Processes the FdoDoubleValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessDoubleValue(FdoDoubleValue& expr);

    /// \brief
    /// Processes the FdoInt16Value passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessInt16Value(FdoInt16Value& expr);

    /// \brief
    /// Processes the FdoInt32Value passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessInt32Value(FdoInt32Value& expr);

    /// \brief
    /// Processes the FdoInt64Value passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessInt64Value(FdoInt64Value& expr);

    /// \brief
    /// Processes the FdoSingleValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessSingleValue(FdoSingleValue& expr);

    /// \brief
    /// Processes the FdoStringValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessStringValue(FdoStringValue& expr);

    /// \brief
    /// Processes the FdoBLOBValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessBLOBValue(FdoBLOBValue& expr);

    /// \brief
    /// Processes the FdoCLOBValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    /// \brief
    /// Processes the FdoGeometryValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input geometry value
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void ProcessGeometryValue(FdoGeometryValue& expr);
    
    virtual void ProcessGeometryValueRect(FdoGeometryValue& expr);
  
public:
  //const std::vector<c_KgOraSqlParamDesc*> GetParamList() const { return m_ParamList; }
  void SetConstantSpatialExtent(const wchar_t* ConstantSpatialExtent);
    
  void SetParamNumberOffset(int ParamNumOffset) { m_ParamNumberOffset=ParamNumOffset; }
  void ApplySqlParameters(c_Oci_Statement* OciStm,bool IsGeodeticCS,long OraSrid,int ParamOffest=0);
  int GetSqlParametersCount();
  
protected:
  
  void AddAsParameter(FdoDataValue& Value);
  void ProcessExpresion( FdoExpression* Expr);
  void AppendString(const wchar_t *Str);
  void PrependString(const wchar_t *Str);
};

#endif