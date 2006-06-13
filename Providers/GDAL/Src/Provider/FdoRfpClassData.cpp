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

void FdoRfpClassData::_buildUp(const FdoPtr<FdoRfpSpatialContextCollection>& contexts, const FdoClassDefinitionP& classDefinition, const FdoPtr<FdoGrfpClassDefinition>& classMapping)
{
    m_classDefinition = classDefinition;
    m_geoRasters = FdoRfpGeoRasterCollection::Create();
    if (classMapping == NULL)
        return;

    // Initialize class member variables to Nil
    m_coord = "";
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

    // Retrieve the spatial context association from the Raster Property
    FdoStringP scName = propraster->GetSpatialContextAssociation();

    // the first one must be the default one or be set by the config file
    FdoPtr<FdoRfpSpatialContext> defaultContext = contexts->GetItem(0);
    FdoStringP defaultCoord = defaultContext->GetCoordinateSystem();

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
            _buildUpGeoRastersFromLocation(location->GetName(), coordSystems);
        else
            _buildUpGeoRastersFromCatalogue(featureCatalogue, coordSystems);
    }
	
    // First decide whether the coordinate system is already set for the class
    // If the Association is set, verify that the SC exists in the SC Collection 
    // de-serialized from the Configuration file. If the SC Name is already set
    // then the correlation information specified by each raster file will not 
    // be used to determine teh SC in FDO
    bool bSCExists = false;
    if (scName != L"") {
        FdoPtr<FdoRfpSpatialContext> context;
        for (int i = 0; i<contexts->GetCount(); i++)
        {
            context = contexts->GetItem(i);
            if (STREQUAL(context->GetCoordinateSystem(), scName))
            {
                bSCExists = true;
                break;
            }
        }

        if (!bSCExists) {
            throw FdoException::Create(
                NlsMsgGet2(
                    GRFP_108_CLASS_SPATIAL_CONTEXT_NOT_DEFINED, 
                    "Spatial Context '%1$ls' defined for Class '%2$ls' was not found.",
                    (FdoString*)scName, fcName));
        }
    }
    else {
        // If the Association is not set, the SC name will be determined from
        // the correlation information from the raster images themselves.
        // Ensure every feature class has only one single CS name
        if (coordSystems->GetCount() == 0) {
            m_coord = defaultCoord;            
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

        // we must decide whether the coordinate system already exists in the providers
        // spatial context collection
        FdoPtr<FdoRfpSpatialContext> context;
        for (int i = 0; i<contexts->GetCount(); i++)
        {
            context = contexts->GetItem(i);
            if (STREQUAL(context->GetCoordinateSystem(), m_coord))
            {
                bSCExists = true;
                break;
            }
        }

        if (bSCExists) // the coordinate system exists
        {
            // Retrieve the SC name
            scName = context->GetName();
            FdoPtr<FdoByteArray> extentArr = context->GetExtent();
    		
            // Calculate the new extent of the existing spatial context
            FdoRfpRect extent = FdoRfpUtil::CreateRectFromGeometryAgf(extentArr);
            extent = extent.Union(m_extent);

            // Set the new extent
            context->SetExtent(FdoRfpUtil::CreateGeometryAgfFromRect(extent));
        }
        else // it's a new coordinate system
        {
            // spatial context name is set by coordinate system name
            // since coordinate system name is unique, the name of spatial context will be also unique.
            scName = m_coord;

            // Create a new SC
            context = new FdoRfpSpatialContext();

            // Set the SC properties
            context->SetName(scName);
            context->SetCoordinateSystem(m_coord);
            context->SetExtent(FdoRfpUtil::CreateGeometryAgfFromRect(m_extent));
            context->SetExtentType(FdoSpatialContextExtentType_Static);

            // Add the SC to the list of SC exposed by the RFP Provider
            contexts->Add(context);
        }
    	
        // Set the Class Definition's Spatial Context Association 
        propraster->SetSpatialContextAssociation(scName);
    }
}

// Extract all rasters from specified location, return value is the associated coordinate system name
void FdoRfpClassData::_buildUpGeoRastersFromLocation(FdoString* location, FdoStringCollection* coordSystems)
{
	FdoRfpGeoRasterExtractor extractor;
	extractor.ExtractRasters(location, m_geoRasters, m_coord, m_extent, m_bFirstRaster, coordSystems);
}
	
// Extract all rasters from image catalogue, return value is the associated coordinate system name
void FdoRfpClassData::_buildUpGeoRastersFromCatalogue(const FdoPtr<FdoGrfpRasterFeatureCollection>& featureCatalogue, FdoStringCollection* coordSystems)
{	
    VALIDATE_ARGUMENT(coordSystems);
    FdoPtr<FdoRfpDatasetCache>  datasetCache = FdoRfpDatasetCache::Create();
    
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

            // coordinate system handling
            //
            // first, get the coordinate system names associated to the rasters in the catalogue
            FdoStringP coord;
            if (bHasGeoInfo && geoRef->HasCoordSystem() && FdoRfpRasterUtil::IsCoordinateSystemValid(geoRef->GetCoorSystem())) 
                coord = geoRef->GetCoorSystem(); // it is a valid coordinate system

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

            FdoRfpGeoBandRasterP geoBandRaster = new FdoRfpGeoBandRasterRot(path, imageDef->GetFrameNumber()-1,
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


FdoRfpClassData* FdoRfpClassData::Create(const FdoPtr<FdoRfpSpatialContextCollection>& contexts, const FdoClassDefinitionP& classDefinition, const FdoPtr<FdoGrfpClassDefinition>& classMapping)
{
	FdoRfpClassData* classData = new FdoRfpClassData();
	classData->_buildUp(contexts, classDefinition, classMapping);
	return classData;
}

FdoRfpClassDataCollection* FdoRfpClassDataCollection::Create()
{
	return new FdoRfpClassDataCollection();
}
