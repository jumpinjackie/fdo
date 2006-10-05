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
#include <Geometry/EnvelopeImpl.h>
#include <Geometry/DirectPositionImpl.h>

using namespace std;




FdoEnvelopeImpl* FdoEnvelopeImpl::Create()
{
    FdoPtr<FdoEnvelopeImpl> envelope = new FdoEnvelopeImpl();

    if (envelope == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(envelope.p);
}

FdoEnvelopeImpl* FdoEnvelopeImpl::Create(double minX, double minY,
                 double maxX, double maxY)
{
    FdoPtr<FdoEnvelopeImpl> envelope = new FdoEnvelopeImpl(minX, minY, maxX, maxY);

    if (envelope == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(envelope.p);
}

FdoEnvelopeImpl* FdoEnvelopeImpl::Create(double minX, double minY, double minZ,
                 double maxX, double maxY, double maxZ)
{
    FdoPtr<FdoEnvelopeImpl> envelope = new FdoEnvelopeImpl(minX, minY, minZ, maxX, maxY, maxZ);

    if (envelope == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(envelope.p);
}

FdoEnvelopeImpl* FdoEnvelopeImpl::Create(FdoEnvelopeImpl& envelopeImpl)
{
    FdoPtr<FdoEnvelopeImpl> envelope = new FdoEnvelopeImpl(envelopeImpl);

    if (envelope == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(envelope.p);
}

FdoEnvelopeImpl* FdoEnvelopeImpl::Create(FdoIDirectPosition* lowerLeft, FdoIDirectPosition * upperRight)
{
    FdoPtr<FdoEnvelopeImpl> envelope = new FdoEnvelopeImpl(lowerLeft, upperRight);

    if (envelope == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(envelope.p);
}

FdoEnvelopeImpl* FdoEnvelopeImpl::Create(FdoInt32 dimensionType, double * ordinates)
{
    FdoPtr<FdoEnvelopeImpl> envelope = new FdoEnvelopeImpl(dimensionType, ordinates);

    if (envelope == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return FDO_SAFE_ADDREF(envelope.p);
}

/************************************************************************/
/* Default constructor                                                                     */
/************************************************************************/
FdoEnvelopeImpl::FdoEnvelopeImpl()
:   m_ordinates(NULL)
{
#ifdef _WIN32
	m_minX = numeric_limits<double>::quiet_NaN();
	m_minY = numeric_limits<double>::quiet_NaN();
	m_minZ = numeric_limits<double>::quiet_NaN();
	m_maxX = numeric_limits<double>::quiet_NaN();
	m_maxY = numeric_limits<double>::quiet_NaN();
	m_maxZ = numeric_limits<double>::quiet_NaN();
#else
	m_minX = NAN;
	m_minY = NAN;
	m_minZ = NAN; 
	m_maxX = NAN;
	m_maxY = NAN;
	m_maxZ = NAN;
#endif
	m_isEmpty = true;
}

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoEnvelopeImpl::FdoEnvelopeImpl(double minX, double minY,
				           double maxX, double maxY)
:   m_ordinates(NULL)
{
	m_minX = minX;
	m_minY = minY;
#ifdef _WIN32
	m_minZ = numeric_limits<double>::quiet_NaN();
#else
	m_minZ = NAN;
#endif
	m_maxX = maxX;
	m_maxY = maxY;
#ifdef _WIN32
	m_maxZ = numeric_limits<double>::quiet_NaN();
#else
	m_maxZ = NAN;
#endif
	m_isEmpty = false;
}

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoEnvelopeImpl::FdoEnvelopeImpl(double minX, double minY, double minZ,
						   double maxX, double maxY, double maxZ)
:   m_ordinates(NULL)
{
	m_minX = minX;
	m_minY = minY;
	m_minZ = minZ;
	m_maxX = maxX;
	m_maxY = maxY;
	m_maxZ = maxZ;
	m_isEmpty = false;
}

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoEnvelopeImpl::FdoEnvelopeImpl(FdoIDirectPosition * lowerLeft, FdoIDirectPosition * upperRight)
:   m_ordinates(NULL)
{
	if ( (NULL == lowerLeft) ||
		 (NULL == upperRight) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoEnvelopeImpl",
                                                               L"lowerLeft/upperRight"));

	m_minX = lowerLeft->GetX();
	m_minY = lowerLeft->GetY();
	m_maxX = upperRight->GetX();
	m_maxY = upperRight->GetY();

	//
	// We can just straight copy the Z ordinate as Envelopes are
	// not sensitive to dimensionality (i.e. they don't have
	// FdoDimensionality attribute
	//
	m_minZ = lowerLeft->GetZ();
	m_maxZ = upperRight->GetZ();

	m_isEmpty = false;
}

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoEnvelopeImpl::FdoEnvelopeImpl(FdoInt32 dimensionType, double * ordinates)
:   m_ordinates(NULL)
{
	if (NULL == ordinates)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoEnvelopeImpl",
                                                               L"ordinates"));

	if (dimensionType == FdoDimensionality_XY)
	{
		m_minX = ordinates[0];
		m_minY = ordinates[1];
#ifdef _WIN32
		m_minZ = numeric_limits<double>::quiet_NaN();
#else
		m_minZ = NAN;
#endif
		m_maxX = ordinates[2];
		m_maxY = ordinates[3];
#ifdef _WIN32
		m_maxZ = numeric_limits<double>::quiet_NaN();
#else
		m_maxZ = NAN;
#endif
		
	}
	else if (dimensionType == (FdoDimensionality_XY | FdoDimensionality_Z))
	{
		m_minX = ordinates[0];
		m_minY = ordinates[1];
		m_minZ = ordinates[2];
		m_maxX = ordinates[3];
		m_maxY = ordinates[4];
		m_maxZ = ordinates[5];
	}
	else
	{
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoEnvelopeImpl",
                                                               L"dimensionType"));
	}

	m_isEmpty = false;
}

/************************************************************************/
/* Copy constructor                                                                     */
/************************************************************************/
FdoEnvelopeImpl::FdoEnvelopeImpl(FdoEnvelopeImpl& envelopeImpl)
:   m_ordinates(NULL)
{
	m_minX = envelopeImpl.m_minX;
	m_minY = envelopeImpl.m_minY;
	m_minZ = envelopeImpl.m_minZ;
	m_maxX = envelopeImpl.m_maxX;
	m_maxY = envelopeImpl.m_maxY;
	m_maxZ = envelopeImpl.m_maxZ;
	m_isEmpty = envelopeImpl.m_isEmpty;
}

/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoEnvelopeImpl::~FdoEnvelopeImpl()
{
    if (NULL != this->m_ordinates)
    {
        delete [] m_ordinates;
        m_ordinates = NULL;
    }
}

/************************************************************************/
/* Get Max X                                                                     */
/************************************************************************/
double FdoEnvelopeImpl::GetMaxX() const
{
	return m_maxX;
}

/************************************************************************/
/* Get Max Y                                                                     */
/************************************************************************/
double FdoEnvelopeImpl::GetMaxY() const
{
	return m_maxY;
}

/************************************************************************/
/* Get Max Z                                                                     */
/************************************************************************/
double FdoEnvelopeImpl::GetMaxZ() const
{
	return m_maxZ;
}

/************************************************************************/
/* Get Min X                                                                     */
/************************************************************************/
double FdoEnvelopeImpl::GetMinX() const
{
	return m_minX;
}

/************************************************************************/
/* Get Min Y                                                                     */
/************************************************************************/
double FdoEnvelopeImpl::GetMinY() const
{
	return m_minY;
}

/************************************************************************/
/* Get Min Z                                                                     */
/************************************************************************/
double FdoEnvelopeImpl::GetMinZ() const
{
	return m_minZ;
}

const double * FdoEnvelopeImpl::GetOrdinates()
{
    if (NULL == m_ordinates)
    {
        m_ordinates = new double[6];
        if (NULL == m_ordinates)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }
    m_ordinates[0] = m_minX;
    m_ordinates[1] = m_minY;
    FdoInt32 index = 2;
    bool hasZ =
#ifdef _WIN32
        ( ! _isnan(m_minZ) );
#else
        ( ! isnan(m_minZ) );
#endif
    if (hasZ)
        m_ordinates[index++] = m_minZ;

    m_ordinates[index++] = m_maxX;
    m_ordinates[index++] = m_maxY;

    if (hasZ)
        m_ordinates[index++] = m_maxZ;

    return m_ordinates;
}

/************************************************************************/
/* Get is empty                                                                     */
/************************************************************************/
bool FdoEnvelopeImpl::GetIsEmpty() const
{
	return m_isEmpty;
}

/************************************************************************/
/* Set Min X                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::SetMinX(double minX)
{
	m_minX = minX;
}

/************************************************************************/
/* Set Min Y                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::SetMinY(double minY)
{
	m_minY = minY;
}

/************************************************************************/
/* Set Min Z                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::SetMinZ(double minZ)
{
	m_minZ = minZ;
}

/************************************************************************/
/* Set Max X                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::SetMaxX(double maxX)
{
	m_maxX = maxX;
}

/************************************************************************/
/* Set Max Y                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::SetMaxY(double maxY)
{
	m_maxY = maxY;
}

/************************************************************************/
/* Set Max Z                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::SetMaxZ(double maxZ)
{
	m_maxZ = maxZ;
}

/************************************************************************/
/* Equality operator                                                                     */
/************************************************************************/
bool FdoEnvelopeImpl::operator==(FdoEnvelopeImpl& rhs) const
{
	bool isEqual = false;

	if (this->m_isEmpty && rhs.GetIsEmpty())
	{
		isEqual = true;
	}
	else if (!this->m_isEmpty && !rhs.GetIsEmpty() &&
#ifdef _WIN32
	         (this->GetMaxX() == rhs.GetMaxX() || (_isnan(this->GetMaxX()) && _isnan(rhs.GetMaxX()))) &&
		     (this->GetMaxY() == rhs.GetMaxY() || (_isnan(this->GetMaxY()) && _isnan(rhs.GetMaxY()))) &&
		     (this->GetMaxZ() == rhs.GetMaxZ() || (_isnan(this->GetMaxZ()) && _isnan(rhs.GetMaxZ()))) &&
		     (this->GetMinX() == rhs.GetMinX() || (_isnan(this->GetMinX()) && _isnan(rhs.GetMinX()))) &&
		     (this->GetMinY() == rhs.GetMinY() || (_isnan(this->GetMinY()) && _isnan(rhs.GetMinY()))) &&
		     (this->GetMinZ() == rhs.GetMinZ() || (_isnan(this->GetMinZ()) && _isnan(rhs.GetMinZ()))))
#else
	         (this->GetMaxX() == rhs.GetMaxX() || (isnan(this->GetMaxX()) && isnan(rhs.GetMaxX()))) &&
		     (this->GetMaxY() == rhs.GetMaxY() || (isnan(this->GetMaxY()) && isnan(rhs.GetMaxY()))) &&
		     (this->GetMaxZ() == rhs.GetMaxZ() || (isnan(this->GetMaxZ()) && isnan(rhs.GetMaxZ()))) &&
		     (this->GetMinX() == rhs.GetMinX() || (isnan(this->GetMinX()) && isnan(rhs.GetMinX()))) &&
		     (this->GetMinY() == rhs.GetMinY() || (isnan(this->GetMinY()) && isnan(rhs.GetMinY()))) &&
		     (this->GetMinZ() == rhs.GetMinZ() || (isnan(this->GetMinZ()) && isnan(rhs.GetMinZ()))))
#endif
	{		
		isEqual = true;
	}

	return isEqual;
}

/************************************************************************/
/* Expand this Envelope                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::Expand(FdoIDirectPosition* point)
{
	double x = point->GetX();
	double y = point->GetY();
	double z = point->GetZ();

	if (m_isEmpty)
	{
		m_isEmpty = false;
		m_minX = x;
		m_minY = y;
		m_minZ = z;
		m_maxX = x;
		m_maxY = y;
		m_maxZ = z;
	}
	else
	{
#ifdef _WIN32
        double llx = (_isnan(m_minX) || m_minX > x) ? x : m_minX;
        double lly = (_isnan(m_minY) || m_minY > y) ? y : m_minY;
        double llz = (_isnan(m_minZ) || m_minZ > z) ? z: m_minZ;
        double urx = (_isnan(m_maxX) || m_maxX < x) ? x: m_maxX;
        double ury = (_isnan(m_maxY) || m_maxY < y) ? y: m_maxY;
        double urz = (_isnan(m_maxZ) || m_maxZ < z) ? z: m_maxZ;
#else
        double llx = (isnan(m_minX) || m_minX > x) ? x : m_minX;
        double lly = (isnan(m_minY) || m_minY > y) ? y : m_minY;
        double llz = (isnan(m_minZ) || m_minZ > z) ? z: m_minZ;
        double urx = (isnan(m_maxX) || m_maxX < x) ? x: m_maxX;
        double ury = (isnan(m_maxY) || m_maxY < y) ? y: m_maxY;
        double urz = (isnan(m_maxZ) || m_maxZ < z) ? z: m_maxZ;

#endif

		m_minX = llx;
		m_minY = lly;
		m_minZ = llz;
		m_maxX = urx;
		m_maxY = ury;
		m_maxZ = urz;
	}
}


/************************************************************************/
/* Expand envelope                                                                     */
/************************************************************************/
void FdoEnvelopeImpl::Expand(FdoIEnvelope* envl)
{
	if (NULL == envl)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoEnvelopeImpl::Expand",
                                                               L"envl"));

    FdoPtr<FdoDirectPositionImpl> pos1 = FdoDirectPositionImpl::Create(envl->GetMaxX(), envl->GetMaxY(), envl->GetMaxZ());
	this->Expand(pos1);

	FdoPtr<FdoDirectPositionImpl> pos2 = FdoDirectPositionImpl::Create(envl->GetMinX(), envl->GetMinY(), envl->GetMinZ());
	this->Expand(pos2);
}


void FdoEnvelopeImpl::Dispose()
{
	delete this;
}

