/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifndef ARCSDEFILTERTOSQL_H
#define ARCSDEFILTERTOSQL_H
#ifdef _WIN32
#pragma once
#endif // _WIN32


// Described the type of simple (or compound) filters supported by ArcSDE Provider:
typedef enum ArcSDEFilterTypeEnum
{
    ArcSDEFilterType_Attribute,
    ArcSDEFilterType_Spatial,
    ArcSDEFilterType_MixedSupported,
    ArcSDEFilterType_MixedUnsupported
};


class ArcSDEFilterToSql :
    public FdoIExpressionProcessor, public FdoIFilterProcessor
{
protected:
    wchar_t *mSql;
    std::vector<SE_FILTER> mSpatialFilters;
    FdoPtr<ArcSDEConnection> m_Connection;
    FdoPtr<FdoClassDefinition> mDefinition;
    bool mFilterAnalyzed;
    bool mUseNesting;

public:
    ArcSDEFilterToSql() { ArcSDEFilterToSql(NULL, NULL); }  // 0-argument constructor to please FdoPtr::operator->
    ArcSDEFilterToSql (ArcSDEConnection *conn, FdoClassDefinition* definition);
    virtual ~ArcSDEFilterToSql (void);
    void Dispose ();

    // The function analyzes the filter and sets flag used during the generation of
    // the corresponding SQL statement to avoid unnecessary nesting.
    void AnalyzeFilter (FdoFilter *filter);

    // Helper function to set a flag.
    void SetFilterAnalyzedFlag (bool value);

    /// <summary>Increase the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns> 
    FdoInt32 AddRef()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which AddRef() method to call.
        //  Calling BOTH AddRef() methods leads to instances of this class being prematurely released.
        return FdoIExpressionProcessor::AddRef();
    }

    /// <summary>Decrease the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns> 
    FdoInt32 Release ()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
        //  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        //BAD:return (FdoIExpressionProcessor::Release () + FdoIFilterProcessor::Release ()  );
        return (FdoIExpressionProcessor::Release());
    }

    void HandleExpr (FdoExpression *exp)
    {
        exp->Process (this);
    }

    void HandleFilter (FdoFilter *filter)
    {
        // Validate that this filter can be handled by ArcSDE PRovider:
        ArcSDEFilterTypeEnum filterType = GetFilterType(filter);
        if (filterType == ArcSDEFilterType_MixedUnsupported)
            throw FdoFilterException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_FILTER_COMBINATION,"The filter provided cannot be handled by ArSDE Provider.  Spatial filters can only be applied in specific filter combinations."));

        // Process this filter:
        filter->Process (this);
    }

    void AppendString (const wchar_t* value);
    wchar_t* GetSql ();
    SHORT GetSpatialFilters(SE_FILTER *&pSpatialFilters);

	/// <summary>Checks FdoFunction passed in as an argument.</summary>
	/// <param name="expr">Input expresion</param> 
	/// <returns>Returns the SDE function id or -1 if not found </returns> 
	int GetSDEFunctionIndex (FdoFunction& expr);

	/// <summary>Checks the expression passed in as an argument for validity wrt the natively supported functions</summary>
	/// <param name="expr">Input expresion</param> 
	/// <returns>Nothing. Throws an exception if the expression is not valid</returns> 
	void IsSDEValidExpression (FdoExpression* expr);

	/// <summary>Validate the computed identifiers wrt the natively supported functions</summary>
	/// <returns> True if all expresions are supported, false otherwise. In addition, 2 output flags. </returns>
	bool ArcSDEFilterToSql::ContainsSDEValidExpressionsOnly(FdoFilter *filter, FdoIdentifierCollection* selectIds, bool& filterValid, bool& selectListValid);

    //
    // FdoIExpressionProcessor
    //

    /// <summary>Processes the FdoBinaryExpression passed in as an argument.</summary>
    /// <param name="expr">Input binary expression</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessBinaryExpression (FdoBinaryExpression& expr);

    ///<summary>Processes the FdoUnaryExpression passed in as an argument.</summary>
    /// <param name="expr">Input unary expression</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessUnaryExpression (FdoUnaryExpression& expr);

    /// <summary>Processes the FdoFunction passed in as an argument.</summary>
    /// <param name="expr">Input function</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessFunction (FdoFunction& expr);

    /// <summary>Processes the FdoIdentifier passed in as an argument.</summary>
    /// <param name="expr">Input indentifier</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessIdentifier (FdoIdentifier& expr);

	/// <summary>Processes the FdoComputedIdentifier passed in as an argument.</summary>
	/// <param name="expr">Input computed indentifier</param> 
	/// <returns>Returns nothing</returns>
	virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr) { return; }

    /// <summary>Processes the FdoParameter passed in as an argument.</summary>
    /// <param name="expr">Input parameter</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessParameter (FdoParameter& expr);

    /// <summary>Processes the FdoBooleanValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessBooleanValue (FdoBooleanValue& expr);

    /// <summary>Processes the FdoByteValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessByteValue (FdoByteValue& expr);

    /// <summary>Processes the FdoDateTimeValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessDateTimeValue (FdoDateTimeValue& expr);

    /// <summary>Processes the FdoDecimalValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessDecimalValue (FdoDecimalValue& expr);

    /// <summary>Processes the FdoDoubleValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessDoubleValue (FdoDoubleValue& expr);

    /// <summary>Processes the FdoInt16Value passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessInt16Value (FdoInt16Value& expr);

    /// <summary>Processes the FdoInt32Value passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessInt32Value (FdoInt32Value& expr);

    /// <summary>Processes the FdoInt64Value passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessInt64Value (FdoInt64Value& expr);

    /// <summary>Processes the FdoSingleValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessSingleValue (FdoSingleValue& expr);

    /// <summary>Processes the FdoStringValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessStringValue (FdoStringValue& expr);

    /// <summary>Processes the FdoBLOBValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessBLOBValue (FdoBLOBValue& expr);

    /// <summary>Processes the FdoCLOBValue passed in as an argument.</summary>
    /// <param name="expr">Input data value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessCLOBValue (FdoCLOBValue& expr);

    /// <summary>Processes the FdoGeometryValue passed in as an argument.</summary>
    /// <param name="expr">Input geometry value</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessGeometryValue (FdoGeometryValue& expr);

    //
    // FdoFilterProcessor
    //

    ///<summary>Processes the FdoBinaryLogicalOperator passed in as an argument.</summary>
    /// <param name="filter">Input the FdoBinaryLogicalOperator</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessBinaryLogicalOperator (FdoBinaryLogicalOperator& filter);

    ///<summary>Processes the FdoUnaryLogicalOperator passed in as an argument.</summary>
    /// <param name="filter">Input the FdoUnaryLogicalOperator</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter);

    ///<summary>Processes the FdoComparisonCondition passed in as an argument.</summary>
    /// <param name="filter">Input the FdoComparisonCondition</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessComparisonCondition (FdoComparisonCondition& filter);

    ///<summary>Processes the FdoInCondition passed in as an argument.</summary>
    /// <param name="filter">Input the FdoInCondition</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessInCondition (FdoInCondition& filter);

    ///<summary>Processes the FdoNullCondition passed in as an argument.</summary>
    /// <param name="filter">Input the FdoNullCondition</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessNullCondition (FdoNullCondition& filter);

    ///<summary>Processes the FdoSpatialCondition passed in as an argument.</summary>
    /// <param name="filter">Input the FdoSpatialCondition</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessSpatialCondition (FdoSpatialCondition& filter);

    ///<summary>Processes the FdoDistanceCondition passed in as an argument.</summary>
    /// <param name="filter">Input the FdoDistanceCondition</param> 
    /// <returns>Returns nothing</returns> 
    void ProcessDistanceCondition (FdoDistanceCondition& filter);

protected:
    void ProcessGeometricCondition (FdoIdentifier* fdoPropertyName, FdoExpression *fdoGeometryExpr, FdoSpatialOperations fdoSpatialOperation, double bBufferDistance = 0.0);

    // Determine the type of filter that's given, and whether or not it is supported by ArcSDE Provider
    ArcSDEFilterTypeEnum GetFilterType(FdoFilter *filter);
};

#endif // ARCSDEFILTERTOSQL_H

