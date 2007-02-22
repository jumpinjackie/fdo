#include "UnitTestUtil.h"
#include "FdoCommonFile.h"
#include "SDF/SdfCommandType.h"
#include "SDF/ICreateSDFFile.h"
#ifndef _WIN32
#include <unistd.h>
#endif

 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

FdoIConnection* UnitTestUtil::OpenConnection( FdoString* fileName, bool re_create )
{
#ifdef _WIN32
	wchar_t fullpath[1024];
	_wfullpath(fullpath, fileName, 1024);
#else
	char cpath[1024];
	char cfullpath[1024];
	wcstombs(cpath, fileName, 1024);
	realpath(cpath, cfullpath);
	wchar_t fullpath[1024];
	mbstowcs(fullpath, cfullpath, 1024);
#endif

    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    FdoIConnection *conn = manager->CreateConnection (L"OSGeo.SDF.3.3");
	if( re_create )
	{
        FdoCommonFile::Delete(fileName, true);

		FdoPtr<FdoICreateSDFFile> crsdf = (FdoICreateSDFFile*)(conn->CreateCommand(SdfCommandType_CreateSDFFile));

		crsdf->SetCoordinateSystemWKT(L"[LL84]");
		crsdf->SetFileName(fullpath);
		crsdf->SetSpatialContextDescription(L"World Coordinate System, Degrees, what else do you need to know?");
		crsdf->SetSpatialContextName(L"World Geodetic Coordinate System, 1984");
		crsdf->SetXYTolerance(17.0);
		crsdf->SetZTolerance(3.14159);

		crsdf->Execute();

	}
    std::wstring connStr = std::wstring(L"File=") + std::wstring(fullpath);
    conn->SetConnectionString(connStr.c_str());
    FdoPtr<FdoIConnectionInfo>info = conn->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> prop = info->GetConnectionProperties();
    conn->Open();

    return conn;
}

// Exports spatial contexts and feature schemas from datastore to XML.
void UnitTestUtil::ExportDb( 
    FdoIConnection* connection, 
    FdoIoStream* stream, 
    FdoXmlSpatialContextFlags* flags
)
{
    stream->Reset();
    FdoXmlWriterP writer = FdoXmlWriter::Create(stream, true, FdoXmlWriter::LineFormat_Indent);
/*
    // Serialize the spatial contexts
    FdoXmlSpatialContextSerializer::XmlSerialize( 
        connection,
        FdoXmlSpatialContextWriterP(
            FdoXmlSpatialContextWriter::Create(writer,flags)
        ),
        flags
    );
*/
    // Get the feature schemas from the Datastore
  
    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    FdoFeatureSchemasP schemas = pDescCmd->Execute();
    FdoIoMemoryStreamP tempStream = FdoIoMemoryStream::Create();

    // Serialize the feature schemas.
    schemas->WriteXml( tempStream );
    tempStream->Reset();
    FdoXmlReaderP reader = FdoXmlReader::Create( tempStream );

    FdoXmlCopyHandlerP copier = FdoXmlCopyHandler::Create( writer );
    reader->Parse( copier );
}


void UnitTestUtil::PrintException( FdoException* e, FILE* fp, FdoBoolean stripLineNo )
{
    FdoPtr<FdoException> currE = e;
    // Add ref to prevent smart pointer from destroying exception.
    FDO_SAFE_ADDREF(e);

    while ( currE ) {
        FdoStringP message = currE->GetExceptionMessage();
        FdoString* pMessage = message;
        if ( stripLineNo ) {
#ifdef _WIN32
            // The following removes the function and line number info from error messages.
            FdoString* nextPos = pMessage;
            FdoString* nextPos2;
            FdoString* nextPos3;
            FdoString* nextPos4;
            FdoStringP subStr;
            int state = 0;
            bool done = false;

            while ( !done ) {
                switch (state) {

                case 0: // Initial state
                    // Look for cpp file name enclosed in brackets
                    nextPos = wcschr( pMessage, '(' );
                    nextPos2 = nextPos ? wcsstr( nextPos, L".cpp" ) : NULL;
                    nextPos3 = nextPos ? wcschr( nextPos, ')' ) : NULL;
                    state = 1;
                    break;

                case 1: // process next cpp file in brackets
                    if ( (nextPos == NULL) || (nextPos2 == NULL) || (nextPos3 == NULL) ) { 
                        // no more cpp file in brackets
                        state = 2;
                    }
                    else {
                        if ( nextPos2 < nextPos3 ) {
                            // theres another one.
                            
                            // It might be in nested '()' some move ahead
                            // to innermost '(' before file name.
                            while (true) {
                                nextPos4 = wcschr( nextPos + 1, '(' );
                                if ( nextPos4 && (nextPos4 < nextPos2) )
                                    nextPos = nextPos4;
                                else
                                    break;
                            }
                            state = 3;
                        }
                        else {
                            // next ')' is before '.cpp', skip current '()'
                            state = 4;
                        }
                    }
                    break;

                case 2: // end state
                    // print rest of message
                    fprintf( fp, "%ls\n", pMessage );
                    done = true;
                    break;

                case 3: // found '( ... .cpp ... )'
                    // Get part to the left of '( ... .cpp ... )' and print it
                    subStr = FdoStringP(pMessage).Mid(0, nextPos - pMessage);
                    fprintf( fp, "%ls", (FdoString*) subStr );

                    // Move current ptr to right of '( ... .cpp ... )'
                    pMessage = nextPos3 + 1;
                    state = 0;
                    break;

                case 4: // Move on to next '( ... .cpp ... )'..
                    nextPos = wcschr( nextPos3, '(' );
                    nextPos2 = nextPos ? wcsstr( nextPos, L".cpp" ) : NULL;
                    nextPos3 = nextPos ? wcschr( nextPos, ')' ) : NULL;
                    state = 1;
                    break;
                }
            }
#else
		fprintf( fp, " %ls \n", pMessage );
#endif
        }
        else {
            fprintf( fp, "%ls\n", pMessage );
        }

        currE = currE->GetCause();
    }
}

void UnitTestUtil::PrintException( FdoException* e, const char* fileName, FdoBoolean stripLineNo )
{
    FILE* fp = fopen( fileName, "w" );

    if ( !fp ) {
        char buffer[500];
        sprintf( buffer, "Failed to open file %s", fileName );
        CPPUNIT_FAIL( buffer );
    }

    PrintException( e, fp, stripLineNo );

    fclose( fp );
}
