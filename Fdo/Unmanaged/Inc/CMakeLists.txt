INSTALL( FILES
    Fdo.h
    FdoStd.h
    FdoCommon.h
    FdoGeometry.h
    FdoSpatial.h
    FdoLimits.h
    DESTINATION include 
)

INSTALL(
    DIRECTORY Common Fdo Geometry Spatial
    DESTINATION include
    PATTERN *.h
    PATTERN "TransformerXalan.h" EXCLUDE
    PATTERN .svn EXCLUDE
)
