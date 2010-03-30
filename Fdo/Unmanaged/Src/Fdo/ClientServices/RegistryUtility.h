#ifndef _FDOREGISTRYUTILITY_H_
#define _FDOREGISTRYUTILITY_H_
/***************************************************************************
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
 ***************************************************************************/

#ifdef _WIN32
#pragma once
#endif

// Include definitions
#include <string>
#include <vector>
#include <Fdo/ClientServices/Provider.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>

XERCES_CPP_NAMESPACE_USE;


// The FdoRegistryUtility class offers a set of utility for interacting with the system datastore
// to register and retrieve provider information.
class FdoRegistryUtility
{
public:
	// Gets a provider's library path for a provider based on the provider name
    static void GetLibraryLocation(const wchar_t* providerName, std::wstring &libraryLocation);

    // Fills a collection object with a list of valid providers defined in the registry
    static void GetProviderCollection(std::vector<FdoProvider*> &providers);

    // Creates a provider key in the system datastore based on the provider properties
    static void RegisterProvider(const wchar_t * name, 
                                 const wchar_t * displayName, 
                                 const wchar_t * description, 
                                 const wchar_t * version, 
                                 const wchar_t * fdoVersion, 
                                 const wchar_t * libraryPath,
                                 bool isManaged);

    // Removes a provider key from the system datastore based on the provider properties
    static void UnregisterProvider(const wchar_t * name);

private:
    static void AddText(DOMDocument *doc, DOMElement* featureProviderElem, const XMLCh* name, const wchar_t *nameValue);
    static bool DeleteProvider(DOMDocument *doc, const wchar_t * name);
    static wchar_t* GetFileName();
	static bool FileExists (const wchar_t* filePath);
    // Caller responsible for freeing document returned by GetDOMDocument
    static DOMDocument* GetDOMDocument();
    static void PutDOMDocument( DOMDocument* doc );
    static void DOMCleanup( 
        DOMDocument** doc, 
        XMLTranscoder** transcoder, 
        XercesDOMParser** parser,
        DOMLSSerializer** serializer,
        XMLFormatTarget** target
    );
};
#endif

