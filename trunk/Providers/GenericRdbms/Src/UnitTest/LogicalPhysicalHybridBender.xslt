<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema" xmlns:gml="http://www.opengis.net/gml" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fdo="http://fdo.osgeo.org/schemas" xmlns:lp="http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical" xmlns="http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical">
	<xsl:param name="providerName"/>
	<xsl:param name="hybridLevel"/>
	<xsl:template match="@name[local-name(..) = 'property' and $hybridLevel &gt; 0]">
		<xsl:attribute name="name">
			<xsl:call-template name="bendPropName"><xsl:with-param name="inName" select="."/></xsl:call-template>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@columnName[local-name(..) = 'property' and $hybridLevel &gt; 0]">
		<xsl:attribute name="columnName">
			<xsl:call-template name="bendPropName"><xsl:with-param name="inName" select="../@name"/></xsl:call-template>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@dataType[local-name(..) = 'property' and $hybridLevel &gt; 0]">
		<xsl:attribute name="dataType">
			<xsl:choose>
				<xsl:when test="$providerName = 'Oracle'">
					<xsl:value-of select="."/>
				</xsl:when>
				<xsl:when test="$providerName = 'MySql'">
					<xsl:choose>
						<xsl:when test=". = 'decimal' and ../@scale = 0">
							<xsl:choose>
								<xsl:when test="../@precision=20">
									<xsl:value-of select="'int64'"/>
								</xsl:when>
								<xsl:when test="../@precision=10">
									<xsl:value-of select="'int32'"/>
								</xsl:when>
								<xsl:when test="../@precision=5">
									<xsl:value-of select="'int16'"/>
								</xsl:when>
								<xsl:when test="../@precision=3">
									<xsl:value-of select="'byte'"/>
								</xsl:when>
								<xsl:when test="../@precision=1">
									<xsl:value-of select="'int16'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:choose>
								<xsl:when test="../@name='VOLUME'">
									<xsl:value-of select="'single'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$providerName = 'PostGIS'">
					<xsl:choose>
						<xsl:when test=". = 'decimal' and ../@scale = 0">
							<xsl:choose>
								<xsl:when test="../@precision=20">
									<xsl:value-of select="'int64'"/>
								</xsl:when>
								<xsl:when test="../@precision=10">
									<xsl:value-of select="'int32'"/>
								</xsl:when>
								<xsl:when test="../@precision=5">
									<xsl:value-of select="'int16'"/>
								</xsl:when>
								<xsl:when test="../@precision=3">
									<xsl:value-of select="'int16'"/>
								</xsl:when>
								<xsl:when test="../@precision=1">
									<xsl:value-of select="'boolean'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:choose>
								<xsl:when test="../@name='VOLUME'">
									<xsl:value-of select="'single'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test=". = 'decimal' and ../@scale = 0">
							<xsl:choose>
								<xsl:when test="../@precision=20">
									<xsl:value-of select="'int64'"/>
								</xsl:when>
								<xsl:when test="../@precision=10">
									<xsl:value-of select="'int32'"/>
								</xsl:when>
								<xsl:when test="../@precision=5">
									<xsl:value-of select="'int16'"/>
								</xsl:when>
								<xsl:when test="../@precision=3">
									<xsl:value-of select="'byte'"/>
								</xsl:when>
								<xsl:when test="../@precision=1">
									<xsl:value-of select="'boolean'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:when test=". = 'double'">
							<xsl:choose>
								<xsl:when test="../@name='VOLUME'">
									<xsl:value-of select="'single'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="."/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@dataType[local-name(..) = 'column' and $hybridLevel &gt; 0]">
		<xsl:attribute name="dataType">
			<xsl:choose>
				<xsl:when test="$providerName = 'Oracle'">
					<xsl:value-of select="."/>
				</xsl:when>
				<xsl:when test="$providerName = 'MySql'">
					<xsl:choose>
						<xsl:when test="../@dataType = 'DECIMAL' and ../@scale = 0">
							<xsl:choose>
								<xsl:when test="../@length=20">
									<xsl:value-of select="'BIGINT'"/>
								</xsl:when>
								<xsl:when test="../@length=10">
									<xsl:value-of select="'INT'"/>
								</xsl:when>
								<xsl:when test="../@length=5">
									<xsl:value-of select="'SMALLINT'"/>
								</xsl:when>
								<xsl:when test="../@length=3">
									<xsl:value-of select="'TINYINT UNSIGNED'"/>
								</xsl:when>
								<xsl:when test="../@length=1">
									<xsl:value-of select="'SMALLINT'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:when test=". = 'DOUBLE'">
							<xsl:choose>
								<xsl:when test="../@name='volume'">
									<xsl:value-of select="'FLOAT'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="'DOUBLE'"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="."/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$providerName = 'PostGIS'">
					<xsl:choose>
						<xsl:when test="../@dataType = 'NUMERIC' and ../@scale = 0">
							<xsl:choose>
								<xsl:when test="../@length=20">
									<xsl:value-of select="'BIGINT'"/>
								</xsl:when>
								<xsl:when test="../@length=10">
									<xsl:value-of select="'INTEGER'"/>
								</xsl:when>
								<xsl:when test="../@length=5">
									<xsl:value-of select="'SMALLINT'"/>
								</xsl:when>
								<xsl:when test="../@length=3">
									<xsl:value-of select="'SMALLINT'"/>
								</xsl:when>
								<xsl:when test="../@length=1">
									<xsl:value-of select="'BOOLEAN'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:when test=". = 'DOUBLE PRECISION'">
							<xsl:choose>
								<xsl:when test="../@name='volume'">
									<xsl:value-of select="'REAL'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="."/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="../@dataType = 'DECIMAL' and ../@scale = 0">
							<xsl:choose>
								<xsl:when test="../@length=20">
									<xsl:value-of select="'BIGINT'"/>
								</xsl:when>
								<xsl:when test="../@length=10">
									<xsl:value-of select="'INTEGER'"/>
								</xsl:when>
								<xsl:when test="../@length=5">
									<xsl:value-of select="'SMALLINT'"/>
								</xsl:when>
								<xsl:when test="../@length=3">
									<xsl:value-of select="'TINYINT'"/>
								</xsl:when>
								<xsl:when test="../@length=1">
									<xsl:value-of select="'BIT'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:when test=". = 'FLOAT'">
							<xsl:choose>
								<xsl:when test="../@name='volume'">
									<xsl:value-of select="'REAL'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="'FLOAT'"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="."/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@precision[local-name(..) = 'property' and $hybridLevel &gt; 0]">
		<xsl:attribute name="precision">
			<xsl:choose>
				<xsl:when test="$providerName = 'Oracle'">
					<xsl:value-of select="."/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="../@dataType = 'decimal' and ../@scale = 0">
							<xsl:choose>
								<xsl:when test=".=20">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=10">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=5">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=3">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=1">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="."/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@length[local-name(..) = 'column' and $hybridLevel &gt; 0]">
		<xsl:attribute name="length">
			<xsl:choose>
				<xsl:when test="$providerName = 'Oracle'">
					<xsl:value-of select="."/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="(../@dataType = 'DECIMAL' or ../@dataType = 'NUMERIC') and ../@scale = 0">
							<xsl:choose>
								<xsl:when test=".=20">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=10">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=5">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=3">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:when test=".=1">
									<xsl:value-of select="'0'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="."/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="."/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@nullable[((../@name = 'closed' and (../../@tableName = 'dbo.acdb3dpolyline' or ../../@tableName = 'public.acdb3dpolyline' or ../../@name = 'dbo.acdb3dpolyline')) or (../@name = 'CLOSED' and ../@tableName = 'dbo.acdb3dpolyline')) and $hybridLevel &gt; 0 and $providerName = 'SQLServerSpatial']">
		<xsl:attribute name="nullable">
			<xsl:value-of select="'True'"/>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@autogenerated[../@name = 'FEATID' and $hybridLevel &gt; 0 and not($providerName = 'Oracle')]">
		<xsl:attribute name="autogenerated">
			<xsl:choose>
				<xsl:when test="../@tableName = 'dbo.entity' or ../@tableName = 'dbo.polyline' or ../@tableName = 'public.entity' or ../@tableName = 'public.polyline' or ../@tableName = 'entity' or ../@tableName = 'polyline'">
					<xsl:value-of select="'False'"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="'True'"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@autogenerated[../@name = 'FEATUREID' and $hybridLevel &gt; 0 and not ($providerName = 'Oracle')]">
		<xsl:attribute name="autogenerated">
			<xsl:value-of select="'True'"/>
		</xsl:attribute>
	</xsl:template>
        <!-- TODO: FeatId should also be readonly for SQLServerSpatial provider -->
	<xsl:template match="@readOnly[../@name = 'FEATID' and $hybridLevel &gt; 0 and ($providerName = 'PostGIS')]">
		<xsl:attribute name="readOnly">
			<xsl:choose>
				<xsl:when test="../@tableName = 'dbo.entity' or ../@tableName = 'dbo.polyline' or ../@tableName = 'public.entity' or ../@tableName = 'public.polyline' or ../@tableName = 'entity' or ../@tableName = 'polyline'">
					<xsl:value-of select="'False'"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="'True'"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@readOnly[../@name = 'FEATUREID' and $hybridLevel &gt; 0 and ($providerName = 'PostGIS')]">
		<xsl:attribute name="readOnly">
			<xsl:value-of select="'True'"/>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@hasElevation[$hybridLevel &gt; 0 and ($providerName = 'SQLServerSpatial' or $providerName = 'MySql')]">
		<xsl:attribute name="hasElevation">
			<xsl:value-of select="'False'"/>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@geometryTypes[$hybridLevel &gt; 0 and ($providerName = 'MySql' or $providerName = 'PostGIS')]">
		<xsl:attribute name="geometryTypes">
			<xsl:value-of select="'4032'"/>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()"/>
		</xsl:copy>
	</xsl:template>
	<xsl:template name="bendPropName">
		<xsl:param name="inName"/>
		<xsl:choose>
			<xsl:when test="$providerName='Oracle'">
				<xsl:value-of select="$inName"/>
			</xsl:when>
			<xsl:when test="$providerName='SqlServer'">
				<xsl:call-template name="tolower">
					<xsl:with-param name="inString" select="$inName"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$providerName='SQLServerSpatial' or $providerName='PostGIS'">
				<xsl:choose>
					<xsl:when test="$inName = 'PLOT_STYLE'">
						<xsl:value-of select="'plot style'"/>
					</xsl:when>
					<xsl:when test="$inName = 'FIRST_NAME'">
						<xsl:value-of select="'first name'"/>
					</xsl:when>
					<xsl:when test="$inName = 'LAST_NAME'">
						<xsl:value-of select="'last name'"/>
					</xsl:when>
					<xsl:when test="$inName = 'CREDIT_RATING'">
						<xsl:value-of select="'credit rating'"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="tolower">
							<xsl:with-param name="inString" select="$inName"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
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
