<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema" xmlns:gml="http://www.opengis.net/gml" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fdo="http://fdo.osgeo.org/schemas" xmlns:lp="http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical" xmlns="http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical">
	<xsl:param name="providerName"/>
	<xsl:template match="lp:schema[@name='F_MetaClass' or (not($providerName='Oracle') and starts-with(@name,'abcdef1234567890'))]"/>
	<xsl:template match="lp:schema[@description='NLS Schema' and ($providerName='PostGIS' or $providerName='SqlServer' or $providerName='SQLServerSpatial')]"/>
	<xsl:template match="lp:class[@name='aCxdATA' and ($providerName='SqlServer' or $providerName='SQLServerSpatial')]"/>
	<xsl:template match="lp:class[@name='Zoning']">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:apply-templates select="lp:tableMapping|lp:identityProperties|lp:properties"/>
			<xsl:element name="uniqueConstraints">
				<xsl:element name="uniqueConstraint">
					<xsl:element name="property">
						<xsl:attribute name="xsi:type">Data</xsl:attribute>
						<xsl:attribute name="name">PolyNum</xsl:attribute>
					</xsl:element>
				</xsl:element>
			</xsl:element>
			<xsl:apply-templates select="lp:table|lp:tables"/>
		</xsl:copy>
	</xsl:template>
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
						<xsl:attribute name="name"><xsl:choose><xsl:when test="string-length(substring-after($upperName,'GEOMETRY')) = 1">Geometry'_si_1</xsl:when><xsl:otherwise><xsl:value-of select="concat($upperName,'_SI_1')"/></xsl:otherwise></xsl:choose></xsl:attribute>
						<xsl:attribute name="description"/>
						<xsl:attribute name="dataType">nvarchar</xsl:attribute>
						<xsl:attribute name="length">255</xsl:attribute>
						<xsl:attribute name="scale">0</xsl:attribute>
						<xsl:attribute name="nullable">True</xsl:attribute>
					</xsl:element>
					<xsl:element name="column">
						<xsl:attribute name="name"><xsl:choose><xsl:when test="string-length(substring-after($upperName,'GEOMETRY')) = 1">Geometry'_si_2</xsl:when><xsl:otherwise><xsl:value-of select="concat($upperName,'_SI_2')"/></xsl:otherwise></xsl:choose></xsl:attribute>
						<xsl:attribute name="description"/>
						<xsl:attribute name="dataType">nvarchar</xsl:attribute>
						<xsl:attribute name="length">255</xsl:attribute>
						<xsl:attribute name="scale">0</xsl:attribute>
						<xsl:attribute name="nullable">True</xsl:attribute>
					</xsl:element>
				</xsl:for-each>
			</xsl:if>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="lp:property[@name='GEOMETRY' and ($providerName='SqlServer' or $providerName='SQLServerSpatial')]"/>
	<xsl:template match="lp:sequence[not($providerName = 'Oracle')]|lp:sequenceSynonym[not($providerName = 'Oracle')]"/>
	<xsl:template match="lp:column[@name = 'CLASSNAME' or @name = 'SCHEMANAME']"/>
	<xsl:template match="lp:column[($providerName='SqlServer' or $providerName='SQLServerSpatial') and @name = 'GEOMETRY1' and  local-name(..) = 'table']"/>
	<xsl:template match="lp:column[@dataType and not(@name = 'CLASSNAME' or @name = 'SCHEMANAME') and not(($providerName='SqlServer' or $providerName='SQLServerSpatial') and @name = 'GEOMETRY1' and  local-name(..) = 'table')]">
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
			<xsl:attribute name="name"><xsl:call-template name="bendColumnName"><xsl:with-param name="inName" select="@name"/></xsl:call-template></xsl:attribute>
			<xsl:attribute name="description"><xsl:value-of select="@description"/></xsl:attribute>
			<xsl:choose>
				<xsl:when test="$providerName = 'Oracle'">
					<xsl:attribute name="dataType"><xsl:value-of select="@dataType"/></xsl:attribute>
					<xsl:attribute name="length"><xsl:value-of select="@length"/></xsl:attribute>
					<xsl:attribute name="scale"><xsl:value-of select="@scale"/></xsl:attribute>
					<xsl:attribute name="nullable"><xsl:value-of select="@nullable"/></xsl:attribute>
				</xsl:when>
				<xsl:when test="$propNode/@xsi:type='Data'">
					<xsl:choose>
						<xsl:when test="$propNode/@dataType = 'string'">
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:choose>
										<xsl:when test="@length &lt; 256">
											<xsl:attribute name="dataType">VARCHAR</xsl:attribute>
											<xsl:attribute name="length"><xsl:value-of select="@length"/></xsl:attribute>
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
								<xsl:when test="$providerName = 'SqlServer' or $providerName = 'SQLServerSpatial'">
									<xsl:choose>
										<xsl:when test="@length &lt; 4001">
											<xsl:attribute name="dataType">nvarchar</xsl:attribute>
											<xsl:choose>
												<xsl:when test="$colName='OV_COL_D'">
													<xsl:attribute name="length">1</xsl:attribute>
												</xsl:when>
												<xsl:otherwise>
													<xsl:attribute name="length"><xsl:value-of select="@length"/></xsl:attribute>
												</xsl:otherwise>
											</xsl:choose>
											<xsl:attribute name="scale">0</xsl:attribute>
										</xsl:when>
										<xsl:when test="@length &lt; 8001">
											<xsl:attribute name="dataType">VARCHAR</xsl:attribute>
											<xsl:attribute name="length"><xsl:value-of select="@length"/></xsl:attribute>
											<xsl:attribute name="scale">0</xsl:attribute>
										</xsl:when>
										<xsl:otherwise>
											<xsl:attribute name="dataType">TEXT</xsl:attribute>
											<xsl:attribute name="length">0</xsl:attribute>
											<xsl:attribute name="scale">0</xsl:attribute>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:when>
								<xsl:when test="$providerName = 'PostGIS'">
									<xsl:attribute name="dataType">VARCHAR</xsl:attribute>
									<xsl:attribute name="length"><xsl:value-of select="@length"/></xsl:attribute>
									<xsl:attribute name="scale">0</xsl:attribute>
								</xsl:when>
							</xsl:choose>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'decimal'">
							<xsl:choose>
								<xsl:when test="$providerName = 'PostGIS'">
									<xsl:attribute name="dataType">NUMERIC</xsl:attribute>
								</xsl:when>
								<xsl:otherwise>
									<xsl:attribute name="dataType">DECIMAL</xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:attribute name="length"><xsl:value-of select="@length"/></xsl:attribute>
							<xsl:attribute name="scale"><xsl:choose><xsl:when test="@scale=-2"><xsl:value-of select="'1'"/></xsl:when><xsl:otherwise><xsl:value-of select="@scale"/></xsl:otherwise></xsl:choose></xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'boolean'">
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">SMALLINT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<xsl:attribute name="dataType">TINYINT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SQLServerSpatial'">
									<xsl:attribute name="dataType">BIT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'PostGIS'">
									<xsl:attribute name="dataType">BOOLEAN</xsl:attribute>
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'byte'">
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">TINYINT UNSIGNED</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer' or $providerName = 'SQLServerSpatial'">
									<xsl:attribute name="dataType">TINYINT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'PostGIS'">
									<xsl:attribute name="dataType">SMALLINT</xsl:attribute>
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'datetime'">
							<xsl:choose>
								<xsl:when test="$providerName = 'PostGIS'">
									<xsl:attribute name="dataType">TIMESTAMP</xsl:attribute>
								</xsl:when>
								<xsl:otherwise>
									<xsl:attribute name="dataType">DATETIME</xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'double'">
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">DOUBLE</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer' or $providerName = 'SQLServerSpatial'">
									<xsl:attribute name="dataType">FLOAT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'PostGIS'">
									<xsl:attribute name="dataType">DOUBLE PRECISION</xsl:attribute>
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'int16'">
							<xsl:attribute name="dataType">SMALLINT</xsl:attribute>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'int32'">
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">INT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer' or $providerName = 'SQLServerSpatial' or $providerName = 'PostGIS'">
									<xsl:attribute name="dataType">INTEGER</xsl:attribute>
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'int64'">
							<xsl:attribute name="dataType">BIGINT</xsl:attribute>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'single'">
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">FLOAT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer'">
									<!-- logged as defect 772006 <xsl:attribute name="dataType">REAL</xsl:attribute>	-->
									<xsl:attribute name="dataType">FLOAT</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SQLServerSpatial' or $providerName = 'PostGIS'">
									<xsl:attribute name="dataType">REAL</xsl:attribute>
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
						<xsl:when test="$propNode/@dataType = 'blob'">
							<xsl:choose>
								<xsl:when test="$providerName = 'MySql'">
									<xsl:attribute name="dataType">LONGBLOB</xsl:attribute>
								</xsl:when>
								<xsl:when test="$providerName = 'SqlServer' or $providerName = 'SQLServerSpatial'">
									<xsl:attribute name="dataType">IMAGE</xsl:attribute>
								</xsl:when>
							</xsl:choose>
							<xsl:attribute name="length">0</xsl:attribute>
							<xsl:attribute name="scale">0</xsl:attribute>
						</xsl:when>
					</xsl:choose>
					<xsl:choose>
						<xsl:when test="($providerName='SqlServer' or $providerName = 'SQLServerSpatial') and (($propNode/@name = 'Closed' and $propNode/../../@name='AcDb3dPolyline') or ($propNode/@name='Extra'))">
							<xsl:attribute name="nullable">True</xsl:attribute>
						</xsl:when>
						<xsl:when test="$providerName='PostGIS' and $propNode/@name = 'seq' and $propNode/../../@name='view2.view_op'">
							<xsl:attribute name="nullable">True</xsl:attribute>
						</xsl:when>
						<xsl:when test="$providerName='PostGIS' and $propNode/@name = 'id' and ($propNode/../../@name='view1' or $propNode/../../@name='view2')">
							<xsl:attribute name="nullable">True</xsl:attribute>
						</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="nullable"><xsl:value-of select="@nullable"/></xsl:attribute>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$propNode/@xsi:type='Geometric'">
					<xsl:choose>
						<xsl:when test="$providerName = 'SqlServer'">
							<xsl:attribute name="dataType">IMAGE</xsl:attribute>
						</xsl:when>
						<xsl:when test="$providerName = 'MySql' or $providerName = 'SQLServerSpatial' or $providerName='PostGIS'">
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
		<xsl:attribute name="tableName"><xsl:call-template name="prefixTableName"><xsl:with-param name="inName" select="."/></xsl:call-template></xsl:attribute>
	</xsl:template>
	<xsl:template match="@columnName">
		<xsl:attribute name="columnName"><xsl:call-template name="bendColumnName"><xsl:with-param name="inName" select="."/></xsl:call-template></xsl:attribute>
	</xsl:template>
	<xsl:template match="@idColumn">
		<xsl:attribute name="idColumn"><xsl:call-template name="bendColumnName"><xsl:with-param name="inName" select="."/></xsl:call-template></xsl:attribute>
	</xsl:template>
	<xsl:template match="@name[local-name(..) = 'column']">
		<xsl:attribute name="name"><xsl:call-template name="bendColumnName"><xsl:with-param name="inName" select="."/></xsl:call-template></xsl:attribute>
	</xsl:template>
	<xsl:template match="@name[local-name(..)='table']">
		<xsl:attribute name="name"><xsl:call-template name="prefixTableName"><xsl:with-param name="inName" select="."/></xsl:call-template></xsl:attribute>
	</xsl:template>
	<xsl:template match="@nullable">
		<xsl:choose>
			<xsl:when test="($providerName = 'SqlServer' or $providerName = 'SQLServerSpatial') and ((../@name = 'Closed' and ../../../@name='AcDb3dPolyline') or (../@name='Extra'))">
				<xsl:attribute name="nullable">True</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="nullable"><xsl:value-of select="."/></xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="@scale">
		<xsl:choose>
			<xsl:when test="not($providerName = 'Oracle')and (. = -2)">
				<xsl:attribute name="scale">1</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="scale"><xsl:value-of select="."/></xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="@pkeyName"/>
	<xsl:template match="@tablespace[not($providerName = 'Oracle')]"/>
	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()"/>
		</xsl:copy>
	</xsl:template>
	<xsl:template name="bendColumnName">
		<xsl:param name="inName"/>
		<xsl:choose>
			<xsl:when test="$providerName='Oracle'">
				<xsl:choose>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX1'">ACDBHATCH_POLYLINE_AC1_ACDB1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX2'">ACDBHATCH_POLYLINE_AC1_SEQ</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA1'">ACDB3DPOLYLINE_ACDBVE1_ACDB1</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA2'">ACDB3DPOLYLINE_ACDBVE1_SEQ</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY_1'">ELECTRICDEVICE_MAINT_1_ELEC1</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY_2'">ELECTRICDEVICE_MAINT_1_DATE1</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE_F1'">EMPLOYEE_A_ADDRESS_EMPLOYEE1</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE_L1'">EMPLOYEE_A_ADDRESS_EMPLOYEE2</xsl:when>
					<xsl:when test="$inName='MAINTENANCE_HISTORY_DESCRIPTI1'">MAINTENANCE_HISTORY_DESCRIP1</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE_HD_OVCL1'">OVCLASSC111_OPC_TABLE1_OVCL1</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE_HD_OPID1'">OVCLASSC111_OPC_TABLE1_OPID1</xsl:when>
					<xsl:when test="$inName='OVCLASSH_FTABLED_OVCLASSH_FEA1'">OVCLASSH_FTABLED_OVCLASSH_F1</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA_SEQ'">POLYLINE_ACDBVERTEXDA1_SEQ</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA_POLYL1'">POLYLINE_ACDBVERTEXDA1_POLY1</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345671'">ABCDEF1234567890ABCDEF123451</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345672' and ../@tableName='ABCDEF1234567890ABCDEF12345675'">ABCDEF1234567890ABCDE4_ABCD1</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345672' and ../@tableName='ABCDEF1234567890ABCDEF12345673'">ABCDEF1234567890ABCDE2_ABCD1</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345672'">ABCDEF1234567890ABCDE1_ABCD1</xsl:when>
					<!--<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345672'"><xsl:value-of select="../@tableName"/></xsl:when>-->
					<xsl:when test="$inName='GBCDEF1234567890ABCDEF12345671'">GBCDEF1234567890ABCDEF123451</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$inName"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$providerName='SqlServer'">
				<xsl:choose>
					<xsl:when test="local-name() = 'idColumn'">
						<xsl:variable name="propName" select="../@name"/>
						<xsl:choose>
							<xsl:when test="$inName='DATE1'">date1</xsl:when>
							<xsl:when test="ancestor::lp:class/lp:properties/lp:property[@name=$propName]/lp:idProperty/lp:property">
								<xsl:call-template name="tolower">
									<xsl:with-param name="inString" select="ancestor::lp:class/lp:properties/lp:property[@name=$propName]/lp:idProperty/lp:property/@name"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:when>
					<xsl:when test="$inName='ACDBENTITY_FEATID'">acdbentity_featid</xsl:when>
					<xsl:when test="$inName='ENTITY_FEATID'">entity_featid</xsl:when>
					<xsl:when test="$inName='ACDBPOLYLINE_FEATID'">acdbpolyline_featid</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_FEATID'">acdbhatch_polyline_featid</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_ACDB1'">acdb3dpolyline_acdbvertexdata_acdb3dpolyline_featid</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_SEQ'">acdb3dpolyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_ACDB1'">acdbhatch_polyline_acdbvertexdata_acdbhatch_polyline_featid</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_SEQ'">acdbhatch_polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_POLY1'">polyline_acdbvertexdata_polyline_featid</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_SEQ'">polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='MAINTENANCE_HISTORY_DESCRIP1'">maintenance history description</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_FEATID'">electricdevice_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_ENTITY_FEATID'">electricdevice_entity_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_ELEC1'">electricdevice_maint history_electricdevice_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_DATE1'">electricdevice_maint history_date1</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE1'">employee_'address_employee_first name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE2'">employee_'address_employee_last name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_FIRST_NAME'">employee_first name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_LAST_NAME'">employee_last name</xsl:when>
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
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OVCL1'">ovclassc111_opc_table_hd_ovclassc111_featid</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OPID1'">ovclassc111_opc_table_hd_opid1</xsl:when>
					<xsl:when test="$inName='OVCLASSH_FTABLED_OVCLASSH_F1'">ovclassh_ftabled_ovclassh_featid</xsl:when>
					<xsl:when test="$inName='CLASSNAME1'">classname1</xsl:when>
					<xsl:when test="$inName='DATE1'">date1</xsl:when>
					<xsl:when test="$inName='GEOMETRY'">geometry</xsl:when>
					<xsl:when test="$inName='NUMBER1'">number1</xsl:when>
					<xsl:when test="$inName='VALUE1'">value</xsl:when>
					<xsl:when test="$inName='n/a'">n/a</xsl:when>
					<xsl:when test="$inName='CLASSID'">classid</xsl:when>
					<xsl:when test="$inName='CLASSNAME'">classname</xsl:when>
					<xsl:when test="$inName='FEATID' and ancestor::lp:class[@name='Parcel' or @name='Zoning']">featid</xsl:when>
					<xsl:when test="$inName='SCHEMANAME'">schemaname</xsl:when>
					<xsl:when test="$inName='REVISIONNUMBER'">revisionnumber</xsl:when>
					<xsl:when test="local-name(..) = 'property'">
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="../@name"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when test="ancestor::lp:class/lp:properties/lp:property[@columnName=$inName]">
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="ancestor::lp:class/lp:properties/lp:property[@columnName=$inName]/@name"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$providerName='SQLServerSpatial'">
				<xsl:choose>
					<xsl:when test="local-name() = 'idColumn'">
						<xsl:variable name="propName" select="../@name"/>
						<xsl:choose>
							<xsl:when test="$inName='DATE1'">date</xsl:when>
							<xsl:when test="ancestor::lp:class/lp:properties/lp:property[@name=$propName]/lp:idProperty/lp:property">
								<xsl:call-template name="tolower">
									<xsl:with-param name="inString" select="ancestor::lp:class/lp:properties/lp:property[@name=$propName]/lp:idProperty/lp:property/@name"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:when>
					<xsl:when test="$inName='ACDBENTITY_FEATID'">acdbentity_featid</xsl:when>
					<xsl:when test="$inName='ENTITY_FEATID'">entity_featid</xsl:when>
					<xsl:when test="$inName='ACDBPOLYLINE_FEATID'">acdbpolyline_featid</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_FEATID'">acdbhatch_polyline_featid</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_ACDB1'">acdb3dpolyline_acdbvertexdata_acdb3dpolyline_featid</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_SEQ'">acdb3dpolyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_ACDB1'">acdbhatch_polyline_acdbvertexdata_acdbhatch_polyline_featid</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_SEQ'">acdbhatch_polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_POLY1'">polyline_acdbvertexdata_polyline_featid</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_SEQ'">polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='MAINTENANCE_HISTORY_DESCRIP1'">maintenance history description</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_FEATID'">electricdevice_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_ENTITY_FEATID'">electricdevice_entity_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_ELEC1'">electricdevice_maint history_electricdevice_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_DATE1'">electricdevice_maint history_date</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE1'">employee_'address_employee_first name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE2'">employee_'address_employee_last name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_FIRST_NAME'">employee_first name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_LAST_NAME'">employee_last name</xsl:when>
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
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OVCL1'">ovclassc111_opc_table_hd_ovclassc111_featid</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OPID1'">ovclassc111_opc_table_hd_opid1</xsl:when>
					<xsl:when test="$inName='OVCLASSH_FTABLED_OVCLASSH_F1'">ovclassh_ftabled_ovclassh_featid</xsl:when>
					<xsl:when test="$inName='CLASSNAME1'">classname1</xsl:when>
					<xsl:when test="$inName='DATE1'">date</xsl:when>
					<xsl:when test="$inName='GEOMETRY'">geometry</xsl:when>
					<xsl:when test="$inName='NUMBER1'">number</xsl:when>
					<xsl:when test="$inName='VALUE1'">value</xsl:when>
					<xsl:when test="$inName='n/a'">n/a</xsl:when>
					<xsl:when test="$inName='CLASSID'">classid</xsl:when>
					<xsl:when test="$inName='CLASSNAME'">classname</xsl:when>
					<xsl:when test="$inName='FEATID' and ancestor::lp:class[@name='Parcel' or @name='Zoning']">featid</xsl:when>
					<xsl:when test="$inName='SCHEMANAME'">schemaname</xsl:when>
					<xsl:when test="$inName='REVISIONNUMBER'">revisionnumber</xsl:when>
					<xsl:when test="local-name(..) = 'property'">
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="../@name"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when test="ancestor::lp:class/lp:properties/lp:property[@columnName=$inName]">
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="ancestor::lp:class/lp:properties/lp:property[@columnName=$inName]/@name"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$providerName='PostGIS'">
				<xsl:choose>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_ACDB1'">acdb3dpolyline_acdbvertexdata_acdb3dpolyline_featid</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_SEQ'">acdb3dpolyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_ACDB1'">acdbhatch_polyline_acdbvertexdata_acdbhatch_polyline_featid</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_SEQ'">acdbhatch_polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_POLY1'">polyline_acdbvertexdata_polyline_featid</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_SEQ'">polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='PLOT_STYLE'">plot style</xsl:when>
					<xsl:when test="$inName='MAINTENANCE_HISTORY_DESCRIP1'">maintenance history description</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_ELEC1'">electricdevice_maint history_electricdevice_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_DATE1'">electricdevice_maint history_date1</xsl:when>
					<xsl:when test="$inName='IT_M_'">it'm #</xsl:when>
					<xsl:when test="$inName='PART_'">part #</xsl:when>
					<xsl:when test="$inName='WORK_DESCRIPTION'">work description</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE1'">employee_'address_employee_first name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE2'">employee_'address_employee_last name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_FIRST_NAME'">employee_first name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_LAST_NAME'">employee_last name</xsl:when>
					<xsl:when test="$inName='CREDIT_RATING'">credit rating</xsl:when>
					<xsl:when test="$inName='FIRST_NAME'">first name</xsl:when>
					<xsl:when test="$inName='LAST_NAME'">last name</xsl:when>
					<xsl:when test="$inName='A_ROOMS'"># rooms</xsl:when>
					<xsl:when test="$inName='A_OCCUPIED'">% occupied</xsl:when>
					<xsl:when test="$inName='A_OCCUPIED1'"># occupied</xsl:when>
					<xsl:when test="$inName='PAV_D'">pav'd</xsl:when>
					<xsl:when test="$inName='GEOMETRY_'">geometry'</xsl:when>
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
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OVCL1'">ovclassc111_opc_table_hd_ovclassc111_featid</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OPID1'">ovclassc111_opc_table_hd_opid1</xsl:when>
					<xsl:when test="$inName='OVCLASSH_FTABLED_OVCLASSH_F1'">ovclassh_ftabled_ovclassh_featid</xsl:when>
					<xsl:when test="$inName='OV_COL_B'">ov_col_B</xsl:when>
					<xsl:when test="$inName='OV_COL_D'">ov_col_D</xsl:when>
					<xsl:when test="$inName='OV_COL_F'">ov_col_F</xsl:when>
					<xsl:when test="$inName='OV_COL_H'">ov_col_H</xsl:when>
					<xsl:when test="$inName='OV_COL_J'">ov_col_J</xsl:when>
					<xsl:when test="$inName='OV_GEOMCOL_A'">ov_geomcol_A</xsl:when>
					<xsl:when test="$inName='OV_GEOMCOL_F'">ov_geomcol_F</xsl:when>
					<xsl:when test="$inName='PARENTID'">ParentId</xsl:when>
					<xsl:when test="$inName='NESTED_COL_A'">NESTED_COL_A</xsl:when>
					<xsl:when test="$inName='OP_PROP_A'">OP_PROP_A</xsl:when>
					<xsl:when test="$inName='GEOMI'">GeomI</xsl:when>
					<xsl:when test="$inName='GEOMK'">GeomK</xsl:when>
					<xsl:when test="$inName='VALUE1'">value</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_ACDB1'">acdb3dpolyline_acdbvertexdata_acdb3dpolyline_featid</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1_SEQ'">acdb3dpolyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_ACDB1'">acdbhatch_polyline_acdbvertexdata_acdbhatch_polyline_featid</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1_SEQ'">acdbhatch_polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_POLY1'">polyline_acdbvertexdata_polyline_featid</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1_SEQ'">polyline_acdbvertexdata_seq</xsl:when>
					<xsl:when test="$inName='MAINTENANCE_HISTORY_DESCRIP1'">maintenance_history_description</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_ELEC1'">electricdevice_maint_history_electricdevice_featid</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1_DATE1'">electricdevice_maint_history_date1</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE1'">employee_a_address_employee_first_name</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_EMPLOYEE2'">employee_a_address_employee_last_name</xsl:when>
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
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OVCL1'">ovclassc111_opc_table_hd_ovclassc111_featid</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1_OPID1'">ovclassc111_opc_table_hd_opid1</xsl:when>
					<xsl:when test="$inName='OVCLASSH_FTABLED_OVCLASSH_F1'">ovclassh_ftabled_ovclassh_featid</xsl:when>
					<xsl:when test="$inName='VALUE1'">value</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="prefixTableName">
		<xsl:param name="inName"/>
		<xsl:variable name="prefix">
			<xsl:choose>
				<xsl:when test="$providerName='SqlServer' or $providerName='SQLServerSpatial'">dbo.</xsl:when>
				<xsl:when test="$providerName='SqlServer' or $providerName='PostGIS'">public.</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="bentName">
			<xsl:call-template name="bendTableName">
				<xsl:with-param name="inName" select="$inName"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:value-of select="concat($prefix,$bentName)"/>
	</xsl:template>
	<xsl:template name="bendTableName">
		<xsl:param name="inName"/>
		<xsl:choose>
			<xsl:when test="$providerName='Oracle'">
				<xsl:choose>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE_HD'">OVCLASSC111_OPC_TABLE1</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE_HD_OPC_1'">OVCLASSC111_OPC_TABLE2</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPS_TABLE_HA'">OVCLASSC111_OPS_TABLE1</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA'">ACDB3DPOLYLINE_ACDBVE1</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA1'">ACDB3DPOLYLINE_ACDBVE2</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVERTEXDATA2'">ACDB3DPOLYLINE_ACDBVE3</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX1'">ACDBHATCH_POLYLINE_AC1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX2'">ACDBHATCH_POLYLINE_AC2</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACDBVERTEX3'">ACDBHATCH_POLYLINE_AC3</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_ACXDATA'">ACDBHATCH_POLYLINE_AC4</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXCOORDINATEVALUE'">ACDBVERTEXCOORDINATEV1</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVERTEXCOOR1'">ACDBVERTEXDATA_ACDBVE1</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVERTEXCOOR2'">ACDBVERTEXDATA_ACDBVE2</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_ENTITY_ACXDATA'">ELECTRICDEVICE_ENTITY1</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY'">ELECTRICDEVICE_MAINT_1</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY_1'">ELECTRICDEVICE_MAINT_2</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_STREET'">EMPLOYEE_A_ADDRESS_ST1</xsl:when>
					<xsl:when test="$inName='MAINT_HISTORY_MAINT_HISTORY_I1'">MAINT_HISTORY_MAINT_H1</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA'">POLYLINE_ACDBVERTEXDA1</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA_ACDBV1'">POLYLINE_ACDBVERTEXDA2</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDATA_ACDBV2'">POLYLINE_ACDBVERTEXDA3</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345671'">ABCDEF1234567890ABCDE1</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345672'">ABCDEF1234567890ABCDE2</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345673'">ABCDEF1234567890ABCDE3</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345674'">ABCDEF1234567890ABCDE4</xsl:when>
					<xsl:when test="$inName='ABCDEF1234567890ABCDEF12345675'">ABCDEF1234567890ABCDE5</xsl:when>
					<xsl:when test="$inName='BBCDEF1234567890ABCDEF12345671'">BBCDEF1234567890ABCDE1</xsl:when>
					<xsl:when test="$inName='OACDEF1234567890ABCDEF12345671'">OACDEF1234567890ABCDE1</xsl:when>
					<xsl:when test="$inName='OBCDEF1234567890ABCDEF12345671'">OBCDEF1234567890ABCDE1</xsl:when>
					<xsl:when test="$inName='OBCDEF1234567890ABCDEF12345672'">OBCDEF1234567890ABCDE2</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$inName"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$providerName='SqlServer' or $providerName='SQLServerSpatial'">
				<xsl:choose>
					<xsl:when test="$inName='ACDBENTITY_ACXDATA'">acdbentity_acxdata</xsl:when>
					<xsl:when test="$inName='ENTITY_ACXDATA'">entity_acxdata</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1'">acdb3dpolyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE2'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE3'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE'">acdbhatch_polyline</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1'">acdbhatch_polyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC2'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC4'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC3'">acdbhatch_polyline_acxdata</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXCOORDINATEV1'">acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVE1'">acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVE2'">acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='A_ADDRESS_STREET'">'address_street</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_ENTITY'">electricdevice_entity</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_ENTITY1'">electricdevice_entity_acxdata</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1'">electricdevice_maint history</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_2'">electricdevice_maint history_maint history item</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_HISTORY'">electricdevice_maint history</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS'">employee_'address</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_ST1'">employee_'address_street</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1'">polyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA2'">polyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA3'">polyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='PARCEL_PERSON'">parcel_person</xsl:when>
					<xsl:when test="$inName='MAINT_HISTORY_MAINT_H1'">maint history_maint history item</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1'">ovclassc111_opc_table_hd</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE2'">ovclassc111_opc_table_hd_opc_table_hda</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPS_TABLE1'">ovclassc111_ops_table_ha</xsl:when>
					<xsl:when test="$inName='F_CLASSDEFINITION'">f_classdefinition</xsl:when>
					<xsl:when test="ancestor::lp:class">
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="ancestor::lp:class/@name"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$providerName='PostGIS'">
				<xsl:choose>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1'">acdb3dpolyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE2'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE3'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1'">acdbhatch_polyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC2'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatev1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC4'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatev2</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC3'">acdbhatch_polyline_acxdata</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXCOORDINATEV1'">acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVE1'">acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVE2'">acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_ENTITY1'">electricdevice_entity_acxdata</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1'">electricdevice_maint history</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_2'">electricdevice_maint history_maint history item</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS'">employee_'address</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_ST1'">employee_'address_street</xsl:when>
					<xsl:when test="$inName='CUSTOMER_BUSINESS'">customer - business</xsl:when>
					<xsl:when test="$inName='CUSTOMER_RESIDENTIAL'">customer - residential</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1'">polyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA2'">polyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA3'">polyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='MAINT_HISTORY'">maint history</xsl:when>
					<xsl:when test="$inName='MAINT_HISTORY_ITEM'">maint history item</xsl:when>
					<xsl:when test="$inName='MAINT_HISTORY_MAINT_H1'">maint history_maint history item</xsl:when>
					<xsl:when test="$inName='WORK_ITEM'">work item</xsl:when>
					<xsl:when test="$inName='BUILD_G'">build'g</xsl:when>
					<xsl:when test="$inName='A1_8_SCHOOL'">1-8 school</xsl:when>
					<xsl:when test="$inName='COGO_POINT'">cogo point</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1'">ovclassc111_opc_table_hd</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE2'">ovclassc111_opc_table_hd_opc_table_hda</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPS_TABLE1'">ovclassc111_ops_table_ha</xsl:when>
					<xsl:when test="$inName='A_ADDRESS'">'address</xsl:when>
					<xsl:when test="$inName='A_ADDRESS_STREET'">'address_street</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE1'">acdb3dpolyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE2'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDB3DPOLYLINE_ACDBVE3'">acdb3dpolyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC1'">acdbhatch_polyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC2'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC4'">acdbhatch_polyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='ACDBHATCH_POLYLINE_AC3'">acdbhatch_polyline_acxdata</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXCOORDINATEV1'">acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVE1'">acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='ACDBVERTEXDATA_ACDBVE2'">acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_ENTITY1'">electricdevice_entity_acxdata</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_1'">electricdevice_maint_history</xsl:when>
					<xsl:when test="$inName='ELECTRICDEVICE_MAINT_2'">electricdevice_maint_history_maint_history_item</xsl:when>
					<xsl:when test="$inName='EMPLOYEE_A_ADDRESS_ST1'">employee_a_address_street</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA1'">polyline_acdbvertexdata</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA2'">polyline_acdbvertexdata_acdbvertexcoordinatevalue</xsl:when>
					<xsl:when test="$inName='POLYLINE_ACDBVERTEXDA3'">polyline_acdbvertexdata_acdbvertexcoordinatevalue1</xsl:when>
					<xsl:when test="$inName='MAINT_HISTORY_MAINT_H1'">maint_history_maint_history_item</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE1'">ovclassc111_opc_table_hd</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPC_TABLE2'">ovclassc111_opc_table_hd_opc_table_hda</xsl:when>
					<xsl:when test="$inName='OVCLASSC111_OPS_TABLE1'">ovclassc111_ops_table_ha</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
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
