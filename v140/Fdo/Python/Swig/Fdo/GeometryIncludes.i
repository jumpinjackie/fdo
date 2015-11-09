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
 
/* Handle the collection templates FdoIGeometry and FdoICurveSegmentAbstractCollection */
%template (BaseFdoIGeometryCollection) FdoCollection<FdoIGeometry,FdoException>;
%template (BaseFdoICurveSegmentAbstractCollection) FdoCollection<FdoICurveSegmentAbstract,FdoException>;
%template (BaseFdoIDirectPositionCollection) FdoCollection<FdoIDirectPosition,FdoException>;
%template (BaseFdoILineStringCollection) FdoCollection<FdoILineString,FdoException>;
%template (BaseFdoIPointCollection) FdoCollection<FdoIPoint,FdoException>;
%template (BaseFdoILinearRingCollection) FdoCollection<FdoILinearRing,FdoException>;
%template (BaseFdoIPolygonCollection) FdoCollection<FdoIPolygon,FdoException>;
%template (BaseFdoICurveStringCollection) FdoCollection<FdoICurveString,FdoException>;
%template (BaseFdoIRingCollection) FdoCollection<FdoIRing,FdoException>;
%template (BaseFdoIICurvePolygonCollection) FdoCollection<FdoICurvePolygon,FdoException>;


/* Include the geometry base classes */
%include "Common/Array.h"
%include "Common/Dimensionality.h"
%include "Common/GeometryType.h"
%include "Geometry/IGeometry.h"
%include "Geometry/IRingAbstract.h"
%include "Geometry/ICurveAbstract.h"
%include "Geometry/ISurfaceAbstract.h"
%include "Geometry/ICurveSegmentAbstract.h"
%include "Geometry/IArcSegmentAbstract.h"
%include "Geometry/IGeometricAggregateAbstract.h"
%include "Geometry/IEnvelope.h"
%include "Geometry/IDirectPosition.h"
%include "Geometry/DirectPositionImpl.h"
%include "Geometry/ILineString.h"
%include "Geometry/IPoint.h"
%include "Geometry/ILinearRing.h"
%include "Geometry/ILineStringSegment.h"
%include "Geometry/IPolygon.h"
%include "Geometry/IMultiPoint.h"
%include "Geometry/IMultiGeometry.h"
%include "Geometry/IMultiLineString.h"
%include "Geometry/IMultiPolygon.h"
%include "Geometry/ICircularArcSegment.h"
%include "Geometry/ICurveString.h"
%include "Geometry/IMultiCurveString.h"
%include "Geometry/IRing.h"
%include "Geometry/ICurvePolygon.h"
%include "Geometry/IMultiCurvePolygon.h"

%include "Geometry/GeometryStream/GeometryStreamFactory.h"
%include "Geometry/GeometryStream/GeometryStreamReader.h"
%include "Geometry/GeometryStream/GeometryStreamWriter.h"
%include "Geometry/GeometryFactoryAbstract.h"
%include "Geometry/Fgf/Factory.h"