#ifndef __FDOODBCCONFIGSQL
#define __FDOODBCCONFIGSQL

#include "ODBCConfig.h"

#ifdef __cplusplus
extern "C" { 
#endif  

//////////////////////////////////////////////////////////////////////
// ODBC Driver Setup DLL functions
//////////////////////////////////////////////////////////////////////

BOOL INSTAPI ConfigDriver(
    HWND            hwndParent, 
    WORD            fRequest, 
    LPCSTR          lpszDriver,
    LPCSTR          lpszArgs, 
    LPSTR           lpszMsg, 
    WORD            cbMsgMax, 
    WORD 	        *pcbMsgOut);

BOOL INSTAPI ConfigDriverW(
    HWND            hwndParent, 
    WORD            fRequest, 
    LPCWSTR         lpszDriver,
    LPCWSTR         lpszArgs, 
    LPWSTR          lpszMsg, 
    WORD            cbMsgMax, 
    WORD 	        *pcbMsgOut);

BOOL INSTAPI ConfigDSN(
    HWND	        hwndParent,
    WORD	        fRequest,
    LPCSTR	        lpszDriver,
    LPCSTR	        lpszAttributes);

BOOL INSTAPI ConfigDSNW(
    HWND	        hwndParent,
    WORD	        fRequest,
    LPCWSTR	        lpszDriver,
    LPCWSTR	        lpszAttributes);

#ifdef __cplusplus
}                                   
#endif 

#endif //__FDOODBCCONFIGSQL