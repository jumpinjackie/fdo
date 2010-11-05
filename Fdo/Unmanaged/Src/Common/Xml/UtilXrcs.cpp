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
#include <malloc.h>
#include <FdoCommon.h>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include "../ConvertUTF.h"
#include <Common/Xml/UtilXrcs.h>
#include <xercesc/framework/MemoryManager.hpp>

void FdoXmlUtilXrcs::Initialize()
{
    // does nothing now... see TransformerXalan
}

FdoStringP FdoXmlUtilXrcs::Xrcs2Unicode( const XMLCh* chars, const XMLSize_t length )
{
    FdoStringP outString;
    XMLSize_t localLength = length;

#ifdef _WIN32

    // On Windows, wchar_t is 16 bits (UTF-16), and XMLCh is 16-bits, so no conversion necessary:
    if (localLength == 0)
        outString = (wchar_t*) chars;  // fast assignment step (no conversion)
    else
    {
		bool inHeap = sizeof(wchar_t)*(length+1) > 512 * 1024 ? true : false;
        wchar_t* target;
		if (inHeap)
			target = (wchar_t*)new unsigned char[sizeof(wchar_t)*(length+1)];
		else
			target = (wchar_t*)alloca(sizeof(wchar_t)*(length+1));
        wcsncpy(target, (const wchar_t*)chars, length);
        target[length] = L'\0';
        outString = target;
		if (inHeap)
			delete[] target;
    }

#else  // _WIN32

    // On Linux, wchar_t is 32 bits (UTF-32), and XMLCh is 16-bits, so we need to convert:
    if (localLength == 0)
        localLength = XERCES_CPP_NAMESPACE::XMLString::stringLen(chars);
    UTF16 *source = (UTF16*)chars;
    UTF32 *targetStart = (UTF32*)alloca(sizeof(UTF32) * (localLength + 1));
    UTF32 *target = targetStart;  // since target will get modified by the following call
    if (conversionOK != ConvertUTF16toUTF32((const UTF16**)&source, source+localLength, &target, target+localLength, lenientConversion))
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_50_XMLTRANSCODEFAILED),
                L"FdoXmlUtilXrcs::Xrcs2Unicode",
                L"XMLString::transcode"
            )
        );
    targetStart[localLength] = 0;  // apply null terminator
    outString = (wchar_t*)targetStart;

#endif // _WIN32

    return outString;
}

XMLCh* FdoXmlUtilXrcs::Unicode2Xrcs( FdoString* chars )
{
#ifdef _WIN32
    // On Windows, wchar_t is 16 bits (UTF-16), and XMLCh is 16-bits, so no conversion necessary:
    XERCES_CPP_NAMESPACE::MemoryManager* const xercsMemoryManager = XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgMemoryManager;
    XMLCh* xBuffer = NULL;
    if (chars == NULL || chars[0] == '\0')
    {
        xBuffer = (XMLCh*)xercsMemoryManager->allocate(sizeof(XMLCh));
        xBuffer[0] = 0;
    }
    else
    {
        size_t length = wcslen(chars);
        xBuffer = (XMLCh*)xercsMemoryManager->allocate(sizeof(XMLCh) * (length + 1));
        wcsncpy(xBuffer, chars, length);
        xBuffer[length] = 0;
    }
#else
    FdoStringP tempString(chars);
    XMLCh* xBuffer = XERCES_CPP_NAMESPACE::XMLString::transcode((const char*) tempString);
#endif

    return xBuffer;
}


