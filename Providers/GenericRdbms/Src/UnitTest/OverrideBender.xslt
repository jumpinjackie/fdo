<?xml version="1.0" encoding="UTF-8" ?>
<stylesheet version="1.0" 
xmlns="http://www.w3.org/1999/XSL/Transform" 
xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
xmlns:gml="http://www.opengis.net/gml" 
xmlns:xs="http://www.w3.org/2001/XMLSchema" 
xmlns:fdo="http://fdo.osgeo.org/schemas"
xmlns:ora="http://www.autodesk.com/isd/fdo/OracleProvider"
xmlns:mql="http://fdomysql.osgeo.org/schemas"
xmlns:sqs="http://www.autodesk.com/isd/fdo/SQLServerProvider"
>

<xsl:param name="providerName"/>
<xsl:param name="providerUri"/>
<xsl:param name="oldOwnerPrefix" />
<xsl:param name="newOwnerPrefix" />
<xsl:template match="ora:SchemaMapping|mql:SchemaMapping|sqs:SchemaMapping">
    <xsl:element name="SchemaMapping" namespace="{$providerUri}">
		<xsl:for-each select="namespace::node()[not(name()='')]">
			<xsl:copy/>
		</xsl:for-each>
		<xsl:variable name="fromMySql">
			<xsl:choose>
				<xsl:when test="contains(@provider,'MySql')">yes</xsl:when>
				<xsl:otherwise>no</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="toMySql">
			<xsl:choose>
				<xsl:when test="contains($providerName,'MySql')">yes</xsl:when>
				<xsl:otherwise>no</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$fromMySql = $toMySql">
				<xsl:if test="@database">
					<xsl:attribute name="database">
							<xsl:call-template name="BendPrefix">
								<xsl:with-param name="inString" select="@database"/>
							</xsl:call-template>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="@owner">
					<xsl:attribute name="owner">
							<xsl:call-template name="BendPrefix">
								<xsl:with-param name="inString" select="@owner"/>
							</xsl:call-template>
					</xsl:attribute>
				</xsl:if>
			</xsl:when>
			<xsl:when test="$fromMySql = 'yes'">
				<xsl:if test="@database">
					<xsl:attribute name="owner">
							<xsl:call-template name="BendPrefix">
								<xsl:with-param name="inString" select="@database"/>
							</xsl:call-template>
					</xsl:attribute>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="@owner">
					<xsl:attribute name="database">
							<xsl:call-template name="BendPrefix">
								<xsl:with-param name="inString" select="@owner"/>
							</xsl:call-template>
					</xsl:attribute>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:if test="@database">
			<xsl:attribute name="database">
					<xsl:call-template name="BendPrefix">
						<xsl:with-param name="inString" select="@database"/>
					</xsl:call-template>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@owner">
			<xsl:attribute name="owner">
					<xsl:call-template name="BendPrefix">
						<xsl:with-param name="inString" select="@owner"/>
					</xsl:call-template>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@tableMapping">
			<xsl:attribute name="tableMapping" >
					<xsl:value-of select="@tableMapping"/>
			</xsl:attribute>
		</xsl:if>
		<xsl:attribute name="provider">
				<xsl:value-of select="$providerName"/>
		</xsl:attribute>
		<xsl:attribute name="name">
				<xsl:value-of select="@name"/>
		</xsl:attribute>
		<xsl:apply-templates select="*"/>
	</xsl:element>
</xsl:template>
<xsl:template match="@*|node()">
  <xsl:copy>
    <xsl:apply-templates select="@*|node()"/>
  </xsl:copy>
  </xsl:template>
  <xsl:template name="BendPrefix">
	  <xsl:param name="inString"/>
	  <xsl:choose>
			<xsl:when test="starts-with($inString, $oldOwnerPrefix)">
				<xsl:value-of select="concat($newOwnerPrefix,substring-after($inString,$oldOwnerPrefix))"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$inString"/>
			</xsl:otherwise>
		</xsl:choose>
  </xsl:template>
</stylesheet>
