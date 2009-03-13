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
#include <cstdio>
#include <cstring>
#include <FdoCommon.h>
#include "xlinks.h"
#include "geometry.h"
#include "feature.h"

FdoXmlReader* FdoGml212Schema::CreateReader()
{
    int                 i;
    FdoIoMemoryStreamP  stream = FdoIoMemoryStream::Create();
    FdoXmlWriterP       writer = FdoXmlWriter::Create(stream);

    // Write the GML and Xlink schemas from static arrays to a stream.
    for ( i = 1; xlinks[i] != NULL; i++ ) 
        writer->WriteBytes( (FdoByte*) xlinks[i], strlen(xlinks[i]) );

    for ( i = 1; geometry[i] != NULL; i++ ) 
        writer->WriteBytes( (FdoByte*) geometry[i], strlen(geometry[i]) );

    for ( i = 1; feature[i] != NULL; i++ ) 
        writer->WriteBytes( (FdoByte*) feature[i], strlen(feature[i]) );

    writer = NULL;
    stream->Reset();

    // return a reader around the stream.
    return( FdoXmlReader::Create(stream) );
}


