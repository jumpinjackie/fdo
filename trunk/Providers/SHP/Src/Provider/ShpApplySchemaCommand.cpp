/*
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
 */

#include "stdafx.h"
#include "ShpProvider.h"
#include "ShpSchemaUtilities.h"

#include <FdoCommonStringUtil.h>

ShpApplySchemaCommand::ShpApplySchemaCommand (FdoIConnection *connection) :
    FdoCommonCommand<FdoIApplySchema, ShpConnection> (connection),
    mIgnoreStates (false)
{
}

/** Do not implement the copy constructor. **/
//ShpApplySchemaCommand::ShpApplySchemaCommand (const ShpApplySchemaCommand &right) { }

ShpApplySchemaCommand::~ShpApplySchemaCommand (void)
{
}

/** Do not implement the assignment operator. **/
//ShpApplySchemaCommand & operator= (const ShpApplySchemaCommand &right);

///<summary>Gets the schema to create.</summary>
/// <returns>Returns the schema that would be created.</returns> 
FdoFeatureSchema* ShpApplySchemaCommand::GetFeatureSchema ()
{
    return (FDO_SAFE_ADDREF (mSchema.p));
}

///<summary>Sets the schema to create.</summary>
/// <param name="value">Input the definition of the schema to create.</param> 
/// <returns>Returns nothing</returns> 
void ShpApplySchemaCommand::SetFeatureSchema (FdoFeatureSchema* value)
{
    mSchema = FDO_SAFE_ADDREF (value);
}

/// <summary> Gets the FdoPhysicalSchemaMapping used to specify how the schema definition
/// gets mapped into physical storage.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* ShpApplySchemaCommand::GetPhysicalMapping ()
{
    return (FDO_SAFE_ADDREF(mSchemaMappings.p));
}

///<summary>Sets the FdoPhysicalSchemaMapping used to specify how the schema definition
///gets mapped into physical storage.</summary>
/// <param name="value">Input the FdoPhysicalSchemaMapping</param> 
/// <returns>Returns nothing</returns> 
void ShpApplySchemaCommand::SetPhysicalMapping (FdoPhysicalSchemaMapping* value)
{
    if (value) {
        FdoStringsP sProviderIds = FdoStringCollection::Create(value->GetProvider(), L".");
        if (sProviderIds->GetCount() < 3) {
            throw FdoException::Create(
                NlsMsgGet(SHP_INVALID_PHYSICAL_SCHEMA_PROVIDER_NAME, "Invalid Schema Mapping Provider Name"));
        }

        FdoStringP sAuthor = FdoStringElementP(sProviderIds->GetItem(0))->GetString();
        FdoStringP sName = FdoStringElementP(sProviderIds->GetItem(1))->GetString();
        FdoStringP sVersion = FdoStringElementP(sProviderIds->GetItem(2))->GetString();
        FdoInt32 iVersion = FdoCommonOSUtil::wtoi(sVersion);

        if (sAuthor != SHP_PROVIDER_AUTHOR || sName != SHP_PROVIDER_SHORTNAME) {
            throw FdoException::Create(
                NlsMsgGet(SHP_INVALID_PHYSICAL_SCHEMA_PROVIDER_NAME, "Invalid Schema Mapping Provider Name"));
        }

        if (iVersion < 3) {
            throw FdoException::Create(
                NlsMsgGet(SHP_INVALID_PHYSICAL_SCHEMA_PROVIDER_VERSION, "Invalid Schema Mapping Provider Version"));
        }

        mSchemaMappings = FDO_SAFE_ADDREF(static_cast<FdoShpOvPhysicalSchemaMapping*>(value));
    }
    else {
        mSchemaMappings = NULL;
    }
}



/// <summary> Indicates whether Execute() will ignore element states 
/// when applying the feature schema.</summary>
/// <returns>Returns true if elements states will be ignored, false otherwise. 
/// </returns> 
FdoBoolean ShpApplySchemaCommand::GetIgnoreStates()
{
    return (mIgnoreStates);
}

///<summary>Changes the handling of element states by Execute().</summary>
/// <param name="ignoreStates">Input True: Execute() will ignore the 
/// element states on the Feature Schema elements. Instead, it 
/// will additively merge the Feature Schema into the current DataStore.
/// False: Execute() will use the element states to determine whether
/// each element is inserted, modified or deleted.</param> 
/// <returns>Returns nothing</returns> 
void ShpApplySchemaCommand::SetIgnoreStates( FdoBoolean ignoreStates )
{
    mIgnoreStates = ignoreStates;
}


//
// internals
//


void ShpApplySchemaCommand::add_schema ()
{
    FdoPtr<ShpLpFeatureSchemaCollection> lpSchemas = mConnection->GetLpSchemas();

    // NOTE: it is safer to ignore this for now, since most people prefer quiet success if schema already exists:
    //// Verify the schema doesn't already exist:
    //FdoPtr<ShpLpFeatureSchema> oldLpSchema = lpSchemas->FindItem(mSchema->GetName());
    //if (oldLpSchema != NULL)
    //    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_125_SCHEMA_ALREADY_EXISTS), mSchema->GetName()));

    // Create new Lp Schema (adds itself automatically to the LpSchemas collection):
    FdoPtr<ShpLpFeatureSchema> newLpSchema = new ShpLpFeatureSchema(lpSchemas, mConnection, NULL, mSchema, mSchemaMappings, GetIgnoreStates());
}

void ShpApplySchemaCommand::modify_schema ()
{
    // Get existing Lp Schema:
    FdoPtr<ShpLpFeatureSchemaCollection> lpSchemas = mConnection->GetLpSchemas();
    FdoPtr<ShpLpFeatureSchema> lpSchema = lpSchemas->GetItem(mSchema->GetName());

    //  Modify the schema according to given information:
    lpSchema->Modify(mConnection, mSchema, mSchemaMappings, GetIgnoreStates ());
}

void ShpApplySchemaCommand::delete_schema ()
{
    // Get existing Lp Schema:
    FdoPtr<ShpLpFeatureSchemaCollection> lpSchemas = mConnection->GetLpSchemas();
    FdoPtr<ShpLpFeatureSchema> lpSchema = lpSchemas->GetItem(mSchema->GetName());

    // Delete the Lp Schema (removes itself automatically from the LpSchemas collection):
    lpSchema->Delete(mConnection);
}

///<summary>Executes the ApplySchema command that creates metadata and physical
///storage for the schema. An exception is thrown if the schema already
///exists or if a schema already exists and the feature provider only
///supports a single schema. If schema changes include deletion of classes
///or the schema itself, then those classes (or the schema, if it is marked
///as deleted) must not contain any instance data. If they do, an exception
///will be thrown.</summary>
/// <returns>Returns nothing</returns> 
void ShpApplySchemaCommand::Execute ()
{
    FdoSchemaElementState state;
    ShpLpFeatureSchemaCollectionP lp_schemas;
    FdoPtr<ShpLpFeatureSchema> schema;

    if ((mSchema == NULL) || (NULL == mSchema->GetName ()) || 0 == wcslen (mSchema->GetName ()))
        throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_NOT_SPECIFIED, "No schema specified for the apply schema command."));

    if (mConnection->IsConfigured ())
        throw FdoException::Create (NlsMsgGet(SHP_APPLY_CONFIGURED, "Apply schema after configuration or override is not allowed."));

    if (NULL != mConnection->GetFile ())
        throw FdoException::Create (NlsMsgGet(SHP_APPLY_SINGLE_SHP, "Apply schema on a single shp is not allowed."));

    // Handle IgnoreStates flag:
    state = mSchema->GetElementState ();
    if (GetIgnoreStates ())
    {
       lp_schemas = mConnection->GetLpSchemas ();
       schema = lp_schemas->FindItem (mSchema->GetName ());
       if (schema == NULL)
           state = FdoSchemaElementState_Added;
       else
           state = FdoSchemaElementState_Modified;
    }

    switch (state)
    {
        // Indicates the schema element has been added.
        case FdoSchemaElementState_Added:
            add_schema ();
            break;

        // Indicates the schema element is marked for deletion.
        case FdoSchemaElementState_Deleted:
            delete_schema ();
            break;

        // Indicates the schema element is not associated with an FdoFeatureSchema.
        case FdoSchemaElementState_Detached:
            // duh, waddo I do
            break;

        // Indicates the schema element has been modified in some way.
        case FdoSchemaElementState_Modified:
            modify_schema ();
            break;

        // Indicates the schema element has not been changed.
        case FdoSchemaElementState_Unchanged:
            break;

        default:
            throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_UNSUPPORTED_ELEMENT_STATE, "Schema element state '%1$d' is not supported.", (int)mSchema->GetElementState()));
    }

    // mark the schema as "unchanged"
    mSchema->AcceptChanges ();

	//*************************************************
	// we don't generate any more the schema.xml (per MAP request)
	//*************************************************

    // write the schema to the schema.xml file
    //wchar_t* file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (mConnection->GetDirectory ()) + wcslen (DEFAULT_SCHEMA_XML) + 1));
    //wcscpy (file, mConnection->GetDirectory ());
    //wcscat (file, DEFAULT_SCHEMA_XML);
    //FdoPtr <FdoXmlWriter> writer = FdoXmlWriter::Create (file, true, FdoXmlWriter::LineFormat_Indent, 128);

    //FdoPtr<FdoIDescribeSchema> describe_schema = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
    //FdoPtr<FdoFeatureSchemaCollection> schemas = describe_schema->Execute ();
    //schemas->WriteXml (writer, NULL);
    //FdoPtr<FdoIDescribeSchemaMapping> describe_mapping = (FdoIDescribeSchemaMapping*)mConnection->CreateCommand (FdoCommandType_DescribeSchemaMapping);
    //// describe_mapping->SetIncludeDefaults (true);
    //FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = describe_mapping->Execute ();
    //if (0 != mappings->GetCount ())
    //    mappings->WriteXml (writer, NULL);
}


