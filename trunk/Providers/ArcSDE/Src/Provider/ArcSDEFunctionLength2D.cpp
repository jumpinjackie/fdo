//
// 
// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
//

#include <stdafx.h>
#include <ArcSDEFunctionLength2D.h>

ArcSDEFunctionLength2D::ArcSDEFunctionLength2D ()
{
	mCoordRef = NULL;
} 

ArcSDEFunctionLength2D::ArcSDEFunctionLength2D (ArcSDEConnection* connection, SE_COORDREF coordRef)
{
	mConnection = connection;
	mCoordRef = coordRef;
} 

ArcSDEFunctionLength2D::~ArcSDEFunctionLength2D ()
{
} 

ArcSDEFunctionLength2D *ArcSDEFunctionLength2D::Create (ArcSDEConnection* connection, SE_COORDREF coordRef)
{
    return new ArcSDEFunctionLength2D(connection, coordRef);
}

FdoLiteralValue *ArcSDEFunctionLength2D::Evaluate (FdoLiteralValueCollection *literal_values)
{
	FdoDouble	length = 0.0;
 
    FdoPtr<FdoGeometryValue> geom_value = (FdoGeometryValue *) literal_values->GetItem(0);
    if (geom_value->IsNull())
		return FdoDoubleValue::Create();

	// Create a geometry object
    FdoByteArray*	fgf = geom_value->GetGeometry();
	SE_SHAPE		result_shape; 

	// Convert to SDE
	convert_fgf_to_sde_shape(mConnection, fgf, mCoordRef, result_shape, TRUE);

	// Compute Length
	LONG lResult = SE_shape_get_length (result_shape, 0, &length);
    handle_sde_err<FdoCommandException> (mConnection->GetConnection (), lResult, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Failed to compute length.");

	return FdoDoubleValue::Create (length);
}


