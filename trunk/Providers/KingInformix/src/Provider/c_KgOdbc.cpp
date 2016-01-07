#include "StdAfx.h"
#include "c_KgOdbc.h"

c_KgOdbc::c_KgOdbc(void)
{
}

c_KgOdbc::~c_KgOdbc(void)
{
}



void c_KgOdbc::GetConnectionUsername()
{
    SQLHDBC       hdbc;
    SQLHENV       henv;
    SQLSMALLINT outlen;
    int           rc;
    char    info[256]; 
    
    server_connect ((UCHAR *)"IDS", &henv, &hdbc);
    {
      rc = SQLGetInfo(hdbc, SQL_DRIVER_NAME,info, 255, &outlen);
      if(rc == SQL_SUCCESS)
      {
        
      }
    }
    
    SQLDisconnect (hdbc);                    /* Close the connection */
    SQLFreeHandle (SQL_HANDLE_DBC, hdbc);    /* Free the database handle */
    SQLFreeHandle (SQL_HANDLE_ENV, henv); 

}