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

#include "FDO\Filter\mgIFilterProcessor.h"

class FdoIFilterProcessor;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

/// \brief
/// The IFilterProcessor interface can be used to process the nodes in a filter
/// tree. It declares a process operation for each concrete class in the filter
/// hierarchy. Providers or client applications can create classes that realize
/// this interface to do something meaningful with a filter hierarchy. For
/// example, a RDBMS feature provider can implement a processor class to convert a
/// filter hierarchy to the SQL equivalent syntax.
private __gc class IFilterProcessorImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor
{
public:
    /// \brief
    /// Constructs a IFilterProcessorImp object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	IFilterProcessorImp(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoIFilterProcessor* GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};
END_NAMESPACE_OSGEO_FDO_FILTER


