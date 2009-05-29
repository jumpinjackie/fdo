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

#ifndef SLTQUERYTRANSLATOR_H
#define SLTQUERYTRANSLATOR_H

#include "StringUtil.h"

typedef std::vector<__int64> recno_list;

struct TCtx
{
    TCtx() : canOmit(false), ids(NULL)
    {
    }

    std::wstring expr;
    DBounds      bounds;
    bool         canOmit;
    recno_list* ids;
};


//Translates an FDO Filter to a SQLite WHERE clause
//Extracts BBOX queries that can be accelerated by
//using the spatial index. Also determines whether the 
//query can use the fast SQLite stepping, where we 
//access data directly from B-Tree memory
class SltQueryTranslator : public FdoIFilterProcessor, 
                           public FdoIExpressionProcessor
{

public:

    SltQueryTranslator(FdoClassDefinition* fc);
    ~SltQueryTranslator();

    SLT_IMPLEMENT_REFCOUNTING
    
public:

    //--------------------------------------------------------------------
    // FdoIFilterProcessor
    //--------------------------------------------------------------------

    virtual void ProcessBinaryLogicalOperator   (FdoBinaryLogicalOperator& filter);
    virtual void ProcessUnaryLogicalOperator    (FdoUnaryLogicalOperator& filter);
    virtual void ProcessComparisonCondition     (FdoComparisonCondition& filter);
    virtual void ProcessInCondition             (FdoInCondition& filter);
    virtual void ProcessNullCondition           (FdoNullCondition& filter);
    virtual void ProcessSpatialCondition        (FdoSpatialCondition& filter);
    virtual void ProcessDistanceCondition       (FdoDistanceCondition& filter);

    //--------------------------------------------------------------------
    // FdoIExpressionProcessor
    //--------------------------------------------------------------------

    virtual void ProcessBinaryExpression        (FdoBinaryExpression& expr);
    virtual void ProcessUnaryExpression         (FdoUnaryExpression& expr);
    virtual void ProcessFunction                (FdoFunction& expr);
    virtual void ProcessIdentifier              (FdoIdentifier& expr);
	virtual void ProcessComputedIdentifier      (FdoComputedIdentifier& expr);
    virtual void ProcessParameter               (FdoParameter& expr);
    virtual void ProcessBooleanValue            (FdoBooleanValue& expr);
    virtual void ProcessByteValue               (FdoByteValue& expr);
    virtual void ProcessDateTimeValue           (FdoDateTimeValue& expr);
    virtual void ProcessDecimalValue            (FdoDecimalValue& expr);
    virtual void ProcessDoubleValue             (FdoDoubleValue& expr);
    virtual void ProcessInt16Value              (FdoInt16Value& expr);
    virtual void ProcessInt32Value              (FdoInt32Value& expr);
    virtual void ProcessInt64Value              (FdoInt64Value& expr);
    virtual void ProcessSingleValue             (FdoSingleValue& expr);
    virtual void ProcessStringValue             (FdoStringValue& expr);
    virtual void ProcessBLOBValue               (FdoBLOBValue& expr);
    virtual void ProcessCLOBValue               (FdoCLOBValue& expr);
    virtual void ProcessGeometryValue           (FdoGeometryValue& expr);

public:

    void GetBBOX(DBounds& ext);
    const wchar_t* GetFilter();
    bool CanUseFastStepping();
    recno_list* DetachIDList();

private:
    
    std::vector<TCtx>           m_evalStack; 
    bool                        m_canUseFastStepping;
    FdoClassDefinition*         m_fc;

};

//Translates an FDO Expression to a SQLite expression
//This class is used to process FdoComputedIdentifiers 
class SltExpressionTranslator : public FdoIExpressionProcessor
{

public:

    SltExpressionTranslator()
    {}
    ~SltExpressionTranslator()
    {}

    SLT_IMPLEMENT_REFCOUNTING
    
protected:
    void HandleExpr( FdoExpression *exp )
    {
        exp->Process( this );
    }

public:

    //--------------------------------------------------------------------
    // FdoIExpressionProcessor
    //--------------------------------------------------------------------

    virtual void ProcessBinaryExpression        (FdoBinaryExpression& expr);
    virtual void ProcessUnaryExpression         (FdoUnaryExpression& expr);
    virtual void ProcessFunction                (FdoFunction& expr);
    virtual void ProcessIdentifier              (FdoIdentifier& expr);
	virtual void ProcessComputedIdentifier      (FdoComputedIdentifier& expr);
    virtual void ProcessParameter               (FdoParameter& expr);
    virtual void ProcessBooleanValue            (FdoBooleanValue& expr);
    virtual void ProcessByteValue               (FdoByteValue& expr);
    virtual void ProcessDateTimeValue           (FdoDateTimeValue& expr);
    virtual void ProcessDecimalValue            (FdoDecimalValue& expr);
    virtual void ProcessDoubleValue             (FdoDoubleValue& expr);
    virtual void ProcessInt16Value              (FdoInt16Value& expr);
    virtual void ProcessInt32Value              (FdoInt32Value& expr);
    virtual void ProcessInt64Value              (FdoInt64Value& expr);
    virtual void ProcessSingleValue             (FdoSingleValue& expr);
    virtual void ProcessStringValue             (FdoStringValue& expr);
    virtual void ProcessBLOBValue               (FdoBLOBValue& expr);
    virtual void ProcessCLOBValue               (FdoCLOBValue& expr);
    virtual void ProcessGeometryValue           (FdoGeometryValue& expr);

public:
    StringBuffer* GetExpression() { return &m_expr; }
    void Reset() { m_expr.Reset(); }

private:
    
    StringBuffer m_expr;
};

//Helper class used at spatial context and count queries
class SltScCHelperTranslator : public FdoIExpressionProcessor
{

public:

    SltScCHelperTranslator(FdoFeatureClass* fc)
    {
        m_fc = fc;
        Reset();
    }
    ~SltScCHelperTranslator()
    {}

    SLT_IMPLEMENT_REFCOUNTING
    
protected:
    void HandleExpr( FdoExpression *exp )
    {
        exp->Process( this );
    }

public:

    //--------------------------------------------------------------------
    // FdoIExpressionProcessor
    //--------------------------------------------------------------------

    virtual void ProcessBinaryExpression        (FdoBinaryExpression& expr){m_error = true;}
    virtual void ProcessUnaryExpression         (FdoUnaryExpression& expr){m_error = true;}
    virtual void ProcessFunction                (FdoFunction& expr);
    virtual void ProcessIdentifier              (FdoIdentifier& expr);
	virtual void ProcessComputedIdentifier      (FdoComputedIdentifier& expr);
    virtual void ProcessParameter               (FdoParameter& expr){m_error = true;}
    virtual void ProcessBooleanValue            (FdoBooleanValue& expr){m_error = true;}
    virtual void ProcessByteValue               (FdoByteValue& expr){m_error = true;}
    virtual void ProcessDateTimeValue           (FdoDateTimeValue& expr){m_error = true;}
    virtual void ProcessDecimalValue            (FdoDecimalValue& expr){m_error = true;}
    virtual void ProcessDoubleValue             (FdoDoubleValue& expr){m_error = true;}
    virtual void ProcessInt16Value              (FdoInt16Value& expr){m_error = true;}
    virtual void ProcessInt32Value              (FdoInt32Value& expr){m_error = true;}
    virtual void ProcessInt64Value              (FdoInt64Value& expr){m_error = true;}
    virtual void ProcessSingleValue             (FdoSingleValue& expr){m_error = true;}
    virtual void ProcessStringValue             (FdoStringValue& expr){m_error = true;}
    virtual void ProcessBLOBValue               (FdoBLOBValue& expr){m_error = true;}
    virtual void ProcessCLOBValue               (FdoCLOBValue& expr){m_error = true;}
    virtual void ProcessGeometryValue           (FdoGeometryValue& expr){m_error = true;}

public:
    void Reset() 
    {
        m_extname = L"";
        m_countname = L"TheCount";
        m_error = false;
    }
    bool IsError(){return m_error;}
    const wchar_t* GetSContextName(){return m_extname.c_str();}
    const wchar_t* GetCountName(){return m_countname.c_str();}

private:
    std::vector<FdoString*> m_stackNames; 
    std::wstring m_extname;
    std::wstring m_countname;
    bool m_error;
    FdoFeatureClass* m_fc;
};

#endif
