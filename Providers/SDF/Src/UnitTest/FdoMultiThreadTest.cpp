
#include "FdoMultiThreadTest.h"
#include <stdio.h>

#ifndef _WIN32
#include <dlfcn.h>
#endif



#define   NUMBER_OF_THREADS     10
static    bool   use_threads = true;

#ifndef  IGNORE_THREAD_TEST
CPPUNIT_TEST_SUITE_REGISTRATION( FdoMultiThreadTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FdoMultiThreadTest, "FdoMultiThreadTest");
#endif

#ifdef _WIN32
DWORD WINAPI StartQuery(LPVOID lpParameter)
#else
void* StartQuery( void *lpParameter) 
#endif
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
	char tmp[1024];
    try
	{

		FdoPtr<FdoISelect> select = (FdoISelect*)cnInfo->mConn->CreateCommand(FdoCommandType_Select); 

		select->SetFeatureClassName(L"DaKlass");

		FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Key LIKE 'DI%' or Key = 'LN0316' or Key = 'DI0022'");


		select->SetFilter(filter);
	    
		FdoPtr<FdoIFeatureReader> rdr = select->Execute();

		int count2 = 0;

		FdoPtr<FdoByteArray> geom;
	    	    
	    
		while (rdr->ReadNext())
		{
			const wchar_t* something = rdr->GetString(L"Name");
			//wcstombs(tmp, something, 1024);
			//printf("Thread(%d):%s\n",cnInfo->connectionId, tmp);
			count2++;
		}
#ifdef _WIN32
		CPPUNIT_ASSERT_MESSAGE("Unexpected number of objects", count2==153 );
#else
		printf("Thread(%d) returned %d objects\n",cnInfo->connectionId,count2);
#endif
		rdr->Close();

	}
	catch (FdoException *ex )
	{
		sprintf(tmp,"FDO error: %ls\n", ex->GetExceptionMessage());
		CPPUNIT_FAIL(tmp);
	}

    return 0;
}

#ifdef _WIN32
DWORD WINAPI StartInsert(LPVOID lpParameter)
#else
void* StartInsert( void *lpParameter) 
#endif
{
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
    connStr += std::wstring(L";ReadOnly=TRUE");

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
            info[i].mConn = manager->CreateConnection (L"OSGeo.SDF.3.2");
			OpenConnection(info[i].mConn,SDF_FILE);
	    } 
    }
    catch (FdoException *ex )
	{
		printf("FDO error: %ls\n", ex->GetExceptionMessage());
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
            StartQuery( &info[i] );

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

void FdoMultiThreadTest::InsertTest()
{
    FunctionInfo funcInfo;
    
    InitInsertFunction(funcInfo.Function1);
    InitInsertFunction(funcInfo.Function2);

    StartTest( &funcInfo );
}

void FdoMultiThreadTest::CombinationTest()
{
    FunctionInfo funcInfo;

    funcInfo.Function1 = StartQuery;
    InitInsertFunction(funcInfo.Function2);

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

