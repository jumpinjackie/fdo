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
#include "FdoConnectionInfoTest.h"
#include "UnitTestUtil.h"

FdoConnectionInfoTest::FdoConnectionInfoTest ()
: mConnection(NULL)
{

}  //  FdoConnectionInfoTest ()

FdoConnectionInfoTest::~FdoConnectionInfoTest ()
{
    mConnection = NULL;
}  //  ~FdoConnectionInfoTest ()

void FdoConnectionInfoTest::setUp ()
{

    set_provider();

}  //  setUp ()

void FdoConnectionInfoTest::tearDown ()
{
    mConnection = NULL;
}  //  tearDown ()

void FdoConnectionInfoTest::set_provider ()
{

}  //  set_provider ()

void FdoConnectionInfoTest::SetExpectedFileList ()
{

}  //  SetExpectedFileList ()

void FdoConnectionInfoTest::TestProviderInfo ()
{

}  //  TestProviderInfo()

void FdoConnectionInfoTest::AddFileNameToList (FdoStringP fileName)
{

    FileNameDataSet *newDataSet = NULL;

    newDataSet = new FileNameDataSet(fileName);
    mExpectedFileList.push_back(newDataSet);

}

void FdoConnectionInfoTest::ClearFileNameList ()
{

    if (mExpectedFileList.size() > 0) {

        for (FdoInt32 i = 0; i < (signed) mExpectedFileList.size(); i++) {

          delete mExpectedFileList[i];
          mExpectedFileList[i] = NULL;

        }  //  for (FdoInt32 i = 0; ...

        mExpectedFileList.clear();

    }  //  if (affected_table_vector.size() > 0) ...

}

void FdoConnectionInfoTest::CheckProviderInfo ()
{
    FdoPtr<FdoIConnectionInfo> connectionInfo =
                                            mConnection->GetConnectionInfo();

    FdoString* featureDataObjectsVersion = connectionInfo->GetFeatureDataObjectsVersion();
    FdoString* providerDescription = connectionInfo->GetProviderDescription();
    FdoString* providerName = connectionInfo->GetProviderName();
    FdoString* providerVersion = connectionInfo->GetProviderVersion();

}  //  CheckDatabaseBasedProviderInfo ()

void FdoConnectionInfoTest::CheckDatabaseBasedProviderInfo ()
{

    printf("Checking for Provider Type and Dependent List Info \n");

    FdoPtr<FdoIConnectionInfo> connectionInfo =
                                            mConnection->GetConnectionInfo();

    // Check the provider type. Only valid option is "Database Server".

    printf(" ...> Checking for Provider Type\n");
    FdoProviderDatastoreType providerType =
                                    connectionInfo->GetProviderDatastoreType();

    switch (providerType) {

      case FdoProviderDatastoreType_DatabaseServer:
        printf(" ......> Found: 'Database Server' (expected result)\n");
        break;

      case FdoProviderDatastoreType_File:
        printf(" ......> Found: 'File' (unexpected result)\n");
        throw FdoException::Create(L"Unexpected provider type 'File' when expecting 'Database Server'");
        break;

      case FdoProviderDatastoreType_WebServer:
        printf(" ......> Found: 'Web Server' (unexpected result)\n");
        throw FdoException::Create(L"Unexpected provider type 'Web Server' when expecting 'Database Server'");
        break;

      case FdoProviderDatastoreType_Unknown:
        printf(" ......> Found: 'Unknown' (unexpected result)\n");
        throw FdoException::Create(L"Unexpected provider type 'Unknown' when expecting 'Database Server'");
        break;

    }  //  switch ...

    // Check for the list of dependent files. Because this is a provider of
    // type Database Server, the invoked function should return a NULL pointer.

    printf(" ...> Checking for list of dependent files\n");
    FdoPtr<FdoStringCollection> stringCollection =
                                    connectionInfo->GetDependentFileNames();
    if (stringCollection != NULL) {

        FdoStringP errMsg =
                FdoStringP::Format(L"Unexpected list of dependent files (%d)\n",
                                   stringCollection->GetCount());
        throw FdoException::Create((FdoString *)errMsg);

    }  //  if (stringCollection != NULL) ...

    printf(" ......> Found no dependent files (expected result)\n");

}  //  CheckDatabaseBasedProviderInfo ()

void FdoConnectionInfoTest::CheckFileBasedProviderInfo ()
{

    printf("Checking for Provider Type and Dependent List Info \n");

    FdoPtr<FdoIConnectionInfo> connectionInfo =
                                            mConnection->GetConnectionInfo();

    // Check the provider type. Only valid option is "File".

    printf(" ...> Checking for Provider Type\n");
    FdoProviderDatastoreType providerType = connectionInfo->GetProviderDatastoreType();

    switch (providerType) {

      case FdoProviderDatastoreType_DatabaseServer:
        printf(" ......> Found: 'Database Server' (unexpected result)\n");
        throw FdoException::Create(L"Unexpected provider type 'Database Server' when expecting 'File'");
        break;

      case FdoProviderDatastoreType_File:
        printf(" ......> Found: 'File' (expected result)\n");
        break;

      case FdoProviderDatastoreType_WebServer:
        printf(" ......> Found: 'Web Server' (unexpected result)\n");
        throw FdoException::Create(L"Unexpected provider type 'Web Server' when expecting 'File'");
        break;

      case FdoProviderDatastoreType_Unknown:
        printf(" ......> Found: 'Unknown' (unexpected result)\n");
        throw FdoException::Create(L"Unexpected provider type 'Unknown' when expecting 'File'");
        break;

    }  //  switch ...

    // Check for the list of dependent files. Because this is a provider of
    // type File, the invoked function should such a list. If a NULL pointer
    // or empty list is returned then this is an error.

    printf(" ...> Checking for list of dependent files\n");
    FdoPtr<FdoStringCollection> stringCollection =
                                    connectionInfo->GetDependentFileNames();
    if ((stringCollection == NULL         ) ||
        (stringCollection->GetCount() == 0)    )
        throw FdoException::Create(L"Unexpected empty list of dependent files");

    FdoInt32 stringCollectionCount = stringCollection->GetCount();
    for (FdoInt32 index = 0; index < stringCollectionCount; index++) {

      FdoPtr<FdoStringElement> stringCollectionElement =
                                                stringCollection->GetItem(index);
      FdoStringP dependentFileName = stringCollectionElement->GetString();
      printf(" ......> Found: '%ls'\n", (FdoString *) dependentFileName);
      CheckList(dependentFileName);

    }  //  for ( ...

    printf(" ...> Cross checking with list of expected dependent files\n");
    CrossCheck();
    printf(" ......> All expected files found\n");

}  //  CheckFileBasedProviderInfo ()

void FdoConnectionInfoTest::CheckList (FdoStringP dependentFileName)
{

    // Query the list of expected file names and check whether or not the
    // provided file name is among the list. If so set the flag to indicate
    // it.

    if (mExpectedFileList.size() > 0)
        for (FdoInt32 i = 0; i < (signed) mExpectedFileList.size(); i++)
          if (dependentFileName.Contains(mExpectedFileList[i]->fileName))
              mExpectedFileList[i]->found = true;

}  //  CheckList ()

void FdoConnectionInfoTest::CrossCheck ()
{

    bool issueException = false;

    // Query the list of expected file names and check if all of them were
    // found. Any file name that has not been found should be reported.

    if (mExpectedFileList.size() > 0)
        for (FdoInt32 i = 0; i < (signed) mExpectedFileList.size(); i++)
          if (!mExpectedFileList[i]->found) {

              printf(" ......> Missing File: '%ls'\n",
                     (FdoString *) mExpectedFileList[i]->fileName);
              issueException = true;

          }  //  if (!mExpectedFileList[i]->found) ...

    if (issueException)
        throw FdoException::Create(L"Expected dependent file(s) not reported");

}  //  CrossCheck ()


// ----------------------- Helper Class Implementation -----------------------

FdoConnectionInfoTest::FileNameDataSet::FileNameDataSet (FdoStringP aFileName)
{

    fileName = aFileName;
    found    = false;

}  //  FileNameDataSet::FileNameDataSet ()

FdoConnectionInfoTest::FileNameDataSet::~FileNameDataSet ()
{

}  //  FileNameDataSet::~FileNameDataSet ()

