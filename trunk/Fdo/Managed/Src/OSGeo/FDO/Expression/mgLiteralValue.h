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

#include "FDO\Expression\mgValueExpression.h"
#include "FDO\Expression\mgLiteralValueType.h"

class FdoLiteralValue;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// LiteralValue is an abstract base class that derives from ValueExpression to
/// form the root for all literal value types.
public __gc class LiteralValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression
{
public:
    /// \brief
    /// Constructs a LiteralValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	LiteralValue(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    /// Gets the literal value type of the LiteralValue.
    /// 
    /// \return
    /// Returns an LiteralValueType
    /// 
    __property NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType get_LiteralValueType ();

public private:
	inline FdoLiteralValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


