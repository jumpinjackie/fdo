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
#include <Common.h>
#include <Common/Xsl/TransformerXalan.h>
#include <stdio.h>

FdoXslTransformer::FdoXslTransformer( 
    FdoXmlReader* inDoc, 
    FdoXmlReader* stylesheet, 
    FdoXmlWriter* outDoc,
    FdoIoTextWriter* log
)
{
    SetInDoc(inDoc);
    SetStylesheet(stylesheet);
    SetOutDoc(outDoc);
    SetLog(log);

    mParameters = FdoDictionary::Create();
}

FdoXslTransformer* FdoXslTransformer::Create( 
    FdoXmlReader* inDoc, 
    FdoXmlReader* stylesheet, 
    FdoXmlWriter* outDoc,
    FdoIoTextWriter* log
)
{
    return( FdoXslTransformerXalan::Create( inDoc, stylesheet, outDoc, log ) );
}

FdoXmlReader* FdoXslTransformer::GetInDoc()
{
    FdoXmlReader* pReader = mInDoc;
    pReader->AddRef();
    return pReader;
}

void FdoXslTransformer::SetInDoc( FdoXmlReader* inDoc )
{
    if ( !inDoc ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"inDoc",
                L"(NULL)",
                L"FdoXslTransformer::SetInDoc"
            )
        );

    inDoc->AddRef();
    mInDoc = inDoc;
}

FdoXmlReader* FdoXslTransformer::GetStylesheet()
{
    FdoXmlReader* pReader = mStylesheet;
    pReader->AddRef();
    return pReader;
}

void FdoXslTransformer::SetStylesheet( FdoXmlReader* stylesheet )
{
    if ( !stylesheet ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"stylesheet",
                L"(NULL)",
                L"FdoXslTransformer::SetStylesheet"
            )
        );

    stylesheet->AddRef();
    mStylesheet = stylesheet;
}

FdoXmlWriter* FdoXslTransformer::GetOutDoc()
{
    FdoXmlWriter* pWriter = mOutDoc;
    pWriter->AddRef();
    return pWriter;
}

void FdoXslTransformer::SetOutDoc( FdoXmlWriter* outDoc )
{
    if ( !outDoc ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"outDoc",
                L"(NULL)",
                L"FdoXslTransformer::SetOutDoc"
            )
        );

    outDoc->AddRef();
    mOutDoc = outDoc;
}

FdoIoTextWriter* FdoXslTransformer::GetLog()
{
    return FDO_SAFE_ADDREF(mLog.p);
}

void FdoXslTransformer::SetLog( FdoIoTextWriter* log )
{
    mLog = FDO_SAFE_ADDREF(log);
}

FdoDictionary* FdoXslTransformer::GetParameters()
{
    FdoDictionary* pParameters = mParameters;
    pParameters->AddRef();
    return pParameters;
}

