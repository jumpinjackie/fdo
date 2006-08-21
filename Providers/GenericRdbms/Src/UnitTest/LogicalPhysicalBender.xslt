<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" 
xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
xmlns:xsi="http://www.w3.org/2001/XMLSchema" 
xmlns:gml="http://www.opengis.net/gml" 
xmlns:xs="http://www.w3.org/2001/XMLSchema" 
xmlns:fdo="http://fdo.osgeo.org/schemas"
xmlns:lp="http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical"
xmlns="http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical"
>

<xsl:param name="providerName"/>
<xsl:template match="lp:schema[@name='F_MetaClass' or starts-with(@name,'abcdef1234567890')]"/>
<xsl:template match="lp:class[@name='aCxdATA' and $providerName='SqlServer']"/>
<xsl:template match="lp:table">
  <xsl:copy>
    <xsl:apply-templates select="@*|node()"/>
	<xsl:if test="$providerName='SqlServer'">
		<xsl:for-each select="../lp:properties/lp:property[@xsi:type='Geometric' and not(@name='Bounds' or @columnName='GEOMETRY1')]">
			<xsl:variable name="upperName">
				<xsl:call-template name="toupper">
					<xsl:with-param name="inString" select="@name"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:element name="column">
				<xsl:attribute name="name">
					<xsl:value-of select="concat($upperName,'_SI_1')"/>
				</xsl:attribute>
				<xsl:attribute name="description"/>
				<xsl:attribute name="dataType">VARCHAR</xsl:attribute>
				<xsl:attribute name="length">255</xsl:attribute>
				<xsl:attribute name="scale">0</xsl:attribute>
				<xsl:attribute name="nullable">True</xsl:attribute>
			</xsl:element>
			<xsl:element name="column">
				<xsl:attribute name="name">
					<xsl:value-of select="concat($upperName,'_SI_2')"/>
				</xsl:attribute>
				<xsl:attribute name="description"/>
				<xsl:attribute name="dataType">VARCHAR</xsl:attribute>
				<xsl:attribute name="length">255</xsl:attribute>
				<xsl:attribute name="scale">0</xsl:attribute>
				<xsl:attribute name="nullable">True</xsl:attribute>
			</xsl:element>
		</xsl:for-each>
	</xsl:if>
  </xsl:copy>
</xsl:template>
<xsl:template match="lp:property[@name='GEOMETRY' and $providerName='SqlServer']"/>
<xsl:template match="lp:sequence|lp:sequenceSynonym"/>
<xsl:template match="lp:column[@name = 'CLASSNAME' or @name = 'SCHEMANAME']"/>
<xsl:template match="lp:column[$providerName  = 'SqlServer' and @name = 'GEOMETRY1' and  local-name(..) = 'table']"/>
<xsl:template match="lp:column[@dataType and not($providerName='Oracle' or @name = 'CLASSNAME' or @name = 'SCHEMANAME') and not($providerName  = 'SqlServer' and @name = 'GEOMETRY1' and  local-name(..) = 'table')]">
	<xsl:variable name="colName" select="@name"/>
	<xsl:choose>
		<xsl:when test="local-name(..) = 'property'">
			<xsl:call-template name="doColumn">
				<xsl:with-param name="propNode" select=".."/>
			</xsl:call-template>
		</xsl:when>
		<xsl:when test="../../lp:properties/lp:property[@columnName=$colName]">
			<xsl:call-template name="doColumn">
				<xsl:with-param name="propNode" select="../../lp:properties/lp:property[@columnName=$colName]"/>
			</xsl:call-template>
		</xsl:when>
	</xsl:choose>
</xsl:template>
<xsl:template name="doColumn">
	<xsl:param name="propNode"/>
	<xsl:variable name="colName" select="@name"/>
    <xsl:element name="column">
		<xsl:attribute name="name">
			<xsl:call-template name="bendColumnName">
				<xsl:with-param name="inName" select="@name"/>
			</xsl:call-template>
		</xsl:attribute>
		<xsl:attribute name="description">
			<xsl:value-of select="@description"/>
		</xsl:attribute>
		<xsl:choose>
			<xsl:when test="$propNode/@xsi:type='Data'">
					<xsl:choose>
						<xsl:when test="$propNode/@dataType = 'string'" >
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:choose>
										<xsl:when test="@length &lt; 256">
											<xsl:attribute name="dataType">VARCHAR</xsl:attribute>	
											<xsl:attribute name="length">
												<xsl:value-of select="@length"/>
											</xsl:attribute>				
											<xsl:attribute name="scale">0</xsl:attribute>	
										</xsl:when>
										<xsl:when test="@length &lt; 65536">
											<xsl:attribute name="dataType">TEXT</xsl:attribute>	
											<xsl:attribute name="length">65535</xsl:attribute>	
											<xsl:attribute name="scale">0</xsl:attribute>	
										</xsl:when>
										<xsl:when test="@length &lt; 16777216">
											<xsl:attribute name="dataType">MEDIUMTEXT</xsl:attribute>	
											<xsl:attribute name="length">1677715</xsl:attribute>	
											<xsl:attribute name="scale">0</xsl:attribute>	
										</xsl:when>
										<xsl:otherwise>
											<xsl:attribute name="dataType">LONGTEXT</xsl:attribute>
											<xsl:attribute name="length">2147483647</xsl:attribute>	
											<xsl:attribute name="scale">0</xsl:attribute>	
										</xsl:otherwise>
									</xsl:choose>
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:choose>
										<xsl:when test="@length &lt; 8001">
											<xsl:attribute name="dataType">VARCHAR</xsl:attribute>	
											<xsl:choose>
												<xsl:when test="$colName='OV_COL_D'">
													<xsl:attribute name="length">1</xsl:attribute>				
												</xsl:when>
												<xsl:otherwise>
													<xsl:attribute name="length">
														<xsl:value-of select="@length"/>
													</xsl:attribute>				
												</xsl:otherwise>
											</xsl:choose>
											<xsl:attribute name="scale">0</xsl:attribute>	
										</xsl:when>
										<xsl:otherwise>
											<xsl:attribute name="dataType">TEXT</xsl:attribute>
											<xsl:attribute name="length">0</xsl:attribute>	
											<xsl:attribute name="scale">0</xsl:attribute>	
										</xsl:otherwise>
									</xsl:choose>
								</xsl:when>
							</xsl:choose>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'decimal'" >
							<xsl:attribute name="dataType">DECIMAL</xsl:attribute>	
							<xsl:attribute name="length">
								<xsl:value-of select="@length"/>
							</xsl:attribute>				
							<xsl:attribute name="scale">
								<xsl:value-of select="@scale"/>
							</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'boolean'" >
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">SMALLINT</xsl:attribute>	
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:attribute name="dataType">TINYINT</xsl:attribute>	
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'byte'" >
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">TINYINT UNSIGNED</xsl:attribute>	
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:attribute name="dataType">TINYINT</xsl:attribute>	
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'datetime'" >
							<xsl:attribute name="dataType">DATETIME</xsl:attribute>	
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'double'" >
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">DOUBLE</xsl:attribute>	
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:attribute name="dataType">FLOAT</xsl:attribute>	
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'int16'" >
							<xsl:attribute name="dataType">SMALLINT</xsl:attribute>	
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'int32'" >
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">INT</xsl:attribute>	
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:attribute name="dataType">INTEGER</xsl:attribute>	
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'int64'" >
							<xsl:attribute name="dataType">BIGINT</xsl:attribute>	
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'single'" >
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">DOUBLE</xsl:attribute>	
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:attribute name="dataType">REAL</xsl:attribute>	
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'blob'" >
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">LONGBLOB</xsl:attribute>	
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:attribute name="dataType">IMAGE</xsl:attribute>	
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>	
							<xsl:attribute name="scale">0</xsl:attribute>	
						</xsl:when>
					</xsl:choose>
					<xsl:choose>
						<xsl:when test="$providerName='SqlServer' and (($propNode/@name = 'Closed' and $propNode/../../@name='AcDb3dPolyline') or ($propNode/@name='Extra'))">
							<xsl:attribute name="nullable">True</xsl:attribute>
						</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="nullable">
								<xsl:value-of select="@nullable"/>
							</xsl:attribute>
						</xsl:otherwise>
				   </xsl:choose>
			</xsl:when>
			<xsl:when test="$propNode/@xsi:type='Geometric'">
				<xsl:choose>
					<xsl:when test="$providerName = 'SqlServer'">
						<xsl:attribute name="dataType">IMAGE</xsl:attribute>	
					</xsl:when>
					<xsl:when test="$providerName = 'MySql'">
						<xsl:attribute name="dataType">geometry</xsl:attribute>	
					</xsl:when>
				</xsl:choose>
				<xsl:attribute name="length">0</xsl:attribute>	
				<xsl:attribute name="scale">0</xsl:attribute>	
				<xsl:attribute name="nullable">True</xsl:attribute>	
			</xsl:when>
		</xsl:choose>
	</xsl:element>
</xsl:template>
<xsl:template match="@tableName">
	<xsl:attribute name="tableName">
		<xsl:call-template name="bendTableName">
			<xsl:with-param name="inName" select="."/>
		</xsl:call-template>
	</xsl:attribute>
</xsl:template>
<xsl:template match="@columnName">
	<xsl:attribute name="columnName">
		<xsl:call-template name="bendColumnName">
			<xsl:with-param name="inName" select="."/>
		</xsl:call-template>
	</xsl:attribute>
</xsl:template>
<xsl:template match="@idColumn">
	<xsl:attribute name="idColumn">
		<xsl:call-template name="tolower">
			<xsl:with-param name="inString" select="."/>
		</xsl:call-template>
	</xsl:attribute>
</xsl:template>
<xsl:template match="@name[local-name(..) = 'column']">
	<xsl:attribute name="name">
		<xsl:call-template name="bendColumnName">
			<xsl:with-param name="inName" select="."/>
		</xsl:call-template>
	</xsl:attribute>
</xsl:template>
<xsl:template match="@name[local-name(..)='table']">
	<xsl:attribute name="name">
		<xsl:call-template name="bendTableName">
			<xsl:with-param name="inName" select="."/>
		</xsl:call-template>
	</xsl:attribute>
</xsl:template>
<xsl:template match="@nullable">
	<xsl:choose>
		<xsl:when test="$providerName = 'SqlServer' and ((../@name = 'Closed' and ../../../@name='AcDb3dPolyline') or (../@name='Extra'))">
			<xsl:attribute name="nullable">True</xsl:attribute>
		</xsl:when>
		<xsl:otherwise>
			<xsl:attribute name="nullable">
				<xsl:value-of select="."/>
			</xsl:attribute>
		</xsl:otherwise>
   </xsl:choose>
</xsl:template>
<xsl:template match="@pkeyName"/>
<xsl:template match="@tablespace"/>
<xsl:template match="@*|node()">
  <xsl:copy>
    <xsl:apply-templates select="@*|node()"/>
  </xsl:copy>
  </xsl:template>
  <xsl:template name="bendColumnName">
	<xsl:param name="inName"/>
	<xsl:choose>
		<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA1'">acdb3dpolyline_acdbvertexdata_acdb3dpolyline_featid</xsl:when>
		<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA2'">acdb3dpolyline_acdbvertexdata_seq</xsl:when>
		<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX1'">acdbhatch_polyline_acdbvertexdata_acdbhatch_polyline_featid</xsl:when>
		<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX2'">acdbhatch_polyline_acdbvertexdata_seq</xsl:when>
		<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA_POLYL1'">polyline_acdbvertexdata_polyline_featid</xsl:when>
		<xsl:when test="$inName='MAINTENANCE_HISTORY_DESCRIPTI1'">maintenance_history_description</xsl:when>
		<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY_1'">electricdevice_maint_history_electricdevice_featid</xsl:when>
		<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY_2'">electricdevice_maint_history_date1</xsl:when>
		<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE_F1'">employee_a_address_employee_first_name</xsl:when>
		<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE_L1'">employee_a_address_employee_last_name</xsl:when>
		<xsl:when test="$inName='REFIXA_PREFIXA_PREFIXA_OPA'">prefixa_prefixa_prefixa_opa</xsl:when>
		<xsl:when test="$inName='REFIXA_PREFIXA_PREFIXA_OPB'">prefixa_prefixa_prefixa_opb</xsl:when>
		<xsl:when test="$inName='BJECTA_PREFIXA_PREFIXA_OPA'">objecta_prefixa_prefixa_opa</xsl:when>
		<xsl:when test="$inName='BJECTA_PREFIXA_PREFIXA_OPB'">objecta_prefixa_prefixa_opb</xsl:when>
		<xsl:when test="$inName='A012345_PREFIXA_PREFIXA_OPA'">testverylongprefix012345_prefixa_prefixa_opa</xsl:when>
		<xsl:when test="$inName='A012345_PREFIXA_PREFIXA_OPB'">testverylongprefix012345_prefixa_prefixa_opb</xsl:when>
		<xsl:when test="$inName='NGPREFIX012345_PREFIXA_OPA'">testverylongprefix012345_prefixa_opa</xsl:when>
		<xsl:when test="$inName='NGPREFIX012345_PREFIXA_OPB'">testverylongprefix012345_prefixa_opb</xsl:when>
		<xsl:when test="$inName='STVERYLONGPREFIX012345_OPA'">testverylongprefix012345_opa</xsl:when>
		<xsl:when test="$inName='STVERYLONGPREFIX012345_OPB'">testverylongprefix012345_opb</xsl:when>
		<xsl:when test="$inName='OVCLASSH_FTABLED_OVCLASSH_FEA1'">ovclassh_ftabled_ovclassh_featid</xsl:when>
		<xsl:when test="$inName='OVCLASSC111_OPC_TABLE_HD_OVCL'">ovclassc111_opc_table_hd_ovclassc111_featid</xsl:when>
		<xsl:when test="$inName='OVCLASSC111_OPC_TABLE_HD_OPID'">ovclassc111_opc_table_hd_opid1</xsl:when>
		<xsl:when test="$inName='GEOMETRY1' and $providerName='SqlServer'">geometry</xsl:when>
		<xsl:otherwise>
			<xsl:call-template name="tolower">
				<xsl:with-param name="inString" select="$inName"/>
			</xsl:call-template>
		</xsl:otherwise>
	</xsl:choose>
  </xsl:template>
  <xsl:template name="bendTableName">
	<xsl:param name="inName"/>
	<xsl:choose>
		<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA1'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
		<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA2'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
		<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX1'">acdbhatch_polyline_acdbvertexdata</xsl:when>
		<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX2'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
		<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX3'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
		<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVERTEXCOOR1'">acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
		<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVERTEXCOOR2'">acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
		<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA_ACDBV1'">polyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
		<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA_ACDBV2'">polyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
		<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY_1'">electricdevice_maint_history_maint_history_item</xsl:when>
		<xsl:when test="$inName='MAINT_HISTORY_MAINT_HISTORY_I1'">maint_history_maint_history_item</xsl:when>
		<xsl:when test="$inName='OVCLASSC111_OPC_TABLE_HD_OPC_1'">ovclassc111_opc_table_hd_opc_table_hda</xsl:when>
		<xsl:otherwise>
			<xsl:call-template name="tolower">
				<xsl:with-param name="inString" select="$inName"/>
			</xsl:call-template>
		</xsl:otherwise>
	</xsl:choose>
  </xsl:template>
  <xsl:template name="tolower">
	  <xsl:param name="inString"/>
	  <xsl:value-of select="translate($inString,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')"/>
  </xsl:template>
  <xsl:template name="toupper">
	  <xsl:param name="inString"/>
	  <xsl:value-of select="translate($inString,'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
  </xsl:template>
</xsl:stylesheet>
