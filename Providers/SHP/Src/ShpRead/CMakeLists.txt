include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${FDO}/Unmanaged/Inc
	${FDOUTILITIES}/Common/Inc
	)

set( SHPRead_SRCS
   DoublePoint.cpp
   BoundingBox.cpp
   BoundingBoxEx.cpp
   ColumnInfo.cpp
   MultiPatchShape.cpp
   MultiPointMShape.cpp
   MultiPointShape.cpp
   MultiPointZShape.cpp
   MZDataBase.cpp
   MZData.cpp
   NullShape.cpp
   PointMShape.cpp
   PointShape.cpp
   PointZShape.cpp
   PolygonMShape.cpp
   PolygonShape.cpp
   PolygonZShape.cpp
   PolylineMShape.cpp
   PolylineShape.cpp
   PolylineZShape.cpp
   PolyShape.cpp
   RowData.cpp
   Shape.cpp
   ShapeCPG.cpp
   ShapeDBF.cpp
   ShapePRJ.cpp
   ShapeFileBase.cpp
   ShapeFile.cpp
   ShapeIndex.cpp
   stdafx.cpp
)

add_library( SHPRead STATIC ${SHPRead_SRCS} )

add_dependencies( SHPRead SHPMessage )
