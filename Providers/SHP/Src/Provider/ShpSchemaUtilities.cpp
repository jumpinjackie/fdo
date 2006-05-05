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
#include "ShpSchemaUtilities.h"

#include <FdoCommonStringUtil.h>




FdoDataType ShpSchemaUtilities::DbfTypeToFdoType (int physicalColumnType)
{
    FdoDataType logicalDataType;

    switch (physicalColumnType)
    {
        case kColumnDecimalType:
            logicalDataType = FdoDataType_Decimal;
            break;
        case kColumnCharType:
            logicalDataType = FdoDataType_String;
            break;
        case kColumnDateType:
            logicalDataType = FdoDataType_DateTime;
            break;
        case kColumnLogicalType:
            logicalDataType = FdoDataType_Boolean;
            break;
        default:
            throw FdoSchemaException::Create(NlsMsgGet(SHP_UNKNOWN_COLUMN_TYPE, "The '%1$d' column type is unknown.", physicalColumnType));
    }

    return (logicalDataType);
}

eDBFColumnType ShpSchemaUtilities::FdoTypeToDbfType (FdoDataType logicalPropertyType)
{
    eDBFColumnType physicalColumnType;

    switch (logicalPropertyType)
    {
        case FdoDataType_Decimal:
        case FdoDataType_Int32:
            physicalColumnType = kColumnDecimalType;
            break;
        case FdoDataType_String:
            physicalColumnType = kColumnCharType;
            break;
        case FdoDataType_DateTime:
            physicalColumnType = kColumnDateType;
            break;
        case FdoDataType_Boolean:
            physicalColumnType = kColumnLogicalType;
            break;
        default:
            physicalColumnType = kColumnUnsupportedType;
    }

    return (physicalColumnType);
}

// Warning:
// ePointZShape, ePointMShape, ePointShape are never returned
// so don't use this method for anything except ApplySchema,
// these are handled on first write to the new file
// What about eMultiPatchShape though?
eShapeTypes ShpSchemaUtilities::FdoGeometryToShapeType (FdoInt32 geometry_types, bool has_elevation, bool has_measure)
{
    eShapeTypes physicalShapeType;

    switch (geometry_types)
    {
        case FdoGeometricType_Point:
            if (has_elevation)
                physicalShapeType = eMultiPointZShape;
            else if (has_measure)
                physicalShapeType = eMultiPointMShape;
            else
                physicalShapeType = eMultiPointShape;
            break;
        case FdoGeometricType_Curve:
            if (has_elevation)
                physicalShapeType = ePolylineZShape;
            else if (has_measure)
                physicalShapeType = ePolylineMShape;
            else
                physicalShapeType = ePolylineShape;
            break;
        case FdoGeometricType_Surface:
            if (has_elevation)
                physicalShapeType = ePolygonZShape;
            else if (has_measure)
                physicalShapeType = ePolygonMShape;
            else
                physicalShapeType = ePolygonShape;
            break;
        case FdoGeometricType_Solid:
        default:
            {
                wchar_t buffer[1024];

                buffer[0] = '\0';
                if (0 != (geometry_types & FdoGeometricType_Point))
                {
                    if ('\0' != buffer[0])
                        wcscat (buffer, L"|");
                    wcscat (buffer, L"FdoGeometricType_Point");
                }
                if (0 != (geometry_types & FdoGeometricType_Curve))
                {
                    if ('\0' != buffer[0])
                        wcscat (buffer, L"|");
                    wcscat (buffer, L"FdoGeometricType_Curve");
                }
                if (0 != (geometry_types & FdoGeometricType_Surface))
                {
                    if ('\0' != buffer[0])
                        wcscat (buffer, L"|");
                    wcscat (buffer, L"FdoGeometricType_Surface");
                }
                if (0 != (geometry_types & FdoGeometricType_Solid))
                {
                    if ('\0' != buffer[0])
                        wcscat (buffer, L"|");
                    wcscat (buffer, L"FdoGeometricType_Solid");
                }
                throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_SHAPE, "The '%1$ls' geometry type is not supported by Shp.", buffer));
            }
    }

    return (physicalShapeType);
}


// Returns the requested logical class definition, paring it down to the given property list
FdoClassDefinition* ShpSchemaUtilities::GetLogicalClassDefinition (ShpConnection* connection, FdoString* logicalClassName, FdoIdentifierCollection* propertySubset)
{
    // Get logical class:
    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition(connection, logicalClassName);
    FdoPtr<FdoClassDefinition> logicalClass = lpClass->GetLogicalClass();
    VALIDATE_POINTER(logicalClass);

    // Create copy of logical class, using given propertySubset:
    FdoPtr<FdoCommonSchemaCopyContext> copyContext = FdoCommonSchemaCopyContext::Create(propertySubset);
    FdoPtr<FdoClassDefinition> logicalClassCopy = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(logicalClass, copyContext);

    // Add any computed properties to cloned logical class as properties:
    FdoCommonSchemaUtil::AddComputedIdentifiersAsProperties(connection, logicalClassCopy, logicalClass, propertySubset);

    return FDO_SAFE_ADDREF(logicalClassCopy.p);
}


ShpLpClassDefinition* ShpSchemaUtilities::GetLpClassDefinition(ShpConnection* connection, FdoString* logicalQualifiedClassName)
{
    VALIDATE_ARGUMENT(logicalQualifiedClassName);
    FdoPtr<FdoIdentifier> logicalClassIdentifier = FdoIdentifier::Create(logicalQualifiedClassName);

    FdoPtr<ShpLpClassDefinition> pRequestedLpClass;

    // Validate the given identifier:
    VALIDATE_ARGUMENT(logicalClassIdentifier);
    FdoInt32 scopeAmount = 0;
    FdoString **scopeNames = logicalClassIdentifier->GetScope(scopeAmount);
    if (scopeAmount > 0)
        throw FdoException::Create(NlsMsgGet(SHP_CLASS_IDENTIFIER_SCOPES_NOT_SUPPORTED, "Fdo Class Identifiers that contain scopes are not supported."));

    // Split the given logical class identifier into schema name and class name:
    FdoString *logicalSchemaName = logicalClassIdentifier->GetSchemaName();
    FdoString *logicalClassName = logicalClassIdentifier->GetName();

    // Search the LogicalPhysical schemas for the requested LogicalPhysical class:
    FdoPtr<ShpLpFeatureSchemaCollection> pLpSchemaColl = connection->GetLpSchema(logicalSchemaName);
    VALIDATE_POINTER(pLpSchemaColl);
    for (int i=0; i<pLpSchemaColl->GetCount(); i++)
    {
        FdoPtr<ShpLpFeatureSchema> pLpSchema = pLpSchemaColl->GetItem(i);
        VALIDATE_POINTER(pLpSchema);

        if ((logicalSchemaName==NULL) || (wcslen(logicalSchemaName)==0) || (wcscmp(logicalSchemaName, pLpSchema->GetName()) == 0))
        {
            FdoPtr<ShpLpClassDefinitionCollection> pLpClasses = pLpSchema->GetLpClasses();
            VALIDATE_POINTER(pLpClasses);

            FdoPtr<ShpLpClassDefinition> pLpClass = pLpClasses->FindItem(logicalClassName);
            if (pLpClass != NULL)
            {
                // Validate that this classname is unique (if it hasn't been qualified with a schema name):
                if (pRequestedLpClass != NULL)
                    throw FdoException::Create(NlsMsgGet(SHP_CLASS_NAME_NOT_UNIQUE, "The unqualified FDO class name '%1$ls' is not unique across all FDO schemas.", logicalClassName));

                pRequestedLpClass = pLpClass;
            }
        }
    }

    if (pRequestedLpClass == NULL) {
        throw FdoSchemaException::Create(NlsMsgGet(SHP_FEATURE_CLASS_NOT_FOUND, "FDO class '%1$ls' not found in schema.", logicalClassName));
    }

    FDO_SAFE_ADDREF(pRequestedLpClass.p);
    return pRequestedLpClass.p;
}


// Returns the physical column name corresponding to the requested logical property name:
FdoString* ShpSchemaUtilities::GetPhysicalColumnName(ShpConnection* connection, FdoString* logicalClassName, FdoString* logicalPropertyName)
{
    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition(connection, logicalClassName);
    FdoPtr<ShpLpPropertyDefinitionCollection> lpProperties = lpClass->GetLpProperties();
    FdoPtr<ShpLpPropertyDefinition> lpProperty = lpProperties->GetItem(logicalPropertyName);
    return lpProperty->GetPhysicalColumnName();
}

FdoStringP ShpSchemaUtilities::GetIdentityPropertyName (ShpConnection* connection, FdoString* class_name)
{
    FdoPtr<FdoClassDefinition> classDef = GetLogicalClassDefinition (connection, class_name, NULL);
    FdoPtr<FdoDataPropertyDefinitionCollection> idProps = classDef->GetIdentityProperties ();
    FdoPtr<FdoDataPropertyDefinition> idProp = idProps->GetItem (0);
    return (idProp->GetName ());
}

