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

#include "FDO\Commands\Feature\mgIFeatureReaderImp.h"
#include "FDO\Commands\Feature\mgIScrollableFeatureReader.h"

class FdoIFeatureReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
ref class PropertyValueCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \brief
/// The IScrollableFeatureReaderImp class is a concrete implementation class for 
/// interface IScrollableFeatureReader.
private ref class IScrollableFeatureReaderImp : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp, 
                                                 public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReader
{
public:
	/// \brief
	/// Returns the number of records of the query result.
	///
    /// \return
	/// Returns number of records.
	virtual System::Int32 Count();
	
	/// \brief
	/// Positions the reader at the first record of the query result.
	///
    /// \return
	/// Returns true if a record is found or false if the result is empty.
	virtual System::Boolean ReadFirst();

	/// \brief
	/// Position the reader at the last record of the query result.
	///
    /// \return
	/// Returns true if a record is found or false if the result is empty.
	virtual System::Boolean ReadLast();

	/// \brief
	/// Advances the reader to the previous item.
	///
	/// \remarks
	/// The default position of the reader is prior to the first item.
	/// Therefore you must call ReadFirst or ReadLast to
	/// begin accessing any data.
	///
	/// \return
	/// Returns true if a record is found or false if reading is complete.
    virtual System::Boolean ReadPrevious();

	/// \brief
	/// Provides the ad-hoc access to the query result.
	///
	/// \remarks
	/// It positions the position of the reader at a given record 
	/// defined by the key. If the record is not found,
	/// then the reader position is unset and false value is returned. 
	/// Once the reader’s position becames unset,
	/// the caller needs to call ReadFirst, ReadLast or ReadAt to re-position 
	/// the reader at a valid location.
	///
	/// \param key
	/// The key that identifies a record.
	///
	/// \return
	/// Returns true if a record is found or false otherwise.
	virtual System::Boolean ReadAt(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ key);

	/// \brief
	/// Provides an ad-hoc access to the query result.
	/// 
	/// \remarks
	/// The recordindex is the one-based nth item in the query result. 
	/// If successful, this method will position the reader at the feature identified by the recordindex.
	///
	/// \param recordindex
	/// The index of the row.
	///
	/// \return
	/// Returns true if a record is found or false otherwise.
    virtual System::Boolean  ReadAtIndex( System::UInt32 recordindex );

	/// \brief
	/// Given a key of a feature, IndexOf will return the one based index or 
    /// the record number of the feature within the returned query result.
	/// \remarks
	/// If the record is not found, then zero is retuned.
	/// This is the mirror function of the ReadAtIndex function.
	/// This method does not affect the reader position. 
	/// ReadAtIndex needs to be called to move the reader to the returned index.
	///
	/// \param key
	/// The key that identifies a record.
	///
	/// \return
	/// Returns true if a record is found or false otherwise.
    virtual System::UInt32 IndexOf(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ key);

internal:
	IScrollableFeatureReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp(unmanaged, autoDelete)
	{

	}

	inline FdoIScrollableFeatureReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


