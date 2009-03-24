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


#pragma once

#include "mgIRingAbstract.h"

class FdoIRingAbstract;
BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IEnvelope;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The IRingAbstractImp class is a ring Geometry helper type (abstract). FdoIRingAbstract is the most general ring type.
/// It is similar to CurveAbstract, but is always closed.
public ref class IRingAbstractImp : 
	public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IRingAbstract
{
public:
    /// \brief
    /// Constructs a managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed element should be automatically deleted 
    /// once it no longer referenced.
    /// 
	IRingAbstractImp(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    /// \brief
    /// Gets the envelope for the object.
    /// 
    /// \return
    /// Returns the envelope
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ Envelope
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ get();
    }

    /// \brief
    /// Gets the dimensionality of ordinates in this object.
    /// 
    /// \remarks
    ///  Values are from the Dimensionality enum.
    /// A return type of "Int32" is used instead of the enum, catering to typical use with bit masking.
    /// 
    /// \return
    /// Returns the ordinate dimensionality
    /// 
    property System::Int32 Dimensionality
    {
        virtual System::Int32 get();
    }

internal:
	inline FdoIRingAbstract* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_GEOMETRY


