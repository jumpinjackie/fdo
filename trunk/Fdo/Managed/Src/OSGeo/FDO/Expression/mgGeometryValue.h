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

#include "FDO\Expression\mgLiteralValue.h"

class FdoGeometryValue;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoGeometryValue class derives from FdoLiteralValue and represents a geometric value.
public __gc class GeometryValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue
{
public:

    /// \brief
    /// Constructs a default instance of an FdoGeometryValue with a value of null.
    /// 
    /// \return
    /// Returns FdoGeometryValue
    /// 
	GeometryValue();

    /// \brief
    /// Constructs an instance of an FdoGeometryValue using the specified arguments.
    /// 
    /// \param geometry 
    /// Input geometry byte array
    /// 
    /// \return
    /// Returns FdoGeometryValue
    /// 
	GeometryValue(System::Byte geometry []);

    /// \brief
    /// Returns true if the FdoGeometryValue represents a null value.
    /// 
    /// \return
    /// Returns true if null geometry
    /// 
	System::Boolean IsNull();

    /// \brief
    /// Gets the geometry as a byte array. The geometry is expected to be
    /// encoded in Autodesk Well-Known Binary format.
    /// 
    /// \return
    /// Returns geometry in FGF format
    /// 
	__property System::Byte get_Geometry() [];

    /// \brief
    /// Sets the geometry as a byte array. The geometry is expected to be
    /// encoded in Autodesk Well-Known Binary format.
    /// 
    /// \param value 
    /// Input geometry in FGF format
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Geometry(System::Byte value []);

    /// \brief
    /// Sets the FdoGeometryValue to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetNull();

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoGeometryValue to the
    /// appropriate expression processor operation.
    /// 
    /// \param processor 
    /// Input the expression processor interface
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns the well defined text string
    /// 
	System::String* ToString();

	GeometryValue(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoGeometryValue* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


