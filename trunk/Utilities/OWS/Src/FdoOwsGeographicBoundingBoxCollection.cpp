 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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

#include <stdafx.h>
#include <OWS/FdoOwsGeographicBoundingBoxCollection.h>

FdoOwsGeographicBoundingBoxCollection* FdoOwsGeographicBoundingBoxCollection::Create ()
{
	return new FdoOwsGeographicBoundingBoxCollection ();
}

FdoOwsGeographicBoundingBoxCollection::FdoOwsGeographicBoundingBoxCollection ()
{
}

FdoOwsGeographicBoundingBoxCollection::~FdoOwsGeographicBoundingBoxCollection (void)
{
}

void FdoOwsGeographicBoundingBoxCollection::Dispose ()
{
	delete this;
}

FdoOwsGeographicBoundingBox* FdoOwsGeographicBoundingBoxCollection::GetExtents () const
{
	FdoBoolean bFirstMatch = true;
    FdoOwsGeographicBoundingBoxP bbox = FdoOwsGeographicBoundingBox::Create ();
	for (FdoInt32 k=0; k<GetCount(); k++)
	{
		FdoOwsGeographicBoundingBoxP extent = GetItem (k);
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
	        if (bbox->GetWestBoundLongitude () > extent->GetWestBoundLongitude ())
		        bbox->SetWestBoundLongitude (extent->GetWestBoundLongitude ());
	        if (bbox->GetEastBoundLongitude () < extent->GetEastBoundLongitude ())
		        bbox->SetEastBoundLongitude (extent->GetEastBoundLongitude ());
	        if (bbox->GetNorthBoundLatitude () < extent->GetNorthBoundLatitude ())
		        bbox->SetNorthBoundLatitude (extent->GetNorthBoundLatitude ());
	        if (bbox->GetSouthBoundLatitude () > extent->GetSouthBoundLatitude ())
		        bbox->SetSouthBoundLatitude (extent->GetSouthBoundLatitude ());
		}
	}

    return FDO_SAFE_ADDREF(bbox.p);
}
