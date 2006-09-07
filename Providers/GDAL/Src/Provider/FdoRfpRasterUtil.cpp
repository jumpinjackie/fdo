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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpRasterUtil.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-02-02 15:58:16 -0500 (Thu, 02 Feb 2006) $
 * $Revision: 31 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpRasterUtil.h"
#include "FdoRfpGlobals.h"

#include "FdoRfpSpatialContext.h"
#include "FdoRfpGeoreference.h"
#include "FdoRfpKeyColor.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "FdoCommonStringUtil.h"
#include <gdal.h>

/// <summary>Get the key color list from a image file
/// It's used by FdoIRaster::GetNullPixelValue()
/// </summary>
/// <param name="name">Input the raster iamge file name</param> 
/// <returns>Returns true if key color is retrieved correctly.
/// return value is a collection of FdoRfpKeyColor
/// </returns> 
bool FdoRfpRasterUtil::GetKeyColor(FdoStringP filePath, FdoPtr<FdoRfpKeyColorCollection>& keyColorColl)
{
    return false;

// TODO

#ifdef notdef
    bool bFound = false;
    char* mbFilePath = NULL;
    wide_to_multibyte(mbFilePath, filePath);

    try 
    {
        const ImageFormatCodec* codec = NULL;
        Atil::FileSpecifier pathName(StringBuffer((int)(strlen(mbFilePath)+1), (Atil::Byte *) mbFilePath), FileSpecifier::kFilePath);
        ImageFormatCodec::FormatDecodability decodability =  FileReadDescriptor::findImageFileCodec(pathName, codec);
        if (decodability == ImageFormatCodec::kReadable)
        {
            FileReadDescriptor fRd(pathName);
            FormatCodecPropertyInterface* prop = fRd.getProperty(FormatCodecPropertyInterface::kTransparency);
            FormatCodecTransparency* corr = static_cast<FormatCodecTransparency*>(prop);

            if (corr != NULL)
            {
                FormatCodecTransparency* pTrans = (FormatCodecTransparency*)prop;
                int num = pTrans->numKeyColors();
                int i;
                Atil::FormatCodecTransparency::Model model = pTrans->getTransparencyModel();

                if (model == Atil::FormatCodecTransparency::kKeyColorList)
                {						
                    switch (pTrans->keyColorComponentBits())
                    {					   
                      case 8:		
                        break;
                      case 16:
                      {
                          Atil::Data16* a16 = new Atil::Data16[num];
                          pTrans->getKeyColors(a16);
                          for (i=0; i<num; i++)
                          {
                              FdoRfpKeyColorP keyColor = new FdoRfpKeyColor();
                              keyColor->value.data16 = a16[i];
                              keyColor->type = FdoRfpKeyColor::kData16;
                              keyColorColl->Add(keyColor);
                          }
                          delete a16;
                          a16 = NULL;
                      }
                      break;
                      case 24:
                      {
                          Atil::RgbColor* a24 = new Atil::RgbColor[num];
                          pTrans->getKeyColors(a24);
                          for (i=0; i<num; i++)
                          {
                              FdoRfpKeyColorP keyColor = new FdoRfpKeyColor();
                              keyColor->value.data32 = a24[i];
                              keyColor->type = FdoRfpKeyColor::kData32;
                              keyColorColl->Add(keyColor);
                          }
                          delete a24;
                          a24 = NULL;
                      }
                      break;
                      case 32:
                      {
                          Atil::Data32* a32 = new Atil::Data32[num];
                          pTrans->getKeyColors(a32);
                          for (i=0; i<num; i++)
                          {
                              FdoRfpKeyColorP keyColor = new FdoRfpKeyColor();
                              keyColor->value.data32 = a32[i];
                              keyColor->type = FdoRfpKeyColor::kData32;
                              keyColorColl->Add(keyColor);
                          }
                          delete a32;
                          a32 = NULL;
                      }
                      break;
                      case 64:
                      {
                          Atil::Data64* a64 = new Atil::Data64[num];
                          pTrans->getKeyColors(a64);
                          for (i=0; i<num; i++)
                          {
                              FdoRfpKeyColorP keyColor = new FdoRfpKeyColor();
                              keyColor->value.data64 = a64[i];
                              keyColor->type = FdoRfpKeyColor::kData64;
                              keyColorColl->Add(keyColor);
                          }
                          delete a64;
                          a64 = NULL;
                      }
                      break;
                    }
                }
                else if (model == Atil::FormatCodecTransparency::kGrayAlpha)
                {
                }
                bFound = true;
            }
        }
    }
    catch (Atil::ATILException* e)
    {
        std::auto_ptr<Atil::ATILException> spe(e);
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_42_GENERICCHAR, "%1$s", spe->getMessage()->data()));		
    }

    return keyColorColl->GetCount() > 0;
#endif
}


/// <summary>Get the geo reference infomation associated with the raster image.</summary>
/// <param name="name">Input the raster iamge file name</param> 
/// <returns>Returns true if geo reference is retrieved correctly.
/// return value is a FdoRfpGeoreference recording the geo information.
/// </returns> 
bool FdoRfpRasterUtil::GetGeoReferenceInfo(GDALDatasetH hDS, FdoPtr<FdoRfpGeoreference>& geoRef)
{
#ifdef notdef
    char* mbFilePath = NULL;
    wide_to_multibyte(mbFilePath, filePath);
#endif

    double adfGeoTransform[6];

    GDALGetGeoTransform( hDS, adfGeoTransform );

    // TODO Set rotation properly!
    geoRef->SetXInsertion( adfGeoTransform[0] );
    geoRef->SetYInsertion( adfGeoTransform[3]+adfGeoTransform[5]*GDALGetRasterYSize(hDS) );
    geoRef->SetXResolution( adfGeoTransform[1] );
    geoRef->SetYResolution( fabs(adfGeoTransform[5]) );
    geoRef->SetXRotation( 0.0 );
    geoRef->SetYRotation( 0.0 );

    const char *pszWKT = GDALGetProjectionRef( hDS );
    if ( pszWKT != NULL && strlen(pszWKT) > 0 )
    {
        // I think we should just be assigning a coordinate
        // system name here, not the whole WKT definition.

        // Commented out to avoid "multiple coordinate system" problems.
        //geoRef->SetCoordSystem(pszWKT);
    }

    return true;
}

/// <summary>Determines whether two spatial contexts are equal,
/// Currently it's decided by whether the coordinate systems 
/// contained in spatial contexts are equal.
/// </summary>
/// <param name="name">Input two spatial contexts to be compared</param> 
/// <returns>Returns true if they are equal</returns> 
bool FdoRfpRasterUtil::IsContextEqual(FdoPtr<FdoRfpSpatialContext>& context1, FdoPtr<FdoRfpSpatialContext>& context2)
{
    if (STREQUAL(context1->GetCoordinateSystem(), context2->GetCoordinateSystem()))
        return true;
    else
        return false;
}

/// <summary>Determines whether a coordinate system name is valid</summary>
/// <param name="name">Input the coordinate system name</param> 
/// <returns>Returns true if the coordinate system name is valid</returns> 
bool FdoRfpRasterUtil::IsCoordinateSystemValid(FdoString* coordSysName)
{
    if (coordSysName == NULL ||  STREQUAL(coordSysName, FdoGrfpGlobals::UserDefinedCoordName))
        return false;
    else
        return true;
}


