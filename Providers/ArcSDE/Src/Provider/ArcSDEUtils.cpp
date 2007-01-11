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
#include <math.h>

#include "ArcSDEUtils.h"

#ifdef _WIN32
#include "mbstring.h" // for multibyte functions
#else //_WIN32
#include <wctype.h>  // for wide-char functions
#endif

// the default message catalog filename
#ifndef _WIN32
char *fdoarcsde_cat = "ArcSDEMessage.cat";
#else
char *fdoarcsde_cat = "ArcSDEMessage.dll";
#endif

// Big Endian
#define wkbXDR ((unsigned char)0)
// Little Endian
#define wkbNDR ((unsigned char)1)



// Constants for ArcSDE Metadata:
const char* METADATA_CN_CLASSSCHEMA = "FdoClass_SchemaName";
const char* METADATA_CN_CLASSNAME   = "FdoClass_Name";
const char* METADATA_CN_CLASSTYPE   = "FdoClass_Type";
const char* METADATA_CN_CLASSDESC   = "FdoClass_Desc";
const char* METADATA_CN_CLASSATTR   = "FdoClass_Attribute";
const char* METADATA_CN_CLASSBASE   = "FdoClass_BaseClass";
const char* METADATA_CN_CLASSABSTRACT = "FdoClass_IsAbstract";
const char* METADATA_CN_PROPNAME    = "FdoProperty_Name";
const char* METADATA_CN_PROPTYPE    = "FdoProperty_Type";
const char* METADATA_CN_PROPDESC    = "FdoProperty_Desc";
const char* METADATA_CN_PROPREADONLY= "FdoProperty_ReadOnly";
const char* METADATA_CN_PROPDEFAULTVALUE= "FdoProperty_DefaultValue";
const char* METADATA_CN_PROPATTR    = "FdoProperty_Attribute";
const char* METADATA_V_DATATYPE_BOOLEAN = "Boolean";
const char* METADATA_V_DATATYPE_BYTE    = "Byte";
const char* METADATA_V_DATATYPE_INT16   = "Int16";
const char* METADATA_V_DATATYPE_INT64   = "Int64";
const char* METADATA_V_DATATYPE_SINGLE  = "Single";
const char* METADATA_V_DATATYPE_DOUBLE  = "Double";
const char* METADATA_V_BOOL_YES = "Yes";
const char* METADATA_V_BOOL_NO  = "No";


unsigned char* readint (unsigned char* wkb, unsigned char order, int *value)
{
    unsigned char buffer[4];

    if (wkbXDR == order)
    {
        for (int i = 0; i < 4; i++)
        {
            buffer[3 - i] = *wkb;
            wkb++;
        }
        *value = *((int *)buffer);
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            buffer[i] = *wkb;
            wkb++;
        }
        *value = *((int *)buffer);
    }

    return (wkb);
}

unsigned char* readdouble (unsigned char* wkb, unsigned char order, double *value)
{
    unsigned char buffer[8];

    if (wkbXDR == order)
    {
        for (int i = 0; i < 8; i++)
        {
            buffer[7 - i] = *wkb;
            wkb++;
        }
        *value = *((double *)buffer);
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            buffer[i] = *wkb;
            wkb++;
        }
        *value = *((double *)buffer);
    }

    return (wkb);
}

unsigned char* writeint (unsigned char* output, unsigned char order, int value)
{
    unsigned char* buffer;

    buffer = (unsigned char*)(&value);
    if (wkbXDR == order)
        for (int i = 0; i < 4; i++)
        {
            *output = buffer[3 - i];
            output++;
        }
    else
        for (int i = 0; i < 4; i++)
        {
            *output = buffer[i];
            output++;
        }

    return (output);
}

unsigned char* writedouble (unsigned char* output, unsigned char order, double value)
{
    unsigned char* buffer;

    buffer = (unsigned char*)(&value);
    if (wkbXDR == order)
        for (int i = 0; i < 8; i++)
        {
            *output = buffer[7 - i];
            output++;
        }
    else
        for (int i = 0; i < 8; i++)
        {
            *output = buffer[i];
            output++;
        }

    return (output);
}


/// Convert the OGS standard well known binary to the brain-dead FGF format
void convert_wkb_to_fgf (unsigned char** output, unsigned char** wkb)
{
    unsigned char order;
    int type;
    double x;
    double y;
    int number;
    int count;

    order = **wkb;
    (*wkb)++;
    *wkb = readint (*wkb, order, &type);
    *output = writeint (*output, wkbNDR, type);
    switch (type)
    {
        case FdoGeometryType_Point: // 1
            *output = writeint (*output, wkbNDR, FdoDimensionality_XY);
            *wkb = readdouble (*wkb, order, &x);
            *output = writedouble (*output, wkbNDR, x);
            *wkb = readdouble (*wkb, order, &y);
            *output = writedouble (*output, wkbNDR, y);
            break;
        case FdoGeometryType_LineString: // 2
            *output = writeint (*output, wkbNDR, FdoDimensionality_XY);
            *wkb = readint (*wkb, order, &count);
            *output = writeint (*output, wkbNDR, count);
            for (int i = 0; i < count; i++)
            {
                *wkb = readdouble (*wkb, order, &x);
                *output = writedouble (*output, wkbNDR, x);
                *wkb = readdouble (*wkb, order, &y);
                *output = writedouble (*output, wkbNDR, y);
            }
            break;
        case FdoGeometryType_Polygon: // 3
            *output = writeint (*output, wkbNDR, FdoDimensionality_XY);
            *wkb = readint (*wkb, order, &number);
            *output = writeint (*output, wkbNDR, number);
            for (int i = 0; i < number; i++)
            {
                *wkb = readint (*wkb, order, &count);
                *output = writeint (*output, wkbNDR, count);
                for (int i = 0; i < count; i++)
                {
                    *wkb = readdouble (*wkb, order, &x);
                    *output = writedouble (*output, wkbNDR, x);
                    *wkb = readdouble (*wkb, order, &y);
                    *output = writedouble (*output, wkbNDR, y);
                }
            }
            break;
        case FdoGeometryType_MultiPoint: // 4
        case FdoGeometryType_MultiLineString: // 5
        case FdoGeometryType_MultiPolygon: // 6
        case FdoGeometryType_MultiGeometry: // 7
            *wkb = readint (*wkb, order, &number);
            *output = writeint (*output, wkbNDR, number);
            for (int i = 0; i < number; i++)
                convert_wkb_to_fgf (output, wkb);
            break;
        default:
            throw FdoException::Create (NlsMsgGet1(ARCSDE_UNSUPPORTED_GEOMETRY_TYPE, "The geometry type '%1$d' is not supported by ArcSDE.", type));
            break;
    }
}



/// Convert FGF format to WKB:
void convert_fgf_to_wkb (unsigned char** outWKB, unsigned char** inFGF)
{
    int type;
    double x;
    double y;
    double dDummy;
    int skipDoubles = 0;
    int number;
    int count;
    int ringCount = 0;
    int lineStringElemCount = 0;
    int elemType = 0;
    unsigned char *ringElementCountPointer = NULL;
    int ringElementCount = 0;
    unsigned char orderWKB = wkbNDR;  // pick little-endian

    //DebugByteArray((unsigned char*)inFGF, 48, "FGF before translating to WKB");  // 48 is a guess

    // write byte ordering:
    **outWKB = orderWKB;
    (*outWKB)++;

    // read & write geometry type:
    *inFGF = readint (*inFGF, wkbNDR, &type);
    if (FdoGeometryType_CurvePolygon == type)
        *outWKB = writeint (*outWKB, orderWKB, FdoGeometryType_Polygon);
    else
        *outWKB = writeint (*outWKB, orderWKB, type);

    // read & write actual geometry;
    // NOTE: we throw away all Z values since WKB doesn't understand them:
    switch (type)
    {
        case FdoGeometryType_Point: // 1
            // read dimensionality:
            *inFGF = readint (*inFGF, wkbNDR, &number);
            skipDoubles = ((number & (FdoDimensionality_XY | FdoDimensionality_Z))?1:0) + ((number & FdoDimensionality_M)?1:0);

            // read & write point:
            *inFGF = readdouble (*inFGF, wkbNDR, &x);
            *outWKB = writedouble (*outWKB, orderWKB, x);
            *inFGF = readdouble (*inFGF, wkbNDR, &y);
            *outWKB = writedouble (*outWKB, orderWKB, y);
            for (int i=0; i<skipDoubles; i++)
                *inFGF = readdouble (*inFGF, wkbNDR, &dDummy);
        break;

        case FdoGeometryType_LineString: // 2
            // read dimensionality:
            *inFGF = readint (*inFGF, wkbNDR, &number);
            skipDoubles = ((number & (FdoDimensionality_XY | FdoDimensionality_Z))?1:0) + ((number & FdoDimensionality_M)?1:0);

            // read & write point count:
            *inFGF = readint (*inFGF, wkbNDR, &count);
            *outWKB = writeint (*outWKB, orderWKB, count);

            // read & write points:
            for (int i = 0; i < count; i++)
            {
                *inFGF = readdouble (*inFGF, wkbNDR, &x);
                *outWKB = writedouble (*outWKB, orderWKB, x);
                *inFGF = readdouble (*inFGF, wkbNDR, &y);
                *outWKB = writedouble (*outWKB, orderWKB, y);

                for (int i=0; i<skipDoubles; i++)
                    *inFGF = readdouble (*inFGF, wkbNDR, &dDummy);
            }
        break;

        case FdoGeometryType_Polygon: // 3
        case FdoGeometryType_CurvePolygon: // 11  
            // read dimensionality:
            *inFGF = readint (*inFGF, wkbNDR, &number);
            skipDoubles = ((number & (FdoDimensionality_XY | FdoDimensionality_Z))?1:0) + ((number & FdoDimensionality_M)?1:0);

            // read & write polygon ring count:
            *inFGF = readint (*inFGF, wkbNDR, &ringCount);
            *outWKB = writeint (*outWKB, orderWKB, ringCount);

            // read & write polygon rings:
            for (int i = 0; i < ringCount; i++)
            {
                // read the element count within this ring:
                *inFGF = readint (*inFGF, wkbNDR, &count);

                // for curve polygon, read & write the starting point:
                if (FdoGeometryType_CurvePolygon == type)
                {
                    // start counting elements, to write out the count later:
                    ringElementCount = 0;
                    ringElementCountPointer = *outWKB;

                    // write the element count within this ring; THIS WILL BE OVERWRITTEN LATER WITH CORRECT NUMBER:
                    *outWKB = writeint (*outWKB, orderWKB, count);

                    *inFGF = readdouble (*inFGF, wkbNDR, &x);
                    *outWKB = writedouble (*outWKB, orderWKB, x);
                    *inFGF = readdouble (*inFGF, wkbNDR, &y);
                    *outWKB = writedouble (*outWKB, orderWKB, y);
                    for (int j=0; j<skipDoubles; j++)
                        *inFGF = readdouble (*inFGF, wkbNDR, &dDummy);
                    ringElementCount++;
                }
                else
                {
                    // write the element count within this ring:
                    *outWKB = writeint (*outWKB, orderWKB, count);
                }

                // read & write one polygon loop:
                for (int k = 0; k < count; k++)
                {
                    if (FdoGeometryType_CurvePolygon == type)
                    {
                        *inFGF = readint (*inFGF, wkbNDR, &elemType);
                        if (FdoGeometryComponentType_CircularArcSegment == elemType)
                        {
                            // read & write arc mid-point:
                            *inFGF = readdouble (*inFGF, wkbNDR, &x);
                            *outWKB = writedouble (*outWKB, orderWKB, x);
                            *inFGF = readdouble (*inFGF, wkbNDR, &y);
                            *outWKB = writedouble (*outWKB, orderWKB, y);
                            for (int l=0; l<skipDoubles; l++)
                                *inFGF = readdouble (*inFGF, wkbNDR, &dDummy);
                            ringElementCount++;

                            // read & write arc end-point:
                            *inFGF = readdouble (*inFGF, wkbNDR, &x);
                            *outWKB = writedouble (*outWKB, orderWKB, x);
                            *inFGF = readdouble (*inFGF, wkbNDR, &y);
                            *outWKB = writedouble (*outWKB, orderWKB, y);
                            for (int m=0; m<skipDoubles; m++)
                                *inFGF = readdouble (*inFGF, wkbNDR, &dDummy);
                            ringElementCount++;
                        }
                        else  // FdoGeometryComponentType_LineStringSegment == elemType
                        {
                            *inFGF = readint (*inFGF, wkbNDR, &lineStringElemCount);
                            for (int n=0; n<lineStringElemCount; n++)
                            {
                                *inFGF = readdouble (*inFGF, wkbNDR, &x);
                                *outWKB = writedouble (*outWKB, orderWKB, x);
                                *inFGF = readdouble (*inFGF, wkbNDR, &y);
                                *outWKB = writedouble (*outWKB, orderWKB, y);
                                for (int o=0; o<skipDoubles; o++)
                                    *inFGF = readdouble (*inFGF, wkbNDR, &dDummy);
                                ringElementCount++;
                            }
                        }
                    }
                    else  // FdoGeometryType_Polygon == type
                    {
                        *inFGF = readdouble (*inFGF, wkbNDR, &x);
                        *outWKB = writedouble (*outWKB, orderWKB, x);
                        *inFGF = readdouble (*inFGF, wkbNDR, &y);
                        *outWKB = writedouble (*outWKB, orderWKB, y);
                        for (int p=0; p<skipDoubles; p++)
                            *inFGF = readdouble (*inFGF, wkbNDR, &dDummy);
                    }
                }

                // write the element count, now that we actually know it:
                if (FdoGeometryType_CurvePolygon == type)
                {
                    // write the element count within this ring:
                    ringElementCountPointer = writeint (ringElementCountPointer, orderWKB, ringElementCount);
                }
            }
        break;

        case FdoGeometryType_MultiPoint: // 4
        case FdoGeometryType_MultiLineString: // 5
        case FdoGeometryType_MultiPolygon: // 6
        case FdoGeometryType_MultiGeometry: // 7
            // read number of geometries stored in the multi type:
            *inFGF = readint (*inFGF, wkbNDR, &number);
            *outWKB = writeint (*outWKB, orderWKB, number);
            for (int i = 0; i < number; i++)
                convert_fgf_to_wkb (outWKB, inFGF);
        break;

        default:
            throw FdoException::Create (NlsMsgGet1(ARCSDE_UNSUPPORTED_GEOMETRY_TYPE, "The geometry type '%1$d' is not supported by ArcSDE.", type));
        break;
    }
}



void DebugByteArray(unsigned char pByteArray[], long lByteCount, char *strMessage)
{
#ifdef _DEBUG
    unsigned char byte = 0;
    char  buff[10];
    std::string strDebug;

    strDebug.append(strMessage);
    strDebug.append(" (ByteCount=");
    strDebug.append(FdoCommonOSUtil::ltoa(lByteCount, buff));
    strDebug.append("): '");
    for (int i=0; i<lByteCount; i++)
    {
        byte = pByteArray[i];
        strDebug.append(FdoCommonOSUtil::itoa(byte, buff));
        if (i<(lByteCount-1)) strDebug.append(", ");
    }
    strDebug.append("'");
#endif  //_DEBUG
}


// NOTE: callers must free the returned coordref via SE_coordref_free()
LONG GetCoordRefFromColumn(ArcSDEConnection* connection, const char *strTable, const char *strColumn, SE_COORDREF &coordref)
{
    SE_LAYERINFO layerinfo;
    LONG result = SE_SUCCESS;

    result = connection->GetArcSDELayerInfo(layerinfo, strTable, strColumn);
    if (SE_SUCCESS == result)
    {
        result = SE_coordref_create (&coordref);
        if (SE_SUCCESS == result)
        {
            result = SE_layerinfo_get_coordref(layerinfo, coordref);
        }
    }

    return result;
}

LONG SetShapeCoordRef(SE_SHAPE &shape, SE_COORDREF &coordref)
{
    LONG result = SE_SUCCESS;

    // extract the WKB before resetting the coordsys, to avoid warping cogo data:
    long lWKBSize = 0;
    long lWKBSizeActual = 0;
    CHAR* wkb = NULL;
    result = SE_shape_get_WKB_size(shape, &lWKBSize);
    if (SE_SUCCESS == result)
    {
        wkb = new CHAR[lWKBSize];
        result = SE_shape_as_WKB(shape, lWKBSize, &lWKBSizeActual, wkb);
        if (SE_SUCCESS == result)
        {
            DebugByteArray((unsigned char*)wkb, lWKBSizeActual, "WKB from ArcSDE shape");

            // set the coordsys:
            result = SE_shape_set_coordref(shape, coordref);

            if (SE_SUCCESS == result)
            {
                // reset the WKB to a valid rectangular polygon query area:
                result = SE_shape_generate_from_WKB((const char*)wkb, shape);
            }
        }
    }

    // clean up:
    delete[] wkb;
    wkb = NULL;

    return result;
}



void AddSpatialFilters(ArcSDEConnection* conn, const FdoSpatialOperations fdoSpatialOperation, const char* sdeColumnName, SE_SHAPE &shape, std::vector<SE_FILTER> &spatialFilters)
{
    //NOTE: see ArcSDE CHM file page "Fetching Data", for details on each ArcSDE spatial operator (e.g. SM_AI_OR_ET, etc)

    // Initialize common settings of filter:
    SE_CONNECTION connection = conn->GetConnection();
    SE_FILTER newSpatialFilter;
    // NOTE: we don't know the name of the table in here, so let caller set newSpatialFilter.table
    strcpy (newSpatialFilter.column, sdeColumnName);
    newSpatialFilter.filter.shape = shape;
    newSpatialFilter.filter_type = SE_SHAPE_FILTER;

    // get query shape type (NULL shapes require special handling):
    LONG shapeType = -1;
    LONG lResult = SE_shape_get_type(shape, &shapeType);
    handle_sde_err<FdoCommandException>(connection, lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

    if (SG_NIL_SHAPE == shapeType)
    {
        // NOTE: if a user does a Disjoint spatial operation on a NULL shape, all shapes pass the test,
        //       so we don't add any spatial filter for that case.
        //       In all other spatial operations on a NULL shape, all shapes fail the test,
        //       so we exclude all results by adding a spatial filter that always fails for all shapes.
        if (FdoSpatialOperations_Disjoint != fdoSpatialOperation)
        {
            SE_COORDREF coordref;
            SE_ENVELOPE coordrefExtent;
            SE_SHAPE    coordrefExtentShape;

            lResult = SE_coordref_create(&coordref);
            handle_sde_err<FdoCommandException>(connection, lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

            lResult = SE_shape_get_coordref(shape, coordref);
            handle_sde_err<FdoCommandException>(connection, lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

            lResult = SE_coordref_get_xy_envelope (coordref, &coordrefExtent);
            handle_sde_err<FdoCommandException>(connection, lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

            lResult = SE_shape_create(coordref, &coordrefExtentShape);
            handle_sde_err<FdoCommandException>(connection, lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

            lResult = SE_shape_generate_rectangle (&coordrefExtent, coordrefExtentShape);
            handle_sde_err<FdoCommandException>(connection, lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

            // Add a new spatial filter (disjoint):
            AddSpatialFilters(conn, FdoSpatialOperations_Disjoint, sdeColumnName, coordrefExtentShape, spatialFilters);

            // Clean up:
            SE_shape_free(shape);
            shape = NULL;
            SE_coordref_free(coordref);
            coordref = NULL;
        }
    }
    else
    {
        // Fill out the rest of the info:
        switch(fdoSpatialOperation)
        {
            case FdoSpatialOperations_Intersects:
                // Add a new spatial filter:
                newSpatialFilter.method = SM_AI_OR_ET;  // EDGE TOUCH OR AREA INTERSECT
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);

                // TODO: what if a point is lying on the boundary of a polygon?  Will this fail?
                // TODO: what if a point is lying on a line but not its endpoints?  Will this fail?
                // TODO: what if two points are equal?  Will this fail?
                // TODO: what if two lines cross, and none of their endpoints are touching?  Will this fail?
            break;

            case FdoSpatialOperations_Inside:  // NO corresponding operation in OpenGIS
            {
                LONG result = SE_SUCCESS;

                // Get shape type, to handle special cases:
                LONG shapeType = 0L;
                result = SE_shape_get_type(shape, &shapeType);
                handle_sde_err<FdoException>(result, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

                if (   (shapeType == SG_LINE_SHAPE)       || (shapeType == SG_SIMPLE_LINE_SHAPE)
                    || (shapeType == SG_MULTI_LINE_SHAPE) || (shapeType == SG_MULTI_SIMPLE_LINE_SHAPE) )
                {
                    // If we are doing Inside of line/simpleline, we need to use SM_SC and then remove all results that touch
                    // the endpoints of the query line; this is because SM_SC_NO_ET is too restrictive.
                    // This is due to ArcSDE's concept of "edge touch" being more restrictive than OpenGIS's concept of "boundary touch".

                    // NOTE: this logic will still fail in one very rare case: if a user does an "inside multiline" spatial query,
                    // and a candidate shape is fully inside one of the of the multiline lines but also has an endpoint
			        // that coincides with an endpoint of a different line in the multiline, it will be excluded when it
                    // should be included.


                    // Add a new spatial filter  (this will return more results than we want, so we need to add more spatial filters):
                    newSpatialFilter.method = SM_SC;
                    newSpatialFilter.truth = true;
                    spatialFilters.push_back(newSpatialFilter);

                    // Add one new spatial filter per line endpoint:
                    LONG lEndpointCount = 0L;
                    SE_SHAPE* endpointShapes = GetEndPointsAsShapes(conn, shape, lEndpointCount);
                    for (long i=0; i<lEndpointCount; i++)
                    {
                        newSpatialFilter.filter.shape = endpointShapes[i];
                        newSpatialFilter.method = SM_CP;
                        newSpatialFilter.truth = false;
                        spatialFilters.push_back(newSpatialFilter);
                    }
                    delete[] endpointShapes;
                }
                else  // non-line query:
                {
                    // Add a new spatial filter:
                    newSpatialFilter.method = SM_SC_NO_ET;
                    newSpatialFilter.truth = true;
                    spatialFilters.push_back(newSpatialFilter);
                }
            }
            break;

            case FdoSpatialOperations_Contains:
                // Add a new spatial filter:
                newSpatialFilter.method = SM_PC;
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);
            break;

            case FdoSpatialOperations_Crosses:
                // Add a new spatial filter:
                newSpatialFilter.method = SM_CP_OR_LCROSS;
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);

                // TODO: what about excluding geometries that are only touching?
                // TODO: what about including polyogns containing a point?  Actually, OpenGIS is confusing on this case.
            break;

            case FdoSpatialOperations_Disjoint:  // Opposite of Intersects operator
                // Add a new spatial filter:
                newSpatialFilter.method = SM_AI_OR_ET;
                newSpatialFilter.truth = false;  // invert the result set
                spatialFilters.push_back(newSpatialFilter);

                // TODO: what if a point is lying on the boundary of a polygon?  Will this fail?
                // TODO: what if a point is lying on a line but not its endpoints?  Will this fail?
                // TODO: what if two points are equal?  Will this fail?
                // TODO: what if two lines cross, and none of their endpoints are touching?  Will this fail?
            break;

            case FdoSpatialOperations_Equals:
                // Add a new spatial filter:
                newSpatialFilter.method = SM_IDENTICAL;
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);
            break;

            //TODO:case FdoSpatialOperations_Overlaps:  // Similar to (A crosses B) AND (B crosses A), except 2 lines overlapping is different.
            //TODO:    sdeSearchMethod = ?;
            //TODO:break;

            case FdoSpatialOperations_Touches:
                // Add a new spatial filter:
                newSpatialFilter.method = SM_AI_OR_ET;
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);

                // Add a new spatial filter:
                newSpatialFilter.method = SM_AI;
                newSpatialFilter.truth = false;
                spatialFilters.push_back(newSpatialFilter);
            break;

            case FdoSpatialOperations_Within:  // OGC states this is the same as switching the operand order of Contains
                // Add a new spatial filter:
                newSpatialFilter.method = SM_SC;
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);
            break;

            case FdoSpatialOperations_CoveredBy:
                // Add a new spatial filter:
                newSpatialFilter.method = SM_SC;
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);

                // Add a new spatial filter:
                newSpatialFilter.method = SM_SC_NO_ET;
                newSpatialFilter.truth = false;  // invert result set
                spatialFilters.push_back(newSpatialFilter);
            break;

            case FdoSpatialOperations_EnvelopeIntersects:
                // Add a new spatial filter:
                newSpatialFilter.method = SM_ENVP;  // ENVELOPES OVERLAP
                newSpatialFilter.truth = true;
                spatialFilters.push_back(newSpatialFilter);
            break;

            default:
                throw FdoException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_SPATIAL_OPERATION, "The given spatial operation is not supported."));
        }
    }
}


#define RESIZE(arr, type, newsize) \
	if (arr==NULL) { \
		arr = (type)malloc(newsize); \
		arr##_cursize = newsize; \
	} else if (arr##_cursize < (newsize)) { \
		free(arr); \
		arr##_cursize = ((newsize) > (arr##_cursize*2)) ? (newsize) : arr##_cursize*2; \
		arr = (type)malloc(arr##_cursize); \
	}


void convert_fgf_to_sde_shape(ArcSDEConnection *connection, FdoByteArray* fgf, SE_COORDREF coordref, SE_SHAPE& result_shape, bool bCropToExtents)
{
    LONG lResult = SE_SUCCESS;

    // Convert FGF to FdoIGeometry:
    FdoFgfGeometryFactory* gf = connection->mGeomFactory;
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(fgf);
    double z,m;
    FdoInt32 dim = geom->GetDimensionality();
    long totalNumPoints = 0;
    long numPoints = 0;
    SE_SHAPE temp_shape = NULL;
    SE_COORDREF temp_coordref = NULL;
    SE_ENVELOPE temp_coordref_envelope;
    SE_ENVELOPE given_coordref_envelope;
    SE_SHAPE *work_shape = NULL;
    FdoException *exception = NULL;
    
    try
    {
        // Prepare the SE_SHAPE object we will work with:
        lResult = SE_shape_create(coordref, &result_shape);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
        if (bCropToExtents)
        {
            // NOTE: ArcSDE won't let us create a shape that extends beyond its associated coordinate system's
            //    extents.  Even a "NULL" coordinate system is considered to have extents (starting at (0,0)
            //    which disallows negative ordinates).  Therefore, we need to create a temporary coordinate
            //    reference that is big enough to store the given shape before it is cropped to the geometry
            //    column's coordinate reference's extents.

            // Set the temporary coordref's extents to fit the given geometry:
            FdoPtr<FdoIEnvelope> fgfEnv = geom->GetEnvelope();
            temp_coordref_envelope.minx = fgfEnv->GetMinX();
            temp_coordref_envelope.maxx = fgfEnv->GetMaxX();
            temp_coordref_envelope.miny = fgfEnv->GetMinY();
            temp_coordref_envelope.maxy = fgfEnv->GetMaxY();
            
            // Enlarge the temporary coordref's extents to also fit the given coorref's extents:
            lResult = SE_coordref_get_xy_envelope(coordref, &given_coordref_envelope);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            if (given_coordref_envelope.minx < temp_coordref_envelope.minx)
                temp_coordref_envelope.minx = given_coordref_envelope.minx;
            if (given_coordref_envelope.maxx > temp_coordref_envelope.maxx)
                temp_coordref_envelope.maxx = given_coordref_envelope.maxx;
            if (given_coordref_envelope.miny < temp_coordref_envelope.miny)
                temp_coordref_envelope.miny = given_coordref_envelope.miny;
            if (given_coordref_envelope.maxy > temp_coordref_envelope.maxy)
                temp_coordref_envelope.maxy = given_coordref_envelope.maxy;

            // Create the temporary coordref
            lResult = SE_coordref_create(&temp_coordref);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            lResult = SE_coordref_set_xy_by_envelope(temp_coordref, &temp_coordref_envelope);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");

            // Work on a temporary shape object:
            lResult = SE_shape_create(temp_coordref, &temp_shape);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");

            work_shape = &temp_shape;
        }
        else
        {
            // work directly on the output shape, since no cropping required:
            work_shape = &result_shape;
        }

        // Convert FdoIGeometry to SE_SHAPE:
        switch (geom->GetDerivedType())
        {
            /*
            case FdoGeometryType_None:
                lResult = SE_shape_make_nil(*work_shape);
                handle_sde_err<FdoCommandException>(m_Connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            break;
            */

            case FdoGeometryType_Point:
            {
                FdoIPoint *point = static_cast<FdoIPoint*>(geom.p);
                SE_POINT sePoint;
                double   *pZ = NULL, *pM = NULL;
                point->GetPositionByMembers(&sePoint.x, &sePoint.y, &z, &m, &dim);
                if (dim & FdoDimensionality_Z)
                    pZ = &z;
                if (dim & FdoDimensionality_M)
                    pM = &m;
                lResult = SE_shape_generate_point (1L, &sePoint, pZ, pM, *work_shape);
                handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiPoint:
            {
                FdoIMultiPoint *multiPoint = static_cast<FdoIMultiPoint*>(geom.p);
                totalNumPoints = multiPoint->GetCount();
                FdoPtr<FdoIPoint> point;

                // Allocate arrays for XY, Z, and M (as required):
                RESIZE(connection->mGeomBuffer_pointsXY, SE_POINT*, totalNumPoints * sizeof(SE_POINT));
                if (dim & FdoDimensionality_Z)
                {
                    RESIZE(connection->mGeomBuffer_pointsZ, double*, totalNumPoints * sizeof(double));
                }
                if (dim & FdoDimensionality_M)
                {
                    RESIZE(connection->mGeomBuffer_pointsM, double*, totalNumPoints * sizeof(double));
                }

                // Populate arrays for XY, Z and M (as required):
                for (FdoInt32 i=0; i<totalNumPoints; i++)
                {
                    point = multiPoint->GetItem(i);
                    point->GetPositionByMembers(&(connection->mGeomBuffer_pointsXY[i].x), &(connection->mGeomBuffer_pointsXY[i].y), &z, &m, &dim);
                    if (dim & FdoDimensionality_Z)
                        connection->mGeomBuffer_pointsZ[i] = z;
                    if (dim & FdoDimensionality_M)
                        connection->mGeomBuffer_pointsM[i] = m;
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_point (totalNumPoints, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, *work_shape);
                handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_LineString:
            {
                FdoILineString *lineString = static_cast<FdoILineString*>(geom.p);
                totalNumPoints = lineString->GetCount();

                // Allocate arrays for XY, Z, and M (as required):
                RESIZE(connection->mGeomBuffer_pointsXY, SE_POINT*, totalNumPoints * sizeof(SE_POINT));
                if (dim & FdoDimensionality_Z)
                {
                    RESIZE(connection->mGeomBuffer_pointsZ, double*, totalNumPoints * sizeof(double));
                }
                if (dim & FdoDimensionality_M)
                {
                    RESIZE(connection->mGeomBuffer_pointsM, double*, totalNumPoints * sizeof(double));
                }


                // Populate arrays for XY, Z and M (as required):
                for (FdoInt32 i=0; i<totalNumPoints; i++)
                {
                    lineString->GetItemByMembers(i, &(connection->mGeomBuffer_pointsXY)[i].x, &(connection->mGeomBuffer_pointsXY)[i].y, &z, &m, &dim);
                    if (dim & FdoDimensionality_Z)
                        connection->mGeomBuffer_pointsZ[i] = z;
                    if (dim & FdoDimensionality_M)
                        connection->mGeomBuffer_pointsM[i] = m;
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_line (totalNumPoints, 1, NULL, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, *work_shape);
                handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiLineString:
            {
                FdoIMultiLineString *multiLineString = static_cast<FdoIMultiLineString*>(geom.p);
                FdoPtr<FdoILineString> lineString;
                FdoInt32 numLineStrings = multiLineString->GetCount();
                RESIZE(connection->mGeomBuffer_offsets, LONG*, numLineStrings * sizeof(LONG));

                // Figure out the total number of points and each linestring's offset in the offset array:
                for (FdoInt32 i=0; i<numLineStrings; i++)
                {
                    // Set this linestring's offset in the 'flat' point arrays:
                    connection->mGeomBuffer_offsets[i] = totalNumPoints;

                    // Add this linestring's number of points to totalNumPoints:
                    lineString = multiLineString->GetItem(i);
                    totalNumPoints += lineString->GetCount();
                }

                // Allocate arrays for XY, Z, and M (as required):
                RESIZE(connection->mGeomBuffer_pointsXY, SE_POINT*, totalNumPoints * sizeof(SE_POINT));
                if (dim & FdoDimensionality_Z)
                {
                    RESIZE(connection->mGeomBuffer_pointsZ, double*, totalNumPoints * sizeof(double));
                }
                if (dim & FdoDimensionality_M)
                {
                    RESIZE(connection->mGeomBuffer_pointsM, double*, totalNumPoints * sizeof(double));
                }

                // Populate arrays for XY, Z and M (as required):
                long lPointIndex = 0;
                for (FdoInt32 i=0; i<numLineStrings; i++)
                {
                    lineString = multiLineString->GetItem(i);
                    numPoints = lineString->GetCount();
                    for (FdoInt32 j=0; j<numPoints; j++)
                    {
                        lineString->GetItemByMembers(j, &(connection->mGeomBuffer_pointsXY)[lPointIndex].x, &(connection->mGeomBuffer_pointsXY)[lPointIndex].y, &z, &m, &dim);
                        if (dim & FdoDimensionality_Z)
                            connection->mGeomBuffer_pointsZ[lPointIndex] = z;
                        if (dim & FdoDimensionality_M)
                            connection->mGeomBuffer_pointsM[lPointIndex] = m;

                        lPointIndex++;
                    }
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_line (totalNumPoints, numLineStrings, connection->mGeomBuffer_offsets, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, *work_shape);
                handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_Polygon:
            {
                FdoIPolygon *polygon = static_cast<FdoIPolygon*>(geom.p);
                FdoPtr<FdoILinearRing> linearRing;

                // Figure out the total number of points:
                linearRing = polygon->GetExteriorRing();
                totalNumPoints = linearRing->GetCount();
                FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
                for (FdoInt32 i=0; i<numInteriorRings; i++)
                {
                    linearRing = polygon->GetInteriorRing(i);
                    totalNumPoints += linearRing->GetCount();
                }

                // Allocate arrays for XY, Z, and M (as required):
                RESIZE(connection->mGeomBuffer_pointsXY, SE_POINT*, totalNumPoints * sizeof(SE_POINT));
                if (dim & FdoDimensionality_Z)
                {
                    RESIZE(connection->mGeomBuffer_pointsZ, double*, totalNumPoints * sizeof(double));
                }
                if (dim & FdoDimensionality_M)
                {
                    RESIZE(connection->mGeomBuffer_pointsM, double*, totalNumPoints * sizeof(double));
                }

                // Populate arrays for XY, Z and M (as required):
                long lPointIndex = 0;
                // add exterior ring's points:
                linearRing = polygon->GetExteriorRing();
                numPoints = linearRing->GetCount();
                for (FdoInt32 i=0; i<numPoints; i++)
                {
                    linearRing->GetItemByMembers(i, &(connection->mGeomBuffer_pointsXY)[lPointIndex].x, &(connection->mGeomBuffer_pointsXY)[lPointIndex].y, &z, &m, &dim);
                    if (dim & FdoDimensionality_Z)
                        connection->mGeomBuffer_pointsZ[lPointIndex] = z;
                    if (dim & FdoDimensionality_M)
                        connection->mGeomBuffer_pointsM[lPointIndex] = m;

                    lPointIndex++;
                }
                // add interior rings' points:
                for (FdoInt32 i=0; i<numInteriorRings; i++)
                {
                    linearRing = polygon->GetInteriorRing(i);
                    numPoints = linearRing->GetCount();
                    for (FdoInt32 j=0; j<numPoints; j++)
                    {
                        linearRing->GetItemByMembers(j, &(connection->mGeomBuffer_pointsXY)[lPointIndex].x, &(connection->mGeomBuffer_pointsXY)[lPointIndex].y, &z, &m, &dim);
                        if (dim & FdoDimensionality_Z)
                            connection->mGeomBuffer_pointsZ[lPointIndex] = z;
                        if (dim & FdoDimensionality_M)
                            connection->mGeomBuffer_pointsM[lPointIndex] = m;

                        lPointIndex++;
                    }
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_polygon (totalNumPoints, 1, NULL, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, *work_shape);
                handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiPolygon:
            {
                FdoIMultiPolygon *multiPolygon = static_cast<FdoIMultiPolygon*>(geom.p);
                FdoPtr<FdoILinearRing> linearRing;
                FdoPtr<FdoIPolygon> polygon;

                // Figure out the total number of points, and the part offsets:
                FdoInt32 numPolygons = multiPolygon->GetCount();
                RESIZE(connection->mGeomBuffer_offsets, LONG*, numPolygons * sizeof(LONG));
                for (FdoInt32 i=0; i<numPolygons;i ++)
                {
                    // Store this polygon's offset in the 'flat' point arrays:
                    connection->mGeomBuffer_offsets[i] = totalNumPoints;

                    // Add this polygon's number of points to totalNumPoints:
                    polygon = multiPolygon->GetItem(i);
                    linearRing = polygon->GetExteriorRing();
                    totalNumPoints += linearRing->GetCount();
                    FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
                    for (FdoInt32 j=0; j<numInteriorRings; j++)
                    {
                        linearRing = polygon->GetInteriorRing(j);
                        totalNumPoints += linearRing->GetCount();
                    }
                }

                // Allocate arrays for XY, Z, and M (as required):
                RESIZE(connection->mGeomBuffer_pointsXY, SE_POINT*, totalNumPoints * sizeof(SE_POINT));
                if (dim & FdoDimensionality_Z)
                {
                    RESIZE(connection->mGeomBuffer_pointsZ, double*, totalNumPoints * sizeof(double));
                }
                if (dim & FdoDimensionality_M)
                {
                    RESIZE(connection->mGeomBuffer_pointsM, double*, totalNumPoints * sizeof(double));
                }

                // Populate arrays for XY, Z and M (as required):
                long lPointIndex = 0;
                for (FdoInt32 i=0; i<numPolygons;i ++)
                {
                    polygon = multiPolygon->GetItem(i);

                    // add exterior ring's points:
                    linearRing = polygon->GetExteriorRing();
                    numPoints = linearRing->GetCount();
                    for (FdoInt32 i=0; i<numPoints; i++)
                    {
                        linearRing->GetItemByMembers(i, &(connection->mGeomBuffer_pointsXY)[lPointIndex].x, &(connection->mGeomBuffer_pointsXY)[lPointIndex].y, &z, &m, &dim);
                        if (dim & FdoDimensionality_Z)
                            connection->mGeomBuffer_pointsZ[lPointIndex] = z;
                        if (dim & FdoDimensionality_M)
                            connection->mGeomBuffer_pointsM[lPointIndex] = m;

                        lPointIndex++;
                    }
                    // add interior rings' points:
                    FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
                    for (FdoInt32 j=0; j<numInteriorRings; j++)
                    {
                        linearRing = polygon->GetInteriorRing(j);
                        numPoints = linearRing->GetCount();
                        for (FdoInt32 k=0; k<numPoints; k++)
                        {
                            linearRing->GetItemByMembers(k, &(connection->mGeomBuffer_pointsXY)[lPointIndex].x, &(connection->mGeomBuffer_pointsXY)[lPointIndex].y, &z, &m, &dim);
                            if (dim & FdoDimensionality_Z)
                                connection->mGeomBuffer_pointsZ[lPointIndex] = z;
                            if (dim & FdoDimensionality_M)
                                connection->mGeomBuffer_pointsM[lPointIndex] = m;

                            lPointIndex++;
                        }
                    }
                }

                // Populate the SE_SHAPE based on the XY,Z, and M arrays:
                lResult = SE_shape_generate_polygon (totalNumPoints, numPolygons, connection->mGeomBuffer_offsets, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, *work_shape);
                handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CONVERT_SHAPE_FAILED, "Failed to convert FGF geometry to ArcSDE shape.");
            }
            break;

            case FdoGeometryType_MultiGeometry:
            case FdoGeometryType_CurveString:
            case FdoGeometryType_CurvePolygon:
            case FdoGeometryType_MultiCurveString:
            case FdoGeometryType_MultiCurvePolygon:
            default:
                throw FdoException::Create(NlsMsgGet1(ARCSDE_UNSUPPORTED_GEOMETRY_TYPE, "The geometry type '%1$d' is not supported by ArcSDE.", geom->GetDerivedType()));
            break;
        }


        // Crop resulting shape to extents, if requested:
        if (bCropToExtents)
        {
            lResult = SE_shape_clip(temp_shape, &given_coordref_envelope, result_shape);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_SHAPE_CLIP_FAILED, "Failed to clip the given shape to the coordinate reference's extents.");
        }
    }
    catch (FdoException *e)
    {
        exception = e;  // re-throw this later, after cleaning up
    }

    // clean up:
    if (temp_shape != NULL)
        SE_shape_free(temp_shape);
    if (temp_coordref != NULL)
        SE_coordref_free(temp_coordref);

    // Re-throw exception, if any:
    if (exception)
        throw exception;
}


#define ADD_POINT(ordinates, iOrdinateIndex, point_array, z, measure, inputCoordIndex)          \
    { ordinates[iOrdinateIndex] = point_array[inputCoordIndex].x;         iOrdinateIndex++;     \
      ordinates[iOrdinateIndex] = point_array[inputCoordIndex].y;         iOrdinateIndex++;     \
      if (bHasZ) { ordinates[iOrdinateIndex] = z[inputCoordIndex];        iOrdinateIndex++; }   \
      if (bHasM) { ordinates[iOrdinateIndex] = measure[inputCoordIndex];  iOrdinateIndex++; } }


void convert_sde_shape_to_fgf(ArcSDEConnection* connection, SE_SHAPE shape, FdoByteArray*& fgf)
{
    LONG lResult = SE_SUCCESS;
    FdoPtr<FdoIGeometry> geom;
    FdoInt32 iOrdinateIndex = 0;

    // Get shape type, whether it has Z/M values, and number of points/parts/subparts:
    LONG shape_type = 0L;
    lResult = SE_shape_get_type (shape, &shape_type);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");
    bool bHasZ = (TRUE==SE_shape_is_3D(shape)) ? true : false;
    bool bHasM = (TRUE==SE_shape_is_measured(shape)) ? true : false;
    FdoDimensionality dim = (FdoDimensionality)(FdoDimensionality_XY | (bHasZ ? FdoDimensionality_Z : 0) | (bHasM ? FdoDimensionality_M : 0));
    LONG numParts = 0L;
    LONG numSubParts = 0L;
    lResult = SE_shape_get_num_parts(shape, &numParts, &numSubParts);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");
    LONG numPoints = 0L;
    lResult = SE_shape_get_num_points (shape, 0, 0, &numPoints);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");

    // Get shape's point/part/subpart data:
	RESIZE(connection->mGeomBuffer_part_offsets, LONG*, numParts * sizeof(LONG));
	RESIZE(connection->mGeomBuffer_subpart_offsets, LONG*, numSubParts * sizeof(LONG));
	RESIZE(connection->mGeomBuffer_pointsXY, SE_POINT*, numPoints * sizeof(SE_POINT));
    if (bHasZ)
	{
		RESIZE(connection->mGeomBuffer_pointsZ,LFLOAT*,numPoints * sizeof(LFLOAT));
	}
    if (bHasM)
	{
		RESIZE(connection->mGeomBuffer_pointsM,LFLOAT*,numPoints * sizeof(LFLOAT));
	}
    lResult = SE_shape_get_all_points (shape, SE_DEFAULT_ROTATION, connection->mGeomBuffer_part_offsets, connection->mGeomBuffer_subpart_offsets, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM);
    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");

    // Convert this shape's data to FdoIGeometry:
    FdoFgfGeometryFactory *gf = connection->mGeomFactory;
    switch (shape_type)
    {
        /*
        case SG_NIL_SHAPE:
        {
            // how to deal with this?  FGF doesnt handle NIL geometry types.
        }
        break;
        */

        case SG_POINT_SHAPE:
        {
            FdoInt32 iNumOrdinates = 2 + (bHasZ ? 1 : 0) + (bHasM ? 1 : 0);
			RESIZE(connection->mGeomBuffer_ordinates, double*, iNumOrdinates * sizeof(double));

            ADD_POINT(connection->mGeomBuffer_ordinates, iOrdinateIndex, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, 0);

            geom = gf->CreatePoint(dim, connection->mGeomBuffer_ordinates);
        }
        break;

        case SG_MULTI_POINT_SHAPE:
        {
            FdoInt32 iNumOrdinates = numParts * (2 + (bHasZ ? 1 : 0) + (bHasM ? 1 : 0));
			RESIZE(connection->mGeomBuffer_ordinates, double*, iNumOrdinates * sizeof(double));

            for (FdoInt32 i=0; i<numParts; i++)
            {
                ADD_POINT(connection->mGeomBuffer_ordinates, iOrdinateIndex, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, i);
            }

            geom = gf->CreateMultiPoint(dim, iNumOrdinates, connection->mGeomBuffer_ordinates);
        }
        break;

        case SG_LINE_SHAPE:
        case SG_SIMPLE_LINE_SHAPE:
        {
            FdoInt32 iNumOrdinates = numPoints * (2 + (bHasZ ? 1 : 0) + (bHasM ? 1 : 0));
			RESIZE(connection->mGeomBuffer_ordinates, double*, iNumOrdinates * sizeof(double));


            for (FdoInt32 i=0; i<numPoints; i++)
            {
                ADD_POINT(connection->mGeomBuffer_ordinates, iOrdinateIndex, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, i);
            }

            geom = gf->CreateLineString(dim, iNumOrdinates, connection->mGeomBuffer_ordinates);
        }
        break;

        case SG_MULTI_LINE_SHAPE:
        case SG_MULTI_SIMPLE_LINE_SHAPE:
        {
            FdoPtr<FdoLineStringCollection> lineStrings = FdoLineStringCollection::Create();
            FdoInt32 iNumOrdinates = numPoints * (2 + (bHasZ ? 1 : 0) + (bHasM ? 1 : 0));
			RESIZE(connection->mGeomBuffer_ordinates, double*, iNumOrdinates * sizeof(double));  // allocate max number of connection->mGeomBuffer_ordinates for one linestring

            int iSdePointIndex = 0;
            for (FdoInt32 i=0; i<numParts; i++)
            {
                iOrdinateIndex = 0;

                long numPointsForPart = 0;
                lResult = SE_shape_get_num_points (shape, i+1, 0, &numPointsForPart);
                handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");

                for (FdoInt32 j=0; j<numPointsForPart; j++)
                {
                    ADD_POINT(connection->mGeomBuffer_ordinates, iOrdinateIndex, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, iSdePointIndex);
                    iSdePointIndex++;
                }

                FdoPtr<FdoILineString> lineString = gf->CreateLineString(dim, iOrdinateIndex, connection->mGeomBuffer_ordinates);
                lineStrings->Add(lineString);
            }

            geom = gf->CreateMultiLineString(lineStrings);
        }
        break;

        case SG_AREA_SHAPE:
        {
            FdoPtr<FdoILinearRing> exteriorRing;
            FdoPtr<FdoLinearRingCollection> interiorRings = FdoLinearRingCollection::Create();
            FdoInt32 iNumOrdinates = numPoints * (2 + (bHasZ ? 1 : 0) + (bHasM ? 1 : 0));
			RESIZE(connection->mGeomBuffer_ordinates, double*, iNumOrdinates * sizeof(double));  // allocate one array big enough for any given polygon ring

            int iSdePointIndex = 0;
            for (FdoInt32 i=0; i<numSubParts; i++)
            {
                long numPointsForSubPart = 0;
                lResult = SE_shape_get_num_points (shape, 1, i+1, &numPointsForSubPart);
                handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");

                iOrdinateIndex = 0;
                for (FdoInt32 j=0; j<numPointsForSubPart; j++)
                {
                    ADD_POINT(connection->mGeomBuffer_ordinates, iOrdinateIndex, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, iSdePointIndex);
                    iSdePointIndex++;
                }

                FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(dim, iOrdinateIndex, connection->mGeomBuffer_ordinates);
                if (i==0)  // subpart 1 is always the outer ring
                    exteriorRing = linearRing;
                else  // subparts 2,3,4,... are always the inner rings
                    interiorRings->Add(linearRing);
            }

            geom = gf->CreatePolygon(exteriorRing, interiorRings);
        }
        break;

        case SG_MULTI_AREA_SHAPE:
        {
            FdoPtr<FdoPolygonCollection> polygons = FdoPolygonCollection::Create();

            FdoPtr<FdoILinearRing> exteriorRing;
            FdoInt32 iNumOrdinates = numPoints * (2 + (bHasZ ? 1 : 0) + (bHasM ? 1 : 0));
			RESIZE(connection->mGeomBuffer_ordinates, double*, iNumOrdinates * sizeof(double));  // allocate one array big enough for any given polygon ring

            int iSdePointIndex = 0;
            for (FdoInt32 p=0; p<numParts; p++)
            {
                LONG numSubPartsForPart = 0L;
                lResult = SE_shape_get_num_subparts (shape, p+1, &numSubPartsForPart);
                handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");

                FdoPtr<FdoLinearRingCollection> interiorRings = FdoLinearRingCollection::Create();

                for (FdoInt32 i=0; i<numSubPartsForPart; i++)
                {
                    long numPointsForSubPart = 0;
                    lResult = SE_shape_get_num_points (shape, p+1, i+1, &numPointsForSubPart);
                    handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF.");

                    iOrdinateIndex = 0;
                    for (FdoInt32 j=0; j<numPointsForSubPart; j++)
                    {
                        ADD_POINT(connection->mGeomBuffer_ordinates, iOrdinateIndex, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM, iSdePointIndex);
                        iSdePointIndex++;
                    }

                    FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(dim, iOrdinateIndex, connection->mGeomBuffer_ordinates);
                    if (i==0)  // subpart 1 is always the outer ring
                        exteriorRing = linearRing;
                    else  // subparts 2,3,4,... are always the inner rings
                        interiorRings->Add(linearRing);
                }

                FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(exteriorRing, interiorRings);
                polygons->Add(polygon);
            }

            geom = gf->CreateMultiPolygon(polygons);
        }
        break;

        default:
            throw FdoException::Create(NlsMsgGet(ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF, "Error encountered while converting ArcSDE shape to FGF."));
        break;
    }
	
    // Convert geom to FGF for caller:
    fgf = gf->GetFgf(geom);
}


bool ClassContainsProperty(FdoClassDefinition *pClassDef, FdoIdentifier* pPropertyId)
{
    // Validate inputs:
    if (pClassDef == NULL)
        throw FdoException::Create(NlsMsgGet(ARCSDE_NULL_ARGUMENT, "A required argument was set to NULL."));
    if (pPropertyId == NULL)
        throw FdoException::Create(NlsMsgGet(ARCSDE_NULL_ARGUMENT, "A required argument was set to NULL."));

    FdoPtr<FdoPropertyDefinitionCollection> pProps = pClassDef->GetProperties();
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> pBaseProps = pClassDef->GetBaseProperties();
    FdoPtr<FdoPropertyDefinition> pProp;

    // try 'main' property collection:
    bool bFound = true;
    try
    {
        pProp = pProps->GetItem(pPropertyId->GetName());
    }
    catch (...)
    {
        bFound = false;
    }

    // try base property collection:
    if (false == bFound)
    {
        bFound = true;
        try
        {
            pProp = static_cast<FdoPropertyDefinition*>(pBaseProps->GetItem(pPropertyId->GetName()));
        }
        catch (...)
        {
            bFound = false;
        }
    }

    return bFound;
}

bool IsIdentityProperty(FdoClassDefinition* pClassDef, FdoString *strIdPropertyName)
{
	bool   bRetVal = false;
	FdoPtr<FdoClassDefinition> pBaseClass;
	FdoPtr<FdoClassDefinition> pBaseParentClass;

	// IMPORTANT: the schema design states that for subclasses, only the root parent class stores
	// the identity properties; this means we need to first find the root parent class (if any):
	FDO_SAFE_ADDREF(pClassDef);
	pBaseClass = pClassDef;
	pBaseParentClass = pClassDef->GetBaseClass();
	while (pBaseParentClass != NULL)
	{
		pBaseClass = pBaseParentClass;  //NOTE: automatic addref here, due to "smart ptr to smart ptr" assignment
		pBaseParentClass = (FdoClassDefinition*)pBaseClass->GetBaseClass();
	}

	// Now look for the identity property in the identity collection of the "absolute" base class:
	FdoPtr<FdoDataPropertyDefinitionCollection> pIdProperties = pBaseClass->GetIdentityProperties();
    bRetVal = (pIdProperties != NULL) && (pIdProperties->GetCount()>0) && pIdProperties->Contains(strIdPropertyName);

	return bRetVal;
}


bool ClassContainsData(FdoIConnection *conn, FdoClassDefinition *fdoClass)
{
    FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
    selectCmd->SetFeatureClassName(fdoClass->GetName());
    FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
    bool bRet = reader->ReadNext();
    reader->Close();
    return bRet;
}



FdoDataType SDEType2FdoType (LONG sdeType)
{
    FdoDataType ret;

    // NOTE: a lot of ArcSDE->RDBMS type mappings are questionable, but we have no control over them.
    //       We don't attempt to workaround ArcSDE's data type limitations, because it's too complicated
    //       (a different mapping for each RDBMS); so we just do a straightforward mapping
    //       between ArcSDE types and FDO types.
    switch (sdeType)
    {
        case SE_SMALLINT_TYPE:        /* 2-byte integer */
            // NOTE: For Oracle RDBMS, SE_SMALLINT_TYPE is actually a NUMBER(4,0).
            //       For other RDBMS, this may be different -- Documentation is unclear.
            ret = FdoDataType_Int16;
            break;
        case SE_INTEGER_TYPE:         /* 4-byte integer */
            ret = FdoDataType_Int32;
            break;
        case SE_INT64_TYPE:           /* 8-byte integer */
            ret = FdoDataType_Int64;
            break;
        case SE_FLOAT_TYPE:           /* 4-byte float */
            // NOTE: For Oracle RDBMS, SE_FLOAT_TYPE is actually a NUMBER(p,s).
            //       For other RDBMS, this may be different -- Documentation is unclear.
            ret = FdoDataType_Single;
            break;
        case SE_DOUBLE_TYPE:          /* 8-byte float */
            // NOTE: For Oracle RDBMS, SE_DOUBLE_TYPE is a fixed-point NUMBER(p,s)!
            //       For other RDBMS, this may be different -- Documentation is unclear.
            ret = FdoDataType_Double;
            break;
        case SE_STRING_TYPE:          /* Null term. Character array */
            ret = FdoDataType_String;
            break;
        case SE_BLOB_TYPE:            /* Variable length data */
            ret = FdoDataType_BLOB;
            break;
        case SE_DATE_TYPE:            /* Struct tm date */
            ret = FdoDataType_DateTime;
            break;
        case SE_SHAPE_TYPE:           /* Feature structure */
            ret = (FdoDataType)-1;  // NOTE: we use -1 internally to denote a geometry property
            break;
        default:
            ret = (FdoDataType) FDODATATYPE_UNHANDLEDSDETYPE;
            break;
    }

    return (ret);
}



// Used by ApplySchema:
LONG FdoType2SDEType (FdoDataType type)
{
    LONG ret;

    // NOTE: a lot of ArcSDE->RDBMS type mappings are questionable, but we have no control over them.
    //       We don't attempt to workaround ArcSDE's data type limitations, because it's too complicated
    //       (a different mapping for each RDBMS); so we just do a straightforward mapping
    //       between ArcSDE types and FDO types.
    switch (type)
    {
        case FdoDataType_Int16:
            ret = SE_SMALLINT_TYPE;
            break;
        case FdoDataType_Int32:
            ret = SE_INTEGER_TYPE;
            break;
        case FdoDataType_Int64:
            ret = SE_INT64_TYPE;
            break;
        case FdoDataType_Single:
            ret = SE_FLOAT_TYPE;
            break;
        case FdoDataType_Double:
            ret = SE_DOUBLE_TYPE;
            break;
        case FdoDataType_String:
            ret = SE_STRING_TYPE;
            break;
        case FdoDataType_BLOB:
            ret = SE_BLOB_TYPE;
            break;
        case FdoDataType_DateTime:
            ret = SE_DATE_TYPE;
            break;


//
// these aren't really supported:
//
        case FdoDataType_Decimal:
            ret = SE_DOUBLE_TYPE;
            break;
        case FdoDataType_Boolean:
            ret = SE_SMALLINT_TYPE;        /* 2-byte integer */
            break;
        case FdoDataType_Byte:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (NlsMsgGet1(ARCSDE_DATATYPE_UNHANDLED, "The FDO DataType %1$d is unsupported.", (int)type));
    }

    return (ret);
}


FdoDateTime SdeDateTime2FdoDateTime(struct tm sdeDateTime)
{
    FdoDateTime fdoDateTime;

    fdoDateTime.year = sdeDateTime.tm_year + 1900;
    fdoDateTime.month = sdeDateTime.tm_mon + 1;
    fdoDateTime.day = sdeDateTime.tm_mday;
    fdoDateTime.hour = sdeDateTime.tm_hour;
    fdoDateTime.minute = sdeDateTime.tm_min;
    fdoDateTime.seconds = (float)sdeDateTime.tm_sec;

    return fdoDateTime;
}

struct tm FdoDateTime2SdeDateTime(FdoDateTime fdoDateTime)
{
    struct tm sdeDateTime;

    // Set date portion:
    if (fdoDateTime.IsDate() || fdoDateTime.IsDateTime())
    {
        sdeDateTime.tm_year = fdoDateTime.year - 1900;
        sdeDateTime.tm_mon = fdoDateTime.month - 1;
        sdeDateTime.tm_mday = fdoDateTime.day;
    }
    else
    {
        // NOTE: there is no way in struct tm to indicate the absence of a date,
        //       so we default to 0,0,1 (jan 1 1900) if no date is specified.
        sdeDateTime.tm_year = 0;
        sdeDateTime.tm_mon = 0;
        sdeDateTime.tm_mday = 1;
    }

    // Set time portion:
    if (fdoDateTime.IsTime() || fdoDateTime.IsDateTime())
    {
        sdeDateTime.tm_hour = fdoDateTime.hour;
        sdeDateTime.tm_min = fdoDateTime.minute;
        sdeDateTime.tm_sec = (int)fdoDateTime.seconds;
    }
    else
    {
        // NOTE: there is no way in struct tm to indicate the absence of a time,
        //       so we default to 0,0,0 (00:00:00) if no time is specified.
        sdeDateTime.tm_hour = 0;
        sdeDateTime.tm_min = 0;
        sdeDateTime.tm_sec = 0;
    }

    return sdeDateTime;
}


FdoString* NlsMsgGetMain(int msg_num, char* default_msg, ...)
{
    va_list varargs;
    va_start(varargs, default_msg);
    FdoString* ret = FdoException::NLSGetMessage(msg_num, default_msg, fdoarcsde_cat, varargs);
    va_end(varargs);

    return ret;
}



bool nameInUse(std::vector<std::string> namesInUse, const CHAR* testName)
{
    bool bNameInUse = false;
    for (unsigned int i=0; (i<namesInUse.size()) && !bNameInUse; i++)
        if (0==FdoCommonOSUtil::stricmp(namesInUse.at(i).c_str(), testName))
            bNameInUse = true;

    return bNameInUse;
}

void generateUniqueName(ArcSDEConnection *conn, const std::vector<std::string> namesInUse, long lMaxLen, const CHAR* suggestedName, bool bStrict, CHAR* dbName, CHAR* ownerName, CHAR *generatedName)
{
    LONG lResult = 0;

    // Truncate the name to acceptable length, careful with lead/trail bytes:
    CHAR *suggestedNameTruncated = new CHAR[lMaxLen];
    strncpy(suggestedNameTruncated, suggestedName, lMaxLen-1);
    if (strlen(suggestedName) >= (unsigned long)lMaxLen-1)
    {
        suggestedNameTruncated[lMaxLen-1] = '\0';
        if (FdoCommonOSUtil::ismbslead((unsigned char*)suggestedNameTruncated, (unsigned char*)&(suggestedNameTruncated[lMaxLen-2])))
            suggestedNameTruncated[lMaxLen-2] = '\0';
    }

    if (bStrict)
    {
        // Add prefix if first character is not alphabetical, careful with lead/trail bytes:
        if (!FdoCommonOSUtil::ismbcalpha((unsigned char*)suggestedNameTruncated, lMaxLen, (unsigned char*)&(suggestedNameTruncated[0])))
        {
            //replace last character with NULL's:
            suggestedNameTruncated[strlen(suggestedNameTruncated)-1] = '\0';
            if (FdoCommonOSUtil::ismbslead((unsigned char*)suggestedNameTruncated, (unsigned char*)&(suggestedNameTruncated[strlen(suggestedNameTruncated)-2])))
                suggestedNameTruncated[strlen(suggestedNameTruncated)-2] = '\0';

            //shift all remaining characters by 1 byte:
            for (long i=(long)strlen(suggestedNameTruncated); i>0; i--)
                suggestedNameTruncated[i] = suggestedNameTruncated[i-1];

            //insert prefix:
            suggestedNameTruncated[0] = 'X';
        }

        // Replace all non-alphanumeric characters with '_':
        for (unsigned int i=0; i<strlen(suggestedNameTruncated); i++)
        {
            if (!FdoCommonOSUtil::ismbcalnum((unsigned char*)suggestedNameTruncated, lMaxLen, (unsigned char*)&(suggestedNameTruncated[i])))
            {
                if (FdoCommonOSUtil::ismbslead((unsigned char*)suggestedNameTruncated, (unsigned char*)&(suggestedNameTruncated[i])))
                {
                    suggestedNameTruncated[i] = '_';
                    i++;
                    suggestedNameTruncated[i] = '_';
                }
                else
                    suggestedNameTruncated[i] = '_';
            }
        }
    }

    // Check for name match:
    long lSuffix = 0;
    CHAR sSuffix[15];
    CHAR *suggestedNameSuffixed = new CHAR[lMaxLen];
    strcpy(suggestedNameSuffixed, suggestedNameTruncated);

    // build qualified name (if an ownerName or dbName was provided):
    CHAR qualifiedSuggestedName[SE_QUALIFIED_TABLE_NAME];
    if (ownerName != NULL)
    {
        // NOTE: we assume that if an ownerName is given, we're generating a table name
        lResult = SE_table_make_qualified_name(conn->GetConnection(), dbName, ownerName, suggestedNameSuffixed, NULL, qualifiedSuggestedName);
        handle_sde_err<FdoException>(conn->GetConnection (), lResult, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");
    }
    else
        strcpy(qualifiedSuggestedName, suggestedNameSuffixed);

    while (nameInUse(namesInUse, qualifiedSuggestedName))
    {
        strcpy(suggestedNameSuffixed, suggestedNameTruncated);

        // Add an integer to the end of the name to try to make it unique:
        lSuffix++;
        FdoCommonOSUtil::ltoa(lSuffix, sSuffix);

        long lSuffixStartIndex = (long)strlen(suggestedNameSuffixed) - (long)strlen(sSuffix);
        if (FdoCommonOSUtil::ismbstrail((unsigned char*)suggestedNameSuffixed, (unsigned char*)&(suggestedNameSuffixed[lSuffixStartIndex])))
            lSuffixStartIndex--;

        strcpy(suggestedNameSuffixed+lSuffixStartIndex, sSuffix);
        suggestedNameSuffixed[lSuffixStartIndex+strlen(sSuffix)] = '\0';

        // build qualified name (if an ownerName or dbName was provided):
        if (ownerName != NULL)
        {
            // NOTE: we assume that if an ownerName is given, we're generating a table name
            lResult = SE_table_make_qualified_name(conn->GetConnection(), dbName, ownerName, suggestedNameSuffixed, NULL, qualifiedSuggestedName);
            handle_sde_err<FdoException>(conn->GetConnection (), lResult, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");
        }
        else
            strcpy(qualifiedSuggestedName, suggestedNameSuffixed);
    }


    // Return generated unique name:
    strcpy (generatedName, qualifiedSuggestedName);

    // Cleanup:
    delete[] suggestedNameSuffixed;
    delete[] suggestedNameTruncated;
}


FdoPropertyValue* GetItemNoThrow(FdoPropertyValueCollection *coll, FdoString* name)
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



void GetFilterInfo(ArcSDEConnection *connection, FdoFilter* filter, FdoClassDefinition* classDef, CHAR* &whereClause, SHORT &spatialFilterCount, SE_FILTER* &spatialFilters)
{
    if (filter == NULL)
    {
        whereClause = NULL;
        spatialFilterCount = 0;
        spatialFilters = NULL;
    }
    else
    {
        CHAR table[SE_QUALIFIED_TABLE_NAME];
        SE_COORDREF coordref;
        FdoPtr<ArcSDEFilterToSql> f2s;
        LONG result = 0L;

        // Validate connection:
        if (connection == NULL)
            throw FdoException::Create (NlsMsgGet(ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));


        // Get whereClause from filter:
        f2s = new ArcSDEFilterToSql (connection, classDef);
        f2s->HandleFilter (filter);
        CHAR* tempWhereClause = NULL;
        wide_to_multibyte (tempWhereClause, f2s->GetSql ());  // volatile, since memory is on stack
        if (0 == strcmp (tempWhereClause, " WHERE "))
            whereClause = NULL;
        else
        {
            whereClause = new CHAR[strlen (tempWhereClause) + 1];
            strcpy (whereClause, tempWhereClause);
        }

        // Get table name from class name:
        connection->ClassToTable(table, classDef);

        // update table name and coordinate system of each spatial filter:
        //TODO: probably move this code to ArcSDEFilterToSQL
        spatialFilterCount = f2s->GetSpatialFilters (spatialFilters);
        for (int i = 0; i < spatialFilterCount; i++)
        {
            // set table name for each spatial filter:
            strcpy (spatialFilters[i].table, table);

            // set coordinate system for each spatial filter to match the column's coordinate system:
            //TODO: use the active spatial context instead of the column's coordinate system?
            result = GetCoordRefFromColumn (connection, table, spatialFilters[i].column, coordref);
            if (SE_SUCCESS == result)
            {
                result = SetShapeCoordRef (spatialFilters[i].filter.shape, coordref);

                // clean up:
                SE_coordref_free (coordref);
            }
            //TODO: if result != SE_SUCCESS at ths point, do something about it (but no exceptions(?),
            // since we are currently hitting non-spatial classes)
        }
    }
}


void ApplyFilterInfoToStream(ArcSDEConnection *connection, SE_STREAM queryStream, const CHAR* table, CHAR* whereClause, const int columnCount, const CHAR** columnNames, const SHORT spatialFilterCount, SE_FILTER* spatialFilters, FdoOrderingOption orderingOption, FdoIdentifierCollection* orderingIds)
{
    LONG result = 0L;
    SE_SQL_CONSTRUCT* sql = NULL;

    // Validate connection:
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet(ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));

    // Validate spatial operators:
    SE_QUERYINFO query_info;
    result = SE_queryinfo_create(&query_info);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");

    // Apply filter info, including order by clause:
    ApplyFilterInfoToQueryInfo(connection, query_info, table, whereClause, columnCount, columnNames, orderingOption, orderingIds);

    // Apply attribute query to stream:
    result = SE_stream_query_with_info(queryStream, query_info);
    handle_sde_err<FdoCommandException>(queryStream, result, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");

    // Apply spatial filters to stream:
    result = SE_stream_set_spatial_constraints (queryStream, SE_OPTIMIZE, FALSE, spatialFilterCount, spatialFilters);
    handle_sde_err<FdoCommandException>(queryStream, result, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

    // clean up:
    SE_queryinfo_free(query_info);
}


// Apply the given info to the given SE_QUERYINFO object:
void ApplyFilterInfoToQueryInfo(ArcSDEConnection *connection, SE_QUERYINFO query_info, const CHAR *table, const CHAR *whereClause, FdoInt32 numProperties, const CHAR **columnNames, FdoOrderingOption orderingOption, FdoIdentifierCollection* orderingIds)
{
    LONG result = 0L;

    // Set where clause:
    if (NULL == whereClause)
        result = SE_queryinfo_set_where_clause(query_info, "");
    else
        result = SE_queryinfo_set_where_clause(query_info, whereClause);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");

    // Set from clause:
    CHAR **tables = NULL;
    tables = (CHAR**)alloca(1 * sizeof(CHAR*)); // new CHAR*[1];
    tables[0] = (CHAR*)alloca(SE_QUALIFIED_TABLE_NAME); // new CHAR[SE_MAX_TABLE_LEN];
    strcpy(tables[0], table);
    result = SE_queryinfo_set_tables(query_info, 1, (const char**)tables, NULL);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");


    // Set select clause:
    result = SE_queryinfo_set_columns(query_info, numProperties, (const char**)columnNames);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");

    // Set order by clause:
    if ((orderingIds != NULL) && (orderingIds->GetCount() > 0))
    {
        std::wstring  orderByClause = ORDERBY_CLAUSE;
        for (FdoInt32 i=0; i<orderingIds->GetCount(); i++)
        {
            if (i>0)
                orderByClause += ORDERBY_SEPARATOR;

            FdoPtr<FdoIdentifier> id = orderingIds->GetItem(i);
            orderByClause += id->GetName();

            if (orderingOption == FdoOrderingOption_Ascending)
                orderByClause += ORDERBY_ASCENDING;
            else
                orderByClause += ORDERBY_DESCENDING;
        }
        CHAR *mbOrderByClause = NULL;
        wide_to_multibyte(mbOrderByClause, (const wchar_t*)orderByClause.c_str());
        result = SE_queryinfo_set_by_clause(query_info, mbOrderByClause);
        handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");
    }
}

// Return true IFF the given function name is a supported aggregate function:
bool IsAggregateFunction(const wchar_t* functionName)
{
    return (0==wcscmp(functionName, FDO_FUNCTION_COUNT))
        || (0==wcscmp(functionName, FDO_FUNCTION_MIN))
        || (0==wcscmp(functionName, FDO_FUNCTION_AVG))
        || (0==wcscmp(functionName, FDO_FUNCTION_MAX))
        || (0==wcscmp(functionName, ARCSDE_FUNCTION_STDDEV))
        || (0==wcscmp(functionName, FDO_FUNCTION_SUM));
}


// Get the SDE type that the given function evaluates to:
LONG GetFunctionSdeType(FdoIConnection *conn, FdoString *fdoFunctionName)
{
    FdoPtr<FdoIExpressionCapabilities> exprCapabilities = conn->GetExpressionCapabilities();
    FdoPtr<FdoFunctionDefinitionCollection> functions = exprCapabilities->GetFunctions();
    FdoPtr<FdoFunctionDefinition> function = functions->GetItem(fdoFunctionName);
    return FdoType2SDEType(function->GetReturnType());
}

// Get the SDE statistic type mask for the given aggregate function name:
LONG GetAggregateSdeStatsMask(FdoString* aggrFunctionName)
{
    if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_MIN))
        return SE_MIN_STATS;
    else if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_AVG))
        return SE_AVERAGE_STATS;
    else if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_MAX))
        return SE_MAX_STATS;
    else if (0==wcscmp(aggrFunctionName, ARCSDE_FUNCTION_STDDEV))
        return SE_STD_DEV_STATS;
    else if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_SUM))
        return SE_AVERAGE_STATS | SE_COUNT_STATS; // need both since sum = avg * count
    else if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_COUNT))
        return SE_COUNT_STATS;
    else
        throw FdoException::Create(NlsMsgGet1(ARCSDE_FUNCTION_NOT_FOUND_OR_WRONG_TYPE, "The given function '%1$ls' was not found or is of the wrong type.", aggrFunctionName));
}



// Get the value of an aggregate function that has return type "double":
double GetAggregateValueDouble(SE_STATS* stats, FdoString* aggrFunctionName)
{
    if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_MIN))
        return stats->min;
    else if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_AVG))
        return stats->average;
    else if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_MAX))
        return stats->max;
    else if (0==wcscmp(aggrFunctionName, ARCSDE_FUNCTION_STDDEV))
        return stats->std_dev;
    else if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_SUM))
        return stats->average * stats->count;
    else
        throw FdoException::Create(NlsMsgGet1(ARCSDE_FUNCTION_NOT_FOUND_OR_WRONG_TYPE, "The given function '%1$ls' was not found or is of the wrong type.", aggrFunctionName));
}

// Get the value of an aggregate function that has return type "FdoInt64":
FdoInt64 GetAggregateValueInt64(SE_STATS* stats, FdoString* aggrFunctionName)
{
    if (0==wcscmp(aggrFunctionName, FDO_FUNCTION_COUNT))
        return stats->count;
    else
        throw FdoException::Create(NlsMsgGet1(ARCSDE_FUNCTION_NOT_FOUND_OR_WRONG_TYPE, "The given function '%1$ls' was not found or is of the wrong type.", aggrFunctionName));
}

// Get the function name of the given identifier, if it is a computed idenfitier; otherwise throw an exception:
FdoString* GetAggregateFunctionName(FdoIdentifier *id)
{
    FdoComputedIdentifier *computedId = dynamic_cast<FdoComputedIdentifier*>(id);
    if (NULL == computedId)
        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));

    FdoPtr<FdoExpression> expr = computedId->GetExpression();
    FdoFunction* function = dynamic_cast<FdoFunction*>(expr.p);
    if (NULL == function)
        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_COMPUTEDID_NOT_AGGREGATE_FUNCTION, "ArcSDE Provider does not support computed identifiers which do not correspond to aggregate functions."));

    return function->GetName();
}

// Get the property name the given aggregate function operates on:
FdoString* GetAggregateFunctionPropertyName(FdoFunction *fdoFunction)
{
    FdoPtr<FdoExpressionCollection> fdoFunctionArgs = fdoFunction->GetArguments();
    if (fdoFunctionArgs->GetCount() != 1)  // arcsde aggregates only operate on one column at a time
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_FUNCTION_WRONG_NUM_ARGUMENTS, "Wrong number of arguments passed to function '%1$ls'.", fdoFunction->GetName()));
    FdoPtr<FdoExpression> fdoFunctionExpr = fdoFunctionArgs->GetItem(0);
    FdoIdentifier* fdoFunctionArg = dynamic_cast<FdoIdentifier*>(fdoFunctionExpr.p);
    if (fdoFunctionArg == NULL)  // arcsde aggregates only operate on property names (identifiers)
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_FUNCTION_WRONG_ARG_TYPE, "Aggregate function argument was not of expected type '%1$ls'.", L"FdoIdentifier"));

    return fdoFunctionArg->GetName();
}


// Gets the endpoints of the given shape as an array of new SE_SHAPE objects;
// The returned SE_SHAPE array needs to be freed by the caller using delete[]:
SE_SHAPE* GetEndPointsAsShapes(ArcSDEConnection *connection, SE_SHAPE shape, long &lEndpointCount)
{
    LONG lResult = SE_SUCCESS;
    SE_SHAPE *resultShapes = NULL;
    FdoException *exception = NULL;
    SE_COORDREF coordref = NULL;

    try
    {
        // Validate shape type:
        LONG shape_type = 0L;
        lResult = SE_shape_get_type (shape, &shape_type);
        handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");
        if ( (shape_type != SG_LINE_SHAPE)       && (shape_type != SG_SIMPLE_LINE_SHAPE)
        && (shape_type != SG_MULTI_LINE_SHAPE) && (shape_type != SG_MULTI_SIMPLE_LINE_SHAPE) )
            throw FdoException::Create(NlsMsgGet(ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition."));

        // Determine whether shape has Z/M values, and number of points/parts/subparts:
        bool bHasZ = (TRUE==SE_shape_is_3D(shape)) ? true : false;
        bool bHasM = (TRUE==SE_shape_is_measured(shape)) ? true : false;
        FdoDimensionality dim = (FdoDimensionality)(FdoDimensionality_XY | (bHasZ ? FdoDimensionality_Z : 0) | (bHasM ? FdoDimensionality_M : 0));
        LONG numParts = 0L;
        LONG numSubParts = 0L;
        lResult = SE_shape_get_num_parts(shape, &numParts, &numSubParts);
        handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");
        LONG numPoints = 0L;
        lResult = SE_shape_get_num_points (shape, 0, 0, &numPoints);
        handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

        // Get shape's point/part/subpart data:
        RESIZE(connection->mGeomBuffer_part_offsets, LONG*, numParts * sizeof(LONG));
        RESIZE(connection->mGeomBuffer_subpart_offsets, LONG*, numSubParts * sizeof(LONG));
        RESIZE(connection->mGeomBuffer_pointsXY, SE_POINT*, numPoints * sizeof(SE_POINT));
        if (bHasZ)
        {
            RESIZE(connection->mGeomBuffer_pointsZ, double*, numPoints * sizeof(double));
        }
        if (bHasM)
        {
            RESIZE(connection->mGeomBuffer_pointsM, double*, numPoints * sizeof(double));
        }
        lResult = SE_shape_get_all_points (shape, SE_DEFAULT_ROTATION, connection->mGeomBuffer_part_offsets, connection->mGeomBuffer_subpart_offsets, connection->mGeomBuffer_pointsXY, connection->mGeomBuffer_pointsZ, connection->mGeomBuffer_pointsM);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

        // Get shape's coordref:
        lResult = SE_coordref_create(&coordref);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");
        lResult = SE_shape_get_coordref(shape, coordref);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

        // extract the endpoints:
        resultShapes = new SE_SHAPE[2*numParts];
        lEndpointCount = 2*numParts;
        for (long i=0; i<numParts; i++)
        {
            // Get number of points for this part:
            long numPointsForPart = 0;
            lResult = SE_shape_get_num_points (shape, i+1, 0, &numPointsForPart);
            handle_sde_err<FdoCommandException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

            for (long j=0; j<2; j++)  // j==0==start endpoint, j==1==end endpoint
            {
                // Determine XY,Z,M of this endpoint:
                SE_POINT *endpointXY = NULL;
                LFLOAT   *endpointZ  = NULL;
                LFLOAT   *endpointM  = NULL;
                LONG lOffset = ((j==0) ? connection->mGeomBuffer_subpart_offsets[connection->mGeomBuffer_part_offsets[i]]
                                       : connection->mGeomBuffer_subpart_offsets[connection->mGeomBuffer_part_offsets[i]]+numPointsForPart-1 );
                endpointXY = connection->mGeomBuffer_pointsXY + lOffset;
                if (bHasZ)
                    endpointZ = connection->mGeomBuffer_pointsZ + lOffset;
                if (bHasM)
                    endpointM = connection->mGeomBuffer_pointsM + lOffset;

                // Create a new SE_SHAPE for this endpoint, and store it in the resultShapes array:
                SE_SHAPE newShape;
                lResult = SE_shape_create(coordref, &newShape);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");
                lResult = SE_shape_generate_point(1, endpointXY, endpointZ, endpointM, newShape);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");
                resultShapes[2*i + j] = newShape;
            }
        }
    }
    catch (FdoException *e)
    {
        exception = e;
    }

    // cleanup (NOTE: the new SE_SHAPE instances will be cleaned up later on, after the stream is executed):
    SE_coordref_free (coordref);

    // Re-throw exception if any:
    if (exception)
        throw exception;

    // Return result (if no exception):
    return resultShapes;
}



FdoString* RdbmsToString(long lRdbmsID)
{
    switch (lRdbmsID)
    {
        case SE_DBMS_IS_OTHER:     return L"Other"; break;
        case SE_DBMS_IS_ORACLE:    return L"Oracle"; break;
        case SE_DBMS_IS_INFORMIX:  return L"Informix"; break;
        case SE_DBMS_IS_SYBASE:    return L"Sybase"; break;
        case SE_DBMS_IS_DB2:       return L"DB2"; break;
        case SE_DBMS_IS_SQLSERVER: return L"SQL Server"; break;
        case SE_DBMS_IS_ARCINFO:   return L"ArcInfo"; break;
        case SE_DBMS_IS_IUS:       return L"IUS"; break;
        case SE_DBMS_IS_DB2_EXT:   return L"DB2_EXT"; break;
        case SE_DBMS_IS_ARCSERVER: return L"ArcServer"; break;
        case SE_DBMS_IS_JET:       return L"Jet"; break;
        default:
        case SE_DBMS_IS_UNKNOWN:   return L"Unknown RDBMS"; break;
    }
}

