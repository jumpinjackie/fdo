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

FdoXmlSaxContext::FdoXmlSaxContext( FdoXmlReader* reader )
{
    FDO_SAFE_ADDREF(reader);
    mpReader = reader;
    mErrors = Errors::Create();
}

FdoXmlSaxContext::~FdoXmlSaxContext(void)
{
    FDO_SAFE_RELEASE(mpReader);
}

FdoXmlSaxContext* FdoXmlSaxContext::Create( FdoXmlReader* reader )
{
    return new FdoXmlSaxContext( reader );
}

FdoXmlReader* FdoXmlSaxContext::GetReader()
{
    FDO_SAFE_ADDREF(mpReader);

    return mpReader;
}

void FdoXmlSaxContext::AddError( FdoException* ex )
{
    mErrors->Add( ex );
}

void FdoXmlSaxContext::ThrowErrors()
{
    FdoPtr<FdoException> ex;
    FdoPtr<FdoException> nextEx;
    FdoInt32 i;

    // Chain the exceptions together
    for ( i = (mErrors->GetCount() - 1); i >= 0; i-- ) {
        ex = mErrors->GetItem(i);
        ex->SetCause(nextEx);
        nextEx = ex;
    }

    // Throw the first exception.
    if ( ex != NULL ) {
        FdoException* pEx = ex;
        FDO_SAFE_ADDREF(pEx);
        throw pEx;
    }
}

void FdoXmlSaxContext::Errors::Dispose()
{
    delete this;
}


