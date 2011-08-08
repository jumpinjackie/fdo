#ifndef FDORDBMSSQLSERVERPROCESSORS_H
#define FDORDBMSSQLSERVERPROCESSORS_H     1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
 *
 */

#define CONV_MEM_BLOCK_SIZE 128

#include "../../Fdo/Other/FdoRdbmsSqlBuilder.h"

class FdoRdbmsSqlServerConnection;

class FdoSqlServerInvalidExpDetProcessor : public virtual FdoIExpressionProcessor, public virtual FdoIFilterProcessor
{
protected:
    FdoSqlServerInvalidExpDetProcessor()
    {
        mState = 0x00; // default state
        mExpSupported = true;
    }

    void HandleExpr( FdoExpression *exp )
    {
        exp->Process( this );
    }
    void HandleFilter( FdoFilter *filter )
    {
        filter->Process( this );
    }
    // used to avoid process the whole expression (in case needed)
    // this can be used to see if there are any unsupported functions used in filters/expressions
    int mState;
    bool mExpSupported;
public:

    virtual void Dispose() { delete this; }

    void Done() { mState = 0x02; } // done state
    void StartStoppable() { mState = 0x01; mExpSupported = true; } // stoppable state

    virtual void ProcessBinaryExpression(FdoBinaryExpression& expr)
    {
        if (mState == 0x02)
            return;
        HandleExpr( FdoPtr<FdoExpression>(expr.GetLeftExpression()) );
        HandleExpr( FdoPtr<FdoExpression>(expr.GetRightExpression()) );
    }
    virtual void ProcessBooleanValue(FdoBooleanValue& expr) {  }
    virtual void ProcessByteValue(FdoByteValue& expr){   }
    virtual void ProcessDateTimeValue(FdoDateTimeValue& expr){   }
    virtual void ProcessDoubleValue(FdoDoubleValue& expr){   }
    virtual void ProcessDecimalValue(FdoDecimalValue& expr){   }
    virtual void ProcessInt16Value(FdoInt16Value& expr){  }
    virtual void ProcessInt32Value(FdoInt32Value& expr){  }
    virtual void ProcessInt64Value(FdoInt64Value& expr){  }
    virtual void ProcessSingleValue(FdoSingleValue& expr){  }
    virtual void ProcessStringValue(FdoStringValue& expr){  }
    virtual void ProcessBLOBValue(FdoBLOBValue& expr){   }
    virtual void ProcessCLOBValue(FdoCLOBValue& expr){  }
    virtual void ProcessGeometryValue(FdoGeometryValue& expr){  }
    virtual void ProcessParameter(FdoParameter& expr){  }
    virtual void ProcessUnaryExpression(FdoUnaryExpression& expr)
    {
        if (mState == 0x02)
            return;
        HandleExpr( FdoPtr<FdoExpression>(expr.GetExpression()) );
    }
    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
    {
        if (mState == 0x02)
            return;
        HandleFilter( FdoPtr<FdoFilter>(filter.GetLeftOperand()) );
        HandleFilter( FdoPtr<FdoFilter>(filter.GetRightOperand()) );
    }
    virtual void ProcessComparisonCondition(FdoComparisonCondition& filter)
    {
        if (mState == 0x02)
            return;
        HandleExpr( FdoPtr<FdoExpression>(filter.GetLeftExpression()) );
        HandleExpr( FdoPtr<FdoExpression>(filter.GetRightExpression()) );
    }
    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter){  }

    virtual void ProcessInCondition(FdoInCondition& filter)
    {
        if (mState == 0x02)
            return;
        ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
    }
    virtual void ProcessNullCondition(FdoNullCondition& filter)
    {
        if (mState == 0x02)
            return;
        ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
    }
    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter){  }

    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
    {
        if (mState == 0x02)
            return;
        HandleFilter( FdoPtr<FdoFilter>(filter.GetOperand()) );
    }

    virtual void ProcessIdentifier(FdoIdentifier& expr) { }

    virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr)
    {
        if (mState == 0x02)
            return;
        HandleExpr( FdoPtr<FdoExpression>(expr.GetExpression()) );
    }
    
    virtual void ProcessFunction(FdoFunction& expr);
    bool IsExpressionSupported() { return mExpSupported; }
};

class FdoRdbmsSqlServerSqlBuilder : public FdoRdbmsSqlBuilder
{
    friend FdoRdbmsSqlServerConnection;
    typedef std::pair< const FdoSmLpClassDefinition*, std::wstring > pair_class_alias;
    typedef std::vector< pair_class_alias > sel_class_alias;
    enum SqlGeometryType
    {
        SqlGeometryType_Geometry,
        SqlGeometryType_Geography
    };

    struct ci_less
    {
      bool operator() (FdoString* s1, FdoString* s2) const
      {
	    return _wcsicmp(s1, s2) < 0;
      }
    };

    typedef std::map<FdoString*, int, ci_less> function_map;

    enum SqlComputedEvalType
    {
        SqlComputedEvalType_Unsupported = 0, // ProcessComputedIdentifier will throw an exception: 'Unsupported FDO type in filters'
        SqlComputedEvalType_Expression = 1,  // ProcessComputedIdentifier will generate a text like: 'A+B'
        SqlComputedEvalType_Alias = 2        // ProcessComputedIdentifier will generate a text like: '(A+B) AS C'
    };

    class pair_working_stack
    {
    public:
        std::wstring first;
        SqlComputedEvalType second;
    public:
        pair_working_stack()
        {
            second = SqlComputedEvalType_Unsupported;
        }
        pair_working_stack(size_t sz)
        {
            first.reserve(sz);
            second = SqlComputedEvalType_Unsupported;
        }
    };

    typedef std::vector< pair_working_stack* > exp_working_stack;
protected:
    // we need here more parameters (connection and a list of classes with aliases)
    FdoRdbmsSqlServerSqlBuilder(FdoRdbmsSqlServerConnection* conn);
    virtual ~FdoRdbmsSqlServerSqlBuilder();

    virtual void Dispose() { delete this; }    

public:

        // in case it contains unsupported functions this method will return NULL
    virtual FdoString* ToSelectSqlString(FdoIdentifier* mainClass, FdoIdentifier* alias, FdoFilter* filter, FdoIdentifierCollection* selectList, 
        const std::vector<NameOrderingPair>& ordering, FdoJoinCriteriaCollection* joinCriteria);

    virtual FdoString* ToAggregateSelectSqlString(FdoIdentifier* mainClass, FdoIdentifier* alias, FdoFilter* filter, FdoIdentifierCollection* selectList,
                                                bool bDistinct, FdoOrderingOption eOrderingOption, FdoIdentifierCollection* ordering, 
                                                FdoFilter* grFilter, FdoIdentifierCollection* grouping, FdoParameterValueCollection* parmValues, 
                                                FdoJoinCriteriaCollection* joinCriteria)
    {
        return NULL;
    }

    //--------------------------------------------------------------------
    // FdoIFilterProcessor
    //--------------------------------------------------------------------
    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);
    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);
    virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);
    virtual void ProcessInCondition(FdoInCondition& filter);
    virtual void ProcessNullCondition(FdoNullCondition& filter);
    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);
    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);

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
    virtual FdoString* GetExpression() { return top_stack()->first.c_str(); }
    void Reset() 
    {
        pair_working_stack* itm = top_stack();
        itm->first.clear();
        itm->second = SqlComputedEvalType_Unsupported;
        m_posStack = 0;
        m_lastGtype = SqlGeometryType_Geometry;
        m_lastSrid = 0;
        m_useBr = false;
        mUsedParameterValues.clear();
        mParams = NULL;
        m_selectList.clear();
        m_hasClaculations = false;
        m_aliasStartChar = L'A';
        m_aliasStartIdxNo = 0;
    }
    void SetComputedEvalType(SqlComputedEvalType type)
    {
        top_stack()->second = type;
    }
    void SetParameterValues (FdoParameterValueCollection* params) { mParams = FDO_SAFE_ADDREF(params); }
    std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* GetUsedParameterValues() { return &mUsedParameterValues; }

private:
    // avoid calling HandleExpr(exp); after a call to push_stack()
    // without calling again push_stack() since it will write the expression
    // in wrong SQL chunk!
    pair_working_stack* push_stack()
    {
        m_posStack++;

        if (m_posStack >= m_builderStack.size())
        {
            m_builderStack.push_back(new pair_working_stack());
            return m_builderStack.back();
        }
        return m_builderStack.at(m_posStack);
    }
    inline void pop_stack()
    {
        m_builderStack.at(m_posStack)->first.clear();
        m_posStack--;
    }
    inline pair_working_stack* top_stack()
    {
        return m_builderStack.at(m_posStack);
    }

    void ExpandCalculations();
    void AssignAliases();
    FdoString* AssignTempAlias(const FdoSmLpClassDefinition* clsDef);

    // ABS(), ACOS(), ASIN(), ATAN(), COS(), EXP(), SIN(), SQRT(), TAN()
    // FLOOR(), CEIL(), UPPER(), LOWER(), LTRIM(), RTRIM(), SOUNDEX()
    void ProcessCommonFunction(FdoFunction& expr);

    // Numeric
    void ProcessRoundFunction(FdoFunction& expr);
    void ProcessSignFunction(FdoFunction& expr);
    void ProcessTruncFunction(FdoFunction& expr);

    // Math
    void ProcessAtn2Function(FdoFunction& expr); // ATN2
    void ProcessLnFunction(FdoFunction& expr);
    void ProcessLogFunction(FdoFunction& expr);
    void ProcessModFunction(FdoFunction& expr);
    void ProcessPowerFunction(FdoFunction& expr);
    void ProcessRemainderFunction(FdoFunction& expr);

    // Geometry
    void ProcessArea2dFunction(FdoFunction& expr);
    void ProcessLength2dFunction(FdoFunction& expr);
    void ProcessXFunction(FdoFunction& expr);
    void ProcessYFunction(FdoFunction& expr);
    void ProcessZMFunction(FdoFunction& expr);
    void ProcessIsValidFunction(FdoFunction& expr);

    // Date
    void ProcessAddMonthsFunction(FdoFunction& expr);
    void ProcessCurrentDateFunction(FdoFunction& expr);
    void ProcessExtractFunction(FdoFunction& expr);
    void ProcessExtractToIntFunction(FdoFunction& expr);
    void ProcessExtractToDblFunction(FdoFunction& expr);
    void ProcessMonthsBetweenFunction(FdoFunction& expr);

    // Conversion
    void ProcessNullValueFunction(FdoFunction& expr); // ISNULL
    void ProcessToDateFunction(FdoFunction& expr);
    void ProcessToDoubleFunction(FdoFunction& expr);
    void ProcessToFloatFunction(FdoFunction& expr);
    void ProcessToInt32Function(FdoFunction& expr);
    void ProcessToInt64Function(FdoFunction& expr);
    void ProcessToStringFunction(FdoFunction& expr);

    // String
    void ProcessConcatFunction(FdoFunction& expr);
    void ProcessInstrFunction(FdoFunction& expr);
    void ProcessLengthFunction(FdoFunction& expr);
    void ProcessLpadFunction(FdoFunction& expr);
    void ProcessRpadFunction(FdoFunction& expr);
    void ProcessSubStrFunction(FdoFunction& expr);
    // void ProcessTranslateFunction(FdoFunction& expr); // TRANSLATE - not supported on server side
    void ProcessTrimFunction(FdoFunction& expr);
    
    // Aggregate
    void ProcessMedianFunction(FdoFunction& expr); // - supported on server side however needs TOP 1 in select when is selected alone
    void ProcessAggregateFunction(FdoFunction& expr); // AVG(), COUNT(), MAX(), MIN(), STDEV(), SUM()
    void ProcessSpatialExtentsFunction(FdoFunction& expr);
    void LoadFunctionDefinitions();
    FdoString* FindCalculation(FdoString* name);
protected:
    exp_working_stack m_builderStack;
    FdoPtr<FdoIdentifierCollection> m_props;
    function_map m_functions;
    wchar_t m_useConv[CONV_MEM_BLOCK_SIZE];
    FdoRdbmsSqlServerConnection* m_fdoConn;
    size_t m_posStack;
    bool m_useBr;

    // keeps the last geometry type when identifier was processed
    // in case identifier was a geometry
    SqlGeometryType m_lastGtype; 
    FdoInt64        m_lastSrid;

    std::vector<std::pair<FdoLiteralValue*, FdoInt64> > mUsedParameterValues; // value and optional the SRID for geometries
    FdoPtr<FdoParameterValueCollection> mParams;
    sel_class_alias m_usedClasses;
    sel_class_alias m_tempUsedClasses;
    std::vector< std::wstring > m_selectList;
    // name, expression
    std::vector< std::pair< std::wstring, std::wstring> > m_lastSelCompIdf;
    bool m_hasClaculations;
    wchar_t m_aliasStartChar;
    int m_aliasStartIdxNo;
    FdoPtr<FdoIdentifier> m_mainClass;
};

#endif
