/*
* 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
*/

#include "stdafx.h"
#include <Fdo.h>

#include "FDO\Xml\mgXmlFeatureSerializer.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlFeatureFlags.h"
#include "FDO\Xml\mgXmlFeatureWriter.h"
#include "FDO\Connections\mgIConnectionImp.h"
#include "FDO\Commands\Feature\mgIFeatureReaderImp.h"
#include "FDO\Schema\mgFeatureSchemaCollection.h"

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureSerializer::XmlSerialize(NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ reader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter^ writer)
{
	EXCEPTION_HANDLER(FdoXmlFeatureSerializer::XmlSerialize(static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp^>(reader)->GetImpObj(), writer->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureSerializer::XmlSerialize(NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ reader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter^ writer, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags^ flags)
{
	EXCEPTION_HANDLER(FdoXmlFeatureSerializer::XmlSerialize(static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp^>(reader)->GetImpObj(), writer->GetImpObj(), flags->GetImpObj()))
}
