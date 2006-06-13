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
#ifndef FDORFPRECT_H
#define FDORFPRECT_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpRect
{
public:
	double m_minX, m_minY, m_maxX, m_maxY;
public:
	FdoRfpRect(double minX, double minY, double maxX, double maxY) : m_minX(minX), 
		m_minY(minY), m_maxX(maxX), m_maxY(maxY) {}
	FdoRfpRect() : m_minX(0),m_minY(0), m_maxX(0), m_maxY(0) {}
	~FdoRfpRect(void) {}

	inline double GetWidth() const
	{
		return m_maxX - m_minX;
	}

	inline double GetHeight() const
	{
		return m_maxY - m_minY;
	}

	inline FdoRfpRect Intersect(const FdoRfpRect& rhs) const
	{
		return FdoRfpRect(m_minX > rhs.m_minX ? m_minX : rhs.m_minX,
			m_minY > rhs.m_minY ? m_minY : rhs.m_minY,
			m_maxX < rhs.m_maxX ? m_maxX : rhs.m_maxX,
			m_maxY < rhs.m_maxY ? m_maxY : rhs.m_maxY);
	}

	inline FdoRfpRect Union(const FdoRfpRect& rhs) const
	{
		return FdoRfpRect(m_minX < rhs.m_minX ? m_minX : rhs.m_minX,
			m_minY < rhs.m_minY ? m_minY : rhs.m_minY,
			m_maxX > rhs.m_maxX ? m_maxX : rhs.m_maxX,
			m_maxY > rhs.m_maxY ? m_maxY : rhs.m_maxY);
	}

	inline bool IsValid() const
	{
		return m_minX < m_maxX && m_minY < m_maxY; 
	}

	inline bool operator==(const FdoRfpRect& rhs) const
	{
		return m_minX == rhs.m_minX &&
				m_minY == rhs.m_minY &&
				m_maxX == rhs.m_maxX &&
				m_maxY == rhs.m_maxY;
	}
	inline bool IsWithin(const FdoRfpRect& rhs) const
	{
		return Intersect(rhs) == *this;
	}
	inline bool IsInside(const FdoRfpRect& rhs) const
	{ 	
		return IsWithin(rhs) &&
				m_minX != rhs.m_minX &&
				m_minY != rhs.m_minY &&
				m_maxX != rhs.m_maxX &&
				m_maxY != rhs.m_maxY;
	}

	inline bool IsIntersecting(const FdoRfpRect& rhs) const
	{	
		return Intersect(rhs).IsValid(); 
	}

};


#endif
