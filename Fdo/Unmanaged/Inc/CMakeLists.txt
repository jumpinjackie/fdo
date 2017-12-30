install( FILES
    Fdo.h
    FdoStd.h
    FdoCommon.h
    FdoGeometry.h
    FdoSpatial.h
    FdoLimits.h
    DESTINATION include
    COMPONENT ${FDO_COMPONENT}
)

install(
    DIRECTORY Common Fdo Geometry Spatial
    DESTINATION include
    COMPONENT ${FDO_COMPONENT}
    PATTERN *.h
    PATTERN "TransformerXalan.h" EXCLUDE
    PATTERN .svn EXCLUDE
)
