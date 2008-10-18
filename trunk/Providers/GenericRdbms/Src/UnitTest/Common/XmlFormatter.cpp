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

#include "Pch.h"
#include "XmlFormatter.h"

XmlFormatter::XmlFormatter(FdoIoStream* inStream, FdoIoStream* outStream, int elementIndent)
{
    mInStream = inStream;
    mOutStream = outStream;
    mIndent = elementIndent;
    mLevel = 0;
}

XmlFormatter::~XmlFormatter(void)
{
}

void XmlFormatter::Format()
{
    FdoByte inBuffer[4096];
    FdoByte outBuffer[8192];
    FdoSize len = 0;
    FdoSize inIdx = 0;
    FdoSize outIdx = 0;
    FdoByte lf = '\n';
    FdoByte cr = '\r';

    while ( (len = mInStream->Read( inBuffer, 4096 )) > 0 ) {
        for ( inIdx = 0, outIdx = 0; inIdx < len; inIdx++ ) {
            if ( (inBuffer[inIdx] != lf) && (inBuffer[inIdx] != cr) )
                outBuffer[outIdx++] = inBuffer[inIdx];
            if ( inBuffer[inIdx] == '>' )
                outBuffer[outIdx++] = lf;
        }

        mOutStream->Write( outBuffer, outIdx );
    }
}
