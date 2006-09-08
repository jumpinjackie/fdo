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

#include "UnitTest.h"
#include "RfpTestThorough.h"

RfpTestThorough::RfpTestThorough(void)
{
}

RfpTestThorough::~RfpTestThorough(void)
{
}

void RfpTestThorough::_setUp()
{
/*
	m_connection = CreateConnection();
	FDO_CPPUNIT_ASSERT(m_connection != NULL);
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
	m_connection->SetConfiguration(stream);
	m_connection->Open();
*/
}

void RfpTestThorough::_tearDown()
{
}

void RfpTestThorough::testCreateConnection()
{
	// try to create connection twice or more times.
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FDO_CPPUNIT_ASSERT(connection != NULL);
	connection = CreateConnection();
	FDO_CPPUNIT_ASSERT(connection != NULL);
	
	FdoPtr<FdoIConnection> connection2 = CreateConnection();
	FDO_CPPUNIT_ASSERT(connection2 != NULL);

	FDO_CPPUNIT_ASSERT(connection.p != connection2.p);
}

void RfpTestThorough::testSetConfiguration()
{	
	// test with NULL pointer.
	{
		FdoPtr<FdoIConnection> connection = CreateConnection();
		FDO_CPPUNIT_ASSERT(connection != NULL);
		connection->SetConfiguration(NULL);
		connection->Open();
		connection->Close();
	}

	// set configuration twice or more.
	{
		FdoPtr<FdoIConnection> connection = CreateConnection();
		FDO_CPPUNIT_ASSERT (connection != NULL);
		FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
		connection->SetConfiguration(stream);
		connection->SetConfiguration(stream);
		connection->SetConfiguration(stream);
		connection->Open();
		connection->Close();
	}

	// re-SetConfiguration when connection is still open.
	{
		FdoPtr<FdoIConnection> connection = CreateConnection();
		FDO_CPPUNIT_ASSERT (connection != NULL);
		FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
		connection->SetConfiguration(stream);
		connection->Open();
		try 
		{ 
			connection->SetConfiguration(stream); 
		}
		catch(FdoException*){/* Ok, expected exception was caught.*/}
		catch(...) { CPPUNIT_ASSERT_MESSAGE ("FdoException expected but got none.", false); }
		
	}

	//
}

void RfpTestThorough::testOpenConnection()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FDO_CPPUNIT_ASSERT (connection != NULL);
	connection->Open();
	FDO_CPPUNIT_ASSERT (connection->GetConnectionState() == FdoConnectionState_Open);
	connection->Close();
	connection->Open();
	FDO_CPPUNIT_ASSERT (connection->GetConnectionState() == FdoConnectionState_Open);
}

void RfpTestThorough::testCreateCommand()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FDO_CPPUNIT_ASSERT (connection != NULL);
	connection->Open();
	FDO_CPPUNIT_ASSERT (connection->GetConnectionState() == FdoConnectionState_Open);
	
	static const int commands[] = 
	{
		FdoCommandType_Select,
	    FdoCommandType_DescribeSchema,
		FdoCommandType_DescribeSchemaMapping,
		FdoCommandType_GetSpatialContexts,
	};
	for (int i=0; i<sizeof(commands)/sizeof(commands[0]); i++)
	{
		FdoPtr<FdoICommand> cmd = connection->CreateCommand(commands[i]);
		FDO_CPPUNIT_ASSERT (cmd != NULL);
	}

	// the folowing line must cause exception.
	// -1 here is an invalid command.
	try
	{
		connection->CreateCommand(-1);
		CPPUNIT_ASSERT_MESSAGE("'CreateCommand' must throw FdoException: invalid parameters", false);
	}
	catch(FdoException*)
	{
		// FdoException caught, that's ok.
	}	
} 

void RfpTestThorough::testActivateSpatialContext()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

    bool bFail = false;
	try
	{
	    //Activate spatial context
	    FdoPtr<FdoIActivateSpatialContext> activateSC = static_cast<FdoIActivateSpatialContext*>(connection->CreateCommand(FdoCommandType_ActivateSpatialContext));
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (bFail);
}

void RfpTestThorough::testGetSpatialContexts()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	FdoICommand* command = connection->CreateCommand(FdoCommandType_GetSpatialContexts);
    FDO_CPPUNIT_ASSERT (command->GetConnection() == connection.p);
    bool bFail = false;
    try 
    { 
	    command->GetParameterValues(); 
	    CPPUNIT_ASSERT_MESSAGE("GetParameterValue must throw FdoException", false);
    } 
    catch(FdoException*) 
    {
	    /* FdoException was caught. Thatis ok. */
	    bFail = true;
    }
    FDO_CPPUNIT_ASSERT (bFail);
    FDO_CPPUNIT_ASSERT (command->GetTransaction() == NULL);
    bFail = false;
    try 
    { 
	    command->SetCommandTimeout(3); 
	    CPPUNIT_ASSERT_MESSAGE("GetParameterValue must throw FdoException", false);
    }	
    catch(FdoException*) 
    {
	    /* FdoException was caught. Thatis ok. */
	    bFail = true;
    }
    FDO_CPPUNIT_ASSERT (bFail);

    FdoPtr<FdoIGetSpatialContexts> cmd = static_cast<FdoIGetSpatialContexts*>(command);
	FDO_CPPUNIT_ASSERT (cmd->GetActiveOnly() == false);
	
	// There are three spatial contexts.
	FdoPtr<FdoISpatialContextReader> reader = cmd->Execute();

	// read the first spatial context.
	FDO_CPPUNIT_ASSERT (reader->ReadNext() == true);
	FDO_CPPUNIT_ASSERT (STREQUAL(reader->GetName(), L"area1"));
	FDO_CPPUNIT_ASSERT (STREQUAL(reader->GetDescription(), L"Spatial context for Area 1"));
	FDO_CPPUNIT_ASSERT (STREQUAL(reader->GetCoordinateSystem(), L"utm_zone1"));	
	FDO_CPPUNIT_ASSERT (reader->GetExtentType() == FdoSpatialContextExtentType_Static);
	FdoPtr<FdoByteArray> p = reader->GetExtent();
	FdoRfpRect rect = FdoRfpUtil::CreateRectFromGeometryAgf(reader->GetExtent());
	FDO_CPPUNIT_ASSERT (rect.m_maxX == 300000);
	FDO_CPPUNIT_ASSERT (rect.m_minX == 200000);
	FDO_CPPUNIT_ASSERT (rect.m_maxY == 1100000);
	FDO_CPPUNIT_ASSERT (rect.m_minY == 1000000);

	CPPUNIT_ASSERT_DOUBLES_EQUAL (0.0000, reader->GetXYTolerance(), 0.000001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL (0.0000, reader->GetZTolerance(), 0.000001);
	FDO_CPPUNIT_ASSERT (reader->IsActive() == true);

	// only one spatial context.
	FDO_CPPUNIT_ASSERT (reader->ReadNext() == false);
}

void RfpTestThorough::testManySpatialContexts()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"ManySpatialContextTest.xml", L"r");

    bool bFail = false;
	try
	{
	    // Read >1 spatial context
	    connection->SetConfiguration(stream);
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (!bFail);
}

void RfpTestThorough::testCreateSpatialContext()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

    bool bFail = false;
	try
	{
	    //Activate spatial context
	    FdoPtr<FdoICreateSpatialContext> createSC = static_cast<FdoICreateSpatialContext*>(connection->CreateCommand(FdoCommandType_CreateSpatialContext));
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (bFail);
}

void RfpTestThorough::testDestroySpatialContext()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

    bool bFail = false;
	try
	{
	    //Activate spatial context
	    FdoPtr<FdoIDestroySpatialContext> createSC = static_cast<FdoIDestroySpatialContext*>(connection->CreateCommand(FdoCommandType_DestroySpatialContext));
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (bFail);
}

void RfpTestThorough::testCloseConnection()
{
	// No config, no open before close.
	{
		FdoIConnection * connection = CreateConnection();
		connection->Close();
		delete connection;
	}

	// No open before close.
	{
		FdoPtr<FdoIConnection> connection = CreateConnection();
		FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
		connection->SetConfiguration(stream);
		// NOTICE: not call open operation.
		connection->Close();
	}

	// Normal sequence: create -> config -> open -> -> close.
	{
		FdoPtr<FdoIConnection> connection = CreateConnection();
		FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
		connection->SetConfiguration(stream);
		connection->Open();
		connection->Close();
	}

	//TODO: What will happen if "Close" called when performing a long operation?
}

// test connection capabilities.
void RfpTestThorough::testCapabilities()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    // Command capabilities.
    {
        FdoPtr<FdoICommandCapabilities> capabilities = connection->GetCommandCapabilities();
        // RFP do NOT support command parameters.
        FDO_CPPUNIT_ASSERT (capabilities->SupportsParameters() == false );
        // RFP do NOT support command timeout.
        FDO_CPPUNIT_ASSERT (capabilities->SupportsTimeout() == false );

        // verify the commands.		
        static const int commands[] = 
            {
                FdoCommandType_Select,
                FdoCommandType_SelectAggregates,
	        FdoCommandType_DescribeSchema,
                FdoCommandType_DescribeSchemaMapping,
                FdoCommandType_GetSpatialContexts
	    };		
		
        FdoInt32 nCnt;
        FdoInt32* cmds = capabilities->GetCommands(nCnt);
        FDO_CPPUNIT_ASSERT(nCnt == sizeof(commands)/sizeof(commands[0]));

        FDO_CPPUNIT_ASSERT (ArrayEqual(cmds, commands, nCnt));
    }
	
    // connection capabilities.
    {
        FdoInt32 length;
        FdoSpatialContextExtentType* types;

        FdoPtr<FdoIConnectionCapabilities> capabilities = connection->GetConnectionCapabilities();
        types = capabilities->GetSpatialContextTypes(length);
		
        FDO_CPPUNIT_ASSERT (length == 1);
        FDO_CPPUNIT_ASSERT (types[0] == FdoSpatialContextExtentType_Static);
        FDO_CPPUNIT_ASSERT (capabilities->GetThreadCapability() == FdoThreadCapability_SingleThreaded);
        FDO_CPPUNIT_ASSERT (capabilities->SupportsConfiguration() == true );
        FDO_CPPUNIT_ASSERT (capabilities->SupportsLocking() == false );
        FDO_CPPUNIT_ASSERT (capabilities->SupportsLongTransactions() == false );
        FDO_CPPUNIT_ASSERT (capabilities->SupportsSQL() == false );
        FDO_CPPUNIT_ASSERT (capabilities->SupportsTimeout() == false );
        FDO_CPPUNIT_ASSERT (capabilities->SupportsTransactions() == false);		
    }

    // expression capabilities.
    {
        FdoInt32 length;
        FdoPtr<FdoIExpressionCapabilities> capabilities = connection->GetExpressionCapabilities();
        FdoExpressionType* types = capabilities->GetExpressionTypes(length);
        FDO_CPPUNIT_ASSERT (length == 1);
        FDO_CPPUNIT_ASSERT (types[0] == FdoExpressionType_Function);

        FdoPtr<FdoFunctionDefinitionCollection> functions = capabilities->GetFunctions();
        FDO_CPPUNIT_ASSERT(functions != NULL);
        FDO_CPPUNIT_ASSERT(functions->GetCount() == 3);
        FdoPtr<FdoFunctionDefinition> mosaicFunction = functions->GetItem(0);
        FDO_CPPUNIT_ASSERT(STRCASEEQ(mosaicFunction->GetName(), L"MOSAIC"));
        FDO_CPPUNIT_ASSERT(mosaicFunction->GetReturnType() == FdoDataType_BLOB);

        FdoPtr<FdoFunctionDefinition> clipFunction = functions->GetItem(1);
        FDO_CPPUNIT_ASSERT(STRCASEEQ(clipFunction->GetName(), L"CLIP"));
        FDO_CPPUNIT_ASSERT(clipFunction->GetReturnType() == FdoDataType_BLOB);
        
        FdoPtr<FdoReadOnlyArgumentDefinitionCollection> args = mosaicFunction->GetArguments();
        FDO_CPPUNIT_ASSERT(args->GetCount() == 1);
        FdoPtr<FdoArgumentDefinition> rasterArg = args->GetItem(0);
        FDO_CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"raster"));
        FDO_CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_BLOB);

        FdoPtr<FdoFunctionDefinition> resampleFunction = functions->GetItem(2);
        FDO_CPPUNIT_ASSERT(STRCASEEQ(resampleFunction->GetName(), L"RESAMPLE"));
        FDO_CPPUNIT_ASSERT(resampleFunction->GetReturnType() == FdoDataType_BLOB);

        args = resampleFunction->GetArguments();
        FDO_CPPUNIT_ASSERT(args->GetCount() == 7);
        rasterArg = args->GetItem(0);
        FDO_CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"raster"));
        FDO_CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_BLOB);
    }

    // filter capabilities.
    {
        FdoInt32 nTypes;
        FdoPtr<FdoIFilterCapabilities> capabilities = connection->GetFilterCapabilities();
		
        {
            FdoConditionType * types;			
            types = capabilities->GetConditionTypes(nTypes);
            FDO_CPPUNIT_ASSERT (nTypes == 2);
            FDO_CPPUNIT_ASSERT (types[0] == FdoConditionType_In);
            FDO_CPPUNIT_ASSERT (types[1] == FdoConditionType_Spatial);
        }

        {
            FdoSpatialOperations* operations = capabilities->GetSpatialOperations(nTypes);
            FDO_CPPUNIT_ASSERT (nTypes == 4);
            int flag = 0;
            for (int i=0; i<4; i++)
            {
                if (operations[i] == FdoSpatialOperations_Intersects)
                    flag += 0x1;
                else if (operations[i] == FdoSpatialOperations_Within)
                    flag += 0x2;
                else if (operations[i] == FdoSpatialOperations_Inside)
                    flag += 0x4;
                else if (operations[i] == FdoSpatialOperations_EnvelopeIntersects)
                    flag += 0x8;

            }
            FDO_CPPUNIT_ASSERT (flag == 0xf);
        }
		
        {
            FDO_CPPUNIT_ASSERT (capabilities->GetDistanceOperations(nTypes) == NULL);
            FDO_CPPUNIT_ASSERT (nTypes == 0);
        }
	

        FDO_CPPUNIT_ASSERT (capabilities->SupportsGeodesicDistance() == false );
        FDO_CPPUNIT_ASSERT (capabilities->SupportsNonLiteralGeometricOperations() == false);		
    }

    // raster capabilites.
    {
        FdoPtr<FdoIRasterCapabilities> capabilities = connection->GetRasterCapabilities();
        FDO_CPPUNIT_ASSERT (capabilities->SupportsRaster() == true);
        FDO_CPPUNIT_ASSERT (capabilities->SupportsStitching() == false);
        FDO_CPPUNIT_ASSERT (capabilities->SupportsSubsampling() == true);
		
        // Tests for "SupportsDataModel".		
        FdoPtr<FdoRasterDataModel> dm = FdoRasterDataModel::Create();
        dm->SetBitsPerPixel(8);
        dm->SetDataModelType( FdoRasterDataModelType_Gray );
        dm->SetDataType(FdoRasterDataType_UnsignedInteger);

        {
            dm->SetOrganization(FdoRasterDataOrganization_Row);
            FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
        }
        {
            dm->SetOrganization(FdoRasterDataOrganization_Image);
            FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
        }

        {
            dm->SetOrganization(FdoRasterDataOrganization_Pixel);

            FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
            {
                // bitional
                dm->SetDataModelType(FdoRasterDataModelType_Bitonal);
                dm->SetBitsPerPixel(1);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // gray
                dm->SetDataModelType(FdoRasterDataModelType_Gray);
                dm->SetBitsPerPixel(8);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(24);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // palette	
                dm->SetDataModelType(FdoRasterDataModelType_Palette);
                dm->SetBitsPerPixel(8);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
                dm->SetBitsPerPixel(24);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // rgb
                dm->SetDataModelType(FdoRasterDataModelType_RGB);
                dm->SetBitsPerPixel(24);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(32);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // rgba
                dm->SetDataModelType(FdoRasterDataModelType_RGBA);
                dm->SetBitsPerPixel(32);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(24);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // signed integer.
                dm->SetDataModelType(FdoRasterDataModelType_Gray);
                dm->SetBitsPerPixel(16);
                dm->SetDataType(FdoRasterDataType_Integer);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(32);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(8);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // unsigned integer.
                dm->SetDataModelType(FdoRasterDataModelType_Gray);
                dm->SetBitsPerPixel(16);
                dm->SetDataType(FdoRasterDataType_UnsignedInteger);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(32);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(8);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(64);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // float
                dm->SetDataModelType(FdoRasterDataModelType_Gray);
                dm->SetBitsPerPixel(32);
                dm->SetDataType(FdoRasterDataType_Float);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(64);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(128);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // we also support RGB for these various data types.
                dm->SetDataModelType(FdoRasterDataModelType_RGB);
                dm->SetBitsPerPixel(96);
                dm->SetDataType(FdoRasterDataType_Float);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(192);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(128);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
                dm->SetDataType(FdoRasterDataType_Integer);
                dm->SetBitsPerPixel(48);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(96);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(24);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
                dm->SetDataType(FdoRasterDataType_UnsignedInteger);
                dm->SetBitsPerPixel(48);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(96);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(192);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
            {
                // we also support RGBA for these various data types.
                dm->SetDataModelType(FdoRasterDataModelType_RGBA);
                dm->SetBitsPerPixel(128);
                dm->SetDataType(FdoRasterDataType_Float);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(256);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(64);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
                dm->SetDataType(FdoRasterDataType_Integer);
                dm->SetBitsPerPixel(64);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(128);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(32);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
                dm->SetDataType(FdoRasterDataType_UnsignedInteger);
                dm->SetBitsPerPixel(64);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(128);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == true);
                dm->SetBitsPerPixel(256);
                FDO_CPPUNIT_ASSERT (capabilities->SupportsDataModel(dm) == false);
            }
        }
    }

    // schema capabilities.
    {
        FdoPtr<FdoISchemaCapabilities> capabilities = connection->GetSchemaCapabilities();
        FDO_CPPUNIT_ASSERT (capabilities->SupportsSchemaOverrides() == true);
        FDO_CPPUNIT_ASSERT (capabilities->SupportsObjectProperties() == false);
        FDO_CPPUNIT_ASSERT (capabilities->SupportsNetworkModel() == false);
        FDO_CPPUNIT_ASSERT (capabilities->SupportsMultipleSchemas() == true);		
        FDO_CPPUNIT_ASSERT (capabilities->SupportsInheritance() == true);
        FDO_CPPUNIT_ASSERT (capabilities->SupportsAssociationProperties() == false);
		
        FdoInt32 nclsTypes, ndataTypes;
        FdoClassType* clstypes = capabilities->GetClassTypes(nclsTypes);
        FdoDataType* datatypes = capabilities->GetDataTypes(ndataTypes);
        FDO_CPPUNIT_ASSERT ((nclsTypes == 1) && (clstypes[0] == FdoClassType_FeatureClass));		
        FDO_CPPUNIT_ASSERT (ndataTypes == 2);
    }
}

void RfpTestThorough::testBeginTransaction()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();
	connection->BeginTransaction();	
}

void RfpTestThorough::testGetSetConnectionTimeout()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");
	connection->SetConfiguration(stream);
	connection->Open();

	bool bFail = false;
	try
	{
		connection->GetConnectionTimeout();
	}
	catch(FdoException*)
	{
		bFail = true;
	}
	FDO_CPPUNIT_ASSERT (bFail);
	bFail = false;
	try
	{
		connection->SetConnectionTimeout(10);
	}
	catch(FdoException*)
	{
		bFail = true;
	}
	FDO_CPPUNIT_ASSERT (bFail);
}

void RfpTestThorough::testCreateSchemaMapping()
{
	//TODO: not finished...
}

void RfpTestThorough::testOpenClose()
{
	FdoPtr<FdoIConnection> connection = CreateConnection();
	connection->SetConnectionString(L"DefaultRasterFileLocation=src/UnitTest/");
	connection->SetConnectionString(L"DefaultRasterFileLocation=src/UnitTest/"); // Should pass
	FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"thoroughTest.xml", L"r");

    connection->Open();

    bool bFail = false;

	try
	{
	    connection->Open(); // should fail
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (bFail);

    bFail = false;

	try
	{
	    connection->SetConnectionString(L"DefaultRasterFileLocation=src/UnitTest/"); // should fail
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (bFail);

    bFail = false;

	try
	{
	    connection->SetConfiguration(stream); // should fail
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (bFail);

    connection->Close(); // should pass
    connection->Close(); // should pass
	connection->Open(); // should pass

    connection->Close(); // should pass
	connection->SetConnectionString(L"DefaultRasterFileLocation=src/UnitTest/"); // should pass
	connection->Open(); // should pass

    connection->Close(); // should pass
	connection->SetConfiguration(stream); // should fail
	connection->Open(); // should pass

    FdoPtr<FdoIConnectionInfo> info = connection->GetConnectionInfo(); // should pass
	FdoPtr<FdoIConnectionPropertyDictionary> dictionary = info->GetConnectionProperties();
    FdoStringP propertyValue = dictionary->GetProperty(L"DefaultRasterFileLocation");

    bFail = false;

	try
	{
	    dictionary->SetProperty(L"DefaultRasterFileLocation", propertyValue); // should fail
	}
	catch(FdoException*)
	{
		bFail = true;
	}

	FDO_CPPUNIT_ASSERT (bFail);

    connection->Close(); // should pass
}
