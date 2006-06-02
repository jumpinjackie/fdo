AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
   debugext.h \
   rdbi.h \
   Rdbms.h \
   tfn.h \
   ut.h \
   Util/da.h \
   Util/langinfo.h \
   Util/os.h \
   Util/stdlib.h \
   Util/string.h \
   Util/thread.h \
   Nls/fdordbms_msg.h \
   Nls/rdbi_msg.h \
   Geometry/Fgf/AllGeometry_c.h \
   SpatialManager/FdoRdbmsSpatialManager.h \
   SpatialManager/FdoRdbmsSpatialSecondaryFilter.h \
   SpatialManager/FdoRdbmsSpatialSqlFilter.h \
   Rdbms/FdoMySql.h \
   Rdbms/FdoOdbc.h \
   Rdbms/FdoRdbmsCommandType.h \
   Rdbms/ICreateSpatialIndex.h \
   Rdbms/IDestroySpatialIndex.h \
   Rdbms/IGetSpatialIndexes.h \
   Rdbms/ISpatialIndexReader.h \
   Rdbms/SpatialIndexDimensionType.h \
   Rdbms/SpatialIndexType.h \
   Rdbms/Override/RdbmsOv.h \
   Rdbms/Override/RdbmsOvAssociationPropertyDefinition.h \
   Rdbms/Override/RdbmsOvClassCollection.h \
   Rdbms/Override/RdbmsOvClassDefinition.h \
   Rdbms/Override/RdbmsOvColumn.h \
   Rdbms/Override/RdbmsOvDataPropertyDefinition.h \
   Rdbms/Override/RdbmsOvGeometricColumn.h \
   Rdbms/Override/RdbmsOvGeometricPropertyDefinition.h \
   Rdbms/Override/RdbmsOvObjectPropertyDefinition.h \
   Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h \
   Rdbms/Override/RdbmsOvPropertyDefinition.h \
   Rdbms/Override/RdbmsOvPropertyDefinitionCollection.h \
   Rdbms/Override/RdbmsOvPropertyMappingClass.h \
   Rdbms/Override/RdbmsOvPropertyMappingConcrete.h \
   Rdbms/Override/RdbmsOvPropertyMappingDefinition.h \
   Rdbms/Override/RdbmsOvPropertyMappingRelation.h \
   Rdbms/Override/RdbmsOvPropertyMappingSingle.h \
   Rdbms/Override/RdbmsOvSchemaAutoGeneration.h \
   Rdbms/Override/RdbmsOvTable.h \
   Rdbms/Override/ODBC/OdbcOv.h \
   Rdbms/Override/ODBC/OdbcOvClassCollection.h \
   Rdbms/Override/ODBC/OdbcOvClassDefinition.h \
   Rdbms/Override/ODBC/OdbcOvColumn.h \
   Rdbms/Override/ODBC/OdbcOvDataPropertyDefinition.h \
   Rdbms/Override/ODBC/OdbcOvGeometricPropertyDefinition.h \
   Rdbms/Override/ODBC/OdbcOvPhysicalSchemaMapping.h \
   Rdbms/Override/ODBC/OdbcOvPropertyDefinition.h \
   Rdbms/Override/ODBC/OdbcOvPropertyDefinitionCollection.h \
   Rdbms/Override/ODBC/OdbcOvTable.h \
   Rdbms/Override/MySQL/MySqlOv.h \
   Rdbms/Override/MySQL/MySqlOvAssociationPropertyDefinition.h \
   Rdbms/Override/MySQL/MySqlOvClassCollection.h \
   Rdbms/Override/MySQL/MySqlOvClassDefinition.h \
   Rdbms/Override/MySQL/MySqlOvColumn.h \
   Rdbms/Override/MySQL/MySqlOvDataPropertyDefinition.h \
   Rdbms/Override/MySQL/MySqlOvDataPropertyDefinitionCollection.h \
   Rdbms/Override/MySQL/MySqlOvGeometricColumn.h \
   Rdbms/Override/MySQL/MySqlOvGeometricPropertyDefinition.h \
   Rdbms/Override/MySQL/MySqlOvObjectPropertyDefinition.h \
   Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h \
   Rdbms/Override/MySQL/MySqlOvPropertyDefinition.h \
   Rdbms/Override/MySQL/MySqlOvPropertyDefinitionCollection.h \
   Rdbms/Override/MySQL/MySqlOvPropertyMappingClass.h \
   Rdbms/Override/MySQL/MySqlOvPropertyMappingConcrete.h \
   Rdbms/Override/MySQL/MySqlOvPropertyMappingDefinition.h \
   Rdbms/Override/MySQL/MySqlOvPropertyMappingRelation.h \
   Rdbms/Override/MySQL/MySqlOvPropertyMappingSingle.h \
   Rdbms/Override/MySQL/MySqlOvStorageEngineType.h \
   Rdbms/Override/MySQL/MySqlOvTable.h \
   Rdbi/connect.h \
   Rdbi/context.h \
   Rdbi/cursor.h \
   Rdbi/methods.h \
   Rdbi/proto.h \
   Rdbi/vndr_info.h

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms ]; then mkdir -p $(INSTALL_DIR)/Rdbms; fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms/Override ]; then mkdir -p $(INSTALL_DIR)/Rdbms/Override; fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms/Override/MySql ]; then mkdir -p $(INSTALL_DIR)/Rdbms/Override/MySql; fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms/Override/Odbc ]; then mkdir -p $(INSTALL_DIR)/Rdbms/Override/Odbc; fi

	@cp -u -f  Rdbms/*.h $(INSTALL_DIR)/Rdbms 
	@cp -u -f  Rdbms/Override/*.h $(INSTALL_DIR)/Rdbms/Override
	@cp -u -f  Rdbms/Override/MySQL/*.h $(INSTALL_DIR)/Rdbms/Override/MySql
	@cp -u -f  Rdbms/Override/ODBC/*.h $(INSTALL_DIR)/Rdbms/Override/Odbc

uninstall :
	@rm -drf $(INSTALL_DIR)/Rdbms

