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
