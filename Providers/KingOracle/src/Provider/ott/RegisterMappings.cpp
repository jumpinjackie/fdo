
#ifndef REGISTERMAPPINGS_ORACLE
# include "RegisterMappings.h"
#endif

void RegisterClasses(oracle::occi::Environment* envOCCI_)
{
  oracle::occi::Map *mapOCCI_ = envOCCI_->getMap();
  mapOCCI_->put("MDSYS.SDO_POINT_TYPE", &SDO_POINT_TYPE2::readSQL, &SDO_POINT_TYPE2::writeSQL);
  mapOCCI_->put("MDSYS.SDO_GEOMETRY", &SDO_GEOMETRY::readSQL, &SDO_GEOMETRY::writeSQL);
  mapOCCI_->put("MDSYS.SDO_TOPO_GEOMETRY", &SDO_TOPO_GEOMETRY::readSQL, &SDO_TOPO_GEOMETRY::writeSQL);
  mapOCCI_->put("MDSYS.SDO_TOPO_OBJECT", &SDO_TOPO_OBJECT::readSQL, &SDO_TOPO_OBJECT::writeSQL);
  mapOCCI_->put("MDSYS.SDO_DIM_ELEMENT", &SDO_DIM_ELEMENT::readSQL, &SDO_DIM_ELEMENT::writeSQL);
}
