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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpClassData.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-02-01 14:42:04 -0500 (Wed, 01 Feb 2006) $
 * $Revision: 26 $
 *
 */
#include "FDORFP.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpGeoBandRasterImp.h"
#include "FdoRfpClassData.h"
#include "FdoRfpGeoRasterExtractor.h"
#include "FdoRfpRasterUtil.h"
#include "FdoRfpGeoreference.h"
#include "FdoRfpSpatialContext.h"
#include "FdoRfpConnection.h"
#include "FdoCommonMiscUtil.h"
#include "FdoRfpDatasetCache.h"
#include <GdalFile/Override/FdoGrfpOverrides.h>
#include <gdal.h>


FdoRfpClassData::FdoRfpClassData(void)
{
}

FdoRfpClassData::~FdoRfpClassData(void)
{
}

void FdoRfpClassData::_buildUp(FdoRfpConnection *conn,
                               const FdoClassDefinitionP& classDefinition, 
                               const FdoPtr<FdoGrfpClassDefinition>& classMapping)
{
    FdoPtr<FdoRfpSpatialContextCollection> contexts = conn->GetSpatialContexts();

    m_classDefinition = classDefinition;
    m_geoRasters = FdoRfpGeoRasterCollection::Create();
    if (classMapping == NULL)
        return;

    // Initialize class member variables to Nil
    m_bFirstRaster = true;

    // Retrieve the properties from the class definition
    FdoPtr<FdoRasterPropertyDefinition> propraster;
    FdoPtr<FdoPropertyDefinitionCollection> props = classDefinition->GetProperties();
    FdoString* fcName = classDefinition->GetName();
    
    // Retrieve the raster property from the class definition
    for (int j = 0; j < props->GetCount(); j++) {
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(j);
        FdoPropertyType propType = prop->GetPropertyType();
        if (propType == FdoPropertyType_RasterProperty) {
            propraster = FDO_SAFE_ADDREF(static_cast<FdoRasterPropertyDefinition*>(prop.p));
            break;
        }
    }

    // If a raster property is not found, throw an exception
    if (NULL == propraster) {
        throw FdoException::Create(NlsMsgGet1(GRFP_46_CLASS_INVALID, "Feature class '%1$ls' is invalid.", fcName));
    }

    // Retrieve the Raster Property Schema Mapping definition
    FdoGrfpRasterDefinitionP rasterDef = classMapping->GetRasterDefinition();
    FdoGrfpRasterLocationsP locations = rasterDef->GetLocations();

    // Define a String collection to hold the possible values of the spatial context
    // definitions and add the default spatial context definiton name to it
    FdoStringCollection* coordSystems = FdoStringCollection::Create();

    // Build Up a set of raster definitions from the directory or from the config file
    FdoInt32 count = locations->GetCount();
    for (int i = 0; i < count; i++)
    {
        FdoGrfpRasterLocationP location = locations->GetItem(i);
        FdoGrfpRasterFeaturesP featureCatalogue = location->GetFeatureCatalogue();

        if (featureCatalogue->GetCount() == 0)
            _buildUpGeoRastersFromLocation(conn, location->GetName(), coordSystems);
        else
            _buildUpGeoRastersFromCatalogue(conn, featureCatalogue, coordSystems);
    }
    
    // Retrieve the spatial context association from the Raster Property
    FdoStringP m_coord = propraster->GetSpatialContextAssociation();

    if (m_coord == L"") {
        if (coordSystems->GetCount() == 0) {
            FdoPtr<FdoRfpSpatialContext> defaultContext = 
                conn->GetDefaultSpatialContext();
            m_coord = defaultContext->GetName();
        }
        else if (coordSystems->GetCount() > 1) { 
            throw FdoException::Create(
                NlsMsgGet1(
                    GRFP_97_MULTI_COORD_SYSTEM_FOR_SINGLE_FEATRUE_CLASS, 
                    "Multiple Coordinate Systems defined for Feature class '%1$ls'.",
                    fcName));
        }
        else {
            m_coord = FdoStringElementP(coordSystems->GetItem(0))->GetString();            
        }

        // Set the Class Definition's Spatial Context Association 
        propraster->SetSpatialContextAssociation(m_coord);
    }

    // Retrieve the context, so we can merge in our extents.
    FdoPtr<FdoRfpSpatialContext> context = contexts->GetItem(contexts->IndexOf(m_coord));
            
    // Calculate the new extent of the existing spatial context
    try {
        FdoPtr<FdoByteArray> extentArr = context->GetExtent();
        FdoRfpRect extent = FdoRfpUtil::CreateRectFromGeometryAgf(extentArr);
        extent = extent.Union(m_extent);

        // Set the new extent
        context->SetExtent(FdoRfpUtil::CreateGeometryAgfFromRect(extent));
    }
    catch( ... ) {
        context->SetExtent(FdoRfpUtil::CreateGeometryAgfFromRect(m_extent));
    }
}

// Extract all rasters from specified location, return value is the associated coordinate system name
void FdoRfpClassData::_buildUpGeoRastersFromLocation(FdoRfpConnection *conn, 
                                                     FdoString* location, 
                                                     FdoStringCollection* coordSystems)
{
    FdoRfpGeoRasterExtractor extractor;
    extractor.ExtractRasters(conn, location, m_geoRasters, m_coord, m_extent, m_bFirstRaster, coordSystems);
}
    
// Extract all rasters from image catalogue, return value is the associated coordinate system name
void FdoRfpClassData::_buildUpGeoRastersFromCatalogue(FdoRfpConnection *conn,
                                                      const FdoPtr<FdoGrfpRasterFeatureCollection>& featureCatalogue, 
                                                      FdoStringCollection* coordSystems)
{    
    VALIDATE_ARGUMENT(coordSystems);
    FdoPtr<FdoRfpDatasetCache>  datasetCache = conn->GetDatasetCache();
    
    FdoInt32 i, j, bandCount;

    FdoInt32 count = featureCatalogue->GetCount();
    for (i = 0; i < count; i++) // for every feature, fetch the bands
    {
        FdoGrfpRasterFeatureDefinitionP featureDef = featureCatalogue->GetItem(i);
        FdoPhysicalElementMappingP location = featureDef->GetParent();

        FdoGrfpRasterBandsP bands = featureDef->GetBands(); 
        bandCount = bands->GetCount();
        if (bandCount == 0) // invalid feature for having no bands.
            continue;

        double insX, insY, resX, resY, rotX, rotY;
        int width, height, frameNumber;
        FdoRfpGeoRasterP geoRaster = FdoRfpGeoRaster::Create();

        for (j = 0; j < bandCount; j++)
        {
            FdoGrfpRasterBandDefinitionP  bandDef = bands->GetItem(j);
            FdoGrfpRasterImageDefinitionP imageDef = bandDef->GetImage();
            GDALDatasetH hDS;

                            
            FdoStringP path = location->GetName();
            path += FdoStringP(FILE_PATH_DELIMITER);
            path += imageDef->GetName();

            hDS = datasetCache->LockDataset( path, false );

            if( hDS == NULL )
                throw FdoException::Create(NlsMsgGet(GRFP_95_CANNOT_GET_IMAGE_INFO, 
                                                     "Fail to get image information."));
            width = GDALGetRasterXSize( hDS );
            height = GDALGetRasterYSize( hDS );
            frameNumber = 1;

            if (imageDef->GetFrameNumber() > frameNumber)
                throw FdoException::Create(NlsMsgGet(GRFP_96_FRAME_NUMBER_OUT_OF_RANGE, "Frame number is out of range."));
            
            FdoRfpGeoreferenceP geoRef = new FdoRfpGeoreference();
            bool bHasGeoInfo = FdoRfpRasterUtil::GetGeoReferenceInfo(hDS, geoRef);

            datasetCache->UnlockDataset( hDS );
            hDS = NULL;

            // coordinate system handling
            //
            // first, get the coordinate system names associated to the rasters in the catalogue
            FdoStringP coord;
            if (bHasGeoInfo && geoRef->HasCoordSystem() 
                && FdoRfpRasterUtil::IsCoordinateSystemValid(geoRef->GetCoorSystem())) 
            { 
                FdoPtr<FdoRfpSpatialContext> context = 
                    conn->GetSpatialContextByWkt( geoRef->GetCoorSystem() );
                coord = context->GetName();
            }

            // Add the coordinate system names to the collection of Spatial Contexts to be returned to the caller
            // It is the callers responsibility to determine what constitutes an error if more than one spatial context
            // exists for a class definition
            if (coord != L"" && coordSystems->IndexOf(coord, false) == -1) {
                coordSystems->Add(coord);
            }
            
            // geo-reference handling
            //
            // if the config file has geo-reference info, it will be taken for preference
            // or geo-ref will be taken from the raster image itself
            FdoGrfpRasterGeoreferenceLocationP imageGeoRef = imageDef->GetGeoreferencedLocation();
            if (imageGeoRef != NULL) // get the geo-ref from configuation file        
            {
                insX = imageGeoRef->GetXInsertionPoint();
                insY = imageGeoRef->GetYInsertionPoint();
                resX = imageGeoRef->GetXResolution();
                resY = imageGeoRef->GetYResolution();
                rotX  = imageGeoRef->GetXRotation();
                rotY  = imageGeoRef->GetYRotation();
            }
            else 
            {
                if (bHasGeoInfo) // the band image is associated with geo-ref info
                {
                    insX = geoRef->GetXInsertion();
                    insY = geoRef->GetYInsertion();
                    resX = geoRef->GetXResolution();
                    resY = geoRef->GetYResolution();
                    rotX = geoRef->GetXRotation();
                    rotY = geoRef->GetYRotation();
                }
                else // there is no any geo-reference infomation associated with the raster image         
                {                    
                    throw FdoException::Create(NlsMsgGet(GRFP_100_NO_GEOREFERENCE, "Raster image has no geo-reference."));                    
                }
            }

            FdoRfpGeoBandRasterP geoBandRaster = new FdoRfpGeoBandRasterRot(m_connection, path, 
                                                                            imageDef->GetFrameNumber()-1,
                                                                            insX, insY,
                                                                            resX, resY,
                                                                            width,height,
                                                                            rotX, rotY);
            geoRaster->AddBand(geoBandRaster);

            // for the extent in spatial context, 
            // it's calculated by union of all of the bounds appearing in this feature class
            if (m_bFirstRaster)
            {
                m_extent = geoBandRaster->GetBounds();
                m_bFirstRaster = false;
            }
            else
                m_extent = m_extent.Union(geoBandRaster->GetBounds());
        }
        geoRaster->SetId(featureDef->GetName());
        geoRaster->SetRasterType(RasterType_MFMB);
        m_geoRasters->Add(geoRaster);
    }
}


FdoRfpClassData* FdoRfpClassData::Create(FdoRfpConnection *conn, const FdoClassDefinitionP& classDefinition, const FdoPtr<FdoGrfpClassDefinition>& classMapping)
{
    FdoRfpClassData* classData = new FdoRfpClassData();

    classData->m_connection = conn; // !!!NOTE!!! Internal Soft-Reference -- Do not increment Ref-Count

    classData->_buildUp(conn, classDefinition, classMapping);
    return classData;
}

FdoRfpClassDataCollection* FdoRfpClassDataCollection::Create()
{
    return new FdoRfpClassDataCollection();
}
