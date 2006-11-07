/*
* Copyright (C) 2006  SL-King d.o.o
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
#include "StdAfx.h"

#ifndef SDO_GEOMETRY_ORACLE
# include "SDO_GEOMETRY.h"
#endif


/*****************************************************************/
//  generated method implementations for the SDO_POINT_TYPE object type.
/*****************************************************************/

oracle::occi::Number SDO_POINT_TYPE2::getX() const
{
  return X;
}

void SDO_POINT_TYPE2::setX(const oracle::occi::Number &value)
{
  X = value;
}

oracle::occi::Number SDO_POINT_TYPE2::getY() const
{
  return Y;
}

void SDO_POINT_TYPE2::setY(const oracle::occi::Number &value)
{
  Y = value;
}

oracle::occi::Number SDO_POINT_TYPE2::getZ() const
{
  return Z;
}

void SDO_POINT_TYPE2::setZ(const oracle::occi::Number &value)
{
  Z = value;
}

void *SDO_POINT_TYPE2::operator new(size_t size)
{
  return oracle::occi::PObject::operator new(size);
}

void *SDO_POINT_TYPE2::operator new(size_t size, const oracle::occi::Connection * sess,
  const OCCI_STD_NAMESPACE::string& table)
{
  return oracle::occi::PObject::operator new(size, sess, table, 
            (char *) "MDSYS.SDO_POINT_TYPE");
}

void *SDO_POINT_TYPE2::operator new(size_t size, void *ctxOCCI_)
{
 return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *SDO_POINT_TYPE2::operator new(size_t size,
    const oracle::occi::Connection *sess,
    const OCCI_STD_NAMESPACE::string &tableName, 
    const OCCI_STD_NAMESPACE::string &typeName,
    const OCCI_STD_NAMESPACE::string &tableSchema, 
    const OCCI_STD_NAMESPACE::string &typeSchema)
{
  return oracle::occi::PObject::operator new(size, sess, tableName,
        typeName, tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string SDO_POINT_TYPE2::getSQLTypeName() const
{
  return OCCI_STD_NAMESPACE::string("MDSYS.SDO_POINT_TYPE");
}

void SDO_POINT_TYPE2::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
    unsigned int &schemaNameLen, void **typeName, unsigned int &typeNameLen) const
{
  PObject::getSQLTypeName(env, &SDO_POINT_TYPE2::readSQL, schemaName,
        schemaNameLen, typeName, typeNameLen);
}

SDO_POINT_TYPE2::SDO_POINT_TYPE2()
{
}

void *SDO_POINT_TYPE2::readSQL(void *ctxOCCI_)
{
  SDO_POINT_TYPE2 *objOCCI_ = new(ctxOCCI_) SDO_POINT_TYPE2(ctxOCCI_);
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (streamOCCI_.isNull())
      objOCCI_->setNull();
    else
      objOCCI_->readSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    delete objOCCI_;
    excep.setErrorCtx(ctxOCCI_);
    return (void *)NULL;
  }
  return (void *)objOCCI_;
}

void SDO_POINT_TYPE2::readSQL(oracle::occi::AnyData& streamOCCI_)
{
   X = streamOCCI_.getNumber();
   Y = streamOCCI_.getNumber();
   Z = streamOCCI_.getNumber();
}

void SDO_POINT_TYPE2::writeSQL(void *objectOCCI_, void *ctxOCCI_)
{
  SDO_POINT_TYPE2 *objOCCI_ = (SDO_POINT_TYPE2 *) objectOCCI_;
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (objOCCI_->isNull())
      streamOCCI_.setNull();
    else
      objOCCI_->writeSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    excep.setErrorCtx(ctxOCCI_);
  }
  return;
}

void SDO_POINT_TYPE2::writeSQL(oracle::occi::AnyData& streamOCCI_)
{
   streamOCCI_.setNumber(X);
   streamOCCI_.setNumber(Y);
   streamOCCI_.setNumber(Z);
}

SDO_POINT_TYPE2::~SDO_POINT_TYPE2()
{
  int i;
}

/*****************************************************************/
//  generated method implementations for the SDO_GEOMETRY object type.
/*****************************************************************/

oracle::occi::Number SDO_GEOMETRY::getSdo_gtype() const
{
  return SDO_GTYPE;
}

void SDO_GEOMETRY::setSdo_gtype(const oracle::occi::Number &value)
{
  SDO_GTYPE = value;
}

oracle::occi::Number SDO_GEOMETRY::getSdo_srid() const
{
  return SDO_SRID;
}

void SDO_GEOMETRY::setSdo_srid(const oracle::occi::Number &value)
{
  SDO_SRID = value;
}

SDO_POINT_TYPE2 * SDO_GEOMETRY::getSdo_point() const
{
  return SDO_POINT;
}

void SDO_GEOMETRY::setSdo_point(SDO_POINT_TYPE2 * value)
{
  SDO_POINT = value;
}

OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& SDO_GEOMETRY::getSdo_elem_info() 
{
  return SDO_ELEM_INFO;
}

const OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& SDO_GEOMETRY::getSdo_elem_info() const
{
  return SDO_ELEM_INFO;
}

void SDO_GEOMETRY::setSdo_elem_info(const OCCI_STD_NAMESPACE::vector< oracle::occi::Number > &value)
{
  SDO_ELEM_INFO = value;
}

OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& SDO_GEOMETRY::getSdo_ordinates() 
{
  return SDO_ORDINATES;
}

const OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& SDO_GEOMETRY::getSdo_ordinates() const
{
  return SDO_ORDINATES;
}

void SDO_GEOMETRY::setSdo_ordinates(const OCCI_STD_NAMESPACE::vector< oracle::occi::Number > &value)
{
  SDO_ORDINATES = value;
}

void *SDO_GEOMETRY::operator new(size_t size)
{
  return oracle::occi::PObject::operator new(size);
}

void *SDO_GEOMETRY::operator new(size_t size, const oracle::occi::Connection * sess,
  const OCCI_STD_NAMESPACE::string& table)
{
  return oracle::occi::PObject::operator new(size, sess, table, 
            (char *) "MDSYS.SDO_GEOMETRY");
}

void *SDO_GEOMETRY::operator new(size_t size, void *ctxOCCI_)
{
 return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *SDO_GEOMETRY::operator new(size_t size,
    const oracle::occi::Connection *sess,
    const OCCI_STD_NAMESPACE::string &tableName, 
    const OCCI_STD_NAMESPACE::string &typeName,
    const OCCI_STD_NAMESPACE::string &tableSchema, 
    const OCCI_STD_NAMESPACE::string &typeSchema)
{
  return oracle::occi::PObject::operator new(size, sess, tableName,
        typeName, tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string SDO_GEOMETRY::getSQLTypeName() const
{
  return OCCI_STD_NAMESPACE::string("MDSYS.SDO_GEOMETRY");
}

void SDO_GEOMETRY::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
    unsigned int &schemaNameLen, void **typeName, unsigned int &typeNameLen) const
{
  PObject::getSQLTypeName(env, &SDO_GEOMETRY::readSQL, schemaName,
        schemaNameLen, typeName, typeNameLen);
}

SDO_GEOMETRY::SDO_GEOMETRY()
{
   SDO_POINT = (SDO_POINT_TYPE2 *) 0;
}

void *SDO_GEOMETRY::readSQL(void *ctxOCCI_)
{
  SDO_GEOMETRY *objOCCI_ = new(ctxOCCI_) SDO_GEOMETRY(ctxOCCI_);
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (streamOCCI_.isNull())
      objOCCI_->setNull();
    else
      objOCCI_->readSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    delete objOCCI_;
    excep.setErrorCtx(ctxOCCI_);
    return (void *)NULL;
  }
  return (void *)objOCCI_;
}

void SDO_GEOMETRY::readSQL(oracle::occi::AnyData& streamOCCI_)
{
   SDO_GTYPE = streamOCCI_.getNumber();
   SDO_SRID = streamOCCI_.getNumber();
   SDO_POINT = (SDO_POINT_TYPE2 *) streamOCCI_.getObject(&SDO_POINT_TYPE2::readSQL);
   oracle::occi::getVector(streamOCCI_, SDO_ELEM_INFO);
   oracle::occi::getVector(streamOCCI_, SDO_ORDINATES);
}

void SDO_GEOMETRY::writeSQL(void *objectOCCI_, void *ctxOCCI_)
{
  SDO_GEOMETRY *objOCCI_ = (SDO_GEOMETRY *) objectOCCI_;
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (objOCCI_->isNull())
      streamOCCI_.setNull();
    else
      objOCCI_->writeSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    excep.setErrorCtx(ctxOCCI_);
  }
  return;
}

void SDO_GEOMETRY::writeSQL(oracle::occi::AnyData& streamOCCI_)
{
   streamOCCI_.setNumber(SDO_GTYPE);
   streamOCCI_.setNumber(SDO_SRID);
   streamOCCI_.setObject(SDO_POINT);
   oracle::occi::setVector(streamOCCI_, SDO_ELEM_INFO);
   oracle::occi::setVector(streamOCCI_, SDO_ORDINATES);
}

SDO_GEOMETRY::~SDO_GEOMETRY()
{
  int i;
  delete SDO_POINT;
}

/*****************************************************************/
//  generated method implementations for the SDO_TOPO_GEOMETRY object type.
/*****************************************************************/

oracle::occi::Number SDO_TOPO_GEOMETRY::getTg_type() const
{
  return TG_TYPE;
}

void SDO_TOPO_GEOMETRY::setTg_type(const oracle::occi::Number &value)
{
  TG_TYPE = value;
}

oracle::occi::Number SDO_TOPO_GEOMETRY::getTg_id() const
{
  return TG_ID;
}

void SDO_TOPO_GEOMETRY::setTg_id(const oracle::occi::Number &value)
{
  TG_ID = value;
}

oracle::occi::Number SDO_TOPO_GEOMETRY::getTg_layer_id() const
{
  return TG_LAYER_ID;
}

void SDO_TOPO_GEOMETRY::setTg_layer_id(const oracle::occi::Number &value)
{
  TG_LAYER_ID = value;
}

oracle::occi::Number SDO_TOPO_GEOMETRY::getTopology_id() const
{
  return TOPOLOGY_ID;
}

void SDO_TOPO_GEOMETRY::setTopology_id(const oracle::occi::Number &value)
{
  TOPOLOGY_ID = value;
}

void *SDO_TOPO_GEOMETRY::operator new(size_t size)
{
  return oracle::occi::PObject::operator new(size);
}

void *SDO_TOPO_GEOMETRY::operator new(size_t size, const oracle::occi::Connection * sess,
  const OCCI_STD_NAMESPACE::string& table)
{
  return oracle::occi::PObject::operator new(size, sess, table, 
            (char *) "MDSYS.SDO_TOPO_GEOMETRY");
}

void *SDO_TOPO_GEOMETRY::operator new(size_t size, void *ctxOCCI_)
{
 return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *SDO_TOPO_GEOMETRY::operator new(size_t size,
    const oracle::occi::Connection *sess,
    const OCCI_STD_NAMESPACE::string &tableName, 
    const OCCI_STD_NAMESPACE::string &typeName,
    const OCCI_STD_NAMESPACE::string &tableSchema, 
    const OCCI_STD_NAMESPACE::string &typeSchema)
{
  return oracle::occi::PObject::operator new(size, sess, tableName,
        typeName, tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string SDO_TOPO_GEOMETRY::getSQLTypeName() const
{
  return OCCI_STD_NAMESPACE::string("MDSYS.SDO_TOPO_GEOMETRY");
}

void SDO_TOPO_GEOMETRY::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
    unsigned int &schemaNameLen, void **typeName, unsigned int &typeNameLen) const
{
  PObject::getSQLTypeName(env, &SDO_TOPO_GEOMETRY::readSQL, schemaName,
        schemaNameLen, typeName, typeNameLen);
}

SDO_TOPO_GEOMETRY::SDO_TOPO_GEOMETRY()
{
}

void *SDO_TOPO_GEOMETRY::readSQL(void *ctxOCCI_)
{
  SDO_TOPO_GEOMETRY *objOCCI_ = new(ctxOCCI_) SDO_TOPO_GEOMETRY(ctxOCCI_);
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (streamOCCI_.isNull())
      objOCCI_->setNull();
    else
      objOCCI_->readSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    delete objOCCI_;
    excep.setErrorCtx(ctxOCCI_);
    return (void *)NULL;
  }
  return (void *)objOCCI_;
}

void SDO_TOPO_GEOMETRY::readSQL(oracle::occi::AnyData& streamOCCI_)
{
   TG_TYPE = streamOCCI_.getNumber();
   TG_ID = streamOCCI_.getNumber();
   TG_LAYER_ID = streamOCCI_.getNumber();
   TOPOLOGY_ID = streamOCCI_.getNumber();
}

void SDO_TOPO_GEOMETRY::writeSQL(void *objectOCCI_, void *ctxOCCI_)
{
  SDO_TOPO_GEOMETRY *objOCCI_ = (SDO_TOPO_GEOMETRY *) objectOCCI_;
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (objOCCI_->isNull())
      streamOCCI_.setNull();
    else
      objOCCI_->writeSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    excep.setErrorCtx(ctxOCCI_);
  }
  return;
}

void SDO_TOPO_GEOMETRY::writeSQL(oracle::occi::AnyData& streamOCCI_)
{
   streamOCCI_.setNumber(TG_TYPE);
   streamOCCI_.setNumber(TG_ID);
   streamOCCI_.setNumber(TG_LAYER_ID);
   streamOCCI_.setNumber(TOPOLOGY_ID);
}

SDO_TOPO_GEOMETRY::~SDO_TOPO_GEOMETRY()
{
  int i;
}

/*****************************************************************/
//  generated method implementations for the SDO_TOPO_OBJECT object type.
/*****************************************************************/

oracle::occi::Number SDO_TOPO_OBJECT::getTopo_id() const
{
  return TOPO_ID;
}

void SDO_TOPO_OBJECT::setTopo_id(const oracle::occi::Number &value)
{
  TOPO_ID = value;
}

oracle::occi::Number SDO_TOPO_OBJECT::getTopo_type() const
{
  return TOPO_TYPE;
}

void SDO_TOPO_OBJECT::setTopo_type(const oracle::occi::Number &value)
{
  TOPO_TYPE = value;
}

void *SDO_TOPO_OBJECT::operator new(size_t size)
{
  return oracle::occi::PObject::operator new(size);
}

void *SDO_TOPO_OBJECT::operator new(size_t size, const oracle::occi::Connection * sess,
  const OCCI_STD_NAMESPACE::string& table)
{
  return oracle::occi::PObject::operator new(size, sess, table, 
            (char *) "MDSYS.SDO_TOPO_OBJECT");
}

void *SDO_TOPO_OBJECT::operator new(size_t size, void *ctxOCCI_)
{
 return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *SDO_TOPO_OBJECT::operator new(size_t size,
    const oracle::occi::Connection *sess,
    const OCCI_STD_NAMESPACE::string &tableName, 
    const OCCI_STD_NAMESPACE::string &typeName,
    const OCCI_STD_NAMESPACE::string &tableSchema, 
    const OCCI_STD_NAMESPACE::string &typeSchema)
{
  return oracle::occi::PObject::operator new(size, sess, tableName,
        typeName, tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string SDO_TOPO_OBJECT::getSQLTypeName() const
{
  return OCCI_STD_NAMESPACE::string("MDSYS.SDO_TOPO_OBJECT");
}

void SDO_TOPO_OBJECT::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
    unsigned int &schemaNameLen, void **typeName, unsigned int &typeNameLen) const
{
  PObject::getSQLTypeName(env, &SDO_TOPO_OBJECT::readSQL, schemaName,
        schemaNameLen, typeName, typeNameLen);
}

SDO_TOPO_OBJECT::SDO_TOPO_OBJECT()
{
}

void *SDO_TOPO_OBJECT::readSQL(void *ctxOCCI_)
{
  SDO_TOPO_OBJECT *objOCCI_ = new(ctxOCCI_) SDO_TOPO_OBJECT(ctxOCCI_);
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (streamOCCI_.isNull())
      objOCCI_->setNull();
    else
      objOCCI_->readSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    delete objOCCI_;
    excep.setErrorCtx(ctxOCCI_);
    return (void *)NULL;
  }
  return (void *)objOCCI_;
}

void SDO_TOPO_OBJECT::readSQL(oracle::occi::AnyData& streamOCCI_)
{
   TOPO_ID = streamOCCI_.getNumber();
   TOPO_TYPE = streamOCCI_.getNumber();
}

void SDO_TOPO_OBJECT::writeSQL(void *objectOCCI_, void *ctxOCCI_)
{
  SDO_TOPO_OBJECT *objOCCI_ = (SDO_TOPO_OBJECT *) objectOCCI_;
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (objOCCI_->isNull())
      streamOCCI_.setNull();
    else
      objOCCI_->writeSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    excep.setErrorCtx(ctxOCCI_);
  }
  return;
}

void SDO_TOPO_OBJECT::writeSQL(oracle::occi::AnyData& streamOCCI_)
{
   streamOCCI_.setNumber(TOPO_ID);
   streamOCCI_.setNumber(TOPO_TYPE);
}

SDO_TOPO_OBJECT::~SDO_TOPO_OBJECT()
{
  int i;
}

/*****************************************************************/
//  generated method implementations for the SDO_DIM_ELEMENT object type.
/*****************************************************************/

OCCI_STD_NAMESPACE::string SDO_DIM_ELEMENT::getSdo_dimname() const
{
  return SDO_DIMNAME;
}

void SDO_DIM_ELEMENT::setSdo_dimname(const OCCI_STD_NAMESPACE::string &value)
{
  SDO_DIMNAME = value;
}

oracle::occi::Number SDO_DIM_ELEMENT::getSdo_lb() const
{
  return SDO_LB;
}

void SDO_DIM_ELEMENT::setSdo_lb(const oracle::occi::Number &value)
{
  SDO_LB = value;
}

oracle::occi::Number SDO_DIM_ELEMENT::getSdo_ub() const
{
  return SDO_UB;
}

void SDO_DIM_ELEMENT::setSdo_ub(const oracle::occi::Number &value)
{
  SDO_UB = value;
}

oracle::occi::Number SDO_DIM_ELEMENT::getSdo_tolerance() const
{
  return SDO_TOLERANCE;
}

void SDO_DIM_ELEMENT::setSdo_tolerance(const oracle::occi::Number &value)
{
  SDO_TOLERANCE = value;
}

void *SDO_DIM_ELEMENT::operator new(size_t size)
{
  return oracle::occi::PObject::operator new(size);
}

void *SDO_DIM_ELEMENT::operator new(size_t size, const oracle::occi::Connection * sess,
  const OCCI_STD_NAMESPACE::string& table)
{
  return oracle::occi::PObject::operator new(size, sess, table, 
            (char *) "MDSYS.SDO_DIM_ELEMENT");
}

void *SDO_DIM_ELEMENT::operator new(size_t size, void *ctxOCCI_)
{
 return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *SDO_DIM_ELEMENT::operator new(size_t size,
    const oracle::occi::Connection *sess,
    const OCCI_STD_NAMESPACE::string &tableName, 
    const OCCI_STD_NAMESPACE::string &typeName,
    const OCCI_STD_NAMESPACE::string &tableSchema, 
    const OCCI_STD_NAMESPACE::string &typeSchema)
{
  return oracle::occi::PObject::operator new(size, sess, tableName,
        typeName, tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string SDO_DIM_ELEMENT::getSQLTypeName() const
{
  return OCCI_STD_NAMESPACE::string("MDSYS.SDO_DIM_ELEMENT");
}

void SDO_DIM_ELEMENT::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
    unsigned int &schemaNameLen, void **typeName, unsigned int &typeNameLen) const
{
  PObject::getSQLTypeName(env, &SDO_DIM_ELEMENT::readSQL, schemaName,
        schemaNameLen, typeName, typeNameLen);
}

SDO_DIM_ELEMENT::SDO_DIM_ELEMENT()
{
}

void *SDO_DIM_ELEMENT::readSQL(void *ctxOCCI_)
{
  SDO_DIM_ELEMENT *objOCCI_ = new(ctxOCCI_) SDO_DIM_ELEMENT(ctxOCCI_);
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (streamOCCI_.isNull())
      objOCCI_->setNull();
    else
      objOCCI_->readSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    delete objOCCI_;
    excep.setErrorCtx(ctxOCCI_);
    return (void *)NULL;
  }
  return (void *)objOCCI_;
}

void SDO_DIM_ELEMENT::readSQL(oracle::occi::AnyData& streamOCCI_)
{
   SDO_DIMNAME = streamOCCI_.getString();
   SDO_LB = streamOCCI_.getNumber();
   SDO_UB = streamOCCI_.getNumber();
   SDO_TOLERANCE = streamOCCI_.getNumber();
}

void SDO_DIM_ELEMENT::writeSQL(void *objectOCCI_, void *ctxOCCI_)
{
  SDO_DIM_ELEMENT *objOCCI_ = (SDO_DIM_ELEMENT *) objectOCCI_;
  oracle::occi::AnyData streamOCCI_(ctxOCCI_);

  try
  {
    if (objOCCI_->isNull())
      streamOCCI_.setNull();
    else
      objOCCI_->writeSQL(streamOCCI_);
  }
  catch (oracle::occi::SQLException& excep)
  {
    excep.setErrorCtx(ctxOCCI_);
  }
  return;
}

void SDO_DIM_ELEMENT::writeSQL(oracle::occi::AnyData& streamOCCI_)
{
   streamOCCI_.setString(SDO_DIMNAME);
   streamOCCI_.setNumber(SDO_LB);
   streamOCCI_.setNumber(SDO_UB);
   streamOCCI_.setNumber(SDO_TOLERANCE);
}

SDO_DIM_ELEMENT::~SDO_DIM_ELEMENT()
{
  
}
