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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpKeyColor.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPKEYCOLOR_H
#define FDORFPKEYCOLOR_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpKeyColor : public FdoIDisposable
{
public:
	enum KeyColorType
	{
		/// <summary>
        /// This entry indicates that the key color is only one bit.
        /// </summary>
        ///
		kData1,

		/// <summary>
        /// This entry indicates that the key color is an eight value.
        /// </summary>
        ///
		kData8,

		/// <summary>
        /// This entry indicates that the key color is a sixteen bit value.
        /// </summary>
        ///
		kData16,

		/// <summary>
        /// This entry indicates that the key color is a thirty two bit value.
        /// </summary>
        ///
		kData32,

		/// <summary>
        /// This entry indicates that the key color is a sixty four bit value.
        /// </summary>
        ///
		kData64
	};

public:
	FdoRfpKeyColor(void);

	/// <summary>
    /// A union of all key color bit sizes and the data model types.
    /// </summary>
    union {
		FdoInt8  data1;
        FdoInt8  data8;
        FdoInt16 data16;
        FdoInt32 data32;
        FdoInt64 data64;
    } value;
	
    KeyColorType type;

protected:
	virtual ~FdoRfpKeyColor(void);
	void Dispose() { delete this; }
};

typedef FdoPtr<FdoRfpKeyColor> FdoRfpKeyColorP;

class FdoRfpKeyColorCollection : public FdoCollection<FdoRfpKeyColor, FdoException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpKeyColorCollection() {}
	virtual ~FdoRfpKeyColorCollection() {}
	void Dispose() { delete this; }
public:
	bool Contains(FdoRfpKeyColorP& val);
	FdoInt32  Add(FdoRfpKeyColorP& val);
	void Add(FdoPtr<FdoRfpKeyColorCollection>& coll);

	static FdoRfpKeyColorCollection* Create();
};

typedef FdoPtr<FdoRfpKeyColorCollection> FdoRfpKeyColorsP;

#endif
