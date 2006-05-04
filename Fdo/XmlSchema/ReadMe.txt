This directory contains the XML Schema files that describe the FDO XML Format:

	FdoDocument.xsd - describes the FDO XML format for:
		Feature Schemas
		Spatial Contexts
		Schema Overrides - allows provider-specific override formats to be plugged in.

	FdoDocumentSample.xsd - extends FdoDocument.xsd to include Schema Overrides for 3 example FDO 
	providers. 

	FdoBase.xsd - basic types defined by FDO.

	FdoOverride.xsd - base types for defining Schema Overrides.

	GML/* - schemas describing GML 3.0, from the Open GIS Consortium. The files are unaltered.

	Xlink/* - Xline format from W3. The files are unaltered.

	XmlSchema - W3 XML Schema format. The files are unaltered.

Note: FdoDocumentSample.xsd imports Schema Overrides, from various providers, through relative paths. 
You may have to modify these paths at your site. 
