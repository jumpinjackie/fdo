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
 */

#ifndef ARCSDESPATIALCONTEXTUTILITY_H
#define ARCSDESPATIALCONTEXTUTILITY_H


class ArcSDESpatialContextUtility
{
public:
    // Convert spatial context name to ArcSDE SRID:
    static LONG SpatialContextNameToSRID(ArcSDEConnection* connection, FdoString *spatialContextName);

    // Get the spatial context name from an ArcSDE SRID:
    static FdoStringP SRIDToSpatialContextName(ArcSDEConnection* connection, LONG srid);

    // Get the spatial context name from an ArcSDE SE_SPATIALREFINFO:
    static FdoStringP GetSpatialContextName(SE_SPATIALREFINFO spatialRefInfo);
};


#endif // ARCSDESPATIALCONTEXTUTILITY_H

