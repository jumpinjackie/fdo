// 
// Copyright (C) 2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 

#include "stdafx.h"
#include "strToFdoType.h"

FdoClassType
StrToFdoType::convStrToFdoClassType(const char * classType) {
	if (strcmp(classType, "FdoClassType_Class") == 0) {
		return FdoClassType_Class;
	} else if (strcmp(classType, "FdoClassType_FeatureClass") == 0) {
		return FdoClassType_FeatureClass;
	} else if (strcmp(classType, "FdoClassType_NetworkClass") == 0) {
		return FdoClassType_NetworkClass;
	} else if (strcmp(classType, "FdoClassType_NetworkLayerClass") == 0) {
		return FdoClassType_NetworkLayerClass;
	} else if (strcmp(classType, "FdoClassType_NetworkNodeClass") == 0) {
		return FdoClassType_NetworkNodeClass;
	} else if (strcmp(classType, "FdoClassType_NetworkLinkClass") == 0) {
		return FdoClassType_NetworkLinkClass;
	} else {
		sprintf(errString, "ERROR unknown FdoClassType: %s", classType);
		throw(errString);
	}
}

FdoPropertyType
StrToFdoType::convStrToFdoPropertyType(const char * propertyType) {
	if (strcmp(propertyType, "FdoPropertyType_DataProperty") == 0) {
		return FdoPropertyType_DataProperty;
	} else if (strcmp(propertyType, "FdoPropertyType_ObjectProperty") == 0) {
		return FdoPropertyType_ObjectProperty;
	} else if (strcmp(propertyType, "FdoPropertyType_GeometricProperty") == 0) {
		return FdoPropertyType_GeometricProperty;
	} else if (strcmp(propertyType, "FdoPropertyType_AssociationProperty") == 0) {
		return FdoPropertyType_AssociationProperty;
	} else if (strcmp(propertyType, "FdoPropertyType_RasterProperty") == 0) {
		return FdoPropertyType_RasterProperty;
	} else {
		sprintf(errString, "ERROR unknown FdoPropertyType: %s", propertyType);
		throw(errString);
	}
}

FdoDataType
StrToFdoType::convStrToFdoDataType(const char * dataType) {
	if (strcmp(dataType, "FdoDataType_Boolean") == 0) {
		return FdoDataType_Boolean;
	} else if (strcmp(dataType, "FdoDataType_Byte") == 0) {
		return FdoDataType_Byte;
	} else if (strcmp(dataType, "FdoDataType_DateTime") == 0) {
		return FdoDataType_DateTime;
	} else if (strcmp(dataType, "FdoDataType_Decimal") == 0) {
		return FdoDataType_Decimal;
	} else if (strcmp(dataType, "FdoDataType_Double") == 0) {
		return FdoDataType_Double;
	} else if (strcmp(dataType, "FdoDataType_Int16") == 0) {
		return FdoDataType_Int16;
	} else if (strcmp(dataType, "FdoDataType_Int32") == 0) {
		return FdoDataType_Int32;
	} else if (strcmp(dataType, "FdoDataType_Int64") == 0) {
		return FdoDataType_Int64;
	} else if (strcmp(dataType, "FdoDataType_Single") == 0) {
		return FdoDataType_Single;
	} else if (strcmp(dataType, "FdoDataType_String") == 0) {
		return FdoDataType_String;
	} else if (strcmp(dataType, "FdoDataType_BLOB") == 0) {
		return FdoDataType_BLOB;
	} else if (strcmp(dataType, "FdoDataType_CLOB") == 0) {
		return FdoDataType_CLOB;
	} else {
		sprintf(errString, "ERROR unknown FdoDataType: %s", dataType);
		throw(errString);
	}
}

FdoGeometricType
StrToFdoType::convStrToFdoGeometricType(const char * geometricType) {
	if (strcmp(geometricType, "FdoGeometricType_Point") == 0) {
		return FdoGeometricType_Point;
	} else if (strcmp(geometricType, "FdoGeometricType_Curve") == 0) {
		return FdoGeometricType_Curve;
	} else if (strcmp(geometricType, "FdoGeometricType_Surface") == 0) {
		return FdoGeometricType_Surface;
	} else if (strcmp(geometricType, "FdoGeometricType_Solid") == 0) {
		return FdoGeometricType_Solid;
	} else {
		sprintf(errString, "ERROR unknown FdoGeometricType: %s", geometricType);
		throw(errString);
	}
}

FdoGeometryType
StrToFdoType::convStrToFdoGeometryType(const char * geometryType) {
	if (strcmp(geometryType, "FdoGeometryType_None") == 0) {
		return FdoGeometryType_None;
	} else if (strcmp(geometryType, "FdoGeometryType_Point") == 0) {
		return FdoGeometryType_Point;
	} else if (strcmp(geometryType, "FdoGeometryType_LineString") == 0) {
		return FdoGeometryType_LineString;
	} else if (strcmp(geometryType, "FdoGeometryType_Polygon") == 0) {
		return FdoGeometryType_Polygon;
	} else if (strcmp(geometryType, "FdoGeometryType_MultiPoint") == 0) {
		return FdoGeometryType_MultiPoint;
	} else if (strcmp(geometryType, "FdoGeometryType_MultiLineString") == 0) {
		return FdoGeometryType_MultiLineString;
	} else if (strcmp(geometryType, "FdoGeometryType_MultiPolygon") == 0) {
		return FdoGeometryType_MultiPolygon;
	} else if (strcmp(geometryType, "FdoGeometryType_MultiGeometry") == 0) {
		return FdoGeometryType_MultiGeometry;
	} else if (strcmp(geometryType, "FdoGeometryType_CurveString") == 0) {
		return FdoGeometryType_CurveString;
	} else if (strcmp(geometryType, "FdoGeometryType_CurvePolygon") == 0) {
		return FdoGeometryType_CurvePolygon;
	} else if (strcmp(geometryType, "FdoGeometryType_MultiCurveString") == 0) {
		return FdoGeometryType_MultiCurveString;
	} else if (strcmp(geometryType, "FdoGeometryType_MultiCurvePolygon") == 0) {
		return FdoGeometryType_MultiCurvePolygon;
	} else {
		sprintf(errString, "ERROR unknown FdoGeometryType: %s", geometryType);
		throw(errString);
	}
}



