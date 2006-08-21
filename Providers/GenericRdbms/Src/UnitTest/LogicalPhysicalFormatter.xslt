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

<xsl:template match="lp:schema[@name='F_MetaClass']"/>
<xsl:template match="lp:column[@name = 'classname' or @name = 'schemaname']">
</xsl:template>
<xsl:template match="lp:column[@dataType and not(@name = 'classname' or @name = 'schemaname')]">
	<xsl:variable name="colName" select="@name"/>
   <xsl:if test="local-name(..)='property' or ../../lp:properties/lp:property[@columnName=$colName or translate(@name,'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')=substring-before($colName,'_SI_1') or translate(@name,'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')=substring-before($colName,'_SI_2')]">
		<xsl:element name="column">
			<xsl:attribute name="name">
				<xsl:value-of select="@name"/>
			</xsl:attribute>
			<xsl:attribute name="description">
				<xsl:value-of select="@description"/>
			</xsl:attribute>
			<xsl:attribute name="dataType">
				<xsl:value-of select="@dataType"/>
			</xsl:attribute>
			<xsl:attribute name="length">
				<xsl:value-of select="@length"/>
			</xsl:attribute>
			<xsl:attribute name="scale">
				<xsl:value-of select="@scale"/>
			</xsl:attribute>
			<xsl:attribute name="nullable">
				<xsl:value-of select="@nullable"/>
			</xsl:attribute>
		</xsl:element>
	</xsl:if>
</xsl:template>
<xsl:template match="@pkeyName"/>
<xsl:template match="@*|node()">
  <xsl:copy>
    <xsl:apply-templates select="@*|node()"/>
  </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
