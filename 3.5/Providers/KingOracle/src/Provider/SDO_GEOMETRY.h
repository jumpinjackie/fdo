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

#ifndef SDO_GEOMETRY_ORACLE
# define SDO_GEOMETRY_ORACLE

#ifndef OCCI_ORACLE
# include <occi.h>
#endif

class SDO_POINT_TYPE2;
class SDO_GEOMETRY;
class SDO_TOPO_GEOMETRY;
class SDO_TOPO_OBJECT;
class SDO_DIM_ELEMENT;

/************************************************************/
//  generated declarations for the SDO_POINT_TYPE object type.
/************************************************************/

class SDO_POINT_TYPE2 : public oracle::occi::PObject {

private:

   oracle::occi::Number X;
   oracle::occi::Number Y;
   oracle::occi::Number Z;

public:

   oracle::occi::Number getX() const;

   void setX(const oracle::occi::Number &value);

   oracle::occi::Number getY() const;

   void setY(const oracle::occi::Number &value);

   oracle::occi::Number getZ() const;

   void setZ(const oracle::occi::Number &value);

   void *operator new(size_t size);

   void *operator new(size_t size, const oracle::occi::Connection * sess,
      const OCCI_STD_NAMESPACE::string& table);

   void *operator new(size_t, void *ctxOCCI_);

   void *operator new(size_t size, const oracle::occi::Connection *sess,
      const OCCI_STD_NAMESPACE::string &tableName, 
      const OCCI_STD_NAMESPACE::string &typeName,
      const OCCI_STD_NAMESPACE::string &tableSchema, 
      const OCCI_STD_NAMESPACE::string &typeSchema);

   OCCI_STD_NAMESPACE::string getSQLTypeName() const;

   void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
      unsigned int &schemaNameLen, void **typeName,
      unsigned int &typeNameLen) const;

   SDO_POINT_TYPE2();

   SDO_POINT_TYPE2(void *ctxOCCI_) : oracle::occi::PObject (ctxOCCI_) { };

   static void *readSQL(void *ctxOCCI_);

   virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

   static void writeSQL(void *objOCCI_, void *ctxOCCI_);

   virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

   ~SDO_POINT_TYPE2();

};

/************************************************************/
//  generated declarations for the SDO_GEOMETRY object type.
/************************************************************/

class SDO_GEOMETRY : public oracle::occi::PObject {

private:

   oracle::occi::Number SDO_GTYPE;
   oracle::occi::Number SDO_SRID;
   SDO_POINT_TYPE2 * SDO_POINT;
   OCCI_STD_NAMESPACE::vector< oracle::occi::Number > SDO_ELEM_INFO;
   OCCI_STD_NAMESPACE::vector< oracle::occi::Number > SDO_ORDINATES;

public:

   oracle::occi::Number getSdo_gtype() const;

   void setSdo_gtype(const oracle::occi::Number &value);

   oracle::occi::Number getSdo_srid() const;

   void setSdo_srid(const oracle::occi::Number &value);

   SDO_POINT_TYPE2 * getSdo_point() const;

   void setSdo_point(SDO_POINT_TYPE2 * value);

   OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& getSdo_elem_info();

   const OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& getSdo_elem_info() const;

   void setSdo_elem_info(const OCCI_STD_NAMESPACE::vector< oracle::occi::Number > &value);

   OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& getSdo_ordinates();

   const OCCI_STD_NAMESPACE::vector< oracle::occi::Number >& getSdo_ordinates() const;

   void setSdo_ordinates(const OCCI_STD_NAMESPACE::vector< oracle::occi::Number > &value);

   void *operator new(size_t size);

   void *operator new(size_t size, const oracle::occi::Connection * sess,
      const OCCI_STD_NAMESPACE::string& table);

   void *operator new(size_t, void *ctxOCCI_);

   void *operator new(size_t size, const oracle::occi::Connection *sess,
      const OCCI_STD_NAMESPACE::string &tableName, 
      const OCCI_STD_NAMESPACE::string &typeName,
      const OCCI_STD_NAMESPACE::string &tableSchema, 
      const OCCI_STD_NAMESPACE::string &typeSchema);

   OCCI_STD_NAMESPACE::string getSQLTypeName() const;

   void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
      unsigned int &schemaNameLen, void **typeName,
      unsigned int &typeNameLen) const;

   SDO_GEOMETRY();

   SDO_GEOMETRY(void *ctxOCCI_) : oracle::occi::PObject (ctxOCCI_) { };

   static void *readSQL(void *ctxOCCI_);

   virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

   static void writeSQL(void *objOCCI_, void *ctxOCCI_);

   virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

   ~SDO_GEOMETRY();

};

/************************************************************/
//  generated declarations for the SDO_TOPO_GEOMETRY object type.
/************************************************************/

class SDO_TOPO_GEOMETRY : public oracle::occi::PObject {

private:

   oracle::occi::Number TG_TYPE;
   oracle::occi::Number TG_ID;
   oracle::occi::Number TG_LAYER_ID;
   oracle::occi::Number TOPOLOGY_ID;

public:

   oracle::occi::Number getTg_type() const;

   void setTg_type(const oracle::occi::Number &value);

   oracle::occi::Number getTg_id() const;

   void setTg_id(const oracle::occi::Number &value);

   oracle::occi::Number getTg_layer_id() const;

   void setTg_layer_id(const oracle::occi::Number &value);

   oracle::occi::Number getTopology_id() const;

   void setTopology_id(const oracle::occi::Number &value);

   void *operator new(size_t size);

   void *operator new(size_t size, const oracle::occi::Connection * sess,
      const OCCI_STD_NAMESPACE::string& table);

   void *operator new(size_t, void *ctxOCCI_);

   void *operator new(size_t size, const oracle::occi::Connection *sess,
      const OCCI_STD_NAMESPACE::string &tableName, 
      const OCCI_STD_NAMESPACE::string &typeName,
      const OCCI_STD_NAMESPACE::string &tableSchema, 
      const OCCI_STD_NAMESPACE::string &typeSchema);

   OCCI_STD_NAMESPACE::string getSQLTypeName() const;

   void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
      unsigned int &schemaNameLen, void **typeName,
      unsigned int &typeNameLen) const;

   SDO_TOPO_GEOMETRY();

   SDO_TOPO_GEOMETRY(void *ctxOCCI_) : oracle::occi::PObject (ctxOCCI_) { };

   static void *readSQL(void *ctxOCCI_);

   virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

   static void writeSQL(void *objOCCI_, void *ctxOCCI_);

   virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

   ~SDO_TOPO_GEOMETRY();

};

/************************************************************/
//  generated declarations for the SDO_TOPO_OBJECT object type.
/************************************************************/

class SDO_TOPO_OBJECT : public oracle::occi::PObject {

private:

   oracle::occi::Number TOPO_ID;
   oracle::occi::Number TOPO_TYPE;

public:

   oracle::occi::Number getTopo_id() const;

   void setTopo_id(const oracle::occi::Number &value);

   oracle::occi::Number getTopo_type() const;

   void setTopo_type(const oracle::occi::Number &value);

   void *operator new(size_t size);

   void *operator new(size_t size, const oracle::occi::Connection * sess,
      const OCCI_STD_NAMESPACE::string& table);

   void *operator new(size_t, void *ctxOCCI_);

   void *operator new(size_t size, const oracle::occi::Connection *sess,
      const OCCI_STD_NAMESPACE::string &tableName, 
      const OCCI_STD_NAMESPACE::string &typeName,
      const OCCI_STD_NAMESPACE::string &tableSchema, 
      const OCCI_STD_NAMESPACE::string &typeSchema);

   OCCI_STD_NAMESPACE::string getSQLTypeName() const;

   void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
      unsigned int &schemaNameLen, void **typeName,
      unsigned int &typeNameLen) const;

   SDO_TOPO_OBJECT();

   SDO_TOPO_OBJECT(void *ctxOCCI_) : oracle::occi::PObject (ctxOCCI_) { };

   static void *readSQL(void *ctxOCCI_);

   virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

   static void writeSQL(void *objOCCI_, void *ctxOCCI_);

   virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

   ~SDO_TOPO_OBJECT();

};

/************************************************************/
//  generated declarations for the SDO_DIM_ELEMENT object type.
/************************************************************/

class SDO_DIM_ELEMENT : public oracle::occi::PObject {

private:

   OCCI_STD_NAMESPACE::string SDO_DIMNAME;
   oracle::occi::Number SDO_LB;
   oracle::occi::Number SDO_UB;
   oracle::occi::Number SDO_TOLERANCE;

public:

   OCCI_STD_NAMESPACE::string getSdo_dimname() const;

   void setSdo_dimname(const OCCI_STD_NAMESPACE::string &value);

   oracle::occi::Number getSdo_lb() const;

   void setSdo_lb(const oracle::occi::Number &value);

   oracle::occi::Number getSdo_ub() const;

   void setSdo_ub(const oracle::occi::Number &value);

   oracle::occi::Number getSdo_tolerance() const;

   void setSdo_tolerance(const oracle::occi::Number &value);

   void *operator new(size_t size);

   void *operator new(size_t size, const oracle::occi::Connection * sess,
      const OCCI_STD_NAMESPACE::string& table);

   void *operator new(size_t, void *ctxOCCI_);

   void *operator new(size_t size, const oracle::occi::Connection *sess,
      const OCCI_STD_NAMESPACE::string &tableName, 
      const OCCI_STD_NAMESPACE::string &typeName,
      const OCCI_STD_NAMESPACE::string &tableSchema, 
      const OCCI_STD_NAMESPACE::string &typeSchema);

   OCCI_STD_NAMESPACE::string getSQLTypeName() const;

   void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
      unsigned int &schemaNameLen, void **typeName,
      unsigned int &typeNameLen) const;

   SDO_DIM_ELEMENT();

   SDO_DIM_ELEMENT(void *ctxOCCI_) : oracle::occi::PObject (ctxOCCI_) { };

   static void *readSQL(void *ctxOCCI_);

   virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

   static void writeSQL(void *objOCCI_, void *ctxOCCI_);

   virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

   ~SDO_DIM_ELEMENT();

};

#endif
