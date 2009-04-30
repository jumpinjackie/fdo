// Copyright (C) 2004-2007  Autodesk, Inc.
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


#include "stdafx.h"
#include "FdoMultiThreadTest.h"
#include "UnitTestUtil.h"
#include "FdoCommonOSUtil.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif



#define   NUMBER_OF_THREADS     10
static    bool   use_threads = true;
static    bool   supports_multiple_writers = false;
static    int    select_loop_count = 10;
static    int    insert_loop_count = 20;

CPPUNIT_TEST_SUITE_REGISTRATION( FdoMultiThreadTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FdoMultiThreadTest, "FdoMultiThreadTest");

void wait_a_bit(int ms)
{
#ifdef _WIN32
        Sleep(ms);
#else
        sleep((ms+999)/1000);
#endif
}

#ifdef _WIN32
DWORD WINAPI StartQuery(LPVOID lpParameter)
#else
void* StartQuery( void *lpParameter) 
#endif
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
	char buffer[1024];
	int  *counts = (int*)alloca( select_loop_count*sizeof(int) );
    try
	{
		wait_a_bit(200); // Give the insert thread a head start
		printf("Start query Thread(%d)\n",cnInfo->connectionId );

		FdoPtr<FdoISelect> select = (FdoISelect*)cnInfo->mConn->CreateCommand(FdoCommandType_Select); 

		select->SetFeatureClassName(L"Parcel");

		//FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Key LIKE 'DI%'");

		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		double coords[] = { 7.2068, 43.7556, 
							7.2088, 43.7556, 
							7.2088, 43.7574, 
							7.2068, 43.7574, 
							7.2068, 43.7556 }; 
		FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
		FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"Data", FdoSpatialOperations_EnvelopeIntersects, gv);

		select->SetFilter(filter);
	    int count2 = 0;
		for( int i=0; i<select_loop_count; i++ )
		{
			FdoPtr<FdoIFeatureReader> rdr = select->Execute();
			while (rdr->ReadNext())
			{
				const wchar_t* something = rdr->GetString(L"Name");
				count2++;
			}
			counts[i] = count2;
			rdr->Close();
			wait_a_bit(100);
		}
		char tmp[12];
		buffer[0]=0;
		for( int i=0; i<select_loop_count; i++ )
		{
			strcat( buffer, FdoCommonOSUtil::itoa(counts[i],tmp) );
			strcat( buffer, " ");
		}

		printf("Thread(%d) is done Counts: %s \n",cnInfo->connectionId, buffer );
		
	}
	catch (FdoException *ex )
	{
		sprintf(buffer,"Query: FDO error: %ls\n", ex->GetExceptionMessage());
		ex->Release();
		CPPUNIT_FAIL(buffer);
	}

    return 0;
}

#ifdef _WIN32
DWORD WINAPI StartInsert(LPVOID lpParameter)
#else
void* StartInsert( void *lpParameter) 
#endif
{
	try
	{
		ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
		printf("Start insert Thread(%d)\n",cnInfo->connectionId );
		for(int i=0; i<insert_loop_count; i++ )
			UnitTestUtil::CreateData(false, cnInfo->mConn ,100, NULL, cnInfo->connectionId);
		printf("Thread(%d) done insert\n",cnInfo->connectionId );
	}
	catch (FdoException *ex )
	{
		char buffer[1024];
		sprintf(buffer,"Insert: FDO error: %ls\n", ex->GetExceptionMessage());
		ex->Release();
		CPPUNIT_FAIL(buffer);
	}
    return 0;
}


FdoMultiThreadTest::FdoMultiThreadTest(void)
{
}

FdoMultiThreadTest::~FdoMultiThreadTest(void)
{
}

void FdoMultiThreadTest::setUp ()
{
	UnitTestUtil::CreateData( true, NULL ,10000);
}

void FdoMultiThreadTest::OpenConnection(FdoIConnection* conn, const wchar_t* path )
{
#ifdef _WIN32
    wchar_t fullpath[1024];
    _wfullpath(fullpath, path, 1024);
#else
    char cpath[1024];
    char cfullpath[1024];
    wcstombs(cpath, path, 1024);
    realpath(cpath, cfullpath);
    wchar_t fullpath[1024];
    mbstowcs(fullpath, cfullpath, 1024);
#endif

    std::wstring connStr = std::wstring(L"File=") + std::wstring(fullpath);
    connStr += std::wstring(L";UseFdoMetadata=TRUE");

    conn->SetConnectionString(connStr.c_str());
    conn->Open();
}

void FdoMultiThreadTest::StartTest ( FunctionInfo *funInfo )
{
#ifdef _WIN32
    HANDLE phThreads[NUMBER_OF_THREADS];
    DWORD dwThreadId = 0;
#else
    pthread_t phThreads[NUMBER_OF_THREADS];
#endif
	
	FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

    ConnectInfo   info[NUMBER_OF_THREADS];
    int i;
    try
    {
        for (int i = 0; i < NUMBER_OF_THREADS; i++)
	    {
            info[i].connectionId = i;
            info[i].mConn = manager->CreateConnection (L"OSGeo.SQLite");
			OpenConnection(info[i].mConn,DESTINATION_FILE);
	    } 
    }
    catch (FdoException *ex )
	{
		printf("FDO error: %ls\n", ex->GetExceptionMessage());
        return;
	}

    bool  toggle = true;
	
	if( ! supports_multiple_writers && use_threads )
	{
#ifdef _WIN32
		phThreads[0] = CreateThread(NULL, 0, funInfo->Function2, &info[0], 0, &dwThreadId);
#else
		pthread_create( &phThreads[0], NULL, funInfo->Function2, (void*) &info[0]);
#endif
	}

	i = ( supports_multiple_writers )?0:1;
    for ( ; i < NUMBER_OF_THREADS; i++)
	{
        if( use_threads )
#ifdef _WIN32
            phThreads[i] = CreateThread(NULL, 0, (toggle)?funInfo->Function1:funInfo->Function2, &info[i], 0, &dwThreadId);
#else
            pthread_create( &phThreads[i], NULL, (toggle)?funInfo->Function1:funInfo->Function2, (void*) &info[i]);
#endif
        else
            StartQuery( &info[i] );

		if( supports_multiple_writers )
			toggle = !toggle;
	} 

	

#ifdef _WIN32
    if( use_threads )
	    WaitForMultipleObjects(NUMBER_OF_THREADS, phThreads, TRUE, INFINITE);
#else
    for ( i = 0; i < NUMBER_OF_THREADS && use_threads; i++)
        pthread_join( phThreads[i], NULL); 
#endif

    for (i = 0; i < NUMBER_OF_THREADS; i++)
        info[i].mConn->Release();

#ifdef _WIN32
	for (i = 0; i < NUMBER_OF_THREADS && use_threads; i++)
		CloseHandle(phThreads[i]);
#endif
}

void FdoMultiThreadTest::QueryTest()
{
    FunctionInfo funcInfo;

    funcInfo.Function1 = StartQuery;
    funcInfo.Function2 = StartQuery;

    StartTest( &funcInfo );
}


void FdoMultiThreadTest::InsertQueryTest()
{
    FunctionInfo funcInfo;

    funcInfo.Function1 = StartQuery;
    funcInfo.Function2 = StartInsert;

    StartTest( &funcInfo );
}

#ifdef _WIN32
void FdoMultiThreadTest::InitInsertFunction( LPTHREAD_START_ROUTINE & Funct )
#else
void FdoMultiThreadTest::InitInsertFunction( void* (*Funct)(void *) )
#endif
{
	Funct = StartInsert;
}

