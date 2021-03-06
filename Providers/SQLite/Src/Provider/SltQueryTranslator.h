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
#include "SltGeomUtils.h"

typedef std::vector<__int64> recno_list;

enum StlFilterType
{
    StlFilterType_None,
    StlFilterType_List,
    StlFilterType_Spatial
};

class IFilterChunk
{
protected:
    StringBuffer* m_content;
public:
    bool         m_canOmit;
    IFilterChunk() : m_canOmit(false)
    {
        m_content = new StringBuffer(20);
    }
    IFilterChunk(const char* str, size_t len) : m_canOmit(false)
    {
        m_content = new StringBuffer(len);
        m_content->Append(str, len);
    }
    IFilterChunk(const wchar_t* str) : m_canOmit(false)
    {
        m_content = new StringBuffer(20);
        m_content->Append(str);
    }
    virtual ~IFilterChunk()
    {
        delete m_content;
        m_content = NULL;
    }
    virtual inline recno_list* GetList()
    {
        return NULL;
    }
    virtual inline DBounds* GetBounds()
    {
        return NULL;
    }
    virtual inline recno_list* DetachIDList()
    {
        return NULL;
    }
    virtual inline StlFilterType GetType()
    {
        return StlFilterType_None;
    }
    virtual void ResetType()
    {}
    virtual void SetString(const wchar_t* str)
    {
        m_content->Reset();
        m_content->Append(str);
    }
    virtual void SetString(const char* str)
    {
        m_content->Reset();
        m_content->Append(str);
    }
    virtual const char* ToString()
    {
        return m_content->Data();
    }
};

class FilterChunk : public IFilterChunk
{
protected:
    StlFilterType m_type;
public:
    union
    {
        DBounds*    m_bounds;
        recno_list* m_ids;
    };
public:
    FilterChunk(StlFilterType type = StlFilterType_None)
        : m_ids(NULL), m_type(type)
    {
    }
    FilterChunk(const char* str, size_t len, StlFilterType type = StlFilterType_None)
        : IFilterChunk(str, len), m_ids(NULL), m_type(type)
    {
    }
    FilterChunk(const wchar_t* str, StlFilterType type = StlFilterType_None)
        : IFilterChunk(str), m_ids(NULL), m_type(type)
    {
    }
    virtual ~FilterChunk()
    {
        if (m_type == StlFilterType_Spatial)
            delete m_bounds;
        else
            delete m_ids;
        m_ids = NULL;
    }
    virtual inline recno_list* GetList()
    {
        return (m_type == StlFilterType_List) ? m_ids : NULL;
    }
    virtual inline DBounds* GetBounds()
    {
        return (m_type == StlFilterType_Spatial) ? m_bounds : NULL;
    }
    virtual inline recno_list* DetachIDList()
    {
        recno_list* lst = NULL;
        if(m_type == StlFilterType_List)
        {
            lst = m_ids;
            m_ids = NULL;
            m_type = StlFilterType_None;
        }
        return lst;
    }
    virtual inline StlFilterType GetType()
    {
        return m_type;
    }
    virtual void ResetType()
    {
        if (m_type == StlFilterType_Spatial)
            delete m_bounds;
        else
            delete m_ids;
        m_ids = NULL;
        m_type = StlFilterType_None;
    }
};

typedef std::vector<IFilterChunk*> FilterChunkList;

enum StlConvReqOperationType
{
    StlConvReqOperationType_Default,
    StlConvReqOperationType_String
};
typedef std::vector<StlConvReqOperationType> ConvReqOperationStack;

class ComplexFilterChunk : public FilterChunk
{
private:
    FilterChunkList m_list;
public:
    ComplexFilterChunk(StlFilterType type = StlFilterType_None) : FilterChunk(type)
    {
    }
    virtual ~ComplexFilterChunk()
    {
        // m_list will be release by the caller
    }
    void AddToList(IFilterChunk* val)
    {
        m_list.push_back(val);
    }
    void ReplaceContent(IFilterChunk* val)
    {
        m_list.clear();
        m_list.push_back(val);
    }
    virtual const char* ToString()
    {
        m_content->Reset();
        for (FilterChunkList::iterator idx = m_list.begin(); idx < m_list.end(); idx++)
        {
            m_content->Append((*idx)->ToString());
        }
        return m_content->Data();
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

    SltQueryTranslator(FdoClassDefinition* fc, bool validateProps = true);
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
    virtual void ProcessSubSelectExpression     (FdoSubSelectExpression& expr);

public:
    const char* GetFilter();
    bool CanUseFastStepping();
    bool MustKeepFilterAlive();

private:
    IFilterChunk* CreateBaseFilterChunk(const char* str, size_t len);
    IFilterChunk* CreateBaseFilterChunk(const wchar_t* str);
    FilterChunk* CreateFilterChunk(const char* str, size_t len, StlFilterType type = StlFilterType_None);
    FilterChunk* CreateFilterChunk(const wchar_t* str, StlFilterType type = StlFilterType_None);
    ComplexFilterChunk* CreateComplexFilterChunk(StlFilterType type = StlFilterType_None);

private:
    
    FilterChunkList             m_evalStack;
    FdoClassDefinition*         m_fc;
    // list used to destroy the allocated objects at the end
    FilterChunkList             m_allocatedObjects;
    // used as cache string operations
    StringBuffer                m_sb;
    // used to convert float/doubles on demand
    ConvReqOperationStack       m_convReqStack;
    int                         m_strgeomOperations;
    bool                        m_mustKeepFilterAlive;
    bool                        m_canUseFastStepping;
    bool                        m_foundEnvInt;
    bool                        m_validateProps;
};

//Translates an FDO Expression to a SQLite expression
//This class is used to process FdoComputedIdentifiers 
// this class will generate A+B as C
class SltExpressionTranslator : public FdoIExpressionProcessor
{

public:

    SltExpressionTranslator(FdoIdentifierCollection* props = NULL, FdoClassDefinition* classDef = NULL, bool avoidExp = false)
    {
        m_props = FDO_SAFE_ADDREF(props);
        m_fc = FDO_SAFE_ADDREF(classDef);
        m_avoidExp = avoidExp || props == NULL || (props->GetCount() == 0);
    }
    virtual ~SltExpressionTranslator()
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
    virtual void ProcessSubSelectExpression     (FdoSubSelectExpression& expr);

public:
    StringBuffer* GetExpression() { return &m_expr; }
    void Reset() { m_expr.Reset(); }

protected:
    ConvReqOperationStack           m_convReqStack;
    FdoPtr<FdoClassDefinition>      m_fc;
    FdoPtr<FdoIdentifierCollection> m_props;
    StringBuffer m_expr;
    bool m_avoidExp;
    char m_useConv[256];
};

//Translates an FDO Expression to a SQLite expression
//This class is used to process FdoComputedIdentifiers but no for select list
// this class will generate A+B
class SltExtractExpressionTranslator : public SltExpressionTranslator
{

public:

    SltExtractExpressionTranslator(FdoIdentifierCollection* props = NULL, bool avoidExp = false)
        : SltExpressionTranslator(props, NULL, avoidExp)
    {
    }
    virtual ~SltExtractExpressionTranslator()
    {}
	virtual void ProcessComputedIdentifier      (FdoComputedIdentifier& expr);
};

//Helper class used at spatial context and count queries
class SltScCHelperTranslator : public FdoIExpressionProcessor
{

public:

    SltScCHelperTranslator(FdoFeatureClass* fc)
    {
        m_extname = L"";
        m_countname = L"";
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
    virtual void ProcessSubSelectExpression     (FdoSubSelectExpression& expr) {m_error = true;}

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
