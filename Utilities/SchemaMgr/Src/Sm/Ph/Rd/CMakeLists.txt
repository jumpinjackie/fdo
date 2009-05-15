include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX )

set( SmPhRd_STAT_SRCS
   AssociationReader.cpp
   BaseObjectReader.cpp
   CharacterSetReader.cpp
   ClassReader.cpp
   CollationReader.cpp
   ColumnReader.cpp
   ConstraintReader.cpp
   CoordSysReader.cpp
   DbObjectBinds.cpp
   DbObjectReader.cpp
   FkeyReader.cpp
   IndexReader.cpp
   Join.cpp
   OwnerReader.cpp
   PkeyReader.cpp
   PropertyReader.cpp
   QueryReader.cpp
   SchemaReader.cpp
   SpatialContextGeomReader.cpp
   SpatialContextGroupReader.cpp
   SpatialContextReader.cpp
   TableJoin.cpp
   ViewReader.cpp
)

add_library( SmPhRd STATIC ${SmPhRd_STAT_SRCS} )
