#include "StdHdr.h"
#include "ODBC.h"


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
