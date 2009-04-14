// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
* Name                                                                  *
*   rdbi.h  - define necessary rdbi constants                           *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*                                                                       *
* Description                                                           *
*       Define  constants  to  be used by routines interfacing to       *
*       the rdbi package.                                               *
*                                                                       *
* Remarks                                                               *
*       The `ocidr' rdbms driver  uses RDBI_MAX_CONNECTS, but its       *
*       Pro*C (Embedded SQL) modules cannot  automatically  adapt       *
*       to   new  values.   If  RDBI_MAX_CONNECTS  is  increased,       *
*       ocidr's hard-coded  Pro*C  db  symbols must be checked to       *
*       see that enough connections are allowed.                        *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/

#ifndef RDBI_H
#define RDBI_H

#define RDBI_MAX_CONNECTS       40

#define RDBI_COMMIT_AUTO_ON     -1
#define RDBI_COMMIT_AUTO_OFF    0
#define RDBI_COMMIT_ONCE        1

#define         RDBI_TABLE_NAME_SIZE            129

#define         RDBI_COLUMN_NAME_SIZE           129

/*  Keywords for values obtained via rdbi_get_info  */

#define RDBI_CONNECT_VARIABLE   "connect_var"

/* rdbi_sql_va varargs constants */

#define RDBI_VA_EOL      0      /* Note: 0 must not be an RDBI internal type */
#define RDBI_VA_DEFNAMES 0x001
#define RDBI_VA_DEFNULLS 0x002
#define RDBI_VA_BNDNAMES 0x004
#define RDBI_VA_BNDNULLS 0x008
#define RDBI_VA_NEXEC    0x010
#define RDBI_VA_EXEC     0x020
#define RDBI_VA_NFETCH   0x040
#define RDBI_VA_FETCH    0x080
#define RDBI_VA_ENDSEL   0x100
#define RDBI_VA_NFETCH1  (RDBI_VA_NFETCH|RDBI_VA_ENDSEL)
#define RDBI_VA_FETCH1   (RDBI_VA_FETCH|RDBI_VA_ENDSEL)

#define RDBI_VA_DEFALL   (RDBI_VA_DEFNAMES|RDBI_VA_DEFNULLS)
#define RDBI_VA_BNDALL   (RDBI_VA_BNDNAMES|RDBI_VA_BNDNULLS)
#define RDBI_VA_ALLNAMES (RDBI_VA_BNDNAMES|RDBI_VA_DEFNAMES)
#define RDBI_VA_ALLNULLS (RDBI_VA_BNDNULLS|RDBI_VA_DEFNULLS)


/* rdbi internal data types :   */

#define RDBI_STRING     7770    /* null-terminated string               */
#define RDBI_CHAR       7771    /* size is obvious                      */
#define RDBI_SHORT      7772
#define RDBI_FIXED_CHAR 7773    /* Must supply a byte_size parameter    */
#define RDBI_INT        7774
#define RDBI_FLOAT      7775
#define RDBI_BLOB       7776    /* Must supply a byte_size parameter    */
#define RDBI_RAW        7777    /* Must supply a byte_size parameter    */
#define RDBI_DOUBLE     7778
#define RDBI_ROWID      7779
#define RDBI_DATE       77710
#define RDBI_LONG       77711   /* Must supply a sizeof(long) parameter */
#define RDBI_GEOMETRY   77712   /* size implicitly sizeof(pIGeometry_def)*/
#define RDBI_BLOB_REF   77713   /* BLOB reference/locator               */
#define RDBI_WSTRING    77714   /* null-terminated wide string          */
#define RDBI_LONGLONG   77715
#define RDBI_BOOLEAN    77716   /* Value storage handled like RDBI_CHAR; must be '1' or '0' */


/* rdbi rdbms data types */

#define RDBI_RDBMS_CHAR     0
#define RDBI_RDBMS_NUMBER   1
#define RDBI_RDBMS_RAW      2
#define RDBI_RDBMS_DATE     3
#define RDBI_RDBMS_ROWID    4
#define RDBI_RDBMS_BLOB     5

#define RDBI_RDBMS_INTEGER  6
#define RDBI_RDBMS_SMALLINT 7
#define RDBI_RDBMS_FLOAT    8
#define RDBI_RDBMS_VARCHAR  9
#define RDBI_RDBMS_MONEY    10

#define RDBI_DATASET_NAME_SIZE      32
#define RDBI_DEFAULT_DATE_SIZE      10
#define RDBI_DEFAULT_ROWID_SIZE     32

#define RDBI_DB_NAME "&&DB" /* replace with database name when found in SQL */
#define	RDBI_DB_NAME_SIZE		32

/************************************************************************
*  Define the maximum message size                                      *
************************************************************************/
#define RDBI_MSG_SIZE       512
/************************************************************************
*  Total number of simultaneous open cursors                            *
************************************************************************/
#define RDBI_MAX_CURSORS    40

/************************************************************************
*  Error codes for context.status                                       *
************************************************************************/
#define RDBI_SUCCESS                            0
#define RDBI_GENERIC_ERROR                      8881    /* No RDBMS equivalent                  */
#define RDBI_MALLOC_FAILED                      8882    /*  "   "       "                       */
#define RDBI_TOO_MANY_CONNECTS                  8883    /*  "   "       "                       */
#define RDBI_END_OF_FETCH                       8884
#define RDBI_DUPLICATE_INDEX                    8885
#define RDBI_INVLD_USER_PSWD                    8886
#define RDBI_INVLD_LKUP_INDEX_TYPE              8887
#define RDBI_NOT_IN_DESC_LIST                   8888    /* when 'position' parm to describe out of bounds   */
#define RDBI_TOO_FEW_CURSORS                    8889
#define RDBI_INVALID_UPDATE                     88810   /* when "where rowid = :b1" is invalid  */
#define RDBI_RESOURCE_LOCK                      88811   /* DDL resource lock encountered        */
#define RDBI_NO_SUCH_TABLE                      88812   /* no such table or view                */
#define RDBI_NOT_CONNECTED                      88813   /* used on an invalid switch() ...      */
#define RDBI_PARSE_ERROR                        88814   /* failed to parse a statement          */
#define RDBI_OBJECT_EXISTS                      88815   /* Table or view already exists         */
#define RDBI_ZERO_LEN_STRING                    88816   /* Tried to bind/define 0 length string */
#define RDBI_INCOMPATIBLE_COLUMN_TYPES          88817   /* Two columns have incompatible types  */
#define RDBI_RESOURCE_BUSY                      88818   /* Resource busy (usually after select  */
                                                        /* for update)                          */
#define RDBI_DATA_TRUNCATED						88819   /* Password file is wrong               */
#define RDBI_GEOMETRY_CONVERION_ERROR           88820   /* Error trying to convert to or from the native geometry */
#define RDBI_NO_SUCH_TABLE_CONSTRAINT           88821   /* No such table constraint             */
#define RDBI_NO_SUCH_CURRENT_OF_CURSOR          88822   /* No such current of cursor found      */
#define RDBI_NO_SUCH_CURRENT_OF_WHERE           88823   /* No such current of where clause      */
#define RDBI_NOT_SELECT_FOR_UPDATE              88824   /* Not a select for update cursor       */
#define RDBI_INVALID_DROP_OPTION                88825   /* Invalid drop option                  */
#define RDBI_NO_SUCH_COLUMN                     88826   /* Invalid column name                  */
#define RDBI_MISSING_DATAFILE                   88827   /* Missing datafile                     */
#define RDBI_INVLD_DESCR_OBJTYPE                88828   /* is not table, view or synonym        */
#define RDBI_VENDOR_SPECIFIC_COLUMN             88829   /* The column is vendor specific (like  */
                                                        /* row id)                              */
#define RDBI_WM_LT_NAME_UNIQUENESS_ERR          88830   /* Long Transaction name is not unique  */
#define RDBI_WM_LT_DOES_NOT_EXIST_ERR           88831   /* Long Transaction does not exist      */
#define RDBI_WM_LT_SESSION_CONFL_ERR            88832   /* Active session for a long transac-   */
                                                        /* tion                                 */
#define RDBI_WM_LT_LIVE_ERR                     88833   /* Root long transaction processing     */
                                                        /* error.                               */
#define RDBI_WM_LT_VERSION_CONFL_ERR            88834   /* Indicates that conflicts exist be-   */
                                                        /* tween a long transaction and its     */
                                                        /* parent.                              */
#define RDBI_WM_LT_LEAF_ERR                     88835   /* The long transaction to be committed */
                                                        /* is not a leaf long transaction.      */
#define RDBI_WM_LK_LOCK_CONFL_ERR               88836   /* Error code if a user tries to lock a */
                                                        /* object that is already locked by a   */
                                                        /* different user.                      */
#define RDBI_WM_LK_VERSION_CONFL_ERR            88837   /* Error code if a user tries to lock a */
                                                        /* object that has already been ver-    */
                                                        /* sioned.                              */
#define RDBI_WM_LT_FREEZE_ERR                   88838   /* Error code if a user tries to invoke */
                                                        /* an operation in a long transaction   */
                                                        /* that is frozen.                      */
#define RDBI_WM_LT_NO_ACCESS_ERR                88839   /* Error code if a user tries to invoke */
                                                        /* an operation in a long transaction   */
                                                        /* that is frozen with the NO_ACCESS    */
                                                        /* option.                              */
#define RDBI_INVALID_OBJECT_FOR_DESC             88840
#define RDBI_WM_VD_TRANSACTION_ERROR             88841  /* Workspace Manager error 20004        */
#define RDBI_WM_VD_ROLE_ERROR                    88842  /* Workspace Manager error 20016        */
#define RDBI_WM_VE_ROLE_ERROR                    88843  /* Workspace Manager error 20017        */
#define RDBI_WM_VE_INDEX_TABLE_ERROR             88844  /* Workspace Manager error 20032        */
#define RDBI_WM_VD_VERSION_CONFL_ERROR           88845  /* Workspace Manager error 20038        */
#define RDBI_WM_VD_SYS_ERROR                     88846  /* Workspace Manager error 20042        */
#define RDBI_WM_VERSIONED_TABLE_EXISTENCE_ERROR  88847  /* Workspace Manager error 20062        */
#define RDBI_WM_VE_HISTORY_ERROR                 88848  /* Workspace Manager error 20066        */
#define RDBI_WM_INTERNAL_ERROR                   88849  /* Workspace Manager error 20079        */
#define RDBI_WM_VE_HISTORY_OPTION_ERROR          88850  /* Workspace Manager error 20080        */
#define RDBI_WM_VE_REF_CONSTRAINTS_1_ERROR       88851  /* Workspace Manager error 20101        */
#define RDBI_WM_VE_REF_CONSTRAINTS_2_ERROR       88852  /* Workspace Manager error 20102        */
#define RDBI_WM_VD_REF_CONSTRAINTS_1_ERROR       88853  /* Workspace Manager error 20103        */
#define RDBI_WM_VD_REF_CONSTRAINTS_2_ERROR       88854  /* Workspace Manager error 20104        */
#define RDBI_WM_VE_UNIQUE_CONSTRAINTS_ERROR      88855  /* Workspace Manager error 20109        */
#define RDBI_WM_TABLE_EXISTENCE_ERROR            88856  /* Workspace Manager error 20129        */
#define RDBI_WM_OPEN_TRANSACTION_ERROR           88857  /* Workspace Manager error 20130        */
#define RDBI_WM_VE_ALREADY_VERSIONED_ERROR       88858  /* Workspace Manager error 20131        */
#define RDBI_WM_VD_NOT_VERSIONED_ERROR           88859  /* Workspace Manager error 20132        */
#define RDBI_WM_VE_NO_PRIMARY_KEY_ERROR          88860  /* Workspace Manager error 20133        */
#define RDBI_WM_VD_ACTIVE_PROCESS_ERROR          88861  /* Workspace Manager error 20134        */
#define RDBI_WM_VE_ACTIVE_PROCESS_ERROR          88862  /* Workspace Manager error 20135        */
#define RDBI_WM_VE_TABLE_NAME_LENGTH_ERROR       88863  /* Workspace Manager error 20136        */
#define RDBI_WM_VD_ACTIVE_SESSION_ERROR          88864  /* Workspace Manager error 20138        */
#define RDBI_WM_WM_ERROR                         88865  /* Workspace Manager error 20171        */
#define RDBI_WM_VE_UNSUPPORTED_DATATYPE_ERROR    88866  /* Workspace Manager error 20174        */
#define RDBI_WM_VE_TRIGGER_COMPILATION_ERROR     88867  /* Workspace Manager error 20176        */
#define RDBI_WM_VE_COMB_COL_NAMES_LENGTH_ERROR   88868  /* Workspace Manager error 20177        */
#define RDBI_WM_VE_COMB_TRIGGER_BODY_ERROR       88869  /* Workspace Manager error 20178        */
#define RDBI_WM_VE_20177_20178_ERROR             88870  /* Workspace Manager error 20179        */
#define RDBI_WM_VE_PK_COLUMN_NUMBER_ERROR        88871  /* Workspace Manager error 20180        */
#define RDBI_WM_VDE_REF_INTEG_CYCLE_ERROR        88872  /* Workspace Manager error 20208        */
#define RDBI_WM_VD_VERSION_CONFL1_ERROR          88873  /* Workspace Manager error 20209        */
#define RDBI_WM_VD_MULTI_LEVEL_REF_INTEG_ERROR   88874  /* Workspace Manager error 20210        */
#define RDBI_WM_ROLE_ERROR                       88875  /* Workspace Manager error 20223        */
#define RDBI_WM_VE_STATEMENT_EXECUTION_ERROR     88876  /* Workspace Manager error 20229        */
#define RDBI_WM_VDE_RECOVERY_ERROR               88877  /* Workspace Manager error 20230        */
#define RDBI_WM_VD_STATEMENT_EXECUTION_ERROR     88878  /* Workspace Manager error 20231        */

#define RDBI_NOT_VALID_USER_IN_DATABASE			 88879  /* SQL Server: user does not have dbase access */

#define RDBI_INSUFFICIENT_PRIVS                  88880  /* SQL Server: user has insufficient privileges */



#endif

