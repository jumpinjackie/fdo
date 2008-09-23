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
 */

#include "stdafx.h"

// FdoRdbmsInsertCommand
#include "FdoRdbmsApplySchemaCommand.h"
#include <Fdo/ClientServices/ProviderNameTokens.h>
#include "../../SchemaMgr/Ph/Mgr.h"

// Class FdoRdbmsApplySchemaCommand
/*  Do not implement the copy constructor to avoid alot of problem. BK
FdoRdbmsApplySchemaCommand::FdoRdbmsApplySchemaCommand(const FdoRdbmsApplySchemaCommand &right)
{
}
*/

FdoRdbmsApplySchemaCommand::FdoRdbmsApplySchemaCommand() :
    mIgnoreStates(false)
{
}

FdoRdbmsApplySchemaCommand::FdoRdbmsApplySchemaCommand(FdoIConnection *connection) :
    mIgnoreStates(false)
{
    mRdbmsConnection = FDO_SAFE_ADDREF(static_cast<FdoRdbmsConnection*>(connection));
}

FdoRdbmsApplySchemaCommand::~FdoRdbmsApplySchemaCommand()
{
}

FdoFeatureSchema* FdoRdbmsApplySchemaCommand::GetFeatureSchema()
{
    return FDO_SAFE_ADDREF( (FdoFeatureSchema*) mpSchema);
}

void FdoRdbmsApplySchemaCommand::SetFeatureSchema(FdoFeatureSchema* value)
{
    mpSchema = FDO_SAFE_ADDREF(value);
}

FdoPhysicalSchemaMapping* FdoRdbmsApplySchemaCommand::GetPhysicalMapping()
{
    return FDO_SAFE_ADDREF((FdoPhysicalSchemaMapping*) mpSchemaMappings);
}

void FdoRdbmsApplySchemaCommand::SetPhysicalMapping(FdoPhysicalSchemaMapping* value)
{
    // If mapping overrides supplied, make sure they are not too new for this
    // provider.
    if ( value )
    {
        FdoPhysicalSchemaMappingP providerMappings = mRdbmsConnection->CreateSchemaMapping();

        if ( !providerMappings ) 
            // TODO: proper exception message (Provider does not support schema overrides)
            throw FdoCommandException::Create();

        // Get the provider name for the mappings and current connection.
        FdoString* mappingProvider =  value->GetProvider();

        FdoString* connectionProvider = providerMappings->GetProvider();

        // Tokenize the provider names so that versions can be compared
        FdoProviderNameTokensP mappingTokens = FdoProviderNameTokens::Create( mappingProvider );
        FdoProviderNameTokensP connectionTokens = FdoProviderNameTokens::Create( connectionProvider );

        if ( mappingTokens->GetNameTokens()->ToString() != connectionTokens->GetNameTokens()->ToString() ) 
            // TODO: proper exception message (wrong type of schema overrides for provider)
            throw FdoCommandException::Create();


        if ( !( mappingTokens <= connectionTokens ) ) {
            // mapping overrides version is later than that of this connection so
            // this connection cannot handle them.
            throw FdoCommandException::Create();
/* TODO proper message
            NlsMsgGet2(
                    FDORDBMS_299,
                    "Connection for provider '%1$ls' cannot process Schema Mapping Set for provider '%2$ls' since it has a future version",
                    connectionProvider,
                    mappingProvider
                )
            );
*/
        }
    }

    mpSchemaMappings = FDO_SAFE_ADDREF(value);
}

FdoBoolean FdoRdbmsApplySchemaCommand::GetIgnoreStates()
{
    return mIgnoreStates;
}

void FdoRdbmsApplySchemaCommand::SetIgnoreStates( FdoBoolean ignoreStates )
{
    mIgnoreStates = ignoreStates;
}

void FdoRdbmsApplySchemaCommand::Execute()
{

    if( ((FdoRdbmsConnection*) NULL) == mRdbmsConnection )
    {
        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    }

    if( ((FdoFeatureSchema*) NULL) == mpSchema )
    {
        throw FdoSchemaException::Create(
            NlsMsgGet( FDORDBMS_168, "Cannot apply schema, no schema was supplied" )
        );
    }

    FdoSchemaManagerP      pSchemaManager = mRdbmsConnection->CreateSchemaManager();

    pSchemaManager->ApplySchema( mpSchema, mpSchemaMappings, mIgnoreStates );
}
