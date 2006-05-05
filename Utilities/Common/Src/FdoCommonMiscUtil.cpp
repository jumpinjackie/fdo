/*
 * 
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

#include "stdafx.h"
#include "FdoCommonMiscUtil.h"
#include "FdoCommonSchemaUtil.h"
#include "FdoCommonOSUtil.h"
#include "FdoCommonFilterExecutor.h"


FdoString* FdoCommonMiscUtil::FdoDataTypeToString (FdoDataType logicalPropertyType)
{
    wchar_t* logicalTypeString;

    switch (logicalPropertyType)
    {
        case FdoDataType_Boolean:
            logicalTypeString = L"FdoDataType_Boolean";
            break;
        case FdoDataType_Byte:
            logicalTypeString = L"FdoDataType_Byte";
            break;
        case FdoDataType_DateTime:
            logicalTypeString = L"FdoDataType_DateTime";
            break;
        case FdoDataType_Decimal:
            logicalTypeString = L"FdoDataType_Decimal";
            break;
        case FdoDataType_Double:
            logicalTypeString = L"FdoDataType_Double";
            break;
        case FdoDataType_Int16:
            logicalTypeString = L"FdoDataType_Int16";
            break;
        case FdoDataType_Int32:
            logicalTypeString = L"FdoDataType_Int32";
            break;
        case FdoDataType_Int64:
            logicalTypeString = L"FdoDataType_Int64";
            break;
        case FdoDataType_Single:
            logicalTypeString = L"FdoDataType_Single";
            break;
        case FdoDataType_String:
            logicalTypeString = L"FdoDataType_String";
            break;
        case FdoDataType_BLOB:
            logicalTypeString = L"FdoDataType_BLOB";
            break;
        case FdoDataType_CLOB:
            logicalTypeString = L"FdoDataType_CLOB";
            break;
        default:
            logicalTypeString = L"Unknown FdoDataType";
            break;
    }

    return (logicalTypeString);
}



FdoString* FdoCommonMiscUtil::FdoPropertyTypeToString (FdoPropertyType logicalPropertyType)
{
    wchar_t* logicalTypeString;

    switch (logicalPropertyType)
    {
        case FdoPropertyType_DataProperty:
            logicalTypeString = L"FdoPropertyType_DataProperty";
            break;
        case FdoPropertyType_ObjectProperty:
            logicalTypeString = L"FdoPropertyType_ObjectProperty";
            break;
        case FdoPropertyType_GeometricProperty:
            logicalTypeString = L"FdoPropertyType_GeometricProperty";
            break;
        case FdoPropertyType_AssociationProperty:
            logicalTypeString = L"FdoPropertyType_AssociationProperty";
            break;
        case FdoPropertyType_RasterProperty:
            logicalTypeString = L"FdoPropertyType_RasterProperty";
            break;
        default:
            logicalTypeString = L"Unknown FdoPropertyType";
            break;
    }

    return (logicalTypeString);
}



FdoString* FdoCommonMiscUtil::FdoClassTypeToString (FdoClassType logicalClassType)
{
    wchar_t* logicalTypeString;

    switch (logicalClassType)
    {
        case FdoClassType_Class:
            logicalTypeString = L"FdoClassType_Class";
            break;
        case FdoClassType_FeatureClass:
            logicalTypeString = L"FdoClassType_FeatureClass";
            break;
        case FdoClassType_NetworkClass:
            logicalTypeString = L"FdoClassType_NetworkClass";
            break;
        case FdoClassType_NetworkLayerClass:
            logicalTypeString = L"FdoClassType_NetworkLayerClass";
            break;
        case FdoClassType_NetworkNodeClass:
            logicalTypeString = L"FdoClassType_NetworkNodeClass";
            break;
        case FdoClassType_NetworkLinkClass:
            logicalTypeString = L"FdoClassType_NetworkLinkClass";
            break;
        default:
            logicalTypeString = L"Unknown FdoClassType";
            break;
    }

    return (logicalTypeString);
};

FdoStringP FdoCommonMiscUtil::FdoCommandTypeToString (FdoInt32 command)
{
    FdoStringP ret;

    switch (command)
    {
        case FdoCommandType_Select:
            ret = L"FdoCommandType_Select";
            break;
        case FdoCommandType_Insert:
            ret = L"FdoCommandType_Insert";
            break;
        case FdoCommandType_Delete:
            ret = L"FdoCommandType_Delete";
            break;
        case FdoCommandType_Update:
            ret = L"FdoCommandType_Update";
            break;
        case FdoCommandType_DescribeSchema:
            ret = L"FdoCommandType_DescribeSchema";
            break;
        case FdoCommandType_DescribeSchemaMapping:
            ret = L"FdoCommandType_DescribeSchemaMapping";
            break;
        case FdoCommandType_ApplySchema:
            ret = L"FdoCommandType_ApplySchema";
            break;
        case FdoCommandType_DestroySchema:
            ret = L"FdoCommandType_DestroySchema";
            break;
        case FdoCommandType_ActivateSpatialContext:
            ret = L"FdoCommandType_ActivateSpatialContext";
            break;
        case FdoCommandType_CreateSpatialContext:
            ret = L"FdoCommandType_CreateSpatialContext";
            break;
        case FdoCommandType_DestroySpatialContext:
            ret = L"FdoCommandType_DestroySpatialContext";
            break;
        case FdoCommandType_GetSpatialContexts:
            ret = L"FdoCommandType_GetSpatialContexts";
            break;
        case FdoCommandType_CreateMeasureUnit:
            ret = L"FdoCommandType_CreateMeasureUnit";
            break;
        case FdoCommandType_DestroyMeasureUnit:
            ret = L"FdoCommandType_DestroyMeasureUnit";
            break;
        case FdoCommandType_GetMeasureUnits:
            ret = L"FdoCommandType_GetMeasureUnits";
            break;
        case FdoCommandType_SQLCommand:
            ret = L"FdoCommandType_SQLCommand";
            break;
        case FdoCommandType_AcquireLock:
            ret = L"FdoCommandType_AcquireLock";
            break;
        case FdoCommandType_GetLockInfo:
            ret = L"FdoCommandType_GetLockInfo";
            break;
        case FdoCommandType_GetLockedObjects:
            ret = L"FdoCommandType_GetLockedObjects";
            break;
        case FdoCommandType_GetLockOwners:
            ret = L"FdoCommandType_GetLockOwners";
            break;
        case FdoCommandType_ReleaseLock:
            ret = L"FdoCommandType_ReleaseLock";
            break;
        case FdoCommandType_ActivateLongTransaction:
            ret = L"FdoCommandType_ActivateLongTransaction";
            break;
        case FdoCommandType_DeactivateLongTransaction:
            ret = L"FdoCommandType_DeactivateLongTransaction";
            break;
        case FdoCommandType_CommitLongTransaction:
            ret = L"FdoCommandType_CommitLongTransaction";
            break;
        case FdoCommandType_CreateLongTransaction:
            ret = L"FdoCommandType_CreateLongTransaction";
            break;
        case FdoCommandType_GetLongTransactions:
            ret = L"FdoCommandType_GetLongTransactions";
            break;
        case FdoCommandType_FreezeLongTransaction:
            ret = L"FdoCommandType_FreezeLongTransaction";
            break;
        case FdoCommandType_RollbackLongTransaction:
            ret = L"FdoCommandType_RollbackLongTransaction";
            break;
        case FdoCommandType_ActivateLongTransactionCheckpoint:
            ret = L"FdoCommandType_ActivateLongTransactionCheckpoint";
            break;
        case FdoCommandType_CreateLongTransactionCheckpoint:
            ret = L"FdoCommandType_CreateLongTransactionCheckpoint";
            break;
        case FdoCommandType_GetLongTransactionCheckpoints:
            ret = L"FdoCommandType_GetLongTransactionCheckpoints";
            break;
        case FdoCommandType_RollbackLongTransactionCheckpoint:
            ret = L"FdoCommandType_RollbackLongTransactionCheckpoint";
            break;
        case FdoCommandType_ChangeLongTransactionPrivileges:
            ret = L"FdoCommandType_ChangeLongTransactionPrivileges";
            break;
        case FdoCommandType_GetLongTransactionPrivileges:
            ret = L"FdoCommandType_GetLongTransactionPrivileges";
            break;
        case FdoCommandType_ChangeLongTransactionSet:
            ret = L"FdoCommandType_ChangeLongTransactionSet";
            break;
        case FdoCommandType_GetLongTransactionsInSet:
            ret = L"FdoCommandType_GetLongTransactionsInSet";
            break;
        case FdoCommandType_NetworkShortestPath:
            ret = L"FdoCommandType_NetworkShortestPath";
            break;
        case FdoCommandType_NetworkAllPaths:
            ret = L"FdoCommandType_NetworkAllPaths";
            break;
        case FdoCommandType_NetworkReachableNodes:
            ret = L"FdoCommandType_NetworkReachableNodes";
            break;
        case FdoCommandType_NetworkReachingNodes:
            ret = L"FdoCommandType_NetworkReachingNodes";
            break;
        case FdoCommandType_NetworkNearestNeighbors:
            ret = L"FdoCommandType_NetworkNearestNeighbors";
            break;
        case FdoCommandType_NetworkWithinCost:
            ret = L"FdoCommandType_NetworkWithinCost";
            break;
        case FdoCommandType_NetworkTSP:
            ret = L"FdoCommandType_NetworkTSP";
            break;
        case FdoCommandType_ActivateTopologyArea:
            ret = L"FdoCommandType_ActivateTopologyArea";
            break;
        case FdoCommandType_DeactivateTopologyArea:
            ret = L"FdoCommandType_DeactivateTopologyArea";
            break;
        case FdoCommandType_ActivateTopologyInCommandResult:
            ret = L"FdoCommandType_ActivateTopologyInCommandResult";
            break;
        case FdoCommandType_DeactivateTopologyInCommandResults:
            ret = L"FdoCommandType_DeactivateTopologyInCommandResults";
            break;
        case FdoCommandType_SelectAggregates:
            ret = L"FdoCommandType_SelectAggregates";
            break;
        default:
            {
                wchar_t buffer[9];

                FdoCommonOSUtil::swprintf (buffer, ELEMENTS(buffer), L"#%d", command);
                ret = buffer;
            }
            break;
    }

    return (ret);
}

FdoStringP FdoCommonMiscUtil::FdoSpatialOperationsToString (FdoSpatialOperations op)
{
    FdoStringP ret;

    switch (op)
    {
        case FdoSpatialOperations_Contains:
            ret = L"FdoSpatialOperations_Contains";
            break;
        case FdoSpatialOperations_Crosses:
            ret = L"FdoSpatialOperations_Crosses";
            break;
        case FdoSpatialOperations_Disjoint:
            ret = L"FdoSpatialOperations_Disjoint";
            break;
        case FdoSpatialOperations_Equals:
            ret = L"FdoSpatialOperations_Equals";
            break;
        case FdoSpatialOperations_Intersects:
            ret = L"FdoSpatialOperations_Intersects";
            break;
        case FdoSpatialOperations_Overlaps:
            ret = L"FdoSpatialOperations_Overlaps";
            break;
        case FdoSpatialOperations_Touches:
            ret = L"FdoSpatialOperations_Touches";
            break;
        case FdoSpatialOperations_Within:
            ret = L"FdoSpatialOperations_Within";
            break;
        case FdoSpatialOperations_CoveredBy:
            ret = L"FdoSpatialOperations_CoveredBy";
            break;
        case FdoSpatialOperations_Inside:
            ret = L"FdoSpatialOperations_Inside";
            break;
        case FdoSpatialOperations_EnvelopeIntersects:
            ret = L"FdoSpatialOperations_EnvelopeIntersects";
            break;
        default:
            {
                wchar_t buffer[9];
                FdoCommonOSUtil::swprintf (buffer, ELEMENTS(buffer), L"#%d", op);
                ret = buffer;
            }
            break;
    }
    return (ret);
}


FdoStringP FdoCommonMiscUtil::FdoGeometryTypeToString (FdoGeometryType geomType)
{
    FdoStringP ret;

    switch (geomType)
    {
        case FdoGeometryType_None:
            ret = L"FdoGeometryType_None";
        break;

        case FdoGeometryType_Point:
            ret = L"FdoGeometryType_Point";
        break;

        case FdoGeometryType_LineString:
            ret = L"FdoGeometryType_LineString";
        break;

        case FdoGeometryType_Polygon:
            ret = L"FdoGeometryType_Polygon";
        break;

        case FdoGeometryType_MultiPoint:
            ret = L"FdoGeometryType_MultiPoint";
        break;

        case FdoGeometryType_MultiLineString:
            ret = L"FdoGeometryType_MultiLineString";
        break;

        case FdoGeometryType_MultiPolygon:
            ret = L"FdoGeometryType_MultiPolygon";
        break;

        case FdoGeometryType_MultiGeometry:
            ret = L"FdoGeometryType_MultiGeometry";
        break;

        case FdoGeometryType_CurveString:
            ret = L"FdoGeometryType_CurveString";
        break;

        case FdoGeometryType_CurvePolygon:
            ret = L"FdoGeometryType_CurvePolygon";
        break;

        case FdoGeometryType_MultiCurveString:
            ret = L"FdoGeometryType_MultiCurveString";
        break;
    
        case FdoGeometryType_MultiCurvePolygon:
            ret = L"FdoGeometryType_MultiCurvePolygon";
        break;

        default:
        {
            wchar_t buffer[9];
            FdoCommonOSUtil::swprintf (buffer, ELEMENTS(buffer), L"#%d", geomType);
            ret = buffer;
        }
        break;
    }

    return ret;
}


// Returns: -1 if d1 < d2,  0 if d1==d2,  1 of d1>d2.
//    Note that if d1 and d2 are different date/time types, they are sorted as follows: datetime < date < time.
int FdoCommonMiscUtil::CompareDateTimes(FdoDateTime d1, FdoDateTime d2)
{
    if (d1.IsDateTime() && d2.IsDate())
        return -1;
    else if (d1.IsDateTime() && d2.IsTime())
        return -1;
    else if (d1.IsDate() && d2.IsTime())
        return -1;
    else if (d1.IsDate() && d2.IsDateTime())
        return 1;
    else if (d1.IsTime() && d2.IsDate())
        return 1;
    else if (d1.IsTime() && d2.IsDateTime())
        return 1;
    else  // both FdoDateTime are the same date/time type
    {
        // Compare date component (if any):
        if (d1.IsDateTime() || d1.IsDate())
        {
            if (d1.year != d2.year)
                return (d1.year < d2.year) ? -1 : 1;
            else if (d1.month != d2.month)
                return  (d1.month < d2.month)  ? -1 : 1;
            else if (d1.day != d2.day)
                return (d1.day < d2.day)  ? -1 : 1;
        }

        // Compare time component (if any):
        if (d1.IsDateTime() || d1.IsTime())
        {
            if (d1.hour != d2.hour)
                return (d1.hour < d2.hour)  ? -1 : 1;
            else if (d1.minute != d2.minute)
                return (d1.minute < d2.minute)  ? -1 : 1;
            else if (d1.seconds != d2.seconds)
                return (d1.seconds < d2.seconds)  ? -1 : 1;
        }
    }

    return 0;  // d1==d2 if we get here
}


int FdoCommonMiscUtil::CompareDataValues(FdoDataValue *d1, FdoDataValue *d2)
{
    if ((d1 == NULL) || (d2 == NULL))
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_60_NULL_POINTER)));

    if (FdoCommonFilterExecutor::IsLessThan(d1, d2))
        return -1;
    else if (FdoCommonFilterExecutor::IsEqualTo(d1,d2))
        return 0;
    else
        return 1;
}


FdoPropertyValue* FdoCommonMiscUtil::GetPropertyValue(FdoString *propName, FdoPropertyType propType, FdoDataType dataType, FdoIReader* reader)
{
    VALIDATE_ARGUMENT(propName);
    VALIDATE_ARGUMENT(reader);

    FdoPtr<FdoPropertyValue> propValue = FdoPropertyValue::Create(propName, NULL);
    FdoPtr<FdoValueExpression> valueExpr;
    // valueExpr is initialized to NULL implicitly by FdoPtr<>, so if reader->IsNull() is true, propValue's value will be set to NULL.

    if (!reader->IsNull(propName))
    {
        if (FdoPropertyType_DataProperty == propType)
        {
            switch (dataType)
            {
            case FdoDataType_Boolean :
                if (reader->IsNull(propName))
                    valueExpr = FdoBooleanValue::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoBooleanValue::Create(reader->GetBoolean(propName));
            break;

            case FdoDataType_Byte :
                if (reader->IsNull(propName))
                    valueExpr = FdoByteValue::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoByteValue::Create(reader->GetByte(propName));
            break;

            case FdoDataType_DateTime :
                if (reader->IsNull(propName))
                    valueExpr = FdoDateTimeValue::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoDateTimeValue::Create(reader->GetDateTime(propName));
            break;

            case FdoDataType_Decimal :
                if (reader->IsNull(propName))
                    valueExpr = FdoDecimalValue::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoDecimalValue::Create(reader->GetDouble(propName));
            break;

            case FdoDataType_Single :
                if (reader->IsNull(propName))
                    valueExpr = FdoSingleValue::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoSingleValue::Create(reader->GetSingle(propName));
            break;

            case FdoDataType_Double :
                if (reader->IsNull(propName))
                    valueExpr = FdoDoubleValue::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoDoubleValue::Create(reader->GetDouble(propName));
            break;

            case FdoDataType_Int16 : 
                if (reader->IsNull(propName))
                    valueExpr = FdoInt16Value::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoInt16Value::Create(reader->GetInt16(propName));
            break;

            case FdoDataType_Int32 : 
                if (reader->IsNull(propName))
                    valueExpr = FdoInt32Value::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoInt32Value::Create(reader->GetInt32(propName));
            break;

            case FdoDataType_Int64 : 
                if (reader->IsNull(propName))
                    valueExpr = FdoInt64Value::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoInt64Value::Create(reader->GetInt64(propName));
            break;

            case FdoDataType_String : 
                if (reader->IsNull(propName))
                    valueExpr = FdoStringValue::Create();  // defaults to IsNull()==true
                else
                    valueExpr = FdoStringValue::Create(reader->GetString(propName));
            break;

            default: 
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED),
                    FdoCommonMiscUtil::FdoDataTypeToString(dataType)));
            }
        }
        else if (FdoPropertyType_GeometricProperty == propType)
        {
                if (reader->IsNull(propName))
                    valueExpr = FdoGeometryValue::Create();  // defaults to IsNull()==true
                else
                {
                    FdoPtr<FdoByteArray> byteArray = reader->GetGeometry(propName);
                    valueExpr = FdoGeometryValue::Create(byteArray);
                }
        }
        else
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_70_PROPERTY_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoPropertyTypeToString(propType)));
    }

    propValue->SetValue(valueExpr);

    return FDO_SAFE_ADDREF(propValue.p);
}



/// <summary> Returns the requested item in the given property value collection, or NULL if not found.</summary>
FdoPropertyValue* FdoCommonMiscUtil::GetItemNoThrow(FdoPropertyValueCollection *coll, FdoString* name)
{
    FdoPtr<FdoPropertyValue> propValue;
    bool bFound=false;
    for (FdoInt32 i=0; (i<coll->GetCount()) && !bFound; i++)
    {
        propValue = coll->GetItem(i);
        FdoPtr<FdoIdentifier> propertyId = propValue->GetName();
        bFound = (0==wcscmp(propertyId->GetName(), name));
    }

    if (bFound)
        return FDO_SAFE_ADDREF(propValue.p);
    else
        return NULL;
}



/// <summary> Handles read-only properties and default values in the given PropertyValue collection.</summary>
void FdoCommonMiscUtil::HandleReadOnlyAndDefaultValues(FdoClassDefinition *classDef, FdoPropertyValueCollection *propValues, bool bMakeNullsExplicit)
{
    FdoPtr<FdoPropertyDefinitionCollection> propertyDefs = classDef->GetProperties();

    for (FdoInt32 i=0; i<propertyDefs->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> propertyDef = propertyDefs->GetItem(i);
        bool bIsSet = false;
        bool bHasDefaultValue = false;
        bool bIsIdentityProperty = false;
        bool bIsReadOnly = false;

        // Get the corresponding property value, if any:
        FdoPtr<FdoPropertyValue> propValue;
        propValue = GetItemNoThrow(propValues, propertyDef->GetName());
        bIsSet = (propValue!=NULL) && FdoPtr<FdoValueExpression>(propValue->GetValue())!=NULL;

        // Determine whether this property is read only:
        if (propertyDef->GetPropertyType() == FdoPropertyType_DataProperty)
        {
            FdoDataPropertyDefinition* dataPropertyDef = static_cast<FdoDataPropertyDefinition*>(propertyDef.p);
            bIsReadOnly = dataPropertyDef->GetReadOnly();
        }

        // Handle this property:
        if (propertyDef->GetPropertyType() == FdoPropertyType_DataProperty)
        {
            FdoDataPropertyDefinition* dataPropertyDef = static_cast<FdoDataPropertyDefinition*>(propertyDef.p);
            bIsIdentityProperty = FdoCommonSchemaUtil::IsIdentityProperty(classDef, dataPropertyDef->GetName());

            FdoString *defaultValue = dataPropertyDef->GetDefaultValue();
            bHasDefaultValue = (defaultValue != NULL) && (wcslen(defaultValue) > 0);


            if (bIsReadOnly)
            {
                if (bIsSet)
                    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_97_CANNOT_SET_READONLY_PROPERTY, "Property '%1$ls' cannot be set because it is read-only.", propertyDef->GetName()));
                if (!bHasDefaultValue && !bIsIdentityProperty)
                    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_98_MISSING_DEFAULT_VALUE, "Read-only property '%1$ls' requires a default value.", propertyDef->GetName()));
                if (bHasDefaultValue && bIsIdentityProperty)
                    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_99_CANNOT_DEFAULT_READONLY_PROPERTY, "Read-only identity property '%1$ls' cannot have a default value.", propertyDef->GetName()));
            }

            if (!bIsSet && bHasDefaultValue)
            {
                if (propValue != NULL)
                    propValue->SetValue(defaultValue);
                else
                {
                    propValue = FdoPropertyValue::Create();
                    propValue->SetName(dataPropertyDef->GetName());
                    propValue->SetValue(defaultValue);
                    propValues->Add(propValue);
                }
            }
        }

        // Convert implicit NULLs into explicit NULLs if requested:
        if (bMakeNullsExplicit && !bIsReadOnly && !bIsSet && (propValue==NULL))
        {
            propValue = FdoPropertyValue::Create();
            propValue->SetName(propertyDef->GetName());
            propValue->SetValue((FdoValueExpression*)NULL);
            propValues->Add(propValue);
        }
    }

    // Validate that all given property values have valid property names:
    for (FdoInt32 i=0; i<propValues->GetCount(); i++)
    {
        FdoPtr<FdoPropertyValue> propVal = propValues->GetItem(i);
        FdoPtr<FdoIdentifier> propValId = propVal->GetName();
        // NOTE: we can skip checking the base properties since this provider currently doesn't support inheritance
        FdoPtr<FdoPropertyDefinition> propertyDef = propertyDefs->FindItem(propValId->GetName());
        if (propertyDef == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_74_PROPERTY_NAME_NOT_FOUND, "The property '%1$ls' was not found.", propValId->GetName()));
    }
}


void FdoCommonMiscUtil::ThrowPropertyConstraintException(FdoDataPropertyDefinition* dataProperty, FdoDataValue* dataValue)
{
    FdoPtr<FdoPropertyValueConstraint> valueConstraint = dataProperty->GetValueConstraint();
    switch (valueConstraint->GetConstraintType())
    {
        case FdoPropertyValueConstraintType_Range:
        {
            FdoPropertyValueConstraintRange* rangeConstraint = static_cast<FdoPropertyValueConstraintRange*>(valueConstraint.p);
            FdoStringP rangeString = FdoStringP::Format(L"%ls %ls N %ls %ls",
                FdoPtr<FdoDataValue>(rangeConstraint->GetMinValue())->ToString(),
                rangeConstraint->GetMinInclusive() ? L"<=" : L"<",
                rangeConstraint->GetMaxInclusive() ? L"<=" : L"<",
                FdoPtr<FdoDataValue>(rangeConstraint->GetMaxValue())->ToString() );
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_111_PROPERTY_RANGE_CONSTRAINT_VIOLATED), dataProperty->GetName(), (FdoString*)rangeString, dataValue->ToString()));
        }
        break;

        case FdoPropertyValueConstraintType_List:
        {
            FdoPropertyValueConstraintList* listConstraint = static_cast<FdoPropertyValueConstraintList*>(valueConstraint.p);
            FdoStringsP listStringCollection;
            FdoPtr<FdoDataValueCollection> listValues = listConstraint->GetConstraintList();
            for (int i=0; i<listValues->GetCount(); i++)
                listStringCollection->Add(FdoPtr<FdoDataValue>(listValues->GetItem(i))->ToString());
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_112_PROPERTY_LIST_CONSTRAINT_VIOLATED), dataProperty->GetName(), (FdoString*)listStringCollection->ToString(), dataValue->ToString()));
        }
        break;

        default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_113_PROPERTY_UNKNOWN_CONSTRAINT_VIOLATED), dataProperty->GetName(), dataValue->ToString()));
        break;
    }
}
