/*
* Copyright (C) 2006  SL-King d.o.o
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

#ifndef _c_KGORAFILTERCAPABILITIES_H
#define _c_KGORAFILTERCAPABILITIES_H


class c_KgOraFilterCapabilities : public FdoIFilterCapabilities
{
public:
    c_KgOraFilterCapabilities ();

protected:
    virtual ~c_KgOraFilterCapabilities ();

protected:
    virtual void Dispose();

public:
   /// \brief
    /// Returns an array of FdoConditionType objects the feature provider supports.
    /// 
    /// \param length 
    /// Output the number of condition types
    /// 
    /// \return
    /// Returns the list of condition types
    /// 
    FDOKGORA_API virtual FdoConditionType* GetConditionTypes(FdoInt32& length);

    /// \brief
    /// Returns an array of FdoSpatialOperation objects the feature provider supports.
    /// 
    /// \param length 
    /// Output the number of spatial operations
    /// 
    /// \return
    /// Returns the list of spartial operations
    /// 
    FDOKGORA_API virtual FdoSpatialOperations* GetSpatialOperations(FdoInt32& length);

    /// \brief
    /// Returns an array of FdoDistanceOperation objects the feature provider supports.
    /// 
    /// \param length 
    /// Output the number of distance operations
    /// 
    /// \return
    /// Returns the list of distance operations
    /// 
    FDOKGORA_API virtual FdoDistanceOperations* GetDistanceOperations(FdoInt32& length);

    /// \brief
    /// Determines if the feature provider supports geodesic distance measurement. Returns false if the feature provider supports only linear distance measurement.
    /// 
    /// \return
    /// Returns true if the feature provider supports geodesic distance measurement
    /// 
    FDOKGORA_API virtual bool SupportsGeodesicDistance();

    /// \brief
    /// Determines if spatial and distance operations can be applied between two 
    /// geometric properties. Returns false if spatial and distance operations can be applied only between 
    /// a geometric property and a literal geometry.
    /// 
    /// \return
    /// Returns true if spatial and distance operations can be applied
    /// 
    FDOKGORA_API virtual bool SupportsNonLiteralGeometricOperations();
};

#endif 

