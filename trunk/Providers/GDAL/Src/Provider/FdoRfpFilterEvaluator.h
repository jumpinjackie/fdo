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
 *
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpFilterEvaluator.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPFILTEREVALUATOR_H
#define FDORFPFILTEREVALUATOR_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <vector>

typedef FdoPtr<FdoFilter> FdoFilterP;
typedef FdoPtr<FdoExpression> FdoExpressionP;
typedef FdoPtr<FdoIdentifier> FdoIdentifierP;

class FdoRfpGeoRaster;
class FdoRfpVariant;
class FdoRfpVariantCollection;

class FdoRfpFilterEvaluator : virtual public FdoIFilterProcessor, virtual public FdoIExpressionProcessor
{
private:
	FdoClassDefinitionP				m_classDef;
	FdoPtr<FdoRfpGeoRaster>		    m_geoRaster;
	FdoFilterP						m_filter;
	FdoPtr<FdoRfpVariantCollection>	m_resultStack;
	FdoPtr<FdoFgfGeometryFactory>	m_agfFactory;

protected:
	FdoRfpFilterEvaluator(void);
	virtual ~FdoRfpFilterEvaluator(void);
	void Dispose() { delete this; }
private:
	//
	//The following are internally used helper funtions
	//
	void _handleExpression(const FdoExpressionP& expr);
	void _handleFilter(const FdoFilterP& filter);
	void _pushResult();
	FdoPtr<FdoRfpVariant> _popResult();
	FdoPtr<FdoRfpVariant> _getResult();
	void _throwInvalidException();
	FdoBoolean _isIdentifierValid(const FdoIdentifierP& identifier);
	FdoBoolean _isIdIdentifierValid(const FdoIdentifierP& identifier);

public:
	///<summary>Processes the FdoBinaryLogicalOperator passed in as an argument.</summary>
	/// <param name="filter">Input the FdoBinaryLogicalOperator</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

	///<summary>Processes the FdoUnaryLogicalOperator passed in as an argument.</summary>
	/// <param name="filter">Input the FdoUnaryLogicalOperator</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

	///<summary>Processes the FdoComparisonCondition passed in as an argument.</summary>
	/// <param name="filter">Input the FdoComparisonCondition</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

	///<summary>Processes the FdoInCondition passed in as an argument.</summary>
	/// <param name="filter">Input the FdoInCondition</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessInCondition(FdoInCondition& filter);

	///<summary>Processes the FdoNullCondition passed in as an argument.</summary>
	/// <param name="filter">Input the FdoNullCondition</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessNullCondition(FdoNullCondition& filter);

	///<summary>Processes the FdoSpatialCondition passed in as an argument.</summary>
	/// <param name="filter">Input the FdoSpatialCondition</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

	///<summary>Processes the FdoDistanceCondition passed in as an argument.</summary>
	/// <param name="filter">Input the FdoDistanceCondition</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);
	/// <summary>Processes the FdoBinaryExpression passed in as an argument.</summary>
	/// <param name="expr">Input binary expression</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

	///<summary>Processes the FdoUnaryExpression passed in as an argument.</summary>
	/// <param name="expr">Input unary expression</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

	/// <summary>Processes the FdoFunction passed in as an argument.</summary>
	/// <param name="expr">Input function</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessFunction(FdoFunction& expr);

	/// <summary>Processes the FdoIdentifier passed in as an argument.</summary>
	/// <param name="expr">Input indentifier</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessIdentifier(FdoIdentifier& expr);

	/// <summary>Processes the FdoComputedIdentifier passed in as an argument.</summary>
	/// <param name="expr">Input computed indentifier</param> 
	/// <returns>Returns nothing</returns>
	virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr) { return; }

	/// <summary>Processes the FdoParameter passed in as an argument.</summary>
	/// <param name="expr">Input parameter</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessParameter(FdoParameter& expr);

	/// <summary>Processes the FdoBooleanValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessBooleanValue(FdoBooleanValue& expr);

	/// <summary>Processes the FdoByteValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessByteValue(FdoByteValue& expr);

	/// <summary>Processes the FdoDateTimeValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

	/// <summary>Processes the FdoDecimalValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessDecimalValue(FdoDecimalValue& expr);

	/// <summary>Processes the FdoDoubleValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessDoubleValue(FdoDoubleValue& expr);

	/// <summary>Processes the FdoInt16Value passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessInt16Value(FdoInt16Value& expr);

	/// <summary>Processes the FdoInt32Value passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessInt32Value(FdoInt32Value& expr);

	/// <summary>Processes the FdoInt64Value passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessInt64Value(FdoInt64Value& expr);

	/// <summary>Processes the FdoSingleValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessSingleValue(FdoSingleValue& expr);

	/// <summary>Processes the FdoStringValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessStringValue(FdoStringValue& expr);

	/// <summary>Processes the FdoBLOBValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessBLOBValue(FdoBLOBValue& expr);

	/// <summary>Processes the FdoCLOBValue passed in as an argument.</summary>
	/// <param name="expr">Input data value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessCLOBValue(FdoCLOBValue& expr);

	/// <summary>Processes the FdoGeometryValue passed in as an argument.</summary>
	/// <param name="expr">Input geometry value</param> 
	/// <returns>Returns nothing</returns> 
	virtual void ProcessGeometryValue(FdoGeometryValue& expr);


public:
	FdoBoolean Evaluate();
	void SetClass(const FdoClassDefinitionP& classDef);
	void SetFilter(const FdoFilterP& filter);
	void SetGeoRaster(const FdoPtr<FdoRfpGeoRaster>& geoRaster);

    /// <summary>Decrease the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns> 
    FdoInt32 Release ()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
        //  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        //BAD:return (FdoIExpressionProcessor::Release () + FdoIFilterProcessor::Release ()  );
        return (this->FdoIFilterProcessor::Release());
    }
	FdoInt32 AddRef()
	{
		return (this->FdoIFilterProcessor::AddRef());
	}

	static FdoRfpFilterEvaluator* Create() { return new FdoRfpFilterEvaluator(); }

};

typedef FdoPtr<FdoRfpFilterEvaluator> FdoRfpFilterEvaluatorP;

#endif
