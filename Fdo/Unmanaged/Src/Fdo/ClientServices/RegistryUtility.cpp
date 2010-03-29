/***************************************************************************
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
 ***************************************************************************/
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <malloc.h>

#ifndef _WIN32
#include "prefix.h"
#include <wchar.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#endif
#include "RegistryUtility.h"
#include "ProviderDef.h"
#include <Fdo/ClientServices/Provider.h>
#include <Fdo/ClientServices/ClientServiceException.h>
#include <Fdo/ClientServices/ProviderNameTokens.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLErrorCodes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/TransService.hpp>
#include <Inc/Sys/stat.h>
#include "Common/Xml/UtilXrcs.h"

#include <xercesc/dom/impl/DOMLSOutputImpl.hpp>

#ifdef _WIN32
#define FILE_PATH_DELIMITER L'\\'
#define FILE_PATH_DELIMITER2 L'/'
#define ABSOLUTE_NAME_START 3
#else
#define FILE_PATH_DELIMITER L'/'
#define FILE_PATH_DELIMITER2 L'\\'
#define ABSOLUTE_NAME_START 1
#endif

#ifndef _WIN32
// macro to convert a multibyte string into a wide character string, allocating space on the stack
#define multibyte_to_wide(w,mb)\
{\
    const char* p = (mb);\
    size_t i = strlen (p);\
    i++;\
    w = (wchar_t*)alloca (i * sizeof (wchar_t));\
    i = mbstowcs (w, p, i);\
    if (0 > i)\
        w = NULL;\
    if (NULL==w) throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));\
}

// macro to convert a wide character string into a multibyte string, allocating space on the stack
#define wide_to_multibyte(mb,w)\
{\
    const wchar_t* p = (w);\
    size_t i = wcslen (p);\
    i++;\
    mb = (char*)alloca (i * 6);\
    i = wcstombs (mb, p, i);\
    if (0 > i)\
        mb = NULL;\
    if (NULL==mb) throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));\
}
#endif


XERCES_CPP_NAMESPACE_USE;

#ifdef _WIN32

extern wchar_t         module[];
extern wchar_t         home_dir[];

wchar_t* getHomeDir ()
{
    return (home_dir);
}

wchar_t* getModule ()
{
    return (module);
}

#else
extern wchar_t* getHomeDir();
extern wchar_t* getModule ();
#endif

bool FdoRegistryUtility::FileExists (const wchar_t* filePath)
{
    bool ret;

#ifdef _WIN32
    size_t length = wcslen (filePath);
    wchar_t* path = (wchar_t*)alloca (sizeof (wchar_t) * (length + 3)); // in case we need to add a \*
    wcscpy (path, filePath);
    if (0 < length)
    {
        // trim off trailing slash
        if (FILE_PATH_DELIMITER == path[length - 1])
            path[--length] = L'\0';
        else if (FILE_PATH_DELIMITER2 == path[length - 1])
            path[--length] = L'\0';
    }
    if ((1 < length) && (L':' == path[length - 1]))
        wcscat (path, L"\\*");
    WIN32_FIND_DATAW findData;
    HANDLE findHandle = ::FindFirstFileW (path, &findData);
    if (INVALID_HANDLE_VALUE != findHandle)
    {
        ::FindClose (findHandle);
        ret = true;
    }
    else
        ret = false;

#else // _WIN32

    char* file;
    int f;

    wide_to_multibyte (file, filePath);
    size_t length = strlen (file);
    if (FILE_PATH_DELIMITER2 == file[length - 1])
        file[length - 1] = FILE_PATH_DELIMITER;
    f = open (file, O_RDONLY, 0);
    ret = (-1 != f);
    if (ret)
        close (f);
#endif
    return (ret);
}

// Gets a Provider's library path for a provider based on the provider name
void FdoRegistryUtility::GetLibraryLocation(const wchar_t* providerName, std::wstring &libraryLocation)
{
    if (providerName == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    if (wcscmp(providerName, L"") == 0) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_2_EMPTYINPUTPARAMETER)));
    }

    if (!FileExists (GetFileName ()))
    {
        throw FdoClientServiceException::Create(
            FdoClientServiceException::NLSGetMessage(
                FDO_NLSID(CLNT_12_FILE_NOT_EXISTS),
                GetFileName()
            )
        );
    }

    bool bFound = false;
    XMLTranscoder* transcoder = NULL;
    DOMDocument *doc = NULL;

    XMLPlatformUtils::Initialize();
    
    try {

        FdoPtr<FdoProviderNameTokens> inputTokens = FdoProviderNameTokens::Create(providerName);
        FdoVectorP inputVersionTokens = inputTokens->GetVersionTokens();
        FdoVectorP currentTokens;
        FdoStringP currentLibraryPath;

        XMLTransService::Codes failReason;
        XMLCh* tmpCh = XMLString::transcode("UTF-8");
        transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(tmpCh, failReason, 1024);
        XMLString::release(&tmpCh);
        doc = GetDOMDocument();
        if (!doc)
        {
            tmpCh = XMLString::transcode("");
            DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tmpCh);
            XMLString::release(&tmpCh);
            tmpCh = XMLString::transcode(c_featureProviderRegistryName.c_str());
            doc = impl->createDocument(NULL, tmpCh, 0);
            XMLString::release(&tmpCh);
        }

        DOMElement*   root = doc->getDocumentElement();
        if (!root)
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
        DOMNodeList *featureProviders = root->getChildNodes();
        char *tmp = XMLString::transcode(root->getNodeName());
        if (strcmp(tmp, c_featureProviderRegistryName.c_str()) == 0)
        {
            XMLString::release(&tmp);
            XMLSize_t numberFeatureProviders = featureProviders->getLength();
            // Search thru the registry two times. The first time an exact match is needed.
            // The second time 
            for (int h = 0; h < 2; h++)
            {
                if (bFound == true)
                    break;
                for (XMLSize_t i=0; i<numberFeatureProviders; i++)
                {
                    if (h == 0)
                    {
                        if (bFound == true)
                            break;
                    }
                        
                    DOMNode *featureProvider = featureProviders->item(i);
				    FdoStringP pFeatureProvider = FdoXmlUtilXrcs::Xrcs2Unicode(featureProvider->getNodeName());
                    if (wcscmp(pFeatureProvider, c_featureProviderNameW.c_str()) == 0)
                    {
                        DOMNodeList *properties = featureProvider->getChildNodes();
                        XMLSize_t numberProperties = properties->getLength();
                        for (XMLSize_t j=0; j<numberProperties; j++)
                        {
                            if (h == 0)
                            {
                                if (bFound == true)
                                    break;
                            }
                            DOMNode *property = properties->item(j);
						    FdoStringP pNodeName = FdoXmlUtilXrcs::Xrcs2Unicode(property->getNodeName());
                            if (wcscmp(pNodeName, c_keyNameValue.c_str()) == 0)
                            {
                                DOMNodeList *names = property->getChildNodes();
                                if (names == NULL || names->getLength() == 0)
                                {
                                    throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
                                }
                                DOMNode *nameNode = names->item(0);
                                FdoStringP nameValue;
                                XMLSize_t chars = 0;
                                XMLSize_t length = XMLString::stringLen(nameNode->getNodeValue());
                                XMLByte *res = new XMLByte[length*6];
                                transcoder->transcodeTo(nameNode->getNodeValue(), length+1, res, length*6, chars, XMLTranscoder::UnRep_Throw);
                                nameValue = (char*) res;
                                delete[] res;

                                bool bEqual = true;
                                if (h == 0)
                                {
                                    bEqual = wcscmp(nameValue, providerName) == 0;
                                }
                                else
                                {
                                    FdoPtr<FdoProviderNameTokens> registryTokens = FdoProviderNameTokens::Create(nameValue);
                                    FdoVectorP registryVersionTokens = registryTokens->GetVersionTokens();

                                    FdoInt32 len1 = inputTokens->GetNameTokens()->GetCount();
                                    FdoInt32 len2 = registryTokens->GetNameTokens()->GetCount();
                                    FdoInt32 destLen = len1 > len2 ? len1 : len2;

                                    if ( destLen > 2 ) 
                                        destLen = 2;

                                    // Compare the vendor and local name tokens.
                                    for ( int k = 0; k < destLen; k++ )
                                    {
                                        FdoStringP tok1 = (k < len1) ? inputTokens->GetNameTokens()->GetString( (int) k) : L"";
                                        FdoStringP tok2 = (k < len2) ? registryTokens->GetNameTokens()->GetString( (int) k) : L"";
                                        if ( tok1 != tok2 ) 
                                        {
                                            bEqual = false;
                                            break;
                                        }
                                    }


                                    if (bEqual)
                                    {
                                        FdoInt32 len1 = inputVersionTokens->GetCount();
                                        FdoInt32 len2 = registryVersionTokens->GetCount();
                                        FdoInt32 destLen = len1;
                                        for ( int k = 0; k < destLen; k++ )
                                        {
                                            double val1 = (k < len1) ? inputVersionTokens->GetValue(k) : -1;
                                            double val2 = (k < len2) ? registryVersionTokens->GetValue(k) : -1;
                                            if (val1 != val2)
                                            {
                                                bEqual = false;
                                                break;
                                            }

                                        }
            
                                        if (bEqual)
                                        {
                                            bEqual = false;
                                            if (!currentTokens || registryVersionTokens > currentTokens)
                                            {
                                                currentTokens = registryVersionTokens;
                                                bEqual = true;
                                            }
                                        }
                                    }
                                }
                                if (bEqual == true)          
                                {
                                    for (int k=0; k<numberProperties; k++)
                                    {
                                        DOMNode *prop = properties->item(k);
									    FdoStringP pProp = FdoXmlUtilXrcs::Xrcs2Unicode(prop->getNodeName());
                                        if (wcscmp(pProp, c_keyLibraryValue.c_str()) == 0)
                                        {
                                            DOMNodeList *libraries = prop->getChildNodes();
                                            if (libraries == NULL || libraries->getLength() == 0)
                                            {
                                                throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
                                            }
                                            DOMNode *libNode = libraries->item(0);
										    FdoStringP pPath = FdoXmlUtilXrcs::Xrcs2Unicode(libNode->getNodeValue());
                                            libraryLocation = (FdoString*)pPath;
                                            bFound = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
            XMLString::release(&tmp);
        DOMCleanup( &doc, &transcoder, NULL, NULL, NULL );
    }
    catch (...) 
    {
        DOMCleanup( &doc, &transcoder, NULL, NULL, NULL );
        XMLPlatformUtils::Terminate();

        throw;
    }

    XMLPlatformUtils::Terminate();

    if (bFound == false)
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_4_PROVIDERNOTREGISTERED)));
}

void FdoRegistryUtility::GetProviderCollection(std::vector<FdoProvider*> &providerCollection)
{
    wchar_t name[256];
    wchar_t displayName[256];
    wchar_t description[512];
    wchar_t version[256];
    wchar_t fdoVersion[256];
    wchar_t libraryPath[512];
    int isManaged = false;

    if (!FileExists (GetFileName ()))
        return;

    XMLPlatformUtils::Initialize();
    XMLTranscoder* transcoder = NULL;
    DOMDocument *doc = NULL;

    try {
        XMLTransService::Codes failReason;
        XMLCh *tmpCh = XMLString::transcode("UTF-8");
        transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(tmpCh, failReason, 1024);
        XMLString::release(&tmpCh);

        doc = GetDOMDocument();
        DOMElement*   root = doc->getDocumentElement();
        if (!root)
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
        char *tmp = XMLString::transcode(root->getNodeName());
        if (strcmp(tmp, c_featureProviderRegistryName.c_str()) == 0)
        {
            XMLString::release(&tmp);
            DOMNodeList *featureProviders = root->getChildNodes();
            XMLSize_t numberFeatureProviders = featureProviders->getLength();
            for (XMLSize_t i=0; i<numberFeatureProviders; i++)
            {

                name[0] = L'\0';
                displayName[0] = L'\0';
                description[0] = L'\0';
                version[0] = L'\0';
                fdoVersion[0] = L'\0';
                libraryPath[0] = L'\0';

                DOMNode *featureProvider = featureProviders->item(i);
                short type = featureProvider->getNodeType();
                if (type != DOMNode::TEXT_NODE)
                {
                    tmp = XMLString::transcode(root->getNodeName());
                    if (strcmp(tmp, c_featureProviderRegistryName.c_str()) == 0)
                    {
                        XMLString::release(&tmp);
                        DOMNodeList *properties = featureProvider->getChildNodes();
                        XMLSize_t numberProperties = properties->getLength();
                        for (XMLSize_t j=0; j<numberProperties; j++)
                        {
                            DOMNode *property = properties->item(j);
                            type = property->getNodeType();
                            if (type != DOMNode::TEXT_NODE)
                            {
                                FdoStringP nameValue;
                                wchar_t propertyName[512];
                                char *tmp = XMLString::transcode(property->getNodeName());
                                mbstowcs(propertyName, tmp, sizeof(propertyName)/sizeof(wchar_t));
                                XMLString::release(&tmp);
    					
                                DOMNodeList *names = property->getChildNodes();
                                if (names == NULL || names->getLength() == 0)
                                {
                                    throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
                                }
                                DOMNode *nameNode = names->item(0);
                                XMLSize_t chars = 0;
                                XMLSize_t length = XMLString::stringLen(nameNode->getNodeValue());
                                XMLByte *res = new XMLByte[length * 6];
                                transcoder->transcodeTo(nameNode->getNodeValue(), length+1, res, length *6, chars, XMLTranscoder::UnRep_Throw);
                                nameValue = (char*) res;
                                delete[] res;
    					
                                if (wcscmp(propertyName, c_keyNameValue.c_str()) == 0)
                                    wcscpy(name, nameValue);
                                else if (wcscmp(propertyName, c_keyDisplayNameValue.c_str()) == 0)
                                    wcscpy(displayName, nameValue);
                                else if (wcscmp(propertyName, c_keyLibraryValue.c_str()) == 0)
                                    wcscpy(libraryPath, nameValue);
                                else if (wcscmp(propertyName, c_keyVersionValue.c_str()) == 0)
                                    wcscpy(version, nameValue);
                                else if (wcscmp(propertyName, c_keyFDOVersionValue.c_str()) == 0)
                                    wcscpy(fdoVersion, nameValue);
                                else if (wcscmp(propertyName, c_keyDescriptionValue.c_str()) == 0)
                                    wcscpy(description, nameValue);
                                else if (wcscmp(propertyName, c_keyIsManagedValue.c_str()) == 0)
                                    isManaged = wcscmp(nameValue, L"True") == 0;
                            }
                        }
                        FdoProvider* provider = new FdoProvider(name, displayName, description, version, fdoVersion, libraryPath, isManaged == 0 ? false : true);
                        if (provider == NULL)
                        {
                            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_5_OUTOFMEMORY)));
                        }
                        providerCollection.push_back(provider);
                    }
                    else
                        XMLString::release(&tmp);
                }
            }
        }
        else
            XMLString::release(&tmp);

        DOMCleanup( &doc, &transcoder, NULL, NULL, NULL );
    }
    catch ( ... ) 
    {
        DOMCleanup( &doc, &transcoder, NULL, NULL, NULL );

        XMLPlatformUtils::Terminate();
        throw;
    }


    XMLPlatformUtils::Terminate();

    return;
}


// Creates a provider key in the registry based on the provider properties
void FdoRegistryUtility::RegisterProvider(const wchar_t * name, 
                                          const wchar_t * displayName, 
                                          const wchar_t * description, 
                                          const wchar_t * version, 
                                          const wchar_t * fdoVersion, 
                                          const wchar_t * libraryPath,
                                          bool isManaged)
{
    XMLPlatformUtils::Initialize();
 
    DOMDocument *doc = NULL;

    try {

        DOMImplementation* impl = NULL;

        XMLCh *tmpCh = XMLString::transcode("");
        impl = DOMImplementationRegistry::getDOMImplementation(tmpCh);
        XMLString::release(&tmpCh);

        bool bfileExist = FileExists (GetFileName ());
        if (bfileExist == false)
        {
            tmpCh =  XMLString::transcode(c_featureProviderRegistryName.c_str());
            doc = impl->createDocument(0, tmpCh, 0);
            XMLString::release(&tmpCh);
        }
        else
        {
            doc = GetDOMDocument();
            isManaged;
        }

        DOMElement*   root = doc->getDocumentElement();
        if (!root)
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
        char *tmp = XMLString::transcode(root->getNodeName());
        if (strcmp(tmp, c_featureProviderRegistryName.c_str()) != 0)
        {
            XMLString::release(&tmp);
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
        }
        XMLString::release(&tmp);
        DeleteProvider(doc, name);

        tmpCh = XMLString::transcode("FeatureProvider");
        DOMElement*  featureProviderElem = doc->createElement(tmpCh);
        XMLString::release(&tmpCh);

        tmpCh = XMLString::transcode("Name");
        AddText(doc, featureProviderElem, tmpCh, name);
        XMLString::release(&tmpCh);

        tmpCh = XMLString::transcode("DisplayName");
        AddText(doc, featureProviderElem, tmpCh, displayName);
        XMLString::release(&tmpCh);

        tmpCh = XMLString::transcode("Description");
        AddText(doc, featureProviderElem, tmpCh, description);
        XMLString::release(&tmpCh);

        tmpCh = XMLString::transcode("IsManaged");
        AddText(doc, featureProviderElem, tmpCh, isManaged ? L"True" : L"False");
        XMLString::release(&tmpCh);

        tmpCh = XMLString::transcode("Version");
        AddText(doc, featureProviderElem, tmpCh, version);
        XMLString::release(&tmpCh);

        tmpCh = XMLString::transcode("FeatureDataObjectsVersion");
        AddText(doc, featureProviderElem, tmpCh, fdoVersion);
        XMLString::release(&tmpCh);

        tmpCh = XMLString::transcode("LibraryPath");
        AddText(doc, featureProviderElem, tmpCh, libraryPath);
        XMLString::release(&tmpCh);

        root->appendChild(featureProviderElem);

        PutDOMDocument(doc);

        DOMCleanup( &doc, NULL, NULL, NULL, NULL );
    }
    catch (...) 
    {
        DOMCleanup( &doc, NULL, NULL, NULL, NULL );

        XMLPlatformUtils::Terminate();
        throw;
    }

    XMLPlatformUtils::Terminate();

    return;
}

void FdoRegistryUtility::UnregisterProvider(const wchar_t * providerName)
{
    if (!FileExists (GetFileName ()))
    {
        throw FdoClientServiceException::Create(
            FdoClientServiceException::NLSGetMessage(
                FDO_NLSID(CLNT_12_FILE_NOT_EXISTS),
                GetFileName()
            )
        );
    }

    XMLPlatformUtils::Initialize();
    DOMDocument *doc = NULL;

    try {
        doc = GetDOMDocument();
        DOMImplementation* impl = NULL;
        XMLCh *tmpCh = XMLString::transcode("LS");
        impl = DOMImplementationRegistry::getDOMImplementation(tmpCh);
        XMLString::release(&tmpCh);
        if (!doc)
        {
            tmpCh = XMLString::transcode(c_featureProviderRegistryName.c_str());
            doc = impl->createDocument(NULL, tmpCh, NULL);
            XMLString::release(&tmpCh);
        }

        DOMElement* root = doc->getDocumentElement();
        if (!root)
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
        char *tmp = XMLString::transcode(root->getNodeName());
        if (strcmp(tmp, c_featureProviderRegistryName.c_str()) != 0)
        {
            XMLString::release(&tmp);
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
        }
        XMLString::release(&tmp);
        bool bDeleted = DeleteProvider(doc, providerName);
        if (bDeleted == false)
        {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_4_PROVIDERNOTREGISTERED)));
        }

        // check if there are any providers left, delete the file if not
        bool providers_present = false;
        DOMNodeList *featureProviders = root->getChildNodes();
        for (int i = 0; i < (int) featureProviders->getLength() && !providers_present; i++)
        {
            DOMNode *featureProvider = featureProviders->item(i);
            short type = featureProvider->getNodeType();
            if (type != DOMNode::TEXT_NODE)
                providers_present = true;
        }

        if (providers_present)
        {
            PutDOMDocument(doc);
        }
        else
#ifdef _WIN32
            _wunlink (GetFileName());
#else
		{
			char* pFileName;
			wide_to_multibyte(pFileName, GetFileName());
			unlink (pFileName);
		}
#endif

        DOMCleanup( &doc, NULL, NULL, NULL, NULL );
    }
    catch (...) 
    {
        DOMCleanup( &doc, NULL, NULL, NULL, NULL );

        XMLPlatformUtils::Terminate();
        throw;
    }

    XMLPlatformUtils::Terminate();

}

void FdoRegistryUtility::AddText(DOMDocument *doc, DOMElement* featureProviderElem, const XMLCh* name, const wchar_t *nameValue)
{
    FdoStringP tmp = nameValue;

    XMLTransService::Codes failReason;
    XMLCh *tmpCh = XMLString::transcode("UTF-8");
    XMLTranscoder* transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(tmpCh, failReason, 1024);
    XMLString::release(&tmpCh);    

    XMLSize_t chars = 0;
    unsigned int length = (unsigned int) strlen((const char*) tmp);
    XMLCh *res = new XMLCh[length+1];
    unsigned char *sizes = new unsigned char[length+1];
    transcoder->transcodeFrom((XMLByte *) (const char*) tmp, length+1, res, length+1, chars, sizes);

    DOMElement*  element = doc->createElement(name);
    DOMText* prodDataVal = doc->createTextNode(res);
    element->appendChild(prodDataVal);
    featureProviderElem->appendChild(element);
    
    delete[] res;
    delete[] sizes;
    delete transcoder;

}

bool FdoRegistryUtility::DeleteProvider(DOMDocument *doc, const wchar_t * name)

{
    DOMElement*   root = doc->getDocumentElement();
    if (!root)
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
    bool bDeleted = false;
    {
        DOMNodeList *featureProviders = root->getChildNodes();
        XMLSize_t numberFeatureProviders = featureProviders->getLength();
        for (XMLSize_t i=0; i<numberFeatureProviders && !bDeleted; i++)
        {
            DOMNode *featureProvider = featureProviders->item(i);
            char *tmp = XMLString::transcode(featureProvider->getNodeName());
            wchar_t nodeName[512];
            mbstowcs(nodeName, tmp, sizeof(nodeName)/sizeof(wchar_t));
            XMLString::release(&tmp);
            if (wcscmp(nodeName, c_featureProviderNameW.c_str()) == 0)
            {
                DOMNodeList *properies = featureProvider->getChildNodes();
                XMLSize_t numberProperties = properies->getLength();
                for (XMLSize_t j=0; j<numberProperties; j++)
                {
                    DOMNode *property = properies->item(j);
                    tmp = XMLString::transcode(property->getNodeName());
                    mbstowcs(nodeName, tmp, sizeof(nodeName)/sizeof(wchar_t));
                    XMLString::release(&tmp);
                    if (wcscmp(nodeName, c_keyNameValue.c_str()) == 0)
                    {
                        DOMNodeList *names = property->getChildNodes();
                        if (names == NULL || names->getLength() == 0)
                            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_11_PARSER_ERROR)));
                        DOMNode *nameNode = names->item(0);
                        tmp = XMLString::transcode(nameNode->getNodeValue());
                        wchar_t nameValue[512];
                        mbstowcs(nameValue, tmp, sizeof(nameValue)/sizeof(wchar_t));
                        XMLString::release(&tmp);
                        if (wcscmp(nameValue, name) == 0)
                        {
                            root->removeChild(featureProvider);
                            bDeleted = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    return bDeleted;
}

#ifdef _WIN32
BOOL GetFdoVersion (wchar_t* module, wchar_t* buffer)
{
    DWORD size;
    DWORD handle;
    void* version;
    VS_FIXEDFILEINFO* info;
    UINT length;
    BOOL ret;

    ret = FALSE;

    if (0 != (size = GetFileVersionInfoSizeW (module, &handle)))
    {
        version = alloca (size);
        if (0 != (ret = GetFileVersionInfoW (module, 0, size, version)))
        {
            if (VerQueryValueW (version, L"\\", (LPVOID *)&info, &length))
            {
                swprintf (buffer, L"%d.%d", info->dwProductVersionMS >> 16, info->dwProductVersionMS & 0xffff);
                ret = TRUE;
            }
        }
    }

    return (ret);
}
#endif

wchar_t *FdoRegistryUtility::GetFileName()
{
    static bool first = true;
    static wchar_t fileName[512];
    
    if (first)
    {
        first = false;
#ifndef _WIN32
        {
            char _fileName[512];
            struct stat my_stat;
            const char *me;
            char *home;
            char *last;
            const char *install = "/usr/local/fdo-3.5.0";

            // Determine the user-specified FDO install location
            char *fdo_home = getenv( "FDOHOME" );
            if ( NULL != fdo_home ) { 
                install = fdo_home;
            }

            // try where we are
            me = SELFPATH;
            home = (char*)alloca (strlen (me) + 1);
            strcpy (home, me);
            last = strrchr (home, '/');
            if (NULL != last)
            {
                last++;
                *last = '\0';
            }
            else
                home = (char*)"./";
            
            sprintf (_fileName, "%s%s", home, "providers.xml");
            if ((0 != stat (_fileName, &my_stat)) || !S_ISREG(my_stat.st_mode))
                // not found or not a file, try the install location
                if ((0 == stat (install, &my_stat)) && S_ISDIR(my_stat.st_mode))
                // Try lib64 first, since some 64 bits linux install put libraries it there
                    sprintf (_fileName, "%s%s", install, "/lib64/providers.xml");
                if ((0 != stat (_fileName, &my_stat)) || !S_ISREG(my_stat.st_mode))
                    sprintf (_fileName, "%s%s", install, "/lib/providers.xml");
			mbstowcs(fileName, _fileName, 512);        }
#else
        {
            swprintf (fileName, L"%ls%ls", getHomeDir (), L"providers.xml");
        }
#endif
    }

    return fileName;
}

DOMDocument* FdoRegistryUtility::GetDOMDocument()
{
    XercesDOMParser* parser = NULL;
    DOMDocument *doc = NULL;

    try {
        DefaultHandler errHandler;

        parser = new XercesDOMParser();
        // Setting the error handler ensures we get the SAXParseException thrown from Xerces.
        parser->setErrorHandler( &errHandler );
#ifndef _WIN32
		{
			char* pFileName;
			wide_to_multibyte(pFileName, GetFileName());
			parser->parse( pFileName);
		}
#else
        parser->parse((const XMLCh*)GetFileName());
#endif
        doc = parser->adoptDocument();
    }
    catch ( const SAXParseException& ex ) 
    {
        char* msg = XMLString::transcode(ex.getMessage());

        FdoClientServiceException* ex2 = FdoClientServiceException::Create(
            FdoClientServiceException::NLSGetMessage(
                FDO_NLSID(CLNT_14_XML_ERROR),
                GetFileName(),
                ex.getLineNumber(),
                msg
            )
        );
        DOMCleanup( NULL, NULL, &parser, NULL, NULL );
        XMLString::release(&msg);

        throw ex2;
    }
    catch ( ... ) 
    {
        DOMCleanup( NULL, NULL, &parser, NULL, NULL );

        throw;
    }

    DOMCleanup( NULL, NULL, &parser, NULL, NULL );

    return doc;
}

void FdoRegistryUtility::PutDOMDocument( DOMDocument* doc )
{
    DOMLSSerializer *serializer = NULL;
    XMLFormatTarget *target = NULL;

    try {
        DOMImplementation* impl = NULL;

        XMLCh *tmpCh = XMLString::transcode("");
        impl = DOMImplementationRegistry::getDOMImplementation(tmpCh);
        XMLString::release(&tmpCh);

        serializer = ((DOMImplementationLS*)impl)->createLSSerializer();
		DOMConfiguration* dc = serializer->getDomConfig();
        if (dc->canSetParameter (XMLUni::fgDOMWRTFormatPrettyPrint, true))
            dc->setParameter (XMLUni::fgDOMWRTFormatPrettyPrint, true);
		
#ifndef _WIN32
		char* pFileName;
		wide_to_multibyte(pFileName, GetFileName());
        target = new LocalFileFormatTarget(pFileName);
#else
        target = new LocalFileFormatTarget((const XMLCh*)GetFileName());
#endif
		
		DOMLSOutputImpl* lsOut = new DOMLSOutputImpl();
		lsOut->setByteStream(target);
		serializer->write(doc->getDocumentElement(), lsOut);

        DOMCleanup( NULL, NULL, NULL, &serializer, &target );

		lsOut->release();
    }
    catch ( const XMLException& ex )
    {
        char* msg = XMLString::transcode(ex.getMessage());

        FdoClientServiceException* ex2 = FdoClientServiceException::Create(
            FdoClientServiceException::NLSGetMessage(
                FDO_NLSID(CLNT_15_XML_WRITE_ERROR),
                GetFileName(),
                msg
            )
        );

        DOMCleanup( NULL, NULL, NULL, &serializer, &target );
        XMLString::release(&msg);

        throw ex2;
    }
    catch ( ... ) 
    {
        DOMCleanup( NULL, NULL, NULL, &serializer, &target );

        throw;
    }
}


void FdoRegistryUtility::DOMCleanup( 
    DOMDocument** doc, 
    XMLTranscoder** transcoder, 
    XercesDOMParser** parser,
    DOMLSSerializer** serializer,
    XMLFormatTarget** target
)
{
    // Ignore errors on cleanup
    try {
        if ( doc && (*doc) ) {
            (*doc)->release();
            (*doc) = NULL;
        }

        if ( transcoder && (*transcoder) ) {
            delete *transcoder;
            (*transcoder) = NULL;
        }

        if ( parser && (*parser) ) {
            delete *parser;
            (*parser) = NULL;
        }

        if ( serializer && (*serializer) ) {
            delete *serializer;
            (*serializer) = NULL;
        }

        if ( target && (*target) ) {
            delete *target;
            (*target) = NULL;
        }
    }
    catch (...) {
    }
}

