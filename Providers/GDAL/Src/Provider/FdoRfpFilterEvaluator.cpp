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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpFilterEvaluator.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpVariant.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpRect.h"
#include "FdoRfpFilterEvaluator.h"

FdoRfpFilterEvaluator::FdoRfpFilterEvaluator(void)
{
	m_resultStack = new FdoRfpVariantCollection();
	m_agfFactory = FdoFgfGeometryFactory::GetInstance();
}

FdoRfpFilterEvaluator::~FdoRfpFilterEvaluator(void)
{
}

void FdoRfpFilterEvaluator::_handleExpression(const FdoExpressionP& expr)
{
	expr->Process(this);
}
void FdoRfpFilterEvaluator::_handleFilter(const FdoFilterP& filter)
{
	filter->Process(this);
}
void FdoRfpFilterEvaluator::_pushResult()
{
	FdoRfpVariantP var = new FdoRfpVariant();
	m_resultStack->Add(var);
}
FdoRfpVariantP FdoRfpFilterEvaluator::_popResult()
{
	FdoInt32 count = m_resultStack->GetCount();
	FdoRfpVariantP var = m_resultStack->GetItem(count - 1);
	m_resultStack->RemoveAt(count - 1);
	return var;
}
FdoRfpVariantP FdoRfpFilterEvaluator::_getResult()
{
	FdoInt32 count = m_resultStack->GetCount();
	FdoRfpVariantP var = m_resultStack->GetItem(count - 1);
	return var;
}

FdoBoolean FdoRfpFilterEvaluator::Evaluate()
{
	if (m_filter == NULL)
		return true;
	_pushResult();
	_handleFilter(m_filter);
	FdoRfpVariantP var = _popResult();
	if (var->GetType() != FRVT_BOOLEAN)
		_throwInvalidException();
	return var->GetBoolean();
}
void FdoRfpFilterEvaluator::SetClass(const FdoClassDefinitionP& classDef)
{
	m_classDef = classDef;
}
void FdoRfpFilterEvaluator::SetFilter(const FdoFilterP& filter)
{
	m_filter = filter;
}
void FdoRfpFilterEvaluator::SetGeoRaster(const FdoRfpGeoRasterP& geoRaster)
{
	m_geoRaster = geoRaster;
}

void FdoRfpFilterEvaluator::_throwInvalidException()
{
	throw FdoException::Create(NlsMsgGet(GRFP_44_INVALID_FILTER, "Invalid filter."));
}

///<summary>Processes the FdoBinaryLogicalOperator passed in as an argument.</summary>
/// <param name="filter">Input the FdoBinaryLogicalOperator</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
	FdoFilterP lhs = filter.GetLeftOperand();
	_pushResult();
	_handleFilter(lhs);
	FdoRfpVariantP result = _popResult();
	if (result->GetType() != FRVT_BOOLEAN)
		_throwInvalidException();
	FdoBinaryLogicalOperations opr = filter.GetOperation();
	if (opr == FdoBinaryLogicalOperations_And && !result->GetBoolean())
	{
		_getResult()->SetBoolean(false);
		return;
	}
	if (opr == FdoBinaryLogicalOperations_Or && result->GetBoolean())
	{
		_getResult()->SetBoolean(true);
		return;
	}
	FdoFilterP rhs = filter.GetRightOperand();
	_pushResult();
	_handleFilter(rhs);
	result = _popResult();
	if (result->GetType() != FRVT_BOOLEAN)
		_throwInvalidException();
	_getResult()->SetBoolean(result->GetBoolean());

}

///<summary>Processes the FdoUnaryLogicalOperator passed in as an argument.</summary>
/// <param name="filter">Input the FdoUnaryLogicalOperator</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
	FdoFilterP filter1 = filter.GetOperand();
	_pushResult();
	_handleFilter(filter1);
	FdoRfpVariantP result = _popResult();
	if (result->GetType() != FRVT_BOOLEAN)
		_throwInvalidException();
	_getResult()->SetBoolean(!result->GetBoolean());
}

///<summary>Processes the FdoComparisonCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoComparisonCondition</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
	_throwInvalidException();
}

FdoBoolean FdoRfpFilterEvaluator::_isIdentifierValid(const FdoPtr<FdoIdentifier>& identifier)
{
	// first find out the feature name, class name and property name from the qualified property name
	FdoStringP featureName, className, propertyName;
	FdoRfpUtil::ParseQPropertyName(identifier->GetText(), featureName, className, propertyName);

	// validate the class name
	if (className.GetLength() != 0)
	{
		if (className != m_classDef->GetName())
			return false;
	}

	// validate the feature name
	if (featureName.GetLength() != 0)
	{
		FdoPtr<FdoSchemaElement> schema = m_classDef->GetParent();
		if (featureName != schema->GetName())
			return false;
	}

	FdoPropertiesP idProps = m_classDef->GetProperties();
	FdoPtr<FdoPropertyDefinition> prop = idProps->FindItem(propertyName);
	if (prop == NULL)
		return false;

	return true;
}

FdoBoolean FdoRfpFilterEvaluator::_isIdIdentifierValid(const FdoPtr<FdoIdentifier>& identifier)
{
	// first find out the feature name, class name and property name from the qualified property name
	FdoStringP featureName, className, propertyName;
	FdoRfpUtil::ParseQPropertyName(identifier->GetText(), featureName, className, propertyName);

	// validate the class name
	if (className.GetLength() != 0)
	{
		if (className != m_classDef->GetName())
			return false;
	}

	// validate the feature name
	if (featureName.GetLength() != 0)
	{
		FdoPtr<FdoSchemaElement> schema = m_classDef->GetParent();
		if (featureName != schema->GetName())
			return false;
	}

	FdoDataPropertiesP idProps = m_classDef->GetIdentityProperties();

	FdoPtr<FdoDataPropertyDefinition> prop = idProps->FindItem(propertyName);
	if (prop == NULL)
		return false;
	return true;
}


///<summary>Processes the FdoInCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoInCondition</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessInCondition(FdoInCondition& filter)
{
	FdoIdentifierP identifier = filter.GetPropertyName();
	if (!_isIdIdentifierValid(identifier))
		_throwInvalidException();
	FdoPtr<FdoValueExpressionCollection> exprs = filter.GetValues();
	FdoInt32 count = exprs->GetCount();
	for (FdoInt32 i = 0; i < count; i++)
	{
		FdoPtr<FdoExpression> expr = exprs->GetItem(i);
		_pushResult();
		_handleExpression(expr);
		FdoRfpVariantP result = _popResult();
		if (result->GetType() != FRVT_STRING)
			_throwInvalidException();
        if (STREQUAL(m_geoRaster->GetId(), result->GetString()))
		{
			_getResult()->SetBoolean(true);
			return;
		}
	}

	_getResult()->SetBoolean(false);
}

///<summary>Processes the FdoNullCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoNullCondition</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessNullCondition(FdoNullCondition& filter)
{
	_throwInvalidException();
}

///<summary>Processes the FdoSpatialCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoSpatialCondition</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
	FdoIdentifierP identifier = filter.GetPropertyName();
	//The identifier must not be a id property and must exist in the properties
	if (_isIdIdentifierValid(identifier) || !_isIdentifierValid(identifier))
		_throwInvalidException();
	FdoExpressionP expr = filter.GetGeometry();
	_pushResult();
	_handleExpression(expr);
	FdoRfpVariantP result = _popResult();
	if (result->GetType() != FRVT_GEOMETRY)
		_throwInvalidException();
	FdoSpatialOperations opr = filter.GetOperation();
	//Get the envelope from the geometry data
	FdoGeometryValue* geoVal = result->GetGeometryValue();
	FdoPtr<FdoByteArray> geometryData = geoVal->GetGeometry();


	FdoInt32 i;
	for (i=0; i<m_geoRaster->GetNumberOfBands(); i++)
	{
		FdoRfpGeoBandRasterP geoBandRaster = m_geoRaster->GetBand(i);
		if (geoBandRaster == NULL)
			continue;

		if (geoBandRaster->IsRotated())
		{
			FdoPtr<FdoFgfGeometryFactory> agfFactory = FdoFgfGeometryFactory::GetInstance();
			FdoPtr<FdoIGeometry> lhs = agfFactory->CreateGeometryFromFgf(geometryData);
			FdoPtr<FdoIGeometry> rhs = geoBandRaster->GetGeometry();
			switch (opr)
			{
			case FdoSpatialOperations_Intersects:
			case FdoSpatialOperations_Within:
				{
					_getResult()->SetBoolean(FdoSpatialUtility::Evaluate(lhs, opr, rhs));
					break;
				}
			case FdoSpatialOperations_EnvelopeIntersects:
				{
					// get the bounding box of the two geometries
					FdoPtr<FdoIEnvelope> envelopeL = lhs->GetEnvelope();
					FdoPtr<FdoIEnvelope> envelopeR = rhs->GetEnvelope();

					FdoRfpRect lhs1 = FdoRfpRect(envelopeL->GetMinX(), envelopeL->GetMinY(), 
						envelopeL->GetMaxX(), envelopeL->GetMaxY());
					FdoRfpRect rhs1 = FdoRfpRect(envelopeR->GetMinX(), envelopeR->GetMinY(), 
						envelopeR->GetMaxX(), envelopeR->GetMaxY());

					_getResult()->SetBoolean(lhs1.IsIntersecting(rhs1));
					break;
				}
			case FdoSpatialOperations_Inside:
				{
					_getResult()->SetBoolean(FdoSpatialUtility::Evaluate(lhs, FdoSpatialOperations_Within, rhs));
					break;
				}
			default:
				_throwInvalidException();
				break;
			};
		}
		else // axis aligned
		{
			FdoRfpRect lhs = geoBandRaster->GetBounds();
			FdoRfpRect rhs = FdoRfpUtil::CreateRectFromGeometryAgf(geometryData);
	    	
			switch (opr)
			{
			case FdoSpatialOperations_Intersects:
			case FdoSpatialOperations_EnvelopeIntersects:
				{
					_getResult()->SetBoolean(lhs.IsIntersecting(rhs));
					break;
				}
			case FdoSpatialOperations_Within:
				{
					_getResult()->SetBoolean(lhs.IsWithin(rhs));
					break;
				}
			case FdoSpatialOperations_Inside:
				{
					_getResult()->SetBoolean(lhs.IsInside(rhs));
					break;
				}
			default:
				_throwInvalidException();
				break;
			};
		}
	}
}

///<summary>Processes the FdoDistanceCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoDistanceCondition</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoBinaryExpression passed in as an argument.</summary>
/// <param name="expr">Input binary expression</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
	_throwInvalidException();
}

///<summary>Processes the FdoUnaryExpression passed in as an argument.</summary>
/// <param name="expr">Input unary expression</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoFunction passed in as an argument.</summary>
/// <param name="expr">Input function</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessFunction(FdoFunction& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoIdentifier passed in as an argument.</summary>
/// <param name="expr">Input indentifier</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessIdentifier(FdoIdentifier& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoParameter passed in as an argument.</summary>
/// <param name="expr">Input parameter</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessParameter(FdoParameter& expr)
{
	_getResult()->SetString(expr.GetName());
}

/// <summary>Processes the FdoBooleanValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessBooleanValue(FdoBooleanValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoByteValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessByteValue(FdoByteValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoDateTimeValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoDecimalValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessDecimalValue(FdoDecimalValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoDoubleValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessDoubleValue(FdoDoubleValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoInt16Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessInt16Value(FdoInt16Value& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoInt32Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessInt32Value(FdoInt32Value& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoInt64Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessInt64Value(FdoInt64Value& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoSingleValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessSingleValue(FdoSingleValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoStringValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessStringValue(FdoStringValue& expr)
{
	_getResult()->SetString(expr.GetString());
}

/// <summary>Processes the FdoBLOBValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessBLOBValue(FdoBLOBValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoCLOBValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessCLOBValue(FdoCLOBValue& expr)
{
	_throwInvalidException();
}

/// <summary>Processes the FdoGeometryValue passed in as an argument.</summary>
/// <param name="expr">Input geometry value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpFilterEvaluator::ProcessGeometryValue(FdoGeometryValue& expr)
{
	_getResult()->SetGeometryValue(&expr);
}
