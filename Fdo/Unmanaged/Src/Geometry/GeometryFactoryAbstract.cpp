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

#include <Geometry/GeometryFactoryAbstract.h>
#include <Geometry/EnvelopeImpl.h>

/*
 * Implement some of the methods for FdoGeometryFactoryAbstract.
 * Only some helper types are supported.
 */

FdoIDirectPosition* FdoGeometryFactoryAbstract::CreatePositionXY(double x, double y)
{
	return FdoDirectPositionImpl::Create(x, y);
}

FdoIDirectPosition* FdoGeometryFactoryAbstract::CreatePositionXYM(double x, double y, double m)
{
    FdoPtr<FdoDirectPositionImpl> pos = FdoDirectPositionImpl::Create(x, y);

	pos->SetM(m);
	pos->SetDimensionality((FdoDimensionality_XY|FdoDimensionality_M));

	return FDO_SAFE_ADDREF(pos.p);
}

FdoIDirectPosition* FdoGeometryFactoryAbstract::CreatePositionXYZ(double x, double y, double z)
{
	return FdoDirectPositionImpl::Create(x, y, z);
}

FdoIDirectPosition* FdoGeometryFactoryAbstract::CreatePositionXYZM(double x, double y, double z, double m)
{
	return FdoDirectPositionImpl::Create(x, y, z, m);
}

FdoIDirectPosition* FdoGeometryFactoryAbstract::CreatePosition(
    FdoInt32 dimensionality,
    const double * ordinates)
{
    FdoPtr<FdoDirectPositionImpl> posImpl =
        FdoDirectPositionImpl::Create(dimensionality, ordinates);

	return FDO_SAFE_ADDREF(posImpl.p);
}

FdoIDirectPosition* FdoGeometryFactoryAbstract::CreatePosition(FdoIDirectPosition* position)
{
    FdoPtr<FdoDirectPositionImpl> posImpl = FdoDirectPositionImpl::Create();

	posImpl->SetX(position->GetX());
	posImpl->SetY(position->GetY());
	posImpl->SetZ(position->GetZ());
	posImpl->SetM(position->GetM());
	posImpl->SetDimensionality(position->GetDimensionality());

	return FDO_SAFE_ADDREF(posImpl.p);
}

// Envelope
FdoIEnvelope* FdoGeometryFactoryAbstract::CreateEnvelope(FdoIDirectPosition * lowerLeft, FdoIDirectPosition * upperRight)
{
    return FdoEnvelopeImpl::Create(lowerLeft, upperRight);
}

FdoIEnvelope* FdoGeometryFactoryAbstract::CreateEnvelope(FdoInt32 dimensionType, double * ordinates)
{
	return FdoEnvelopeImpl::Create(dimensionType, ordinates);
}

FdoIEnvelope* FdoGeometryFactoryAbstract::CreateEnvelopeXY(double lowerLeftX, double lowerLeftY, double upperRightX, double upperRightY)
{
    return FdoEnvelopeImpl::Create(lowerLeftX, lowerLeftY, upperRightX, upperRightY);
}

FdoIEnvelope* FdoGeometryFactoryAbstract::CreateEnvelopeXYZ(double lowerLeftX, double lowerLeftY, double lowerLeftZ, 
													   double upperRightX, double upperRightY, double upperRightZ)
{
    return FdoEnvelopeImpl::Create(lowerLeftX, lowerLeftY, lowerLeftZ, 
                                   upperRightX, upperRightY, upperRightZ);
}

FdoIEnvelope* FdoGeometryFactoryAbstract::CreateEnvelope(FdoIEnvelope * envelope)			// Pseudo copy constructor
{
	if ( 0 == envelope )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoEnvelopeImpl* envlImpl = FdoEnvelopeImpl::Create(
		envelope->GetMinX(),
		envelope->GetMinY(),
		envelope->GetMinZ(),
		envelope->GetMaxX(),
		envelope->GetMaxY(),
		envelope->GetMaxZ());

    return envlImpl;
}

FdoIGeometry * FdoGeometryFactoryAbstract::CreateGeometry(FdoString* fgft)
{
	throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}



