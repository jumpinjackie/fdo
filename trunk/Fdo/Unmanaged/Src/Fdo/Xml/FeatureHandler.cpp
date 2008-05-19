// 
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

#include <Fdo/Xml/FeatureHandler.h>

///<summary>
/// This is only a dummy implementation to facilitate linkage of WFS provider,
/// it will be updated later.
///</summary>

FdoXmlFeatureHandler* FdoXmlFeatureHandler::FeatureDocumentStart(FdoXmlFeatureContext*)
{
	return NULL;
}

void FdoXmlFeatureHandler::FeatureDocumentEnd(FdoXmlFeatureContext*)
{
}

FdoXmlFeatureHandler* FdoXmlFeatureHandler::FeatureCollectionStart(FdoXmlFeatureContext*,FdoClassDefinition*)
{
	return NULL;
}

FdoBoolean FdoXmlFeatureHandler::FeatureCollectionEnd(FdoXmlFeatureContext*)
{
	return false;
}

FdoXmlFeatureHandler* FdoXmlFeatureHandler::FeatureStart(FdoXmlFeatureContext*,FdoClassDefinition*)
{
	return NULL;
}


FdoBoolean FdoXmlFeatureHandler::FeatureEnd(FdoXmlFeatureContext*)
{
	return false;
}

FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoBoolean)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoByte)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDateTime)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDouble)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt16)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt32)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt64)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoFloat)
{
	return false;
}



FdoBoolean FdoXmlFeatureHandler::FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoString*)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureStartRasterProperty(FdoXmlFeatureContext*, FdoString*, FdoIRaster*)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureEndRasterProperty(FdoXmlFeatureContext*)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureStartLobProperty(FdoXmlFeatureContext*, FdoString*)
{
	return false;
}



FdoBoolean FdoXmlFeatureHandler::FeatureEndLobProperty(FdoXmlFeatureContext*)
{
	return false;
}



FdoBoolean FdoXmlFeatureHandler::FeatureGeometricProperty(FdoXmlFeatureContext*, FdoString*, FdoByte*, FdoInt32)
{
	return false;
}

FdoXmlFeatureHandler* FdoXmlFeatureHandler::FeatureStartObjectProperty(FdoXmlFeatureContext*, FdoString*, FdoClassDefinition*)
{
	return NULL;
}

FdoBoolean FdoXmlFeatureHandler::FeatureEndObjectProperty(FdoXmlFeatureContext*)
{
	return false;
}



FdoXmlFeatureHandler* FdoXmlFeatureHandler::FeatureStartAssociationProperty(FdoXmlFeatureContext*, FdoString*, FdoClassDefinition*)
{
	return NULL;
}

FdoBoolean FdoXmlFeatureHandler::FeatureEndAssociationProperty(FdoXmlFeatureContext*)
{
	return false;
}



FdoBoolean FdoXmlFeatureHandler::FeatureCharacters(FdoXmlFeatureContext*, FdoString*)
{
	return false;
}


FdoBoolean FdoXmlFeatureHandler::FeatureBinaryData(FdoXmlFeatureContext*, FdoByte*, FdoSize)
{
	return false;
}



