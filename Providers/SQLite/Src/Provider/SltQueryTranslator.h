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

enum StlSpatialTypeOperation
{
    StlSpatialTypeOperation_None,
    StlSpatialTypeOperation_And,
    StlSpatialTypeOperation_Or
};

class FilterChunk
{
protected:
    StringBuffer m_content;
    StlSpatialTypeOperation m_operation;
public:
    DBounds      m_bounds;
    bool         m_canOmit;
    recno_list* m_ids;
public:
    FilterChunk(StlSpatialTypeOperation op = StlSpatialTypeOperation_None)
        : m_ids(NULL), m_operation(op), m_canOmit(false)
    {
        
    }
    FilterChunk(const char* str, size_t len, StlSpatialTypeOperation op = StlSpatialTypeOperation_None)
        : m_ids(NULL), m_operation(op), m_canOmit(false)
    {
        m_content.Append(str, len);
    }
    FilterChunk(const wchar_t* str, StlSpatialTypeOperation op = StlSpatialTypeOperation_None)
        : m_ids(NULL), m_operation(op), m_canOmit(false)
    {
        m_content.Append(str);
    }
    virtual ~FilterChunk()
    {
        delete m_ids;
        m_ids = NULL;
    }
    StlSpatialTypeOperation GetOperation()
    {
        return m_operation;
    }
    void SetOperation(StlSpatialTypeOperation op)
    {
        m_operation = op;
    }
    virtual void SetString(const wchar_t* str)
    {
        m_content.Reset();
        m_content.Append(str);
    }
    virtual void SetString(const char* str)
    {
        m_content.Reset();
        m_content.Append(str);
    }
    virtual const char* ToString()
    {
        return m_content.Data();
    }
};

typedef std::vector<FilterChunk*> FilterChunkList;

class ComplexFilterChunk : public FilterChunk
{
private:
    FilterChunkList m_list;
public:
    ComplexFilterChunk(StlSpatialTypeOperation op = StlSpatialTypeOperation_None) : FilterChunk(op)
    {
    }
    virtual ~ComplexFilterChunk()
    {
        // m_list will be release by the caller
    }
    void AddToList(FilterChunk* val)
    {
        m_list.push_back(val);
    }
    void ReplaceContent(FilterChunk* val)
    {
        m_list.clear();
        m_list.push_back(val);
    }
    virtual const char* ToString()
    {
        m_content.Reset();
        for (FilterChunkList::iterator idx = m_list.begin(); idx < m_list.end(); idx++)
        {
            m_content.Append((*idx)->ToString());
        }
        return m_content.Data();
    }
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
    const char* GetFilter();
    bool CanUseFastStepping();
    recno_list* DetachIDList();
    void Reset();
    bool MustKeepFilterAlive() {return m_keepFilterAlive;}

private:
    FilterChunk* CreateFilterChunk(const char* str, size_t len, StlSpatialTypeOperation op = StlSpatialTypeOperation_None);
    FilterChunk* CreateFilterChunk(const wchar_t* str, StlSpatialTypeOperation op = StlSpatialTypeOperation_None);
    ComplexFilterChunk* CreateComplexFilterChunk(StlSpatialTypeOperation op = StlSpatialTypeOperation_None);

private:
    
    FilterChunkList             m_evalStack;
    // this chunk needs to be restore in case we find out we cannot use fast stepping
    ComplexFilterChunk*         m_fastSteppingChunk;
    // this restoring value will be set for m_fastSteppingChunk
    FilterChunk*                m_restoreValue;
    // resulting bounding box in case we can reduce the query area
    DBounds                     m_bounds;
    bool                        m_canUseFastStepping;
    FdoClassDefinition*         m_fc;
    bool                        m_keepFilterAlive;

    // list used to destroy the allocated objects at the end
    FilterChunkList             m_allocatedObjects;
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
        m_extname = L"";
        m_countname = L"TheCount";
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
