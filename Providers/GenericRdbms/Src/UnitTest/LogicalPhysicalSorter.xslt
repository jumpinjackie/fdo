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
<xsl:template match="lp:properties">
  <xsl:copy>
    <xsl:apply-templates select="lp:property">
		<xsl:sort select="@name"/>
    </xsl:apply-templates>
  </xsl:copy>
</xsl:template>
<xsl:template match="lp:table">
  <xsl:copy>
    <xsl:apply-templates select="@*|lp:target"/>
    <xsl:apply-templates select="lp:property">
		<xsl:sort select="@name"/>
    </xsl:apply-templates>
    <xsl:apply-templates select="lp:column">
		<xsl:sort select="@name"/>
    </xsl:apply-templates>
  </xsl:copy>
</xsl:template>
<xsl:template match="@*|node()">
  <xsl:copy>
    <xsl:apply-templates select="@*|node()"/>
  </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
