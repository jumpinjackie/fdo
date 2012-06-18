//
//Copyright (C) 2004-2006  Autodesk, Inc.
//
//This library is free software; you can redistribute it and/or
//modify it under the terms of version 2.1 of the GNU Lesser
//General Public License as published by the Free Software Foundation.
//
//This library is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: RDBI_3
//
// MessageText:
//
//  RDBMS: Normal, successful completion.
//
#define RDBI_3                           0x000006DDL

//
// MessageId: RDBI_4
//
// MessageText:
//
//  RDBMS: Memory allocation failure.
//
#define RDBI_4                           0x000006DEL

//
// MessageId: RDBI_5
//
// MessageText:
//
//  RDBMS: Table or view already exists.
//
#define RDBI_5                           0x000006DFL

//
// MessageId: RDBI_6
//
// MessageText:
//
//  RDBMS: Too many connections active - can't establish another.
//
#define RDBI_6                           0x000006E0L

//
// MessageId: RDBI_7
//
// MessageText:
//
//  RDBMS: Cannot allocate another cursor. Must free one first.
//
#define RDBI_7                           0x000006E1L

//
// MessageId: RDBI_8
//
// MessageText:
//
//  RDBMS: Select expression or bind variable does not exist.
//
#define RDBI_8                           0x000006E2L

//
// MessageId: RDBI_9
//
// MessageText:
//
//  RDBMS: Either no row has been fetched, or this is not a 'select for update'.
//
#define RDBI_9                           0x000006E3L

//
// MessageId: RDBI_10
//
// MessageText:
//
//  RDBMS: Not connected to the specified database.
//
#define RDBI_10                          0x000006E4L

//
// MessageId: RDBI_12
//
// MessageText:
//
//  RDBI_12: RDBMS: Failed to bind/define zero length string
//
#define RDBI_12                          0x000006E6L

//
// MessageId: RDBI_13
//
// MessageText:
//
//  RDBMS: Lock conflict with another user
//
#define RDBI_13                          0x000006E7L

//
// MessageId: RDBI_14
//
// MessageText:
//
//  RDBMS: Password file has wrong entry
//
#define RDBI_14                          0x000006E8L

//
// MessageId: RDBI_16
//
// MessageText:
//
//  RDBMS: Failed to locate cursor referenced by current of cursor clause
//
#define RDBI_16                          0x000006EAL

//
// MessageId: RDBI_17
//
// MessageText:
//
//  RDBMS: Cursor referenced by current of cursor clause must be a for update cursor
//
#define RDBI_17                          0x000006EBL

//
// MessageId: RDBI_20
//
// MessageText:
//
//  Illegal (null or empty) transaction id.
//
#define RDBI_20                          0x000006EDL

//
// MessageId: RDBI_21
//
// MessageText:
//
//  No current database is open; cannot begin transaction '%1$ls' .
//
#define RDBI_21                          0x000006EEL

//
// MessageId: RDBI_22
//
// MessageText:
//
//  Illegal (null or empty) transaction id.
//
#define RDBI_22                          0x000006EFL

//
// MessageId: RDBI_23
//
// MessageText:
//
//  There are no transactions to end. ['%1$ls']
//
#define RDBI_23                          0x000006F0L

//
// MessageId: RDBI_24
//
// MessageText:
//
//  RDBI_24: Illegal (null) transaction id pointer.
//
#define RDBI_24                          0x000006F1L

//
// MessageId: RDBI_25
//
// MessageText:
//
//  RDBMS: Incompatible column type
//
#define RDBI_25                          0x000006F2L

//
// MessageId: RDBI_26
//
// MessageText:
//
//  RDBMS: End-of-fetch reached
//
#define RDBI_26                          0x000006F3L

//
// MessageId: RDBI_27
//
// MessageText:
//
//  RDBMS: Failed to locate current of <cursor> clause in SQL statement
//
#define RDBI_27                          0x000006F4L

//
// MessageId: RDBI_29
//
// MessageText:
//
//  Transaction nesting error. '%1$ls'
//
#define RDBI_29                          0x000006F6L

//
// MessageId: RDBI_30
//
// MessageText:
//
//  No current database is open; cannot free cursor '%1$ls'
//
#define RDBI_30                          0x000006F7L

//
// MessageId: RDBI_32
//
// MessageText:
//
//  RDBMS: Invalid object type to describe.
//
#define RDBI_32                          0x000006F9L

//
// MessageId: RDBI_33
//
// MessageText:
//
//  "RDBMS(%1$ls): Geometry conversion error."
//
#define RDBI_33                          0x000006FAL

//
// MessageId: RDBI_34
//
// MessageText:
//
//  RDBMS: %1$ls
//
#define RDBI_34                          0x000006FBL

//
// MessageId: RDBI_35
//
// MessageText:
//
//  RDBMS: Data truncation error.
//
#define RDBI_35                          0x000006FCL

//
// MessageId: FDORDBMS_1
//
// MessageText:
//
//  Different users not allowed for multiple connection
//
#define FDORDBMS_1                       0x0000089EL

//
// MessageId: FDORDBMS_2
//
// MessageText:
//
//  Failed: Cannot get user name: error code
//
#define FDORDBMS_2                       0x0000089FL

//
// MessageId: FDORDBMS_3
//
// MessageText:
//
//  Cannot create datastore, name '%1$ls' is a reserved word
//
#define FDORDBMS_3                       0x000008A0L

//
// MessageId: FDORDBMS_5
//
// MessageText:
//
//  Connection string is not set
//
#define FDORDBMS_5                       0x000008A2L

//
// MessageId: FDORDBMS_6
//
// MessageText:
//
//  Cannot turn on long transactions for table '%1$ls': it is accessed through a database link.
//
#define FDORDBMS_6                       0x000008A3L

//
// MessageId: FDORDBMS_7
//
// MessageText:
//
//  Cannot turn on persistent locking for table '%1$ls': it is accessed through a database link.
//
#define FDORDBMS_7                       0x000008A4L

//
// MessageId: FDORDBMS_10
//
// MessageText:
//
//  Command not supported
//
#define FDORDBMS_10                      0x000008A7L

//
// MessageId: FDORDBMS_11
//
// MessageText:
//
//  Memory error
//
#define FDORDBMS_11                      0x000008A8L

//
// MessageId: FDORDBMS_12
//
// MessageText:
//
//  Connection already opened
//
#define FDORDBMS_12                      0x000008A9L

//
// MessageId: FDORDBMS_13
//
// MessageText:
//
//  Connection not established
//
#define FDORDBMS_13                      0x000008AAL

//
// MessageId: FDORDBMS_14
//
// MessageText:
//
//  Primary key is not defined
//
#define FDORDBMS_14                      0x000008ABL

//
// MessageId: FDORDBMS_15
//
// MessageText:
//
//  Feature ID does not exist
//
#define FDORDBMS_15                      0x000008ACL

//
// MessageId: FDORDBMS_17
//
// MessageText:
//
//  End of feature data or NextFeature not called
//
#define FDORDBMS_17                      0x000008AEL

//
// MessageId: FDORDBMS_18
//
// MessageText:
//
//  End of feature data
//
#define FDORDBMS_18                      0x000008AFL

//
// MessageId: FDORDBMS_19
//
// MessageText:
//
//  Query ended
//
#define FDORDBMS_19                      0x000008B0L

//
// MessageId: FDORDBMS_20
//
// MessageText:
//
//  Case not handled yet
//
#define FDORDBMS_20                      0x000008B1L

//
// MessageId: FDORDBMS_22
//
// MessageText:
//
//  Internal error
//
#define FDORDBMS_22                      0x000008B3L

//
// MessageId: FDORDBMS_23
//
// MessageText:
//
//  Not implemented yet
//
#define FDORDBMS_23                      0x000008B4L

//
// MessageId: FDORDBMS_24
//
// MessageText:
//
//  Expected object or association property
//
#define FDORDBMS_24                      0x000008B5L

//
// MessageId: FDORDBMS_25
//
// MessageText:
//
//  Primary key or foreign columns missing
//
#define FDORDBMS_25                      0x000008B6L

//
// MessageId: FDORDBMS_26
//
// MessageText:
//
//  Primary key and foreign column count should be the same
//
#define FDORDBMS_26                      0x000008B7L

//
// MessageId: FDORDBMS_27
//
// MessageText:
//
//  Unsupported Property mapping type
//
#define FDORDBMS_27                      0x000008B8L

//
// MessageId: FDORDBMS_28
//
// MessageText:
//
//  Property '%1$ls' is not found
//
#define FDORDBMS_28                      0x000008B9L

//
// MessageId: FDORDBMS_29
//
// MessageText:
//
//  Unsupported FDO type in expression
//
#define FDORDBMS_29                      0x000008BAL

//
// MessageId: FDORDBMS_30
//
// MessageText:
//
//  Unknown expression
//
#define FDORDBMS_30                      0x000008BBL

//
// MessageId: FDORDBMS_33
//
// MessageText:
//
//  Unhandled filter condition
//
#define FDORDBMS_33                      0x000008BEL

//
// MessageId: FDORDBMS_34
//
// MessageText:
//
//  Table name or Primary key for class '%1$ls' does not exist
//
#define FDORDBMS_34                      0x000008BFL

//
// MessageId: FDORDBMS_35
//
// MessageText:
//
//  Class is null
//
#define FDORDBMS_35                      0x000008C0L

//
// MessageId: FDORDBMS_36
//
// MessageText:
//
//  Property values collection is not set
//
#define FDORDBMS_36                      0x000008C1L

//
// MessageId: FDORDBMS_37
//
// MessageText:
//
//  Feature class does not have a Geometric property
//
#define FDORDBMS_37                      0x000008C2L

//
// MessageId: FDORDBMS_39
//
// MessageText:
//
//  Property value is NULL
//
#define FDORDBMS_39                      0x000008C4L

//
// MessageId: FDORDBMS_41
//
// MessageText:
//
//  SQL string not initialized
//
#define FDORDBMS_41                      0x000008C6L

//
// MessageId: FDORDBMS_42
//
// MessageText:
//
//  Column %1$ls not found
//
#define FDORDBMS_42                      0x000008C7L

//
// MessageId: FDORDBMS_43
//
// MessageText:
//
//  Query ended
//
#define FDORDBMS_43                      0x000008C8L

//
// MessageId: FDORDBMS_45
//
// MessageText:
//
//  Schema error; no primary key found
//
#define FDORDBMS_45                      0x000008CAL

//
// MessageId: FDORDBMS_46
//
// MessageText:
//
//  No geometry value
//
#define FDORDBMS_46                      0x000008CBL

//
// MessageId: FDORDBMS_47
//
// MessageText:
//
//  UTF8 conversion failed
//
#define FDORDBMS_47                      0x000008CCL

//
// MessageId: FDORDBMS_48
//
// MessageText:
//
//  Unknown database data type
//
#define FDORDBMS_48                      0x000008CDL

//
// MessageId: FDORDBMS_49
//
// MessageText:
//
//  Unknown fdo data type
//
#define FDORDBMS_49                      0x000008CEL

//
// MessageId: FDORDBMS_50
//
// MessageText:
//
//  Invalid or missing username and/or password
//
#define FDORDBMS_50                      0x000008CFL

//
// MessageId: FDORDBMS_51
//
// MessageText:
//
//  not a geometry type
//
#define FDORDBMS_51                      0x000008D0L

//
// MessageId: FDORDBMS_52
//
// MessageText:
//
//  Index out of range
//
#define FDORDBMS_52                      0x000008D1L

//
// MessageId: FDORDBMS_53
//
// MessageText:
//
//  Unsupported Schema Property Type
//
#define FDORDBMS_53                      0x000008D2L

//
// MessageId: FDORDBMS_54
//
// MessageText:
//
//  Unhandled type: %1$d
//
#define FDORDBMS_54                      0x000008D3L

//
// MessageId: FDORDBMS_55
//
// MessageText:
//
//  Unknown dbi error
//
#define FDORDBMS_55                      0x000008D4L

//
// MessageId: FDORDBMS_56
//
// MessageText:
//
//  Property '%1$ls' not found
//
#define FDORDBMS_56                      0x000008D5L

//
// MessageId: FDORDBMS_57
//
// MessageText:
//
//  End of feature data or NextFeature not called
//
#define FDORDBMS_57                      0x000008D6L

//
// MessageId: FDORDBMS_58
//
// MessageText:
//
//  Not supported yet
//
#define FDORDBMS_58                      0x000008D7L

//
// MessageId: FDORDBMS_59
//
// MessageText:
//
//  Property '%1$ls' not defined for class '%2$ls'
//
#define FDORDBMS_59                      0x000008D8L

//
// MessageId: FDORDBMS_60
//
// MessageText:
//
//  Schema not found for class '%1$ls'
//
#define FDORDBMS_60                      0x000008D9L

//
// MessageId: FDORDBMS_61
//
// MessageText:
//
//  Unknown error
//
#define FDORDBMS_61                      0x000008DAL

//
// MessageId: FDORDBMS_62
//
// MessageText:
//
//  End of rows or ReadNext not called
//
#define FDORDBMS_62                      0x000008DBL

//
// MessageId: FDORDBMS_63
//
// MessageText:
//
//  Filter processing error
//
#define FDORDBMS_63                      0x000008DCL

//
// MessageId: FDORDBMS_64
//
// MessageText:
//
//  Geometry property not supported on non FeatureClass objects
//
#define FDORDBMS_64                      0x000008DDL

//
// MessageId: FDORDBMS_65
//
// MessageText:
//
//  Property '%1$ls' is not an object or association property type
//
#define FDORDBMS_65                      0x000008DEL

//
// MessageId: FDORDBMS_66
//
// MessageText:
//
//  Missing or badly defined target class for property '%1$ls' of class '%2$ls'
//
#define FDORDBMS_66                      0x000008DFL

//
// MessageId: FDORDBMS_67
//
// MessageText:
//
//  Property '%1$ls' is an object property and cannot be returned through a basic type; use GetObject
//
#define FDORDBMS_67                      0x000008E0L

//
// MessageId: FDORDBMS_68
//
// MessageText:
//
//  Operation requires administrator role
//
#define FDORDBMS_68                      0x000008E1L

//
// MessageId: FDORDBMS_69
//
// MessageText:
//
//  Failed to retrieve class name
//
#define FDORDBMS_69                      0x000008E2L

//
// MessageId: FDORDBMS_70
//
// MessageText:
//
//  Failed to clear lock report
//
#define FDORDBMS_70                      0x000008E3L

//
// MessageId: FDORDBMS_71
//
// MessageText:
//
//  Failed to close lock conflict reader
//
#define FDORDBMS_71                      0x000008E4L

//
// MessageId: FDORDBMS_72
//
// MessageText:
//
//  Failed to close lock object reader
//
#define FDORDBMS_72                      0x000008E5L

//
// MessageId: FDORDBMS_73
//
// MessageText:
//
//  Failed to close lock owners reader
//
#define FDORDBMS_73                      0x000008E6L

//
// MessageId: FDORDBMS_74
//
// MessageText:
//
//  Failed to create an identity object
//
#define FDORDBMS_74                      0x000008E7L

//
// MessageId: FDORDBMS_75
//
// MessageText:
//
//  DBI lock error
//
#define FDORDBMS_75                      0x000008E8L

//
// MessageId: FDORDBMS_76
//
// MessageText:
//
//  Failed to construct command to retrieve lock information in DBI
//
#define FDORDBMS_76                      0x000008E9L

//
// MessageId: FDORDBMS_77
//
// MessageText:
//
//  DBI unlock error
//
#define FDORDBMS_77                      0x000008EAL

//
// MessageId: FDORDBMS_78
//
// MessageText:
//
//  Error during initial load of lock conflict information
//
#define FDORDBMS_78                      0x000008EBL

//
// MessageId: FDORDBMS_79
//
// MessageText:
//
//  Error during initial load of lock info data
//
#define FDORDBMS_79                      0x000008ECL

//
// MessageId: FDORDBMS_80
//
// MessageText:
//
//  Error during initial load of lock owner information
//
#define FDORDBMS_80                      0x000008EDL

//
// MessageId: FDORDBMS_81
//
// MessageText:
//
//  Failed to retrieve lock info data
//
#define FDORDBMS_81                      0x000008EEL

//
// MessageId: FDORDBMS_82
//
// MessageText:
//
//  Failed to retrieve identity
//
#define FDORDBMS_82                      0x000008EFL

//
// MessageId: FDORDBMS_83
//
// MessageText:
//
//  Invalid class name
//
#define FDORDBMS_83                      0x000008F0L

//
// MessageId: FDORDBMS_84
//
// MessageText:
//
//  Invalid DBI connection
//
#define FDORDBMS_84                      0x000008F1L

//
// MessageId: FDORDBMS_85
//
// MessageText:
//
//  Failed to create lock conflict report
//
#define FDORDBMS_85                      0x000008F2L

//
// MessageId: FDORDBMS_86
//
// MessageText:
//
//  Lock execution error
//
#define FDORDBMS_86                      0x000008F3L

//
// MessageId: FDORDBMS_87
//
// MessageText:
//
//  Failed to create a lock object reader
//
#define FDORDBMS_87                      0x000008F4L

//
// MessageId: FDORDBMS_88
//
// MessageText:
//
//  Failed to create a lock owner reader
//
#define FDORDBMS_88                      0x000008F5L

//
// MessageId: FDORDBMS_89
//
// MessageText:
//
//  Failed to retrieve lock owner
//
#define FDORDBMS_89                      0x000008F6L

//
// MessageId: FDORDBMS_90
//
// MessageText:
//
//  Failed to retrieve lock type
//
#define FDORDBMS_90                      0x000008F7L

//
// MessageId: FDORDBMS_91
//
// MessageText:
//
//  Failed to allocate memory
//
#define FDORDBMS_91                      0x000008F8L

//
// MessageId: FDORDBMS_92
//
// MessageText:
//
//  Failed to set the lock request command
//
#define FDORDBMS_92                      0x000008F9L

//
// MessageId: FDORDBMS_93
//
// MessageText:
//
//  Failed to read next lock conflict data set
//
#define FDORDBMS_93                      0x000008FAL

//
// MessageId: FDORDBMS_94
//
// MessageText:
//
//  Failed to read next lock info data set
//
#define FDORDBMS_94                      0x000008FBL

//
// MessageId: FDORDBMS_95
//
// MessageText:
//
//  Failed to read next owner data set
//
#define FDORDBMS_95                      0x000008FCL

//
// MessageId: FDORDBMS_96
//
// MessageText:
//
//  Unlock execution error
//
#define FDORDBMS_96                      0x000008FDL

//
// MessageId: FDORDBMS_97
//
// MessageText:
//
//  Failed to construct command to retrieve lock information in Oracle Workspace Manager
//
#define FDORDBMS_97                      0x000008FEL

//
// MessageId: FDORDBMS_98
//
// MessageText:
//
//  Oracle Workspace Manager lock error
//
#define FDORDBMS_98                      0x000008FFL

//
// MessageId: FDORDBMS_99
//
// MessageText:
//
//  Oracle Workspace Manager unlock error
//
#define FDORDBMS_99                      0x00000900L

//
// MessageId: FDORDBMS_100
//
// MessageText:
//
//  Unknown error code
//
#define FDORDBMS_100                     0x00000901L

//
// MessageId: FDORDBMS_101
//
// MessageText:
//
//  Failed to execute a DBI command
//
#define FDORDBMS_101                     0x00000902L

//
// MessageId: FDORDBMS_102
//
// MessageText:
//
//  Conversion error
//
#define FDORDBMS_102                     0x00000903L

//
// MessageId: FDORDBMS_103
//
// MessageText:
//
//  Invalid parameter
//
#define FDORDBMS_103                     0x00000904L

//
// MessageId: FDORDBMS_104
//
// MessageText:
//
//  Process class request error
//
#define FDORDBMS_104                     0x00000905L

//
// MessageId: FDORDBMS_105
//
// MessageText:
//
//  Failed to maintain lock data
//
#define FDORDBMS_105                     0x00000906L

//
// MessageId: FDORDBMS_106
//
// MessageText:
//
//  Process feature class request error
//
#define FDORDBMS_106                     0x00000907L

//
// MessageId: FDORDBMS_107
//
// MessageText:
//
//  Foreign column missing
//
#define FDORDBMS_107                     0x00000908L

//
// MessageId: FDORDBMS_109
//
// MessageText:
//
//  LineString must have >= 2 points
//
#define FDORDBMS_109                     0x0000090AL

//
// MessageId: FDORDBMS_110
//
// MessageText:
//
//  Unsupported geometry as spatial constraint
//
#define FDORDBMS_110                     0x0000090BL

//
// MessageId: FDORDBMS_111
//
// MessageText:
//
//  Unsupported spatial operation
//
#define FDORDBMS_111                     0x0000090CL

//
// MessageId: FDORDBMS_112
//
// MessageText:
//
//  Failed to create a lock conflict object
//
#define FDORDBMS_112                     0x0000090DL

//
// MessageId: FDORDBMS_113
//
// MessageText:
//
//  Failed to initialize a lock conflict object
//
#define FDORDBMS_113                     0x0000090EL

//
// MessageId: FDORDBMS_114
//
// MessageText:
//
//  Unsupported distance operation
//
#define FDORDBMS_114                     0x0000090FL

//
// MessageId: FDORDBMS_115
//
// MessageText:
//
//  Unsupported geometry as distance constraint
//
#define FDORDBMS_115                     0x00000910L

//
// MessageId: FDORDBMS_116
//
// MessageText:
//
//  Unsupported geometry type
//
#define FDORDBMS_116                     0x00000911L

//
// MessageId: FDORDBMS_117
//
// MessageText:
//
//  DataStore
//
#define FDORDBMS_117                     0x00000912L

//
// MessageId: FDORDBMS_118
//
// MessageText:
//
//  Username
//
#define FDORDBMS_118                     0x00000913L

//
// MessageId: FDORDBMS_119
//
// MessageText:
//
//  Password
//
#define FDORDBMS_119                     0x00000914L

//
// MessageId: FDORDBMS_120
//
// MessageText:
//
//  Service
//
#define FDORDBMS_120                     0x00000915L

//
// MessageId: FDORDBMS_121
//
// MessageText:
//
//  Not all the required properties are set: '%1$ls'
//
#define FDORDBMS_121                     0x00000916L

//
// MessageId: FDORDBMS_122
//
// MessageText:
//
//  Cannot create sequence %1$ls.%2$ls, an object of that name already exists.
//
#define FDORDBMS_122                     0x00000917L

//
// MessageId: FDORDBMS_123
//
// MessageText:
//
//  Cannot create synonym %1$ls.%2$ls, an object of that name already exists.
//
#define FDORDBMS_123                     0x00000918L

//
// MessageId: FDORDBMS_124
//
// MessageText:
//
//  Error modifying table '%1$ls': changing tablespace for existing table not supported.
//
#define FDORDBMS_124                     0x00000919L

//
// MessageId: FDORDBMS_125
//
// MessageText:
//
//  Field '%1$ls' is not retrieved by query: '%2$ls'.
//
#define FDORDBMS_125                     0x0000091AL

//
// MessageId: FDORDBMS_126
//
// MessageText:
//
//  Free message, pleas re-use.
//
#define FDORDBMS_126                     0x0000091BL

//
// MessageId: FDORDBMS_127
//
// MessageText:
//
//  Error in sequence for property '%1$ls': cannot create synonym for non-existent foreign sequence %2$ls.
//
#define FDORDBMS_127                     0x0000091CL

//
// MessageId: FDORDBMS_128
//
// MessageText:
//
//  Error creating synonym '%1$ls': referenced database object not specified.
//
#define FDORDBMS_128                     0x0000091DL

//
// MessageId: FDORDBMS_129
//
// MessageText:
//
//  Error loading synonym '%1$ls': cannot change referenced database object.
//
#define FDORDBMS_129                     0x0000091EL

//
// MessageId: FDORDBMS_130
//
// MessageText:
//
//  Error loading synonym '%1$ls': not present in all_synonyms view.
//
#define FDORDBMS_130                     0x0000091FL

//
// MessageId: FDORDBMS_131
//
// MessageText:
//
//  Cannot add spatial index to column %1$ls, it already has one.
//
#define FDORDBMS_131                     0x00000920L

//
// MessageId: FDORDBMS_132
//
// MessageText:
//
//  Cannot modify index type for spatial index %1$ls, modification not supported.
//
#define FDORDBMS_132                     0x00000921L

//
// MessageId: FDORDBMS_133
//
// MessageText:
//
//  Cannot modify dimensionality for spatial index %1$ls, modification not supported.
//
#define FDORDBMS_133                     0x00000922L

//
// MessageId: FDORDBMS_134
//
// MessageText:
//
//  Spatial Index '%1$ls' must have a single column (currently has 0 or multiple columns).
//
#define FDORDBMS_134                     0x00000923L

//
// MessageId: FDORDBMS_135
//
// MessageText:
//
//  Column '%1$ls' for Spatial Index '%2$ls' is not geometric.
//
#define FDORDBMS_135                     0x00000924L

//
// MessageId: FDORDBMS_136
//
// MessageText:
//
//  Error creating spatial context %1$ls, coordinate system %2$ls is not in current datastore.
//
#define FDORDBMS_136                     0x00000925L

//
// MessageId: FDORDBMS_137
//
// MessageText:
//
//  Attribute Dependency from %1$ls to %2$ls contains non-existing FkColumn %3$ls
//
#define FDORDBMS_137                     0x00000926L

//
// MessageId: FDORDBMS_138
//
// MessageText:
//
//  Primary/Foreign column number mismatched for OWM-enabled foreign key constraint '%1$ls'
//
#define FDORDBMS_138                     0x00000927L

//
// MessageId: FDORDBMS_139
//
// MessageText:
//
//  Cannot add not null column to table %1$ls; the table is OWM-enabled and has foreign keys
//
#define FDORDBMS_139                     0x00000928L

//
// MessageId: FDORDBMS_140
//
// MessageText:
//
//  Cannot OWM-enable table %1$ls in server that does not support Oracle Workspace Manager
//
#define FDORDBMS_140                     0x00000929L

//
// MessageId: FDORDBMS_141
//
// MessageText:
//
//  Field %1$ls has unsupported RDBI type (%2$d)
//
#define FDORDBMS_141                     0x0000092AL

//
// MessageId: FDORDBMS_142
//
// MessageText:
//
//  Could not open file: '%1$ls'
//
#define FDORDBMS_142                     0x0000092BL

//
// MessageId: FDORDBMS_143
//
// MessageText:
//
//  #else without #if[n]def in file: '%1$ls'
//
#define FDORDBMS_143                     0x0000092CL

//
// MessageId: FDORDBMS_144
//
// MessageText:
//
//  #endif without #if[n]def in file: '%1$ls'
//
#define FDORDBMS_144                     0x0000092DL

//
// MessageId: FDORDBMS_145
//
// MessageText:
//
//  Unknown directive '%1$ls' in file: '%2$ls'
//
#define FDORDBMS_145                     0x0000092EL

//
// MessageId: FDORDBMS_146
//
// MessageText:
//
//  One or more missing #endifs in file: '%1$ls'
//
#define FDORDBMS_146                     0x0000092FL

//
// MessageId: FDORDBMS_147
//
// MessageText:
//
//  Cannot modify column '%1$ls', operation is not supported.
//
#define FDORDBMS_147                     0x00000930L

//
// MessageId: FDORDBMS_148
//
// MessageText:
//
//  Cannot create view on %1$ls.%2$ls.%3$ls; %4$ls Provider does not support views on objects on other database servers.
//
#define FDORDBMS_148                     0x00000931L

//
// MessageId: FDORDBMS_163
//
// MessageText:
//
//  Cannot destroy schema; no schema name specified
//
#define FDORDBMS_163                     0x00000940L

//
// MessageId: FDORDBMS_168
//
// MessageText:
//
//  Cannot apply schema, no schema was supplied
//
#define FDORDBMS_168                     0x00000945L

//
// MessageId: FDORDBMS_182
//
// MessageText:
//
//  Unable to find schema
//
#define FDORDBMS_182                     0x00000953L

//
// MessageId: FDORDBMS_187
//
// MessageText:
//
//  Schema error; Feature class %1$ls has no feature id property
//
#define FDORDBMS_187                     0x00000958L

//
// MessageId: FDORDBMS_211
//
// MessageText:
//
//  Reader is not positioned at a row
//
#define FDORDBMS_211                     0x00000970L

//
// MessageId: FDORDBMS_230
//
// MessageText:
//
//  Spatial condition can only be used with feature classes
//
#define FDORDBMS_230                     0x00000983L

//
// MessageId: FDORDBMS_231
//
// MessageText:
//
//  Object property '%1$ls' not found
//
#define FDORDBMS_231                     0x00000984L

//
// MessageId: FDORDBMS_239
//
// MessageText:
//
//  Invalid lock type '%1$ls' for command '%2$ls'
//
#define FDORDBMS_239                     0x0000098DL

//
// MessageId: FDORDBMS_240
//
// MessageText:
//
//  Function not supported by this reader
//
#define FDORDBMS_240                     0x0000098EL

//
// MessageId: FDORDBMS_248
//
// MessageText:
//
//  FdoInCondition is missing the property name
//
#define FDORDBMS_248                     0x0000099CL

//
// MessageId: FDORDBMS_249
//
// MessageText:
//
//  FdoInCondition has an empty value list
//
#define FDORDBMS_249                     0x0000099DL

//
// MessageId: FDORDBMS_250
//
// MessageText:
//
//  FdoUnaryLogicalOperator is missing the operand
//
#define FDORDBMS_250                     0x0000099EL

//
// MessageId: FDORDBMS_251
//
// MessageText:
//
//  FdoUnaryLogicalOperator supports only the 'Not' operation
//
#define FDORDBMS_251                     0x0000099FL

//
// MessageId: FDORDBMS_252
//
// MessageText:
//
//  FdoComparisonCondition is missing the left expression
//
#define FDORDBMS_252                     0x000009A0L

//
// MessageId: FDORDBMS_253
//
// MessageText:
//
//  FdoComparisonCondition is missing the right expression
//
#define FDORDBMS_253                     0x000009A1L

//
// MessageId: FDORDBMS_254
//
// MessageText:
//
//  FdoBinaryLogicalOperator is missing the left operand
//
#define FDORDBMS_254                     0x000009A2L

//
// MessageId: FDORDBMS_255
//
// MessageText:
//
//  FdoBinaryLogicalOperator is missing the right operand
//
#define FDORDBMS_255                     0x000009A3L

//
// MessageId: FDORDBMS_256
//
// MessageText:
//
//  FdoUnaryExpression is missing the expression
//
#define FDORDBMS_256                     0x000009A4L

//
// MessageId: FDORDBMS_257
//
// MessageText:
//
//  FdoUnaryExpression supports only the negate operation
//
#define FDORDBMS_257                     0x000009A5L

//
// MessageId: FDORDBMS_260
//
// MessageText:
//
//  Unable to get exclusive access to one or more features
//
#define FDORDBMS_260                     0x000009A8L

//
// MessageId: FDORDBMS_261
//
// MessageText:
//
//  Property '%1$ls' not selected
//
#define FDORDBMS_261                     0x000009A9L

//
// MessageId: FDORDBMS_262
//
// MessageText:
//
//  Table for class '%1$ls' does not exist
//
#define FDORDBMS_262                     0x000009AAL

//
// MessageId: FDORDBMS_265
//
// MessageText:
//
//  Table does not exist for class '%1$ls'
//
#define FDORDBMS_265                     0x000009ADL

//
// MessageId: FDORDBMS_266
//
// MessageText:
//
//  Creating/Updating a standalone instance for class '%1$ls' is not allowed
//
#define FDORDBMS_266                     0x000009AEL

//
// MessageId: FDORDBMS_267
//
// MessageText:
//
//  Column does not exist for property '%1$ls'
//
#define FDORDBMS_267                     0x000009AFL

//
// MessageId: FDORDBMS_268
//
// MessageText:
//
//  Table does not exist for object property '%1$ls'
//
#define FDORDBMS_268                     0x000009B0L

//
// MessageId: FDORDBMS_273
//
// MessageText:
//
//  Class name size exceeds the internal storage limit
//
#define FDORDBMS_273                     0x000009B5L

//
// MessageId: FDORDBMS_276
//
// MessageText:
//
//  Setting an abstract class is not currently supported for this command
//
#define FDORDBMS_276                     0x000009B9L

//
// MessageId: FDORDBMS_289
//
// MessageText:
//
//  Association property is required
//
#define FDORDBMS_289                     0x000009C8L

//
// MessageId: FDORDBMS_290
//
// MessageText:
//
//  Cannot set a read only association property
//
#define FDORDBMS_290                     0x000009C9L

//
// MessageId: FDORDBMS_291
//
// MessageText:
//
//  Association property '%1$ls' and property '%2$ls' must have the same value or only one should be set
//
#define FDORDBMS_291                     0x000009CAL

//
// MessageId: FDORDBMS_292
//
// MessageText:
//
//  Association identity properties and identity columns mismatch
//
#define FDORDBMS_292                     0x000009CBL

//
// MessageId: FDORDBMS_293
//
// MessageText:
//
//  Property type and value type mismatch for property %1$ls
//
#define FDORDBMS_293                     0x000009CCL

//
// MessageId: FDORDBMS_294
//
// MessageText:
//
//  Invalid Oracle Schema Mapping table mapping type '%1$ls'
//
#define FDORDBMS_294                     0x000009CDL

//
// MessageId: FDORDBMS_295
//
// MessageText:
//
//  Invalid Oracle Schema Mapping column type '%1$ls'
//
#define FDORDBMS_295                     0x000009CEL

//
// MessageId: FDORDBMS_296
//
// MessageText:
//
//  Invalid Oracle Schema Mapping table mapping type %1$d
//
#define FDORDBMS_296                     0x000009CFL

//
// MessageId: FDORDBMS_299
//
// MessageText:
//
//  Cannot change column name for property %1$ls from %2$ls to %3$ls
//
#define FDORDBMS_299                     0x000009D2L

//
// MessageId: FDORDBMS_308
//
// MessageText:
//
//  Cannot generate column name for %1$ls
//
#define FDORDBMS_308                     0x000009DBL

//
// MessageId: FDORDBMS_312
//
// MessageText:
//
//  Column %1$ls is not in the current datastore; datastore schema needs to be upgraded 
//
#define FDORDBMS_312                     0x000009DFL

//
// MessageId: FDORDBMS_318
//
// MessageText:
//
//  Associated objects need to be deleted first
//
#define FDORDBMS_318                     0x000009E5L

//
// MessageId: FDORDBMS_322
//
// MessageText:
//
//  Spatial Context '%1$ls' not found
//
#define FDORDBMS_322                     0x000009E9L

//
// MessageId: FDORDBMS_323
//
// MessageText:
//
//  Spatial Index creation failed. The specified geometry property is not associated to Spatial Context '%1$ls'
//
#define FDORDBMS_323                     0x000009EAL

//
// MessageId: FDORDBMS_324
//
// MessageText:
//
//  Spatial Index '%1$ls' not found
//
#define FDORDBMS_324                     0x000009EBL

//
// MessageId: FDORDBMS_325
//
// MessageText:
//
//  Active Spatial Context not found
//
#define FDORDBMS_325                     0x000009ECL

//
// MessageId: FDORDBMS_326
//
// MessageText:
//
//  The geometry property specified has no Spatial Index
//
#define FDORDBMS_326                     0x000009EDL

//
// MessageId: FDORDBMS_327
//
// MessageText:
//
//  Spatial Index '%1$ls' unrelated to the specified geometry property
//
#define FDORDBMS_327                     0x000009EEL

//
// MessageId: FDORDBMS_328
//
// MessageText:
//
//  Cannot create Spatial Index '%1$ls', it already exists
//
#define FDORDBMS_328                     0x000009EFL

//
// MessageId: FDORDBMS_329
//
// MessageText:
//
//  Ordinate dimensionality mismatch on geometric property '%1$ls' of the feature class '%2$ls'. Only XY and XYZ allowed
//
#define FDORDBMS_329                     0x000009F0L

//
// MessageId: FDORDBMS_330
//
// MessageText:
//
//  Ordinate dimensionality mismatch on geometric property '%1$ls'
//
#define FDORDBMS_330                     0x000009F1L

//
// MessageId: FDORDBMS_331
//
// MessageText:
//
//  No conflict view name available
//
#define FDORDBMS_331                     0x000009F2L

//
// MessageId: FDORDBMS_332
//
// MessageText:
//
//  Failed to get exclusive access to specified objects
//
#define FDORDBMS_332                     0x000009F3L

//
// MessageId: FDORDBMS_333
//
// MessageText:
//
//  Class '%1$ls' not found
//
#define FDORDBMS_333                     0x000009F4L

//
// MessageId: FDORDBMS_352
//
// MessageText:
//
//  Invalid Long Transaction Name; cannot create long transaction
//
#define FDORDBMS_352                     0x00000A07L

//
// MessageId: FDORDBMS_353
//
// MessageText:
//
//  Invalid context for command '%1$ls'
//
#define FDORDBMS_353                     0x00000A08L

//
// MessageId: FDORDBMS_354
//
// MessageText:
//
//  Unsupported Oracle version (<9i) detected for command '%1$ls'
//
#define FDORDBMS_354                     0x00000A09L

//
// MessageId: FDORDBMS_355
//
// MessageText:
//
//  Unsupported lock type '%1$ls' for command '%2$ls'
//
#define FDORDBMS_355                     0x00000A0AL

//
// MessageId: FDORDBMS_356
//
// MessageText:
//
//  Error during initial load of long transaction info data
//
#define FDORDBMS_356                     0x00000A0BL

//
// MessageId: FDORDBMS_357
//
// MessageText:
//
//  Long transaction description too long (max: 255)
//
#define FDORDBMS_357                     0x00000A0CL

//
// MessageId: FDORDBMS_358
//
// MessageText:
//
//  Invalid long transaction name '%1$ls'
//
#define FDORDBMS_358                     0x00000A0DL

//
// MessageId: FDORDBMS_359
//
// MessageText:
//
//  Long transaction name cannot be null
//
#define FDORDBMS_359                     0x00000A0EL

//
// MessageId: FDORDBMS_360
//
// MessageText:
//
//  Failed to read next long transaction info data set
//
#define FDORDBMS_360                     0x00000A0FL

//
// MessageId: FDORDBMS_361
//
// MessageText:
//
//  %1$ls: Long transaction '%2$ls' does not exist
//
#define FDORDBMS_361                     0x00000A10L

//
// MessageId: FDORDBMS_362
//
// MessageText:
//
//  Long transaction name '%1$ls' not unique; cannot create
//
#define FDORDBMS_362                     0x00000A11L

//
// MessageId: FDORDBMS_363
//
// MessageText:
//
//  Invalid long transaction name ('%1$ls') for command '%2$ls'
//
#define FDORDBMS_363                     0x00000A12L

//
// MessageId: FDORDBMS_364
//
// MessageText:
//
//  Table %1$ls is not in the current datastore; datastore schema needs to be upgraded 
//
#define FDORDBMS_364                     0x00000A13L

//
// MessageId: FDORDBMS_366
//
// MessageText:
//
//  %1$ls: Cannot process active long transaction
//
#define FDORDBMS_366                     0x00000A1EL

//
// MessageId: FDORDBMS_367
//
// MessageText:
//
//  %1$ls: Cannot process root long transaction
//
#define FDORDBMS_367                     0x00000A1FL

//
// MessageId: FDORDBMS_368
//
// MessageText:
//
//  %1$ls: Long transaction '%2$ls' or descendent active in a session
//
#define FDORDBMS_368                     0x00000A20L

//
// MessageId: FDORDBMS_370
//
// MessageText:
//
//  The spatial context '%1$ls' of the geometry property '%2$ls' of the feature class '%3$ls' does not match the active spatial context '%4$ls'
//
#define FDORDBMS_370                     0x00000A22L

//
// MessageId: FDORDBMS_375
//
// MessageText:
//
//  Failed to create long transaction conflict enumerator
//
#define FDORDBMS_375                     0x00000A27L

//
// MessageId: FDORDBMS_376
//
// MessageText:
//
//  Invalid conflict solution flags in long transaction conflict enumerator
//
#define FDORDBMS_376                     0x00000A28L

//
// MessageId: FDORDBMS_377
//
// MessageText:
//
//  %1$ls: Long transaction '%2$ls' is not a leaf long transaction
//
#define FDORDBMS_377                     0x00000A29L

//
// MessageId: FDORDBMS_379
//
// MessageText:
//
//  Failed to create a FdoILockedObjectsReader object
//
#define FDORDBMS_379                     0x00000A2BL

//
// MessageId: FDORDBMS_380
//
// MessageText:
//
//  Invalid lock owner specification
//
#define FDORDBMS_380                     0x00000A2CL

//
// MessageId: FDORDBMS_381
//
// MessageText:
//
//  Failed execute read request on FdoIGetLockedObjectsReader
//
#define FDORDBMS_381                     0x00000A2DL

//
// MessageId: FDORDBMS_383
//
// MessageText:
//
//  Reader '%1$ls' is not positioned correctly
//
#define FDORDBMS_383                     0x00000A2FL

//
// MessageId: FDORDBMS_384
//
// MessageText:
//
//  %1$ls: Locking is not supported for either the connection or class (%2$ls)
//
#define FDORDBMS_384                     0x00000A30L

//
// MessageId: FDORDBMS_385
//
// MessageText:
//
//  Property '%1$ls' value is NULL; use IsNull method before trying to access the property value
//
#define FDORDBMS_385                     0x00000A31L

//
// MessageId: FDORDBMS_386
//
// MessageText:
//
//  Column '%1$ls' value is NULL; use IsNull method before trying to access this column value
//
#define FDORDBMS_386                     0x00000A32L

//
// MessageId: FDORDBMS_387
//
// MessageText:
//
//  Connection already has an active transaction
//
#define FDORDBMS_387                     0x00000A33L

//
// MessageId: FDORDBMS_388
//
// MessageText:
//
//  Schema error class '%1$ls': a row must be unique in order to insert a streamed LOB
//
#define FDORDBMS_388                     0x00000A34L

//
// MessageId: FDORDBMS_389
//
// MessageText:
//
//  Identity properties are not updateable
//
#define FDORDBMS_389                     0x00000A35L

//
// MessageId: FDORDBMS_390
//
// MessageText:
//
//  Shape type mismatch on geometric property '%1$ls' of the feature class '%2$ls'.
//
#define FDORDBMS_390                     0x00000A36L

//
// MessageId: FDORDBMS_392
//
// MessageText:
//
//  A value is required for the not nullable property %1$ls
//
#define FDORDBMS_392                     0x00000A38L

//
// MessageId: FDORDBMS_393
//
// MessageText:
//
//  Property %1$ls is not user modifiable
//
#define FDORDBMS_393                     0x00000A39L

//
// MessageId: FDORDBMS_394
//
// MessageText:
//
//  Failed to retrieve long transaction information
//
#define FDORDBMS_394                     0x00000A3AL

//
// MessageId: FDORDBMS_395
//
// MessageText:
//
//  Error in sequence for property '%1$ls': %2$ls is not a sequence.
//
#define FDORDBMS_395                     0x00000A3BL

//
// MessageId: FDORDBMS_396
//
// MessageText:
//
//  Error in foreign sequence for property '%1$ls': %2$ls is not a synonym.
//
#define FDORDBMS_396                     0x00000A3CL

//
// MessageId: FDORDBMS_397
//
// MessageText:
//
//  Error in foreign sequence for property '%1$ls': %2$ls is not a synonym for 3$ls.
//
#define FDORDBMS_397                     0x00000A3DL

//
// MessageId: FDORDBMS_399
//
// MessageText:
//
//  Failed to lock object(s) due to a lock conflict
//
#define FDORDBMS_399                     0x00000A3FL

//
// MessageId: FDORDBMS_400
//
// MessageText:
//
//  Failed to lock object(s) due to a version conflict
//
#define FDORDBMS_400                     0x00000A40L

//
// MessageId: FDORDBMS_401
//
// MessageText:
//
//  Invalid geometry value on geometric property '%1$ls' of the feature class '%2$ls'. It contains arc(s) with collinear points
//
#define FDORDBMS_401                     0x00000A41L

//
// MessageId: FDORDBMS_402
//
// MessageText:
//
//  %1$ls: Failed to load version conflicts
//
#define FDORDBMS_402                     0x00000A42L

//
// MessageId: FDORDBMS_403
//
// MessageText:
//
//  %1$ls: Version conflicts caused by Oracle data from the following Oracle users outside current data store: %2$ls
//
#define FDORDBMS_403                     0x00000A43L

//
// MessageId: FDORDBMS_406
//
// MessageText:
//
//  Property '%1$ls' from class '%2$ls' has no database mapping
//
#define FDORDBMS_406                     0x00000A46L

//
// MessageId: FDORDBMS_407
//
// MessageText:
//
//  Attempt to associate an instance of class '%1$ls' to a non-existing instance of class '%2$ls'
//
#define FDORDBMS_407                     0x00000A47L

//
// MessageId: FDORDBMS_408
//
// MessageText:
//
//  An instance of class '%1$ls' is already associated to the same instance of class '%2$ls'
//
#define FDORDBMS_408                     0x00000A48L

//
// MessageId: FDORDBMS_409
//
// MessageText:
//
//  Long transaction '%1$ls' does not exist
//
#define FDORDBMS_409                     0x00000A49L

//
// MessageId: FDORDBMS_410
//
// MessageText:
//
//  Cannot process active long transaction
//
#define FDORDBMS_410                     0x00000A4AL

//
// MessageId: FDORDBMS_411
//
// MessageText:
//
//  Cannot process root long transaction
//
#define FDORDBMS_411                     0x00000A4BL

//
// MessageId: FDORDBMS_412
//
// MessageText:
//
//  Long transaction '%1$ls' is not a leaf long transaction
//
#define FDORDBMS_412                     0x00000A4CL

//
// MessageId: FDORDBMS_413
//
// MessageText:
//
//  Long transaction '%1$ls' or descendent active in a session
//
#define FDORDBMS_413                     0x00000A4DL

//
// MessageId: FDORDBMS_414
//
// MessageText:
//
//  Version conflicts caused by data from the following users outside current data store: %1$ls
//
#define FDORDBMS_414                     0x00000A4EL

//
// MessageId: FDORDBMS_415
//
// MessageText:
//
//  Failed to load version conflicts
//
#define FDORDBMS_415                     0x00000A4FL

//
// MessageId: FDORDBMS_416
//
// MessageText:
//
//  Invalid user session id
//
#define FDORDBMS_416                     0x00000A50L

//
// MessageId: FDORDBMS_417
//
// MessageText:
//
//  %1$ls: A generic error has been identified
//
#define FDORDBMS_417                     0x00000A51L

//
// MessageId: FDORDBMS_418
//
// MessageText:
//
//  %1$ls: The long transaction '%2$ls' is frozen
//
#define FDORDBMS_418                     0x00000A52L

//
// MessageId: FDORDBMS_419
//
// MessageText:
//
//  %1$ls: The long transaction '%2$ls' is inaccessible
//
#define FDORDBMS_419                     0x00000A53L

//
// MessageId: FDORDBMS_420
//
// MessageText:
//
//  %1$ls: The long transaction '%2$ls' and/or one of its descendents does not exist
//
#define FDORDBMS_420                     0x00000A54L

//
// MessageId: FDORDBMS_421
//
// MessageText:
//
//  %1$ls: The long transaction '%2$ls' and/or one of its descendents is frozen
//
#define FDORDBMS_421                     0x00000A55L

//
// MessageId: FDORDBMS_422
//
// MessageText:
//
//  %1$ls: The long transaction '%2$ls' and/or one of its descendents is inaccessible
//
#define FDORDBMS_422                     0x00000A56L

//
// MessageId: FDORDBMS_423
//
// MessageText:
//
//  Failed to activate long transaction '%1$ls' in context
//
#define FDORDBMS_423                     0x00000A57L

//
// MessageId: FDORDBMS_424
//
// MessageText:
//
//  Long transaction '%1$ls' is locked and cannot be activated
//
#define FDORDBMS_424                     0x00000A58L

//
// MessageId: FDORDBMS_426
//
// MessageText:
//
//  Long transaction context cache not initialized
//
#define FDORDBMS_426                     0x00000A5AL

//
// MessageId: FDORDBMS_427
//
// MessageText:
//
//  Failed to add '%1ls' to long transaction context cache
//
#define FDORDBMS_427                     0x00000A5BL

//
// MessageId: FDORDBMS_428
//
// MessageText:
//
//  Failed to generate the qualification clause
//
#define FDORDBMS_428                     0x00000A5CL

//
// MessageId: FDORDBMS_429
//
// MessageText:
//
//  Failed to activate long transaction context query
//
#define FDORDBMS_429                     0x00000A5DL

//
// MessageId: FDORDBMS_430
//
// MessageText:
//
//  Failed to read data from long transaction context query
//
#define FDORDBMS_430                     0x00000A5EL

//
// MessageId: FDORDBMS_431
//
// MessageText:
//
//  Long transaction dependencies contain cycles; Long transaction cache load failed
//
#define FDORDBMS_431                     0x00000A5FL

//
// MessageId: FDORDBMS_432
//
// MessageText:
//
//  Long transaction dependencies update failed
//
#define FDORDBMS_432                     0x00000A60L

//
// MessageId: FDORDBMS_433
//
// MessageText:
//
//  Dependency (%1$ls,%2$ls) causes relationship cycle
//
#define FDORDBMS_433                     0x00000A61L

//
// MessageId: FDORDBMS_434
//
// MessageText:
//
//  Unable to establish dependency (%1$ls,%2$ls)
//
#define FDORDBMS_434                     0x00000A62L

//
// MessageId: FDORDBMS_435
//
// MessageText:
//
//  Unable to update long transaction context cache
//
#define FDORDBMS_435                     0x00000A63L

//
// MessageId: FDORDBMS_436
//
// MessageText:
//
//  The MySQL storage engine '%1$ls' is not supported for creation of new tables
//
#define FDORDBMS_436                     0x00000A64L

//
// MessageId: FDORDBMS_437
//
// MessageText:
//
//  Root long transaction cannot be frozen
//
#define FDORDBMS_437                     0x00000A65L

//
// MessageId: FDORDBMS_438
//
// MessageText:
//
//  Failed to retrieve data for active long transaction %1$ls
//
#define FDORDBMS_438                     0x00000A66L

//
// MessageId: FDORDBMS_439
//
// MessageText:
//
//  Root long transaction cannot be %1$ls
//
#define FDORDBMS_439                     0x00000A67L

//
// MessageId: FDORDBMS_440
//
// MessageText:
//
//  Active transaction - Cannot %1$ls transaction %2$ls
//
#define FDORDBMS_440                     0x00000A68L

//
// MessageId: FDORDBMS_441
//
// MessageText:
//
//  Cannot start transaction; request to %1$ls long transaction '%2$ls' canceled
//
#define FDORDBMS_441                     0x00000A69L

//
// MessageId: FDORDBMS_442
//
// MessageText:
//
//  %1$ls failure due to inaccessible data store(s): %2$ls
//
#define FDORDBMS_442                     0x00000A6AL

//
// MessageId: FDORDBMS_443
//
// MessageText:
//
//  Invalid number of ancestors for long transaction %1$ls
//
#define FDORDBMS_443                     0x00000A6BL

//
// MessageId: FDORDBMS_444
//
// MessageText:
//
//  Failed to retrieve data for active long transaction %1$ls
//
#define FDORDBMS_444                     0x00000A6CL

//
// MessageId: FDORDBMS_445
//
// MessageText:
//
//  Failed to clear digraph for long transaction context
//
#define FDORDBMS_445                     0x00000A6DL

//
// MessageId: FDORDBMS_446
//
// MessageText:
//
//  Cannot commit long transaction '%1$ls' (not a leaf long transaction)
//
#define FDORDBMS_446                     0x00000A6EL

//
// MessageId: FDORDBMS_447
//
// MessageText:
//
//  Failed to retrieve data
//
#define FDORDBMS_447                     0x00000A6FL

//
// MessageId: FDORDBMS_448
//
// MessageText:
//
//  Description
//
#define FDORDBMS_448                     0x00000A70L

//
// MessageId: FDORDBMS_449
//
// MessageText:
//
//  LtMode
//
#define FDORDBMS_449                     0x00000A71L

//
// MessageId: FDORDBMS_450
//
// MessageText:
//
//  LockMode
//
#define FDORDBMS_450                     0x00000A72L

//
// MessageId: FDORDBMS_451
//
// MessageText:
//
//  TableSpace
//
#define FDORDBMS_451                     0x00000A73L

//
// MessageId: FDORDBMS_452
//
// MessageText:
//
//  Locking is not supported for this datastore
//
#define FDORDBMS_452                     0x00000A74L

//
// MessageId: FDORDBMS_453
//
// MessageText:
//
//  Lock Info Reader query is not active
//
#define FDORDBMS_453                     0x00000A75L

//
// MessageId: FDORDBMS_454
//
// MessageText:
//
//  Object properties are not supported.
//
#define FDORDBMS_454                     0x00000A76L

//
// MessageId: FDORDBMS_455
//
// MessageText:
//
//  Association properties are not supported.
//
#define FDORDBMS_455                     0x00000A77L

//
// MessageId: FDORDBMS_456
//
// MessageText:
//
//  Long Transaction error: Cannot update a non recent feature object.
//
#define FDORDBMS_456                     0x00000A78L

//
// MessageId: FDORDBMS_457
//
// MessageText:
//
//  Lock request processing error
//
#define FDORDBMS_457                     0x00000A79L

//
// MessageId: FDORDBMS_458
//
// MessageText:
//
//  Transaction must be started before requesting transaction lock
//
#define FDORDBMS_458                     0x00000A7AL

//
// MessageId: FDORDBMS_459
//
// MessageText:
//
//  No Lock Manager; cannot execute Long Transaction Manager requests
//
#define FDORDBMS_459                     0x00000A7BL

//
// MessageId: FDORDBMS_460
//
// MessageText:
//
//  No long transaction with id '%ls' exists
//
#define FDORDBMS_460                     0x00000A7CL

//
// MessageId: FDORDBMS_461
//
// MessageText:
//
//  Long Transaction error: Cannot update alternatives
//
#define FDORDBMS_461                     0x00000A7DL

//
// MessageId: FDORDBMS_462
//
// MessageText:
//
//  UserId
//
#define FDORDBMS_462                     0x00000A7EL

//
// MessageId: FDORDBMS_463
//
// MessageText:
//
//  DataSourceName
//
#define FDORDBMS_463                     0x00000A7FL

//
// MessageId: FDORDBMS_464
//
// MessageText:
//
//  ConnectionString
//
#define FDORDBMS_464                     0x00000A80L

//
// MessageId: FDORDBMS_465
//
// MessageText:
//
//  GenerateDefaultGeometryProperty
//
#define FDORDBMS_465                     0x00000A81L

//
// MessageId: FDORDBMS_466
//
// MessageText:
//
//  Long Transaction '%ls' is locked by another user
//
#define FDORDBMS_466                     0x00000A82L

//
// MessageId: FDORDBMS_467
//
// MessageText:
//
//  Creating geometric properties from ordinate columns is not supported.
//
#define FDORDBMS_467                     0x00000A83L

//
// MessageId: FDORDBMS_468
//
// MessageText:
//
//  No column for geometric property '%1$ls'.
//
#define FDORDBMS_468                     0x00000A84L

//
// MessageId: FDORDBMS_469
//
// MessageText:
//
//  Cannot find identity property '%1$ls' in property value collection %2$ls.
//
#define FDORDBMS_469                     0x00000A85L

//
// MessageId: FDORDBMS_470
//
// MessageText:
//
//  Autodesk FDO Provider for Oracle
//
#define FDORDBMS_470                     0x00000A86L

//
// MessageId: FDORDBMS_471
//
// MessageText:
//
//  Read/write access to feature data in an Oracle-based data store. Supports spatial indexing, long transactions, and persistent locking.
//
#define FDORDBMS_471                     0x00000A87L

//
// MessageId: FDORDBMS_472
//
// MessageText:
//
//  OSGeo FDO Provider for MySQL
//
#define FDORDBMS_472                     0x00000A88L

//
// MessageId: FDORDBMS_473
//
// MessageText:
//
//  Read/write access to MySQL-based data store. Supports spatial data types and spatial query operations.
//
#define FDORDBMS_473                     0x00000A89L

//
// MessageId: FDORDBMS_474
//
// MessageText:
//
//  Autodesk FDO Provider for SQL Server
//
#define FDORDBMS_474                     0x00000A8AL

//
// MessageId: FDORDBMS_475
//
// MessageText:
//
//  Read/write access to feature data in a MS SQL Server-based data store. Supports geospatial and nongeospatial data and schema R/W access.
//
#define FDORDBMS_475                     0x00000A8BL

//
// MessageId: FDORDBMS_476
//
// MessageText:
//
//  OSGeo FDO Provider for ODBC
//
#define FDORDBMS_476                     0x00000A8CL

//
// MessageId: FDORDBMS_477
//
// MessageText:
//
//  Read/write access to ODBC-based data store. Supports XYZ point features.
//
#define FDORDBMS_477                     0x00000A8DL

//
// MessageId: FDORDBMS_478
//
// MessageText:
//
//  Error creating spatial context %1$ls (SRID=%2$ld), the WKT provided does not match the catalog.
//
#define FDORDBMS_478                     0x00000A8EL

//
// MessageId: FDORDBMS_479
//
// MessageText:
//
//  Geometric value is outside of spatial index extents.
//
#define FDORDBMS_479                     0x00000A8FL

//
// MessageId: FDORDBMS_480
//
// MessageText:
//
//  Incomplete date/time setting.
//
#define FDORDBMS_480                     0x00000A90L

//
// MessageId: FDORDBMS_481
//
// MessageText:
//
//  The runtime was not found (module %1$ls).
//
#define FDORDBMS_481                     0x00000A91L

//
// MessageId: FDORDBMS_482
//
// MessageText:
//
//  The runtime was not found (procedure %1$ls in module %2$ls).
//
#define FDORDBMS_482                     0x00000A92L

//
// MessageId: FDORDBMS_483
//
// MessageText:
//
//  Invalid connection string '%1$ls'
//
#define FDORDBMS_483                     0x00000A93L

//
// MessageId: FDORDBMS_484
//
// MessageText:
//
//  Invalid connection property name '%1$ls'
//
#define FDORDBMS_484                     0x00000A95L

//
// MessageId: FDORDBMS_485
//
// MessageText:
//
//  No column for property '%1$ls'.
//
#define FDORDBMS_485                     0x00000A96L

//
// MessageId: FDORDBMS_486
//
// MessageText:
//
//  Expected property '%1$ls' to be a geometric property.
//
#define FDORDBMS_486                     0x00000A97L

//
// MessageId: FDORDBMS_487
//
// MessageText:
//
//  '%1$ls': Invalid data detected in any of the following properties: '%2$ls'.
//
#define FDORDBMS_487                     0x00000A98L

//
// MessageId: FDORDBMS_488
//
// MessageText:
//
//  '%1$ls': Invalid MBC format: missing root cell.
//
#define FDORDBMS_488                     0x00000A99L

//
// MessageId: FDORDBMS_489
//
// MessageText:
//
//  '%1$ls': Invalid MBC format: illegal character.
//
#define FDORDBMS_489                     0x00000A9AL

//
// MessageId: FDORDBMS_490
//
// MessageText:
//
//  '%1$ls': MBC error: too many skip characters specified.
//
#define FDORDBMS_490                     0x00000A9BL

//
// MessageId: FDORDBMS_491
//
// MessageText:
//
//  '%1$ls': MBC error: depth exceeds maximum depth.
//
#define FDORDBMS_491                     0x00000A9CL

//
// MessageId: FDORDBMS_492
//
// MessageText:
//
//  '%1$ls': MBC error: extents outside spatial index extents.
//  
//  
//  
//  
//  
//  
//  
//
#define FDORDBMS_492                     0x00000A9DL

