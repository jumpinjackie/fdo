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
 * Revision Control Modification History
 *
 *         $Id: //providers_reliant/TestCommon/src/TestCommonFeatureCommands.cpp#1 $
 *     $Author: dalcoup $
 *   $DateTime: 2006/09/25 15:47:54 $
 *     $Change: 11018 $
 *
 */

#include "stdafx.h"
#include "TestCommonFeatureCommands.h"
#include "TestCommonMiscUtil.h"


void TestCommonFeatureCommands::duplicateComputedIdTest (FdoIConnection* conn, FdoString* className, FdoString* propName)
{
	FdoPtr<FdoIFeatureReader> myReader;
    wchar_t buff[256];

    try
    {
    	FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(className);
        FdoCommonOSUtil::swprintf(buff, ELEMENTS(buff), L"(%ls*2) AS %ls", propName, propName);  // same name as real property/column!
        FdoPtr<FdoComputedIdentifier> cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(buff);
        FdoPtr<FdoIdentifierCollection> idCol = selCmd->GetPropertyNames();
        idCol->Add( cmpId );
        FdoPtr<FdoIdentifier> segId = FdoIdentifier::Create(propName);
        try
        {
            idCol->Add( segId );
            CPPUNIT_ASSERT_MESSAGE("Expected exception due to duplicate ids but didn't receive one.", false);
        }
        catch (FdoException *e)
        {
            // We expect to get an exception here
            e->Release();
        }
    }
    catch( FdoException *ex )
    {
        TestCommonFail(ex);
    }

    try
    {
    	FdoPtr<FdoISelectAggregates> selAggrCmd = (FdoISelectAggregates*)conn->CreateCommand( FdoCommandType_SelectAggregates );
        selAggrCmd->SetFeatureClassName(className);
        FdoCommonOSUtil::swprintf(buff, ELEMENTS(buff), L"(%ls*2) AS %ls", propName, propName);  // same name as real property/column!
        FdoPtr<FdoComputedIdentifier> cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(buff);
        FdoPtr<FdoIdentifierCollection> idCol = selAggrCmd->GetPropertyNames();
        idCol->Add( cmpId );
        FdoPtr<FdoIdentifier> segId = FdoIdentifier::Create(propName);
        try
        {
            idCol->Add( segId );
            CPPUNIT_ASSERT_MESSAGE("Expected exception due to duplicate ids but didn't receive one.", false);
        }
        catch (FdoException *e)
        {
            // We expect to get an exception here
            e->Release();
        }
    }
    catch( FdoException *ex )
    {
        TestCommonFail(ex);
    }
}

#pragma warning(push)
#pragma warning( disable : 4189 )

void TestCommonFeatureCommands::secondComputedIdTest (FdoIConnection* conn, FdoString* className, FdoString* numericPropName)
{
	FdoPtr<FdoIFeatureReader>myReader;
	FdoPtr<FdoISelect>selCmd;

    if( conn != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(className);
            wchar_t buff[256];
            FdoCommonOSUtil::swprintf(buff, ELEMENTS(buff), L"(%ls*2) AS acomputedprop", numericPropName);
            FdoPtr<FdoComputedIdentifier> cmpId = (FdoComputedIdentifier*)FdoExpression::Parse(buff);
            FdoPtr<FdoIdentifierCollection> idCol = selCmd->GetPropertyNames();
            FdoPtr<FdoIdentifier> segId = FdoIdentifier::Create(numericPropName);
            idCol->Add( segId );
            idCol->Add( cmpId );  // NOTE: the computed id needs to come second to reproduce defect 817232 
            myReader = selCmd->Execute();

            if( myReader != NULL  )
            {
                FdoPtr<FdoClassDefinition> cls = myReader->GetClassDefinition();
                CPPUNIT_ASSERT_MESSAGE("Class should be computed", cls->GetIsComputed());
                FdoPtr<FdoPropertyDefinitionCollection> props = cls->GetProperties();
                FdoPtr<FdoPropertyDefinition> compProp = props->FindItem(L"acomputedprop");
                CPPUNIT_ASSERT_MESSAGE("computed property not found", compProp!=NULL);

                while ( myReader->ReadNext() )
                {
                    if (myReader->IsNull(numericPropName))
                        CPPUNIT_ASSERT_MESSAGE("computed prop should also be null", myReader->IsNull(L"acomputedprop"));
                    else
                    {
                        FdoInt32 numvalue = myReader->GetInt32(numericPropName);

                        // NOTE: there seems to be a bug here, the following line causes a heap corruption in Odbc Access
                        //FdoInt32 compvalue = myReader->GetInt32(L"acomputedprop");

                        //FdoStringP numvalueS = myReader->GetString(numericPropName);
                        //FdoStringP compvalueS = myReader->GetString(L"acomputedprop");
                        //FdoInt32 numvalue = numvalueS.ToLong();
                        //FdoInt32 compvalue = compvalueS.ToLong();
                        //CPPUNIT_ASSERT_MESSAGE("incorrect computed value", 2*numvalue == compvalue);
                    }
                }

            }
        }
        catch( FdoException *ex )
        {
            TestCommonFail(ex);
        }
    }
}

#pragma warning(pop)
