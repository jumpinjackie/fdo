//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

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
