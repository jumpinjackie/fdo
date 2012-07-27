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
 */

#ifndef FDOOWSOGCFILTERSERIALIZER_H
#define FDOOWSOGCFILTERSERIALIZER_H

#ifdef WIN32
#pragma once
#endif

class FdoOwsOgcFilterSerializer : public FdoIFilterProcessor, public FdoIExpressionProcessor
{
public:
    /// \brief
    /// Serialize the FdoFilter passed in to OGC format. This static method is a simple
    /// wrapper for convenience.
    /// 
    /// \param filter 
    /// Input the FdoFilter
    /// \param writer 
    /// Input the FdoXmlWriter
    /// \param srsName 
    /// Input the SRS name
    /// \param prefix 
    /// Input the property name prefix
    /// 
    /// \return
    /// Returns nothing
    /// 
	FDOOWS_API static void Serialize (FdoFilter* filter, FdoXmlWriter* writer, FdoString* srsName, FdoString* prefix);

public:
    /// \brief
    /// Factory method to create an instance of FdoOwsOgcFilterSerializer class.
    /// 
    /// \return
    /// An instance of FdoOwsOgcFilterSerializer class.
    /// 
	FDOOWS_API static FdoOwsOgcFilterSerializer* Create ();

public:
    /// \brief
    /// Serialize the FdoFilter passed in to OGC format.
    /// 
    /// \param filter 
    /// Input the FdoFilter
    /// 
    /// <return>Returns nothing
    /// </returns>
    /// 
	FDOOWS_API void Serialize (FdoFilter* filter);

    /// \brief
    /// Increase the reference count.
    /// 
    /// \return
    /// Returns the new reference count (value for debugging use only).
    /// 
    FDOOWS_API FdoInt32 AddRef ()
    {
    /// NOTE: due to multiple inheritance, there is an ambiguity in which AddRef() method to call.
    ///  Calling BOTH AddRef() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::AddRef ();
	}

	FDOOWS_API FdoInt32 Release ()
    {
    /// NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
    ///  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::Release ();
    }

	FDOOWS_API void SetXmlWriter (FdoXmlWriter* writer) { mWriter = writer; }
	FDOOWS_API FdoXmlWriter* GetXmlWriter () { return mWriter; }
	
	FDOOWS_API void SetSrsName (FdoString* srsName) { mSrsName = srsName; }
	FDOOWS_API FdoString* GetSrsName () { return mSrsName; }

	FDOOWS_API void SetPrefix (FdoString* propPrefix) { mPrefix = propPrefix; }
	FDOOWS_API FdoString* GetPrefix () { return mPrefix; }

    /// Overrides from FdoIFilterProcessor

    /// \brief
    /// Processes the FdoBinaryLogicalOperator passed in as an argument.
    /// 
    /// \param filter 
    /// Input the FdoBinaryLogicalOperator
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    /// \brief
    /// Processes the FdoUnaryLogicalOperator passed in as an argument.
    /// 
    /// \param filter 
    /// Input the FdoUnaryLogicalOperator
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    /// \brief
    /// Processes the FdoComparisonCondition passed in as an argument.
    /// 
    /// \param filter 
    /// Input the FdoComparisonCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    /// \brief
    /// Processes the FdoInCondition passed in as an argument.
    /// 
    /// \param filter 
    /// Input the FdoInCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessInCondition(FdoInCondition& filter);

    /// \brief
    /// Processes the FdoNullCondition passed in as an argument.
    /// 
    /// \param filter 
    /// Input the FdoNullCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessNullCondition(FdoNullCondition& filter);

    /// \brief
    /// Processes the FdoSpatialCondition passed in as an argument.
    /// 
    /// \param filter 
    /// Input the FdoSpatialCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    /// \brief
    /// Processes the FdoDistanceCondition passed in as an argument.
    /// 
    /// \param filter 
    /// Input the FdoDistanceCondition
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);

    /// Overrides from FdoIExpressionProcessor

    /// \brief
    /// Processes the FdoBinaryExpression passed in as an argument.
    /// 
    /// \param expr 
    /// Input binary expression
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

    /// \brief
    /// Processes the FdoUnaryExpression passed in as an argument.
    /// 
    /// \param expr 
    /// Input unary expression
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    /// \brief
    /// Processes the FdoFunction passed in as an argument.
    /// 
    /// \param expr 
    /// Input function
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessFunction(FdoFunction& expr);

    /// \brief
    /// Processes the FdoIdentifier passed in as an argument.
    /// 
    /// \param expr 
    /// Input indentifier
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessIdentifier(FdoIdentifier& expr);

    /// \brief
    /// Processes the FdoComputedIdentifier passed in as an argument.
    /// 
    /// \param expr 
    /// Input indentifier
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

    /// \brief
    /// Processes the FdoParameter passed in as an argument.
    /// 
    /// \param expr 
    /// Input parameter
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessParameter(FdoParameter& expr);

    /// \brief
    /// Processes the FdoBooleanValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessBooleanValue(FdoBooleanValue& expr);

    /// \brief
    /// Processes the FdoByteValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessByteValue(FdoByteValue& expr);

    /// \brief
    /// Processes the FdoDateTimeValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    /// \brief
    /// Processes the FdoDecimalValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessDecimalValue(FdoDecimalValue& expr);

    /// \brief
    /// Processes the FdoDoubleValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessDoubleValue(FdoDoubleValue& expr);

    /// \brief
    /// Processes the FdoInt16Value passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessInt16Value(FdoInt16Value& expr);

    /// \brief
    /// Processes the FdoInt32Value passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessInt32Value(FdoInt32Value& expr);

    /// \brief
    /// Processes the FdoInt64Value passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessInt64Value(FdoInt64Value& expr);

    /// \brief
    /// Processes the FdoSingleValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessSingleValue(FdoSingleValue& expr);

    /// \brief
    /// Processes the FdoStringValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessStringValue(FdoStringValue& expr);

    /// \brief
    /// Processes the FdoBLOBValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessBLOBValue(FdoBLOBValue& expr);

    /// \brief
    /// Processes the FdoCLOBValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input data value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    /// \brief
    /// Processes the FdoGeometryValue passed in as an argument.
    /// 
    /// \param expr 
    /// Input geometry value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOOWS_API virtual void ProcessGeometryValue(FdoGeometryValue& expr);	

protected:
	FdoOwsOgcFilterSerializer ();
	virtual void Dispose ();	

private:
	void _validate();
	void _serializeNameValuePair(FdoIdentifier* prop, FdoExpression* value);

	FdoXmlWriter*    mWriter;
	FdoStringP       mSrsName;
	FdoStringP       mPrefix;
};

#endif//FDOOWSOGCFILTERSERIALIZER_H


