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

#include "FDO\Commands\LongTransaction\mgLongTransactionConflictResolution.h"

class PropertyValueCollection;
class ILongTransactionConflictDirectiveEnumerator;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
public __gc class PropertyValueCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \ingroup (interfaces)
/// \interface ILongTransactionConflictDirectiveEnumerator
/// \brief
/// The ILongTransactionConflictDirective interface is used to specify how
/// conflict should be resolved for a particular object.
public __gc __interface ILongTransactionConflictDirectiveEnumerator : public System::IDisposable
{
public:
    /// \brief
    /// Gets the class name of the current feature in conflict.
    /// 
    /// \return
    /// Returns the class name
    /// 
	__property System::String* get_FeatureClassName();

    /// \brief
    /// Returns PropertyValueCollection containing the property values that
    /// uniquely identify the current feature in conflict.
    /// 
    /// \return
    /// Returns PropertyValueCollection
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection* get_Identity();

    /// \brief
    /// Gets the LongTransactionConflictResolution value to use for the current
    /// feature. The default value is LongTransactionConflictResolution_Child.
    /// 
    /// \return
    /// Returns LongTransactionConflictResolution value
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution get_Resolution();

    /// \brief
    /// Sets the LongTransactionConflictResolution value to use for the current
    /// feature. The default value is LongTransactionConflictResolution_Child.
    /// 
    /// \param value 
    /// Input the LongTransactionConflictResolution value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Resolution(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionConflictResolution value);

    /// \brief
    /// Returns the number of conflicts reported by this enumerator.
    /// 
    /// \return
    /// Returns the number of conflicts
    /// 
	__property System::Int32 get_Count();

    /// \brief
    /// Advances the enumerator to the next conflict. The default position of the
    /// enumerator is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data. ReadNext returns true until the end of the
    /// directives is reached. After the end of the directives is passed,
    /// subsequent calls to ReadNext return false until Reset is called.
    /// 
    /// \return
    /// Returns True until the end of the
    /// directives is reached
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Sets the enumerator to its initial position, which is before the first
    /// conflict.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Reset();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


