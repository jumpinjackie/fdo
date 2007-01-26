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
 */

#include "stdafx.h"
#include "FdoWfsServiceMetadata.h"
#include "FdoWfsFeatureType.h"
#include "FdoWfsFeatureTypeList.h"
#include "FdoWfsOgcFilterCapabilities.h"
#include "FdoWfsCapabilitiesSaxContext.h"

FdoWfsServiceMetadata::FdoWfsServiceMetadata()
{
    m_featureTypeList = FdoWfsFeatureTypeList::Create();
    m_filterCapabilities = FdoWfsOgcFilterCapabilities::Create();
}

FdoWfsServiceMetadata::~FdoWfsServiceMetadata()
{
}

FdoWfsServiceMetadata* FdoWfsServiceMetadata::Create()
{
    return new FdoWfsServiceMetadata();
}

FdoWfsFeatureTypeList* FdoWfsServiceMetadata::GetFeatureTypeList()
{
    return FDO_SAFE_ADDREF(m_featureTypeList.p);
}

FdoWfsOgcFilterCapabilities* FdoWfsServiceMetadata::GetOGCFilterCapabilities()
{
    return FDO_SAFE_ADDREF(m_filterCapabilities.p);
}

FdoStringCollection* FdoWfsServiceMetadata::GetCRSNames ()
{
	_buildUpCRS ();
	return FDO_SAFE_ADDREF (m_CRSNames.p);
}

FdoOwsGeographicBoundingBoxCollection* FdoWfsServiceMetadata::GetCRSExtents ()
{
	_buildUpCRS ();
	return FDO_SAFE_ADDREF (m_CRSExtents.p);
}

FdoXmlSaxHandler* FdoWfsServiceMetadata::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));
		
        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);

        if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::FeatureTypeList) == 0)
        {
            m_featureTypeList->InitFromXml(context, atts);
            pRet = m_featureTypeList.p;
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Filter_Capabilities) == 0)
        {
            m_filterCapabilities->InitFromXml(context, atts);
            pRet = m_filterCapabilities.p;
        }
        else
            pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);

	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;

}

FdoBoolean FdoWfsServiceMetadata::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));
		
        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);
        if (!(FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::FeatureTypeList) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Filter_Capabilities) == 0))
            ret = BaseType::XmlEndElement(context, uri, name, qname);

	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return ret;
}


FdoXmlSaxContext* FdoWfsServiceMetadata::GetSaxContext()
{
    return new FdoWfsCapabilitiesSaxContext(FdoPtr<FdoXmlReader>(GetXmlReader()));
}

void FdoWfsServiceMetadata::_buildUpCRS ()
{
	if (m_CRSNames && m_CRSExtents)
	{
		// the CRS information has been built up already.
		return;
	}
	else
	{
		m_CRSNames = FdoStringCollection::Create ();	
		m_CRSExtents = FdoOwsGeographicBoundingBoxCollection::Create ();
	}

	FdoWfsFeatureTypesP featTypes = m_featureTypeList->GetFeatureTypes ();
	FdoInt32 cntFeatTypes = featTypes->GetCount ();

	// get a unique set of the CRS names. 
	for (FdoInt32 i=0; i<cntFeatTypes; i++)
	{
		FdoWfsFeatureTypeP featType = featTypes->GetItem (i);
		FdoStringP crsName = featType->GetSRS ();
		if (m_CRSNames->IndexOf (crsName) == -1)
		{
			m_CRSNames->Add (crsName);
		}
	}

	// for each CRS, calculate the corresponding extent.
	FdoInt32 cntCRS = m_CRSNames->GetCount ();
	for (FdoInt32 i=0; i<cntCRS; i++)
	{
		FdoOwsGeographicBoundingBoxP bbox = FdoOwsGeographicBoundingBox::Create ();
		m_CRSExtents->Add (bbox);
		FdoString* crsName = m_CRSNames->GetString (i);
		bool bFirstMatch = true;

		FdoInt32 cntFeatTypes = featTypes->GetCount ();
		for (FdoInt32 j=0; j<cntFeatTypes; j++)
		{
			FdoWfsFeatureTypeP featType = featTypes->GetItem (j);
			FdoString* name = featType->GetSRS ();
			if (wcscmp (crsName, name) == 0)
			{
				// get the collection of the extents of this feature type
				FdoOwsGeographicBoundingBoxCollectionP extents = featType->GetSRSExtents ();
				FdoInt32 cntBoxes = extents->GetCount ();
				for (FdoInt32 k=0; k<cntBoxes; k++)
				{
					FdoOwsGeographicBoundingBoxP extent = extents->GetItem (k);
					if (bFirstMatch)
					{
						bbox->SetEastBoundLongitude (extent->GetEastBoundLongitude ());
						bbox->SetNorthBoundLatitude (extent->GetNorthBoundLatitude ());
						bbox->SetSouthBoundLatitude (extent->GetSouthBoundLatitude ());
						bbox->SetWestBoundLongitude (extent->GetWestBoundLongitude ());
						
						bFirstMatch = false;
					}
					else
					{
						_getTotalExtent (bbox, extent);
					}
				}
			}
		}
	}
}

void FdoWfsServiceMetadata::_getTotalExtent (FdoOwsGeographicBoundingBox* desBox, const FdoOwsGeographicBoundingBox* srcBox)
{
	if (desBox->GetWestBoundLongitude () > srcBox->GetWestBoundLongitude ())
		desBox->SetWestBoundLongitude (srcBox->GetWestBoundLongitude ());
	if (desBox->GetEastBoundLongitude () < srcBox->GetEastBoundLongitude ())
		desBox->SetEastBoundLongitude (srcBox->GetEastBoundLongitude ());
	if (desBox->GetNorthBoundLatitude () < srcBox->GetNorthBoundLatitude ())
		desBox->SetNorthBoundLatitude (srcBox->GetNorthBoundLatitude ());
	if (desBox->GetSouthBoundLatitude () > srcBox->GetSouthBoundLatitude ())
		desBox->SetSouthBoundLatitude (srcBox->GetSouthBoundLatitude ());
}

FdoWfsFeatureType* FdoWfsServiceMetadata::GetFeatureType(FdoIdentifier* className)
{
    FdoPtr<FdoWfsFeatureTypeCollection> featTypes = m_featureTypeList->GetFeatureTypes();
    FdoPtr<FdoWfsFeatureType> featureType = featTypes->FindItem (className->GetName());
    if (featureType == NULL)
    {
        featureType = featTypes->FindItem (className->GetText());
        if (featureType == NULL)
        {
            std::wstring lhs = className->GetName();
	        for (int i = 0; i < featTypes->GetCount(); i++)
            {
		        FdoPtr<FdoWfsFeatureType> fType = featTypes->GetItem(i);
                std::wstring rhs = fType->GetName();
                std::wstring::size_type idxSep = rhs.find(L':');
                bool foundit = false;
                if (idxSep != std::wstring::npos)
                {
                    std::wstring::size_type szComp = rhs.size() - (idxSep+1);
                    if(lhs.size() == szComp && !rhs.compare(idxSep+1, szComp < lhs.size() ? lhs.size() : szComp, lhs))
                        foundit = true;
                }
                else {
                    if (rhs == className->GetName())
                        foundit = true;
                }
                if (foundit)
                {
                    featureType = fType;
                    break;
                }
            }
        }
    }
    return FDO_SAFE_ADDREF (featureType.p);
}
