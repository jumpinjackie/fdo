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
#include "FdoSqlCmdTest.h"
#include "UnitTestUtil.h"

#ifdef _DEBUG
#define  DBG(X)    X
#else
#define  DBG(X)
#endif

#define  SQLCMD_TABLE_NAME  L"FdoSqlTest"

FdoSqlCmdTest::FdoSqlCmdTest(void)
{
#ifdef _WIN32
        _putenv("GVC_TRACE_FILE=d:\\geometrytest.txt");
#else
        putenv("GVC_TRACE_FILE=d:\\geometrytest.txt");
#endif
}

FdoSqlCmdTest::~FdoSqlCmdTest(void)
{

}

void FdoSqlCmdTest::setUp ()
{
    set_provider();
    connect();
}

void FdoSqlCmdTest::tearDown ()
{
    if( mConnection != NULL )
        mConnection->Close();
    mConnection = NULL;
}

void FdoSqlCmdTest::connect ()
{
    try
    {
        mConnection = (FdoIConnection *) UnitTestUtil::GetConnection(L"", true);

    }
    catch (FdoException *ex)
    {
        ( printf("FDO error: %ls\n", ex->GetExceptionMessage()) );
        if( mConnection != NULL )
            mConnection->Close();
        mConnection= NULL;
        ex->Release();
        throw;
    }
}

void FdoSqlCmdTest::CreateTable ()
{
    FdoPtr<FdoISQLCommand> sqlCmd;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            wchar_t *number_type = get_number_type();
            FdoStringP sql = FdoStringP::Format(L"create table %ls (Name varchar(64) not null, id %ls(10,0), amount %ls(38,8), bool_val %ls(1), date_val %ls )", 
                                SQLCMD_TABLE_NAME, number_type, number_type, number_type, get_datetime_type());
            sqlCmd->SetSQLStatement( sql );
            sqlCmd->ExecuteNonQuery();
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }

}

void FdoSqlCmdTest::BasicInsert ()
{
    FdoPtr<FdoISQLCommand> sqlCmd;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            FdoStringP sql = FdoStringP::Format(L"insert into %ls values ('name1',10,2000.3456,1, %ls)", SQLCMD_TABLE_NAME, get_datetime_sys());
            sqlCmd->SetSQLStatement( sql );
            sqlCmd->ExecuteNonQuery();

            // Need to use the following date format: "YYYY-MM-DD-HH24-MI-SS"
            sql = FdoStringP::Format(L"insert into %ls values ('name3',10,2000.3456,1, %ls)", SQLCMD_TABLE_NAME, get_datetime());
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            sqlCmd->ExecuteNonQuery();
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }

}
void FdoSqlCmdTest::BindInsert ()
{
    return;
}

void FdoSqlCmdTest::Delete ()
{
    FdoPtr<FdoISQLCommand> sqlCmd;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            FdoStringP sql = FdoStringP::Format(L"delete from %ls where id > 0", SQLCMD_TABLE_NAME);
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            int colDeleted = sqlCmd->ExecuteNonQuery();

            DBG(printf("Deleted %d columns\n", colDeleted ));
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }

}

static void ProcessSpatialContextReader( FdoISpatialContextReader * reader )
{
    int                         count = 0;

    while ( reader->ReadNext() )
    {
        const wchar_t* sc_name = reader->GetName();
        const wchar_t* cs_name = reader->GetCoordinateSystem();
        const wchar_t* descr = reader->GetDescription();
        const wchar_t* cs_wkt = reader->GetCoordinateSystemWkt();

        FdoPtr<FdoByteArray> extent = reader->GetExtent();
        FdoSpatialContextExtentType type = reader->GetExtentType();
        bool is_active = reader->IsActive();

        DBG(printf("\tsc_name=%ls\n\t\t   cs_name=%ls\n\t\t   descr=%ls\n\t\t   type=%d\n\t\t   active=%d\n", 
                 sc_name, cs_name, descr, type, is_active));
        DBG(printf("\tWKT: '%ls'\n", cs_wkt ));
        count++;
    }
    DBG(printf("\t    retrieved %d SCs\n", count));
    CPPUNIT_ASSERT( count >= 1 );
}

void FdoSqlCmdTest::doGetSC()
{
    bool        active_only = false;
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoIGetSpatialContexts> gscCmd;
            FdoPtr<FdoISpatialContextReader> reader;
            DBG(printf("\n.Getting Spatial Contexts (active_only=%s)\n", active_only? "true":"false" ));

            ///////////////////////////////////////////////////////////////////////////////////////////
            // This will initialize for fetching the SC
            gscCmd = (FdoIGetSpatialContexts *)mConnection->CreateCommand( FdoCommandType_GetSpatialContexts );

            gscCmd->SetActiveOnly(active_only);
        
            // Get a SC reader
            reader = gscCmd->Execute();

            ProcessSpatialContextReader(reader);

            ///////////////////////////////////////////////////////////////////////////////////////////
            // Now try again, fetching all spatial contexts.
            active_only = false;
            reader = gscCmd->Execute();
            ProcessSpatialContextReader(reader);
        }
        catch (FdoException *ex )
        {
            printf("doGetSC(): Failed (active=%d).\n", active_only);
		    TestCommonFail (ex);
        }
    }
}

void FdoSqlCmdTest::CreateActivateDestroySC()
{
    FdoPtr<FdoICreateSpatialContext>   cscCmd;
    FdoPtr<FdoIDestroySpatialContext>  dscCmd;
    FdoPtr<FdoIActivateSpatialContext> ascCmd;

    DBG(printf("\n.Creating/Activating/Destroying Spatial Context\n"));
    if( mConnection != NULL )
    {
        try
        {
            cscCmd = (FdoICreateSpatialContext *)mConnection->CreateCommand( FdoCommandType_CreateSpatialContext );

    #pragma message ("ToDo: investigate trancation failure with longer SC names on MySQL.")
            cscCmd->SetName(L"SC_X"); 
            cscCmd->SetDescription(L"For testing only");
            cscCmd->SetCoordinateSystem(L"");

            // envelope as Polygon
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIEnvelope> env = gf->CreateEnvelopeXY(-10000000, -10000000, 10000000, 10000000);
            FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(env); 
            FdoPtr<FdoByteArray> ext = gf->GetFgf( geom );
            cscCmd->SetExtent( ext );
		    cscCmd->SetCoordinateSystemWkt(L"test WKT");

    #pragma message ("ToDo: add CreateSpatialContext test that updates.")

            cscCmd->SetXYTolerance(.0000001);
            cscCmd->SetZTolerance(.001);
            
            cscCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context creation.\n");
            throw ex;
        }

        /////////////////////////////////

        try
        {
            FdoPtr<FdoIConnection> mConn = UnitTestUtil::GetConnection(L"", false);

            ascCmd = (FdoIActivateSpatialContext *)mConn->CreateCommand( FdoCommandType_ActivateSpatialContext );

            ascCmd->SetName(L"SC_X");
      
            ascCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context activation.\n");
            throw ex;
        }

        /////////////////////////////////

        try
        {
            FdoPtr<FdoIConnection> mConn = UnitTestUtil::GetConnection(L"", false);

            ascCmd = (FdoIActivateSpatialContext *)mConn->CreateCommand( FdoCommandType_ActivateSpatialContext );

            ascCmd->SetName(L"SC_X");
      
            ascCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context activation.\n");
            throw ex;
        }

        //////////////////////////////////

        try
        {
            FdoPtr<FdoIConnection> mConn = UnitTestUtil::GetConnection(L"", false);

            dscCmd = (FdoIDestroySpatialContext *)mConn->CreateCommand( FdoCommandType_DestroySpatialContext );

            dscCmd->SetName(L"SC_X");
      
            dscCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context desctruction.\n");
            throw ex;
        }
    }
}

void FdoSqlCmdTest::DropTable ()
{
    FdoPtr<FdoISQLCommand> sqlCmd;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            FdoStringP sql = FdoStringP::Format(L"drop table %ls", SQLCMD_TABLE_NAME);
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            sqlCmd->ExecuteNonQuery();
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }

}


void FdoSqlCmdTest::ReadQueryResult( FdoISQLDataReader *myReader )
{
    for( int i = 0; i<myReader->GetColumnCount(); i++ )
    {
        const wchar_t * name = myReader->GetColumnName( i);
        if(  myReader->IsNull( name ) )
        {
            DBG(printf("Null value  "));
            continue;
        }
        switch( myReader->GetColumnType( name ) )
        {
            case FdoDataType_Boolean:
                    DBG(printf("%s ",myReader->GetBoolean( name )?"True":"False"));
                break;

            case FdoDataType_Byte:
                    DBG(printf("%x ",myReader->GetByte( name ) ));
                break;

            case FdoDataType_DateTime:
				{
				 FdoDateTime  time = myReader->GetDateTime( name );
				 DBG(printf("year=%d, month=%d, day=%d, hour=%d, minute=%d, seconds=%d", time.year, time.month, time.day, time.hour, time.minute, time.seconds));
				}
                break;

            case FdoDataType_Decimal:
                DBG(printf("%g ",myReader->GetDouble( name ) ));
                break;

            case FdoDataType_Double:
                DBG(printf("%g ",myReader->GetDouble( name ) ));
                break;


            case FdoDataType_Int16:
                DBG(printf("%d ",myReader->GetInt16( name ) ));
                break;


            case FdoDataType_Int32:
                DBG(printf("%d ",myReader->GetInt32( name ) ));
                break;


            case FdoDataType_Int64:
                DBG(printf("%d ",myReader->GetInt64( name ) ));
                break;


            case FdoDataType_Single:
                DBG(printf("%f ",myReader->GetSingle( name ) ));
                break;


            case FdoDataType_String:
                DBG(printf("%ls ",myReader->GetString( name ) ));
                break;


            case FdoDataType_BLOB:
            case FdoDataType_CLOB:
                throw FdoException::Create(L"Not supported");
                break;

            default:
                throw FdoException::Create(L"Unknown fdo data type");
        }
    }
    printf("\n");
}

void FdoSqlCmdTest::Query ()
{
    FdoPtr<FdoISQLDataReader> myReader;
    FdoPtr<FdoISQLCommand> selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISQLCommand> selCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            FdoStringP sql = FdoStringP::Format(L"select * from %ls", SQLCMD_TABLE_NAME);
            selCmd->SetSQLStatement( (FdoString *)sql );
            myReader = selCmd->ExecuteReader();
            selCmd = NULL;
            if( myReader != NULL  )
            {
                for( int i = 0; i<myReader->GetColumnCount(); i++ )
                {
                    const wchar_t * name = myReader->GetColumnName( i);
                    DBG(printf("%ls(%s) ", name, FdoTypeToStr(myReader->GetColumnType( name ) ) ));
                }
                DBG(printf("\n"));
                while ( myReader->ReadNext() )
                {
                    ReadQueryResult( myReader );
                }
                myReader = NULL;
            }
            selCmd = NULL;
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }

}

void FdoSqlCmdTest::QueryGeometry()
{
    FdoPtr<FdoISQLDataReader>myReader;
    FdoPtr<FdoISQLCommand>selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISQLCommand> selCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            selCmd->SetSQLStatement(L"select * from acdb3dpolyline");
            myReader = selCmd->ExecuteReader();

            FdoStringP  geomColName = L"geometry";

            selCmd->SetSQLStatement(GetGeometrySelectStatement());

            myReader = selCmd->ExecuteReader();
            if( myReader != NULL  )
            {
                for( int i = 0; i<myReader->GetColumnCount(); i++ )
                {
                    const wchar_t * name = myReader->GetColumnName( i);
                    if( myReader->GetPropertyType( name ) != FdoPropertyType_GeometricProperty )
                    {
                        printf("Geometric property is Expected and got something else (it must be a data property)\n" );
                    }
                }
                int count = 0;
                while ( myReader->ReadNext() && count++ < 5 )
                {
                    if ( !myReader->IsNull( geomColName) )
                    {
                        FdoPtr<FdoByteArray> byteArray = myReader->GetGeometry( geomColName );
                        DBG(printf(" \tGeometry byte array size: %d\n", byteArray->GetCount()));
                    }
                }
            }
        }
        catch( FdoException *ex )
        {
			TestCommonFail (ex);
        }
    }

}

char* FdoSqlCmdTest::FdoTypeToStr( FdoDataType type )
{
    switch( type )
    {
        case FdoDataType_Boolean:
            return "Boolean";

        case FdoDataType_Byte:
            return "Byte";

        case FdoDataType_DateTime:
            return "DateTime";

        case FdoDataType_Decimal:
            return "Decimal";

        case FdoDataType_Double:
            return "Double";

        case FdoDataType_Int16:
            return "Int16";

        case FdoDataType_Int32:
            return "Int32";

        case FdoDataType_Int64:
            return "Int64";

        case FdoDataType_Single:
            return "Single";

        case FdoDataType_String:
            return "String";

        case FdoDataType_BLOB:
            return "BLOB";

        case FdoDataType_CLOB:
            return "CLOB";

        default:
            return "Unknown fdo data type"; // pass type as parameter
    }
}

FdoString *FdoSqlCmdTest::GetGeometrySelectStatement()
{
	return L"select geometry from acdb3dpolyline";
}

