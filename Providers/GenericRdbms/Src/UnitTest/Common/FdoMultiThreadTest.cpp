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
#include "FdoMultiThreadTest.h"
#include "UnitTestUtil.h"
#include "FdoInsertTest.h"
#include "FdoUpdateTest.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif

#define   NUMBER_OF_THREADS     10
static    bool   use_threads = true;

FdoMultiThreadTest::FdoMultiThreadTest(void)
{
}

FdoMultiThreadTest::~FdoMultiThreadTest(void)
{
}

void FdoMultiThreadTest::setUp ()
{
    set_provider();
}

FdoIConnection * FdoMultiThreadTest::GetConnection()
{
	return UnitTestUtil::GetConnection(L"", true);
}

void FdoMultiThreadTest::StartTest ( FunctionInfo *funInfo )
{
#ifdef _WIN32
    HANDLE phThreads[NUMBER_OF_THREADS];
    DWORD dwThreadId = 0;
#else
    pthread_t phThreads[NUMBER_OF_THREADS];
#endif
    ConnectInfo   info[NUMBER_OF_THREADS];
    int i;
    try
    {
        for (int i = 0; i < NUMBER_OF_THREADS; i++)
	    {
            info[i].connectionId = i;
            info[i].mConn = GetConnection();
            info[i].msgException = L"";
	    } 
    }
    catch (FdoException *ex )
	{
		char msg[512];
		sprintf(msg,"%ls", ex->GetExceptionMessage());
		CPPUNIT_FAIL(msg);
        return;
	}

    bool  toggle = true;
    for ( i = 0; i < NUMBER_OF_THREADS; i++)
	{
        if( use_threads )
#ifdef _WIN32
            phThreads[i] = CreateThread(NULL, 0, (toggle)?funInfo->Function1:funInfo->Function2, &info[i], 0, &dwThreadId);
#else
            pthread_create( &phThreads[i], NULL, (toggle)?funInfo->Function1:funInfo->Function2, (void*) &info[i]);
#endif
        else
		{
			if(toggle)
				funInfo->Function1( &info[i] );
			else
				funInfo->Function2( &info[i] );
            
		}

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
    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        if (info[i].msgException.GetLength() != 0)
            TestCommonFail(FdoException::Create(info[i].msgException));
    }
}

void FdoMultiThreadTest::QueryTest()
{
    FunctionInfo funcInfo;

    funcInfo.Function1 = StartQuery;
    funcInfo.Function2 = StartQuery;

    StartTest( &funcInfo );
}

void FdoMultiThreadTest::InsertTest()
{
    FunctionInfo funcInfo;
    
    funcInfo.Function1 = StartInsert;
    funcInfo.Function2 = StartInsert;

    StartTest( &funcInfo );
}

void FdoMultiThreadTest::UpdateTest()
{
    FunctionInfo funcInfo;
    
    funcInfo.Function1 = StartUpdate;
    funcInfo.Function2 = StartUpdate;

    StartTest( &funcInfo );
}

void FdoMultiThreadTest::CombinationTest()
{
    FunctionInfo funcInfo;

    funcInfo.Function1 = StartQuery;
    funcInfo.Function2 = StartInsert;

    StartTest( &funcInfo );
}

void FdoMultiThreadTest::ReadAcDbPolyline( int threadId, FdoIFeatureReader *myReader )
{
    try
    {
        if( ! myReader->IsNull(L"FeatId") )
        {
            //printf("\t(ThreadId:%d) FeatId: %d\n", threadId,myReader->GetInt32(L"FeatId") );
            //fflush(stdout);
        }
        if( ! myReader->IsNull(L"layer") )
        {
           // printf("\t(ThreadId:%d)Layer: %ls\n", threadId,myReader->GetString(L"layer") );
           // fflush(stdout);
        }
    }
    catch( FdoException *ex)
    {
        printf("(ThreadId:%d)FDO exception: %ls \n", threadId, ex->GetExceptionMessage() );
    }
}

#ifdef _WIN32
DWORD WINAPI FdoMultiThreadTest::StartQuery(LPVOID lpParameter)
#else
void* FdoMultiThreadTest::StartQuery( void *lpParameter) 
#endif
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
	int count = 0;
    FdoPtr<FdoIConnection>mConn;
    try
	{

		FdoPtr<FdoISelect>selCmd = (FdoISelect*)cnInfo->mConn->CreateCommand(  FdoCommandType_Select );
        selCmd->SetFilter(L"segcount < 20");
        selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute( );
        if( myReader != NULL )
        {
            while ( myReader->ReadNext() )
            {
                count++;
				ReadAcDbPolyline(cnInfo->connectionId ,myReader );
            }
			printf("(ThreadId:%d) read %d objects\n",cnInfo->connectionId,count);
			fflush(stdout);
        }
	}
	catch (FdoException *ex )
	{
		printf("FDO error: %ls\n", ex->GetExceptionMessage());
        cnInfo->msgException = ex->GetExceptionMessage();
        ex->Release();
	}

    return 0;
}

#ifdef _WIN32
DWORD WINAPI FdoMultiThreadTest::StartInsert(LPVOID lpParameter)
#else
void* FdoMultiThreadTest::StartInsert( void *lpParameter) 
#endif
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
    FdoInsertTest   test;

    test.DisableFailures();
    try
    {
        test.MainInsertTest( cnInfo->mConn );
    }
	catch (FdoException *ex )
	{
		printf("FDO error: %ls\n", ex->GetExceptionMessage());
        cnInfo->msgException = ex->GetExceptionMessage();
        ex->Release();
	}
    return 0;
}

#ifdef _WIN32
DWORD WINAPI FdoMultiThreadTest::StartUpdate(LPVOID lpParameter)
#else
void* FdoMultiThreadTest::StartUpdate( void *lpParameter) 
#endif
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
    FdoUpdateTest   test;

    test.DisableFailures();
    try
    {
        test.MainFdoUpdateTest( cnInfo->mConn );
    }
	catch (FdoException *ex )
	{
		printf("FDO error: %ls\n", ex->GetExceptionMessage());
        cnInfo->msgException = ex->GetExceptionMessage();
        ex->Release();
	}
    return 0;
}
