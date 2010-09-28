//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_RASTERCAPABILITIES_H_INCLUDED
#define FDOPOSTGIS_RASTERCAPABILITIES_H_INCLUDED

//
// Forward declarations
//
class FdoRasterDataModel;

namespace fdo { namespace postgis {

/// Implementation of interface describing raster capabilities
/// supported by PostGIS provider.
///
class RasterCapabilities : public FdoIRasterCapabilities
{
public:

    /// Default constructor.
    RasterCapabilities();

    //
    // FdoIRasterCapabilities interface
    //

    /// Return true if the provider has raster capability.
    bool SupportsRaster();

    /// Return true if the provider has the capability to stitch images
    /// from a select command (query) together into a single image.        
    bool SupportsStitching();
        
    /// Return true if the provider can reduce the resolution of an image.
    bool SupportsSubsampling();

    /// Return true if the provider can handle the provided data model.        
    bool SupportsDataModel(FdoRasterDataModel *model);

protected:

    /// Destructor.
    virtual ~RasterCapabilities();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_RASTERCAPABILITIES_H_INCLUDED
