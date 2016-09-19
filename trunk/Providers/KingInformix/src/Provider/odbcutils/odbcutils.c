void returncode_check  (SQLHDBC     handle,
                        SQLHSTMT    hstmt,
                        int         rc,
                        char       *str);

static void rc_to_str  (int         rc,
                        char       *str);


/***************************************************************************
*
*  server_connect -- connect to server
*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*  Purpose:
*    This function establishes a connection to the server.
*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*  Parameters:
*    datasource   <Input>  == (char *) ODBC datasource name.
*    henv         <Input>  == (SQLHENV *) Pointer to the ODBC environment.
*    SQLHDBC      <Input>  == (SQLHDBC *) The ODBC connection handle.
*
*    RETURN       <Output> == (void)
*
****************************************************************************/

void server_connect (
    SQLCHAR *datasource,
    SQLHENV *henv,
    SQLHDBC *hdbc
    )
{
    int            rc;
    SDWORD         dbms_err;
    SWORD          length;
    unsigned char  err_msg[SQL_MAX_MESSAGE_LENGTH];
    unsigned char  state[6];

    /* Load the connectivity variables... */

    printf ("Connecting to datasource %s ...\n", datasource);

    /* Allocate memory for the ODBC environment handle and initialize
       the ODBC environment. */

    rc = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, henv);

    if (rc != SQL_SUCCESS)
    {
        printf ("SQLAllocHandle failed with %d\n", rc);
        exit (1);
    }

    /* Set the ODBC version to 3.0 */
    rc = SQLSetEnvAttr (*henv, SQL_ATTR_ODBC_VERSION,
                        (SQLPOINTER) SQL_OV_ODBC3, 0);
    if (rc != SQL_SUCCESS)
    {
        printf ("SQLSetEnvAttr failed with %d\n", rc);

        SQLGetDiagRecA(SQL_HANDLE_ENV, *henv, 1, state,
                       &dbms_err, err_msg,
                       SQL_MAX_MESSAGE_LENGTH - 1, &length);

        printf ("DBMS error code:%d, SQL state: %s, message: %s\n",
                dbms_err, state, err_msg);

        exit (1);
    }

    /* Allocate memory for a ODBC connection handle within
       the environment just established. */

    rc = SQLAllocHandle (SQL_HANDLE_DBC, *henv, hdbc);
    if (rc != SQL_SUCCESS)
    {
        printf ("SQLAllocConnect failed with %d\n", rc);

        SQLGetDiagRecA(SQL_HANDLE_ENV, *henv, 1, state,
                       &dbms_err, err_msg,
                       SQL_MAX_MESSAGE_LENGTH - 1, &length);

        printf ("DBMS error code:%d, SQL state: %s, message: %s\n",
                dbms_err, state, err_msg);

        exit (1);
    }

    /* Establish the database connection */

    rc = SQLConnectA(*hdbc, datasource, SQL_NTS,
                     (SQLCHAR *) "", SQL_NTS,    /* username */
                     (SQLCHAR *) "", SQL_NTS);   /* password */
    returncode_check (*hdbc, (SQLHSTMT)NULL, rc, "SQLConnect");

    return;
}

/***************************************************************************
*
*  returncode_check -- checks the return code for SQL_SUCCESS
*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*  Purpose:
*    This program expands the return error code and displays
*    the results if the return code is not SQL_SUCCESS.
*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*  Parameters:
*    hdbc    <Input>  == (SQLHDBC) The ODBC connection handle.
*    hstmt   <Input>  == (SQLHSTMT) The ODBC SQL statement handle.
*    rc      <Input>  == (int) The error return code.
*    str     <Input>  == (char *) The ODBC function the error occurred on.
*
*    RETURN  <Output> == (void)
*
****************************************************************************/

void returncode_check (
    SQLHDBC   hdbc,
    SQLHSTMT  hstmt,
    int       rc,
    char     *str
   )
{
    SDWORD         dbms_err = 0;
    SWORD          length;
    unsigned char  err_msg[SQL_MAX_MESSAGE_LENGTH];
    unsigned char  state[6];
    char           rc_str[80];


    if (rc != SQL_SUCCESS)
    {
        /* Convert the return code (rc) to its character string
           equivalent (rc_str)*/       

        rc_to_str (rc, rc_str);

        /* The SQLError function returns the SQL state, the native DBMS
           error code, and a pointer to the associated native DBMS error
           message. */

        if (hdbc != NULL)
            SQLGetDiagRecA(SQL_HANDLE_DBC, hdbc, 1, state, &dbms_err,
                           err_msg, SQL_MAX_MESSAGE_LENGTH - 1, &length);
        else
            SQLGetDiagRecA(SQL_HANDLE_STMT, hstmt, 1, state, &dbms_err,
                           err_msg, SQL_MAX_MESSAGE_LENGTH - 1, &length);

        printf ("%s ERROR (%s): DBMS code:%d, SQL state: %s, message: \n%s\n",
                str, rc_str, dbms_err, state, err_msg);


        if (rc != SQL_SUCCESS_WITH_INFO)
        {
            /* Only fatal out if more severe than SQL_SUCCESS_WITH_INFO */
            if (NULL != hstmt)
            {
                /* Try to release locks, & other resources */
                SQLFreeStmt (hstmt, SQL_DROP);  /* Release the SQL statment handle   */
                /* and free all resources associated */
                /* with it. */
            }

            SQLDisconnect (hdbc);  /* Close the connection */
            SQLFreeConnect (hdbc); /* Free the connection handle */
            exit (1);
        }
    }
}


/***************************************************************************
*
*  rc_to_str -- converts ODBC return code to string
*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*  Purpose:
*    The ODBC return code integer is converted to its string equivalent.
*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*  Parameters:
*    rc      <Input>    == (int) The ODBC return code.
*    str     <Input>    == (char *) A pointer to the error code string.
*
*    RETURN  <Output>   == (void)
*
****************************************************************************/

static void rc_to_str (
    int   rc,
    char *str
    )
{

    /* Expand the error code (rc) to its string (str) */

    switch (rc)
    {
        case SQL_SUCCESS:
            strcpy (str, "SQL_SUCCESS");
            break;

        case SQL_SUCCESS_WITH_INFO:
            strcpy (str, "SQL_SUCCESS_WITH_INFO");
            break;

        case SQL_NO_DATA_FOUND:
            strcpy (str, "SQL_NO_DATA_FOUND");
            break;

        case SQL_NEED_DATA:
            strcpy (str, "SQL_NEED_DATA");
            break;

        case SQL_STILL_EXECUTING:
            strcpy (str, "SQL_STILL_EXECUTING");
            break;

        case SQL_ERROR:
            strcpy (str, "SQL_ERROR");
            break;

        case SQL_INVALID_HANDLE:
            strcpy (str, "SQL_INVALID_HANDLE");
            break;

        default:
            strcpy (str, "UNKNOWN RETURN CODE!!");
            break;
    }
}

