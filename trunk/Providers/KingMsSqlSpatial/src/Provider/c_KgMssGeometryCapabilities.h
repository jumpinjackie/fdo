/*
* Copyright (C) 2007  Haris Kurtagic
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
*/

#ifndef _c_KGORAGEOMETRYCAPABILITIES_H
#define _c_KGORAGEOMETRYCAPABILITIES_H


class c_KgMssGeometryCapabilities : public FdoIGeometryCapabilities
{
public:
    c_KgMssGeometryCapabilities ();

protected:
    virtual ~c_KgMssGeometryCapabilities ();

protected:
    virtual void Dispose ();

public:
    /// \brief
    /// Return the list of supported geometry types. For example, if a client wanted to know if a provider supported
    /// multi-polygons, it would call GetGeometryTypes and check if the MultiPolygon type was listed.
    /// 
    /// \param length 
    /// Output the number of geometry types.
    /// 
    /// \return
    /// Returns the list of geometry types
    /// 
    FDOKGMSS_API virtual FdoGeometryType* GetGeometryTypes( FdoInt32& Length);

    /// \brief
    /// Return the list of supported component types. For example, if a client wanted to know if circular arcs were supported 
    /// by a provider, it would call GetGeometryComponentTypes and check for CircularArcSegment in the returned list.
    /// 
    /// \param length 
    /// Output the number of component types.
    /// 
    /// \return
    /// Returns the list of component types
    /// 
    FDOKGMSS_API virtual FdoGeometryComponentType* GetGeometryComponentTypes( FdoInt32& Length );

    /// \brief
    /// Returns the supported dimensionalities which are based on the bit masks defined in the GisDimensionality enum. 
    /// The Z and M bits indicate if the provider supports 3d or Measure dimensions in geometry data. The XY bit should be set too.
    /// 
    /// \return
    /// Returns the dimensionalities
    /// 
    FDOKGMSS_API virtual FdoInt32 GetDimensionalities();
};

#endif 

