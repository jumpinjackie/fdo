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
#ifndef FDOPOSTGIS_SPATIALCONTEXT_H_INCLUDED
#define FDOPOSTGIS_SPATIALCONTEXT_H_INCLUDED

#include "PostGisProvider.h"

#include <cassert>

namespace fdo { namespace postgis {

/// Defines concept of Spatial Context for PostGIS provider.
///
class SpatialContext : public FdoDisposable
{
public:

    /// Type of FDO smart pointer for Connection class.
    typedef FdoPtr<SpatialContext> Ptr;
    
    /// Default constructor.
    SpatialContext();

    /// Destructor.
    virtual ~SpatialContext();
    
    /// \todo To be documented
    virtual FdoString* GetName() const;

    /// \todo To be documented
    virtual void SetName(FdoString* name);

    /// \todo To be documented
    virtual FdoString* GetDescription() const;

    /// \todo To be documented
    virtual void SetDescription(FdoString* desc);

    /// \todo To be documented
    virtual FdoString* GetCoordinateSystem() const;

    /// \todo To be documented
    virtual void SetCoordSysName(FdoString* csName);

    /// \todo To be documented
    virtual FdoString* GetCoordinateSystemWkt() const;

    /// \todo To be documented
    virtual void SetCoordinateSystemWkt(FdoString* wkt);

    /// \todo To be documented
    virtual FdoSpatialContextExtentType GetExtentType() const;

    /// \todo To be documented
    virtual void SetExtentType(FdoSpatialContextExtentType type);

    /// \todo To be documented
    virtual FdoByteArray* GetExtent() const;

    /// \todo To be documented
    virtual void SetExtent(FdoByteArray* extent);

    /// \todo To be documented
    virtual double GetXYTolerance() const;

    /// \todo To be documented
    virtual void SetXYTolerance(double tol);

    /// \todo To be documented
    virtual double GetZTolerance() const;

    /// \todo To be documented
    virtual void SetZTolerance(double tol);

    /// \todo To be documented
	bool GetIsExtentUpdated() const;

    /// \todo To be documented
	void SetIsExtentUpdated(bool value);

    //
    // FdoNamedCollection interface
    //

    /// Check if the object does allow to change its name.
    /// Not allowing name change allows more efficient random access to FdoDictionary.
    FdoBoolean CanSetName() const;


protected:

private:
	
};


inline bool SpatialContext::CanSetName() const
{
    return true;
}

inline bool SpatialContext::GetIsExtentUpdated() const
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

inline void SpatialContext::SetIsExtentUpdated(bool value)
{
    assert(!"NOT IMPLEMENTED");
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SPATIALCONTEXT_H_INCLUDED
