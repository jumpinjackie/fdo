// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#include <Fdo/Schema/GeometricPropertyDefinition.h>
#include <Fdo/Schema/GeometricType.h>
#include "XmlContext.h"



enum FdoGeometryTypeHexCode
{
    FdoGeometryTypeHexCode_None              = 0x00020,
    FdoGeometryTypeHexCode_Point             = 0x00040,
    FdoGeometryTypeHexCode_LineString        = 0x00080,
    FdoGeometryTypeHexCode_Polygon           = 0x00100,
    FdoGeometryTypeHexCode_MultiPoint        = 0x00200,
    FdoGeometryTypeHexCode_MultiLineString   = 0x00400,
    FdoGeometryTypeHexCode_MultiPolygon      = 0x00800,
    FdoGeometryTypeHexCode_MultiGeometry     = 0x01000,
    FdoGeometryTypeHexCode_CurveString       = 0x02000,
    FdoGeometryTypeHexCode_CurvePolygon      = 0x04000,
    FdoGeometryTypeHexCode_MultiCurveString  = 0x08000,
    FdoGeometryTypeHexCode_MultiCurvePolygon = 0x10000,
};


static const FdoInt32 All_GEOMETRY_POINT               = (FdoGeometryTypeHexCode_Point | FdoGeometryTypeHexCode_MultiPoint);
static const FdoInt32 ALL_GEOMETRY_CURVE               = (FdoGeometryTypeHexCode_LineString | FdoGeometryTypeHexCode_MultiLineString | FdoGeometryTypeHexCode_CurveString | FdoGeometryTypeHexCode_MultiCurveString);
static const FdoInt32 ALL_GEOMETRY_SURFACE             = (FdoGeometryTypeHexCode_Polygon | FdoGeometryTypeHexCode_MultiPolygon | FdoGeometryTypeHexCode_CurvePolygon | FdoGeometryTypeHexCode_MultiCurvePolygon);
static const FdoInt32 ALL_GEOMETRY_POINT_CURVE         = (All_GEOMETRY_POINT | ALL_GEOMETRY_CURVE);
static const FdoInt32 ALL_GEOMETRY_POINT_SURFACE       = (All_GEOMETRY_POINT | ALL_GEOMETRY_SURFACE);
static const FdoInt32 ALL_GEOMETRY_CURVE_SURFACE       = (ALL_GEOMETRY_CURVE | ALL_GEOMETRY_SURFACE);
static const FdoInt32 ALL_GEOMETRY_POINT_CURVE_SURFACE = (All_GEOMETRY_POINT | ALL_GEOMETRY_CURVE | ALL_GEOMETRY_SURFACE);



FdoGeometricPropertyDefinition *FdoGeometricPropertyDefinition::Create()
{
    return new FdoGeometricPropertyDefinition();
}

FdoGeometricPropertyDefinition *FdoGeometricPropertyDefinition::Create(FdoString *name, FdoString *description, bool system)
{
    return new FdoGeometricPropertyDefinition(name, description, system);
}

FdoGeometricPropertyDefinition::~FdoGeometricPropertyDefinition()
{
}

void FdoGeometricPropertyDefinition::Dispose()
{
    delete this;
}

FdoGeometricPropertyDefinition::FdoGeometricPropertyDefinition() : FdoPropertyDefinition()
{
    SetToZero();
}

FdoGeometricPropertyDefinition::FdoGeometricPropertyDefinition(FdoString *name, FdoString *description, bool system) :
    FdoPropertyDefinition(name,description, system)
{
    SetToZero();
}

FdoInt32 FdoGeometricPropertyDefinition::GetGeometricType(int pos)
{
    switch (pos) 
    {
        case 0:  return FdoGeometricType_Point;   break;
        case 1:  return FdoGeometricType_Curve;   break;
        case 2:  return FdoGeometricType_Surface; break;
        case 3:  return FdoGeometricType_Solid;   break;
        default: return -1;                       break;
    }
}

FdoInt32 FdoGeometricPropertyDefinition::MapGeometryTypeToHexCode(FdoGeometryType gType)
{
    switch (gType)
    {
        case FdoGeometryType_None:              return FdoGeometryTypeHexCode_None;              break;
        case FdoGeometryType_Point:             return FdoGeometryTypeHexCode_Point;             break;
        case FdoGeometryType_LineString:        return FdoGeometryTypeHexCode_LineString;        break;
        case FdoGeometryType_Polygon:           return FdoGeometryTypeHexCode_Polygon;           break;
        case FdoGeometryType_MultiPoint:        return FdoGeometryTypeHexCode_MultiPoint;        break;
        case FdoGeometryType_MultiLineString:   return FdoGeometryTypeHexCode_MultiLineString;   break;
        case FdoGeometryType_MultiPolygon:      return FdoGeometryTypeHexCode_MultiPolygon;      break;
        case FdoGeometryType_MultiGeometry:     return FdoGeometryTypeHexCode_MultiGeometry;     break;
        case FdoGeometryType_CurveString:       return FdoGeometryTypeHexCode_CurveString;       break;
        case FdoGeometryType_CurvePolygon:      return FdoGeometryTypeHexCode_CurvePolygon;      break;
        case FdoGeometryType_MultiCurveString:  return FdoGeometryTypeHexCode_MultiCurveString;  break;
        case FdoGeometryType_MultiCurvePolygon: return FdoGeometryTypeHexCode_MultiCurvePolygon; break;
        default: throw FdoException::Create (FdoException::NLSGetMessage(
                                                    FDO_NLSID(FDO_128_GEOMETRY_MAPPING_ERROR))); break;
    }
}

FdoInt32 FdoGeometricPropertyDefinition::MapGeometryTypeToHexCode(int pos)
{
    switch (pos)
    {
        case  0: return FdoGeometryTypeHexCode_None;              break;
        case  1: return FdoGeometryTypeHexCode_Point;             break;
        case  2: return FdoGeometryTypeHexCode_LineString;        break;
        case  3: return FdoGeometryTypeHexCode_Polygon;           break;
        case  4: return FdoGeometryTypeHexCode_MultiPoint;        break;
        case  5: return FdoGeometryTypeHexCode_MultiLineString;   break;
        case  6: return FdoGeometryTypeHexCode_MultiPolygon;      break;
        case  7: return FdoGeometryTypeHexCode_MultiGeometry;     break;
        case  8: return FdoGeometryTypeHexCode_CurveString;       break;
        case  9: return FdoGeometryTypeHexCode_CurvePolygon;      break;
        case 10: return FdoGeometryTypeHexCode_MultiCurveString;  break;
        case 11: return FdoGeometryTypeHexCode_MultiCurvePolygon; break;
        default: throw FdoException::Create (FdoException::NLSGetMessage(
                                                    FDO_NLSID(FDO_128_GEOMETRY_MAPPING_ERROR))); break;
    }
}

FdoGeometryType FdoGeometricPropertyDefinition::MapHexCodeToGeometryType(FdoInt32 gTypeHexCode)
{
    switch (gTypeHexCode)
    {
        case FdoGeometryTypeHexCode_None:              return FdoGeometryType_None;              break;
        case FdoGeometryTypeHexCode_Point:             return FdoGeometryType_Point;             break;
        case FdoGeometryTypeHexCode_LineString:        return FdoGeometryType_LineString;        break;
        case FdoGeometryTypeHexCode_Polygon:           return FdoGeometryType_Polygon;           break;
        case FdoGeometryTypeHexCode_MultiPoint:        return FdoGeometryType_MultiPoint;        break;
        case FdoGeometryTypeHexCode_MultiLineString:   return FdoGeometryType_MultiLineString;   break;
        case FdoGeometryTypeHexCode_MultiPolygon:      return FdoGeometryType_MultiPolygon;      break;
        case FdoGeometryTypeHexCode_MultiGeometry:     return FdoGeometryType_MultiGeometry;     break;
        case FdoGeometryTypeHexCode_CurveString:       return FdoGeometryType_CurveString;       break;
        case FdoGeometryTypeHexCode_CurvePolygon:      return FdoGeometryType_CurvePolygon;      break;
        case FdoGeometryTypeHexCode_MultiCurveString:  return FdoGeometryType_MultiCurveString;  break;
        case FdoGeometryTypeHexCode_MultiCurvePolygon: return FdoGeometryType_MultiCurvePolygon; break;
        default: throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(
                                                              FDO_128_GEOMETRY_MAPPING_ERROR))); break;
    }
}

void FdoGeometricPropertyDefinition::CacheGeometryTypes()
{ 
    FdoInt32 currTypeHexCode;

    // Based on the current geometry type value fill the corresponding vector with the
    // selected geometry types.
    m_typesSize = 0;
    for (int i = 0; i < MAX_GEOMETRY_TYPE_SIZE; i++)
    {
        currTypeHexCode = MapGeometryTypeToHexCode(i);
        if ((m_geometryTypes & currTypeHexCode) > 0)
            m_types[m_typesSize++] = MapHexCodeToGeometryType(currTypeHexCode);
    }
}

FdoPropertyType FdoGeometricPropertyDefinition::GetPropertyType()
{
    return FdoPropertyType_GeometricProperty;
}

FdoGeometryType *FdoGeometricPropertyDefinition::GetSpecificGeometryTypes(FdoInt32 &length)
{
    if (m_cacheGeometryTypes)
    {
        CacheGeometryTypes();
        m_cacheGeometryTypes = false;
    }

    length = m_typesSize;
    return m_types;
}

FdoInt32 FdoGeometricPropertyDefinition::GetSpecificGeometryTypes()
{
    return m_geometryTypes;
}

void FdoGeometricPropertyDefinition::SetSpecificGeometryTypes(FdoGeometryType *types, FdoInt32 length)
{
    bool     areaRepFound  = false;
    bool     lineRepFound  = false;
    bool     pointRepFound = false;
    FdoInt32 typeRep;

    InitGeometryTypes();
    m_geometryTypes = 0x00000;
    _StartChanges();

    // Setting the geometry type triggers an automated update of the geometric type. The
    // following sets the geometry types. While navigating through the list of given geometry
    // types determine the geometric type that needs to be set.
    for (int i = 0; i < length; i++)
    {

        if ((types[i] == FdoGeometryType_Point     ) ||
            (types[i] == FdoGeometryType_MultiPoint)    )
             pointRepFound = true;

        if ((types[i] == FdoGeometryType_LineString      ) ||
            (types[i] == FdoGeometryType_MultiLineString ) ||
            (types[i] == FdoGeometryType_CurveString     ) ||
            (types[i] == FdoGeometryType_MultiCurveString)    )
             lineRepFound = true;

        if ((types[i] == FdoGeometryType_Polygon          ) ||
            (types[i] == FdoGeometryType_MultiPolygon     ) ||
            (types[i] == FdoGeometryType_CurvePolygon     ) ||
            (types[i] == FdoGeometryType_MultiCurvePolygon)    )
             areaRepFound = true;

        if (types[i] == FdoGeometryType_MultiGeometry)
            areaRepFound = lineRepFound = pointRepFound = true;

        typeRep = MapGeometryTypeToHexCode(types[i]);
        m_geometryTypes = m_geometryTypes | typeRep;
    }

    // The following sets the geometric type based on the findings when
    // navigating the list of provided geometry types.
    m_geometricTypes = 0x00;
    if (areaRepFound)
        m_geometricTypes = m_geometricTypes | FdoGeometricType_Surface;
    if (lineRepFound)
        m_geometricTypes = m_geometricTypes | FdoGeometricType_Curve;
    if (pointRepFound)
        m_geometricTypes = m_geometricTypes | FdoGeometricType_Point;

    if ((m_geometryTypes != m_geometryTypesCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged))
        SetElementState(FdoSchemaElementState_Modified);

    if ((m_geometricTypes != m_geometricTypesCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged))
        SetElementState(FdoSchemaElementState_Modified);
}

FdoInt32 FdoGeometricPropertyDefinition::GetGeometryTypes()
{ 
    return m_geometricTypes;
}

void FdoGeometricPropertyDefinition::SetGeometryTypes(FdoInt32 value)
{
    FdoInt32 currGeometricType;

    InitGeometryTypes();
    _StartChanges();

    // Setting the geometric type automatically triggers the setting of the
    // corresponding geometric types. The following sets the given geometric type.
    m_geometricTypes = value;

    // The following sets the corresponding geometry types.

    m_geometryTypes = 0x00000;

    for (int i = 0; i < MAX_GEOMETRIC_TYPE_SIZE; i++)
    {
        currGeometricType = GetGeometricType(i);
        if (currGeometricType != -1)
        {
            if ((m_geometricTypes & currGeometricType) > 0)
            {
                switch (currGeometricType)
                {
                    case FdoGeometricType_Point:
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_Point);
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiPoint);
                        break;
                    case FdoGeometricType_Curve:
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_LineString);
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiLineString);
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_CurveString);
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiCurveString);
                        break;
                    case FdoGeometricType_Surface:
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_Polygon);
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiPolygon);
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_CurvePolygon);
                        m_geometryTypes = m_geometryTypes | MapGeometryTypeToHexCode(FdoGeometryType_MultiCurvePolygon);
                        break;
                }
            }
        }
    }

    if ((m_geometryTypes != m_geometryTypesCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged))
        SetElementState(FdoSchemaElementState_Modified);

    if ((m_geometricTypes != m_geometricTypesCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged))
        SetElementState(FdoSchemaElementState_Modified);
}

bool FdoGeometricPropertyDefinition::GetReadOnly()
{
    return m_readOnly;
}

void FdoGeometricPropertyDefinition::SetReadOnly(bool value)
{
    _StartChanges();
    m_readOnly = value;
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoGeometricPropertyDefinition::GetHasElevation()
{
    return m_hasElevation;
}

void FdoGeometricPropertyDefinition::SetHasElevation(bool value)
{
    _StartChanges();
    m_hasElevation = value;
    if ((m_hasElevation != m_hasElevationCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged))
        SetElementState(FdoSchemaElementState_Modified);
}

bool FdoGeometricPropertyDefinition::GetHasMeasure()
{
    return m_hasMeasure;
}

void FdoGeometricPropertyDefinition::SetHasMeasure(bool value)
{
    _StartChanges();
    m_hasMeasure = value;
    if ((m_hasMeasure != m_hasMeasureCHANGED) || (GetElementState() != FdoSchemaElementState_Unchanged))
        SetElementState(FdoSchemaElementState_Modified);
}

void FdoGeometricPropertyDefinition::SetSpatialContextAssociation(FdoString *value)
{
    _StartChanges();
    m_associatedSCName = value;
    if ((m_associatedSCName.ICompare(m_associatedSCNameCHANGED) != 0) || (GetElementState() != FdoSchemaElementState_Unchanged))
        SetElementState(FdoSchemaElementState_Modified);
}

FdoString *FdoGeometricPropertyDefinition::GetSpatialContextAssociation()
{
    return (m_associatedSCName.ICompare(L"") == 0) ? (FdoString *) NULL : (FdoString *) m_associatedSCName;
}

void FdoGeometricPropertyDefinition::_StartChanges()
{
    m_allDefaults = false;

    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoPropertyDefinition::_StartChanges();

        m_geometryTypesCHANGED = m_geometryTypes;
        m_geometricTypesCHANGED = m_geometricTypes;
        m_readOnlyCHANGED = m_readOnly;
        m_hasElevationCHANGED = m_hasElevation;
        m_hasMeasureCHANGED = m_hasMeasure;
		m_associatedSCNameCHANGED = m_associatedSCName;
    }
}

void FdoGeometricPropertyDefinition::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        m_geometryTypes = m_geometryTypesCHANGED;
        m_geometricTypes = m_geometricTypesCHANGED;
        m_readOnly = m_readOnlyCHANGED;
        m_hasElevation = m_hasElevationCHANGED;
        m_hasMeasure = m_hasMeasureCHANGED;
		m_associatedSCName = m_associatedSCNameCHANGED;
    }
}

void FdoGeometricPropertyDefinition::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_AcceptChanges();
}

void FdoGeometricPropertyDefinition::Set(FdoPropertyDefinition *pProperty, FdoSchemaMergeContext *pContext)
{
    FdoPropertyDefinition::Set(pProperty, pContext);

    // Base function catches property type mismatch so silently quit on type mismatch
    if ( GetPropertyType() == pProperty->GetPropertyType() ) {
        if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pProperty->GetElementState() == FdoSchemaElementState_Modified) ) {
            FdoGeometricPropertyDefinition* pGeomProperty = (FdoGeometricPropertyDefinition*) pProperty;

            // Set each member from the given geometric property. The same pattern is followed 
            // for each:
            //
            // If new and old values differ
            //    If modification allowed (always allowed if this is a new property).
            //      Perform the modification
            //    else
            //      log an error
            //    end if
            //  end if

            // Geometric Types
            if ( GetGeometryTypes() != pGeomProperty->GetGeometryTypes() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModGeomTypes(pGeomProperty)) ) 
                    SetGeometryTypes( pGeomProperty->GetGeometryTypes() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_99_MODGEOMTYPES),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Geometry Types
            if ( GetSpecificGeometryTypes() != pGeomProperty->GetSpecificGeometryTypes() ) {
            // A NEW FUNCTION TO REPLACE CALL TO CanModGeomTypes MAY BE REQUIRED???
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModGeomTypes(pGeomProperty)) ) {
                    FdoInt32 geomTypeSize = 0;
                    FdoGeometryType *geomType = pGeomProperty->GetSpecificGeometryTypes(geomTypeSize);
                    SetSpecificGeometryTypes( geomType, geomTypeSize );
                }
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_99_MODGEOMTYPES),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            //Elevation
            if ( GetHasElevation() != pGeomProperty->GetHasElevation() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModGeomElevation(pGeomProperty)) ) 
                    SetHasElevation( pGeomProperty->GetHasElevation() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_100_MODGEOMELEVATION),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Measure Dimension
            if ( GetHasMeasure() != pGeomProperty->GetHasMeasure() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModGeomMeasure(pGeomProperty)) ) 
                    SetHasMeasure( pGeomProperty->GetHasMeasure() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_101_MODGEOMMEASURE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Spatial Context
            if ( FdoStringP(GetSpatialContextAssociation()) != FdoStringP(pGeomProperty->GetSpatialContextAssociation()) ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModGeomSC(pGeomProperty)) ) 
                    SetSpatialContextAssociation( pGeomProperty->GetSpatialContextAssociation() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_102_MODPROPSC),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) FdoStringP(GetSpatialContextAssociation()),
                                    (FdoString*) FdoStringP(pGeomProperty->GetSpatialContextAssociation())
                                )
                            )
                        )
                    );
            }

            // ReadOnly setting
            if ( GetReadOnly() != GetReadOnly() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModGeomReadOnly(pGeomProperty)) ) 
                    SetReadOnly( pGeomProperty->GetReadOnly() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(pGeomProperty->GetReadOnly() ? SCHEMA_95_MODPROPRDONLY : SCHEMA_96_MODPROPWRITABLE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }
        }
    }
}

void FdoGeometricPropertyDefinition::InitFromXml(const FdoString *propertyTypeName, FdoSchemaXmlContext *pContext, FdoXmlAttributeCollection *attrs)
{
    // Error if FDO and XML property types are not the same.
    if (wcscmp(propertyTypeName, L"GeometricProperty") != 0) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_25_PROPERTYTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    // Initialize generic property parts
    FdoPropertyDefinition::InitFromXml(pContext, attrs );

    // Reset attributes to defaults
    m_geometryTypes = ALL_GEOMETRY_POINT_CURVE_SURFACE;
    m_geometricTypes = FdoGeometricType_All;
    m_readOnly = false;
    m_hasElevation = false;
    m_hasMeasure = false;
	m_associatedSCName = L"";
    m_allDefaults = true;

    // Set attributes from the XML attributes.
    FdoXmlAttributeP attr = attrs->FindItem(L"readOnly");
    if (attr != NULL)
        SetReadOnly(FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem(L"hasElevation");
    if (attr != NULL)
        SetHasElevation(FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem(L"hasMeasure");
    if (attr != NULL)
        SetHasMeasure(FdoStringP(attr->GetValue()).ToBoolean());

	attr = attrs->FindItem(L"associatedSCName");
    if (attr != NULL)
		SetSpatialContextAssociation(attr->GetValue());	
}

FdoXmlSaxHandler* FdoGeometricPropertyDefinition::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // Handle generic sub-elements
    FdoXmlSaxHandler* pRet = FdoSchemaElement::XmlStartElement(context, uri, name, qname, atts);
    FdoPtr<FdoPropertyDefinition> pProp;

    if (!pRet) {
        // Not generic, so handle here. Only Geometric types are handled.
        if (wcscmp(name, L"GeometricTypes") == 0) {
            // starting to read geometric types. Initialize the read
            m_XmlGeometricTypes = 0;
        }

        if (wcscmp(name, L"GeometricType") == 0) {

            // Set up a SAX handler to read the geometric type from the
            // element content.
            m_XmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = m_XmlContentHandler;
        }

        if (wcscmp(name, L"GeometryTypes") == 0) {
            // Sstarting to read geometric types. Initialize the geometry type array
            // by setting all values to -1 and the corresponding counter to 0.
            for (FdoInt32 i = 0; (i  < m_XmlGeometryTypesCount); m_XmlGeometryTypes[i] = (FdoGeometryType)-1, i++) ;
            m_XmlGeometryTypesCount = 0;
        }

        if (wcscmp(name, L"GeometryType") == 0) {

            // Set up a SAX handler to read the geometric type from the
            // element content.
            m_XmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = m_XmlContentHandler;
        }
    }

    return(pRet);
}

FdoBoolean FdoGeometricPropertyDefinition::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname 
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // End any generic sub-elements.
    FdoSchemaElement::XmlEndElement(context, uri, name, qname);

    if (wcscmp(name, L"GeometricTypes") == 0) {
        // all geometric types have been read so set the member.
        // set to all types if none were read.
        SetGeometryTypes(m_XmlGeometricTypes == 0 ? FdoGeometricType_All : m_XmlGeometricTypes);
    }

    if (wcscmp(name, L"GeometricType") == 0) {
        FdoStringP sGeomType = m_XmlContentHandler->GetString();

        // Convert the string to a bit
        if (sGeomType == L"point")
            m_XmlGeometricTypes |= FdoGeometricType_Point;
        else if (sGeomType == L"curve")
            m_XmlGeometricTypes |= FdoGeometricType_Curve;
        else if (sGeomType == L"surface")
            m_XmlGeometricTypes |= FdoGeometricType_Surface;
        else if (sGeomType == L"solid")
            m_XmlGeometricTypes |= FdoGeometricType_Solid;

    }

    if (wcscmp(name, L"GeometryTypes") == 0) {
        // All geometry types have been read so set the member. If no geometry
        // types were specified set all of them by default to be consistent with
        // the setting of the geometric types.
        if (m_XmlGeometryTypesCount == 0) {
            m_XmlGeometryTypes[0]   = FdoGeometryType_Point;
            m_XmlGeometryTypes[1]   = FdoGeometryType_LineString;
            m_XmlGeometryTypes[2]   = FdoGeometryType_Polygon;
            m_XmlGeometryTypes[3]   = FdoGeometryType_MultiPoint;
            m_XmlGeometryTypes[4]   = FdoGeometryType_MultiLineString;
            m_XmlGeometryTypes[5]   = FdoGeometryType_MultiPolygon;
            m_XmlGeometryTypes[6]   = FdoGeometryType_CurveString;
            m_XmlGeometryTypes[7]   = FdoGeometryType_CurvePolygon;
            m_XmlGeometryTypes[8]   = FdoGeometryType_MultiCurveString;
            m_XmlGeometryTypes[9]   = FdoGeometryType_MultiCurvePolygon;
            m_XmlGeometryTypes[10]  = FdoGeometryType_MultiGeometry;
            m_XmlGeometryTypesCount = 11;
        }
        SetSpecificGeometryTypes(m_XmlGeometryTypes, m_XmlGeometryTypesCount);
    }

    if (wcscmp(name, L"GeometryType") == 0) {
        FdoStringP sGeomType = m_XmlContentHandler->GetString();

        // Convert the string to the corresponding FDO geometry type and add
        // it to the collection of specified geometry types. Note that it has
        // to be ensured that the collection only contains one entry for the
        // specified type.
        // First, determine the corresponding FDO type for the value currently
        // read.
        FdoGeometryType currentGeometryType = FdoGeometryType_None;
        if (sGeomType == L"point")             currentGeometryType = FdoGeometryType_Point;
        if (sGeomType == L"multipoint")        currentGeometryType = FdoGeometryType_MultiPoint;
        if (sGeomType == L"linestring")        currentGeometryType = FdoGeometryType_LineString;
        if (sGeomType == L"multilinestring")   currentGeometryType = FdoGeometryType_MultiLineString;
        if (sGeomType == L"curvestring")       currentGeometryType = FdoGeometryType_CurveString;
        if (sGeomType == L"multicurvestring")  currentGeometryType = FdoGeometryType_MultiCurveString;
        if (sGeomType == L"polygon")           currentGeometryType = FdoGeometryType_Polygon;
        if (sGeomType == L"multipolygon")      currentGeometryType = FdoGeometryType_MultiPolygon;
        if (sGeomType == L"curvepolygon")      currentGeometryType = FdoGeometryType_CurvePolygon;
        if (sGeomType == L"multicurvepolygon") currentGeometryType = FdoGeometryType_MultiCurvePolygon;
        if (sGeomType == L"multigeometry")     currentGeometryType = FdoGeometryType_MultiGeometry;

        // Next check if the geometry type can be added. If yes, add it to the
        // collection, otherwise ignore it because it is already part of the collection.
        bool found;
        FdoInt32 i;
        for (i = 0, found = false;
             ((i  < m_XmlGeometryTypesCount) && (!found));
             found = (m_XmlGeometryTypes[i] == currentGeometryType), i++) ;

        if (!found)
            m_XmlGeometryTypes[m_XmlGeometryTypesCount++] = currentGeometryType;
    }

    return(false);
}


void FdoGeometricPropertyDefinition::_writeXml(FdoSchemaXmlContext* pContext)
{
    FdoXmlWriterP writer = pContext->GetXmlWriter();

    // Write the geometric property element
    writer->WriteStartElement(L"GeometricProperty");

    // Write the geometric attributes.
    if (m_readOnly) 
        writer->WriteAttribute(L"readOnly", L"true");

    writer->WriteAttribute(L"hasElevation", m_hasElevation ? L"true" : L"false");
    writer->WriteAttribute(L"hasMeasure", m_hasMeasure ? L"true" : L"false");

    // Write the geometric attributes.
    if (m_associatedSCName.GetLength() > 0) 
        writer->WriteAttribute(L"associatedSCName", m_associatedSCName);

    // Write the generic property attributes
    FdoPropertyDefinition::_writeXml(pContext);

    // Write the geometric types
    writer->WriteStartElement(L"GeometricTypes");
    
    // Convert each masked bit into a string
    if ((m_geometricTypes & FdoGeometricType_Point) != 0) {
        writer->WriteStartElement(L"GeometricType");
        writer->WriteCharacters(L"point");
        writer->WriteEndElement();
    }

    if ((m_geometricTypes & FdoGeometricType_Curve) != 0) {
        writer->WriteStartElement(L"GeometricType");
        writer->WriteCharacters(L"curve");
        writer->WriteEndElement();
    }

    if ((m_geometricTypes & FdoGeometricType_Surface) != 0) {
        writer->WriteStartElement(L"GeometricType");
        writer->WriteCharacters(L"surface");
        writer->WriteEndElement();
    }

    if ((m_geometricTypes & FdoGeometricType_Solid) != 0) {
        writer->WriteStartElement(L"GeometricType");
        writer->WriteCharacters(L"solid");
        writer->WriteEndElement();
    }

    // close the geometry types
    writer->WriteEndElement();

    // Process all geometry types.
    writer->WriteStartElement(L"GeometryTypes");

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_Point)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"point");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_MultiPoint)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"multipoint");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_LineString)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"linestring");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_MultiLineString)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"multilinestring");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_CurveString)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"curvestring");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_MultiCurveString)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"multicurvestring");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_Polygon)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"polygon");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_MultiPolygon)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"multipolygon");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_CurvePolygon)){
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"curvepolygon");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_MultiCurvePolygon)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"multicurvepolygon");
        writer->WriteEndElement();
    }

    if (m_geometryTypes & MapGeometryTypeToHexCode(FdoGeometryType_MultiGeometry)) {
        writer->WriteStartElement(L"GeometryType");
        writer->WriteCharacters(L"multigeometry");
        writer->WriteEndElement();
    }

    // close the geometry property.
    writer->WriteEndElement();

    // Close the geometric property element
    writer->WriteEndElement();
}

bool FdoGeometricPropertyDefinition::GetAllDefaults()
{
    return m_allDefaults;
}

void FdoGeometricPropertyDefinition::InitGeometryTypes()
{
    for (int i = 0; i < MAX_GEOMETRY_TYPE_SIZE; i++)
        m_types[i] = FdoGeometryType_None;
    m_cacheGeometryTypes = true;
    m_typesSize          = 0;
}

void FdoGeometricPropertyDefinition::SetToZero()
{
    m_geometryTypes = ALL_GEOMETRY_POINT_CURVE_SURFACE;
    m_geometricTypes = FdoGeometricType_All;
    m_readOnly = false;
    m_hasElevation = false;
    m_hasMeasure = false;
	m_associatedSCName =L"";

    m_geometryTypesCHANGED = ALL_GEOMETRY_POINT_CURVE_SURFACE;
    m_geometricTypesCHANGED = FdoGeometricType_All;
    m_readOnlyCHANGED = false;
    m_hasElevationCHANGED = false;
    m_hasMeasureCHANGED = false;
	m_associatedSCNameCHANGED = L"";

    m_allDefaults = true;

    m_XmlGeometryTypesCount = 0;

    InitGeometryTypes();
}

