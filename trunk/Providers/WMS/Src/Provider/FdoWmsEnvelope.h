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
#ifndef FDOWMSENVELOPE_H
#define FDOWMSENVELOPE_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "FdoWmsRect.h"

class FdoWmsEnvelope : public FdoIEnvelope
{
//
// Data members
//
private:
	FdoWmsRect		m_rect;

//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoWmsEnvelope(double minX, double minY, double maxX, double maxY) : m_rect(minX, minY, maxX, maxY) {}
	virtual ~FdoWmsEnvelope(){}
	void Dispose() { delete this;}
public:
	static FdoWmsEnvelope* Create(double minX, double minY, double maxX, double maxY)
	{
		return new FdoWmsEnvelope(minX, minY, maxX, maxY);
	}
//
// Exposed functions
//
public:

    /// <summary>
    /// Gets the minimum X coordinate value.
    /// </summary>
    /// <remarks>
    /// This is the smallest X ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the X coordinate for the point in the rear, lower-left
    /// corner of the envelope.
    /// </remarks>
    /// <returns>Returns the miniumum X ordinate; numeric_limits<double>::quiet_NaN() by default</returns>
	virtual double GetMinX() const { return m_rect.m_minX; }

    /// <summary>
    /// Gets the minimum Y coordinate value.
    /// </summary>
    /// <remarks>
    /// This is the smallest Y ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Y coordinate for the point in the rear, lower-left
    /// corner of the envelope.
    /// </remarks>
    /// <returns>Returns the miniumum Y ordinate; numeric_limits<double>::quiet_NaN() by default</returns>
	virtual double GetMinY() const { return m_rect.m_minY; }

    /// <summary>
    /// Gets the minimum Z coordinate value.
    /// </summary>
    /// <remarks>
    /// This is the smallest Z ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Z coordinate for the point in the rear, lower-left
    /// corner of the envelope.
    /// </remarks>
    /// <returns>Returns the miniumum Z ordinate; numeric_limits<double>::quiet_NaN() by default</returns>
	virtual double GetMinZ() const { return 0; }

    /// <summary>
    /// Gets the maximum X coordinate value.
    /// </summary>
    /// <remarks>
    /// This is the largest X ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the X coordinate for the point in the front, upper-right
    /// corner of the envelope.
    /// </remarks>
    /// <returns>Returns the maxiumum X ordinate; numeric_limits<double>::quiet_NaN() by default</returns>
	virtual double GetMaxX() const { return m_rect.m_maxX; }

    /// <summary>
    /// Gets the maximum Y coordinate value.
    /// </summary>
    /// <remarks>
    /// This is the largest Y ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Y coordinate for the point in the front, upper-right
    /// corner of the envelope.
    /// </remarks>
    /// <returns>Returns the maxiumum Y ordinate; numeric_limits<double>::quiet_NaN() by default</returns>
	virtual double GetMaxY() const { return m_rect.m_maxY; }

    /// <summary>
    /// Gets the maximum Z coordinate value.
    /// </summary>
    /// <remarks>
    /// This is the largest Z ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Z coordinate for the point in the front, upper-right
    /// corner of the envelope.
    /// </remarks>
    /// <returns>Returns the maxiumum Z ordinate; numeric_limits<double>::quiet_NaN() by default</returns>
	virtual double GetMaxZ() const { return 0; }

    /// <summary>
    /// Indicates whether the envelope's extents are set.
    /// </summary>
    /// <returns>Returns 'true' if none of the X, Y and Z extents have been set; 'false' otherwise</returns>
	virtual bool GetIsEmpty() const { return false; }
   
    ///<summary>Gets the ordinates as an array.</summary>
    /// <remarks>The caller must not free the returned array.
    /// The ordinates are in the order XYZXYZ, with only those present 
    /// according to the dimensionality.
    /// </remarks>
    /// <returns>Returns the ordinates</returns> 
    virtual const double * GetOrdinates() { throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED))); };
};

typedef FdoPtr<FdoWmsEnvelope> FdoWmsEnvelopeP;

#endif
