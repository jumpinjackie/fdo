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

#include "FDO\Expression\mgIExpressionProcessor.h"

class FdoIExpressionProcessor;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

/// \brief
/// The FdoIExpressionProcessor interface can be used to process the nodes in an
/// Expression tree. It declares an process operation for each concrete class in
/// the FdoExpression hierarchy. Providers or client applications can create
/// classes that realize this interface to do something meaningful with an
/// FdoExpression hierarchy. For example, an RDBMS feature provider implements a
/// processor class to convert an FdoExpression hierarchy to the SQL equivalent
/// syntax.
private __gc class IExpressionProcessorImp  : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor
{
public:
	IExpressionProcessorImp(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
	System::Void ReleaseUnmanagedObject();

public private:
	inline FdoIExpressionProcessor* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


