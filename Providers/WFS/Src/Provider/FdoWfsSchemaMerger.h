/*
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

#ifndef FDOWFSSCHEMAMERGER_H
#define FDOWFSSCHEMAMERGER_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWfsNamedIoStreamCollection;
class FdoWfsSchemaCopyHandler;

class FdoWfsSchemaMerger
{
private:
    FdoPtr<FdoWfsNamedIoStreamCollection> m_loadedStreams;
    FdoPtr<FdoDictionary> m_mergedList;
    FdoPtr<FdoIoMemoryStream> m_mergedSchema;
    FdoPtr<FdoWfsSchemaCopyHandler> m_copier;
    bool m_FeatureLoaded;
    bool m_GeometryLoaded;
    bool m_XlinksLoaded;
	FdoStringP m_version;
	std::map<FdoStringP,char** > m_Gml311;

public:
    FdoWfsSchemaMerger();
	FdoWfsSchemaMerger(FdoString* version);
    virtual ~FdoWfsSchemaMerger();
    FdoIoStream* MergeSchema(FdoIoStream* schema, FdoString* location, FdoString* uri);

// helper
private:
    void _mergeSchema(FdoIoStream* schema, FdoString* location, FdoString* uri);
    FdoStringP _getFullLocation(FdoString* relativeLocation, FdoString* baseLocation);

	FdoStringP _getXSDName(FdoStringP location);
};


#endif

