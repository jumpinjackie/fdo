#include "Pch.h"
#include "UnitTestUtil.h"
#include "c_OCI_API.h"
#include <cstdlib>

FdoIConnection* UnitTestUtil::CreateOraConnection(FdoString* connectionString)
{
    FdoPtr<IConnectionManager> connMgr = FdoFeatureAccessManager::GetConnectionManager();
    FdoPtr<FdoIConnection> conn = connMgr->CreateConnection(L"King.Oracle");

    conn->SetConnectionString(connectionString);

    return conn.Detach();
}

FdoIConnection* UnitTestUtil::CreateOraConnection(FdoString* service, FdoString* username, FdoString* password)
{
    FdoPtr<IConnectionManager> connMgr = FdoFeatureAccessManager::GetConnectionManager();
    FdoPtr<FdoIConnection> conn = connMgr->CreateConnection(L"King.Oracle");

    FdoPtr<FdoIConnectionInfo> connInfo = conn->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> connDict = connInfo->GetConnectionProperties();

    connDict->SetProperty(L"Service", service);
    connDict->SetProperty(L"Username", username);
    connDict->SetProperty(L"Password", password);
    connDict->SetProperty(L"OracleSchema", username);

    return conn.Detach();
}

FdoIConnection* UnitTestUtil::CreateDefaultConnection()
{
    FdoStringP connStr;
    const char* str = std::getenv("KG_DEFAULT_ORA_CONNECTION");
    if (NULL != str)
    {
        connStr = str;
    }
    else
    {
        connStr = L"Username=fdounittest;Password=fdounittest;Service=//localhost:1521/xe;OracleSchema=fdounittest";
    }
    return CreateOraConnection(connStr);
}

FdoStringP UnitTestUtil::GetFeatureClass(FdoString* tableName, FdoString* geomColumn, FdoString* userName)
{
    FdoStringP className;
    if (NULL == userName)
    {
        className = GetUsername().Upper();
    }
    else
    {
        className = FdoStringP(userName).Upper();
    }
    className += L"~";
    className += FdoStringP(tableName).Upper();
    className += L"~";
    className += FdoStringP(geomColumn).Upper();
    return className;
}

FdoStringP UnitTestUtil::GetUsername()
{
    FdoStringP username = D_CONN_USERNAME;
    const char* envUser = std::getenv("KG_ORA_USERNAME");
    if (NULL != envUser)
    {
        username = envUser;
    }
    return username;
}

FdoStringP UnitTestUtil::GetPassword()
{
    FdoStringP password = D_CONN_PASS;
    const char* envPass = std::getenv("KG_ORA_PASSWORD");
    if (NULL != envPass)
    {
        password = envPass;
    }
    return password;
}

FdoStringP UnitTestUtil::GetService()
{
    FdoStringP service = D_CONN_SERVICE;
    const char* envSvc = std::getenv("KG_ORA_SERVICE");
    if (NULL != envSvc)
    {
        service = envSvc;
    }
    return service;
}

c_Oci_Connection* UnitTestUtil::CreateOCIConnection()
{
    FdoStringP username = GetUsername();
    FdoStringP password = GetPassword();
    FdoStringP service = GetService();

    return c_OCI_API::CreateConnection(username, password, service);
}