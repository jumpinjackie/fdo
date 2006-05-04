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
#include <Std.h>
#include <Fdo/Commands/CommandType.h>
#include <Fdo/Commands/SpatialContext/ICreateSpatialContext.h>
#include <Fdo/Commands/SpatialContext/IGetSpatialContexts.h>
#include <Fdo/Commands/SpatialContext/ISpatialContextReader.h>
#include <Fdo/Xml/SpatialContextSerializer.h>

void FdoXmlSpatialContextSerializer::XmlDeserialize( 
    FdoIConnection*                 connection, 
	FdoXmlSpatialContextReader*     reader,
    FdoXmlSpatialContextFlags*      flags
)
{
    FdoDictionaryP skipSC = FdoDictionary::Create();
    FdoPtr<FdoIConnectionCapabilities> connCaps = connection->GetConnectionCapabilities();

    if ( flags && flags->GetConflictOption() == FdoXmlSpatialContextFlags::ConflictOption_Skip ) {
        // Skipping sc's that already exist so build of a list of them.
        // Note: this might be slow when the connection has a lot of sc's
        // but there is no command to directly access an sc.
        FdoPtr<FdoIGetSpatialContexts> cmd = (FdoIGetSpatialContexts*) connection->CreateCommand( FdoCommandType_GetSpatialContexts );
        cmd->SetActiveOnly(false);

        FdoPtr<FdoISpatialContextReader> reader = cmd->Execute();

        while ( reader->ReadNext() ) {
            skipSC->Add( 
                FdoDictionaryElementP(
                    FdoDictionaryElement::Create( reader->GetName(), L"" )
                )
            );
        }    
    }

    FdoPtr<FdoICreateSpatialContext> cmd = (FdoICreateSpatialContext*) connection->CreateCommand( FdoCommandType_CreateSpatialContext );

    // Allow updating only if specified by the flags.
    if ( flags && flags->GetConflictOption() == FdoXmlSpatialContextFlags::ConflictOption_Update )
        cmd->SetUpdateExisting( true );
    else
        cmd->SetUpdateExisting( false );

    while ( reader->ReadNext() ) {
        // When conflict option is "Skip", skip any sc's already on the connection.
        // There is a race condition in that this sc might have been added by 
        // someone else, after the skipSC list was built. However, this is not
        // catastrophic since a "spatial context already exists" exception will 
        // be throw, meaning that XmlDeserialize will revert to behaving 
        // as if conflict option were set to "Add".
        if ( !skipSC->Contains(reader->GetName()) ) {
            // Skip the default context if not including it.
            if ( (wcscmp(reader->GetName(), L"Default") != 0) || (flags && flags->GetIncludeDefault())  ) {
                cmd->SetName( reader->GetName() );
                cmd->SetDescription( reader->GetDescription() );
                cmd->SetCoordinateSystem( reader->GetCoordinateSystem() );
 
                // Skip setting WKT if provider can determine it from 
                // coordinate system name.
                if ( !connCaps->SupportsCSysWKTFromCSysName() )
                    cmd->SetCoordinateSystemWkt( reader->GetCoordinateSystemWkt() );

                cmd->SetExtentType( reader->GetExtentType() );
                FdoByteArray* extent = reader->GetExtent();
                cmd->SetExtent( extent );
                extent->Release();
                cmd->SetXYTolerance( reader->GetXYTolerance() );
                cmd->SetZTolerance( reader->GetZTolerance() );

                cmd->Execute();
            }
        }
    }
}

void FdoXmlSpatialContextSerializer::XmlSerialize( 
    FdoIConnection*                 connection, 
	FdoXmlSpatialContextWriter*     writer,
    FdoXmlSpatialContextFlags*      flags
)
{
    FdoPtr<FdoIGetSpatialContexts> cmd = (FdoIGetSpatialContexts*) connection->CreateCommand( FdoCommandType_GetSpatialContexts );
    cmd->SetActiveOnly(false);

    FdoPtr<FdoISpatialContextReader> reader = cmd->Execute();

    while ( reader->ReadNext() ) {
        // Skip default spatial context if not writing it.
        if ( (wcscmp(reader->GetName(), L"Default") != 0) || (flags && flags->GetIncludeDefault())  ) {
            writer->SetName( reader->GetName() );
            writer->SetDescription( reader->GetDescription() );
            writer->SetCoordinateSystem( reader->GetCoordinateSystem() );
            writer->SetCoordinateSystemWkt( reader->GetCoordinateSystemWkt() );
            writer->SetExtentType( reader->GetExtentType() );
            FdoByteArray* extent = reader->GetExtent();
            writer->SetExtent( extent );
            // Note: reader->GetExtent() does not do an AddRef, so no release is done here.
            writer->SetXYTolerance( reader->GetXYTolerance() );
            writer->SetZTolerance( reader->GetZTolerance() );

            writer->WriteSpatialContext();
        }
    }
}


