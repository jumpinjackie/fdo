#ifndef UNIT_TEST_UTIL_H
#define UNIT_TEST_UTIL_H

#include "stdafx.h"
#include "Fdo.h"
#include <FdoCommonFile.h>

class UnitTestUtil
{
public:
    static FdoIConnection* CreateOgrConnection(FdoString* dataSource, FdoBoolean bReadOnly = true)
    {
        FdoPtr<IConnectionManager> connMgr = FdoFeatureAccessManager::GetConnectionManager();
        FdoPtr<FdoIConnection> conn = connMgr->CreateConnection(L"OSGeo.OGR");

        FdoPtr<FdoIConnectionInfo> connInfo = conn->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> connDict = connInfo->GetConnectionProperties();
        if (NULL != dataSource)
            connDict->SetProperty(L"DataSource", dataSource);
        connDict->SetProperty(L"ReadOnly", bReadOnly ? L"TRUE" : L"FALSE");

        return conn.Detach();
    }

    static void CopySourceFilesTo(FdoString* srcDir, FdoString* dir, FdoStringCollection* fileNames)
    {
        if (!FdoCommonFile::FileExists(dir))
            FdoCommonFile::MkDir(dir);

        std::wstring srcPath;
        std::wstring dstPath;

        for (FdoInt32 i = 0; i < fileNames->GetCount(); i++)
        {
            FdoStringElement* fn = fileNames->GetItem(i);

            srcPath = srcDir;
            srcPath += L"/";
            srcPath += fn->GetString();

            dstPath = dir;
            dstPath += L"/";
            dstPath += fn->GetString();

            if (FdoCommonFile::FileExists(dstPath.c_str()))
                FdoCommonFile::Delete(dstPath.c_str(), true);
            FdoCommonFile::Copy(srcPath.c_str(), dstPath.c_str());
        }
    }
};

#endif