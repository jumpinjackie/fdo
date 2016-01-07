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

#include <stdafx.h>
#include <OWS/FdoOwsOgcFilterSerializer.h>
#include "FdoOwsOgcGeometrySerializer.h"
#include <OWS/FdoOwsGlobals.h>

FdoOwsOgcFilterSerializer::FdoOwsOgcFilterSerializer ()
	: mWriter(NULL) 
{
}

FdoOwsOgcFilterSerializer* FdoOwsOgcFilterSerializer::Create ()
{
	return new FdoOwsOgcFilterSerializer;
}

void FdoOwsOgcFilterSerializer::Serialize (FdoFilter* filter)
{
	VALIDATE_ARGUMENT(filter);

	_validate();
	filter->Process (this);
}

void FdoOwsOgcFilterSerializer::Serialize (	FdoFilter* filter, FdoXmlWriter* writer, FdoString* srsName, FdoString* prefix)
{
	FdoPtr<FdoOwsOgcFilterSerializer> serializer = FdoOwsOgcFilterSerializer::Create ();
	serializer->SetXmlWriter (writer);
	serializer->SetSrsName (srsName);
	serializer->SetPrefix (prefix);

	serializer->Serialize (filter);
}

void FdoOwsOgcFilterSerializer::Dispose ()
{
	delete this;
}

void FdoOwsOgcFilterSerializer::ProcessBinaryLogicalOperator (FdoBinaryLogicalOperator& filter)
{
	// Binary operator: AND, OR
	FdoBinaryLogicalOperations blo = filter.GetOperation();
	switch(blo)
	{
	case FdoBinaryLogicalOperations_And:
		mWriter->WriteStartElement(FdoOwsGlobals::Logical_And);
		break;
	case FdoBinaryLogicalOperations_Or:
		mWriter->WriteStartElement(FdoOwsGlobals::Logical_Or);
		break;
	default:
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_86_UNSUPPORTED_BINARY_OPERATION), "Unsupported FDO binary operation."));
	}
	
	FdoPtr<FdoFilter> lFilter = filter.GetLeftOperand ();
	this->Serialize (lFilter);
	FdoPtr<FdoFilter> rFilter = filter.GetRightOperand ();
	this->Serialize (rFilter);	

	mWriter->WriteEndElement();	
}

void FdoOwsOgcFilterSerializer::ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter)
{
	// Unary operator: NOT
	FdoUnaryLogicalOperations ulo = filter.GetOperation();
	switch (ulo)
	{
		case FdoUnaryLogicalOperations_Not:
		{
			mWriter->WriteStartElement (FdoOwsGlobals::Logical_Not);
			FdoPtr<FdoFilter> operand = filter.GetOperand ();
			this->Serialize (operand);
			mWriter->WriteEndElement();
			break;
		}
		default:
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_87_UNSUPPORTED_UNARY_OPERATION), "Unsupported FDO unary operation."));
	}
}

void FdoOwsOgcFilterSerializer::ProcessComparisonCondition (FdoComparisonCondition& filter)
{
	FdoComparisonOperations operation = filter.GetOperation();
	FdoString* startElement = NULL;
	switch(operation)
	{
	case FdoComparisonOperations_EqualTo:
		startElement = FdoOwsGlobals::PropertyIsEqualTo; 
		break;
	case FdoComparisonOperations_NotEqualTo:
		startElement = FdoOwsGlobals::PropertyIsNotEqualTo; 
		break;
	case FdoComparisonOperations_GreaterThan:
		startElement = FdoOwsGlobals::PropertyIsGreaterThan;
		break;
	case FdoComparisonOperations_GreaterThanOrEqualTo:
		startElement = FdoOwsGlobals::PropertyIsGreaterThanOrEqualTo;
		break;
	case FdoComparisonOperations_LessThan:
		startElement = FdoOwsGlobals::PropertyIsLessThan;
		break;
	case FdoComparisonOperations_LessThanOrEqualTo:
		startElement = FdoOwsGlobals::PropertyIsLessThanOrEqualTo;
		break;
	// Note: "LIKE" is special.
	case FdoComparisonOperations_Like:
	{
		startElement = FdoOwsGlobals::PropertyIsLike;
		mWriter->WriteStartElement(startElement);
		mWriter->WriteAttribute(FdoOwsGlobals::wildCard, L"*");
		mWriter->WriteAttribute(FdoOwsGlobals::singleChar, L"#");
		mWriter->WriteAttribute(FdoOwsGlobals::escapeChar, L"!");
		mWriter->WriteStartElement(FdoOwsGlobals::PropertyName);

		if (mPrefix.GetLength())
		{
			mWriter->WriteCharacters(mPrefix + L":");
		}
			
		mWriter->WriteCharacters(filter.GetLeftExpression()->ToString());			
		mWriter->WriteEndElement();
		mWriter->WriteStartElement(FdoOwsGlobals::Literal);
		FdoPtr<FdoExpression> rExpr = filter.GetRightExpression ();
		FdoStringValue* pliteral = static_cast<FdoStringValue *>(rExpr.p);
		mWriter->WriteCharacters(pliteral->GetString());
		// Write end tag "</Literal>"
		mWriter->WriteEndElement();  
		// Write end tag "</PropertyIsLike>"
		mWriter->WriteEndElement();
        return;
	}
	default:
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_83_UNSUPPORTED_COMPARISON_OPERATION), "Unsupported FDO comparison operation."));
		break;
	}

	mWriter->WriteStartElement(startElement);	
	FdoPtr<FdoExpression> lExpr = filter.GetLeftExpression ();
	lExpr->Process(this);
	FdoPtr<FdoExpression> rExpr = filter.GetRightExpression ();
	rExpr->Process(this);	
	mWriter->WriteEndElement();
}

void FdoOwsOgcFilterSerializer::ProcessInCondition (FdoInCondition& filter)
{
	FdoPtr<FdoIdentifier> prop = filter.GetPropertyName();
	FdoPtr<FdoValueExpressionCollection> propValues = filter.GetValues();
	FdoInt32 cnt = propValues->GetCount();

	if (cnt <= 0)
	{
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_108_INVALID_FDO_IN_CONDITION), "Invalid FDO In condition."));
	}
	else if (cnt == 1)
	{
		FdoPtr<FdoValueExpression> propValue = propValues->GetItem (0);
		_serializeNameValuePair(prop, propValue);
	}
	else
	{
		mWriter->WriteStartElement(FdoOwsGlobals::Logical_Or);
		for (FdoInt32 i=0; i<cnt; i++)
		{
			FdoPtr<FdoValueExpression> propValue = propValues->GetItem (i);
			_serializeNameValuePair(prop, propValue);
		}

		mWriter->WriteEndElement();		
	}
}

void FdoOwsOgcFilterSerializer::ProcessNullCondition (FdoNullCondition& filter)
{
	mWriter->WriteStartElement(FdoOwsGlobals::PropertyIsNull);
	mWriter->WriteStartElement(FdoOwsGlobals::PropertyName);
	if (mPrefix.GetLength())
	{
		mWriter->WriteCharacters(mPrefix + L":");
	}
	mWriter->WriteCharacters(filter.GetPropertyName()->ToString());
	mWriter->WriteEndElement();
	mWriter->WriteEndElement();
}

void FdoOwsOgcFilterSerializer::ProcessSpatialCondition (FdoSpatialCondition& filter)
{
	FdoSpatialOperations operation = filter.GetOperation();						
	FdoString* startElement = NULL;

	switch (operation)
	{
	//#pragma message ("TODO: Support OGC Operator BBOX through EnvelopeIntersects?")
	case FdoSpatialOperations_EnvelopeIntersects:
		startElement = FdoOwsGlobals::Intersects;	break;
	case FdoSpatialOperations_Contains: 
		startElement = FdoOwsGlobals::Contains;		break;
	case FdoSpatialOperations_Crosses:
		startElement = FdoOwsGlobals::Crosses;		break;
	case FdoSpatialOperations_Disjoint:
		startElement = FdoOwsGlobals::Disjoint;		break;
	case FdoSpatialOperations_Equals:
		startElement = FdoOwsGlobals::Equals;		break;
	case FdoSpatialOperations_Intersects:
		startElement = FdoOwsGlobals::Intersects;	break;
	case FdoSpatialOperations_Overlaps:
		startElement = FdoOwsGlobals::Overlaps;		break;
	case FdoSpatialOperations_Touches:
		startElement = FdoOwsGlobals::Touches;		break;
	case FdoSpatialOperations_Within:
		startElement = FdoOwsGlobals::Within;		break;
    // Error cases
	case FdoSpatialOperations_CoveredBy:
	case FdoSpatialOperations_Inside:
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_105_UNSUPPORTED_SPATIAL_OPERATION), "Unsupported FDO spatial operation."));
		break;
	}
	mWriter->WriteStartElement(startElement);				
	mWriter->WriteStartElement(FdoOwsGlobals::PropertyName);
	if (mPrefix.GetLength())
	{
		mWriter->WriteCharacters(mPrefix + L":");
	}
	mWriter->WriteCharacters(filter.GetPropertyName()->ToString());
	mWriter->WriteEndElement ();
	FdoPtr<FdoExpression> geomExpr = filter.GetGeometry ();
	geomExpr->Process(this);
	mWriter->WriteEndElement();
}

void FdoOwsOgcFilterSerializer::ProcessDistanceCondition (FdoDistanceCondition& filter)
{
	FdoDistanceOperations distanceOperation = filter.GetOperation();
	switch(distanceOperation)
	{
	case FdoDistanceOperations_Beyond:
		mWriter->WriteStartElement(FdoOwsGlobals::Beyond);
		break;
	case FdoDistanceOperations_Within:
		mWriter->WriteStartElement(FdoOwsGlobals::DWithin);
		break;
	}
			
	FdoPtr<FdoExpression> geometry = filter.GetGeometry();

	geometry->Process (this);

	double distance = filter.GetDistance();
	static const int SIZE = 256;
	wchar_t szDistance[SIZE];
	swprintf (szDistance, SIZE - 1, L"%f", distance);
	mWriter->WriteStartElement(FdoOwsGlobals::Distance);
	//
	// How to get the measure unit is to be determined. The default value is used here.
	//
	mWriter->WriteAttribute(FdoOwsGlobals::unit, L"http://www.uomdict.com/uom.html#meters");	
	mWriter->WriteCharacters(szDistance);	
	mWriter->WriteEndElement(); // end tag </Distance>
	mWriter->WriteEndElement(); // </Beyond> or </DWithin>
}

void FdoOwsOgcFilterSerializer::_validate ()
{
	if (this->mWriter == NULL)
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR), "Unexpected error encountered."));
}

void FdoOwsOgcFilterSerializer::_serializeNameValuePair(FdoIdentifier* prop, FdoExpression* value)
{
	mWriter->WriteStartElement(FdoOwsGlobals::PropertyIsEqualTo);
	mWriter->WriteStartElement(FdoOwsGlobals::PropertyName);
	if (mPrefix.GetLength())
	{
		mWriter->WriteCharacters(mPrefix + L":");
	}
	mWriter->WriteCharacters(prop->ToString());
	mWriter->WriteEndElement();
	value->Process (this);
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoBinaryExpression passed in as an argument.</summary>
/// <param name="expr">Input binary expression</param>
/// <returns>Returns nothing</returns>
void FdoOwsOgcFilterSerializer::ProcessBinaryExpression (FdoBinaryExpression& expr)
{
	FdoBinaryOperations operation = expr.GetOperation();
	switch (operation)
	{
	case FdoBinaryOperations_Add:
		mWriter->WriteStartElement(FdoOwsGlobals::Add);	break;
	case FdoBinaryOperations_Subtract:
		mWriter->WriteStartElement(FdoOwsGlobals::Sub);	break;
	case FdoBinaryOperations_Multiply:
		mWriter->WriteStartElement(FdoOwsGlobals::Mul);	break;
	case FdoBinaryOperations_Divide:
		mWriter->WriteStartElement(FdoOwsGlobals::Div);	break;
	default:
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_86_UNSUPPORTED_BINARY_OPERATION), "Unsupported FDO binary operation."));
	}

	FdoPtr<FdoExpression> lExpr = expr.GetLeftExpression ();
	lExpr->Process(this);
	FdoPtr<FdoExpression> rExpr = expr.GetRightExpression ();
	rExpr->Process(this);
	mWriter->WriteEndElement();	
}

///<summary>Processes the FdoUnaryExpression passed in as an argument.</summary>
/// <param name="expr">Input unary expression</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessUnaryExpression (FdoUnaryExpression& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoFunction passed in as an argument.</summary>
/// <param name="expr">Input function</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessFunction(FdoFunction& expr)
{
	FdoString* name = expr.GetName();
	mWriter->WriteStartElement(FdoOwsGlobals::Function);
	mWriter->WriteAttribute(FdoOwsGlobals::name, name);
	FdoPtr<FdoExpressionCollection> args = expr.GetArguments();
	FdoInt32 cnt = args->GetCount();
	for (FdoInt32 i=0; i<cnt; i++)
	{
		FdoPtr<FdoExpression> expression = args->GetItem(i);
		expression->Process(this);
	}
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoIdentifier passed in as an argument.</summary>
/// <param name="expr">Input indentifier</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessIdentifier(FdoIdentifier& expr)
{
	FdoString* name = expr.GetName();
	mWriter->WriteStartElement(FdoOwsGlobals::PropertyName);
	if (mPrefix.GetLength())
	{
		mWriter->WriteCharacters(mPrefix + L":");
	}
	mWriter->WriteCharacters(name);
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoComputedIdentifier passed in as an argument.</summary>
/// <param name="expr">Input indentifier</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
	//#pragma message ("TODO: Does WFS support comnputed identifiers?")
	ProcessIdentifier (expr);	
}

/// <summary>Processes the FdoParameter passed in as an argument.</summary>
/// <param name="expr">Input parameter</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessParameter(FdoParameter& /*expr*/)
{
	//#pragma message ("TODO: Does WFS support parameters?")
    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_107_PARAEMETRS_NOT_SUPPORTED_IN_FILTER), "Parameters not supported in the FDO Filter."));
}

/// <summary>Processes the FdoBooleanValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessBooleanValue(FdoBooleanValue& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoByteValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessByteValue(FdoByteValue& expr)
{
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
}

/// <summary>Processes the FdoDateTimeValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
}

/// <summary>Processes the FdoDecimalValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessDecimalValue(FdoDecimalValue& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoDoubleValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessDoubleValue(FdoDoubleValue& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoInt16Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessInt16Value(FdoInt16Value& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoInt32Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessInt32Value(FdoInt32Value& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoInt64Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessInt64Value(FdoInt64Value& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoSingleValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessSingleValue(FdoSingleValue& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.IsNull() ? L"" : expr.ToString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoStringValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessStringValue(FdoStringValue& expr)
{
	mWriter->WriteStartElement(FdoOwsGlobals::Literal);
	mWriter->WriteCharacters(expr.GetString());
	mWriter->WriteEndElement();
}

/// <summary>Processes the FdoBLOBValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessBLOBValue(FdoBLOBValue& /* expr */)
{
	//#pragma message ("TODO: Does WFS support LOB?")
    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_106_LOB_NOT_SUPPORTED_IN_FILTER), "LOB objects not supported in the FDO Filter."));
}

/// <summary>Processes the FdoCLOBValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessCLOBValue(FdoCLOBValue& /* expr */)
{
	//#pragma message ("TODO: Does WFS support LOB?")
    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_106_LOB_NOT_SUPPORTED_IN_FILTER), "LOB objects not supported in the FDO Filter."));
}

/// <summary>Processes the FdoGeometryValue passed in as an argument.</summary>
/// <param name="expr">Input geometry value</param> 
/// <returns>Returns nothing</returns> 
void FdoOwsOgcFilterSerializer::ProcessGeometryValue(FdoGeometryValue& expr)
{
	FdoPtr<FdoByteArray> gba = expr.GetGeometry();
	FdoPtr<FdoFgfGeometryFactory> geoFactory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIGeometry> geometry = geoFactory->CreateGeometryFromFgf(gba);
	FdoOwsOgcGeometrySerializer::SerializeGeometry(geometry, mWriter, mSrsName);	
}
