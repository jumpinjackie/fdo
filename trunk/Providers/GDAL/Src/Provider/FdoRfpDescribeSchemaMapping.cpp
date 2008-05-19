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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpDescribeSchemaMapping.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */


#include "FDORFP.h"
#include "FdoRfpConnection.h"
#include "FdoRfpDescribeSchemaMapping.h"
#include <GdalFile/Override/FdoGrfpOverrides.h>

FdoRfpDescribeSchemaMapping::FdoRfpDescribeSchemaMapping(FdoIConnection* connection) :
	FdoRfpCommand<FdoIDescribeSchemaMapping>(connection), m_includeDefaults(true)
{
}
FdoRfpDescribeSchemaMapping::~FdoRfpDescribeSchemaMapping()
{
}

/// <summary>Gets the name of the schema for which to describe
/// schema mappings.</summary>
/// <returns>Returns the schema name</returns> 
FdoString* FdoRfpDescribeSchemaMapping::GetSchemaName()
{
	return m_schemaName;
}

/// <summary>Sets the name of the schema to describe. This function is optional; if not
/// specified, execution of the command will describe the mappings for
/// all schemas.</summary>
/// <param name="value">Input the schema name</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpDescribeSchemaMapping::SetSchemaName(FdoString* value)
{
	m_schemaName = value;
}

/// <summary>Gets the current "include default mappings" setting.</summary>
/// <returns>Returns True if Execute() will include default mappings.</returns> 
FdoBoolean FdoRfpDescribeSchemaMapping::GetIncludeDefaults()
{
	return m_includeDefaults;
}

/// <summary>Sets the "include default mappings" setting.</summary>
/// <param name="includeDefaults">Input True: Execute() will return
/// all mappings for the feature schema(s).
/// False: Execute() will not include default logical to physical
/// mappings, only those mappings that have been overridden will 
/// be returned.</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpDescribeSchemaMapping::SetIncludeDefaults( FdoBoolean includeDefaults )
{
	m_includeDefaults = includeDefaults;
}

/// <summary>Executes the DescribeSchemaMapping command and returns a 
/// FdoPhysicalSchemaMappingCollection. If the specified schema name does not exist,
/// the Execute method throws an exception.</summary>
/// <returns>Returns the schema mapping collection for the requested feature schemas.</returns> 
FdoPhysicalSchemaMappingCollection* FdoRfpDescribeSchemaMapping::Execute()
{
    FdoRfpConnectionP connection = static_cast<FdoRfpConnection*>(GetConnection());
    FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = connection->GetSchemaMappings();
    FdoPtr<FdoPhysicalSchemaMappingCollection> ret = FdoPhysicalSchemaMappingCollection::Create();
    FdoInt32 count = schemaMappings->GetCount();
    if (m_schemaName.GetLength() == 0) // return all schema mappings
    {
        for (int i = 0; i < count; i++)
        {
            FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = schemaMappings->GetItem(i);
            schemaMapping = _cloneSchemaMapping(schemaMapping);
            ret->Add(schemaMapping);
        }
    }
    else	// only return the schema mapping specified by the name	
    {
        for (int i = 0; i < count; i++)
        {
            FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = schemaMappings->GetItem(i);
            if (STRCASEEQ(m_schemaName, schemaMapping->GetName()))
            {
                schemaMapping = _cloneSchemaMapping(schemaMapping);
                ret->Add(schemaMapping);
            }
        }
        if (ret->GetCount() == 0)
            throw FdoCommandException::Create(NlsMsgGet1(GRFP_75_SCHEMA_MAPPING_NOT_FOUND, "Feature schema mapping '%1$ls' not found.", (const wchar_t*)m_schemaName));
    }

    return FDO_SAFE_ADDREF(ret.p);
}

FdoPtr<FdoGrfpClassDefinition> FdoRfpDescribeSchemaMapping::_cloneClass(const FdoPtr<FdoGrfpClassDefinition>& classDef)
{
    // suffix "l" means left, suffix "r" means right
    FdoPtr<FdoGrfpClassDefinition> ret = FdoGrfpClassDefinition::Create();
    // name
    ret->SetName(classDef->GetName());
    // copy raster definition
    FdoPtr<FdoGrfpRasterDefinition> rasterDefl = FdoGrfpRasterDefinition::Create();
    ret->SetRasterDefinition(rasterDefl);
    FdoPtr<FdoGrfpRasterDefinition> rasterDefr = classDef->GetRasterDefinition();
    // name
    rasterDefl->SetName(rasterDefr->GetName());
    // locations
    FdoPtr<FdoGrfpRasterLocationCollection> locationsl = rasterDefl->GetLocations();
    FdoPtr<FdoGrfpRasterLocationCollection> locationsr = rasterDefr->GetLocations();
    FdoInt32 count = locationsr->GetCount();

    int i, j, k;
    // clone location one by one
    for (i = 0; i < count; i++)
    {
        FdoPtr<FdoGrfpRasterLocation> locationl = FdoGrfpRasterLocation::Create();
        locationsl->Add(locationl);
        FdoPtr<FdoGrfpRasterLocation> locationr = locationsr->GetItem(i);
        // copy location name
        locationl->SetName(locationr->GetName());

        // raster features;
        FdoPtr<FdoGrfpRasterFeatureCollection> rasterFeaturesl = locationl->GetFeatureCatalogue();
        FdoPtr<FdoGrfpRasterFeatureCollection> rasterFeaturesr = locationr->GetFeatureCatalogue();		
        FdoInt32 countFeatures = rasterFeaturesr->GetCount();
        for (k = 0; k < countFeatures; k++)
        {
            // copy feature one by one
            FdoGrfpRasterFeatureDefinitionP rasterFeaturel = FdoGrfpRasterFeatureDefinition::Create();
            rasterFeaturesl->Add(rasterFeaturel);
            FdoGrfpRasterFeatureDefinitionP rasterFeaturer = rasterFeaturesr->GetItem(k);
            // copy feature name
            rasterFeaturel->SetName(rasterFeaturer->GetName());

            // raster bands
            FdoPtr<FdoGrfpRasterBandCollection> rasterBandsl = rasterFeaturel->GetBands();
            FdoPtr<FdoGrfpRasterBandCollection> rasterBandsr = rasterFeaturer->GetBands();			
            FdoInt32 countBands = rasterBandsr->GetCount();
			
            // clone raster band one by one
            for (j = 0; j < countBands; j++)
            {
                // raster image
                FdoPtr<FdoGrfpRasterBandDefinition> rasterBandl = FdoGrfpRasterBandDefinition::Create();
                rasterBandsl->Add(rasterBandl);
                FdoPtr<FdoGrfpRasterBandDefinition> rasterBandr = rasterBandsr->GetItem(j);
                rasterBandl->SetName(rasterBandr->GetName());
                rasterBandl->SetBandNumber(rasterBandr->GetBandNumber());

                FdoPtr<FdoGrfpRasterImageDefinition> rasterImagel = FdoGrfpRasterImageDefinition::Create();
                rasterBandl->SetImage(rasterImagel);
                FdoPtr<FdoGrfpRasterImageDefinition> rasterImager = rasterBandr->GetImage();
                FdoGrfpRasterGeoreferenceLocationP geoReference_r = rasterImager->GetGeoreferencedLocation();

                FdoGrfpRasterGeoreferenceLocationP geoReference1 = FdoGrfpRasterGeoreferenceLocation::Create();
                rasterImagel->SetGeoreferencedLocation(geoReference1);

                // copy raster image name
                rasterImagel->SetName(rasterImager->GetName());
                rasterImagel->SetFrameNumber(rasterImager->GetFrameNumber());

                // extents
                geoReference1->SetXInsertionPoint(geoReference_r->GetXInsertionPoint());
                geoReference1->SetYInsertionPoint(geoReference_r->GetYInsertionPoint());
                geoReference1->SetXResolution(geoReference_r->GetXResolution());
                geoReference1->SetYResolution(geoReference_r->GetYResolution());
                geoReference1->SetXRotation(geoReference_r->GetXRotation());
                geoReference1->SetYRotation(geoReference_r->GetYRotation());
            }
        }
    }

    return ret;

}

FdoPtr<FdoPhysicalSchemaMapping> FdoRfpDescribeSchemaMapping::_cloneSchemaMapping(const FdoPtr<FdoPhysicalSchemaMapping>& schemaMapping)
{
    FdoPtr<FdoGrfpPhysicalSchemaMapping> schemaMappingl = FdoGrfpPhysicalSchemaMapping::Create();
    FdoPtr<FdoGrfpPhysicalSchemaMapping> schemaMappingr = SP_STATIC_CAST(FdoGrfpPhysicalSchemaMapping, schemaMapping);
    // name
    schemaMappingl->SetName(schemaMapping->GetName());
    // classes
    FdoPtr<FdoGrfpClassCollection> classesl = schemaMappingl->GetClasses();
    FdoPtr<FdoGrfpClassCollection> classesr = schemaMappingr->GetClasses();
    FdoInt32 count = classesr->GetCount();
    for (int i = 0; i < count; i++)
    {
        FdoPtr<FdoGrfpClassDefinition> classr = classesr->GetItem(i);
        FdoPtr<FdoGrfpClassDefinition> classl = _cloneClass(classr);
        classesl->Add(classl);
    }
    return FDO_SAFE_ADDREF(schemaMappingl.p);
}


