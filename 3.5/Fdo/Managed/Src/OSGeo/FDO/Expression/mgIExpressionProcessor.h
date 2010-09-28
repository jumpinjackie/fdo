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

#include "FDO\Expression\mgExpression.h"

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

/// \ingroup (OSGeoFDOExpression)
/// \interface OSGeo::FDO::Expression::IExpressionProcessor
/// \brief
/// The IExpressionProcessor interface can be used to process the nodes in an
/// Expression tree. It declares an process operation for each concrete class in
/// the Expression hierarchy. Providers or client applications can create
/// classes that realize this interface to do something meaningful with an
/// Expression hierarchy. For example, an RDBMS feature provider implements a
/// processor class to convert an Expression hierarchy to the SQL equivalent
/// syntax.
public interface class IExpressionProcessor  : public System::IDisposable
{
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


