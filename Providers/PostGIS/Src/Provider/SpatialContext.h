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

#include "PostGisProvider.h" // Includes FDO headers

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
    
    /// Get SRID number associated with the spatial context.
    FdoInt32 GetSRID() const;

    /// Set SRID value identifying definition of spatial context.
    void SetSRID(FdoInt32 srid);

    /// Get name of the spatial context.
    /// The PostGIS provider composes spatial context name by concatenating
    /// "PostGIS" string with SRID number separated by underscore.
    /// For example: PostGIS_4326
    ///
    virtual FdoString* GetName() const;

    /// Set name of the spatial context.
    virtual void SetName(FdoString* name);

    /// Get description of the spatial context.
    /// Currently, full description is only provided for default spatial context.
    /// PostGIS meta-schema does not include anything like longer
    /// description that could be returned here.
    /// 
    /// \todo Where we could get the SC description from?
    /// 
    virtual FdoString* GetDescription() const;

    /// Set description of the spatial context.
    /// Currently, this description is not stored anywhere.
    /// 
    /// \todo Where we could store the SC description?
    /// 
    virtual void SetDescription(FdoString* desc);

    /// Get name of coordinate system.
    /// The name is taken off from the coordinate system WKT string.
    /// 
    virtual FdoString* GetCoordinateSystem() const;

    /// Set name of coordinate system.
    virtual void SetCoordinateSystem(FdoString* csName);

    /// Get coordinate system parameters encoded as WKT string.
    virtual FdoString* GetCoordinateSystemWkt() const;

    /// Set coordinate system parameters by passing WKT string.
    virtual void SetCoordinateSystemWkt(FdoString* csWkt);

    /// \todo To be documented
    virtual FdoSpatialContextExtentType GetExtentType() const;

    /// \todo To be documented
    virtual void SetExtentType(FdoSpatialContextExtentType type);

    /// \todo To be documented
    virtual FdoByteArray* GetExtent() const;

    /// \todo To be documented
    virtual void SetExtent(FdoByteArray* extent);
    virtual void SetExtent(FdoEnvelopeImpl* extent);

    /// \todo To be documented
    virtual double GetXYTolerance() const;

    /// \todo To be documented
    virtual void SetXYTolerance(double xyTolerance);

    /// \todo To be documented
    virtual double GetZTolerance() const;

    /// \todo To be documented
    virtual void SetZTolerance(double zTolerance);

    /// \todo To be documented
    virtual double GetMTolerance() const;

    /// \todo To be documented
    virtual void SetMTolerance(double mTolerance);
    
    /// \todo To be documented
	bool GetIsExtentUpdated() const;

    /// \todo To be documented
	void SetIsExtentUpdated(bool isUpdated);

    //
    // FdoNamedCollection interface
    //

    /// Check if the object does allow to change its name.
    /// Not allowing name change allows more efficient random
    /// access to FdoDictionary.
    /// \return
    /// Always return true is returned.
    FdoBoolean CanSetName() const;

private:
	
    FdoInt32 mSRID;
	FdoStringP mName;
	FdoStringP mDescription;
	FdoStringP mCoordSysName;
	FdoStringP mCoordSysWkt;
	
	FdoSpatialContextExtentType mExtentType;
	FdoPtr<FdoEnvelopeImpl> mExtent;
	bool mIsExtentUpdated;
	
	double mXYTolerance;
	double mZTolerance;
	double mMTolerance;

};


inline bool SpatialContext::CanSetName() const
{
    return true;
}

inline bool SpatialContext::GetIsExtentUpdated() const
{
    return mIsExtentUpdated;
}

inline void SpatialContext::SetIsExtentUpdated(bool isUpdated)
{
    mIsExtentUpdated = isUpdated;
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SPATIALCONTEXT_H_INCLUDED
