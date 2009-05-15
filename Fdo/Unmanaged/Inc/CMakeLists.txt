INSTALL( FILES
        Fdo.h
        FdoStd.h
        FdoCommon.h
        FdoGeometry.h
        DESTINATION include )

INSTALL(
        DIRECTORY Common Fdo Geometry
        DESTINATION include
        PATTERN ".svn" EXCLUDE
        PATTERN "TransformerXalan.h" EXCLUDE
        )
