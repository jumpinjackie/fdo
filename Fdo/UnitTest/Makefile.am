##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = UnitTest

EXTRA_DIST = \
    sc_v2.xml \
    schema_all_v2.xml \
    schema_all_v2_out_master.txt \
    schema_ext_b_master.txt \
    schema_ext_c_master.txt \
    schema_ext1b.xml \
    gml_blob.xml \
    gml_blob_hex.xml \
    gml_blob_hex_schema.xml \
    gml_blob_schema.xml \
    gml_inherited_feature_schema.xml \
    gml_multi_geometry.xml \
    gml_multi_geometry_schema.xml \
    gml_nested_collection.xml \
    gml_nested_feature.xml \
    gml_nested_geometry.xml \
    gml_nested_geometry_schema.xml \
    gml_nested_object.xml \
    gml_nested_object_schema.xml \
    gml_simple_feature.xml \
    gml_simple_feature_schema.xml \
    gml_simple_geometry.xml \
    gml_simple_geometry_schema.xml \
    gml_simple_object.xml \
    gml_simple_object_schema.xml \
	gml3_multi_geometry.xml \
	gml3_multi_geometry_schema.xml \
	gml3_simple_geometry.xml \
	gml3_simple_geometry_schema.xml \
    g_coords.txt \
    sc1a_master.txt \
    sc1b_master.txt \
    sc2a_master.txt \
    sc2b_master.txt \
    schema_all1_master.txt \
    schema_ext1.xml \
    schema_ext_a_master.txt \
    schema_mappings_212_master.txt \
    schema_merge1a.xml \
    schema_merge1b.xml \
    schema_merge2a_master.txt \
    schema_merge2b_master.txt \
    schema_merge_err_master.txt \
    schema_refs1a_master.txt \
    schema_refs1b_master.txt \
    schema_refs1c_master.txt \
    schema_refs2_master.txt \
    schema_refs_err_master.txt \
    schema_ref_err1_master.txt \
    schema_ref_err2_master.txt \
    schema_unsupported.xsd \
    schema_uns_high_master.txt \
    schema_uns_low_master.txt \
    schema_uns_norm_master.txt \
    sc_err1a_master.txt \
    sc_err1b_master.txt \
    gml_write_master.txt \
	gml3_write_master.txt \
    sc_error.xml \
    UnitTest.vcproj

UnitTest_SOURCES = \
    app.cpp \
    CapabilitiesTest.cpp \
    ClientServicesTest.cpp \
    CommandTest.cpp \
    CommonTest.cpp \
    ExpressionParseTest.cpp \
    ExpressionTest.cpp \
    FilterParseTest.cpp \
    FilterTest.cpp \
    GeometryTest.cpp \
    GeometryMemPerfTest.cpp \
    IoTest.cpp \
    Pch.cpp \
    SchemaTest.cpp \
    SpatialContextTest.cpp \
    TestProcessor.cpp \
    UnitTestUtil.cpp \
    XmlFormatter.cpp \
    XmlTest.cpp \
    GmlWriteTest.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
    SchemaTest.h \
    CapabilitiesTest.h \
    ClientServicesTest.h \
    CommandTest.h \
    CommonTest.h \
    ExpressionParseTest.h \
    ExpressionTest.h \
    FilterParseTest.h \
    FilterTest.h \
    GeometryMemPerfTest.h \
    GeometryTest.h \
    IoTest.h \
    GmlTest.h \
    GmlWriteTest.h \
    Pch.h \
    SpatialContextTest.h \
    SpatialUtilityTest.h \
    TestProcessor.h \
    UnitTestUtil.h \
    XmlFormatter.h \
    XmlTest.h

INCLUDES = \
  -I../Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include

UnitTest_LDADD = \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  ../Unmanaged/Src/libFDO.la

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

