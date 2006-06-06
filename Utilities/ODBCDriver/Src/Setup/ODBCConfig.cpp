#include "Stdafx.h"
#include "ODBCConfig.h"


BOOL INSTAPI ConfigDriver(
    HWND            hwndParent, 
    WORD            fRequest, 
    LPCSTR          lpszDriver,
    LPCSTR          lpszArgs, 
    LPSTR           lpszMsg, 
    WORD            cbMsgMax, 
    WORD 	        *pcbMsgOut)
{
    return TRUE;
}

BOOL INSTAPI ConfigDriverW(
    HWND            hwndParent, 
    WORD            fRequest, 
    LPCWSTR         lpszDriver,
    LPCWSTR         lpszArgs, 
    LPWSTR          lpszMsg, 
    WORD            cbMsgMax, 
    WORD 	        *pcbMsgOut)
{
    return TRUE;
}

BOOL INSTAPI ConfigDSN (
    HWND	        hwndParent,
    WORD	        fRequest,
    LPCSTR	        lpszDriver,
    LPCSTR	        lpszAttributes)
{
    return TRUE;
}

BOOL INSTAPI ConfigDSNW (
    HWND	        hwndParent,
    WORD	        fRequest,
    LPCWSTR	        lpszDriver,
    LPCWSTR	        lpszAttributes)
{
    return TRUE;
}
