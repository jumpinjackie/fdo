include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

set( FDOGeometry_STAT_SRCS
   Parse/yyFgft.cpp
   Fgf/CircularArcSegment.cpp
   Fgf/CurvePolygon.cpp
   Fgf/CurveString.cpp
   Fgf/GeometryFactory.cpp
   Fgf/LineString.cpp
   Fgf/LineStringSegment.cpp
   Fgf/LinearRing.cpp
   Fgf/MultiCurvePolygon.cpp
   Fgf/MultiCurveString.cpp
   Fgf/MultiGeometry.cpp
   Fgf/MultiLineString.cpp
   Fgf/MultiPoint.cpp
   Fgf/MultiPolygon.cpp
   Fgf/Point.cpp
   Fgf/Polygon.cpp
   Fgf/Ring.cpp
   Fgf/Util.cpp
   Fgf/GeometryDll.cpp
   Fgf/GeometryThreadData.cpp
   DirectPositionImpl.cpp
   EnvelopeImpl.cpp
   GeometryFactoryAbstract.cpp
   GeometryUtility.cpp
   Parse/LexFgft.cpp
   Parse/ParseFgft.cpp
)

add_library( FDOGeometry STATIC ${FDOGeometry_STAT_SRCS} )
