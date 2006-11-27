/*
* 
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

#include <limits>       // For quiet_NaN()
#include <float.h>      // For _isnan()
#include <math.h>

#include <Geometry/GeometryStd.h>
#include <Geometry/DirectPositionImpl.h>

using namespace std;


FdoDirectPositionImpl* FdoDirectPositionImpl::Create()
{
    FdoPtr<FdoDirectPositionImpl> newPosition = new FdoDirectPositionImpl();

    if (newPosition == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(newPosition.p);
}

FdoDirectPositionImpl* FdoDirectPositionImpl::Create(double x, double y)
{
    FdoPtr<FdoDirectPositionImpl> newPosition = new FdoDirectPositionImpl(x, y);

    if (newPosition == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(newPosition.p);
}

FdoDirectPositionImpl* FdoDirectPositionImpl::Create(double x, double y, double z)
{
    FdoPtr<FdoDirectPositionImpl> newPosition = new FdoDirectPositionImpl(x, y, z);

    if (newPosition == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(newPosition.p);
}

FdoDirectPositionImpl* FdoDirectPositionImpl::Create(double x, double y, double z, double m)
{
    FdoPtr<FdoDirectPositionImpl> newPosition = new FdoDirectPositionImpl(x, y, z, m);

    if (newPosition == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(newPosition.p);
}

FdoDirectPositionImpl* FdoDirectPositionImpl::Create(FdoInt32 dimensionality, const double * ordinates)
{
    FdoPtr<FdoDirectPositionImpl> newPosition = new FdoDirectPositionImpl(dimensionality, ordinates);

    if (newPosition == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(newPosition.p);
}

FdoDirectPositionImpl* FdoDirectPositionImpl::Create(const FdoDirectPositionImpl& position)
{
    FdoPtr<FdoDirectPositionImpl> newPosition = new FdoDirectPositionImpl(position);

    if (newPosition == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(newPosition.p);
}

FdoDirectPositionImpl* FdoDirectPositionImpl::Create(const FdoIDirectPosition* pos)
{
    FdoPtr<FdoDirectPositionImpl> newPosition = new FdoDirectPositionImpl(pos);

    if (newPosition == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(newPosition.p);
}

FdoDirectPositionImpl::FdoDirectPositionImpl()
:   m_ordinates(NULL)
{
	m_x = numeric_limits<double>::quiet_NaN();
	m_y = numeric_limits<double>::quiet_NaN();
	m_z = numeric_limits<double>::quiet_NaN();
	m_m = numeric_limits<double>::quiet_NaN();
	m_dimensionality = FdoDimensionality_XY;
}


FdoDirectPositionImpl::FdoDirectPositionImpl(const FdoDirectPositionImpl& directPosition) 
:   m_ordinates(NULL)
{
	m_x = directPosition.GetX();
	m_y = directPosition.GetY();
	m_z = directPosition.GetZ();
	m_m = directPosition.GetM();
	m_dimensionality = directPosition.GetDimensionality();
}

FdoDirectPositionImpl::FdoDirectPositionImpl(double x, double y)
:   m_ordinates(NULL)
{
	m_x = x;
	m_y = y;
	m_z = numeric_limits<double>::quiet_NaN();
	m_m = numeric_limits<double>::quiet_NaN();
	m_dimensionality = FdoDimensionality_XY;
}


FdoDirectPositionImpl::FdoDirectPositionImpl(double x, double y, double z)
:   m_ordinates(NULL)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_m = numeric_limits<double>::quiet_NaN();
	m_dimensionality = FdoDimensionality_XY|FdoDimensionality_Z;
}

FdoDirectPositionImpl::FdoDirectPositionImpl(double x, double y, double z, double m) 
:   m_ordinates(NULL)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_m = m;
	m_dimensionality = (FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M);
}

FdoDirectPositionImpl::FdoDirectPositionImpl(FdoInt32 dimensionality, const double* ordinates) 
:   m_ordinates(NULL)
{
    bool hasZ = (dimensionality&FdoDimensionality_Z) != 0;
    bool hasM = (dimensionality&FdoDimensionality_M) != 0;
	m_x = ordinates[0];
	m_y = ordinates[1];
    m_z = hasZ ? ordinates[2] : numeric_limits<double>::quiet_NaN();
    m_m = ( hasZ && hasM ) ? ordinates[3] : ( hasM ? ordinates[2] : numeric_limits<double>::quiet_NaN() );
	m_dimensionality = dimensionality;
}

FdoDirectPositionImpl& FdoDirectPositionImpl::operator=(const FdoDirectPositionImpl& position)
{
	this->m_x = position.m_x;
	this->m_y = position.m_y;
	this->m_z = position.m_z;
	this->m_m = position.m_m;
	this->m_dimensionality = position.m_dimensionality;

    if (NULL != this->m_ordinates)
    {
        delete [] m_ordinates;
        m_ordinates = NULL;
    }

	return *this;   // SSP: Addref?
}

FdoDirectPositionImpl& FdoDirectPositionImpl::operator=(const FdoIDirectPosition& position)
{
	this->m_x = position.GetX();
	this->m_y = position.GetY();
	this->m_z = position.GetZ();
	this->m_m = position.GetM();
	this->m_dimensionality = position.GetDimensionality();

    if (NULL != this->m_ordinates)
    {
        delete [] m_ordinates;
        m_ordinates = NULL;
    }

	return *this;   // SSP: Addref?
}


bool FdoDirectPositionImpl::operator==(const FdoDirectPositionImpl& rhs) const
{
#ifdef _WIN32
	if ((this->GetX() == rhs.GetX() || (_isnan(this->GetX()) && _isnan(rhs.GetX()))) &&
		(this->GetY() == rhs.GetY() || (_isnan(this->GetY()) && _isnan(rhs.GetY()))) &&
		(this->GetZ() == rhs.GetZ() || (_isnan(this->GetZ()) && _isnan(rhs.GetZ()))) &&
		(this->GetM() == rhs.GetM() || (_isnan(this->GetM()) && _isnan(rhs.GetM()))) &&
#else
	if ((this->GetX() == rhs.GetX() || (isnan(this->GetX()) && isnan(rhs.GetX()))) &&
		(this->GetY() == rhs.GetY() || (isnan(this->GetY()) && isnan(rhs.GetY()))) &&
		(this->GetZ() == rhs.GetZ() || (isnan(this->GetZ()) && isnan(rhs.GetZ()))) &&
		(this->GetM() == rhs.GetM() || (isnan(this->GetM()) && isnan(rhs.GetM()))) &&
#endif
		this->GetDimensionality() == rhs.GetDimensionality())

		return true;
	else
		return false;
}


FdoDirectPositionImpl::FdoDirectPositionImpl(const FdoIDirectPosition* pos)
:   m_ordinates(NULL)
{
	if ( NULL == pos )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoDirectPositionImpl",
                                                               L"pos"));

	this->m_x = pos->GetX();
	this->m_y = pos->GetY();
	this->m_z = pos->GetZ();
	this->m_m = pos->GetM();
	this->m_dimensionality = pos->GetDimensionality();
}

FdoDirectPositionImpl::~FdoDirectPositionImpl()
{
    if (NULL != m_ordinates)
    {
        delete [] m_ordinates;
        m_ordinates = NULL;
    }
}


double FdoDirectPositionImpl::GetX() const
{
	return m_x;
}



double FdoDirectPositionImpl::GetY() const
{
	return m_y;
}


double FdoDirectPositionImpl::GetZ() const
{
	return m_z;
}


double FdoDirectPositionImpl::GetM() const
{
	return m_m;
}

FdoInt32 FdoDirectPositionImpl::GetDimensionality() const
{
	return m_dimensionality;
}

const double * FdoDirectPositionImpl::GetOrdinates()
{
    if (NULL == m_ordinates)
    {
        m_ordinates = new double[4];
        if (NULL == m_ordinates)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }
    m_ordinates[0] = m_x;
    m_ordinates[1] = m_y;
    FdoInt32 index = 2;
    bool hasZ = (m_dimensionality&FdoDimensionality_Z) != 0;
    bool hasM = (m_dimensionality&FdoDimensionality_M) != 0;
    if (hasZ)
        m_ordinates[index++] = m_z;
    if (hasM)
        m_ordinates[index] = m_m;

    return m_ordinates;
}

void FdoDirectPositionImpl::SetX(double x)
{
	m_x = x;
}

void FdoDirectPositionImpl::SetY(double y)
{
	m_y = y;
}

void FdoDirectPositionImpl::SetZ(double z)
{
	m_z = z;
}

void FdoDirectPositionImpl::SetM(double m)
{
	m_m = m;
}

void FdoDirectPositionImpl::SetDimensionality(FdoInt32 dimensionality)
{
	m_dimensionality = dimensionality;
}

void FdoDirectPositionImpl::Dispose()
{
	delete this;
}

