#include "StdHdr.h"
#include "ODBCDriver.h"
#include <string>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    BOOL bResult = FALSE;
    CHAR module[MAX_PATH];
    CHAR home_dir[MAX_PATH];
    CHAR* last = NULL;
    DWORD nchars = 0;

    try {
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        nchars = GetModuleFileNameA((HINSTANCE)hModule, module, MAX_PATH);
        if (nchars > 0) {   
            strcpy (home_dir, module);
            last = strrchr (home_dir, '\\');
            if (NULL != last) {
                *last = '\0'; 
                bResult = TRUE;
            }
        }

        if (bResult) {
            bResult = InstallDriver(home_dir);
        }

        break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
        bResult = UninstallDriver();
    }
    }
    catch (FdoException* e)
    {
		_RPT1(0, "\n%ls\n", e->GetExceptionMessage()); 
        FdoExceptionP cause = e->GetCause();
        while (cause != NULL) {
		    _RPT1(0, "\n%ls\n", cause->GetExceptionMessage()); 
            cause = cause->GetCause();
        }
    }
    catch (...)
    {
		_RPT1(0, "\n%s\n", "Unhandled Exception"); 
    }

    return bResult;
}

BOOL InstallDriver(CHAR* lpszDriverPath)
{
    BOOL bResult = FALSE;
    CHAR lpszPathOut[MAX_PATH];
    DWORD dwUsageCount = 0;
    WORD wPathOutMax = MAX_PATH;
    WORD wPathOut = 0;
    RETCODE retcode = SQL_ERROR;
    LPCSTR lpszDriverName = "OSGeo FDO SDF Driver (*.sdf)";
    LPCSTR lpszDriver = "OSGeo FDO SDF Driver (*.sdf)\0"
                        "Driver=ODBCDriver.dll\0"
                        "Setup=ODBCSetup.dll\0"
                        "APILevel=0\0"
                        "ConnectFunctions=YYY\0"
                        "DriverODBCVer=03.00\0"
                        "FileExtns=*.sdf\0"
                        "FileUsage=2\0"
                        "SQLLevel=0\0"
                        "\0";
    LPCSTR lpszArgs = "TestOptionA=1\0TestOptionB=2\0TestOptionC=3\0\0";
    CHAR lpszMsg[1024];
    WORD cbMsgMax = 1024;
    WORD cbMsgOut = 0;

    bResult = UninstallDriver();
    if (bResult == FALSE) {
        return bResult;
    }

    bResult = SQLInstallDriverEx(
        lpszDriver,
        lpszDriverPath,
        lpszPathOut,
        wPathOutMax,
        &wPathOut,
        ODBC_INSTALL_INQUIRY,
        &dwUsageCount);

    if (bResult == FALSE) {
        ThrowDriverError();
    }

    bResult = SQLInstallDriverEx(
        lpszDriver,
        lpszDriverPath,
        lpszPathOut,
        wPathOutMax,
        &wPathOut,
        ODBC_INSTALL_COMPLETE,
        &dwUsageCount);

    if (bResult == FALSE) {
        ThrowDriverError();
    }

    bResult = SQLConfigDriver(
         NULL,                          
         ODBC_INSTALL_DRIVER,           
         lpszDriverName,    
         lpszArgs,                           
         lpszMsg,                           
         cbMsgMax,                             
         &cbMsgOut );    

    if (bResult == FALSE) {
        ThrowDriverError();
    }

    return bResult;
}

BOOL UninstallDriver()
{
    BOOL bResult = FALSE;
    WORD cbPathOutMax = 1024;
    DWORD dwUsageCount = 0;
    CHAR lpszBuf[10240];
    CHAR* lpszBuf_P = lpszBuf;
    WORD cbBufMax = 10240;
    WORD wBufOut = 0;
    LPCSTR lpszDriver = "OSGeo FDO SDF Driver (*.sdf)";

    bResult = SQLGetInstalledDrivers(
         lpszBuf,
         cbBufMax,
         &wBufOut);

    if (bResult == FALSE) {
        ThrowDriverError();
    }

    FdoStringP sDriver;
    FdoBoolean bFound = false;
    do
    {
        sDriver = lpszBuf_P;
        if (sDriver == FdoStringP(lpszDriver)) {
            bResult = SQLConfigDriver(
                 NULL,                          
                 ODBC_REMOVE_DRIVER,            
                 lpszDriver,    
                 "",                           
                 "",                           
                 0,                             
                 NULL );                        

            if (bResult == FALSE) {
                ThrowDriverError();
            }

            bResult = SQLRemoveDriver(
                lpszDriver,
                TRUE,
                &dwUsageCount);

            if (bResult == FALSE) {
                ThrowDriverError();
            }

            while (dwUsageCount > 0) {
                bResult = SQLRemoveDriver(
                    lpszDriver,
                    TRUE,
                    &dwUsageCount);

                if (bResult == FALSE) {
                    ThrowDriverError();
                }
            }

            bFound = true;
        }

        lpszBuf_P = strchr( lpszBuf_P, '\0' ) + 1;
    }
    while( lpszBuf_P[1] != '\0' && bFound == false );

    return bResult;
}

VOID ThrowDriverError()
{
    WORD iError = 1;
    DWORD pfErrorCode = 0;
    CHAR lpszErrorMsg[1024];
    WORD cbErrorMsgMax = 1024;
    WORD pcbErrorMsg = 0;
    RETCODE retcode = SQL_SUCCESS;
    FdoExceptionP exception;

    while (retcode == SQL_SUCCESS_WITH_INFO || retcode == SQL_SUCCESS) {
        retcode = SQLInstallerError(
             iError++,
             &pfErrorCode,
             lpszErrorMsg,
             cbErrorMsgMax,
             &pcbErrorMsg);

        FdoExceptionP exception2 = FdoException::Create(FdoStringP(lpszErrorMsg), exception);
        exception = exception2;
    }

    throw exception;
}