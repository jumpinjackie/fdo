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
// MessageId: ADB_1
//
// MessageText:
//
//  ADB_1: Status for layer %1$d not found
//
#define ADB_1                            0x00000001L

//
// MessageId: ADB_2
//
// MessageText:
//
//  ADB_2: Invalid index type %1$d specified, cannot compare index lookup key with found index.
//
#define ADB_2                            0x00000002L

//
// MessageId: ADB_3
//
// MessageText:
//
//  ADB_3: Invalid index lookup key type %1$d specified, cannot delete index from memory resident schema.
//
#define ADB_3                            0x00000003L

//
// MessageId: ADB_4
//
// MessageText:
//
//  ADB_4: Append to index array failed, NULL address returned, %1$s.
//
#define ADB_4                            0x00000004L

//
// MessageId: ADB_5
//
// MessageText:
//
//  ADB_5: Invalid index lookup key %1$d specified, cannot perform index search.
//
#define ADB_5                            0x00000005L

//
// MessageId: ADB_6
//
// MessageText:
//
//  ADB_6: Get from index array position %1$d failed, NULL address returned, %2$s.
//
#define ADB_6                            0x00000006L

//
// MessageId: ADB_7
//
// MessageText:
//
//  ADB_7: Unable to allocate index context array, NULL pointer returned.
//
#define ADB_7                            0x00000007L

//
// MessageId: ADB_8
//
// MessageText:
//
//  ADB_8: Index retrieval not activated, cannot return index.
//
#define ADB_8                            0x00000008L

//
// MessageId: ADB_9
//
// MessageText:
//
//  ADB_9: Unable to allocate index array, NULL pointer returned.
//
#define ADB_9                            0x00000009L

//
// MessageId: ADB_10
//
// MessageText:
//
//  ADB_10: Index name `%1$s' truncated, cannot add index to memory resident schema.
//
#define ADB_10                           0x0000000AL

//
// MessageId: ADB_11
//
// MessageText:
//
//  ADB_11: Table name `%1$s' truncated, cannot add index `%2$s' to memory resident schema.
//
#define ADB_11                           0x0000000BL

//
// MessageId: ADB_12
//
// MessageText:
//
//  ADB_12: Invalid foreign key type %1$d specified, cannot compare foreign key lookup key with found foreign key.
//
#define ADB_12                           0x0000000CL

//
// MessageId: ADB_13
//
// MessageText:
//
//  ADB_13: Invalid key lookup type %1$d specified, cannot delete key from memory resident schema.
//
#define ADB_13                           0x0000000DL

//
// MessageId: ADB_14
//
// MessageText:
//
//  ADB_14: Append to key array failed, NULL address returned, %1$s.
//
#define ADB_14                           0x0000000EL

//
// MessageId: ADB_15
//
// MessageText:
//
//  ADB_15: Invalid lookup key %1$d specified, cannot perform key search.
//
#define ADB_15                           0x0000000FL

//
// MessageId: ADB_16
//
// MessageText:
//
//  ADB_16: Get from key array position %1$d failed, NULL address returned, %2$s.
//
#define ADB_16                           0x00000010L

//
// MessageId: ADB_17
//
// MessageText:
//
//  ADB_17: Cannot activate fetch of keys, maximum %1$d key retrievals currently active.
//
#define ADB_17                           0x00000011L

//
// MessageId: ADB_18
//
// MessageText:
//
//  ADB_18: Unable to allocate key context array, NULL pointer returned.
//
#define ADB_18                           0x00000012L

//
// MessageId: ADB_19
//
// MessageText:
//
//  ADB_19: Key get identifier %1$d must be in the range 0 <= id <= %2$d, cannot activate key fetch.
//
#define ADB_19                           0x00000013L

//
// MessageId: ADB_20
//
// MessageText:
//
//  ADB_20: Key retrieval identifier %1$d not activated, cannot return key(s).
//
#define ADB_20                           0x00000014L

//
// MessageId: ADB_21
//
// MessageText:
//
//  ADB_21: Key get identifier %1$d must be in the range 0 <= id <= %2$d, cannot activate key fetch.
//
#define ADB_21                           0x00000015L

//
// MessageId: ADB_22
//
// MessageText:
//
//  ADB_22: Unable to allocate foreign key array, NULL pointer returned.
//
#define ADB_22                           0x00000016L

//
// MessageId: ADB_23
//
// MessageText:
//
//  ADB_23:  name `%1$s' truncated, cannot add key to memory resident schema.
//
#define ADB_23                           0x00000017L

//
// MessageId: ADB_24
//
// MessageText:
//
//  ADB_24: Table name `%1$s' truncated, cannot add key `%2$s' to memory resident schema.
//
#define ADB_24                           0x00000018L

//
// MessageId: ADB_25
//
// MessageText:
//
//  ADB_25: Primary key name `%1$s' truncated, cannot add key `%2$s' to memory resident schema.
//
#define ADB_25                           0x00000019L

//
// MessageId: ADB_26
//
// MessageText:
//
//  ADB_26: Key relation `%1$s' truncated, cannot add key `%2$s' to memory resident schema.
//
#define ADB_26                           0x0000001AL

//
// MessageId: ADB_27
//
// MessageText:
//
//  ADB_27: Cannot retrieve feature codes for non-existant table: %1$s
//
#define ADB_27                           0x0000001BL

//
// MessageId: ADB_28
//
// MessageText:
//
//  ADB_28: Error finding networks in memory resident schema
//
#define ADB_28                           0x0000001CL

//
// MessageId: ADB_29
//
// MessageText:
//
//  ADB_29: No more memory residents schemas available, %1$d in use.
//
#define ADB_29                           0x0000001DL

//
// MessageId: ADB_30
//
// MessageText:
//
//  ADB_30: Not enough column locations supplied. Only %1$d returned.
//
#define ADB_30                           0x0000001EL

//
// MessageId: ADB_31
//
// MessageText:
//
//  ADB_31: database: 
//
#define ADB_31                           0x0000001FL

//
// MessageId: ADB_32
//
// MessageText:
//
//  ADB_32: table: 
//
#define ADB_32                           0x00000020L

//
// MessageId: ADB_33
//
// MessageText:
//
//  ADB_33: column: 
//
#define ADB_33                           0x00000021L

//
// MessageId: ADB_34
//
// MessageText:
//
//  ADB_34: Error finding specified layer in memory resident schema
//
#define ADB_34                           0x00000022L

//
// MessageId: ADB_35
//
// MessageText:
//
//  ADB_35: Layer %1$d not found - could not modify it.
//
#define ADB_35                           0x00000023L

//
// MessageId: ADB_36
//
// MessageText:
//
//  ADB_36: Error finding network in memory resident schema
//
#define ADB_36                           0x00000024L

//
// MessageId: ADB_37
//
// MessageText:
//
//  ADB_37: Could not find network %1$d to modify it
//
#define ADB_37                           0x00000025L

//
// MessageId: ADB_38
//
// MessageText:
//
//  ADB_38: Invalid connection id
//
#define ADB_38                           0x00000026L

//
// MessageId: ADB_39
//
// MessageText:
//
//  ADB_39: Stub for adb_get_row called
//
#define ADB_39                           0x00000027L

//
// MessageId: ADB_40
//
// MessageText:
//
//  ADB_40: Stub for adb_deac_quer called
//
#define ADB_40                           0x00000028L

//
// MessageId: ADB_41
//
// MessageText:
//
//  ADB_41: Stub for adb_act_quer called
//
#define ADB_41                           0x00000029L

//
// MessageId: ADB_42
//
// MessageText:
//
//  ADB_42: Stub for adb_del_feat called
//
#define ADB_42                           0x0000002AL

//
// MessageId: ADB_43
//
// MessageText:
//
//  ADB_43: Stub for adb_add_lktb called
//
#define ADB_43                           0x0000002BL

//
// MessageId: ADB_44
//
// MessageText:
//
//  ADB_44: Stub for adb_get_lktb called
//
#define ADB_44                           0x0000002CL

//
// MessageId: ADB_45
//
// MessageText:
//
//  ADB_45: Stub for adb_add_feat called
//
#define ADB_45                           0x0000002DL

//
// MessageId: ADB_46
//
// MessageText:
//
//  ADB_46: Invalid version number %1$d, cannot activate version
//
#define ADB_46                           0x0000002EL

//
// MessageId: ADB_47
//
// MessageText:
//
//  ADB_47: Version %1$d does not exist, cannot activate version
//
#define ADB_47                           0x0000002FL

//
// MessageId: ADB_48
//
// MessageText:
//
//  ADB_48: Version %1$d is locked by lock name '%2$s', cannot activate version
//
#define ADB_48                           0x00000030L

//
// MessageId: ADB_49
//
// MessageText:
//
//  ADB_49: Another session has version %1$d active, cannot activate version
//
#define ADB_49                           0x00000031L

//
// MessageId: ADB_50
//
// MessageText:
//
//  ADB_50: Error activating version %1$d, cannot activate version
//
#define ADB_50                           0x00000032L

//
// MessageId: ADB_51
//
// MessageText:
//
//  ADB_51: Invalid version number %1$d, version not added
//
#define ADB_51                           0x00000033L

//
// MessageId: ADB_52
//
// MessageText:
//
//  ADB_52: Invalid version number %1$d, version not added
//
#define ADB_52                           0x00000034L

//
// MessageId: ADB_53
//
// MessageText:
//
//  ADB_53: Version number %1$d or name '%2$s' already exists, version not added
//
#define ADB_53                           0x00000035L

//
// MessageId: ADB_54
//
// MessageText:
//
//  ADB_54: Version name '%1$s' already exists, version not added
//
#define ADB_54                           0x00000036L

//
// MessageId: ADB_55
//
// MessageText:
//
//  ADB_55: Failed to initialize version context for database '%1$s'.
//
#define ADB_55                           0x00000037L

//
// MessageId: ADB_56
//
// MessageText:
//
//  ADB_56: Can't reinitialize version dependency cache
//
#define ADB_56                           0x00000038L

//
// MessageId: ADB_57
//
// MessageText:
//
//  ADB_57: Invalid generation %1$ld for version dependency (%2$d,%3$d), cache load failed
//
#define ADB_57                           0x00000039L

//
// MessageId: ADB_58
//
// MessageText:
//
//  ADB_58: Invalid version dependency (%1$d,%2$d), cache load failed
//
#define ADB_58                           0x0000003AL

//
// MessageId: ADB_59
//
// MessageText:
//
//  ADB_59: Can't add version dependency (%1$d,%2$d), cache load failed
//
#define ADB_59                           0x0000003BL

//
// MessageId: ADB_60
//
// MessageText:
//
//  ADB_60: Version dependencies contain cycles, cache load failed
//
#define ADB_60                           0x0000003CL

//
// MessageId: ADB_61
//
// MessageText:
//
//  ADB_61: Version dependencies have an invalid next generation number, cache load failed
//
#define ADB_61                           0x0000003DL

//
// MessageId: ADB_62
//
// MessageText:
//
//  ADB_62: No active version, cannot retrieve active version information
//
#define ADB_62                           0x0000003EL

//
// MessageId: ADB_63
//
// MessageText:
//
//  ADB_63: Invalid version number %1$d, cannot retrieve version information
//
#define ADB_63                           0x0000003FL

//
// MessageId: ADB_64
//
// MessageText:
//
//  ADB_64: Invalid version number %1$d.
//
#define ADB_64                           0x00000040L

//
// MessageId: ADB_65
//
// MessageText:
//
//  ADB_65: Invalid request locking request %1$d for table '%2$s'.
//
#define ADB_65                           0x00000041L

//
// MessageId: ADB_66
//
// MessageText:
//
//  ADB_66: Invalid versioning query mode
//
#define ADB_66                           0x00000042L

//
// MessageId: ADB_67
//
// MessageText:
//
//  ADB_67: Cannot add invalid version number %1$d to version selection set
//
#define ADB_67                           0x00000043L

//
// MessageId: ADB_68
//
// MessageText:
//
//  ADB_68: Lock name `%1$s' is already open for this session.
//
#define ADB_68                           0x00000044L

//
// MessageId: ADB_69
//
// MessageText:
//
//  ADB_69: Lock name `%1$s' is already open for this session, cannot open lock name `%2$s'.
//
#define ADB_69                           0x00000045L

//
// MessageId: ADB_70
//
// MessageText:
//
//  ADB_70: Parameter opened_by_conflict NULL pointer, unable to open lock name `%1$s'.
//
#define ADB_70                           0x00000046L

//
// MessageId: ADB_71
//
// MessageText:
//
//  ADB_71: Lock name `%1$s' does not exist, lock not opened.
//
#define ADB_71                           0x00000047L

//
// MessageId: ADB_72
//
// MessageText:
//
//  ADB_72: Failed to connect to %1$s. Invalid Password
//
#define ADB_72                           0x00000048L

//
// MessageId: ADB_73
//
// MessageText:
//
//  ADB_73: Failed to connect to %1$s. RDBMS error %2$d
//
#define ADB_73                           0x00000049L

//
// MessageId: ADB_74
//
// MessageText:
//
//  ADB_74: Warning:  unable to remove constraint on table '%1$s'.
//
#define ADB_74                           0x0000004AL

//
// MessageId: ADB_75
//
// MessageText:
//
//  ADB_75: Warning:  unable to remove constraint on table '%1$s'.
//
#define ADB_75                           0x0000004BL

//
// MessageId: ADB_76
//
// MessageText:
//
//  ADB_76: Unknown db_or_cache value %1$d, cannot delete keys.
//
#define ADB_76                           0x0000004CL

//
// MessageId: ADB_77
//
// MessageText:
//
//  ADB_77: Database '%1$s' already exists.
//
#define ADB_77                           0x0000004DL

//
// MessageId: ADB_78
//
// MessageText:
//
//  ADB_78: The RDBMS user '%1$s' already exists.
//
#define ADB_78                           0x0000004EL

//
// MessageId: ADB_79
//
// MessageText:
//
//  ADB_79: Error creating system tables for '%1$s'.
//
#define ADB_79                           0x0000004FL

//
// MessageId: ADB_80
//
// MessageText:
//
//  ADB_80: %1$s: warning, no lock name to close ... continuing
//
#define ADB_80                           0x00000050L

//
// MessageId: ADB_81
//
// MessageText:
//
//  ADB_81: Delete of '%1$s' %2$ld in table '%3$s' not done.\nRow with that value was not found.
//
#define ADB_81                           0x00000051L

//
// MessageId: ADB_82
//
// MessageText:
//
//  ADB_82: Failed to delete primary/foreign key: %1$s for table: %2$s
//
#define ADB_82                           0x00000052L

//
// MessageId: ADB_83
//
// MessageText:
//
//  ADB_83: Network %1$d not found
//
#define ADB_83                           0x00000053L

//
// MessageId: ADB_84
//
// MessageText:
//
//  ADB_84: The following tables could not be dropped:
//
#define ADB_84                           0x00000054L

//
// MessageId: ADB_85
//
// MessageText:
//
//  ADB_85:     %1$s
//
#define ADB_85                           0x00000055L

//
// MessageId: ADB_86
//
// MessageText:
//
//  ADB_86: Not all tables deleted for '%1$s'; database is partially deleted.
//
#define ADB_86                           0x00000056L

//
// MessageId: ADB_87
//
// MessageText:
//
//  ADB_87: Repeat database delete operation after remaining tables may be dropped.
//
#define ADB_87                           0x00000057L

//
// MessageId: ADB_88
//
// MessageText:
//
//  ADB_88: Error removing ingres user '%1$s'.
//
#define ADB_88                           0x00000058L

//
// MessageId: ADB_89
//
// MessageText:
//
//  ADB_89: Database deletion failed for database '%1$s'.
//
#define ADB_89                           0x00000059L

//
// MessageId: ADB_90
//
// MessageText:
//
//  ADB_90: Error - Database name '%1$s' not found in Content Directory
//
#define ADB_90                           0x0000005AL

//
// MessageId: ADB_91
//
// MessageText:
//
//  ADB_91: Failed to connect to %1$s. Invalid Password
//
#define ADB_91                           0x0000005BL

//
// MessageId: ADB_92
//
// MessageText:
//
//  ADB_92: Failed to connect to %1$s. RDBMS error %2$d
//
#define ADB_92                           0x0000005CL

//
// MessageId: ADB_93
//
// MessageText:
//
//  ADB_93: Failed to connect to %1$s. Invalid Password
//
#define ADB_93                           0x0000005DL

//
// MessageId: ADB_94
//
// MessageText:
//
//  ADB_94: Could not open file: '%1$s'
//
#define ADB_94                           0x0000005EL

//
// MessageId: ADB_95
//
// MessageText:
//
//  ADB_95: #else without #if[n]def in file: '%1$s'
//
#define ADB_95                           0x0000005FL

//
// MessageId: ADB_96
//
// MessageText:
//
//  ADB_96: #endif without #if[n]def in file: '%1$s'
//
#define ADB_96                           0x00000060L

//
// MessageId: ADB_97
//
// MessageText:
//
//  ADB_97: Unknown directive '%1$s' in file: '%2$s'
//
#define ADB_97                           0x00000061L

//
// MessageId: ADB_98
//
// MessageText:
//
//  ADB_98: One or more missing #endifs in file: '%1$s'
//
#define ADB_98                           0x00000062L

//
// MessageId: ADB_99
//
// MessageText:
//
//  ADB_99: Maximum feature number of %1$ld has been reached.\nNo more feature numbers available for this database.
//
#define ADB_99                           0x00000063L

//
// MessageId: ADB_100
//
// MessageText:
//
//  ADB_100: Invalid 'distinct' parameter ignored
//
#define ADB_100                          0x00000064L

//
// MessageId: ADB_101
//
// MessageText:
//
//  ADB_101: Database not open.
//
#define ADB_101                          0x00000065L

//
// MessageId: ADB_102
//
// MessageText:
//
//  ADB_102: More than one %1$s header record found, cannot continue.
//
#define ADB_102                          0x00000066L

//
// MessageId: ADB_103
//
// MessageText:
//
//  ADB_103: Couldn't find space '%1$s' definition
//
#define ADB_103                          0x00000067L

//
// MessageId: ADB_104
//
// MessageText:
//
//  ADB_104: Incomplete space definition for '%1$s'
//
#define ADB_104                          0x00000068L

//
// MessageId: ADB_105
//
// MessageText:
//
//  ADB_105: Using default space parameters
//
#define ADB_105                          0x00000069L

//
// MessageId: ADB_106
//
// MessageText:
//
//  ADB_106: Input space name is too long
//
#define ADB_106                          0x0000006AL

//
// MessageId: ADB_107
//
// MessageText:
//
//  ADB_107: Couldn't open file: %1$s
//
#define ADB_107                          0x0000006BL

//
// MessageId: ADB_108
//
// MessageText:
//
//  ADB_108: Host database `%1$s' does not exist
//
#define ADB_108                          0x0000006CL

//
// MessageId: ADB_109
//
// MessageText:
//
//  ADB_109: Host database `%1$s' is open in exclusive mode
//
#define ADB_109                          0x0000006DL

//
// MessageId: ADB_110
//
// MessageText:
//
//  ADB_110: Unable to disconnect static number connection
//
#define ADB_110                          0x0000006EL

//
// MessageId: ADB_111
//
// MessageText:
//
//  ADB_111: Parameter lock_spec NULL pointer, unable to return active lock name specification.
//
#define ADB_111                          0x0000006FL

//
// MessageId: ADB_112
//
// MessageText:
//
//  ADB_112: Cannot drop primary key: %1$s
//
#define ADB_112                          0x00000070L

//
// MessageId: ADB_113
//
// MessageText:
//
//  ADB_113: There are existing foreign key relationship(s)
//
#define ADB_113                          0x00000071L

//
// MessageId: ADB_114
//
// MessageText:
//
//  ADB_114: NULL or empty table_name specified, cannot add foreign key.
//
#define ADB_114                          0x00000072L

//
// MessageId: ADB_115
//
// MessageText:
//
//  ADB_115: Ncol <= 0 specified, cannot add foreign key for table %1$s.
//
#define ADB_115                          0x00000073L

//
// MessageId: ADB_116
//
// MessageText:
//
//  ADB_116: NULL or empty p_key_name specified, cannot add foreign key for table %1$s.
//
#define ADB_116                          0x00000074L

//
// MessageId: ADB_117
//
// MessageText:
//
//  ADB_117: NULL or empty p_table_name specified, cannot add foreign key for table %1$s.
//
#define ADB_117                          0x00000075L

//
// MessageId: ADB_118
//
// MessageText:
//
//  ADB_118: P_ncol <= 0 specified, cannot add foreign key for table %1$s.
//
#define ADB_118                          0x00000076L

//
// MessageId: ADB_119
//
// MessageText:
//
//  ADB_119: Foreign key %1$s already exists, cannot create foreign key.
//
#define ADB_119                          0x00000077L

//
// MessageId: ADB_120
//
// MessageText:
//
//  ADB_120: Empty column name string specified, cannot add foreign key for table %1$s.
//
#define ADB_120                          0x00000078L

//
// MessageId: ADB_121
//
// MessageText:
//
//  ADB_121: Empty primary key column name string specified, cannot add foreign key for table %1$s.
//
#define ADB_121                          0x00000079L

//
// MessageId: ADB_122
//
// MessageText:
//
//  ADB_122: Foreign key name %1$s already exists.
//
#define ADB_122                          0x0000007AL

//
// MessageId: ADB_123
//
// MessageText:
//
//  ADB_123: Empty column name string specified, cannot add foreign key for table %1$s.
//
#define ADB_123                          0x0000007BL

//
// MessageId: ADB_124
//
// MessageText:
//
//  ADB_124: Foreign key name %1$s already exists.
//
#define ADB_124                          0x0000007CL

//
// MessageId: ADB_125
//
// MessageText:
//
//  ADB_125: Unknown lookup key type %1$d specified, cannot get primary keys.
//
#define ADB_125                          0x0000007DL

//
// MessageId: ADB_126
//
// MessageText:
//
//  ADB_126: Unknown lookup key type %1$d specified, cannot get foreign keys.
//
#define ADB_126                          0x0000007EL

//
// MessageId: ADB_127
//
// MessageText:
//
//  ADB_127: NULL or empty table_name specified, cannot add primary key.
//
#define ADB_127                          0x0000007FL

//
// MessageId: ADB_128
//
// MessageText:
//
//  ADB_128: Ncol <= 0 specified, cannot add primary key for table %1$s.
//
#define ADB_128                          0x00000080L

//
// MessageId: ADB_129
//
// MessageText:
//
//  ADB_129: Primary key %1$s already exists, cannot create primary key.
//
#define ADB_129                          0x00000081L

//
// MessageId: ADB_130
//
// MessageText:
//
//  ADB_130: Empty column name string specified, cannot add primary key for table %1$s.
//
#define ADB_130                          0x00000082L

//
// MessageId: ADB_131
//
// MessageText:
//
//  ADB_131: Primary key name %1$s already exists.
//
#define ADB_131                          0x00000083L

//
// MessageId: ADB_132
//
// MessageText:
//
//  ADB_132: Empty column name string specified, cannot add primary key for table %1$s.
//
#define ADB_132                          0x00000084L

//
// MessageId: ADB_133
//
// MessageText:
//
//  ADB_133: Primary key name %1$s already exists, cannot add column %2$s.
//
#define ADB_133                          0x00000085L

//
// MessageId: ADB_134
//
// MessageText:
//
//  ADB_134: Waiting to lock feature %1$d
//
#define ADB_134                          0x00000086L

//
// MessageId: ADB_135
//
// MessageText:
//
//  ADB_135: Unable to get number of tables.
//
#define ADB_135                          0x00000087L

//
// MessageId: ADB_136
//
// MessageText:
//
//  ADB_136: Unable to get known row from adb_lktb_cache.
//
#define ADB_136                          0x00000088L

//
// MessageId: ADB_137
//
// MessageText:
//
//  ADB_137: Cannot set row in table `%1$s' to table name: `%2$s' .
//
#define ADB_137                          0x00000089L

//
// MessageId: ADB_138
//
// MessageText:
//
//  ADB_138: Unable to get number of tables.
//
#define ADB_138                          0x0000008AL

//
// MessageId: ADB_139
//
// MessageText:
//
//  ADB_139: Unable to get known row from adb_lktb_cache.
//
#define ADB_139                          0x0000008BL

//
// MessageId: ADB_140
//
// MessageText:
//
//  ADB_140: Unable to locate known adb_lktb_cache entry.
//
#define ADB_140                          0x0000008CL

//
// MessageId: ADB_141
//
// MessageText:
//
//  ADB_141: Append to adb_lktb_cache failed, NULL element address returned.
//
#define ADB_141                          0x0000008DL

//
// MessageId: ADB_142
//
// MessageText:
//
//  ADB_142: Append to adb_lktb_cache failed, NULL element address returned.
//
#define ADB_142                          0x0000008EL

//
// MessageId: ADB_143
//
// MessageText:
//
//  ADB_143: Unable to allocate adb_lktb_cache array, NULL pointer returned.
//
#define ADB_143                          0x0000008FL

//
// MessageId: ADB_144
//
// MessageText:
//
//  ADB_144: Error loading default statistics for '%1$s'.
//
#define ADB_144                          0x00000090L

//
// MessageId: ADB_145
//
// MessageText:
//
//  ADB_145: Layer %1$d not found
//
#define ADB_145                          0x00000091L

//
// MessageId: ADB_146
//
// MessageText:
//
//  ADB_146: Network %1$d not found
//
#define ADB_146                          0x00000092L

//
// MessageId: ADB_147
//
// MessageText:
//
//  ADB_147: Unable to get information about `%1$s', no datbase active
//
#define ADB_147                          0x00000093L

//
// MessageId: ADB_148
//
// MessageText:
//
//  ADB_148: Can not drop `%1$s', no datbase active
//
#define ADB_148                          0x00000094L

//
// MessageId: ADB_149
//
// MessageText:
//
//  ADB_149: object `%1$s.%2$s' not found, cannot drop object.
//
#define ADB_149                          0x00000095L

//
// MessageId: ADB_150
//
// MessageText:
//
//  ADB_150: Invalid type `%1$c' specified, cannot drop object `%2$s.%3$s'.
//
#define ADB_150                          0x00000096L

//
// MessageId: ADB_151
//
// MessageText:
//
//  ADB_151: Cannot open more than %1$d databases. Open Failed on '%2$s'
//
#define ADB_151                          0x00000097L

//
// MessageId: ADB_152
//
// MessageText:
//
//  ADB_152: Database/password combination is not valid
//
#define ADB_152                          0x00000098L

//
// MessageId: ADB_153
//
// MessageText:
//
//  ADB_153: Database `%1$s' does not exist in the RDBMS
//
#define ADB_153                          0x00000099L

//
// MessageId: ADB_154
//
// MessageText:
//
//  ADB_154: Open failed. Database '%1$s' is open exclusive to %2$s
//
#define ADB_154                          0x0000009AL

//
// MessageId: ADB_155
//
// MessageText:
//
//  ADB_155: Cannot get exclusive access to database '%1$s'
//
#define ADB_155                          0x0000009BL

//
// MessageId: ADB_156
//
// MessageText:
//
//  ADB_156: Database '%1$s' is already open.
//
#define ADB_156                          0x0000009CL

//
// MessageId: ADB_157
//
// MessageText:
//
//  ADB_157: Plan fetch has not been activated
//
#define ADB_157                          0x0000009DL

//
// MessageId: ADB_158
//
// MessageText:
//
//  ADB_158: Invalid plan number %1$d, plan not added
//
#define ADB_158                          0x0000009EL

//
// MessageId: ADB_159
//
// MessageText:
//
//  ADB_159: Plan 0 is permanent and cannot be deleted.
//
#define ADB_159                          0x0000009FL

//
// MessageId: ADB_160
//
// MessageText:
//
//  ADB_160: Database/password combination is not valid
//
#define ADB_160                          0x000000A0L

//
// MessageId: ADB_161
//
// MessageText:
//
//  ADB_161: Range table for user number %1$d not initialized; cannot empty table.
//
#define ADB_161                          0x000000A1L

//
// MessageId: ADB_162
//
// MessageText:
//
//  ADB_162: Range table for user number %1$d not initialized; cannot insert row.
//
#define ADB_162                          0x000000A2L

//
// MessageId: ADB_163
//
// MessageText:
//
//  ADB_163: Range table for user number %1$d not initialized; cannot flush rows.
//
#define ADB_163                          0x000000A3L

//
// MessageId: ADB_164
//
// MessageText:
//
//  ADB_164: Range table for user number %1$d not initialized; cannot drop table.
//
#define ADB_164                          0x000000A4L

//
// MessageId: ADB_165
//
// MessageText:
//
//  ADB_165: Maximum number of %1$ld has been reached for sequence '%2$s' .\nNo more numbers available for this database.
//
#define ADB_165                          0x000000A5L

//
// MessageId: ADB_166
//
// MessageText:
//
//  ADB_166: Surface '%1$s' already exists
//
#define ADB_166                          0x000000A6L

//
// MessageId: ADB_167
//
// MessageText:
//
//  ADB_167: Surface '%1$s' does not exist
//
#define ADB_167                          0x000000A7L

//
// MessageId: ADB_168
//
// MessageText:
//
//  ADB_168: Surface '%1$s' was not found
//
#define ADB_168                          0x000000A8L

//
// MessageId: ADB_169
//
// MessageText:
//
//  ADB_169: Invalid database identifier
//
#define ADB_169                          0x000000A9L

//
// MessageId: ADB_170
//
// MessageText:
//
//  ADB_170: Failed to find synonym: '%1$s'
//
#define ADB_170                          0x000000AAL

//
// MessageId: ADB_171
//
// MessageText:
//
//  ADB_171: Invalid object_type %1$d specified, cannot retrieve indexes.
//
#define ADB_171                          0x000000ABL

//
// MessageId: ADB_172
//
// MessageText:
//
//  ADB_172: Can't generate temporary table for %1$s;
//
#define ADB_172                          0x000000ACL

//
// MessageId: ADB_173
//
// MessageText:
//
//  ADB_173:    More than %1$d users have this database open
//
#define ADB_173                          0x000000ADL

//
// MessageId: ADB_174
//
// MessageText:
//
//  ADB_174: Cannot set row in table \"%1$s\" to table name: \"%2$s\" .
//
#define ADB_174                          0x000000AEL

//
// MessageId: ADB_175
//
// MessageText:
//
//  ADB_175: Cannot open more than %1$d databases.
//
#define ADB_175                          0x000000AFL

//
// MessageId: ADB_176
//
// MessageText:
//
//  ADB_176: NULL or empty table_name specified, cannot add index.
//
#define ADB_176                          0x000000B0L

//
// MessageId: ADB_177
//
// MessageText:
//
//  ADB_177: Ncol <= 0 specified, cannot add index for table %1$s.
//
#define ADB_177                          0x000000B1L

//
// MessageId: ADB_178
//
// MessageText:
//
//  ADB_178: Index %1$s already exists, cannot create index.
//
#define ADB_178                          0x000000B2L

//
// MessageId: ADB_179
//
// MessageText:
//
//  ADB_179: Unknown index type specified, cannot add index for table %1$s.
//
#define ADB_179                          0x000000B3L

//
// MessageId: ADB_180
//
// MessageText:
//
//  ADB_180: Empty column name string specified, cannot add index for table %1$s.
//
#define ADB_180                          0x000000B4L

//
// MessageId: ADB_181
//
// MessageText:
//
//  ADB_181: Primary key %1$s not found; skipping foreign key %2$s.
//
#define ADB_181                          0x000000B5L

//
// MessageId: ADB_182
//
// MessageText:
//
//  ADB_182: Sequence '%1$s' is not defined, cannot retrieve max. generation number.
//
#define ADB_182                          0x000000B6L

//
// MessageId: ADB_183
//
// MessageText:
//
//  ADB_183: Unable to allocate shared lock tables cache, NULL pointer returned.
//
#define ADB_183                          0x000000B7L

//
// MessageId: ADB_184
//
// MessageText:
//
//  ADB_184: Unable to get value from the shared lock tables cache.
//
#define ADB_184                          0x000000B8L

//
// MessageId: ADB_185
//
// MessageText:
//
//  ADB_185: Append to the shared lock tables cache failed.
//
#define ADB_185                          0x000000B9L

//
// MessageId: ADB_186
//
// MessageText:
//
//  ADB_186: Unsupported date mask in nls_date_format session parameter.
//
#define ADB_186                          0x000000BAL

//
// MessageId: ADB_187
//
// MessageText:
//
//  ADB_187: Unable to locate %1$s%2$s %3$s%4$s %5$s%6$s
//
#define ADB_187                          0x000000BBL

//
// MessageId: ADB_188
//
// MessageText:
//
//  ADB_188: Restoring version %1$s data into version %2$s content directory
//
#define ADB_188                          0x000000BCL

//
// MessageId: ADB_189
//
// MessageText:
//
//  ADB_189: Content Directory backup version (%1$s) is incompatible with current version (%2$s).\nContent Directory data will not be restored
//
#define ADB_189                          0x000000BDL

//
// MessageId: ADB_190
//
// MessageText:
//
//  ADB_190: This backup is old; content directory info will not be restored.
//
#define ADB_190                          0x000000BEL

//
// MessageId: ADB_191
//
// MessageText:
//
//  ADB_191: Invalid default password mode %1$d.
//
#define ADB_191                          0x000000BFL

//
// MessageId: ADB_192
//
// MessageText:
//
//  ADB_192: Database '%1$s' at '%2$s' does not exist.
//
#define ADB_192                          0x000000C0L

//
// MessageId: ADB_193
//
// MessageText:
//
//  ADB_193: Class %1$ld not found
//
#define ADB_193                          0x000000C1L

//
// MessageId: ADB_194
//
// MessageText:
//
//  ADB_194: Invalid class number %1$ld, class not added
//
#define ADB_194                          0x000000C2L

//
// MessageId: ADB_195
//
// MessageText:
//
//  ADB_195: Cannot delete class %1$ld while it is the default class for table %2$s
//
#define ADB_195                          0x000000C3L

//
// MessageId: ADB_196
//
// MessageText:
//
//  ADB_196: Cannot delete class %1$ld while it is being referenced in the master object table
//
#define ADB_196                          0x000000C4L

//
// MessageId: ADB_197
//
// MessageText:
//
//  ADB_197: Error finding class in memory resident schema.
//
#define ADB_197                          0x000000C5L

//
// MessageId: ADB_198
//
// MessageText:
//
//  ADB_198: Specified class name is already in use.
//
#define ADB_198                          0x000000C6L

//
// MessageId: ADB_199
//
// MessageText:
//
//  ADB_199: Class mechanism has not been enabled for this database.  Class %1$ld not found.
//
#define ADB_199                          0x000000C7L

//
// MessageId: ADB_200
//
// MessageText:
//
//  ADB_200: The specified primary column\'s name is too long: %1$s
//
#define ADB_200                          0x000000C8L

//
// MessageId: ADB_201
//
// MessageText:
//
//  ADB_201: The table name %1$s is too long.
//
#define ADB_201                          0x000000C9L

//
// MessageId: ADB_202
//
// MessageText:
//
//  ADB_202: The class name '%1$s' already exists.
//
#define ADB_202                          0x000000CAL

//
// MessageId: ADB_203
//
// MessageText:
//
//  ADB_203: Parent version %1$ld of appropriate type does not exist for version %2$ld, version not added.
//
#define ADB_203                          0x000000CBL

//
// MessageId: ADB_204
//
// MessageText:
//
//  ADB_204: Warning: Using method DROP_RECREATE, triggers will be dropped, LTC tables must be re-enabled.
//
#define ADB_204                          0x000000CCL

//
// MessageId: ADB_205
//
// MessageText:
//
//  ADB_205: Schema out of date - upgrade %1$s table schema for %2$s column.
//
#define ADB_205                          0x000000CDL

//
// MessageId: ADB_206
//
// MessageText:
//
//  ADB_206: Plan group fetch has not been activated.
//
#define ADB_206                          0x000000CEL

//
// MessageId: ADB_207
//
// MessageText:
//
//  ADB_207: Could not add geometry table %1$s to RDBMS.
//
#define ADB_207                          0x000000CFL

//
// MessageId: ADB_208
//
// MessageText:
//
//  ADB_208: Could not create spatial index for table %1$s.
//
#define ADB_208                          0x000000D0L

//
// MessageId: AH_1
//
// MessageText:
//
//  AH_1: System error: Memory allocation for %1$d bytes failed
//
#define AH_1                             0x000000D1L

//
// MessageId: AH_2
//
// MessageText:
//
//  AH_2: Requested area polygon does not exist
//
#define AH_2                             0x000000D2L

//
// MessageId: AH_3
//
// MessageText:
//
//  AH_3: Requested area polygon is not complete
//
#define AH_3                             0x000000D3L

//
// MessageId: AH_4
//
// MessageText:
//
//  AH_4: System error: Memory allocation failed for %1$d bytes
//
#define AH_4                             0x000000D4L

//
// MessageId: AH_5
//
// MessageText:
//
//  AH_5: Clipping to boundary of a multi-polygon area definition is not supported.
//
#define AH_5                             0x000000D5L

//
// MessageId: AH_6
//
// MessageText:
//
//  AH_6: Features will be clipped to boundary of first polygon in list only.
//
#define AH_6                             0x000000D6L

//
// MessageId: AH_7
//
// MessageText:
//
//  AH_7: System error (ah_ldaredges): too many edges found
//
#define AH_7                             0x000000D7L

//
// MessageId: AH_8
//
// MessageText:
//
//  AH_8: System error (ah_ldaredges): non-existent edge in poly
//
#define AH_8                             0x000000D8L

//
// MessageId: AH_9
//
// MessageText:
//
//  AH_9: System error (ah_ldaredges): non-existent edge in poly
//
#define AH_9                             0x000000D9L

//
// MessageId: AH_10
//
// MessageText:
//
//  AH_10: Asked to complete feature %1$ld but it is not a polygon
//
#define AH_10                            0x000000DAL

//
// MessageId: AH_11
//
// MessageText:
//
//  AH_11: No connection from %1$s of edge %2$ld
//
#define AH_11                            0x000000DBL

//
// MessageId: AH_12
//
// MessageText:
//
//  AH_12: Edge %1$d runs into a different loop's edge %2$d
//
#define AH_12                            0x000000DCL

//
// MessageId: AH_13
//
// MessageText:
//
//  AH_13: Edge %1$d returns to loop but not to start of loop
//
#define AH_13                            0x000000DDL

//
// MessageId: AH_14
//
// MessageText:
//
//  AH_14: No exterior loop was found for polygon %1$ld
//
#define AH_14                            0x000000DEL

//
// MessageId: AH_15
//
// MessageText:
//
//  AH_15: More than one exterior loop was found for polygon %1$ld
//
#define AH_15                            0x000000DFL

//
// MessageId: AH_16
//
// MessageText:
//
//  AH_16: Broken or incomplete loop detected, starting with edge number %1$d
//
#define AH_16                            0x000000E0L

//
// MessageId: AH_17
//
// MessageText:
//
//  AH_17: Unable to allocate memory for polygon %1$ld's topology table
//
#define AH_17                            0x000000E1L

//
// MessageId: AH_18
//
// MessageText:
//
//  AH_18: Error retrieving topology for polygon %1$ld
//
#define AH_18                            0x000000E2L

//
// MessageId: AH_19
//
// MessageText:
//
//  AH_19: Unable to allocate more memory for polygon %1$ld's topology table
//
#define AH_19                            0x000000E3L

//
// MessageId: AH_20
//
// MessageText:
//
//  AH_20: Unable to allocate memory for polygon %1$ld's topology table
//
#define AH_20                            0x000000E4L

//
// MessageId: AH_21
//
// MessageText:
//
//  AH_21: Edge %1$d, though in edge table for polygon %2$d, was not found
//
#define AH_21                            0x000000E5L

//
// MessageId: AH_22
//
// MessageText:
//
//  AH_22: Centroid for polygon %1$ld is outside the loop
//
#define AH_22                            0x000000E6L

//
// MessageId: AH_23
//
// MessageText:
//
//  AH_23: Interior loops for polygon %1$ld are not entirely withing exterior loop
//
#define AH_23                            0x000000E7L

//
// MessageId: CM_1
//
// MessageText:
//
//  CM_1: %1$s: cache not initialized
//
#define CM_1                             0x000000E8L

//
// MessageId: CM_2
//
// MessageText:
//
//  CM_2: %1$s: There is no separate cache for table '%2$s'
//
#define CM_2                             0x000000E9L

//
// MessageId: CM_3
//
// MessageText:
//
//  CM_3: %1$s: cache not initialized
//
#define CM_3                             0x000000EAL

//
// MessageId: CM_4
//
// MessageText:
//
//  CM_4: %1$s: There is no separate cache for table '%2$s'
//
#define CM_4                             0x000000EBL

//
// MessageId: CM_5
//
// MessageText:
//
//  CM_5: %1$s: failed. Cache is not initialized
//
#define CM_5                             0x000000ECL

//
// MessageId: CM_6
//
// MessageText:
//
//  CM_6: %1$s: valid direction is (1) or (-1)
//
#define CM_6                             0x000000EDL

//
// MessageId: CM_7
//
// MessageText:
//
//  CM_7: %1$s: coordinate retrieval not initilized
//
#define CM_7                             0x000000EEL

//
// MessageId: CM_8
//
// MessageText:
//
//  CM_8: %1$s: there is no active cache, cannot update
//
#define CM_8                             0x000000EFL

//
// MessageId: CM_9
//
// MessageText:
//
//  CM_9: %1$s: there is no active cache, cannot update
//
#define CM_9                             0x000000F0L

//
// MessageId: CM_10
//
// MessageText:
//
//  CM_10: %1$s: there is no active cache, cannot update
//
#define CM_10                            0x000000F1L

//
// MessageId: CM_11
//
// MessageText:
//
//  CM_11: %1$s: expected (%2$ld) points from feature (%3$ld) retrieved (%4$ld)
//
#define CM_11                            0x000000F2L

//
// MessageId: CM_12
//
// MessageText:
//
//  CM_12: %1$s: there is no active cache, cannot update
//
#define CM_12                            0x000000F3L

//
// MessageId: CM_13
//
// MessageText:
//
//  CM_13: %1$s: there is no active cache, cannot update
//
#define CM_13                            0x000000F4L

//
// MessageId: CM_14
//
// MessageText:
//
//  CM_14: %1$s: there is no active cache, cannot update
//
#define CM_14                            0x000000F5L

//
// MessageId: CM_15
//
// MessageText:
//
//  CM_15: %1$s: failed, there is no active Cache
//
#define CM_15                            0x000000F6L

//
// MessageId: CM_16
//
// MessageText:
//
//  CM_16: %1$s: failed, there is no active Cache
//
#define CM_16                            0x000000F7L

//
// MessageId: CM_17
//
// MessageText:
//
//  CM_17: %1$s: edge by node retrieval not initialized
//
#define CM_17                            0x000000F8L

//
// MessageId: CM_18
//
// MessageText:
//
//  CM_18: %1$s: failed, there is no active Cache
//
#define CM_18                            0x000000F9L

//
// MessageId: CM_19
//
// MessageText:
//
//  CM_19: %1$s: failed, there is no active Cache
//
#define CM_19                            0x000000FAL

//
// MessageId: CM_20
//
// MessageText:
//
//  CM_20: %1$s: edge by node retrieval not initialized
//
#define CM_20                            0x000000FBL

//
// MessageId: CM_21
//
// MessageText:
//
//  CM_21: %1$s: failed, there is no active Cache
//
#define CM_21                            0x000000FCL

//
// MessageId: CM_22
//
// MessageText:
//
//  CM_22: %1$s: could not create edge feature # (%2$ld)
//
#define CM_22                            0x000000FDL

//
// MessageId: CM_23
//
// MessageText:
//
//  CM_23: %1$s: failed, there is no active Cache
//
#define CM_23                            0x000000FEL

//
// MessageId: CM_24
//
// MessageText:
//
//  CM_24: %1$s: could not update edge feature # (%2$ld)
//
#define CM_24                            0x000000FFL

//
// MessageId: CM_25
//
// MessageText:
//
//  CM_25: %1$s: failed, there is no active Cache
//
#define CM_25                            0x00000100L

//
// MessageId: CM_26
//
// MessageText:
//
//  CM_26: %1$s: could not update polygon (%2$ld) for edge feature # (%3$ld))
//
#define CM_26                            0x00000101L

//
// MessageId: CM_27
//
// MessageText:
//
//  CM_27: %1$s: failed, there is no active Cache
//
#define CM_27                            0x00000102L

//
// MessageId: CM_28
//
// MessageText:
//
//  CM_28: %1$s: failed to clear polygon (%2$ld) for edge feature # (%3$ld))
//
#define CM_28                            0x00000103L

//
// MessageId: CM_29
//
// MessageText:
//
//  CM_29: %1$s: failed, there is no active Cache
//
#define CM_29                            0x00000104L

//
// MessageId: CM_30
//
// MessageText:
//
//  CM_30: %1$s: could not delete edge feature # (%2$ld), end side = '%3$c'
//
#define CM_30                            0x00000105L

//
// MessageId: CM_31
//
// MessageText:
//
//  CM_31: %1$s: failed, there is no active Cache
//
#define CM_31                            0x00000106L

//
// MessageId: CM_32
//
// MessageText:
//
//  CM_32: %1$s: could not update node info for edge feature # (%2$ld), end side = '%3$c'
//
#define CM_32                            0x00000107L

//
// MessageId: CM_33
//
// MessageText:
//
//  CM_33: %1$s: failed, there is no active Cache
//
#define CM_33                            0x00000108L

//
// MessageId: CM_34
//
// MessageText:
//
//  CM_34: %1$s: cache not initialized
//
#define CM_34                            0x00000109L

//
// MessageId: CM_35
//
// MessageText:
//
//  CM_35: %1$s: cache not initialized
//
#define CM_35                            0x0000010AL

//
// MessageId: CM_36
//
// MessageText:
//
//  CM_36: %1$s: there is no active cache, cannot update
//
#define CM_36                            0x0000010BL

//
// MessageId: CM_37
//
// MessageText:
//
//  CM_37: %1$s: there is no active cache, cannot update
//
#define CM_37                            0x0000010CL

//
// MessageId: CM_38
//
// MessageText:
//
//  CM_38: %1$s: there is no active cache, cannot update
//
#define CM_38                            0x0000010DL

//
// MessageId: CM_39
//
// MessageText:
//
//  CM_39: %1$s: there is no active cache, cannot update
//
#define CM_39                            0x0000010EL

//
// MessageId: CM_40
//
// MessageText:
//
//  CM_40: %1$s: there is no active cache, cannot update
//
#define CM_40                            0x0000010FL

//
// MessageId: CM_41
//
// MessageText:
//
//  CM_41: %1$s: there is no active feature, cannot get original
//
#define CM_41                            0x00000110L

//
// MessageId: CM_42
//
// MessageText:
//
//  CM_42: %1$s: there is no active feature, cannot add point
//
#define CM_42                            0x00000111L

//
// MessageId: CM_43
//
// MessageText:
//
//  CM_43: %1$s: there is no active feature, cannot delete points
//
#define CM_43                            0x00000112L

//
// MessageId: CM_44
//
// MessageText:
//
//  CM_44: %1$s: there is no active feature, cannot update
//
#define CM_44                            0x00000113L

//
// MessageId: CM_45
//
// MessageText:
//
//  CM_45: %1$s: there is no active feature, cannot update
//
#define CM_45                            0x00000114L

//
// MessageId: CM_46
//
// MessageText:
//
//  CM_46: %1$s: there is no active feature, cannot update
//
#define CM_46                            0x00000115L

//
// MessageId: CM_47
//
// MessageText:
//
//  CM_47: %1$s: there is no active feature, cannot update
//
#define CM_47                            0x00000116L

//
// MessageId: CM_48
//
// MessageText:
//
//  CM_48: %1$s: there is no active feature, cannot update
//
#define CM_48                            0x00000117L

//
// MessageId: CM_49
//
// MessageText:
//
//  CM_49: %1$s: internal error, NULL label row passed as input to this function.
//
#define CM_49                            0x00000118L

//
// MessageId: CM_50
//
// MessageText:
//
//  CM_50: %1$s: there is no active feature, cannot update label number %2$d.
//
#define CM_50                            0x00000119L

//
// MessageId: CM_51
//
// MessageText:
//
//  CM_51: %1$s: there is no active feature, cannot delete label number %2$d.
//
#define CM_51                            0x0000011AL

//
// MessageId: CM_52
//
// MessageText:
//
//  CM_52: %1$s: there is no active feature, cannot update
//
#define CM_52                            0x0000011BL

//
// MessageId: CM_53
//
// MessageText:
//
//  CM_53: %1$s: there is no active feature, cannot update
//
#define CM_53                            0x0000011CL

//
// MessageId: CM_54
//
// MessageText:
//
//  CM_54: %1$s: there is no active feature, cannot update
//
#define CM_54                            0x0000011DL

//
// MessageId: CM_55
//
// MessageText:
//
//  CM_55: %1$s: failed, there is no active Cache
//
#define CM_55                            0x0000011EL

//
// MessageId: CM_56
//
// MessageText:
//
//  CM_56: %1$s: internal error, cm_labels_create() returned a NULL pointer, cannot continue.
//
#define CM_56                            0x0000011FL

//
// MessageId: CM_57
//
// MessageText:
//
//  CM_57: Feature %1$ld not found, cannot get labels.
//
#define CM_57                            0x00000120L

//
// MessageId: CM_58
//
// MessageText:
//
//  CM_58: %1$s: label retrieval not initialized
//
#define CM_58                            0x00000121L

//
// MessageId: CM_59
//
// MessageText:
//
//  CM_59: %1$s: failed, there is no active Cache
//
#define CM_59                            0x00000122L

//
// MessageId: CM_60
//
// MessageText:
//
//  CM_60: %1$s: failed, there is no active Cache
//
#define CM_60                            0x00000123L

//
// MessageId: CM_61
//
// MessageText:
//
//  CM_61: %1$s: failed, there is no active Cache
//
#define CM_61                            0x00000124L

//
// MessageId: CM_62
//
// MessageText:
//
//  CM_62: %1$s: failed, there is no active Cache
//
#define CM_62                            0x00000125L

//
// MessageId: CM_63
//
// MessageText:
//
//  CM_63: %1$s: failed, there is no active Cache
//
#define CM_63                            0x00000126L

//
// MessageId: CM_64
//
// MessageText:
//
//  CM_64: Feature %1$ld not found, cannot get label number %2$d.
//
#define CM_64                            0x00000127L

//
// MessageId: CM_65
//
// MessageText:
//
//  CM_65: %1$s: failed, cannot get labels for feature (%2$ld)
//
#define CM_65                            0x00000128L

//
// MessageId: CM_66
//
// MessageText:
//
//  CM_66: %1$s: failed, there is no active Cache
//
#define CM_66                            0x00000129L

//
// MessageId: CM_67
//
// MessageText:
//
//  CM_67: %1$s: failed, there is no active Cache
//
#define CM_67                            0x0000012AL

//
// MessageId: CM_68
//
// MessageText:
//
//  CM_68: %1$s: failed there is no acitve cache
//
#define CM_68                            0x0000012BL

//
// MessageId: CM_69
//
// MessageText:
//
//  CM_69: %1$s: user query retrieval not initilized
//
#define CM_69                            0x0000012CL

//
// MessageId: CM_70
//
// MessageText:
//
//  CM_70: %1$s: failed there is no acitve cache
//
#define CM_70                            0x0000012DL

//
// MessageId: CM_71
//
// MessageText:
//
//  CM_71: %1$s: invalid cache id number %2$d
//
#define CM_71                            0x0000012EL

//
// MessageId: CM_72
//
// MessageText:
//
//  CM_72: %1$s: Cache activate failed
//
#define CM_72                            0x0000012FL

//
// MessageId: CM_73
//
// MessageText:
//
//  CM_73: %1$s: Cache allocation failed, too many allocated already
//
#define CM_73                            0x00000130L

//
// MessageId: CM_74
//
// MessageText:
//
//  CM_74: %1$s: invalid cache id number %2$d
//
#define CM_74                            0x00000131L

//
// MessageId: CM_75
//
// MessageText:
//
//  CM_75: %1$s: Cache Manager invalid id, cannot deallocate
//
#define CM_75                            0x00000132L

//
// MessageId: CM_76
//
// MessageText:
//
//  CM_76: %1$s: cannot destroy handle
//
#define CM_76                            0x00000133L

//
// MessageId: CM_77
//
// MessageText:
//
//  CM_77: %1$s: Cache Manager not initialized
//
#define CM_77                            0x00000134L

//
// MessageId: CM_78
//
// MessageText:
//
//  CM_78: %1$s: Unknown table %2$s, cannot flush
//
#define CM_78                            0x00000135L

//
// MessageId: CM_79
//
// MessageText:
//
//  CM_79: Stub for cm_layer_net_get
//
#define CM_79                            0x00000136L

//
// MessageId: CM_80
//
// MessageText:
//
//  CM_80: %1$s: null Cache handle
//
#define CM_80                            0x00000137L

//
// MessageId: CM_81
//
// MessageText:
//
//  CM_81: %1$s: bad magic number
//
#define CM_81                            0x00000138L

//
// MessageId: CM_82
//
// MessageText:
//
//  CM_82: %1$s: invalid start position (%2$ld) for active feature
//
#define CM_82                            0x00000139L

//
// MessageId: CM_83
//
// MessageText:
//
//  CM_83: %1$s: invalid end position (%2$ld) for active feature
//
#define CM_83                            0x0000013AL

//
// MessageId: CM_84
//
// MessageText:
//
//  CM_84: %1$s: invalid end position less than start position
//
#define CM_84                            0x0000013BL

//
// MessageId: CM_85
//
// MessageText:
//
//  CM_85: %1$s: received NULL pointer to pointer
//
#define CM_85                            0x0000013CL

//
// MessageId: CM_86
//
// MessageText:
//
//  CM_86: %1$s: cannot destroy handle
//
#define CM_86                            0x0000013DL

//
// MessageId: CM_87
//
// MessageText:
//
//  CM_87: %1$s: cannot load all coordinates for feature number %2$ld
//
#define CM_87                            0x0000013EL

//
// MessageId: CM_88
//
// MessageText:
//
//  CM_88: %1$s: expected coords for %2$d different master rows, got %3$d
//
#define CM_88                            0x0000013FL

//
// MessageId: CM_89
//
// MessageText:
//
//  CM_89: Get of coords ds array size failed, %1$s.
//
#define CM_89                            0x00000140L

//
// MessageId: CM_90
//
// MessageText:
//
//  CM_90: Get of coord row from ds array position %1$d failed, %2$s.
//
#define CM_90                            0x00000141L

//
// MessageId: CM_91
//
// MessageText:
//
//  CM_91: Destruction of coord object failed, %1$s.
//
#define CM_91                            0x00000142L

//
// MessageId: CM_92
//
// MessageText:
//
//  CM_92: Coordinate shift failed, %1$s.
//
#define CM_92                            0x00000143L

//
// MessageId: CM_93
//
// MessageText:
//
//  CM_93: Unable to create coordinate buffer, %1$s.
//
#define CM_93                            0x00000144L

//
// MessageId: CM_94
//
// MessageText:
//
//  CM_94: Unable to append point to coordinate buffer, %1$s
//
#define CM_94                            0x00000145L

//
// MessageId: CM_95
//
// MessageText:
//
//  CM_95: Unable to insert point in coordinate buffer, %1$s
//
#define CM_95                            0x00000146L

//
// MessageId: CM_96
//
// MessageText:
//
//  CM_96: %1$s: failed to duplicate coords
//
#define CM_96                            0x00000147L

//
// MessageId: CM_97
//
// MessageText:
//
//  CM_97: %1$s: row for feature %2$d plan %3$d version %4$d (%5$d points) already has %6$d points
//
#define CM_97                            0x00000148L

//
// MessageId: CM_98
//
// MessageText:
//
//  CM_98: %1$s: coords retrieval failed for feature %2$ld
//
#define CM_98                            0x00000149L

//
// MessageId: CM_99
//
// MessageText:
//
//  CM_99: %1$s: coords retrieval failed for feature %2$ld
//
#define CM_99                            0x0000014AL

//
// MessageId: CM_100
//
// MessageText:
//
//  CM_100: Point position %1$ld is invalid for %2$ld point feature %3$ld
//
#define CM_100                           0x0000014BL

//
// MessageId: CM_101
//
// MessageText:
//
//  CM_101: %1$s: invalid position = (%2$ld) for insert into active (%3$d) point feature
//
#define CM_101                           0x0000014CL

//
// MessageId: CM_102
//
// MessageText:
//
//  CM_102: %1$s: invalid position = (%2$ld) for update in active (%3$d) point feature
//
#define CM_102                           0x0000014DL

//
// MessageId: CM_103
//
// MessageText:
//
//  CM_103: %1$s: failed to update/add point
//
#define CM_103                           0x0000014EL

//
// MessageId: CM_104
//
// MessageText:
//
//  CM_104: %1$s: null object handle
//
#define CM_104                           0x0000014FL

//
// MessageId: CM_105
//
// MessageText:
//
//  CM_105: %1$s: null private pointer
//
#define CM_105                           0x00000150L

//
// MessageId: CM_106
//
// MessageText:
//
//  CM_106: %1$s: bad magic number
//
#define CM_106                           0x00000151L

//
// MessageId: CM_107
//
// MessageText:
//
//  CM_107: %1$s: cannot destroy handle
//
#define CM_107                           0x00000152L

//
// MessageId: CM_108
//
// MessageText:
//
//  CM_108: %1$s: null handle
//
#define CM_108                           0x00000153L

//
// MessageId: CM_109
//
// MessageText:
//
//  CM_109: %1$s: bad magic number
//
#define CM_109                           0x00000154L

//
// MessageId: CM_110
//
// MessageText:
//
//  CM_110: %1$s: point position %2$ld for %3$ld point copy buffer
//
#define CM_110                           0x00000155L

//
// MessageId: CM_111
//
// MessageText:
//
//  CM_111: %1$s: Cannot create an existing edge record
//
#define CM_111                           0x00000156L

//
// MessageId: CM_112
//
// MessageText:
//
//  CM_112: %1$s: Cannot update a deleted edge record
//
#define CM_112                           0x00000157L

//
// MessageId: CM_113
//
// MessageText:
//
//  CM_113: %1$s: Cannot update a deleted edge record
//
#define CM_113                           0x00000158L

//
// MessageId: CM_114
//
// MessageText:
//
//  CM_114: %1$s: Cannot update a deleted edge record
//
#define CM_114                           0x00000159L

//
// MessageId: CM_115
//
// MessageText:
//
//  CM_115: %1$s: cannot destroy handle
//
#define CM_115                           0x0000015AL

//
// MessageId: CM_116
//
// MessageText:
//
//  CM_116: Get of ds array size failed, -1 returned, %1$s.
//
#define CM_116                           0x0000015BL

//
// MessageId: CM_117
//
// MessageText:
//
//  CM_117: Append of master row to ds array failed, NULL address returned, %1$s.
//
#define CM_117                           0x0000015CL

//
// MessageId: CM_118
//
// MessageText:
//
//  CM_118: Get of master row from ds array position %1$d failed, NULL address returned, %2$s.
//
#define CM_118                           0x0000015DL

//
// MessageId: CM_119
//
// MessageText:
//
//  CM_119: Destroy of master rows ds array failed, %1$s
//
#define CM_119                           0x0000015EL

//
// MessageId: CM_120
//
// MessageText:
//
//  CM_120: Append of edge to ds array failed, NULL address returned, %1$s.
//
#define CM_120                           0x0000015FL

//
// MessageId: CM_121
//
// MessageText:
//
//  CM_121: Get of edge row from ds array position %1$d failed, NULL address returned, %2$s.
//
#define CM_121                           0x00000160L

//
// MessageId: CM_122
//
// MessageText:
//
//  CM_122: Delete of edge row from ds array position %1$d failed, %2$s.
//
#define CM_122                           0x00000161L

//
// MessageId: CM_123
//
// MessageText:
//
//  CM_123: Index_of search for edge row with fn_edge = %1$ld and end_side = '%2$c' in ds array failed, %3$s.
//
#define CM_123                           0x00000162L

//
// MessageId: CM_124
//
// MessageText:
//
//  CM_124: Address search for edge row with index %1$ld in ds array failed, %2$s.
//
#define CM_124                           0x00000163L

//
// MessageId: CM_125
//
// MessageText:
//
//  CM_125: Destroy of edge rows ds array failed, %1$s
//
#define CM_125                           0x00000164L

//
// MessageId: CM_126
//
// MessageText:
//
//  CM_126: Clean of edge rows ds array failed, %1$s
//
#define CM_126                           0x00000165L

//
// MessageId: CM_127
//
// MessageText:
//
//  CM_127: %1$s: failed to allocate fetch list
//
#define CM_127                           0x00000166L

//
// MessageId: CM_128
//
// MessageText:
//
//  CM_128: Issuing rollback to recover from transaction failure
//
#define CM_128                           0x00000167L

//
// MessageId: CM_129
//
// MessageText:
//
//  CM_129: Rollback complete
//
#define CM_129                           0x00000168L

//
// MessageId: CM_130
//
// MessageText:
//
//  CM_130: %1$s: cannot n_destroy handle
//
#define CM_130                           0x00000169L

//
// MessageId: CM_131
//
// MessageText:
//
//  CM_131: %1$s: null unit definition
//
#define CM_131                           0x0000016AL

//
// MessageId: CM_132
//
// MessageText:
//
//  CM_132: %1$s: bad magic number
//
#define CM_132                           0x0000016BL

//
// MessageId: CM_133
//
// MessageText:
//
//  CM_133: %1$s: cannot p_destroy handle
//
#define CM_133                           0x0000016CL

//
// MessageId: CM_134
//
// MessageText:
//
//  CM_134: %1$s: null unit definition
//
#define CM_134                           0x0000016DL

//
// MessageId: CM_135
//
// MessageText:
//
//  CM_135: %1$s: bad magic number
//
#define CM_135                           0x0000016EL

//
// MessageId: CM_136
//
// MessageText:
//
//  CM_136: %1$s: could not update edge feature # (%2$ld)
//
#define CM_136                           0x0000016FL

//
// MessageId: CM_137
//
// MessageText:
//
//  CM_137: %1$s: null handle
//
#define CM_137                           0x00000170L

//
// MessageId: CM_138
//
// MessageText:
//
//  CM_138: %1$s: bad magic number
//
#define CM_138                           0x00000171L

//
// MessageId: CM_139
//
// MessageText:
//
//  CM_139: Invalid point number %1$ld for a %2$ld point feature
//
#define CM_139                           0x00000172L

//
// MessageId: CM_140
//
// MessageText:
//
//  CM_140: %1$s: two features already active
//
#define CM_140                           0x00000173L

//
// MessageId: CM_141
//
// MessageText:
//
//  CM_141: %1$s: Internal error, unable to load active feature (%2$ld) into master cache
//
#define CM_141                           0x00000174L

//
// MessageId: CM_142
//
// MessageText:
//
//  CM_142: %1$s: Feature %2$ld not found, cannot update area
//
#define CM_142                           0x00000175L

//
// MessageId: CM_143
//
// MessageText:
//
//  CM_143: %1$s: Add of row to master cache failed, NULL returned from cm_master_add().
//
#define CM_143                           0x00000176L

//
// MessageId: CM_144
//
// MessageText:
//
//  CM_144: %1$s: Create of coords handle failed, NULL returned from cm_coords_create().
//
#define CM_144                           0x00000177L

//
// MessageId: CM_145
//
// MessageText:
//
//  CM_145: %1$s: Create of labels handle failed, NULL returned from cm_labels_create().
//
#define CM_145                           0x00000178L

//
// MessageId: CM_146
//
// MessageText:
//
//  CM_146: %1$s: Get of master cache row failed for feature %2$d, NULL returned from master->get().
//
#define CM_146                           0x00000179L

//
// MessageId: CM_147
//
// MessageText:
//
//  CM_147: %1$s: Unable to load coordinates for feature %2$d.
//
#define CM_147                           0x0000017AL

//
// MessageId: CM_148
//
// MessageText:
//
//  CM_148: %1$s: Unable to load labels for feature %2$d.
//
#define CM_148                           0x0000017BL

//
// MessageId: CM_149
//
// MessageText:
//
//  CM_149: %1$s: Unable to create labels object for feature %2$d.
//
#define CM_149                           0x0000017CL

//
// MessageId: CM_150
//
// MessageText:
//
//  CM_150: %1$s: Create of original master cache row failed for feature %2$d, NULL returned from cm_master_create().
//
#define CM_150                           0x0000017DL

//
// MessageId: CM_151
//
// MessageText:
//
//  CM_151: Failed to load labels for feature %1$ld, cannot continue.
//
#define CM_151                           0x0000017EL

//
// MessageId: CM_152
//
// MessageText:
//
//  CM_152: %1$s: Validation of feature handle failed, cannot destroy handle.
//
#define CM_152                           0x0000017FL

//
// MessageId: CM_153
//
// MessageText:
//
//  CM_153: %1$s: cannot compute length for feature (%2$ld)
//
#define CM_153                           0x00000180L

//
// MessageId: CM_154
//
// MessageText:
//
//  CM_154: %1$s: Feature %2$ld not found, cannot update has_attrs
//
#define CM_154                           0x00000181L

//
// MessageId: CM_155
//
// MessageText:
//
//  CM_155: %1$s: internal error, labels object for feature %2$ld not available, cannot delete label number %3$d.
//
#define CM_155                           0x00000182L

//
// MessageId: CM_156
//
// MessageText:
//
//  CM_156: %1$s: internal error, NULL label row passed as input to this function.
//
#define CM_156                           0x00000183L

//
// MessageId: CM_157
//
// MessageText:
//
//  CM_157: %1$s: internal error, labels object for feature %2$ld not available, cannot update label number %3$d.
//
#define CM_157                           0x00000184L

//
// MessageId: CM_158
//
// MessageText:
//
//  CM_158: %1$s: Feature %2$ld not found, cannot update row_lock
//
#define CM_158                           0x00000185L

//
// MessageId: CM_159
//
// MessageText:
//
//  CM_159: %1$s: two features already active
//
#define CM_159                           0x00000186L

//
// MessageId: CM_160
//
// MessageText:
//
//  CM_160: %1$s: points for feature %2$ld weren't loaded
//
#define CM_160                           0x00000187L

//
// MessageId: CM_161
//
// MessageText:
//
//  CM_161: %1$s: Unable to get master record for feature %2$ld
//
#define CM_161                           0x00000188L

//
// MessageId: CM_162
//
// MessageText:
//
//  CM_162: %1$s: points for feature %2$ld weren't loaded
//
#define CM_162                           0x00000189L

//
// MessageId: CM_163
//
// MessageText:
//
//  CM_163: %1$s: Feature %2$ld has fewer points than expected
//
#define CM_163                           0x0000018AL

//
// MessageId: CM_164
//
// MessageText:
//
//  CM_164: %1$s: points for feature %2$ld weren't loaded
//
#define CM_164                           0x0000018BL

//
// MessageId: CM_165
//
// MessageText:
//
//  CM_165: %1$s: internal error, NULL adb_g_master_def returned, cannot filter points.
//
#define CM_165                           0x0000018CL

//
// MessageId: CM_166
//
// MessageText:
//
//  CM_166: %1$s: points for feature %2$ld weren't loaded
//
#define CM_166                           0x0000018DL

//
// MessageId: CM_167
//
// MessageText:
//
//  CM_167: Cannot set z value; feature %1$ld is not a constant-z feature
//
#define CM_167                           0x0000018EL

//
// MessageId: CM_168
//
// MessageText:
//
//  CM_168: %1$s: Feature %2$ld not found, cannot update text
//
#define CM_168                           0x0000018FL

//
// MessageId: CM_169
//
// MessageText:
//
//  CM_169: %1$s: null handle
//
#define CM_169                           0x00000190L

//
// MessageId: CM_170
//
// MessageText:
//
//  CM_170: %1$s: bad magic number
//
#define CM_170                           0x00000191L

//
// MessageId: CM_171
//
// MessageText:
//
//  CM_171: %1$s: internal error, handle validation failed, cannot destroy handle.
//
#define CM_171                           0x00000192L

//
// MessageId: CM_172
//
// MessageText:
//
//  CM_172: %1$s: Internal error, cannot locate master row feat_num %2$ld associated with label number %3$d.
//
#define CM_172                           0x00000193L

//
// MessageId: CM_173
//
// MessageText:
//
//  CM_173: %1$s: Internal error, NULL label handle, cannot continue label fetch.
//
#define CM_173                           0x00000194L

//
// MessageId: CM_174
//
// MessageText:
//
//  CM_174: %1$s: internal error, label %2$ld version %3$ld for feature %4$ld plan %5$ld already in cache
//
#define CM_174                           0x00000195L

//
// MessageId: CM_175
//
// MessageText:
//
//  CM_175: Get of master ds array size failed, -1 returned, %1$s.
//
#define CM_175                           0x00000196L

//
// MessageId: CM_176
//
// MessageText:
//
//  CM_176: Append of master row to ds array failed, NULL address returned, %1$s.
//
#define CM_176                           0x00000197L

//
// MessageId: CM_177
//
// MessageText:
//
//  CM_177: Get of master row from ds array position %1$d failed, NULL address returned, %2$s.
//
#define CM_177                           0x00000198L

//
// MessageId: CM_178
//
// MessageText:
//
//  CM_178: Get of labels ds array size failed, -1 returned, %1$s.
//
#define CM_178                           0x00000199L

//
// MessageId: CM_179
//
// MessageText:
//
//  CM_179: Append of label to ds array failed, NULL address returned, %1$s.
//
#define CM_179                           0x0000019AL

//
// MessageId: CM_180
//
// MessageText:
//
//  CM_180: Get of label row from ds array position %1$d failed, NULL address returned, %2$s.
//
#define CM_180                           0x0000019BL

//
// MessageId: CM_181
//
// MessageText:
//
//  CM_181: Insert to label row to ds_array failed, NULL address returned, %1$s.
//
#define CM_181                           0x0000019CL

//
// MessageId: CM_182
//
// MessageText:
//
//  CM_182: Delete of label row (label number %1$d) from ds_array failed, %2$s
//
#define CM_182                           0x0000019DL

//
// MessageId: CM_183
//
// MessageText:
//
//  CM_183: Index lookup of label row in ds array failed, -1 returned, %1$s.
//
#define CM_183                           0x0000019EL

//
// MessageId: CM_184
//
// MessageText:
//
//  CM_184: %1$s: internal error, attempt to save same label number %2$d in ds array, cannot continue labels fetch.
//
#define CM_184                           0x0000019FL

//
// MessageId: CM_185
//
// MessageText:
//
//  CM_185: %1$s: Internal error, NULL pointer returned ds_arr_create request.
//
#define CM_185                           0x000001A0L

//
// MessageId: CM_186
//
// MessageText:
//
//  CM_186: %1$s: internal error, NULL master row returned by cm_master_public_get() for feature number %2$ld.
//
#define CM_186                           0x000001A1L

//
// MessageId: CM_187
//
// MessageText:
//
//  CM_187: %1$s: internal error, cm_labels_create() returned a NULL pointer, cannot continue.
//
#define CM_187                           0x000001A2L

//
// MessageId: CM_188
//
// MessageText:
//
//  CM_188: %1$s: internal error, ds_arr_create() returned a NULL pointer, cannot continue.
//
#define CM_188                           0x000001A3L

//
// MessageId: CM_189
//
// MessageText:
//
//  CM_189: %1$s: labels retrieval failed for feature %2$ld
//
#define CM_189                           0x000001A4L

//
// MessageId: CM_190
//
// MessageText:
//
//  CM_190: %1$s: internal error, NULL label row passed as input to this function.
//
#define CM_190                           0x000001A5L

//
// MessageId: CM_191
//
// MessageText:
//
//  CM_191: %1$s: null handle
//
#define CM_191                           0x000001A6L

//
// MessageId: CM_192
//
// MessageText:
//
//  CM_192: %1$s: bad magic number
//
#define CM_192                           0x000001A7L

//
// MessageId: CM_193
//
// MessageText:
//
//  CM_193: %1$s: NULL master handle cannot be destroyed.
//
#define CM_193                           0x000001A8L

//
// MessageId: CM_194
//
// MessageText:
//
//  CM_194: %1$s: Master handle validation failed, cannot destroy handle.
//
#define CM_194                           0x000001A9L

//
// MessageId: CM_195
//
// MessageText:
//
//  CM_195: Unable to create list of features, %1$s
//
#define CM_195                           0x000001AAL

//
// MessageId: CM_196
//
// MessageText:
//
//  CM_196: Unable to append to list of features, %1$s
//
#define CM_196                           0x000001ABL

//
// MessageId: CM_197
//
// MessageText:
//
//  CM_197: Unable to destroy list of features, %1$s
//
#define CM_197                           0x000001ACL

//
// MessageId: CM_198
//
// MessageText:
//
//  CM_198: Unable to clean list of features, %1$s
//
#define CM_198                           0x000001ADL

//
// MessageId: CM_199
//
// MessageText:
//
//  CM_199: Unable to determine size of list of features, %1$s
//
#define CM_199                           0x000001AEL

//
// MessageId: CM_200
//
// MessageText:
//
//  CM_200: Unable to get requested item from list of features, %1$s
//
#define CM_200                           0x000001AFL

//
// MessageId: CM_201
//
// MessageText:
//
//  CM_201: Unable to find requested item in list of features, %1$s
//
#define CM_201                           0x000001B0L

//
// MessageId: CM_202
//
// MessageText:
//
//  CM_202: Issuing rollback to recover from transaction failure
//
#define CM_202                           0x000001B1L

//
// MessageId: CM_203
//
// MessageText:
//
//  CM_203: Rollback complete
//
#define CM_203                           0x000001B2L

//
// MessageId: CM_204
//
// MessageText:
//
//  CM_204: %1$s: Corrupt master cache handle
//
#define CM_204                           0x000001B3L

//
// MessageId: CM_205
//
// MessageText:
//
//  CM_205: %1$s: add to ccl failed, no rows returned
//
#define CM_205                           0x000001B4L

//
// MessageId: CM_206
//
// MessageText:
//
//  CM_206: \nCM Spacial query statistics ............
//
#define CM_206                           0x000001B5L

//
// MessageId: CM_207
//
// MessageText:
//
//  CM_207: One point features       = (%1$ld)
//
#define CM_207                           0x000001B6L

//
// MessageId: CM_208
//
// MessageText:
//
//  CM_208: Two point features       = (%1$ld)
//
#define CM_208                           0x000001B7L

//
// MessageId: CM_209
//
// MessageText:
//
//  CM_209: More point features      = (%1$ld)
//
#define CM_209                           0x000001B8L

//
// MessageId: CM_210
//
// MessageText:
//
//  CM_210: \nCM MASTER Cache statistics  ............
//
#define CM_210                           0x000001B9L

//
// MessageId: CM_211
//
// MessageText:
//
//  CM_211: Cache hits               = (%1$ld)
//
#define CM_211                           0x000001BAL

//
// MessageId: CM_212
//
// MessageText:
//
//  CM_212: Cache misses             = (%1$ld)
//
#define CM_212                           0x000001BBL

//
// MessageId: CM_213
//
// MessageText:
//
//  CM_213: Last row hits            = (%1$ld)
//
#define CM_213                           0x000001BCL

//
// MessageId: CM_214
//
// MessageText:
//
//  CM_214: Last row misses          = (%1$ld)
//
#define CM_214                           0x000001BDL

//
// MessageId: CM_215
//
// MessageText:
//
//  CM_215: Work Cache hits          = (%1$ld)
//
#define CM_215                           0x000001BEL

//
// MessageId: CM_216
//
// MessageText:
//
//  CM_216: Work Cache misses        = (%1$ld)
//
#define CM_216                           0x000001BFL

//
// MessageId: CM_217
//
// MessageText:
//
//  CM_217: Cache Replaced/Flushed   = (%1$ld)
//
#define CM_217                           0x000001C0L

//
// MessageId: CM_218
//
// MessageText:
//
//  CM_218: Rows Replaced/Flushed    = (%1$ld)
//
#define CM_218                           0x000001C1L

//
// MessageId: CM_219
//
// MessageText:
//
//  CM_219: \nCM RDBMS statistics         ............
//
#define CM_219                           0x000001C2L

//
// MessageId: CM_220
//
// MessageText:
//
//  CM_220: RDBMS fetches on Master  = (%1$ld)
//
#define CM_220                           0x000001C3L

//
// MessageId: CM_221
//
// MessageText:
//
//  CM_221: RDBMS inserts on Master  = (%1$ld)
//
#define CM_221                           0x000001C4L

//
// MessageId: CM_222
//
// MessageText:
//
//  CM_222: RDBMS updates on Master  = (%1$ld)
//
#define CM_222                           0x000001C5L

//
// MessageId: CM_223
//
// MessageText:
//
//  CM_223: RDBMS deletes on Master  = (%1$ld)
//
#define CM_223                           0x000001C6L

//
// MessageId: CM_224
//
// MessageText:
//
//  CM_224: RDBMS fetches on Coords  = (%1$ld)
//
#define CM_224                           0x000001C7L

//
// MessageId: CM_225
//
// MessageText:
//
//  CM_225: RDBMS fetches on Labels  = (%1$ld)
//
#define CM_225                           0x000001C8L

//
// MessageId: CM_226
//
// MessageText:
//
//  CM_226: \nCM end of statistics         ............
//
#define CM_226                           0x000001C9L

//
// MessageId: CM_227
//
// MessageText:
//
//  CM_227: %1$s: null handle
//
#define CM_227                           0x000001CAL

//
// MessageId: CM_228
//
// MessageText:
//
//  CM_228: %1$s: bad magic number
//
#define CM_228                           0x000001CBL

//
// MessageId: CM_229
//
// MessageText:
//
//  CM_229: %1$s: where clause id (%2$d) is out of range
//
#define CM_229                           0x000001CCL

//
// MessageId: CM_230
//
// MessageText:
//
//  CM_230: %1$s: cannot deactivate query id (%2$d), not active
//
#define CM_230                           0x000001CDL

//
// MessageId: CM_231
//
// MessageText:
//
//  CM_231: %1$s: where clause id (%2$d) is out of range
//
#define CM_231                           0x000001CEL

//
// MessageId: CM_232
//
// MessageText:
//
//  CM_232: %1$s: could not find reference for query id (%2$d)
//
#define CM_232                           0x000001CFL

//
// MessageId: CM_233
//
// MessageText:
//
//  CM_233: %1$s: out of where clause slots
//
#define CM_233                           0x000001D0L

//
// MessageId: CM_234
//
// MessageText:
//
//  CM_234: %1$s: failed to create query for where clause (%2$s)
//
#define CM_234                           0x000001D1L

//
// MessageId: CM_235
//
// MessageText:
//
//  CM_235: %1$s: failed to create a handle for query (%2$d)
//
#define CM_235                           0x000001D2L

//
// MessageId: CM_236
//
// MessageText:
//
//  CM_236: %1$s: cannot destroy handle
//
#define CM_236                           0x000001D3L

//
// MessageId: CM_237
//
// MessageText:
//
//  CM_237: %1$s: master cache does not exists for query (%2$d)
//
#define CM_237                           0x000001D4L

//
// MessageId: CM_238
//
// MessageText:
//
//  CM_238: \nCM User query (%1$d)statistics ............
//
#define CM_238                           0x000001D5L

//
// MessageId: CM_239
//
// MessageText:
//
//  CM_239: One point features       = (%1$ld)
//
#define CM_239                           0x000001D6L

//
// MessageId: CM_240
//
// MessageText:
//
//  CM_240: Two point features       = (%1$ld)
//
#define CM_240                           0x000001D7L

//
// MessageId: CM_241
//
// MessageText:
//
//  CM_241: More point features      = (%1$ld)
//
#define CM_241                           0x000001D8L

//
// MessageId: CM_242
//
// MessageText:
//
//  CM_242: \nCM RDBMS statistics         ............
//
#define CM_242                           0x000001D9L

//
// MessageId: CM_243
//
// MessageText:
//
//  CM_243: RDBMS fetches on Master  = (%1$ld)
//
#define CM_243                           0x000001DAL

//
// MessageId: CM_244
//
// MessageText:
//
//  CM_244: RDBMS rows returned      = (%1$ld)
//
#define CM_244                           0x000001DBL

//
// MessageId: CM_245
//
// MessageText:
//
//  CM_245: \nCM end of statistics         ............
//
#define CM_245                           0x000001DCL

//
// MessageId: CM_246
//
// MessageText:
//
//  CM_246: %1$s: null handle
//
#define CM_246                           0x000001DDL

//
// MessageId: CM_247
//
// MessageText:
//
//  CM_247: %1$s: bad magic number
//
#define CM_247                           0x000001DEL

//
// MessageId: CM_248
//
// MessageText:
//
//  CM_248: %1$s: feature %2$d has corrupt data versions for plan %3$d
//
#define CM_248                           0x000001DFL

//
// MessageId: CM_249
//
// MessageText:
//
//  CM_249: %1$s: Feature %2$ld has version %3$ld coordinates but no version %4$ld master record
//
#define CM_249                           0x000001E0L

//
// MessageId: CM_250
//
// MessageText:
//
//  CM_250: %1$s: internal error, label %2$ld version %3$ld for feature %3$ld has no g_master version
//
#define CM_250                           0x000001E1L

//
// MessageId: CM_251
//
// MessageText:
//
//  CM_251: %1$s: Internal Error: Permanent Edge deletes aren't supposed to happen here
//
#define CM_251                           0x000001E2L

//
// MessageId: CM_252
//
// MessageText:
//
//  CM_252: %1$s: Feature %2$ld not found, cannot update version
//
#define CM_252                           0x000001E3L

//
// MessageId: CM_253
//
// MessageText:
//
//  CM_253: %1$s: coords for feature %2$ld plan %3$ld version %4$ld not in RDBMS
//
#define CM_253                           0x000001E4L

//
// MessageId: CM_254
//
// MessageText:
//
//  CM_254: %1$s: Feature %2$ld not found
//
#define CM_254                           0x000001E5L

//
// MessageId: CM_255
//
// MessageText:
//
//  CM_255: %1$s: Feature %2$d has no matching replica in plan %3$d
//
#define CM_255                           0x000001E6L

//
// MessageId: CM_256
//
// MessageText:
//
//  CM_256: %1$s: Feature retrieval query initialized
//
#define CM_256                           0x000001E7L

//
// MessageId: CM_257
//
// MessageText:
//
//  CM_257: %1$s: Can't create static feature %2$d :exceeds maximum feature sequence %3$d
//
#define CM_257                           0x000001E8L

//
// MessageId: CM_258
//
// MessageText:
//
//  CM_258: %1$s: Can't update database: there is no active version
//
#define CM_258                           0x000001E9L

//
// MessageId: CM_259
//
// MessageText:
//
//  CM_259: %1$s: No version of edge in selection set
//
#define CM_259                           0x000001EAL

//
// MessageId: CM_260
//
// MessageText:
//
//  CM_260: %1$s: Can't update feature %2$ld: Changed version or plan while feature was active
//
#define CM_260                           0x000001EBL

//
// MessageId: CM_261
//
// MessageText:
//
//  CM_261: %1$s: feature %2$ld plan %3$ld label %4$d has corrupt version chain at version %5$ld
//
#define CM_261                           0x000001ECL

//
// MessageId: CM_262
//
// MessageText:
//
//  CM_262: %1$s: there is an active feature, cannot change active plan
//
#define CM_262                           0x000001EDL

//
// MessageId: CM_263
//
// MessageText:
//
//  CM_263: %1$s: failed, cannot return a single row in all-versions query mode.
//
#define CM_263                           0x000001EEL

//
// MessageId: CM_264
//
// MessageText:
//
//  CM_264: %1$s: failed, cannot activate feature in all-versions query mode.
//
#define CM_264                           0x000001EFL

//
// MessageId: CM_265
//
// MessageText:
//
//  CM_265: %1$s: failed, cannot create feature in all-versions query mode.
//
#define CM_265                           0x000001F0L

//
// MessageId: CM_266
//
// MessageText:
//
//  CM_266: %1$s: failed, cannot update feature in all-versions query mode.
//
#define CM_266                           0x000001F1L

//
// MessageId: CM_267
//
// MessageText:
//
//  CM_267: %1$s: failed, cannot delete feature in all-versions query mode.
//
#define CM_267                           0x000001F2L

//
// MessageId: CM_268
//
// MessageText:
//
//  CM_268: %1$s: Can't update database in all-versions query mode.
//
#define CM_268                           0x000001F3L

//
// MessageId: CM_269
//
// MessageText:
//
//  CM_269: %1$s: failed, cannot create edge feature in all-versions query mode.
//
#define CM_269                           0x000001F4L

//
// MessageId: CM_270
//
// MessageText:
//
//  CM_270: %1$s: failed, cannot update edge feature in all-versions query mode.
//
#define CM_270                           0x000001F5L

//
// MessageId: CM_271
//
// MessageText:
//
//  CM_271: %1$s: failed, cannot update polygon in all-versions query mode.
//
#define CM_271                           0x000001F6L

//
// MessageId: CM_272
//
// MessageText:
//
//  CM_272: %1$s: failed, cannot update node info for edge feature in all-versions query mode.
//
#define CM_272                           0x000001F7L

//
// MessageId: CM_273
//
// MessageText:
//
//  CM_273: %1$s: cannot update coordinates in all-version query mode.
//
#define CM_273                           0x000001F8L

//
// MessageId: CM_274
//
// MessageText:
//
//  CM_274: %1$s: cannot update row in all-versions query mode.
//
#define CM_274                           0x000001F9L

//
// MessageId: CM_275
//
// MessageText:
//
//  CM_275: %1$s: internal error, RDBI column type %2$ld not supported by cm_rdb package.
//
#define CM_275                           0x000001FAL

//
// MessageId: CM_276
//
// MessageText:
//
//  CM_276: %1$s: plan handle validation failed.
//
#define CM_276                           0x000001FBL

//
// MessageId: CM_277
//
// MessageText:
//
//  CM_277: %1$s: NULL plan handle cannot be destroyed.
//
#define CM_277                           0x000001FCL

//
// MessageId: CM_278
//
// MessageText:
//
//  CM_278: %1$s: Cannot get active plan, no plan is active.
//
#define CM_278                           0x000001FDL

//
// MessageId: CM_279
//
// MessageText:
//
//  CM_279: %1$s: plan %2$ld not found in database, cannot activate.
//
#define CM_279                           0x000001FEL

//
// MessageId: CM_280
//
// MessageText:
//
//  CM_280: %1$s: rdb handle validation failed.
//
#define CM_280                           0x000001FFL

//
// MessageId: CM_281
//
// MessageText:
//
//  CM_281: %1$s: NULL rdb handle cannot be destroyed.
//
#define CM_281                           0x00000200L

//
// MessageId: CM_282
//
// MessageText:
//
//  CM_282: %1$s: plan %2$ld for feature %3$ld not found in plan table.
//
#define CM_282                           0x00000201L

//
// MessageId: CM_283
//
// MessageText:
//
//  CM_283: %1$s: Plan %2$ld not found.
//
#define CM_283                           0x00000202L

//
// MessageId: CM_284
//
// MessageText:
//
//  CM_284: %1$s: Plan %2$ld exists, Cannot insert.
//
#define CM_284                           0x00000203L

//
// MessageId: CM_285
//
// MessageText:
//
//  CM_285: %1$s: Plan %2$ld doesn't exist, Cannot update.
//
#define CM_285                           0x00000204L

//
// MessageId: CM_286
//
// MessageText:
//
//  CM_286: %1$s: there is not active feature, cannot get geometric type.
//
#define CM_286                           0x00000205L

//
// MessageId: CM_287
//
// MessageText:
//
//  CM_287: %1$s: there is not active feature, cannot delete parameters.
//
#define CM_287                           0x00000206L

//
// MessageId: CM_288
//
// MessageText:
//
//  CM_288: %1$s: there is not active feature, cannot add parameters.
//
#define CM_288                           0x00000207L

//
// MessageId: CM_289
//
// MessageText:
//
//  CM_289: %1$s: failed to add curve definition.
//
#define CM_289                           0x00000208L

//
// MessageId: CM_290
//
// MessageText:
//
//  CM_290: %1$s: there is no active feature, cannot expand parameters.
//
#define CM_290                           0x00000209L

//
// MessageId: CM_291
//
// MessageText:
//
//  CM_291: %1$s: ccl handle validation failed.
//
#define CM_291                           0x0000020AL

//
// MessageId: CM_292
//
// MessageText:
//
//  CM_292: %1$s: NULL ccl handle cannot be destroyed.
//
#define CM_292                           0x0000020BL

//
// MessageId: CM_293
//
// MessageText:
//
//  CM_293: %1$s: object handle validation failed.
//
#define CM_293                           0x0000020CL

//
// MessageId: CM_294
//
// MessageText:
//
//  CM_294: update
//
#define CM_294                           0x0000020DL

//
// MessageId: CM_295
//
// MessageText:
//
//  CM_295: delete
//
#define CM_295                           0x0000020EL

//
// MessageId: CM_296
//
// MessageText:
//
//  CM_296: %1$s: Object %2$ld not found, cannot %3$s.
//
#define CM_296                           0x0000020FL

//
// MessageId: CM_297
//
// MessageText:
//
//  CM_297: %1$s: NULL object handle cannot be destroyed.
//
#define CM_297                           0x00000210L

//
// MessageId: CM_298
//
// MessageText:
//
//  CM_298: %1$s: failed to insert object %2$ld into cache.
//
#define CM_298                           0x00000211L

//
// MessageId: CM_299
//
// MessageText:
//
//  CM_299: %1$s: Can't update feature %2$ld: Changed update mode while feature was active
//
#define CM_299                           0x00000212L

//
// MessageId: CM_300
//
// MessageText:
//
//  CM_300: create
//
#define CM_300                           0x00000213L

//
// MessageId: CM_301
//
// MessageText:
//
//  CM_301: object
//
#define CM_301                           0x00000214L

//
// MessageId: CM_302
//
// MessageText:
//
//  CM_302: row
//
#define CM_302                           0x00000215L

//
// MessageId: CM_303
//
// MessageText:
//
//  CM_303: %1$s: failed, cannot %2$s %3$s in all-versions query mode.
//
#define CM_303                           0x00000216L

//
// MessageId: CM_304
//
// MessageText:
//
//  CM_304: %1$s: Can't create static object number %2$ld :exceeds maximum feature sequence.
//
#define CM_304                           0x00000217L

//
// MessageId: CM_305
//
// MessageText:
//
//  CM_305: Conflicting input parameters: plan %1$s, plan_group %2$s.
//
#define CM_305                           0x00000218L

//
// MessageId: CM_306
//
// MessageText:
//
//  CM_306: %1$s: Cannot get plan group %2$ld."
//
#define CM_306                           0x00000219L

//
// MessageId: CM_307
//
// MessageText:
//
//  CM_307: %1$s: plan groups handle validation failed.
//
#define CM_307                           0x0000021AL

//
// MessageId: CM_308
//
// MessageText:
//
//  CM_308: %1$s: Plan group %2$ld not found.
//
#define CM_308                           0x0000021BL

//
// MessageId: CM_309
//
// MessageText:
//
//  CM_309: %1$s: NULL plan group handle cannot be destroyed.
//
#define CM_309                           0x0000021CL

//
// MessageId: CM_310
//
// MessageText:
//
//  CM_310: %1$s: Plan group %2$ld exists, Cannot insert.
//
#define CM_310                           0x0000021DL

//
// MessageId: CM_311
//
// MessageText:
//
//  CM_311: %1$s: Plan group %2$ld doesn't exist, Cannot update.
//
#define CM_311                           0x0000021EL

//
// MessageId: CM_312
//
// MessageText:
//
//  CM_312: %1$s: Feature %2$ld  plan %3$ld  version %4$ld has no coordinates, cannot insert.
//
#define CM_312                           0x0000021FL

//
// MessageId: CM_313
//
// MessageText:
//
//  CM_313: %1$s: NULL geometry for feature number %2$ld.
//
#define CM_313                           0x00000220L

//
// MessageId: CM_314
//
// MessageText:
//
//  CM_314: %1$s: not enough points for feature number %2$ld (expected %3$ld got %4$ld).
//
#define CM_314                           0x00000221L

//
// MessageId: CM_315
//
// MessageText:
//
//  CM_315: %1$s: Cannot set active plan %2$ld.
//
#define CM_315                           0x00000222L

//
// MessageId: CSM_1
//
// MessageText:
//
//  CSM_1: You must be a super user to delete a definition
//
#define CSM_1                            0x00000223L

//
// MessageId: CSM_2
//
// MessageText:
//
//  CSM_2: Can't retrieve definition of circuit %1$d of Geodetic Datum
//
#define CSM_2                            0x00000224L

//
// MessageId: CSM_3
//
// MessageText:
//
//  CSM_3: Coordinate system `%1$s' doesn't exist
//
#define CSM_3                            0x00000225L

//
// MessageId: CSM_4
//
// MessageText:
//
//  CSM_4: You must be a super user to load definitions
//
#define CSM_4                            0x00000226L

//
// MessageId: CSM_5
//
// MessageText:
//
//  CSM_5: Projection is not specified
//
#define CSM_5                            0x00000227L

//
// MessageId: CSM_6
//
// MessageText:
//
//  CSM_6: Can't obtain definition of `%1$s' for formatting.
//
#define CSM_6                            0x00000228L

//
// MessageId: CSM_7
//
// MessageText:
//
//  CSM_7: csm_def_pp: Invalid Handle
//
#define CSM_7                            0x00000229L

//
// MessageId: CSM_8
//
// MessageText:
//
//  CSM_8: csm_def_separate: No definition provided
//
#define CSM_8                            0x0000022AL

//
// MessageId: CSM_9
//
// MessageText:
//
//  CSM_9: csm_def_separate: Unable to obtain definition type
//
#define CSM_9                            0x0000022BL

//
// MessageId: CSM_10
//
// MessageText:
//
//  CSM_10: csm_def_separate: unkown definition type '%1$s'
//
#define CSM_10                           0x0000022CL

//
// MessageId: CSM_11
//
// MessageText:
//
//  CSM_11: Undefined projection: %1$s; %2$s can't be updated.
//
#define CSM_11                           0x0000022DL

//
// MessageId: CSM_12
//
// MessageText:
//
//  CSM_12: Can't find unit definition: %1$s
//
#define CSM_12                           0x0000022EL

//
// MessageId: CSM_13
//
// MessageText:
//
//  CSM_13: Can't find projection definition: %1$s
//
#define CSM_13                           0x0000022FL

//
// MessageId: CSM_14
//
// MessageText:
//
//  CSM_14: Can't find spheroid definition: %1$s
//
#define CSM_14                           0x00000230L

//
// MessageId: CSM_15
//
// MessageText:
//
//  CSM_15: %1$s: handle is null
//
#define CSM_15                           0x00000231L

//
// MessageId: CSM_16
//
// MessageText:
//
//  CSM_16: %1$s: handle has null private
//
#define CSM_16                           0x00000232L

//
// MessageId: CSM_17
//
// MessageText:
//
//  CSM_17: %1$s: handle has bad magic number
//
#define CSM_17                           0x00000233L

//
// MessageId: CSM_18
//
// MessageText:
//
//  CSM_18: %1$s: invalid column number (%2$d)
//
#define CSM_18                           0x00000234L

//
// MessageId: CSM_19
//
// MessageText:
//
//  CSM_19: %1$s: invalid column number '%2$d'
//
#define CSM_19                           0x00000235L

//
// MessageId: CSM_20
//
// MessageText:
//
//  CSM_20: %1$s: invalid column number '%2$d'
//
#define CSM_20                           0x00000236L

//
// MessageId: CSM_21
//
// MessageText:
//
//  CSM_21: %1$s: unknown listing type '%2$d'
//
#define CSM_21                           0x00000237L

//
// MessageId: CSM_22
//
// MessageText:
//
//  CSM_22: %1$s: cannot destroy handle
//
#define CSM_22                           0x00000238L

//
// MessageId: CSM_23
//
// MessageText:
//
//  CSM_23: %1$s: cannot get list identifier, invalid handle
//
#define CSM_23                           0x00000239L

//
// MessageId: CSM_24
//
// MessageText:
//
//  CSM_24: %1$s: illigal line number (%2$d), valid listing lines are: from (1) to (%3$d)
//
#define CSM_24                           0x0000023AL

//
// MessageId: CSM_25
//
// MessageText:
//
//  CSM_25: %1$s: cannot get list identifier, invalid handle
//
#define CSM_25                           0x0000023BL

//
// MessageId: CSM_26
//
// MessageText:
//
//  CSM_26: %1$s: cannot get list identifier, invalid handle
//
#define CSM_26                           0x0000023CL

//
// MessageId: CSM_27
//
// MessageText:
//
//  CSM_27: %1$s: cannot get list identifier, invalid handle
//
#define CSM_27                           0x0000023DL

//
// MessageId: CSM_28
//
// MessageText:
//
//  CSM_28: %1$s: cannot get table column, invalid handle
//
#define CSM_28                           0x0000023EL

//
// MessageId: CSM_29
//
// MessageText:
//
//  CSM_29: %1$s: invalid listing row (NIL)
//
#define CSM_29                           0x0000023FL

//
// MessageId: CSM_30
//
// MessageText:
//
//  CSM_30: %1$s: unknown listing identifier '%2$d'
//
#define CSM_30                           0x00000240L

//
// MessageId: CSM_31
//
// MessageText:
//
//  CSM_31: %1$s: null list definition
//
#define CSM_31                           0x00000241L

//
// MessageId: CSM_32
//
// MessageText:
//
//  CSM_32: %1$s: bad magic number
//
#define CSM_32                           0x00000242L

//
// MessageId: CSM_33
//
// MessageText:
//
//  CSM_33: csm_prjn_binary: don't know how to convert %1$s
//
#define CSM_33                           0x00000243L

//
// MessageId: CSM_34
//
// MessageText:
//
//  CSM_34: Csm_prjn_cache_cons: Invalid request
//
#define CSM_34                           0x00000244L

//
// MessageId: CSM_35
//
// MessageText:
//
//  CSM_35: %1$s: corrupted parameter
//
#define CSM_35                           0x00000245L

//
// MessageId: CSM_36
//
// MessageText:
//
//  CSM_36: Can not convert definition of the `%1$s' coordinate system
//
#define CSM_36                           0x00000246L

//
// MessageId: CSM_37
//
// MessageText:
//
//  CSM_37: Invalid `Base' specification in %1$s definition
//
#define CSM_37                           0x00000247L

//
// MessageId: CSM_38
//
// MessageText:
//
//  CSM_38: Can't find the definition of '%1$s'
//
#define CSM_38                           0x00000248L

//
// MessageId: CSM_39
//
// MessageText:
//
//  CSM_39: Corrupted cache entry for '%1$s'
//
#define CSM_39                           0x00000249L

//
// MessageId: CSM_40
//
// MessageText:
//
//  CSM_40: Can't find base projection (%1$s) for %2$s
//
#define CSM_40                           0x0000024AL

//
// MessageId: CSM_41
//
// MessageText:
//
//  CSM_41: Projection `%1$s' doesn't exist in the system
//
#define CSM_41                           0x0000024BL

//
// MessageId: CSM_42
//
// MessageText:
//
//  CSM_42: Base mismatch: `%1$s' and `%2$s'
//
#define CSM_42                           0x0000024CL

//
// MessageId: CSM_43
//
// MessageText:
//
//  CSM_43: Definition of `%1$s' does not match
//
#define CSM_43                           0x0000024DL

//
// MessageId: CSM_44
//
// MessageText:
//
//  CSM_44: Can't retrieve definition of `%1$s'
//
#define CSM_44                           0x0000024EL

//
// MessageId: CSM_45
//
// MessageText:
//
//  CSM_45: Can't delete - definition of `%1$s' is based on `%2$s'
//
#define CSM_45                           0x0000024FL

//
// MessageId: CSM_46
//
// MessageText:
//
//  CSM_46: Can't delete the %1$s projection - %2$d dependencies detected
//
#define CSM_46                           0x00000250L

//
// MessageId: CSM_47
//
// MessageText:
//
//  CSM_47: Can't find definition of `%1$s' projection
//
#define CSM_47                           0x00000251L

//
// MessageId: CSM_48
//
// MessageText:
//
//  CSM_48: Can't initialize the exception table
//
#define CSM_48                           0x00000252L

//
// MessageId: CSM_49
//
// MessageText:
//
//  CSM_49: %1$s: %2$s is not a valid base abbreviation
//
#define CSM_49                           0x00000253L

//
// MessageId: CSM_50
//
// MessageText:
//
//  CSM_50: Base parameter is missing
//
#define CSM_50                           0x00000254L

//
// MessageId: CSM_51
//
// MessageText:
//
//  CSM_51: Base parameter is missing in `%1$s' projections
//
#define CSM_51                           0x00000255L

//
// MessageId: CSM_52
//
// MessageText:
//
//  CSM_52: Can't recognize the %1$s projection/coordinate system
//
#define CSM_52                           0x00000256L

//
// MessageId: CSM_53
//
// MessageText:
//
//  CSM_53: csm/prjn/get_names: Can't obtain the definition #%1$d
//
#define CSM_53                           0x00000257L

//
// MessageId: CSM_54
//
// MessageText:
//
//  CSM_54: Can't recognize the %1$s projection/coordinate system
//
#define CSM_54                           0x00000258L

//
// MessageId: CSM_55
//
// MessageText:
//
//  CSM_55: csm/def/pp: invalid handle
//
#define CSM_55                           0x00000259L

//
// MessageId: CSM_56
//
// MessageText:
//
//  CSM_56: csm_prjn_hndl_ascii: Invalid Projection Handle
//
#define CSM_56                           0x0000025AL

//
// MessageId: CSM_57
//
// MessageText:
//
//  CSM_57: Invalid base specification - will result in circular base dependency
//
#define CSM_57                           0x0000025BL

//
// MessageId: CSM_58
//
// MessageText:
//
//  CSM_58: NULL variant
//
#define CSM_58                           0x0000025CL

//
// MessageId: CSM_59
//
// MessageText:
//
//  CSM_59: Invalid parameter name in definition of %1$s
//
#define CSM_59                           0x0000025DL

//
// MessageId: CSM_60
//
// MessageText:
//
//  CSM_60: Invalid parameter value in `%1$s %2$s'
//
#define CSM_60                           0x0000025EL

//
// MessageId: CSM_61
//
// MessageText:
//
//  CSM_61: Can't find definition of `%1$s' projection
//
#define CSM_61                           0x0000025FL

//
// MessageId: CSM_62
//
// MessageText:
//
//  CSM_62: Warning: New projection definitions will be processed only
//
#define CSM_62                           0x00000260L

//
// MessageId: CSM_63
//
// MessageText:
//
//  CSM_63: Can't find definition of `%1$s' projection
//
#define CSM_63                           0x00000261L

//
// MessageId: CSM_64
//
// MessageText:
//
//  CSM_64: Non-Existing projection definitions has been encountered
//
#define CSM_64                           0x00000262L

//
// MessageId: CSM_65
//
// MessageText:
//
//  CSM_65: %1$s is already in the system
//
#define CSM_65                           0x00000263L

//
// MessageId: CSM_66
//
// MessageText:
//
//  CSM_66: Base projection is not specified
//
#define CSM_66                           0x00000264L

//
// MessageId: CSM_67
//
// MessageText:
//
//  CSM_67: Base projection '%1$s' doesn't exist
//
#define CSM_67                           0x00000265L

//
// MessageId: CSM_68
//
// MessageText:
//
//  CSM_68: csm_prjn_vld_parms: invalid base type %1$s
//
#define CSM_68                           0x00000266L

//
// MessageId: CSM_69
//
// MessageText:
//
//  CSM_69: %1$s parameters do not match: %2$s V.S. %3$s
//
#define CSM_69                           0x00000267L

//
// MessageId: CSM_70
//
// MessageText:
//
//  CSM_70: '%1$s' parameters ('%2$s' and '%3$s') don't match
//
#define CSM_70                           0x00000268L

//
// MessageId: CSM_71
//
// MessageText:
//
//  CSM_71: '%1$s' parameters ('%2$s' and '%3$s') don't match
//
#define CSM_71                           0x00000269L

//
// MessageId: CSM_72
//
// MessageText:
//
//  CSM_72: '%1$s' parameters ('%2$s' and '%3$s') don't match
//
#define CSM_72                           0x0000026AL

//
// MessageId: CSM_73
//
// MessageText:
//
//  CSM_73: Datum type mismatch: `%1$s' and `%2$s'
//
#define CSM_73                           0x0000026BL

//
// MessageId: CSM_74
//
// MessageText:
//
//  CSM_74: Circuit number mismatch:  %1$d and %2$d
//
#define CSM_74                           0x0000026CL

//
// MessageId: CSM_75
//
// MessageText:
//
//  CSM_75: Number of standard parallels doesn't match
//
#define CSM_75                           0x0000026DL

//
// MessageId: CSM_76
//
// MessageText:
//
//  CSM_76: '%1$s' parameters ('%2$s' and '%3$s') do not match
//
#define CSM_76                           0x0000026EL

//
// MessageId: CSM_77
//
// MessageText:
//
//  CSM_77: '%1$s' parameters ('%2$s' and '%3$s') do not match
//
#define CSM_77                           0x0000026FL

//
// MessageId: CSM_78
//
// MessageText:
//
//  CSM_78: '%1$s' parameters ('%2$s' and '%3$s') do not match
//
#define CSM_78                           0x00000270L

//
// MessageId: CSM_79
//
// MessageText:
//
//  CSM_79: '%1$s' parameters ('%2$s' and '%3$s') do not match
//
#define CSM_79                           0x00000271L

//
// MessageId: CSM_80
//
// MessageText:
//
//  CSM_80: Origin specification is missing in definition of %1$s
//
#define CSM_80                           0x00000272L

//
// MessageId: CSM_81
//
// MessageText:
//
//  CSM_81: Unexpected spheroid `%1$s' in definition of `%2$s' - Hayford expected
//
#define CSM_81                           0x00000273L

//
// MessageId: CSM_82
//
// MessageText:
//
//  CSM_82: Invalid datum, `%1$s', in NZMG based projection
//
#define CSM_82                           0x00000274L

//
// MessageId: CSM_83
//
// MessageText:
//
//  CSM_83: '%1$s' parameter is missing in definition of %2$s
//
#define CSM_83                           0x00000275L

//
// MessageId: CSM_84
//
// MessageText:
//
//  CSM_84: Spheroid `%1$s' in definition of `%2$s' - Bessel expected
//
#define CSM_84                           0x00000276L

//
// MessageId: CSM_85
//
// MessageText:
//
//  CSM_85: Unexpected spheroid `%1$s' in definition of `%2$s' - Hayford expected
//
#define CSM_85                           0x00000277L

//
// MessageId: CSM_86
//
// MessageText:
//
//  CSM_86: Invalid Easting specification, %1$d in zone %2$s
//
#define CSM_86                           0x00000278L

//
// MessageId: CSM_87
//
// MessageText:
//
//  CSM_87: Specified latitude label `%1$s' is invalid
//
#define CSM_87                           0x00000279L

//
// MessageId: CSM_88
//
// MessageText:
//
//  CSM_88: Invalid lower left latitude label: `%1$s' 
//
#define CSM_88                           0x0000027AL

//
// MessageId: CSM_89
//
// MessageText:
//
//  CSM_89: Invalid Northing specification, %1$s in zone %2$s
//
#define CSM_89                           0x0000027BL

//
// MessageId: CSM_90
//
// MessageText:
//
//  CSM_90: Warning: only %1$d missing latitude labels were stored
//
#define CSM_90                           0x0000027CL

//
// MessageId: CSM_91
//
// MessageText:
//
//  CSM_91: Invalid zone specification '%1$s'
//
#define CSM_91                           0x0000027DL

//
// MessageId: CSM_92
//
// MessageText:
//
//  CSM_92: Invalid zone specification '%1$s'
//
#define CSM_92                           0x0000027EL

//
// MessageId: CSM_93
//
// MessageText:
//
//  CSM_93: Invalid zone specification '%1$s'
//
#define CSM_93                           0x0000027FL

//
// MessageId: CSM_94
//
// MessageText:
//
//  CSM_94: Invalid zone specification '%1$s'
//
#define CSM_94                           0x00000280L

//
// MessageId: CSM_95
//
// MessageText:
//
//  CSM_95: Can't find the definition of %1$s base projection
//
#define CSM_95                           0x00000281L

//
// MessageId: CSM_96
//
// MessageText:
//
//  CSM_96: The %1$s parameter is missing
//
#define CSM_96                           0x00000282L

//
// MessageId: CSM_97
//
// MessageText:
//
//  CSM_97: The %1$s parameter is missing
//
#define CSM_97                           0x00000283L

//
// MessageId: CSM_98
//
// MessageText:
//
//  CSM_98: Reserved Abbreviation conflict: '%1$s' should be '%2$s'
//
#define CSM_98                           0x00000284L

//
// MessageId: CSM_99
//
// MessageText:
//
//  CSM_99: Reserved Name conflict: '%1$s' should be '%2$s'
//
#define CSM_99                           0x00000285L

//
// MessageId: CSM_100
//
// MessageText:
//
//  CSM_100: Two instances, `%1$s' and `%2$s', of parameter %3$s has been encountered
//
#define CSM_100                          0x00000286L

//
// MessageId: CSM_101
//
// MessageText:
//
//  CSM_101: Can't retrieve the definitions of '%1$s'
//
#define CSM_101                          0x00000287L

//
// MessageId: CSM_102
//
// MessageText:
//
//  CSM_102: Unable to retrieve parameter sequence %1$d
//
#define CSM_102                          0x00000288L

//
// MessageId: CSM_103
//
// MessageText:
//
//  CSM_103: Unexpected Parameter `%1$s' in `%2$s' based projection
//
#define CSM_103                          0x00000289L

//
// MessageId: CSM_104
//
// MessageText:
//
//  CSM_104: The %1$s parameter is missing
//
#define CSM_104                          0x0000028AL

//
// MessageId: CSM_105
//
// MessageText:
//
//  CSM_105: Inconsistent abbreviation <%1$s> and <%2$s>
//
#define CSM_105                          0x0000028BL

//
// MessageId: CSM_106
//
// MessageText:
//
//  CSM_106: Specification of the `%1$s' parameter is missing in %2$s based projection
//
#define CSM_106                          0x0000028CL

//
// MessageId: CSM_107
//
// MessageText:
//
//  CSM_107: A numeric value is expected for parameter %1$s
//
#define CSM_107                          0x0000028DL

//
// MessageId: CSM_108
//
// MessageText:
//
//  CSM_108: Invalid numeric value in parameter %1$s
//
#define CSM_108                          0x0000028EL

//
// MessageId: CSM_109
//
// MessageText:
//
//  CSM_109: Specified unit of %1$s parameter is unrecognizable - an angular quantity must be specified
//
#define CSM_109                          0x0000028FL

//
// MessageId: CSM_110
//
// MessageText:
//
//  CSM_110: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_110                          0x00000290L

//
// MessageId: CSM_111
//
// MessageText:
//
//  CSM_111: Specified value of %1$s parameter is out of range
//
#define CSM_111                          0x00000291L

//
// MessageId: CSM_112
//
// MessageText:
//
//  CSM_112: Inavlid Scale factor value
//
#define CSM_112                          0x00000292L

//
// MessageId: CSM_113
//
// MessageText:
//
//  CSM_113: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_113                          0x00000293L

//
// MessageId: CSM_114
//
// MessageText:
//
//  CSM_114: Range error: Latitude value crosses the pole
//
#define CSM_114                          0x00000294L

//
// MessageId: CSM_115
//
// MessageText:
//
//  CSM_115: Range error: Longitude values exceed valid limits
//
#define CSM_115                          0x00000295L

//
// MessageId: CSM_116
//
// MessageText:
//
//  CSM_116: Range error in latitude values
//
#define CSM_116                          0x00000296L

//
// MessageId: CSM_117
//
// MessageText:
//
//  CSM_117: Range error in longitude values
//
#define CSM_117                          0x00000297L

//
// MessageId: CSM_118
//
// MessageText:
//
//  CSM_118: Origin of projection is outside the valid range
//
#define CSM_118                          0x00000298L

//
// MessageId: CSM_119
//
// MessageText:
//
//  CSM_119: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_119                          0x00000299L

//
// MessageId: CSM_120
//
// MessageText:
//
//  CSM_120: Value of %1$s is outside the valid range
//
#define CSM_120                          0x0000029AL

//
// MessageId: CSM_121
//
// MessageText:
//
//  CSM_121: Error in (%1$s) - it defines Transversal aspect, same longitudes.
//
#define CSM_121                          0x0000029BL

//
// MessageId: CSM_122
//
// MessageText:
//
//  CSM_122: Error in (%1$s) - invalid centre line parameters, same latitudes.
//
#define CSM_122                          0x0000029CL

//
// MessageId: CSM_123
//
// MessageText:
//
//  CSM_123: Error in (%1$s) - invalid centre line parameters, latidute 1 = 0.0.
//
#define CSM_123                          0x0000029DL

//
// MessageId: CSM_124
//
// MessageText:
//
//  CSM_124: Error in (%1$s) - invalid latitude of origin.
//
#define CSM_124                          0x0000029EL

//
// MessageId: CSM_125
//
// MessageText:
//
//  CSM_125: Error in (%1$s) - Centre Line First and Second point must be on the same side of the Equator.
//
#define CSM_125                          0x0000029FL

//
// MessageId: CSM_126
//
// MessageText:
//
//  CSM_126: Error in (%1$s) - Min. or Max. of a Centre Line is between First and Second point .
//
#define CSM_126                          0x000002A0L

//
// MessageId: CSM_127
//
// MessageText:
//
//  CSM_127: Error in (%1$s) - Maximum of a Centre Line is between First and Second point .
//
#define CSM_127                          0x000002A1L

//
// MessageId: CSM_128
//
// MessageText:
//
//  CSM_128: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_128                          0x000002A2L

//
// MessageId: CSM_129
//
// MessageText:
//
//  CSM_129: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_129                          0x000002A3L

//
// MessageId: CSM_130
//
// MessageText:
//
//  CSM_130: Value of %1$s parameter is incorrect - a linear quantity must be specified
//
#define CSM_130                          0x000002A4L

//
// MessageId: CSM_131
//
// MessageText:
//
//  CSM_131: Spheroid `%1$s' doesn't exist in the system
//
#define CSM_131                          0x000002A5L

//
// MessageId: CSM_132
//
// MessageText:
//
//  CSM_132: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_132                          0x000002A6L

//
// MessageId: CSM_133
//
// MessageText:
//
//  CSM_133: Equal latitudes for standard parallels on opposite sidesof the equator
//
#define CSM_133                          0x000002A7L

//
// MessageId: CSM_134
//
// MessageText:
//
//  CSM_134: %1$s %2$s doesn't exist in the system
//
#define CSM_134                          0x000002A8L

//
// MessageId: CSM_135
//
// MessageText:
//
//  CSM_135: Invalid character '%1$c'  in value of %2$s parameter
//
#define CSM_135                          0x000002A9L

//
// MessageId: CSM_136
//
// MessageText:
//
//  CSM_136: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_136                          0x000002AAL

//
// MessageId: CSM_137
//
// MessageText:
//
//  CSM_137: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_137                          0x000002ABL

//
// MessageId: CSM_138
//
// MessageText:
//
//  CSM_138: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_138                          0x000002ACL

//
// MessageId: CSM_139
//
// MessageText:
//
//  CSM_139: Specified value of %1$s parameter (%2$s) is incorrect
//
#define CSM_139                          0x000002ADL

//
// MessageId: CSM_140
//
// MessageText:
//
//  CSM_140: %1$s must be one of '%2$s' or '%3$s' - %4$s is invalid
//
#define CSM_140                          0x000002AEL

//
// MessageId: CSM_141
//
// MessageText:
//
//  CSM_141: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_141                          0x000002AFL

//
// MessageId: CSM_142
//
// MessageText:
//
//  CSM_142: Specified value of %1$s parameter is incorrect - default: %2$s
//
#define CSM_142                          0x000002B0L

//
// MessageId: CSM_143
//
// MessageText:
//
//  CSM_143: Specified value of %1$s parameter is incorrect
//
#define CSM_143                          0x000002B1L

//
// MessageId: CSM_144
//
// MessageText:
//
//  CSM_144: Specified value of %1$s parameter is incorrect
//
#define CSM_144                          0x000002B2L

//
// MessageId: CSM_145
//
// MessageText:
//
//  CSM_145: Specified value of %1$s parameter is incorrect
//
#define CSM_145                          0x000002B3L

//
// MessageId: CSM_146
//
// MessageText:
//
//  CSM_146: Invalid zone specification '%1$s'
//
#define CSM_146                          0x000002B4L

//
// MessageId: CSM_147
//
// MessageText:
//
//  CSM_147: Spheroid %1$s is already in the cache
//
#define CSM_147                          0x000002B5L

//
// MessageId: CSM_148
//
// MessageText:
//
//  CSM_148: Definition of the `%1$s' spheroid doesn't exist
//
#define CSM_148                          0x000002B6L

//
// MessageId: CSM_149
//
// MessageText:
//
//  CSM_149: Major axis of the `%1$s' spheroid doesn't match the system definition
//
#define CSM_149                          0x000002B7L

//
// MessageId: CSM_150
//
// MessageText:
//
//  CSM_150: Minor axis of the `%1$s' spheroid doesn't match the system definition
//
#define CSM_150                          0x000002B8L

//
// MessageId: CSM_151
//
// MessageText:
//
//  CSM_151: %1$s: criterion is not valid for delete spheroid '%2$s'
//
#define CSM_151                          0x000002B9L

//
// MessageId: CSM_152
//
// MessageText:
//
//  CSM_152: csm_sphr_delete: could not delete spheroid '%1$s' from the '%2$s' table
//
#define CSM_152                          0x000002BAL

//
// MessageId: CSM_153
//
// MessageText:
//
//  CSM_153: %1$s: could not find spheroid in cache '%2$s'
//
#define CSM_153                          0x000002BBL

//
// MessageId: CSM_154
//
// MessageText:
//
//  CSM_154: %1$s: could not delete spheroid (%2$s) from cache
//
#define CSM_154                          0x000002BCL

//
// MessageId: CSM_155
//
// MessageText:
//
//  CSM_155: Invalid radius specification: '%1$s'
//
#define CSM_155                          0x000002BDL

//
// MessageId: CSM_156
//
// MessageText:
//
//  CSM_156: Invalid radius specification: '%1$s'
//
#define CSM_156                          0x000002BEL

//
// MessageId: CSM_157
//
// MessageText:
//
//  CSM_157: Invalid unit in spheroid specification: '%1$s'
//
#define CSM_157                          0x000002BFL

//
// MessageId: CSM_158
//
// MessageText:
//
//  CSM_158: %1$s: could not allocate %2$s table cache
//
#define CSM_158                          0x000002C0L

//
// MessageId: CSM_159
//
// MessageText:
//
//  CSM_159: Can't retrieve modified spheroid definitions
//
#define CSM_159                          0x000002C1L

//
// MessageId: CSM_160
//
// MessageText:
//
//  CSM_160: NULL spheroid name
//
#define CSM_160                          0x000002C2L

//
// MessageId: CSM_161
//
// MessageText:
//
//  CSM_161: Invalid semi-major parameter value in  `%1$s'
//
#define CSM_161                          0x000002C3L

//
// MessageId: CSM_162
//
// MessageText:
//
//  CSM_162: Invalid semi-minor parameter value in  `%1$s'
//
#define CSM_162                          0x000002C4L

//
// MessageId: CSM_163
//
// MessageText:
//
//  CSM_163: Null spheroid handle
//
#define CSM_163                          0x000002C5L

//
// MessageId: CSM_164
//
// MessageText:
//
//  CSM_164: Uninitialized spheroid handle
//
#define CSM_164                          0x000002C6L

//
// MessageId: CSM_165
//
// MessageText:
//
//  CSM_165: csm_sphr_validate: bad magic number
//
#define CSM_165                          0x000002C7L

//
// MessageId: CSM_166
//
// MessageText:
//
//  CSM_166: csm_ta_criterion::Can't recognize table <%1$s>
//
#define CSM_166                          0x000002C8L

//
// MessageId: CSM_167
//
// MessageText:
//
//  CSM_167: Internal definition for %1$s table not found
//
#define CSM_167                          0x000002C9L

//
// MessageId: CSM_168
//
// MessageText:
//
//  CSM_168: Can't determine the abbreviation of '%1$s'
//
#define CSM_168                          0x000002CAL

//
// MessageId: CSM_169
//
// MessageText:
//
//  CSM_169: There are no new definitions to store
//
#define CSM_169                          0x000002CBL

//
// MessageId: CSM_170
//
// MessageText:
//
//  CSM_170: Coordinate System Management doesn't recognize the %1$s table
//
#define CSM_170                          0x000002CCL

//
// MessageId: CSM_171
//
// MessageText:
//
//  CSM_171: The <%1$s> table is not used by coordinate system package
//
#define CSM_171                          0x000002CDL

//
// MessageId: CSM_172
//
// MessageText:
//
//  CSM_172: The `%1$s' parameter in the unit definition is too long: `%2$s'
//
#define CSM_172                          0x000002CEL

//
// MessageId: CSM_173
//
// MessageText:
//
//  CSM_173: Can't find the base (%1$s) for definition of `%2$s'
//
#define CSM_173                          0x000002CFL

//
// MessageId: CSM_174
//
// MessageText:
//
//  CSM_174: %1$s: invalid input
//
#define CSM_174                          0x000002D0L

//
// MessageId: CSM_175
//
// MessageText:
//
//  CSM_175: %1$s: '%2$s' unit is not angular
//
#define CSM_175                          0x000002D1L

//
// MessageId: CSM_176
//
// MessageText:
//
//  CSM_176: Invalid angular input: '%1$s'
//
#define CSM_176                          0x000002D2L

//
// MessageId: CSM_177
//
// MessageText:
//
//  CSM_177: Invalid Degree format
//
#define CSM_177                          0x000002D3L

//
// MessageId: CSM_178
//
// MessageText:
//
//  CSM_178: %1$s: Invalid quantity
//
#define CSM_178                          0x000002D4L

//
// MessageId: CSM_179
//
// MessageText:
//
//  CSM_179: Can't convert `%1$s'
//
#define CSM_179                          0x000002D5L

//
// MessageId: CSM_180
//
// MessageText:
//
//  CSM_180: Can't find definition of unit '%1$s'
//
#define CSM_180                          0x000002D6L

//
// MessageId: CSM_181
//
// MessageText:
//
//  CSM_181: (%1$s) is not compatible with (%2$s)
//
#define CSM_181                          0x000002D7L

//
// MessageId: CSM_182
//
// MessageText:
//
//  CSM_182: %1$s: cannot get base unit
//
#define CSM_182                          0x000002D8L

//
// MessageId: CSM_183
//
// MessageText:
//
//  CSM_183: Definition of `%1$s' unit doesn't exist
//
#define CSM_183                          0x000002D9L

//
// MessageId: CSM_184
//
// MessageText:
//
//  CSM_184: Definition of `%1$s' unit doesn't match
//
#define CSM_184                          0x000002DAL

//
// MessageId: CSM_185
//
// MessageText:
//
//  CSM_185: %1$s: cannot get conv_to
//
#define CSM_185                          0x000002DBL

//
// MessageId: CSM_186
//
// MessageText:
//
//  CSM_186: %1$s: cannot get conv_to
//
#define CSM_186                          0x000002DCL

//
// MessageId: CSM_187
//
// MessageText:
//
//  CSM_187: %1$s: cannot destroy handle
//
#define CSM_187                          0x000002DDL

//
// MessageId: CSM_188
//
// MessageText:
//
//  CSM_188: %1$s: invalid handle.
//
#define CSM_188                          0x000002DEL

//
// MessageId: CSM_189
//
// MessageText:
//
//  CSM_189: %1$s: not supported unit type (%2$s)
//
#define CSM_189                          0x000002DFL

//
// MessageId: CSM_190
//
// MessageText:
//
//  CSM_190: Invalid unit specification: '%1$s'
//
#define CSM_190                          0x000002E0L

//
// MessageId: CSM_191
//
// MessageText:
//
//  CSM_191: %1$s: invalid mask for unit type = (%2$d)
//
#define CSM_191                          0x000002E1L

//
// MessageId: CSM_192
//
// MessageText:
//
//  CSM_192: %1$s: floating point exception
//
#define CSM_192                          0x000002E2L

//
// MessageId: CSM_193
//
// MessageText:
//
//  CSM_193: Can't retrieve the new unit definitions
//
#define CSM_193                          0x000002E3L

//
// MessageId: CSM_194
//
// MessageText:
//
//  CSM_194: Can't modify `%1$s', only scale factor may be modified
//
#define CSM_194                          0x000002E4L

//
// MessageId: CSM_195
//
// MessageText:
//
//  CSM_195: `%1$s', base definition of `%2$s', is missing
//
#define CSM_195                          0x000002E5L

//
// MessageId: CSM_196
//
// MessageText:
//
//  CSM_196: `%1$s', base definition of `%2$s', is missing
//
#define CSM_196                          0x000002E6L

//
// MessageId: CSM_197
//
// MessageText:
//
//  CSM_197: NULL unit name
//
#define CSM_197                          0x000002E7L

//
// MessageId: CSM_198
//
// MessageText:
//
//  CSM_198: Invalid type parameter value in  definition of `%1$s'
//
#define CSM_198                          0x000002E8L

//
// MessageId: CSM_199
//
// MessageText:
//
//  CSM_199: Invalid Linear/Angular symbol parameter in `%1$s' definition
//
#define CSM_199                          0x000002E9L

//
// MessageId: CSM_200
//
// MessageText:
//
//  CSM_200: Invalid area symbol parameter in `%1$s' definition
//
#define CSM_200                          0x000002EAL

//
// MessageId: CSM_201
//
// MessageText:
//
//  CSM_201: Invalid volumetric symbol parameter in `%1$s' definition
//
#define CSM_201                          0x000002EBL

//
// MessageId: CSM_202
//
// MessageText:
//
//  CSM_202: Missing conversion factor in %1$s definition
//
#define CSM_202                          0x000002ECL

//
// MessageId: CSM_203
//
// MessageText:
//
//  CSM_203: Invalid conversion factor in %1$s definition
//
#define CSM_203                          0x000002EDL

//
// MessageId: CSM_204
//
// MessageText:
//
//  CSM_204: Missing base symbol name in %1$s definition
//
#define CSM_204                          0x000002EEL

//
// MessageId: CSM_205
//
// MessageText:
//
//  CSM_205: %1$s: cannot set standard
//
#define CSM_205                          0x000002EFL

//
// MessageId: CSM_206
//
// MessageText:
//
//  CSM_206: %1$s: cannot set standard
//
#define CSM_206                          0x000002F0L

//
// MessageId: CSM_207
//
// MessageText:
//
//  CSM_207: Unknown unit type: '%1$s'
//
#define CSM_207                          0x000002F1L

//
// MessageId: CSM_208
//
// MessageText:
//
//  CSM_208: %1$s: cannot get symbol
//
#define CSM_208                          0x000002F2L

//
// MessageId: CSM_209
//
// MessageText:
//
//  CSM_209: %1$s: cannot get type
//
#define CSM_209                          0x000002F3L

//
// MessageId: CSM_210
//
// MessageText:
//
//  CSM_210: %1$s: null unit definition
//
#define CSM_210                          0x000002F4L

//
// MessageId: CSM_211
//
// MessageText:
//
//  CSM_211: %1$s: bad magic number
//
#define CSM_211                          0x000002F5L

//
// MessageId: CSM_212
//
// MessageText:
//
//  CSM_212: csm_ut_prjn_id: Null input
//
#define CSM_212                          0x000002F6L

//
// MessageId: CSM_213
//
// MessageText:
//
//  CSM_213: Failed to open: %1$s
//
#define CSM_213                          0x000002F7L

//
// MessageId: CSM_214
//
// MessageText:
//
//  CSM_214: Unexpected empty file: %1$s
//
#define CSM_214                          0x000002F8L

//
// MessageId: CSM_215
//
// MessageText:
//
//  CSM_215: Search column: [%1$s] not found
//
#define CSM_215                          0x000002F9L

//
// MessageId: CSM_216
//
// MessageText:
//
//  CSM_216: Dependent projections are: 
//
#define CSM_216                          0x000002FAL

//
// MessageId: CSM_217
//
// MessageText:
//
//  CSM_217:  and %1$s
//
#define CSM_217                          0x000002FBL

//
// MessageId: CSU_1
//
// MessageText:
//
//  CSU_1: %1$s: invalid handle supplied
//
#define CSU_1                            0x000002FCL

//
// MessageId: CSU_2
//
// MessageText:
//
//  CSU_2: %1$s: invalid area scale 0.0 in handle
//
#define CSU_2                            0x000002FDL

//
// MessageId: CSU_3
//
// MessageText:
//
//  CSU_3: %1$s: invalid handle supplied
//
#define CSU_3                            0x000002FEL

//
// MessageId: CSU_4
//
// MessageText:
//
//  CSU_4: csu_msr_calc_e2: Can not compute eccentricity squared
//
#define CSU_4                            0x000002FFL

//
// MessageId: CSU_5
//
// MessageText:
//
//  CSU_5: %1$s: invalid handle supplied
//
#define CSU_5                            0x00000300L

//
// MessageId: CSU_6
//
// MessageText:
//
//  CSU_6: %1$s: invalid handle supplied
//
#define CSU_6                            0x00000301L

//
// MessageId: CSU_7
//
// MessageText:
//
//  CSU_7: %1$s: invalid handle supplied
//
#define CSU_7                            0x00000302L

//
// MessageId: CSU_8
//
// MessageText:
//
//  CSU_8: %1$s: invalid handle supplied
//
#define CSU_8                            0x00000303L

//
// MessageId: CSU_9
//
// MessageText:
//
//  CSU_9: %1$s: invalid handle supplied
//
#define CSU_9                            0x00000304L

//
// MessageId: CSU_10
//
// MessageText:
//
//  CSU_10: %1$s: mixed coordinate systems is not supported
//
#define CSU_10                           0x00000305L

//
// MessageId: CSU_11
//
// MessageText:
//
//  CSU_11: %1$s: invalid handle supplied
//
#define CSU_11                           0x00000306L

//
// MessageId: CSU_12
//
// MessageText:
//
//  CSU_12: %1$s: invalid handle supplied
//
#define CSU_12                           0x00000307L

//
// MessageId: CSU_13
//
// MessageText:
//
//  CSU_13: %1$s: invalid length scale 0.0 in handle
//
#define CSU_13                           0x00000308L

//
// MessageId: CSU_14
//
// MessageText:
//
//  CSU_14: %1$s: invalid handle supplied
//
#define CSU_14                           0x00000309L

//
// MessageId: CSU_15
//
// MessageText:
//
//  CSU_15: %1$s: can't match area units with linear or angular
//
#define CSU_15                           0x0000030AL

//
// MessageId: CSU_16
//
// MessageText:
//
//  CSU_16: %1$s: don't know how to match incompatible types
//
#define CSU_16                           0x0000030BL

//
// MessageId: CSU_17
//
// MessageText:
//
//  CSU_17: %1$s: can't convert from meters to %2$s
//
#define CSU_17                           0x0000030CL

//
// MessageId: CSU_18
//
// MessageText:
//
//  CSU_18: %1$s: can't convert from %2$s to radians
//
#define CSU_18                           0x0000030DL

//
// MessageId: CSU_19
//
// MessageText:
//
//  CSU_19: %1$s: can't convert from radians to %2$s
//
#define CSU_19                           0x0000030EL

//
// MessageId: CSU_20
//
// MessageText:
//
//  CSU_20: %1$s: can't convert from %2$s to meters
//
#define CSU_20                           0x0000030FL

//
// MessageId: CSU_21
//
// MessageText:
//
//  CSU_21: %1$s: invalid handle supplied
//
#define CSU_21                           0x00000310L

//
// MessageId: CSU_22
//
// MessageText:
//
//  CSU_22: %1$s: can't get the definition of %2$s
//
#define CSU_22                           0x00000311L

//
// MessageId: CSU_23
//
// MessageText:
//
//  CSU_23: %1$s: can't get the definition of %2$s
//
#define CSU_23                           0x00000312L

//
// MessageId: CSU_24
//
// MessageText:
//
//  CSU_24: %1$s: can't find transformation function for Lat/Long to '%2$s'.
//
#define CSU_24                           0x00000313L

//
// MessageId: CSU_25
//
// MessageText:
//
//  CSU_25: %1$s: initialization of projection '%2$s' failed.
//
#define CSU_25                           0x00000314L

//
// MessageId: CSU_26
//
// MessageText:
//
//  CSU_26: Can not transform coordinate (Latitude: %1$s rad, Longitude: %2$s rad)
//
#define CSU_26                           0x00000315L

//
// MessageId: CSU_27
//
// MessageText:
//
//  CSU_27: Can not transform coordinate (Latitude: %1$s rad, Longitude: %2$s rad)
//
#define CSU_27                           0x00000316L

//
// MessageId: CSU_28
//
// MessageText:
//
//  CSU_28: Transformations from '%1$s' to '%2$s' are not possible.
//
#define CSU_28                           0x00000317L

//
// MessageId: CSU_29
//
// MessageText:
//
//  CSU_29: Invalid spheroid for FROM projection '%1$s'.
//
#define CSU_29                           0x00000318L

//
// MessageId: CSU_30
//
// MessageText:
//
//  CSU_30: Invalid spheroid for TO projection '%1$s'.
//
#define CSU_30                           0x00000319L

//
// MessageId: CSU_31
//
// MessageText:
//
//  CSU_31: Spheroids for '%1$s' and '%2$s' projections are not equal.
//
#define CSU_31                           0x0000031AL

//
// MessageId: CSU_32
//
// MessageText:
//
//  CSU_32: Initialization of FROM projection '%1$s' failed.
//
#define CSU_32                           0x0000031BL

//
// MessageId: CSU_33
//
// MessageText:
//
//  CSU_33: Initialization of TO projection '%1$s' failed.
//
#define CSU_33                           0x0000031CL

//
// MessageId: CSU_34
//
// MessageText:
//
//  CSU_34: Projections '%1$s' and '%2$s' do not overlap.
//
#define CSU_34                           0x0000031DL

//
// MessageId: CSU_35
//
// MessageText:
//
//  CSU_35: csu_ctr_dst_handle: invalid handle.
//
#define CSU_35                           0x0000031EL

//
// MessageId: CSU_36
//
// MessageText:
//
//  CSU_36: csu_ctr_transform: invalid handle.
//
#define CSU_36                           0x0000031FL

//
// MessageId: CSU_37
//
// MessageText:
//
//  CSU_37: csu_ctr_transform: invalid handle.
//
#define CSU_37                           0x00000320L

//
// MessageId: CSU_38
//
// MessageText:
//
//  CSU_38: csu_ctr_transform - transformation not allowed for given handle ('%1$s' to '%2$s').
//
#define CSU_38                           0x00000321L

//
// MessageId: CSU_39
//
// MessageText:
//
//  CSU_39: %1$s: null unit definition.
//
#define CSU_39                           0x00000322L

//
// MessageId: CSU_40
//
// MessageText:
//
//  CSU_40: %1$s: null private definition.
//
#define CSU_40                           0x00000323L

//
// MessageId: CSU_41
//
// MessageText:
//
//  CSU_41: %1$s: bad magic number.
//
#define CSU_41                           0x00000324L

//
// MessageId: CSU_42
//
// MessageText:
//
//  CSU_42: Erroring installing Albers Conic - equal latitudes for st. parallels on opposite sides of equator.
//
#define CSU_42                           0x00000325L

//
// MessageId: CSU_43
//
// MessageText:
//
//  CSU_43: Error installing Lambert Conformal - equal latitudes for st. parallels on opposite sides of equator.
//
#define CSU_43                           0x00000326L

//
// MessageId: CSU_44
//
// MessageText:
//
//  CSU_44: csu_ctr_ins_nzmg - unknown NZMG Datum (%1$d).
//
#define CSU_44                           0x00000327L

//
// MessageId: CSU_45
//
// MessageText:
//
//  CSU_45: Error installing Obique Mercator - invalid centre line parameters.
//
#define CSU_45                           0x00000328L

//
// MessageId: CSU_46
//
// MessageText:
//
//  CSU_46: Error installing Obique Mercator - invalid centre line parameters.
//
#define CSU_46                           0x00000329L

//
// MessageId: CSU_47
//
// MessageText:
//
//  CSU_47: Error installing Obique Mercator - invalid latitude of origin.
//
#define CSU_47                           0x0000032AL

//
// MessageId: CSU_48
//
// MessageText:
//
//  CSU_48: Error installing Simple Equidistant Conic - equal latitudes for st. parallels on opposite sides of equator.
//
#define CSU_48                           0x0000032BL

//
// MessageId: CSU_49
//
// MessageText:
//
//  CSU_49: csu_ctr_trs_ae_ll - improper input easting and northing.
//
#define CSU_49                           0x0000032CL

//
// MessageId: CSU_50
//
// MessageText:
//
//  CSU_50: csu_ctr_trs_er_ll - improper input easting and northing.
//
#define CSU_50                           0x0000032DL

//
// MessageId: CSU_51
//
// MessageText:
//
//  CSU_51:  csu_ctr_trs_gv_ll - improper input easting and northing.
//
#define CSU_51                           0x0000032EL

//
// MessageId: CSU_52
//
// MessageText:
//
//  CSU_52: csu_ctr_trs_la_ll - improper input easting and northing.
//
#define CSU_52                           0x0000032FL

//
// MessageId: CSU_53
//
// MessageText:
//
//  CSU_53: csu_ctr_trs_ll_ae - point projects into a circle.
//
#define CSU_53                           0x00000330L

//
// MessageId: CSU_54
//
// MessageText:
//
//  CSU_54: csu_ctr_trs_ll_gn - point projects into infinity.
//
#define CSU_54                           0x00000331L

//
// MessageId: CSU_55
//
// MessageText:
//
//  CSU_55: csu_ctr_trs_ll_gv - point cannot be projected.
//
#define CSU_55                           0x00000332L

//
// MessageId: CSU_56
//
// MessageText:
//
//  CSU_56: csu_ctr_trs_ll_la - point projects into a circle.
//
#define CSU_56                           0x00000333L

//
// MessageId: CSU_57
//
// MessageText:
//
//  CSU_57: csu_ctr_trs_ll_lc - point cannot be projected.
//
#define CSU_57                           0x00000334L

//
// MessageId: CSU_58
//
// MessageText:
//
//  CSU_58: csu_ctr_trs_ll_merc - transformation cannot be computed at the poles.
//
#define CSU_58                           0x00000335L

//
// MessageId: CSU_59
//
// MessageText:
//
//  CSU_59: csu_ctr_trs_ll_om - point projects into infinity.
//
#define CSU_59                           0x00000336L

//
// MessageId: CSU_60
//
// MessageText:
//
//  CSU_60: csu_ctr_trs_ll_or - point cannot be projected.
//
#define CSU_60                           0x00000337L

//
// MessageId: CSU_61
//
// MessageText:
//
//  CSU_61: csu_ctr_trs_ll_st - point projects into infinity.
//
#define CSU_61                           0x00000338L

//
// MessageId: CSU_62
//
// MessageText:
//
//  CSU_62: csu_ctr_trs_ll_tm - point projects into infinity.
//
#define CSU_62                           0x00000339L

//
// MessageId: CSU_63
//
// MessageText:
//
//  CSU_63: %1$s: Point projects into infinity
//
#define CSU_63                           0x0000033AL

//
// MessageId: CSU_64
//
// MessageText:
//
//  CSU_64: %1$s: Internal arithmetic error during conversion from lat/long to tm coordinates
//
#define CSU_64                           0x0000033BL

//
// MessageId: CSU_67
//
// MessageText:
//
//  CSU_67: csu_ctr_trs_or_ll - improper input easting and northing.
//
#define CSU_67                           0x0000033CL

//
// MessageId: CSU_68
//
// MessageText:
//
//  CSU_68: csu_ctr_trs_si_ll - improper input easting and northing.
//
#define CSU_68                           0x0000033DL

//
// MessageId: CSU_69
//
// MessageText:
//
//  CSU_69: Divide by zero error in Transverse Mercator to Lat/Long transformation
//
#define CSU_69                           0x0000033EL

//
// MessageId: CSU_70
//
// MessageText:
//
//  CSU_70: %1$s: %2$s
//
#define CSU_70                           0x0000033FL

//
// MessageId: CSU_75
//
// MessageText:
//
//  CSU_75: csu_ctr_trs_vg_ll - improper input easting and northing.
//
#define CSU_75                           0x00000340L

//
// MessageId: CSU_76
//
// MessageText:
//
//  CSU_76: csu_ctr_trs_vg_ll - latitude failed to converge.
//
#define CSU_76                           0x00000341L

//
// MessageId: CSU_77
//
// MessageText:
//
//  CSU_77: csu_ctr_adj_long - failed to adjust longitude
//
#define CSU_77                           0x00000342L

//
// MessageId: CSU_78
//
// MessageText:
//
//  CSU_78: csu_ctr_adj_long - failed to adjust longitude
//
#define CSU_78                           0x00000343L

//
// MessageId: CSU_79
//
// MessageText:
//
//  CSU_79: csu_ctr_ut_cmpt_es - error computing eccentricity squared! Major axis is 0.
//
#define CSU_79                           0x00000344L

//
// MessageId: CSU_80
//
// MessageText:
//
//  CSU_80: csu_ctr_ut_cmpt_inves - error computing inverse eccentricity; Minor axis is 0.
//
#define CSU_80                           0x00000345L

//
// MessageId: CSU_81
//
// MessageText:
//
//  CSU_81: csu_ctr_ut_cmpt_phi1 - latitude failed to converge.
//
#define CSU_81                           0x00000346L

//
// MessageId: CSU_82
//
// MessageText:
//
//  CSU_82: csu_ctr_ut_cmpt_phi2 - latitude failed to converge.
//
#define CSU_82                           0x00000347L

//
// MessageId: CSU_83
//
// MessageText:
//
//  CSU_83: csu_ctr_ut_cmpt_phi3 - latitude failed to converge.
//
#define CSU_83                           0x00000348L

//
// MessageId: CSU_84
//
// MessageText:
//
//  CSU_84: csu_ctr_ut_cmpt_rad - Invalid major axis.
//
#define CSU_84                           0x00000349L

//
// MessageId: CSU_85
//
// MessageText:
//
//  CSU_85: csu_ctr_ut_cmpt_rad - Invalid minor axis.
//
#define CSU_85                           0x0000034AL

//
// MessageId: CSU_86
//
// MessageText:
//
//  CSU_86: csu_ctr_ut_ft_pt_latd - latitude failed to converge.
//
#define CSU_86                           0x0000034BL

//
// MessageId: CSYSUT_1
//
// MessageText:
//
//  CSYSUT_1: Too many signs in %1$s
//
#define CSYSUT_1                         0x0000034CL

//
// MessageId: DBGQL_1
//
// MessageText:
//
//  DBGQL_1: `name' is a mandatory parameter, NULL or empty string found.
//
#define DBGQL_1                          0x0000034DL

//
// MessageId: DBGQL_2
//
// MessageText:
//
//  DBGQL_2: size for bind name `%1$s' not resolved, cannot bind input variable.
//
#define DBGQL_2                          0x0000034EL

//
// MessageId: DBGQL_3
//
// MessageText:
//
//  DBGQL_3: `address' is a mandatory parameter, NULL pointer found.
//
#define DBGQL_3                          0x0000034FL

//
// MessageId: DBGQL_4
//
// MessageText:
//
//  DBGQL_4: Bind column name `%1$s' not found, cannot set data type and size for this column.
//
#define DBGQL_4                          0x00000350L

//
// MessageId: DBGQL_5
//
// MessageText:
//
//  DBGQL_5: Execute/set_val_a done, cannot bind name `%1$s'.
//
#define DBGQL_5                          0x00000351L

//
// MessageId: DBGQL_6
//
// MessageText:
//
//  DBGQL_6: Bind variable cursor not found, cannot continue.
//
#define DBGQL_6                          0x00000352L

//
// MessageId: DBGQL_7
//
// MessageText:
//
//  DBGQL_7: `name' is a mandatory parameter, NULL or empty string found.
//
#define DBGQL_7                          0x00000353L

//
// MessageId: DBGQL_8
//
// MessageText:
//
//  DBGQL_8: size for define name `%1$s' not resolved, cannot define output variable.
//
#define DBGQL_8                          0x00000354L

//
// MessageId: DBGQL_9
//
// MessageText:
//
//  DBGQL_9: `address' is a mandatory parameter for column `%1$s', NULL pointer found.
//
#define DBGQL_9                          0x00000355L

//
// MessageId: DBGQL_10
//
// MessageText:
//
//  DBGQL_10: Select column name `%1$s' not found, cannot define datatype and size for this column.
//
#define DBGQL_10                         0x00000356L

//
// MessageId: DBGQL_11
//
// MessageText:
//
//  DBGQL_11: Cannot locate information for a DBI_RESERVED_FEAT_INFO, cannot select `feat_info'
//
#define DBGQL_11                         0x00000357L

//
// MessageId: DBGQL_12
//
// MessageText:
//
//  DBGQL_12: Allocate of `define' data structure failed, cannot define `feat_info' column.
//
#define DBGQL_12                         0x00000358L

//
// MessageId: DBGQL_13
//
// MessageText:
//
//  DBGQL_13: cannot get define position %1$d, NULL address returned
//
#define DBGQL_13                         0x00000359L

//
// MessageId: DBGQL_14
//
// MessageText:
//
//  DBGQL_14: internal error, parameter `sql' is NULL or an empty string, cannot continue exec_coc.
//
#define DBGQL_14                         0x0000035AL

//
// MessageId: DBGQL_15
//
// MessageText:
//
//  DBGQL_15: Not a `select ... for update ...' statement, invalid current of cursor operation.
//
#define DBGQL_15                         0x0000035BL

//
// MessageId: DBGQL_16
//
// MessageText:
//
//  DBGQL_16: Row fetching > 1, invalid current of cursor operation.
//
#define DBGQL_16                         0x0000035CL

//
// MessageId: DBGQL_17
//
// MessageText:
//
//  DBGQL_17: More than one object in the `from' clause, invalid current of cursor operation.
//
#define DBGQL_17                         0x0000035DL

//
// MessageId: DBGQL_18
//
// MessageText:
//
//  DBGQL_18: Current of cursor operation failed, row not changed.
//
#define DBGQL_18                         0x0000035EL

//
// MessageId: DBGQL_19
//
// MessageText:
//
//  DBGQL_19: Issuing rollback to recover from transaction failure
//
#define DBGQL_19                         0x0000035FL

//
// MessageId: DBGQL_20
//
// MessageText:
//
//  DBGQL_20: Rollback complete
//
#define DBGQL_20                         0x00000360L

//
// MessageId: DBGQL_21
//
// MessageText:
//
//  DBGQL_21: Current of cursor operation failed, row not changed.
//
#define DBGQL_21                         0x00000361L

//
// MessageId: DBGQL_22
//
// MessageText:
//
//  DBGQL_22: Current of cursor operation failed, row not changed.
//
#define DBGQL_22                         0x00000362L

//
// MessageId: DBGQL_23
//
// MessageText:
//
//  DBGQL_23: Failed to convert lock conflict constant %1$d.
//
#define DBGQL_23                         0x00000363L

//
// MessageId: DBGQL_24
//
// MessageText:
//
//  DBGQL_24: Failed to locate bind variable %1$s, cannot continue.
//
#define DBGQL_24                         0x00000364L

//
// MessageId: DBGQL_25
//
// MessageText:
//
//  DBGQL_25: Value for bind location `%1$s' not provided, cannot continue execute.
//
#define DBGQL_25                         0x00000365L

//
// MessageId: DBGQL_26
//
// MessageText:
//
//  DBGQL_26: Invalid feature number specification. '%1$s', restricted to numeric values, cannot continue parse.
//
#define DBGQL_26                         0x00000366L

//
// MessageId: DBGQL_27
//
// MessageText:
//
//  DBGQL_27: Unsupported feature number conversion from DBI_T_ data type of %1$d to data type of DBI_T_LONG.
//
#define DBGQL_27                         0x00000367L

//
// MessageId: DBGQL_28
//
// MessageText:
//
//  DBGQL_28: Failed to establish active version, cannot continue.
//
#define DBGQL_28                         0x00000368L

//
// MessageId: DBGQL_29
//
// MessageText:
//
//  DBGQL_29: Issuing rollback to recover from transaction failure
//
#define DBGQL_29                         0x00000369L

//
// MessageId: DBGQL_30
//
// MessageText:
//
//  DBGQL_30: Rollback complete
//
#define DBGQL_30                         0x0000036AL

//
// MessageId: DBGQL_31
//
// MessageText:
//
//  DBGQL_31: Internal input storage not allocated for bind location %1$s, cannot continue versioned insert.
//
#define DBGQL_31                         0x0000036BL

//
// MessageId: DBGQL_32
//
// MessageText:
//
//  DBGQL_32: internal error, feat_num unavailable in bind data structure, cannot continue execute.
//
#define DBGQL_32                         0x0000036CL

//
// MessageId: DBGQL_33
//
// MessageText:
//
//  DBGQL_33: %1$s
//
#define DBGQL_33                         0x0000036DL

//
// MessageId: DBGQL_34
//
// MessageText:
//
//  DBGQL_34: In gid %1$d: no gid's are defined, %2$s.
//
#define DBGQL_34                         0x0000036EL

//
// MessageId: DBGQL_35
//
// MessageText:
//
//  DBGQL_35: In gid %1$d: cannot get number of current gid's, %2$s.
//
#define DBGQL_35                         0x0000036FL

//
// MessageId: DBGQL_36
//
// MessageText:
//
//  DBGQL_36: In gid %1$d: not found, %2$s.
//
#define DBGQL_36                         0x00000370L

//
// MessageId: DBGQL_37
//
// MessageText:
//
//  DBGQL_37: In gid %1$d: null address returned, %2$s.
//
#define DBGQL_37                         0x00000371L

//
// MessageId: DBGQL_38
//
// MessageText:
//
//  DBGQL_38: In gid %1$d: previously deleted, %2$s.
//
#define DBGQL_38                         0x00000372L

//
// MessageId: DBGQL_39
//
// MessageText:
//
//  DBGQL_39: unable to allocate gid array, NULL pointer returned.
//
#define DBGQL_39                         0x00000373L

//
// MessageId: DBGQL_40
//
// MessageText:
//
//  DBGQL_40: unable to allocate parse array, NULL pointer returned.
//
#define DBGQL_40                         0x00000374L

//
// MessageId: DBGQL_41
//
// MessageText:
//
//  DBGQL_41: unable to allocate gid define array, NULL pointer returned.
//
#define DBGQL_41                         0x00000375L

//
// MessageId: DBGQL_42
//
// MessageText:
//
//  DBGQL_42: unable to allocate gid bind array, NULL pointer returned.
//
#define DBGQL_42                         0x00000376L

//
// MessageId: DBGQL_43
//
// MessageText:
//
//  DBGQL_43: unable to allocate gid cm flush table array, NULL pointer returned.
//
#define DBGQL_43                         0x00000377L

//
// MessageId: DBGQL_44
//
// MessageText:
//
//  DBGQL_44: Append to bind array failed, NULL address returned.
//
#define DBGQL_44                         0x00000378L

//
// MessageId: DBGQL_45
//
// MessageText:
//
//  DBGQL_45: Get from bind array position %1$d failed, NULL address returned.
//
#define DBGQL_45                         0x00000379L

//
// MessageId: DBGQL_46
//
// MessageText:
//
//  DBGQL_46: Append to define array failed, NULL address returned.
//
#define DBGQL_46                         0x0000037AL

//
// MessageId: DBGQL_47
//
// MessageText:
//
//  DBGQL_47: Get from define array position %1$d failed, NULL address returned.
//
#define DBGQL_47                         0x0000037BL

//
// MessageId: DBGQL_48
//
// MessageText:
//
//  DBGQL_48: Get from gid array position %1$d failed, NULL address returned.
//
#define DBGQL_48                         0x0000037CL

//
// MessageId: DBGQL_49
//
// MessageText:
//
//  DBGQL_49: Append to gid array failed, NULL address returned.
//
#define DBGQL_49                         0x0000037DL

//
// MessageId: DBGQL_50
//
// MessageText:
//
//  DBGQL_50: Get from parse linear array position %1$d failed, NULL address returned.
//
#define DBGQL_50                         0x0000037EL

//
// MessageId: DBGQL_51
//
// MessageText:
//
//  DBGQL_51: Append to parse linear array failed, NULL address returned.
//
#define DBGQL_51                         0x0000037FL

//
// MessageId: DBGQL_52
//
// MessageText:
//
//  DBGQL_52: Get from cm flush table array position %1$d failed, NULL address returned.
//
#define DBGQL_52                         0x00000380L

//
// MessageId: DBGQL_53
//
// MessageText:
//
//  DBGQL_53: Append to cm flush table array failed, NULL address returned.
//
#define DBGQL_53                         0x00000381L

//
// MessageId: DBGQL_54
//
// MessageText:
//
//  DBGQL_54: More than one `for ...' lock clause found in GQL statement, cannot continue parse.
//
#define DBGQL_54                         0x00000382L

//
// MessageId: DBGQL_55
//
// MessageText:
//
//  DBGQL_55: Cannot `select ... for draw', not a feature query, cannot continue parse.
//
#define DBGQL_55                         0x00000383L

//
// MessageId: DBGQL_56
//
// MessageText:
//
//  DBGQL_56: Cannot `select ... for draw', unable to establish whether a feature query, cannot continue parse.
//
#define DBGQL_56                         0x00000384L

//
// MessageId: DBGQL_57
//
// MessageText:
//
//  DBGQL_57: Cannot `select ... for draw', not a feature query, cannot continue parse.
//
#define DBGQL_57                         0x00000385L

//
// MessageId: DBGQL_58
//
// MessageText:
//
//  DBGQL_58: More than one table in `from' clause of `for update', cannot continue parse.
//
#define DBGQL_58                         0x00000386L

//
// MessageId: DBGQL_59
//
// MessageText:
//
//  DBGQL_59: Failed to determine SQL statement table name, cannot continue parse.
//
#define DBGQL_59                         0x00000387L

//
// MessageId: DBGQL_60
//
// MessageText:
//
//  DBGQL_60: `select ... from %1$s for update' table contains columns that are allowed to be updated, cannot continue parse.
//
#define DBGQL_60                         0x00000388L

//
// MessageId: DBGQL_61
//
// MessageText:
//
//  DBGQL_61: `select ... for lock' valid for one table in the from clause, %1$d found, cannot continue parse.
//
#define DBGQL_61                         0x00000389L

//
// MessageId: DBGQL_62
//
// MessageText:
//
//  DBGQL_62: `select ... for lock' clause invalid for specified table, no %1$s column, cannot continue parse. 
//
#define DBGQL_62                         0x0000038AL

//
// MessageId: DBGQL_63
//
// MessageText:
//
//  DBGQL_63: `select ... for lock' clause invalid for specified table, no %1$s column, cannot continue parse. 
//
#define DBGQL_63                         0x0000038BL

//
// MessageId: DBGQL_64
//
// MessageText:
//
//  DBGQL_64: Group function not valid on `select ... for *lock*' statements, cannot continue parse.
//
#define DBGQL_64                         0x0000038CL

//
// MessageId: DBGQL_65
//
// MessageText:
//
//  DBGQL_65: `select ... for lock', no active lock set, cannot continue parse.
//
#define DBGQL_65                         0x0000038DL

//
// MessageId: DBGQL_66
//
// MessageText:
//
//  DBGQL_66: Table lookup of DBI_RESERVED_FEAT_INFO type not found, cannot continue parse.
//
#define DBGQL_66                         0x0000038EL

//
// MessageId: DBGQL_67
//
// MessageText:
//
//  DBGQL_67: Define column %1$s not allocated, cannot continue parse.
//
#define DBGQL_67                         0x0000038FL

//
// MessageId: DBGQL_68
//
// MessageText:
//
//  DBGQL_68: Invalid current of cursor statement, must be `update' or `delete', cannot continue parse.
//
#define DBGQL_68                         0x00000390L

//
// MessageId: DBGQL_69
//
// MessageText:
//
//  DBGQL_69: Unable to allocate parse array, NULL pointer returned.
//
#define DBGQL_69                         0x00000391L

//
// MessageId: DBGQL_70
//
// MessageText:
//
//  DBGQL_70: size for define name `%1$s' not resolved.
//
#define DBGQL_70                         0x00000392L

//
// MessageId: DBGQL_71
//
// MessageText:
//
//  DBGQL_71: select ... from %1$s ... for update ..., 
//
#define DBGQL_71                         0x00000393L

//
// MessageId: DBGQL_72
//
// MessageText:
//
//  DBGQL_72: insert into %1$s ..., 
//
#define DBGQL_72                         0x00000394L

//
// MessageId: DBGQL_73
//
// MessageText:
//
//  DBGQL_73: delete from %1$s ..., 
//
#define DBGQL_73                         0x00000395L

//
// MessageId: DBGQL_74
//
// MessageText:
//
//  DBGQL_74: update %1$s set ..., 
//
#define DBGQL_74                         0x00000396L

//
// MessageId: DBGQL_75
//
// MessageText:
//
//  DBGQL_75: GQL statement object %1$s ..., 
//
#define DBGQL_75                         0x00000397L

//
// MessageId: DBGQL_76
//
// MessageText:
//
//  DBGQL_76: root 
//
#define DBGQL_76                         0x00000398L

//
// MessageId: DBGQL_77
//
// MessageText:
//
//  DBGQL_77: is not updateable
//
#define DBGQL_77                         0x00000399L

//
// MessageId: DBGQL_78
//
// MessageText:
//
//  DBGQL_78: , system maintained
//
#define DBGQL_78                         0x0000039AL

//
// MessageId: DBGQL_79
//
// MessageText:
//
//  DBGQL_79: , hidden primary key 
//
#define DBGQL_79                         0x0000039BL

//
// MessageId: DBGQL_80
//
// MessageText:
//
//  DBGQL_80: , hidden row lock 
//
#define DBGQL_80                         0x0000039CL

//
// MessageId: DBGQL_81
//
// MessageText:
//
//  DBGQL_81: , cannot continue parse.
//
#define DBGQL_81                         0x0000039DL

//
// MessageId: DBGQL_82
//
// MessageText:
//
//  DBGQL_82: Group function not valid for this feature based query, cannot continue parse
//
#define DBGQL_82                         0x0000039EL

//
// MessageId: DBGQL_83
//
// MessageText:
//
//  DBGQL_83: Table name not resolved, cannot continue parse.
//
#define DBGQL_83                         0x0000039FL

//
// MessageId: DBGQL_84
//
// MessageText:
//
//  DBGQL_84: GQL within clause is invalid for no_active_plan, cannot continue parse.
//
#define DBGQL_84                         0x000003A0L

//
// MessageId: DBGQL_85
//
// MessageText:
//
//  DBGQL_85: GQL spatial column in select column list is invalid for no_active_plan, cannot continue parse.
//
#define DBGQL_85                         0x000003A1L

//
// MessageId: DBGQL_86
//
// MessageText:
//
//  DBGQL_86: RDBMS: GQL syntax error at line %1$d, near '%2$.40s'
//
#define DBGQL_86                         0x000003A2L

//
// MessageId: DBGQL_87
//
// MessageText:
//
//  DBGQL_87: %1$s
//
#define DBGQL_87                         0x000003A3L

//
// MessageId: DBGQL_88
//
// MessageText:
//
//  DBGQL_88: `select ... for lock' on a table with no row lock columns, cannot continue parse.
//
#define DBGQL_88                         0x000003A4L

//
// MessageId: DBGQL_89
//
// MessageText:
//
//  DBGQL_89: Table lookup of DBI_RESERVED_FEAT_INFO type not found, cannot continue parse.
//
#define DBGQL_89                         0x000003A5L

//
// MessageId: DBGQL_90
//
// MessageText:
//
//  DBGQL_90: Define column %1$s not allocated, cannot continue parse.
//
#define DBGQL_90                         0x000003A6L

//
// MessageId: DBGQL_91
//
// MessageText:
//
//  DBGQL_91: Group function not valid for this feature based query, cannot continue parse.
//
#define DBGQL_91                         0x000003A7L

//
// MessageId: DBGQL_92
//
// MessageText:
//
//  DBGQL_92: Order of feature code '%1$s' -> '%2$s' is not ascending
//
#define DBGQL_92                         0x000003A8L

//
// MessageId: DBGQL_93
//
// MessageText:
//
//  DBGQL_93: Feature code used by another table or overlaps another range - Invalid feature code range.
//
#define DBGQL_93                         0x000003A9L

//
// MessageId: DBGQL_94
//
// MessageText:
//
//  DBGQL_94: Could not enter the primary table name into the feat_code table.
//
#define DBGQL_94                         0x000003AAL

//
// MessageId: DBGQL_95
//
// MessageText:
//
//  DBGQL_95: Feature code range not inserted into the memory resident schema
//
#define DBGQL_95                         0x000003ABL

//
// MessageId: DBGQL_96
//
// MessageText:
//
//  DBGQL_96: Invalid value '%1$s' for VISION_WITHIN_METHOD; using default of '0'
//
#define DBGQL_96                         0x000003ACL

//
// MessageId: DBGQL_97
//
// MessageText:
//
//  DBGQL_97: Illegal clip option %1$d specified
//
#define DBGQL_97                         0x000003ADL

//
// MessageId: DBGQL_98
//
// MessageText:
//
//  DBGQL_98: Invalid value '%1$s' for VISION_WITHIN_QT_DEPTH; using default of '%2$d'
//
#define DBGQL_98                         0x000003AEL

//
// MessageId: DBGQL_99
//
// MessageText:
//
//  DBGQL_99: Invalid value '%1$s' for VISION_WITHIN_MAX_RANGE_LIST_LENGTH; using default of '%2$d'
//
#define DBGQL_99                         0x000003AFL

//
// MessageId: DBGQL_100
//
// MessageText:
//
//  DBGQL_100: Coordinate system units not the same, ratio <= 0.0, cannot continue parse.
//
#define DBGQL_100                        0x000003B0L

//
// MessageId: DBGQL_101
//
// MessageText:
//
//  DBGQL_101: Nonexistent feature number %1$d in 'within' clause
//
#define DBGQL_101                        0x000003B1L

//
// MessageId: DBGQL_102
//
// MessageText:
//
//  DBGQL_102: Feature number %1$d in 'within' clause is not a polygon
//
#define DBGQL_102                        0x000003B2L

//
// MessageId: DBGQL_103
//
// MessageText:
//
//  DBGQL_103: Feature number %1$d in 'within' clause is not a complete polygon
//
#define DBGQL_103                        0x000003B3L

//
// MessageId: DBGQL_104
//
// MessageText:
//
//  DBGQL_104: Coordinate system units not the same, ratio <= 0.0, cannot continue parse.
//
#define DBGQL_104                        0x000003B4L

//
// MessageId: DBGQL_105
//
// MessageText:
//
//  DBGQL_105: No range list formulated; translate 'within' clause to SQL
//
#define DBGQL_105                        0x000003B5L

//
// MessageId: DBGQL_106
//
// MessageText:
//
//  DBGQL_106: Invalid feature number specification. '%1$s', restricted to numeric values, cannot continue parse.
//
#define DBGQL_106                        0x000003B6L

//
// MessageId: DBGQL_107
//
// MessageText:
//
//  DBGQL_107: No `within' clause in parse tree, failed to replace, cannot continue parse.
//
#define DBGQL_107                        0x000003B7L

//
// MessageId: DBGQL_108
//
// MessageText:
//
//  DBGQL_108: %1$s%2$sobject %3$s %4$s%5$s%6$s%7$s%8$s%9$s%10$s
//
#define DBGQL_108                        0x000003B8L

//
// MessageId: DBGQL_109
//
// MessageText:
//
//  DBGQL_109: Internal error, cannot establish where to insert column `%1$s' into the SQL statement, cannot continue parse.
//
#define DBGQL_109                        0x000003B9L

//
// MessageId: DBGQL_110
//
// MessageText:
//
//  DBGQL_110: Table lookup of DBI_RESERVED_* type %1$d not found, cannot continue parse.
//
#define DBGQL_110                        0x000003BAL

//
// MessageId: DBGQL_111
//
// MessageText:
//
//  DBGQL_111: Define column %1$s not allocated, cannot continue parse.
//
#define DBGQL_111                        0x000003BBL

//
// MessageId: DBGQL_112
//
// MessageText:
//
//  DBGQL_112: Define column not allocated, cannot continue parse.
//
#define DBGQL_112                        0x000003BCL

//
// MessageId: DBGQL_113
//
// MessageText:
//
//  DBGQL_113: Object `%1$s%2$s%3$s' does not exist in the RDBMS, cannot continue parse.
//
#define DBGQL_113                        0x000003BDL

//
// MessageId: DBGQL_114
//
// MessageText:
//
//  DBGQL_114: Update statement table not found, cannot continue parse.
//
#define DBGQL_114                        0x000003BEL

//
// MessageId: DBGQL_115
//
// MessageText:
//
//  DBGQL_115: Cannot locate delete statement in parse tree, cannot continue parse.
//
#define DBGQL_115                        0x000003BFL

//
// MessageId: DBGQL_116
//
// MessageText:
//
//  DBGQL_116: Unknown edit operation type %1$d specified, cannot continue parse.
//
#define DBGQL_116                        0x000003C0L

//
// MessageId: DBGQL_117
//
// MessageText:
//
//  DBGQL_117: Table '%1$s' not found, cannot continue parse.
//
#define DBGQL_117                        0x000003C1L

//
// MessageId: DBGQL_118
//
// MessageText:
//
//  DBGQL_118: `insert into ... select ...' to a primary table, version or row lock based table `%1$s' is unsupported, cannot continue parse.
//
#define DBGQL_118                        0x000003C2L

//
// MessageId: DBGQL_119
//
// MessageText:
//
//  DBGQL_119: Column %1$s is a reserved system column, cannot insert column value, cannot continue parse.
//
#define DBGQL_119                        0x000003C3L

//
// MessageId: DBGQL_120
//
// MessageText:
//
//  DBGQL_120: Column %1$s is a reserved system column, cannot insert column value, cannot continue parse.
//
#define DBGQL_120                        0x000003C4L

//
// MessageId: DBGQL_121
//
// MessageText:
//
//  DBGQL_121: Mismatch between number of values %1$d and column names %2$d specified for insert statement, cannot continue parse.
//
#define DBGQL_121                        0x000003C5L

//
// MessageId: DBGQL_122
//
// MessageText:
//
//  DBGQL_122: Insert statement table not found, cannot continue parse.
//
#define DBGQL_122                        0x000003C6L

//
// MessageId: DBGQL_123
//
// MessageText:
//
//  DBGQL_123: `insert into ... select ...' to a version or row lock based table `%1$s' is unsupported, cannot continue parse.
//
#define DBGQL_123                        0x000003C7L

//
// MessageId: DBGQL_124
//
// MessageText:
//
//  DBGQL_124: Insert statement table not found, cannot continue parse.
//
#define DBGQL_124                        0x000003C8L

//
// MessageId: DBGQL_125
//
// MessageText:
//
//  DBGQL_125: `insert into ... select ...' to a version or row lock based table `%1$s' is unsupported, cannot continue parse.
//
#define DBGQL_125                        0x000003C9L

//
// MessageId: DBGQL_126
//
// MessageText:
//
//  DBGQL_126: Invalid feature number specification. '%1$s', restricted to numeric values, cannot continue parse.
//
#define DBGQL_126                        0x000003CAL

//
// MessageId: DBGQL_127
//
// MessageText:
//
//  DBGQL_127: Cannot locate feat_num value or bind location for `insert' into primary table `%1$s', cannot continue parse.
//
#define DBGQL_127                        0x000003CBL

//
// MessageId: DBGQL_128
//
// MessageText:
//
//  DBGQL_128: Insert statement table not found, cannot continue parse.
//
#define DBGQL_128                        0x000003CCL

//
// MessageId: DBGQL_129
//
// MessageText:
//
//  DBGQL_129: Table '%1$s' not found, cannot continue parse.
//
#define DBGQL_129                        0x000003CDL

//
// MessageId: DBGQL_130
//
// MessageText:
//
//  DBGQL_130: End of insert value list not found, cannot continue parse.
//
#define DBGQL_130                        0x000003CEL

//
// MessageId: DBGQL_131
//
// MessageText:
//
//  DBGQL_131: Start of insert value list not found, cannot continue parse.
//
#define DBGQL_131                        0x000003CFL

//
// MessageId: DBGQL_132
//
// MessageText:
//
//  DBGQL_132: `insert into ... select ...' to a primary table, version, row lock, or class based table `%1$s' is unsupported, cannot continue parse.
//
#define DBGQL_132                        0x000003D0L

//
// MessageId: DBGQL_133
//
// MessageText:
//
//  DBGQL_133: Update statement table not found, cannot continue parse.
//
#define DBGQL_133                        0x000003D1L

//
// MessageId: DBGQL_134
//
// MessageText:
//
//  DBGQL_134: Cannot locate update statement in parse tree, cannot continue parse.
//
#define DBGQL_134                        0x000003D2L

//
// MessageId: DBGQL_135
//
// MessageText:
//
//  DBGQL_135: Update of system column `%1$s' not allowed, cannot continue parse.
//
#define DBGQL_135                        0x000003D3L

//
// MessageId: DBGQL_136
//
// MessageText:
//
//  DBGQL_136: Update by set of a column using a query is not supported, cannot continue parse.
//
#define DBGQL_136                        0x000003D4L

//
// MessageId: DBGQL_137
//
// MessageText:
//
//  DBGQL_137: Failed to locate table %1$s%2$s%3$s, cannot continue parse.
//
#define DBGQL_137                        0x000003D5L

//
// MessageId: DBGQL_138
//
// MessageText:
//
//  DBGQL_138: Unknown DBI_RESERVED column type constant %1$d, cannot continue parse.
//
#define DBGQL_138                        0x000003D6L

//
// MessageId: DBGQL_139
//
// MessageText:
//
//  DBGQL_139: The system column '%1$s' found in table '%2$s' without associated system column '%3$s', cannot continue parse.
//
#define DBGQL_139                        0x000003D7L

//
// MessageId: DBGQL_140
//
// MessageText:
//
//  DBGQL_140: Object `%1$s%2$s%3$s' does not exist in the RDBMS, cannot continue parse.
//
#define DBGQL_140                        0x000003D8L

//
// MessageId: DBGQL_141
//
// MessageText:
//
//  DBGQL_141: Warning, string `%1$s' truncated to %2$d characters, parse continuing.
//
#define DBGQL_141                        0x000003D9L

//
// MessageId: DBGQL_142
//
// MessageText:
//
//  DBGQL_142: Internal error, feature number not defined, cannot get feature information.
//
#define DBGQL_142                        0x000003DAL

//
// MessageId: DBGQL_143
//
// MessageText:
//
//  DBGQL_143: `dbi_feat_info_def' for feature number %1$ld not found, cannot get feature information.
//
#define DBGQL_143                        0x000003DBL

//
// MessageId: DBGQL_144
//
// MessageText:
//
//  DBGQL_144: `feat_info' is not available, cannot convert select column `%1$s'.
//
#define DBGQL_144                        0x000003DCL

//
// MessageId: DBGQL_145
//
// MessageText:
//
//  DBGQL_145: Distance is not available for pseudo column `point_distance'.
//
#define DBGQL_145                        0x000003DDL

//
// MessageId: DBGQL_146
//
// MessageText:
//
//  DBGQL_146: Internal error, feature number not defined, cannot compute `point_distance'.
//
#define DBGQL_146                        0x000003DEL

//
// MessageId: DBGQL_147
//
// MessageText:
//
//  DBGQL_147: `dbi_feat_info_def' for feature number %1$ld not found, cannot compute `point_distance'
//
#define DBGQL_147                        0x000003DFL

//
// MessageId: DBGQL_148
//
// MessageText:
//
//  DBGQL_148: Warning: ignoring nonexistent feature %1$ld in candidate list
//
#define DBGQL_148                        0x000003E0L

//
// MessageId: DBGQL_149
//
// MessageText:
//
//  DBGQL_149: Coordinate system units not the same, ratio <= 0.0, cannot continue paring.
//
#define DBGQL_149                        0x000003E1L

//
// MessageId: DBGQL_150
//
// MessageText:
//
//  DBGQL_150: Parameter `n' is a NULL int, unable to return number of select columns.
//
#define DBGQL_150                        0x000003E2L

//
// MessageId: DBGQL_151
//
// MessageText:
//
//  DBGQL_151: `name' is a mandatory parameter, NULL or empty string found.
//
#define DBGQL_151                        0x000003E3L

//
// MessageId: DBGQL_152
//
// MessageText:
//
//  DBGQL_152: Parameter `col' is a NULL dbi_slct_def, unable to return select column information.
//
#define DBGQL_152                        0x000003E4L

//
// MessageId: DBGQL_153
//
// MessageText:
//
//  DBGQL_153: Select column name `%1$s' not found, cannot describe this column.
//
#define DBGQL_153                        0x000003E5L

//
// MessageId: DBGQL_154
//
// MessageText:
//
//  DBGQL_154: Parameter `within' is a NULL dbi_within_def, unable to return within information.
//
#define DBGQL_154                        0x000003E6L

//
// MessageId: DBGQL_155
//
// MessageText:
//
//  DBGQL_155: `name' is a mandatory parameter, NULL or empty string found.
//
#define DBGQL_155                        0x000003E7L

//
// MessageId: DBGQL_156
//
// MessageText:
//
//  DBGQL_156: `ascii' is a mandatory parameter, NULL pointer found.
//
#define DBGQL_156                        0x000003E8L

//
// MessageId: DBGQL_157
//
// MessageText:
//
//  DBGQL_157: Select column name `%1$s' not found, cannot get value in ascii format.
//
#define DBGQL_157                        0x000003E9L

//
// MessageId: DBGQL_158
//
// MessageText:
//
//  DBGQL_158: Output variable address not defined, NULL found.
//
#define DBGQL_158                        0x000003EAL

//
// MessageId: DBGQL_159
//
// MessageText:
//
//  DBGQL_159: Select column name `%1$s' value is not ascii convertible, cannot return value in ascii format.
//
#define DBGQL_159                        0x000003EBL

//
// MessageId: DBGQL_160
//
// MessageText:
//
//  DBGQL_160: Select column name `%1$s' value has been truncated.
//
#define DBGQL_160                        0x000003ECL

//
// MessageId: DBGQL_161
//
// MessageText:
//
//  DBGQL_161: Select column name `%1$s' not found, cannot get value.
//
#define DBGQL_161                        0x000003EDL

//
// MessageId: DBGQL_162
//
// MessageText:
//
//  DBGQL_162: `address' is a mandatory parameter, NULL pointer found.
//
#define DBGQL_162                        0x000003EEL

//
// MessageId: DBGQL_163
//
// MessageText:
//
//  DBGQL_163: Output variable address not defined, NULL found.
//
#define DBGQL_163                        0x000003EFL

//
// MessageId: DBGQL_164
//
// MessageText:
//
//  DBGQL_164: Cannot put value for %1$d byte column `%2$s' into %3$d byte buffer.
//
#define DBGQL_164                        0x000003F0L

//
// MessageId: DBGQL_165
//
// MessageText:
//
//  DBGQL_165: `dbi_feat_info_def' for feature number %1$ld not found
//
#define DBGQL_165                        0x000003F1L

//
// MessageId: DBGQL_166
//
// MessageText:
//
//  DBGQL_166: Empty string given as GQL statement, cannot continue parse.
//
#define DBGQL_166                        0x000003F2L

//
// MessageId: DBGQL_167
//
// MessageText:
//
//  DBGQL_167: `option' is a mandatory parameter, NULL or empty string found.
//
#define DBGQL_167                        0x000003F3L

//
// MessageId: DBGQL_168
//
// MessageText:
//
//  DBGQL_168: Parse error: %1$s
//
#define DBGQL_168                        0x000003F4L

//
// MessageId: DBGQL_169
//
// MessageText:
//
//  DBGQL_169: Retrieval of clipped-out points is not implemented...
//
#define DBGQL_169                        0x000003F5L

//
// MessageId: DBGQL_170
//
// MessageText:
//
//  DBGQL_170: Only points within search areas will be retrieved.
//
#define DBGQL_170                        0x000003F6L

//
// MessageId: DBGQL_171
//
// MessageText:
//
//  DBGQL_171: Nonexistent feature %1$ld requested for point retrieval
//
#define DBGQL_171                        0x000003F7L

//
// MessageId: DBGQL_172
//
// MessageText:
//
//  DBGQL_172: Layer %1$d is protected against read
//
#define DBGQL_172                        0x000003F8L

//
// MessageId: DBGQL_173
//
// MessageText:
//
//  DBGQL_173: Cannot find polygon %1$ld;  not using this polygon for spatial query
//
#define DBGQL_173                        0x000003F9L

//
// MessageId: DBGQL_174
//
// MessageText:
//
//  DBGQL_174: Empty string given as `ascii' parameter value, cannot set ascii bind value.
//
#define DBGQL_174                        0x000003FAL

//
// MessageId: DBGQL_175
//
// MessageText:
//
//  DBGQL_175: Parameter `name' is NULL or an empty string, cannot set ascii bind value.
//
#define DBGQL_175                        0x000003FBL

//
// MessageId: DBGQL_176
//
// MessageText:
//
//  DBGQL_176: Bind column name `%1$s' not found, cannot set input value.
//
#define DBGQL_176                        0x000003FCL

//
// MessageId: DBGQL_177
//
// MessageText:
//
//  DBGQL_177: Internal input storage not allocated for bind location %1$s, cannot continue set_val_a.
//
#define DBGQL_177                        0x000003FDL

//
// MessageId: DBGQL_178
//
// MessageText:
//
//  DBGQL_178: Warning, bind name `%1$s' value truncated.
//
#define DBGQL_178                        0x000003FEL

//
// MessageId: DBGQL_179
//
// MessageText:
//
//  DBGQL_179: Unknown option %1$s specified.
//
#define DBGQL_179                        0x000003FFL

//
// MessageId: DBGQL_180
//
// MessageText:
//
//  DBGQL_180: gid %1$d in execute phase, can no longer define array sizes.
//
#define DBGQL_180                        0x00000400L

//
// MessageId: DBGQL_181
//
// MessageText:
//
//  DBGQL_181: Gid %1$d in execute phase, can no longer bind array sizes.
//
#define DBGQL_181                        0x00000401L

//
// MessageId: DBGQL_182
//
// MessageText:
//
//  DBGQL_182: Gid %1$d in execute phase, can no longer cache lock feature.
//
#define DBGQL_182                        0x00000402L

//
// MessageId: DBGQL_183
//
// MessageText:
//
//  DBGQL_183: Gid %1$d not an insert GQL statement, cannot set lock request.
//
#define DBGQL_183                        0x00000403L

//
// MessageId: DBGQL_184
//
// MessageText:
//
//  DBGQL_184: Gid %1$d no active lock, cannot set lock request.
//
#define DBGQL_184                        0x00000404L

//
// MessageId: DBGQL_185
//
// MessageText:
//
//  DBGQL_185: Gid %1$d in execute phase, cannot set lock request.
//
#define DBGQL_185                        0x00000405L

//
// MessageId: DBGQL_186
//
// MessageText:
//
//  DBGQL_186: Gid %1$d not an insert GQL statement, cannot set version exclusive lock request.
//
#define DBGQL_186                        0x00000406L

//
// MessageId: DBGQL_187
//
// MessageText:
//
//  DBGQL_187: Gid %1$d no active lock, cannot set version exclusive lock request.
//
#define DBGQL_187                        0x00000407L

//
// MessageId: DBGQL_188
//
// MessageText:
//
//  DBGQL_188: Gid %1$d in execute phase, cannot set version exclusive lock request.
//
#define DBGQL_188                        0x00000408L

//
// MessageId: DBGQL_189
//
// MessageText:
//
//  DBGQL_189: Define output defaults: Number of primary key columns %1$d does not match found columns %2$d in select column.
//
#define DBGQL_189                        0x00000409L

//
// MessageId: DBGQL_190
//
// MessageText:
//
//  DBGQL_190: Can't find a token for inserting default space parameters for table.
//
#define DBGQL_190                        0x0000040AL

//
// MessageId: DBGQL_191
//
// MessageText:
//
//  DBGQL_191: Can't find a token for inserting default space parameters for index.
//
#define DBGQL_191                        0x0000040BL

//
// MessageId: DBGQL_192
//
// MessageText:
//
//  DBGQL_192: Invalid value '%1$s' for VISION_PARE_REPORT_LEVEL; using default of '0'
//
#define DBGQL_192                        0x0000040CL

//
// MessageId: DBGQL_193
//
// MessageText:
//
//  DBGQL_193: Pared %1$d candidate features down to %2$d (%3$d skipped).
//
#define DBGQL_193                        0x0000040DL

//
// MessageId: DBGQL_194
//
// MessageText:
//
//  DBGQL_194: Candidate types: %1$d pt, %2$d l, %3$d n, %4$d inc poly, %5$d cmpl poly
//
#define DBGQL_194                        0x0000040EL

//
// MessageId: DBGQL_195
//
// MessageText:
//
//  DBGQL_195: Kept types:      %1$d pt, %2$d l, %3$d n, %4$d inc poly, %5$d cmpl poly
//
#define DBGQL_195                        0x0000040FL

//
// MessageId: DBGQL_196
//
// MessageText:
//
//  DBGQL_196: Paring took %1$s seconds, processing %2$s cand feat/sec.
//
#define DBGQL_196                        0x00000410L

//
// MessageId: DBGQL_197
//
// MessageText:
//
//  DBGQL_197: Invalid value '%1$s' for VISION_FETCH_REPORT_LEVEL; using default of '0'
//
#define DBGQL_197                        0x00000411L

//
// MessageId: DBGQL_198
//
// MessageText:
//
//  DBGQL_198: Gid %1$d fetched %2$d rows in %2$s seconds (%3$s rows/sec).
//
#define DBGQL_198                        0x00000412L

//
// MessageId: DBGQL_199
//
// MessageText:
//
//  DBGQL_199: Gid %1$d candidates pared to %2$d features.
//
#define DBGQL_199                        0x00000413L

//
// MessageId: DBGQL_200
//
// MessageText:
//
//  DBGQL_200: Invalid value '%1$s' for VISION_EXECUTE_REPORT_LEVEL; using default of '0'
//
#define DBGQL_200                        0x00000414L

//
// MessageId: DBGQL_201
//
// MessageText:
//
//  DBGQL_201: Gid %1$d: execution processed %2$d rows in %3$s seconds (%4$s rows/sec).
//
#define DBGQL_201                        0x00000415L

//
// MessageId: DBGQL_202
//
// MessageText:
//
//  DBGQL_202: `select distinct ... for *lock*' not valid, cannot continue parse.
//
#define DBGQL_202                        0x00000416L

//
// MessageId: DBGQL_203
//
// MessageText:
//
//  DBGQL_203: drop index %1$s..., index '%2$s' is system maintained, cannot continue parse.
//
#define DBGQL_203                        0x00000417L

//
// MessageId: DBGQL_204
//
// MessageText:
//
//  DBGQL_204: Row is : '%1$s'
//
#define DBGQL_204                        0x00000418L

//
// MessageId: DBGQL_205
//
// MessageText:
//
//  DBGQL_205: SQL statement is : '%1$s'
//
#define DBGQL_205                        0x00000419L

//
// MessageId: DBGQL_206
//
// MessageText:
//
//  DBGQL_206: BLOB type (unprintable)
//
#define DBGQL_206                        0x0000041AL

//
// MessageId: DBGQL_207
//
// MessageText:
//
//  DBGQL_207: RAW type (unprintable)
//
#define DBGQL_207                        0x0000041BL

//
// MessageId: DBGQL_208
//
// MessageText:
//
//  DBGQL_208: Cannot set attribute flag - layer %1$d is protected against write.
//
#define DBGQL_208                        0x0000041CL

//
// MessageId: DBGQL_209
//
// MessageText:
//
//  DBGQL_209: Layer %1$d is protected against write.
//
#define DBGQL_209                        0x0000041DL

//
// MessageId: DBGQL_210
//
// MessageText:
//
//  DBGQL_210: Can't update database in all-versions query mode.
//
#define DBGQL_210                        0x0000041EL

//
// MessageId: DBGQL_211
//
// MessageText:
//
//  DBGQL_211: alter table %1$s..., table '%2$s' constains conditional data, cannot %3$s constraints.
//
#define DBGQL_211                        0x0000041FL

//
// MessageId: DBGQL_212
//
// MessageText:
//
//  DBGQL_212: alter table %1$s..., table '%2$s' constains shared or discared locks, cannot %3$s constraints.
//
#define DBGQL_212                        0x00000420L

//
// MessageId: DBGQL_213
//
// MessageText:
//
//  DBGQL_213: Can't do a conditional update to a primary key value."
//
#define DBGQL_213                        0x00000421L

//
// MessageId: DBGQL_214
//
// MessageText:
//
//  DBGQL_214: Table `%1$s' not found.  Cannot create shared lock table.
//
#define DBGQL_214                        0x00000422L

//
// MessageId: DBGQL_215
//
// MessageText:
//
//  DBGQL_215: No shared lock table for `%1$s', shared locks skipped.
//
#define DBGQL_215                        0x00000423L

//
// MessageId: DBGQL_216
//
// MessageText:
//
//  DBGQL_216: Bind column `row_lock' not found for table `%1$s'.
//
#define DBGQL_216                        0x00000424L

//
// MessageId: DBGQL_217
//
// MessageText:
//
//  DBGQL_217: Gid %1$d no active lock, cannot set commit lock request.
//
#define DBGQL_217                        0x00000425L

//
// MessageId: DBGQL_218
//
// MessageText:
//
//  DBGQL_218: Gid %1$d in execute phase, cannot set commit lock request.
//
#define DBGQL_218                        0x00000426L

//
// MessageId: DBGQL_219
//
// MessageText:
//
//  DBGQL_219: `select ... for commit_lock' valid only for table `%1$s'.
//
#define DBGQL_219                        0x00000427L

//
// MessageId: DBGQL_220
//
// MessageText:
//
//  DBGQL_220: Can't update a primary key value, shared or discarded lock set.
//
#define DBGQL_220                        0x00000428L

//
// MessageId: DBGQL_221
//
// MessageText:
//
//  DBGQL_221: unable to allocate sid array, NULL pointer returned.
//
#define DBGQL_221                        0x00000429L

//
// MessageId: DBGQL_222
//
// MessageText:
//
//  DBGQL_222: In sid %1$d: no sid's are defined, %2$s.
//
#define DBGQL_222                        0x0000042AL

//
// MessageId: DBGQL_223
//
// MessageText:
//
//  DBGQL_223: In sid %1$d: cannot get number of current sid's, %2$s.
//
#define DBGQL_223                        0x0000042BL

//
// MessageId: DBGQL_224
//
// MessageText:
//
//  DBGQL_224: In sid %1$d: not found, %2$s.
//
#define DBGQL_224                        0x0000042CL

//
// MessageId: DBGQL_225
//
// MessageText:
//
//  DBGQL_225: In sid %1$d: null address returned, %2$s.
//
#define DBGQL_225                        0x0000042DL

//
// MessageId: DBGQL_226
//
// MessageText:
//
//  DBGQL_226: In sid %1$d: previously deleted, %2$s.
//
#define DBGQL_226                        0x0000042EL

//
// MessageId: DBGQL_227
//
// MessageText:
//
//  DBGQL_227: Retrieval of clipped-out segments is not implemented...
//
#define DBGQL_227                        0x0000042FL

//
// MessageId: DBGQL_228
//
// MessageText:
//
//  DBGQL_228: Only segments within search areas will be retrieved.
//
#define DBGQL_228                        0x00000430L

//
// MessageId: DBGQL_229
//
// MessageText:
//
//  DBGQL_229: %1$s: pid %2$ld is not a sid.
//
#define DBGQL_229                        0x00000431L

//
// MessageId: DBGQL_230
//
// MessageText:
//
//  DBGQL_230: %1$s: sid %2$ld is not a pid.
//
#define DBGQL_230                        0x00000432L

//
// MessageId: DBGQL_231
//
// MessageText:
//
//  DBGQL_231: Expect type %1$s.
//
#define DBGQL_231                        0x00000433L

//
// MessageId: DBGQL_232
//
// MessageText:
//
//  DBGQL_232: Class not found for g_class %1$ld.
//
#define DBGQL_232                        0x00000434L

//
// MessageId: DBGQL_233
//
// MessageText:
//
//  DBGQL_233: Bind data for %1$s must be of type LONG.
//
#define DBGQL_233                        0x00000435L

//
// MessageId: DBGQL_234
//
// MessageText:
//
//  DBGQL_234: The null indicator must not be set when specifying bind parameters for %1$s.
//
#define DBGQL_234                        0x00000436L

//
// MessageId: DBGQL_235
//
// MessageText:
//
//  DBGQL_235: Failed to generate valid object number.  Cannot continue to execute.
//
#define DBGQL_235                        0x00000437L

//
// MessageId: DBGQL_236
//
// MessageText:
//
//  DBGQL_236: Both a class literal and a class bind variable were found.\nThis seems like an internal logic problem.
//
#define DBGQL_236                        0x00000438L

//
// MessageId: DBGQL_237
//
// MessageText:
//
//  DBGQL_237: G_Class parameters do not agree with one another.\nThis seems like an internal logic problem.
//
#define DBGQL_237                        0x00000439L

//
// MessageId: DBGQL_238
//
// MessageText:
//
//  DBGQL_238: Both an object literal and an object bind variable were found.\nThis seems like an internal logic problem.
//
#define DBGQL_238                        0x0000043AL

//
// MessageId: DBGQL_239
//
// MessageText:
//
//  DBGQL_239: The g_class was specified, but not the object number.\nThis seems like an internal logic problem.
//
#define DBGQL_239                        0x0000043BL

//
// MessageId: DBGQL_240
//
// MessageText:
//
//  DBGQL_240: An object number was specified, but no g_class.\nThis seems like an internal logic problem.
//
#define DBGQL_240                        0x0000043CL

//
// MessageId: DBGQL_241
//
// MessageText:
//
//  DBGQL_241: G_master_obj has not been created yet.
//
#define DBGQL_241                        0x0000043DL

//
// MessageId: DBGQL_242
//
// MessageText:
//
//  DBGQL_242: Name of object column is too long: %1$s
//
#define DBGQL_242                        0x0000043EL

//
// MessageId: DBGQL_243
//
// MessageText:
//
//  DBGQL_243: Could not get the name of the object number column for table %1$s.
//
#define DBGQL_243                        0x0000043FL

//
// MessageId: DBGQL_244
//
// MessageText:
//
//  DBGQL_244: Cannot specify a g_class column when the table %1$s is not class-based.
//
#define DBGQL_244                        0x00000440L

//
// MessageId: DBGQL_245
//
// MessageText:
//
//  DBGQL_245: Bind variable for %1$s must be declared as either a LONG or a STRING.
//
#define DBGQL_245                        0x00000441L

//
// MessageId: DBGQL_246
//
// MessageText:
//
//  DBGQL_246: This spatial query operator is not supported.
//
#define DBGQL_246                        0x00000442L

//
// MessageId: DBI_1
//
// MessageText:
//
//  DBI_1: Unable to retrieve feature info from copy buffer
//
#define DBI_1                            0x00000443L

//
// MessageId: DBI_2
//
// MessageText:
//
//  DBI_2: invalid coordinate system name
//
#define DBI_2                            0x00000444L

//
// MessageId: DBI_3
//
// MessageText:
//
//  DBI_3: Storage coordinate must be identified prior to input/output
//
#define DBI_3                            0x00000445L

//
// MessageId: DBI_4
//
// MessageText:
//
//  DBI_4: Unable to retrieve definition of `%1$s' coordinate system
//
#define DBI_4                            0x00000446L

//
// MessageId: DBI_5
//
// MessageText:
//
//  DBI_5: failed to duplicate the input
//
#define DBI_5                            0x00000447L

//
// MessageId: DBI_6
//
// MessageText:
//
//  DBI_6: Storage coordinate must be identified prior to input
//
#define DBI_6                            0x00000448L

//
// MessageId: DBI_7
//
// MessageText:
//
//  DBI_7: Unable to retrieve definition of `%1$s' coordinate system
//
#define DBI_7                            0x00000449L

//
// MessageId: DBI_8
//
// MessageText:
//
//  DBI_8: Unable to retrieve definition of `%1$s' coordinate system
//
#define DBI_8                            0x0000044AL

//
// MessageId: DBI_9
//
// MessageText:
//
//  DBI_9: Storage coordinate must be set prior to output
//
#define DBI_9                            0x0000044BL

//
// MessageId: DBI_10
//
// MessageText:
//
//  DBI_10: Unable to retrieve definition of `%1$s' coordinate system
//
#define DBI_10                           0x0000044CL

//
// MessageId: DBI_11
//
// MessageText:
//
//  DBI_11: Unable to retrieve definition of `%1$s' coordinate system
//
#define DBI_11                           0x0000044DL

//
// MessageId: DBI_12
//
// MessageText:
//
//  DBI_12: Storage coordinate is already set to '%1$s'
//
#define DBI_12                           0x0000044EL

//
// MessageId: DBI_13
//
// MessageText:
//
//  DBI_13: Unable to retrieve definition of `%1$s' coordinate system
//
#define DBI_13                           0x0000044FL

//
// MessageId: DBI_14
//
// MessageText:
//
//  DBI_14: Invalid physical coordinate system '%1$s'
//
#define DBI_14                           0x00000450L

//
// MessageId: DBI_15
//
// MessageText:
//
//  DBI_15: Invalid physical coordinate system '%1$s'
//
#define DBI_15                           0x00000451L

//
// MessageId: DBI_16
//
// MessageText:
//
//  DBI_16: Network %1$d is in layer %2$d, but layer %3$d was specified
//
#define DBI_16                           0x00000452L

//
// MessageId: DBI_17
//
// MessageText:
//
//  DBI_17: Network %1$d is in layer %2$d, but layer %3$d was specified
//
#define DBI_17                           0x00000453L

//
// MessageId: DBI_18
//
// MessageText:
//
//  DBI_18: Replica of feature %1$ld already exists in plan %2$d
//
#define DBI_18                           0x00000454L

//
// MessageId: DBI_19
//
// MessageText:
//
//  DBI_19: Feature %1$ld is locked, cannot add a replica
//
#define DBI_19                           0x00000455L

//
// MessageId: DBI_20
//
// MessageText:
//
//  DBI_20: New replica of feature %1$ld must be in the same layer, existing layer %2$d, new layer %3$d
//
#define DBI_20                           0x00000456L

//
// MessageId: DBI_21
//
// MessageText:
//
//  DBI_21: New replica of feature %1$ld must be in the same network, existing network %2$d, new network %3$d
//
#define DBI_21                           0x00000457L

//
// MessageId: DBI_22
//
// MessageText:
//
//  DBI_22: New replica of feature %1$ld must have the same Class ID %2$ld (not %3$ld)
//
#define DBI_22                           0x00000458L

//
// MessageId: DBI_23
//
// MessageText:
//
//  DBI_23: Attempt to add zero point feature, deleted.
//
#define DBI_23                           0x00000459L

//
// MessageId: DBI_24
//
// MessageText:
//
//  DBI_24: Requested feature was not found.
//
#define DBI_24                           0x0000045AL

//
// MessageId: DBI_25
//
// MessageText:
//
//  DBI_25: NULL lock_name pointer, cannot get lock name for feature %1$ld.
//
#define DBI_25                           0x0000045BL

//
// MessageId: DBI_26
//
// MessageText:
//
//  DBI_26: Feature %1$ld not found, cannot check feature lock.
//
#define DBI_26                           0x0000045CL

//
// MessageId: DBI_27
//
// MessageText:
//
//  DBI_27: Lock id %1$ld not found, cannot get lock for feature %2$ld.
//
#define DBI_27                           0x0000045DL

//
// MessageId: DBI_28
//
// MessageText:
//
//  DBI_28: Invalid feature lock request %1$d, cannot return lock information.
//
#define DBI_28                           0x0000045EL

//
// MessageId: DBI_29
//
// MessageText:
//
//  DBI_29: Cannot split, feature is locked
//
#define DBI_29                           0x0000045FL

//
// MessageId: DBI_30
//
// MessageText:
//
//  DBI_30: Lock '%1$s' does not exist; lock not activated
//
#define DBI_30                           0x00000460L

//
// MessageId: DBI_31
//
// MessageText:
//
//  DBI_31: Lock already opened by: %1$s
//
#define DBI_31                           0x00000461L

//
// MessageId: DBI_33
//
// MessageText:
//
//  DBI_33: Cannot delete the active lock name, no lock name is active.
//
#define DBI_33                           0x00000462L

//
// MessageId: DBI_34
//
// MessageText:
//
//  DBI_34: Cannot delete lock name %1$s, it does not exist.
//
#define DBI_34                           0x00000463L

//
// MessageId: DBI_35
//
// MessageText:
//
//  DBI_35: Cannot delete lock name %1$s, it is active to %2$s.
//
#define DBI_35                           0x00000464L

//
// MessageId: DBI_36
//
// MessageText:
//
//  DBI_36: Lock already opened by: %1$s
//
#define DBI_36                           0x00000465L

//
// MessageId: DBI_37
//
// MessageText:
//
//  DBI_37: Lock already opened by: %1$s
//
#define DBI_37                           0x00000466L

//
// MessageId: DBI_38
//
// MessageText:
//
//  DBI_38: Unable to allocate in-memory feature list.
//
#define DBI_38                           0x00000467L

//
// MessageId: DBI_39
//
// MessageText:
//
//  DBI_39: Unable to allocate transaction feature list.
//
#define DBI_39                           0x00000468L

//
// MessageId: DBI_40
//
// MessageText:
//
//  DBI_40: Append to in_mem_flist array failed, NULL element address returned.
//
#define DBI_40                           0x00000469L

//
// MessageId: DBI_41
//
// MessageText:
//
//  DBI_41: %1$d features to process.
//
#define DBI_41                           0x0000046AL

//
// MessageId: DBI_42
//
// MessageText:
//
//  DBI_42: Unable to get known feature list element position %1$d.
//
#define DBI_42                           0x0000046BL

//
// MessageId: DBI_43
//
// MessageText:
//
//  DBI_43: Append to in_mem_flist array failed, NULL element address returned.
//
#define DBI_43                           0x0000046CL

//
// MessageId: DBI_44
//
// MessageText:
//
//  DBI_44: Unable to delete feature %1$d ... continuing
//
#define DBI_44                           0x0000046DL

//
// MessageId: DBI_45
//
// MessageText:
//
//  DBI_45: Processed %1$d features\r
//
#define DBI_45                           0x0000046EL

//
// MessageId: DBI_46
//
// MessageText:
//
//  DBI_46: Rolled back current transaction; %1$d features rolled back.
//
#define DBI_46                           0x0000046FL

//
// MessageId: DBI_47
//
// MessageText:
//
//  DBI_47: Deleted %1$d out of %2$d features.
//
#define DBI_47                           0x00000470L

//
// MessageId: DBI_51
//
// MessageText:
//
//  DBI_51: Unable to lock table %1$s
//
#define DBI_51                           0x00000471L

//
// MessageId: DBI_52
//
// MessageText:
//
//  DBI_52: Unlocking '%1$s' row lock(s) by deleting rows from table '%2$s' ...
//
#define DBI_52                           0x00000472L

//
// MessageId: DBI_53
//
// MessageText:
//
//  DBI_53: %1$ld rows unlocked by deletion.
//
#define DBI_53                           0x00000473L

//
// MessageId: DBI_54
//
// MessageText:
//
//  DBI_54: %1$d rows could not be unlocked by deletion because they have been shared locked.
//
#define DBI_54                           0x00000474L

//
// MessageId: DBI_55
//
// MessageText:
//
//  DBI_55: Unlocking '%1$s' row lock(s) from table '%2$s' ...
//
#define DBI_55                           0x00000475L

//
// MessageId: DBI_56
//
// MessageText:
//
//  DBI_56: %1$ld rows unlocked by update.
//
#define DBI_56                           0x00000476L

//
// MessageId: DBI_57
//
// MessageText:
//
//  DBI_57:  %1$ld shared locks processed.
//
#define DBI_57                           0x00000477L

//
// MessageId: DBI_58
//
// MessageText:
//
//  DBI_58: Unlocking '%1$s' row lock(s) by deleting rows from table '%2$s' ...
//
#define DBI_58                           0x00000478L

//
// MessageId: DBI_59
//
// MessageText:
//
//  DBI_59: %1$ld rows unlocked by deletion.
//
#define DBI_59                           0x00000479L

//
// MessageId: DBI_60
//
// MessageText:
//
//  DBI_60: %1$d rows could not be unlocked by deletion because they have been shared locked.
//
#define DBI_60                           0x0000047AL

//
// MessageId: DBI_61
//
// MessageText:
//
//  DBI_61: Unlocking '%1$s' row lock(s) from table '%2$s' ...
//
#define DBI_61                           0x0000047BL

//
// MessageId: DBI_62
//
// MessageText:
//
//  DBI_62: %1$ld rows unlocked by update.
//
#define DBI_62                           0x0000047CL

//
// MessageId: DBI_63
//
// MessageText:
//
//  DBI_63:  %1$ld shared locks processed.
//
#define DBI_63                           0x0000047DL

//
// MessageId: DBI_64
//
// MessageText:
//
//  DBI_64: No active locking session, cannot unlock rows
//
#define DBI_64                           0x0000047EL

//
// MessageId: DBI_65
//
// MessageText:
//
//  DBI_65: Unable to get number of tables.
//
#define DBI_65                           0x0000047FL

//
// MessageId: DBI_66
//
// MessageText:
//
//  DBI_66: Unable to get known table with lock info
//
#define DBI_66                           0x00000480L

//
// MessageId: DBI_67
//
// MessageText:
//
//  DBI_67: Unable to allocate tbl_with_lks array, NULL pointer returned.
//
#define DBI_67                           0x00000481L

//
// MessageId: DBI_68
//
// MessageText:
//
//  DBI_68: Append to tbl_with_lks array failed, NULL element address returned.
//
#define DBI_68                           0x00000482L

//
// MessageId: DBI_69
//
// MessageText:
//
//  DBI_69: tbl_with_lks must be NULL, will be allocated, unable to get tables with locks.
//
#define DBI_69                           0x00000483L

//
// MessageId: DBI_70
//
// MessageText:
//
//  DBI_70: Plan %1$d not found; cannot activate it.
//
#define DBI_70                           0x00000484L

//
// MessageId: DBI_71
//
// MessageText:
//
//  DBI_71: Database id %1$d not found in database catalog
//
#define DBI_71                           0x00000485L

//
// MessageId: DBI_72
//
// MessageText:
//
//  DBI_72: Lock %1$s for plan %2$d could not be activated: in use by %3$s
//
#define DBI_72                           0x00000486L

//
// MessageId: DBI_73
//
// MessageText:
//
//  DBI_73: Previously active plan : %1$d could not be unlocked.
//
#define DBI_73                           0x00000487L

//
// MessageId: DBI_74
//
// MessageText:
//
//  DBI_74: Lock %1$s for plan %2$d could not be activated: in use by %3$s
//
#define DBI_74                           0x00000488L

//
// MessageId: DBI_75
//
// MessageText:
//
//  DBI_75: Could not generate a lock name for the plan
//
#define DBI_75                           0x00000489L

//
// MessageId: DBI_76
//
// MessageText:
//
//  DBI_76: Lock %1$s for plan %2$d could not be activated: in use by %3$s
//
#define DBI_76                           0x0000048AL

//
// MessageId: DBI_77
//
// MessageText:
//
//  DBI_77: Plan : %1$d could not be locked.
//
#define DBI_77                           0x0000048BL

//
// MessageId: DBI_78
//
// MessageText:
//
//  DBI_78: Plan : %1$d could not be locked
//
#define DBI_78                           0x0000048CL

//
// MessageId: DBI_79
//
// MessageText:
//
//  DBI_79: Corrupt active csys
//
#define DBI_79                           0x0000048DL

//
// MessageId: DBI_80
//
// MessageText:
//
//  DBI_80: '%1$s' cannot be used for plan creation
//
#define DBI_80                           0x0000048EL

//
// MessageId: DBI_81
//
// MessageText:
//
//  DBI_81: Plan 0 cannot be deleted.  It is permanent.
//
#define DBI_81                           0x0000048FL

//
// MessageId: DBI_82
//
// MessageText:
//
//  DBI_82: Invalid plan number: %1$d
//
#define DBI_82                           0x00000490L

//
// MessageId: DBI_83
//
// MessageText:
//
//  DBI_83: Active plan cannot be deleted.
//
#define DBI_83                           0x00000491L

//
// MessageId: DBI_84
//
// MessageText:
//
//  DBI_84: Plan number %1$d does not exist
//
#define DBI_84                           0x00000492L

//
// MessageId: DBI_85
//
// MessageText:
//
//  DBI_85: Could not generate a lock name for deleting plan %1$d
//
#define DBI_85                           0x00000493L

//
// MessageId: DBI_86
//
// MessageText:
//
//  DBI_86: Lock %1$s for plan %2$d could not be activated: in use by %3$s
//
#define DBI_86                           0x00000494L

//
// MessageId: DBI_87
//
// MessageText:
//
//  DBI_87: Plan : %1$d could not be locked.
//
#define DBI_87                           0x00000495L

//
// MessageId: DBI_88
//
// MessageText:
//
//  DBI_88: Plan : %1$d locked exclusively by %2$s
//
#define DBI_88                           0x00000496L

//
// MessageId: DBI_89
//
// MessageText:
//
//  DBI_89: Plan %1$d cannot be deleted since it contains %2$d features.
//
#define DBI_89                           0x00000497L

//
// MessageId: DBI_90
//
// MessageText:
//
//  DBI_90: Could not verify authorization for layer %1$ld
//
#define DBI_90                           0x00000498L

//
// MessageId: DBI_91
//
// MessageText:
//
//  DBI_91: No write permission for layer %1$ld
//
#define DBI_91                           0x00000499L

//
// MessageId: DBI_92
//
// MessageText:
//
//  DBI_92: Invalid coordinate type %1$d is not in range (%2$d to %3$d).
//
#define DBI_92                           0x0000049AL

//
// MessageId: DBI_93
//
// MessageText:
//
//  DBI_93: Invalid coordinate type %1$d is not in range (%2$d to %3$d).
//
#define DBI_93                           0x0000049BL

//
// MessageId: DBI_94
//
// MessageText:
//
//  DBI_94: Storage coordinate system '%1$s' does not exist.
//
#define DBI_94                           0x0000049CL

//
// MessageId: DBI_95
//
// MessageText:
//
//  DBI_95: Storage_resolution field %1$s is not greater than 0.0
//
#define DBI_95                           0x0000049DL

//
// MessageId: DBI_96
//
// MessageText:
//
//  DBI_96: Field MIN_X greater than MAX_X in dbi_extent_def structure
//
#define DBI_96                           0x0000049EL

//
// MessageId: DBI_97
//
// MessageText:
//
//  DBI_97: Field MIN_Y greater than MAX_Y in dbi_extent_def structure
//
#define DBI_97                           0x0000049FL

//
// MessageId: DBI_98
//
// MessageText:
//
//  DBI_98: Feature code beginning '%1$0.12s' is too long
//
#define DBI_98                           0x000004A0L

//
// MessageId: DBI_99
//
// MessageText:
//
//  DBI_99: Invalid feature type %1$d is not in range (%2$d to %3$d).
//
#define DBI_99                           0x000004A1L

//
// MessageId: DBI_100
//
// MessageText:
//
//  DBI_100: Negative layer number %1$d is invalid
//
#define DBI_100                          0x000004A2L

//
// MessageId: DBI_101
//
// MessageText:
//
//  DBI_101: Less than or equal to zero network number %1$d is invalid
//
#define DBI_101                          0x000004A3L

//
// MessageId: DBI_102
//
// MessageText:
//
//  DBI_102: Field name: %1$s exceeds maximum allowable length (%2$d)
//
#define DBI_102                          0x000004A4L

//
// MessageId: DBI_103
//
// MessageText:
//
//  DBI_103: Invalid field_type value %1$d is not in range (%2$d to %3$d).
//
#define DBI_103                          0x000004A5L

//
// MessageId: DBI_104
//
// MessageText:
//
//  DBI_104: Field field_size has invalid negative values
//
#define DBI_104                          0x000004A6L

//
// MessageId: DBI_105
//
// MessageText:
//
//  DBI_105: Invalid negative layer number: %1$d.
//
#define DBI_105                          0x000004A7L

//
// MessageId: DBI_106
//
// MessageText:
//
//  DBI_106: Network number %1$d is invalid. It must be greater than 0
//
#define DBI_106                          0x000004A8L

//
// MessageId: DBI_107
//
// MessageText:
//
//  DBI_107: Network layer number %1$d is invalid.
//
#define DBI_107                          0x000004A9L

//
// MessageId: DBI_108
//
// MessageText:
//
//  DBI_108: Network type '%1$c' is invalid.
//
#define DBI_108                          0x000004AAL

//
// MessageId: DBI_109
//
// MessageText:
//
//  DBI_109: Non-NULL plan_info pointer is required
//
#define DBI_109                          0x000004ABL

//
// MessageId: DBI_110
//
// MessageText:
//
//  DBI_110: Plan number %1$d is not positive
//
#define DBI_110                          0x000004ACL

//
// MessageId: DBI_111
//
// MessageText:
//
//  DBI_111: Coordinate system '%1$s' does not exist
//
#define DBI_111                          0x000004ADL

//
// MessageId: DBI_112
//
// MessageText:
//
//  DBI_112: Storage_resolution field %1$s is not greater than 0.0
//
#define DBI_112                          0x000004AEL

//
// MessageId: DBI_113
//
// MessageText:
//
//  DBI_113: Z_resolution field %1$s is not greater than 0.0
//
#define DBI_113                          0x000004AFL

//
// MessageId: DBI_114
//
// MessageText:
//
//  DBI_114: Structure pointer point_posn is NULL
//
#define DBI_114                          0x000004B0L

//
// MessageId: DBI_115
//
// MessageText:
//
//  DBI_115: Field posn_type value '%1$c' is invalid
//
#define DBI_115                          0x000004B1L

//
// MessageId: DBI_116
//
// MessageText:
//
//  DBI_116: Table '%1$s' not found for %2$s
//
#define DBI_116                          0x000004B2L

//
// MessageId: DBI_117
//
// MessageText:
//
//  DBI_117: '%1$s' is not a table, so is not suitable for %2$s
//
#define DBI_117                          0x000004B3L

//
// MessageId: DBI_118
//
// MessageText:
//
//  DBI_118: Invalid feature type %1$d is not in range (%2$d to %3$d).
//
#define DBI_118                          0x000004B4L

//
// MessageId: DBI_119
//
// MessageText:
//
//  DBI_119: Negative layer number %1$d is invalid
//
#define DBI_119                          0x000004B5L

//
// MessageId: DBI_120
//
// MessageText:
//
//  DBI_120: Negative network number %1$d is invalid
//
#define DBI_120                          0x000004B6L

//
// MessageId: DBI_121
//
// MessageText:
//
//  DBI_121: Feature number %1$d is invalid
//
#define DBI_121                          0x000004B7L

//
// MessageId: DBI_122
//
// MessageText:
//
//  DBI_122: Invalid search specification %1$d is not in range (%2$d to %3$d).
//
#define DBI_122                          0x000004B8L

//
// MessageId: DBI_123
//
// MessageText:
//
//  DBI_123: '%1$s' string improperly constructed.
//
#define DBI_123                          0x000004B9L

//
// MessageId: DBI_124
//
// MessageText:
//
//  DBI_124: Cosine and sine angles of rotation must within +/-%1$s of 1.0 when squared and added
//
#define DBI_124                          0x000004BAL

//
// MessageId: DBI_125
//
// MessageText:
//
//  DBI_125: Field field_size has invalid negative values
//
#define DBI_125                          0x000004BBL

//
// MessageId: DBI_126
//
// MessageText:
//
//  DBI_126: Cannot allocate memory for table list.
//
#define DBI_126                          0x000004BCL

//
// MessageId: DBI_127
//
// MessageText:
//
//  DBI_127: Invalid version number %1$d.
//
#define DBI_127                          0x000004BDL

//
// MessageId: DBI_128
//
// MessageText:
//
//  DBI_128: Transaction active - cannot continue version commit...
//
#define DBI_128                          0x000004BEL

//
// MessageId: DBI_129
//
// MessageText:
//
//  DBI_129: Version active - cannot continue commit.
//
#define DBI_129                          0x000004BFL

//
// MessageId: DBI_130
//
// MessageText:
//
//  DBI_130: Table '%1$s' not found.
//
#define DBI_130                          0x000004C0L

//
// MessageId: DBI_131
//
// MessageText:
//
//  DBI_131: There is no active lock to lock version
//
#define DBI_131                          0x000004C1L

//
// MessageId: DBI_132
//
// MessageText:
//
//  DBI_132: Version %1$d is locked by lock name '%2$s', cannot lock version
//
#define DBI_132                          0x000004C2L

//
// MessageId: DBI_133
//
// MessageText:
//
//  DBI_133: Invalid version number %1$d.
//
#define DBI_133                          0x000004C3L

//
// MessageId: DBI_134
//
// MessageText:
//
//  DBI_134: Transaction active - cannot continue version rollback...
//
#define DBI_134                          0x000004C4L

//
// MessageId: DBI_135
//
// MessageText:
//
//  DBI_135: Version active - cannot continue commit.
//
#define DBI_135                          0x000004C5L

//
// MessageId: DBI_136
//
// MessageText:
//
//  DBI_136: Table '%1$s' not found.
//
#define DBI_136                          0x000004C6L

//
// MessageId: DBI_137
//
// MessageText:
//
//  DBI_137: FAILURE: GQL syntax error at line %1$d, near '%2$.30s'
//
#define DBI_137                          0x000004C7L

//
// MessageId: DBI_138
//
// MessageText:
//
//  DBI_138: feature code '%1$s' invalid
//
#define DBI_138                          0x000004C8L

//
// MessageId: DBI_139
//
// MessageText:
//
//  DBI_139: Invalid Attention Point table found
//
#define DBI_139                          0x000004C9L

//
// MessageId: DBI_140
//
// MessageText:
//
//  DBI_140: Feature has attributes, but no primary table found.
//
#define DBI_140                          0x000004CAL

//
// MessageId: DBI_141
//
// MessageText:
//
//  DBI_141: Feature %1$ld must have the same primary table,\n existing feat_code '%2$s', table '%3$s',\n new feat_code '%4$s', table '%5$s'
//
#define DBI_141                          0x000004CBL

//
// MessageId: DBI_142
//
// MessageText:
//
//  DBI_142: %1$d could not be locked.
//
#define DBI_142                          0x000004CCL

//
// MessageId: DBI_143
//
// MessageText:
//
//  DBI_143: %1$d in conflict
//
#define DBI_143                          0x000004CDL

//
// MessageId: DBI_144
//
// MessageText:
//
//  DBI_144: %1$d could not be unlocked.
//
#define DBI_144                          0x000004CEL

//
// MessageId: DBI_145
//
// MessageText:
//
//  DBI_145: Choose only interior, exterior or both edge types
//
#define DBI_145                          0x000004CFL

//
// MessageId: DBI_146
//
// MessageText:
//
//  DBI_146:  Version Query Mode is not DATAVIEW or RECENT
//
#define DBI_146                          0x000004D0L

//
// MessageId: DBI_147
//
// MessageText:
//
//  DBI_147: \tCan't retrieve contiguous edges for polygon %1$d
//
#define DBI_147                          0x000004D1L

//
// MessageId: DBI_148
//
// MessageText:
//
//  DBI_148: connection not found at current_edge %1$d
//
#define DBI_148                          0x000004D2L

//
// MessageId: DBI_149
//
// MessageText:
//
//  DBI_149: Polygon edge %1$ld has no points
//
#define DBI_149                          0x000004D3L

//
// MessageId: DBI_150
//
// MessageText:
//
//  DBI_150: Edge %1$ld's record missing from edge table
//
#define DBI_150                          0x000004D4L

//
// MessageId: DBI_151
//
// MessageText:
//
//  DBI_151: Invalid edge specifier %1$c, must be i, e, or b
//
#define DBI_151                          0x000004D5L

//
// MessageId: DBI_152
//
// MessageText:
//
//  DBI_152: Invalid low feature code >%1$s<
//
#define DBI_152                          0x000004D6L

//
// MessageId: DBI_153
//
// MessageText:
//
//  DBI_153: Invalid high feature code >%1$s<
//
#define DBI_153                          0x000004D7L

//
// MessageId: DBI_154
//
// MessageText:
//
//  DBI_154: Invalid primary table name >%1$s<
//
#define DBI_154                          0x000004D8L

//
// MessageId: DBI_156
//
// MessageText:
//
//  DBI_156: Order of feature code '%1$s' -> '%2$s' is not ascending
//
#define DBI_156                          0x000004D9L

//
// MessageId: DBI_157
//
// MessageText:
//
//  DBI_157: Feature code used by another table or overlaps another range - Invalid feature code range.
//
#define DBI_157                          0x000004DAL

//
// MessageId: DBI_158
//
// MessageText:
//
//  DBI_158: Could not enter the primary table name into the feat_code table.
//
#define DBI_158                          0x000004DBL

//
// MessageId: DBI_159
//
// MessageText:
//
//  DBI_159: Feature code range not inserted into the memory resident schema
//
#define DBI_159                          0x000004DCL

//
// MessageId: DBI_160
//
// MessageText:
//
//  DBI_160: Primary flag '%1$c' is invalid
//
#define DBI_160                          0x000004DDL

//
// MessageId: DBI_161
//
// MessageText:
//
//  DBI_161: There is no currently active database. Extents cannot be changed
//
#define DBI_161                          0x000004DEL

//
// MessageId: DBI_162
//
// MessageText:
//
//  DBI_162: Active plan %1$d information not found
//
#define DBI_162                          0x000004DFL

//
// MessageId: DBI_163
//
// MessageText:
//
//  DBI_163: To change extents, the database must be opened exclusively
//
#define DBI_163                          0x000004E0L

//
// MessageId: DBI_164
//
// MessageText:
//
//  DBI_164: The maximum x value is less than or equal to the minimum x value
//
#define DBI_164                          0x000004E1L

//
// MessageId: DBI_165
//
// MessageText:
//
//  DBI_165: The maximum y value is less than or equal to the minimum y value
//
#define DBI_165                          0x000004E2L

//
// MessageId: DBI_166
//
// MessageText:
//
//  DBI_166: You cannot change the database resolution once there is data in it
//
#define DBI_166                          0x000004E3L

//
// MessageId: DBI_167
//
// MessageText:
//
//  DBI_167: You cannot shrink the database coverage area once there is data in it
//
#define DBI_167                          0x000004E4L

//
// MessageId: DBI_168
//
// MessageText:
//
//  DBI_168: You must retain the same database centroid once there is data in the database
//
#define DBI_168                          0x000004E5L

//
// MessageId: DBI_169
//
// MessageText:
//
//  DBI_169: Ran out of points
//
#define DBI_169                          0x000004E6L

//
// MessageId: DBI_170
//
// MessageText:
//
//  DBI_170: Cleaning database %1$s, please wait ...
//
#define DBI_170                          0x000004E7L

//
// MessageId: DBI_171
//
// MessageText:
//
//  DBI_171: Cannot find description of database.
//
#define DBI_171                          0x000004E8L

//
// MessageId: DBI_172
//
// MessageText:
//
//  DBI_172: One or more layers are protected against write in database %1$s,\n Database clean not allowed
//
#define DBI_172                          0x000004E9L

//
// MessageId: DBI_173
//
// MessageText:
//
//  DBI_173: Problems deleting feature data.
//
#define DBI_173                          0x000004EAL

//
// MessageId: DBI_174
//
// MessageText:
//
//  DBI_174: Database connect identifier invalid in close
//
#define DBI_174                          0x000004EBL

//
// MessageId: DBI_175
//
// MessageText:
//
//  DBI_175: Database id %1$d not found in database catalog
//
#define DBI_175                          0x000004ECL

//
// MessageId: DBI_176
//
// MessageText:
//
//  DBI_176: Lock %1$s for plan %2$d could not be activated: in use by %3$s
//
#define DBI_176                          0x000004EDL

//
// MessageId: DBI_177
//
// MessageText:
//
//  DBI_177: Plan : %1$d could not be unlocked.
//
#define DBI_177                          0x000004EEL

//
// MessageId: DBI_178
//
// MessageText:
//
//  DBI_178: Coordinate system information has not been initialized.
//
#define DBI_178                          0x000004EFL

//
// MessageId: DBI_179
//
// MessageText:
//
//  DBI_179: WARNING:  Existing user '%1$s' will have unrestricted access to database '%2$s'.
//
#define DBI_179                          0x000004F0L

//
// MessageId: DBI_180
//
// MessageText:
//
//  DBI_180: %1$s can't be used for map creation
//
#define DBI_180                          0x000004F1L

//
// MessageId: DBI_181
//
// MessageText:
//
//  DBI_181: Cannot connect to the host database '%1$s'
//
#define DBI_181                          0x000004F2L

//
// MessageId: DBI_182
//
// MessageText:
//
//  DBI_182: Unable to delete display list files. Database %1$s is not open
//
#define DBI_182                          0x000004F3L

//
// MessageId: DBI_183
//
// MessageText:
//
//  DBI_183: Display list deletion: layer %1$d of database %2$s does not exist.
//
#define DBI_183                          0x000004F4L

//
// MessageId: DBI_184
//
// MessageText:
//
//  DBI_184: Invalid network number: %1$d
//
#define DBI_184                          0x000004F5L

//
// MessageId: DBI_185
//
// MessageText:
//
//  DBI_185: Network number %1$d does not exist
//
#define DBI_185                          0x000004F6L

//
// MessageId: DBI_186
//
// MessageText:
//
//  DBI_186: Network %1$d cannot be deleted since it contains %2$d features.
//
#define DBI_186                          0x000004F7L

//
// MessageId: DBI_187
//
// MessageText:
//
//  DBI_187: Cannot delete from table '%1$s' - it contains a spatial reference.
//
#define DBI_187                          0x000004F8L

//
// MessageId: DBI_188
//
// MessageText:
//
//  DBI_188: One or more layers are protected against write in database %1$s,\n Database delete not allowed
//
#define DBI_188                          0x000004F9L

//
// MessageId: DBI_189
//
// MessageText:
//
//  DBI_189: parameter `datatype' is a NULL integer, unable to return data type.
//
#define DBI_189                          0x000004FAL

//
// MessageId: DBI_190
//
// MessageText:
//
//  DBI_190: parameter `size' is a NULL int, unable to return size.
//
#define DBI_190                          0x000004FBL

//
// MessageId: DBI_191
//
// MessageText:
//
//  DBI_191: Column `%1$s' not found in table `%2$s', cannot return `datatype' and `size'
//
#define DBI_191                          0x000004FCL

//
// MessageId: DBI_192
//
// MessageText:
//
//  DBI_192: Measurement handle has not been initialized.
//
#define DBI_192                          0x000004FDL

//
// MessageId: DBI_193
//
// MessageText:
//
//  DBI_193: Measurement handle has not been initialized.
//
#define DBI_193                          0x000004FEL

//
// MessageId: DBI_194
//
// MessageText:
//
//  DBI_194: Specified layer %1$d not found
//
#define DBI_194                          0x000004FFL

//
// MessageId: DBI_195
//
// MessageText:
//
//  DBI_195: Specified network %1$d not found
//
#define DBI_195                          0x00000500L

//
// MessageId: DBI_196
//
// MessageText:
//
//  DBI_196: Primary table not found for feature code: '%1$s'
//
#define DBI_196                          0x00000501L

//
// MessageId: DBI_197
//
// MessageText:
//
//  DBI_197: Invalid network number: %1$d
//
#define DBI_197                          0x00000502L

//
// MessageId: DBI_198
//
// MessageText:
//
//  DBI_198: Network %1$d not found
//
#define DBI_198                          0x00000503L

//
// MessageId: DBI_199
//
// MessageText:
//
//  DBI_199: Form operation requested for non-polygon network (%1$d)
//
#define DBI_199                          0x00000504L

//
// MessageId: DBI_200
//
// MessageText:
//
//  DBI_200: Cannot form polygons: Version Query Mode is not DATAVIEW or RECENT
//
#define DBI_200                          0x00000505L

//
// MessageId: DBI_201
//
// MessageText:
//
//  DBI_201: %1$s: %2$s
//
#define DBI_201                          0x00000506L

//
// MessageId: DBI_202
//
// MessageText:
//
//  DBI_202: %1$s: %2$s
//
#define DBI_202                          0x00000507L

//
// MessageId: DBI_203
//
// MessageText:
//
//  DBI_203: Run out of points in feature
//
#define DBI_203                          0x00000508L

//
// MessageId: DBI_204
//
// MessageText:
//
//  DBI_204: Coordinate systems are not initialized
//
#define DBI_204                          0x00000509L

//
// MessageId: DBI_205
//
// MessageText:
//
//  DBI_205: Input/Output/Storage coordinate system specifier '%1$c' is invalid
//
#define DBI_205                          0x0000050AL

//
// MessageId: DBI_206
//
// MessageText:
//
//  DBI_206: %1$s: %2$s
//
#define DBI_206                          0x0000050BL

//
// MessageId: DBI_207
//
// MessageText:
//
//  DBI_207: %1$s: %2$s
//
#define DBI_207                          0x0000050CL

//
// MessageId: DBI_208
//
// MessageText:
//
//  DBI_208: Coordinate system information has not been initialized.
//
#define DBI_208                          0x0000050DL

//
// MessageId: DBI_209
//
// MessageText:
//
//  DBI_209: There is no currently open database
//
#define DBI_209                          0x0000050EL

//
// MessageId: DBI_210
//
// MessageText:
//
//  DBI_210: Invalid negative layer number: %1$d
//
#define DBI_210                          0x0000050FL

//
// MessageId: DBI_211
//
// MessageText:
//
//  DBI_211: Coordinate system information has not been initialized.
//
#define DBI_211                          0x00000510L

//
// MessageId: DBI_212
//
// MessageText:
//
//  DBI_212: Invalid coordinate system specified for measure handle.
//
#define DBI_212                          0x00000511L

//
// MessageId: DBI_213
//
// MessageText:
//
//  DBI_213: Network number %1$d is invalid. It must be greater than 1
//
#define DBI_213                          0x00000512L

//
// MessageId: DBI_214
//
// MessageText:
//
//  DBI_214: Network number %1$d is invalid. It must be greater than 1
//
#define DBI_214                          0x00000513L

//
// MessageId: DBI_215
//
// MessageText:
//
//  DBI_215: Interior/Exterior flag must be 'i' or 'e', not '%1$c'
//
#define DBI_215                          0x00000514L

//
// MessageId: DBI_216
//
// MessageText:
//
//  DBI_216: Circuit number '%1$d' is invalid; %2$s
//
#define DBI_216                          0x00000515L

//
// MessageId: DBI_217
//
// MessageText:
//
//  DBI_217: Could not access database extents; OCD spots differences not installed.
//
#define DBI_217                          0x00000516L

//
// MessageId: DBI_218
//
// MessageText:
//
//  DBI_218: Coordinate system information has not been initialized; %1$s
//
#define DBI_218                          0x00000517L

//
// MessageId: DBI_219
//
// MessageText:
//
//  DBI_219: Loading spot difference table...
//
#define DBI_219                          0x00000518L

//
// MessageId: DBI_220
//
// MessageText:
//
//  DBI_220: Can't transform extents to GD; %1$s
//
#define DBI_220                          0x00000519L

//
// MessageId: DBI_221
//
// MessageText:
//
//  DBI_221: Can't convert database extent to GD; %1$s
//
#define DBI_221                          0x0000051AL

//
// MessageId: DBI_222
//
// MessageText:
//
//  DBI_222: Database extents exceed circuit extents; %1$s
//
#define DBI_222                          0x0000051BL

//
// MessageId: DBI_223
//
// MessageText:
//
//  DBI_223: Can't convert database extent to GD; %1$s
//
#define DBI_223                          0x0000051CL

//
// MessageId: DBI_224
//
// MessageText:
//
//  DBI_224: Database extents exceed circuit extents; %1$s
//
#define DBI_224                          0x0000051DL

//
// MessageId: DBI_225
//
// MessageText:
//
//  DBI_225: search outside margin
//
#define DBI_225                          0x0000051EL

//
// MessageId: DBI_226
//
// MessageText:
//
//  DBI_226: Not enough OCD spot differences fall within database extents; %1$s
//
#define DBI_226                          0x0000051FL

//
// MessageId: DBI_227
//
// MessageText:
//
//  DBI_227: Invalid label orientation: %1$d
//
#define DBI_227                          0x00000520L

//
// MessageId: DBI_228
//
// MessageText:
//
//  DBI_228: Invalid label number: %1$d
//
#define DBI_228                          0x00000521L

//
// MessageId: DBI_229
//
// MessageText:
//
//  DBI_229: Unable to lock table %1$s
//
#define DBI_229                          0x00000522L

//
// MessageId: DBI_230
//
// MessageText:
//
//  DBI_230: no active coordinate system record
//
#define DBI_230                          0x00000523L

//
// MessageId: DBI_231
//
// MessageText:
//
//  DBI_231: Can't find any record of connection
//
#define DBI_231                          0x00000524L

//
// MessageId: DBI_232
//
// MessageText:
//
//  DBI_232: Corrupt plan list in database log
//
#define DBI_232                          0x00000525L

//
// MessageId: DBI_233
//
// MessageText:
//
//  DBI_233: Null coordinate system information
//
#define DBI_233                          0x00000526L

//
// MessageId: DBI_234
//
// MessageText:
//
//  DBI_234: Input/Output coordinate system specifier '%1$c' is invalid
//
#define DBI_234                          0x00000527L

//
// MessageId: DBI_235
//
// MessageText:
//
//  DBI_235: database '%1$s' is already open
//
#define DBI_235                          0x00000528L

//
// MessageId: DBI_236
//
// MessageText:
//
//  DBI_236: Can't log any more databases
//
#define DBI_236                          0x00000529L

//
// MessageId: DBI_237
//
// MessageText:
//
//  DBI_237: Database id %1$d not found in database catalog
//
#define DBI_237                          0x0000052AL

//
// MessageId: DBI_238
//
// MessageText:
//
//  DBI_238: bad request connection not open
//
#define DBI_238                          0x0000052BL

//
// MessageId: DBI_239
//
// MessageText:
//
//  DBI_239: Named database '%1$s' not found
//
#define DBI_239                          0x0000052CL

//
// MessageId: DBI_240
//
// MessageText:
//
//  DBI_240: Networks may not be moved from one layer to another
//
#define DBI_240                          0x0000052DL

//
// MessageId: DBI_241
//
// MessageText:
//
//  DBI_241: Network types may not be changed
//
#define DBI_241                          0x0000052EL

//
// MessageId: DBI_242
//
// MessageText:
//
//  DBI_242: Coordinate system information is not logged
//
#define DBI_242                          0x0000052FL

//
// MessageId: DBI_243
//
// MessageText:
//
//  DBI_243: %1$s() must be replaced by %2$s()
//
#define DBI_243                          0x00000530L

//
// MessageId: DBI_244
//
// MessageText:
//
//  DBI_244: Invalid access mode `%1$s' for database `%2$s'
//
#define DBI_244                          0x00000531L

//
// MessageId: DBI_245
//
// MessageText:
//
//  DBI_245: Versioning context not set, cannot open database `%1$s'
//
#define DBI_245                          0x00000532L

//
// MessageId: DBI_246
//
// MessageText:
//
//  DBI_246: Invalid operator %1$d for overlay
//
#define DBI_246                          0x00000533L

//
// MessageId: DBI_247
//
// MessageText:
//
//  DBI_247: Sliver ceiling must be at least as great as sliver floor
//
#define DBI_247                          0x00000534L

//
// MessageId: DBI_248
//
// MessageText:
//
//  DBI_248: Invalid sliver floor threshold %1$s
//
#define DBI_248                          0x00000535L

//
// MessageId: DBI_249
//
// MessageText:
//
//  DBI_249: Invalid sliver ceiling threshold %1$s
//
#define DBI_249                          0x00000536L

//
// MessageId: DBI_250
//
// MessageText:
//
//  DBI_250: Can't overlay into network %1$d: not empty
//
#define DBI_250                          0x00000537L

//
// MessageId: DBI_251
//
// MessageText:
//
//  DBI_251: Cannot overlay polygons: Version Query Mode is not DATAVIEW or RECENT
//
#define DBI_251                          0x00000538L

//
// MessageId: DBI_252
//
// MessageText:
//
//  DBI_252: Overlay operation requested for non-existent network (%1$d)
//
#define DBI_252                          0x00000539L

//
// MessageId: DBI_253
//
// MessageText:
//
//  DBI_253: Overlay operation requested for non-polygon network (%1$ld)
//
#define DBI_253                          0x0000053AL

//
// MessageId: DBI_254
//
// MessageText:
//
//  DBI_254: Invalid network number: %1$d
//
#define DBI_254                          0x0000053BL

//
// MessageId: DBI_255
//
// MessageText:
//
//  DBI_255: Network %1$d not found
//
#define DBI_255                          0x0000053CL

//
// MessageId: DBI_256
//
// MessageText:
//
//  DBI_256: Cannot parcelize network: Version Query Mode is not DATAVIEW or RECENT
//
#define DBI_256                          0x0000053DL

//
// MessageId: DBI_257
//
// MessageText:
//
//  DBI_257: Feature cannot be read
//
#define DBI_257                          0x0000053EL

//
// MessageId: DBI_258
//
// MessageText:
//
//  DBI_258: Layer %1$d is protected against read
//
#define DBI_258                          0x0000053FL

//
// MessageId: DBI_259
//
// MessageText:
//
//  DBI_259: Feature cannot be updated
//
#define DBI_259                          0x00000540L

//
// MessageId: DBI_260
//
// MessageText:
//
//  DBI_260: Layer %1$d is protected against write or update
//
#define DBI_260                          0x00000541L

//
// MessageId: DBI_261
//
// MessageText:
//
//  DBI_261: The resolution snapped extents are not within the coordinate system extents.
//
#define DBI_261                          0x00000542L

//
// MessageId: DBI_262
//
// MessageText:
//
//  DBI_262: null information; can't restore
//
#define DBI_262                          0x00000543L

//
// MessageId: DBI_263
//
// MessageText:
//
//  DBI_263: No active information; can't restore
//
#define DBI_263                          0x00000544L

//
// MessageId: DBI_264
//
// MessageText:
//
//  DBI_264: Corrupt plan list in database catalog
//
#define DBI_264                          0x00000545L

//
// MessageId: DBI_265
//
// MessageText:
//
//  DBI_265: Database '%1$s' doesn't exist for setting password
//
#define DBI_265                          0x00000546L

//
// MessageId: DBI_266
//
// MessageText:
//
//  DBI_266: System error: cannot allocate memory
//
#define DBI_266                          0x00000547L

//
// MessageId: DBI_267
//
// MessageText:
//
//  DBI_267: System error: cannot allocate memory
//
#define DBI_267                          0x00000548L

//
// MessageId: DBI_268
//
// MessageText:
//
//  DBI_268: Surface '%1$s' does not begin with alphabetic
//
#define DBI_268                          0x00000549L

//
// MessageId: DBI_269
//
// MessageText:
//
//  DBI_269: allocating string
//
#define DBI_269                          0x0000054AL

//
// MessageId: DBI_270
//
// MessageText:
//
//  DBI_270: Surface '%1$s' does not begin with alphabetic
//
#define DBI_270                          0x0000054BL

//
// MessageId: DBI_271
//
// MessageText:
//
//  DBI_271: allocating string
//
#define DBI_271                          0x0000054CL

//
// MessageId: DBI_272
//
// MessageText:
//
//  DBI_272: allocating string
//
#define DBI_272                          0x0000054DL

//
// MessageId: DBI_273
//
// MessageText:
//
//  DBI_273: Invalid connect id specified for datbase switch
//
#define DBI_273                          0x0000054EL

//
// MessageId: DBI_274
//
// MessageText:
//
//  DBI_274: missing database name
//
#define DBI_274                          0x0000054FL

//
// MessageId: DBI_275
//
// MessageText:
//
//  DBI_275: Not connected to database
//
#define DBI_275                          0x00000550L

//
// MessageId: DBI_276
//
// MessageText:
//
//  DBI_276: Corrupt plan list in database catalog
//
#define DBI_276                          0x00000551L

//
// MessageId: DBI_277
//
// MessageText:
//
//  DBI_277: Coordinate system information has not been initialized.
//
#define DBI_277                          0x00000552L

//
// MessageId: DBI_278
//
// MessageText:
//
//  DBI_278: Corner points do not define a rectangle
//
#define DBI_278                          0x00000553L

//
// MessageId: DBI_279
//
// MessageText:
//
//  DBI_279: Coordinate system information has not been initialized.
//
#define DBI_279                          0x00000554L

//
// MessageId: DBI_280
//
// MessageText:
//
//  DBI_280: Can't transform from input to output
//
#define DBI_280                          0x00000555L

//
// MessageId: DBI_281
//
// MessageText:
//
//  DBI_281: First character cannot be a blank.
//
#define DBI_281                          0x00000556L

//
// MessageId: DBI_282
//
// MessageText:
//
//  DBI_282: Invalid feature code '%1$s',\nAllowable characters are: alphanumerics, $, _, and blanks.
//
#define DBI_282                          0x00000557L

//
// MessageId: DBI_283
//
// MessageText:
//
//  DBI_283: Network layer %1$d is invalid
//
#define DBI_283                          0x00000558L

//
// MessageId: DBI_284
//
// MessageText:
//
//  DBI_284: Couldn't change extents
//
#define DBI_284                          0x00000559L

//
// MessageId: DBI_285
//
// MessageText:
//
//  DBI_285: Invalid destination specification for extent computation.
//
#define DBI_285                          0x0000055AL

//
// MessageId: DBI_286
//
// MessageText:
//
//  DBI_286: Invalid source specification for extent computation.
//
#define DBI_286                          0x0000055BL

//
// MessageId: DBI_287
//
// MessageText:
//
//  DBI_287: Invalid source specification for extent computation.
//
#define DBI_287                          0x0000055CL

//
// MessageId: DBI_288
//
// MessageText:
//
//  DBI_288: Invalid destination specification for transformation handle.
//
#define DBI_288                          0x0000055DL

//
// MessageId: DBI_289
//
// MessageText:
//
//  DBI_289: Invalid source specification for transformation handle.
//
#define DBI_289                          0x0000055EL

//
// MessageId: DBI_290
//
// MessageText:
//
//  DBI_290: Invalid source specification for transformation handle.
//
#define DBI_290                          0x0000055FL

//
// MessageId: DBI_291
//
// MessageText:
//
//  DBI_291: Invalid destination specification for transformation.
//
#define DBI_291                          0x00000560L

//
// MessageId: DBI_292
//
// MessageText:
//
//  DBI_292: Invalid source specification for transformation.
//
#define DBI_292                          0x00000561L

//
// MessageId: DBI_293
//
// MessageText:
//
//  DBI_293: Invalid source specification for transformation.
//
#define DBI_293                          0x00000562L

//
// MessageId: DBI_294
//
// MessageText:
//
//  DBI_294: missing feat_info_def parameter
//
#define DBI_294                          0x00000563L

//
// MessageId: DBI_295
//
// MessageText:
//
//  DBI_295: Feature %1$ld has unique field(s) ... attribute copy aborted
//
#define DBI_295                          0x00000564L

//
// MessageId: DBI_296
//
// MessageText:
//
//  DBI_296: Feature %1$ld has unique, non-null field(s) ... attribute copy aborted.
//
#define DBI_296                          0x00000565L

//
// MessageId: DBI_297
//
// MessageText:
//
//  DBI_297: Warning:  unique attribute value(s) set to null.
//
#define DBI_297                          0x00000566L

//
// MessageId: DBI_298
//
// MessageText:
//
//  DBI_298: Prefix lock name '%1$s' too long, maximum length is %2$d
//
#define DBI_298                          0x00000567L

//
// MessageId: DBI_299
//
// MessageText:
//
//  DBI_299: Lock name '%1$s' too long, maximum length is %2$d
//
#define DBI_299                          0x00000568L

//
// MessageId: DBI_300
//
// MessageText:
//
//  DBI_300: Invalid lock id %1$d, lock not added
//
#define DBI_300                          0x00000569L

//
// MessageId: DBI_301
//
// MessageText:
//
//  DBI_301: Invalid lock id %1$d, lock not added
//
#define DBI_301                          0x0000056AL

//
// MessageId: DBI_302
//
// MessageText:
//
//  DBI_302: Lock description '%1$s' truncated, maximum length is %2$d
//
#define DBI_302                          0x0000056BL

//
// MessageId: DBI_303
//
// MessageText:
//
//  DBI_303: Processing features
//
#define DBI_303                          0x0000056CL

//
// MessageId: DBI_304
//
// MessageText:
//
//  DBI_304: Cannot find primary table for feature %1$s.
//
#define DBI_304                          0x0000056DL

//
// MessageId: DBI_305
//
// MessageText:
//
//  DBI_305: Polygon-edge relationship found for feature %1$ld, cannot commit.
//
#define DBI_305                          0x0000056EL

//
// MessageId: DBI_306
//
// MessageText:
//
//  DBI_306: Edge-polygon relationship found for feature %1$ld, cannot rollback.
//
#define DBI_306                          0x0000056FL

//
// MessageId: DBI_307
//
// MessageText:
//
//  DBI_307: There are transactions to end ...
//
#define DBI_307                          0x00000570L

//
// MessageId: DBI_308
//
// MessageText:
//
//  DBI_308: Selection table '%1$s' does not have row version %2$d.
//
#define DBI_308                          0x00000571L

//
// MessageId: DBI_309
//
// MessageText:
//
//  DBI_309: Selection table '%1$s' not found, cannot rollback versioned data.
//
#define DBI_309                          0x00000572L

//
// MessageId: DBI_310
//
// MessageText:
//
//  DBI_310: Table '%1$s' does not have a version column.
//
#define DBI_310                          0x00000573L

//
// MessageId: DBI_311
//
// MessageText:
//
//  DBI_311: Table '%1$s' is a primary table, cannot rollback versioned data.
//
#define DBI_311                          0x00000574L

//
// MessageId: DBI_312
//
// MessageText:
//
//  DBI_312: Primary key list table '%1$s' either not found or cannot be used.
//
#define DBI_312                          0x00000575L

//
// MessageId: DBI_313
//
// MessageText:
//
//  DBI_313: Version active in another session - cannot continue version rollback.
//
#define DBI_313                          0x00000576L

//
// MessageId: DBI_314
//
// MessageText:
//
//  DBI_314: Table '%1$s' not found.
//
#define DBI_314                          0x00000577L

//
// MessageId: DBI_315
//
// MessageText:
//
//  DBI_315: missing feat_info_def parameter
//
#define DBI_315                          0x00000578L

//
// MessageId: DBI_316
//
// MessageText:
//
//  DBI_316: Feature %1$ld has unique field(s) ... attribute copy aborted
//
#define DBI_316                          0x00000579L

//
// MessageId: DBI_317
//
// MessageText:
//
//  DBI_317: Feature %1$ld has unique, non-null field(s) ... attribute copy aborted.
//
#define DBI_317                          0x0000057AL

//
// MessageId: DBI_318
//
// MessageText:
//
//  DBI_318: Warning:  unique attribute value(s) set to null.
//
#define DBI_318                          0x0000057BL

//
// MessageId: DBI_319
//
// MessageText:
//
//  DBI_319: Version %1$ld does not exist.
//
#define DBI_319                          0x0000057CL

//
// MessageId: DBI_320
//
// MessageText:
//
//  DBI_320: Insufficient information to report lock conflict.
//
#define DBI_320                          0x0000057DL

//
// MessageId: DBI_321
//
// MessageText:
//
//  DBI_321: Table '%1$s' is a system table, cannot %2$s constraints.
//
#define DBI_321                          0x0000057EL

//
// MessageId: DBI_322
//
// MessageText:
//
//  DBI_322: Table '%1$s' has versioned data, cannot drop constraints.
//
#define DBI_322                          0x0000057FL

//
// MessageId: DBI_323
//
// MessageText:
//
//  DBI_323: Table '%1$s' has shared or discarded locks, cannot %2$s constraints.
//
#define DBI_323                          0x00000580L

//
// MessageId: DBI_324
//
// MessageText:
//
//  DBI_324: Error updating table '%1$s'.
//
#define DBI_324                          0x00000581L

//
// MessageId: DBI_325
//
// MessageText:
//
//  DBI_325: Cannot commit connectivity record, referenced feature %1$d is locked.
//
#define DBI_325                          0x00000582L

//
// MessageId: DBI_326
//
// MessageText:
//
//  DBI_326: Table '%1$s' is a system table, cannot commit versioned data.
//
#define DBI_326                          0x00000583L

//
// MessageId: DBI_327
//
// MessageText:
//
//  DBI_327: Cannot commit connectivity record, referenced feature %1$d is not permanent.
//
#define DBI_327                          0x00000584L

//
// MessageId: DBI_328
//
// MessageText:
//
//  DBI_328: No version defined, cannot continue."
//
#define DBI_328                          0x00000585L

//
// MessageId: DBI_329
//
// MessageText:
//
//  DBI_329: Cannot rollback version 0 data.
//
#define DBI_329                          0x00000586L

//
// MessageId: DBI_330
//
// MessageText:
//
//  DBI_330: Cannot commit version 0 data.
//
#define DBI_330                          0x00000587L

//
// MessageId: DBI_331
//
// MessageText:
//
//  DBI_331: Table '%1$s' does not have primary keys, cannot process shared row lock.
//
#define DBI_331                          0x00000588L

//
// MessageId: DBI_332
//
// MessageText:
//
//  DBI_332: Unable to get next feature \n
//
#define DBI_332                          0x00000589L

//
// MessageId: DBI_333
//
// MessageText:
//
//  DBI_333: Error processing feature\n
//
#define DBI_333                          0x0000058AL

//
// MessageId: DBI_334
//
// MessageText:
//
//  DBI_334: Unable to activate feature %1$d\n
//
#define DBI_334                          0x0000058BL

//
// MessageId: DBI_335
//
// MessageText:
//
//  DBI_335: Unable to close feature %1$d\n
//
#define DBI_335                          0x0000058CL

//
// MessageId: DBI_336
//
// MessageText:
//
//  DBI_336: Error adding entry to '%1$s' for database '%2$s' 
//
#define DBI_336                          0x0000058DL

//
// MessageId: DBI_337
//
// MessageText:
//
//  DBI_337: \nNumber of features with less than %1$d points: %2$d\n
//
#define DBI_337                          0x0000058EL

//
// MessageId: DBI_338
//
// MessageText:
//
//  DBI_338: Unable to reallocate in pool, num pts = %1$d\n
//
#define DBI_338                          0x0000058FL

//
// MessageId: DBI_339
//
// MessageText:
//
//  DBI_339: Unable to reallocate out pool, num pts = %1$d\n
//
#define DBI_339                          0x00000590L

//
// MessageId: DBI_340
//
// MessageText:
//
//  DBI_340: Unable to get a point.\n
//
#define DBI_340                          0x00000591L

//
// MessageId: DBI_341
//
// MessageText:
//
//  DBI_341: Unable to Delete pts in feature\n
//
#define DBI_341                          0x00000592L

//
// MessageId: DBI_342
//
// MessageText:
//
//  DBI_342: Unable to Insert pts in feature.\n
//
#define DBI_342                          0x00000593L

//
// MessageId: DBI_343
//
// MessageText:
//
//  DBI_343: Number of reduced features: %1$d
//
#define DBI_343                          0x00000594L

//
// MessageId: DBI_344
//
// MessageText:
//
//  DBI_344: (%1$s percent of the %2$d features selected)\n
//
#define DBI_344                          0x00000595L

//
// MessageId: DBI_345
//
// MessageText:
//
//  DBI_345: Number of discarded points: %1$d
//
#define DBI_345                          0x00000596L

//
// MessageId: DBI_346
//
// MessageText:
//
//  DBI_346: Unable to get next row in query 
//
#define DBI_346                          0x00000597L

//
// MessageId: DBI_347
//
// MessageText:
//
//  DBI_347: Unable to get the feature information
//
#define DBI_347                          0x00000598L

//
// MessageId: DBI_348
//
// MessageText:
//
//  DBI_348: No features eligible for point reduction. 
//
#define DBI_348                          0x00000599L

//
// MessageId: DBI_349
//
// MessageText:
//
//  DBI_349: Algorithm # of output points exceeds # of input points
//
#define DBI_349                          0x0000059AL

//
// MessageId: DBI_350
//
// MessageText:
//
//  DBI_350: Unable to deactivate query.
//
#define DBI_350                          0x0000059BL

//
// MessageId: DBI_351
//
// MessageText:
//
//  DBI_351: Unable to close database.
//
#define DBI_351                          0x0000059CL

//
// MessageId: DBI_352
//
// MessageText:
//
//  DBI_352: FDO - Feature point/line reduction facility completed.
//
#define DBI_352                          0x0000059DL

//
// MessageId: DBI_353
//
// MessageText:
//
//  DBI_353: Constraint '%1$s' is for table '%2$s', not table '%3$s'.
//
#define DBI_353                          0x0000059EL

//
// MessageId: DBI_354
//
// MessageText:
//
//  DBI_354: Table '%1$s' is a connectivity table, cannot drop foreign key constraint.
//
#define DBI_354                          0x0000059FL

//
// MessageId: DBI_355
//
// MessageText:
//
//  DBI_355: No licenses available. Quitting...
//
#define DBI_355                          0x000005A0L

//
// MessageId: DBI_356
//
// MessageText:
//
//  DBI_356: %1$sMode value 0X%2$X has undefined bits set.
//
#define DBI_356                          0x000005A1L

//
// MessageId: DBI_357
//
// MessageText:
//
//  DBI_357: Arc_step field %1$s is less than 0.0
//
#define DBI_357                          0x000005A2L

//
// MessageId: DBI_358
//
// MessageText:
//
//  DBI_358: Invalid temporary storage specification: %1$s
//
#define DBI_358                          0x000005A3L

//
// MessageId: DBI_359
//
// MessageText:
//
//  DBI_359: Inconsistent area extents specified.
//
#define DBI_359                          0x000005A4L

//
// MessageId: DBI_360
//
// MessageText:
//
//  DBI_360: Invalid value for cell amount.
//
#define DBI_360                          0x000005A5L

//
// MessageId: DBI_361
//
// MessageText:
//
//  DBI_361: Invalid value for cell start."
//
#define DBI_361                          0x000005A6L

//
// MessageId: DBI_362
//
// MessageText:
//
//  DBI_362: Cell start=%1$d while cell amount=0."
//
#define DBI_362                          0x000005A7L

//
// MessageId: DBI_363
//
// MessageText:
//
//  DBI_363: Can't activate edge in polygon network in Direct Version Update mode."
//
#define DBI_363                          0x000005A8L

//
// MessageId: DBI_364
//
// MessageText:
//
//  DBI_364: An object column name from the table '%1$s' must be specified.
//
#define DBI_364                          0x000005A9L

//
// MessageId: DBI_365
//
// MessageText:
//
//  DBI_365: Column '%1$s' must be of type LONG.
//
#define DBI_365                          0x000005AAL

//
// MessageId: DBI_366
//
// MessageText:
//
//  DBI_366: Invalid value for class number: %1$ld.  Must be positive.
//
#define DBI_366                          0x000005ABL

//
// MessageId: DBI_367
//
// MessageText:
//
//  DBI_367: Column '%1$s' in table '%2$s' contains values which are duplicates of existing feature numbers.  This column is not suitable as a primary column for this table.
//
#define DBI_367                          0x000005ACL

//
// MessageId: DBI_368
//
// MessageText:
//
//  DBI_368: NULL Input(s)."
//
#define DBI_368                          0x000005ADL

//
// MessageId: DBI_369
//
// MessageText:
//
//  DBI_369: Expect a TYPE_COLUMN_SPEC node"
//
#define DBI_369                          0x000005AEL

//
// MessageId: DBI_370
//
// MessageText:
//
//  DBI_370: Expect a TYPE_COLUMN_NAME leaf"
//
#define DBI_370                          0x000005AFL

//
// MessageId: DBI_371
//
// MessageText:
//
//  DBI_371: %d 'G_CLASS' columns found, discarded except the last one."
//
#define DBI_371                          0x000005B0L

//
// MessageId: DBI_372
//
// MessageText:
//
//  DBI_372: column name list has more elements than value list.
//
#define DBI_372                          0x000005B1L

//
// MessageId: DBI_373
//
// MessageText:
//
//  DBI_373: A table may have been specified which was converted previously.
//
#define DBI_373                          0x000005B2L

//
// MessageId: DBI_374
//
// MessageText:
//
//  DBI_374: Invalid primary column name specified for class.
//
#define DBI_374                          0x000005B3L

//
// MessageId: DBI_375
//
// MessageText:
//
//  DBI_375: The primary column %1$s has already been specified for this table.\nThe column %2$s will be ignored.
//
#define DBI_375                          0x000005B4L

//
// MessageId: DBI_376
//
// MessageText:
//
//  DBI_376: Object %1$ld was not found.
//
#define DBI_376                          0x000005B5L

//
// MessageId: DBI_377
//
// MessageText:
//
//  DBI_377: Cannot drop all of the classes associated with table %1$s as long as the table still exists.
//
#define DBI_377                          0x000005B6L

//
// MessageId: DBI_378
//
// MessageText:
//
//  DBI_378: Prior to converting a table where a new %1$s column must be created, no transactions should be active.
//
#define DBI_378                          0x000005B7L

//
// MessageId: DBI_379
//
// MessageText:
//
//  DBI_379: The specified column '%1$s' cannot be found in table '%2$s'.
//
#define DBI_379                          0x000005B8L

//
// MessageId: DBI_380
//
// MessageText:
//
//  DBI_380: Illegal null pointer passed to function: %1$s
//
#define DBI_380                          0x000005B9L

//
// MessageId: DBI_381
//
// MessageText:
//
//  DBI_381: An invalid table name (a null pointer or simply an empty string) was passed.
//
#define DBI_381                          0x000005BAL

//
// MessageId: DBI_382
//
// MessageText:
//
//  DBI_382: Cannot find a primary key for table %1$s. Since this is a versioned table, it is necessary to create a primary key on at least one of the table's columns in order to properly create it's first class.
//
#define DBI_382                          0x000005BBL

//
// MessageId: DBI_383
//
// MessageText:
//
//  DBI_383: Insufficient privilege for %1$s %2$s, application %3$s.
//
#define DBI_383                          0x000005BCL

//
// MessageId: DBI_384
//
// MessageText:
//
//  DBI_384: Cannot delete plan %1$ld, it is active to %2$s.
//
#define DBI_384                          0x000005BDL

//
// MessageId: DBI_385
//
// MessageText:
//
//  DBI_385: Error activating plan %1$ld, cannot activate plan.
//
#define DBI_385                          0x000005BEL

//
// MessageId: DBI_386
//
// MessageText:
//
//  DBI_386: Selection table '%1$s' not found, cannot commit versioned data.
//
#define DBI_386                          0x000005BFL

//
// MessageId: DBI_387
//
// MessageText:
//
//  DBI_387: Table '%1$s' is a primary table, cannot commit versioned data.
//
#define DBI_387                          0x000005C0L

//
// MessageId: DBI_388
//
// MessageText:
//
//  DBI_388: Version active in another session - cannot continue version commit.
//
#define DBI_388                          0x000005C1L

//
// MessageId: DBI_389
//
// MessageText:
//
//  DBI_389: Lock '%1$s' is locked by another user, cannot activate lock.
//
#define DBI_389                          0x000005C2L

//
// MessageId: DBI_390
//
// MessageText:
//
//  DBI_390: More than one branch per scenario set specified.
//
#define DBI_390                          0x000005C3L

//
// MessageId: DBI_391
//
// MessageText:
//
//  DBI_391: Table '%1$s': scenario set %2$ld has row(s) in other branch(es).
//
#define DBI_391                          0x000005C4L

//
// MessageId: DBI_392
//
// MessageText:
//
//  DBI_392: Specified version %1$ld is not ancestor of the commited version(s).
//
#define DBI_392                          0x000005C5L

//
// MessageId: DBI_393
//
// MessageText:
//
//  DBI_393: Cannot commit version %1$ld to the same version %2$ld.
//
#define DBI_393                          0x000005C6L

//
// MessageId: DBI_394
//
// MessageText:
//
//  DBI_394: Cannot commit: from_version %1$ld is not only dependend on to_version %2$ld.
//
#define DBI_394                          0x000005C7L

//
// MessageId: DBI_395
//
// MessageText:
//
//  DBI_395: Cannot rollback version. Version dependencies differ from version list.
//
#define DBI_395                          0x000005C8L

//
// MessageId: DBI_396
//
// MessageText:
//
//  DBI_396: Cannot commit version. Version dependencies differ from version list.
//
#define DBI_396                          0x000005C9L

//
// MessageId: DBI_397
//
// MessageText:
//
//  DBI_397: Cannot direct rollback a scenario set version (%1$ld).
//
#define DBI_397                          0x000005CAL

//
// MessageId: DBI_398
//
// MessageText:
//
//  DBI_398: For sdo enabled databases, clean method should be either DELETE_ROWS or TRUNCATE only.
//
#define DBI_398                          0x000005CBL

//
// MessageId: DBI_399
//
// MessageText:
//
//  DBI_399: Plan %1$ld not found in database.
//
#define DBI_399                          0x000005CCL

//
// MessageId: DBI_400
//
// MessageText:
//
//  DBI_400: No geometry table found for plan %1$ld  plan_group %2$ld.
//
#define DBI_400                          0x000005CDL

//
// MessageId: DBI_401
//
// MessageText:
//
//  DBI_401: No geometry table mapping found for plan %1$ld.
//
#define DBI_401                          0x000005CEL

//
// MessageId: DBI_402
//
// MessageText:
//
//  DBI_402: Database is not empty, cannot grow the extents.
//
#define DBI_402                          0x000005CFL

//
// MessageId: DBI_403
//
// MessageText:
//
//  DBI_403: Feature %1$ld is not a line; cannot assign parametric curve geometry."
//
#define DBI_403                          0x000005D0L

//
// MessageId: DBI_404
//
// MessageText:
//
//  DBI_404: No primary key found for table '%1$s'."
//
#define DBI_404                          0x000005D1L

//
// MessageId: DBU_1
//
// MessageText:
//
//  DBU_1: Couldn't create error table for network %1$d.
//
#define DBU_1                            0x000005D2L

//
// MessageId: DBU_2
//
// MessageText:
//
//  DBU_2: Coordinate systems are not logged for the active database.
//
#define DBU_2                            0x000005D3L

//
// MessageId: DBU_3
//
// MessageText:
//
//  DBU_3: Coordinate systems are not logged for the active database.
//
#define DBU_3                            0x000005D4L

//
// MessageId: DBU_4
//
// MessageText:
//
//  DBU_4: Coordinate system is not initialized;  cannot set rectangle
//
#define DBU_4                            0x000005D5L

//
// MessageId: DBU_5
//
// MessageText:
//
//  DBU_5: Cannot set area when input csys differs from output csys.
//
#define DBU_5                            0x000005D6L

//
// MessageId: DBU_6
//
// MessageText:
//
//  DBU_6: due to physical projection limits
//
#define DBU_6                            0x000005D7L

//
// MessageId: DBU_7
//
// MessageText:
//
//  DBU_7: Invalid clipping option (%1$d) given for feature querying
//
#define DBU_7                            0x000005D8L

//
// MessageId: DBU_8
//
// MessageText:
//
//  DBU_8: Couldn't open EXT_PTS with write access.
//
#define DBU_8                            0x000005D9L

//
// MessageId: DBU_9
//
// MessageText:
//
//  DBU_9: %1$d is an unknown field type
//
#define DBU_9                            0x000005DAL

//
// MessageId: DBU_10
//
// MessageText:
//
//  DBU_10: Feature %1$d not found, can't report lock conflict
//
#define DBU_10                           0x000005DBL

//
// MessageId: DBU_11
//
// MessageText:
//
//  DBU_11: Failure in dbu_fdr_next
//
#define DBU_11                           0x000005DCL

//
// MessageId: DBU_12
//
// MessageText:
//
//  DBU_12: Cannot close feature - none active
//
#define DBU_12                           0x000005DDL

//
// MessageId: DBU_13
//
// MessageText:
//
//  DBU_13: Feature %1$ld is locked by another user - cannot delete
//
#define DBU_13                           0x000005DEL

//
// MessageId: DBU_14
//
// MessageText:
//
//  DBU_14: Version Conflict - cannot delete feature %1$ld
//
#define DBU_14                           0x000005DFL

//
// MessageId: DBU_15
//
// MessageText:
//
//  DBU_15: Feature %1$ld is locked by another user - cannot delete
//
#define DBU_15                           0x000005E0L

//
// MessageId: DBU_16
//
// MessageText:
//
//  DBU_16: No replicas found for feature %1$d
//
#define DBU_16                           0x000005E1L

//
// MessageId: DBU_17
//
// MessageText:
//
//  DBU_17: Version Conflict - cannot delete feature %1$ld plan %2$ld
//
#define DBU_17                           0x000005E2L

//
// MessageId: DBU_18
//
// MessageText:
//
//  DBU_18: Interior/Exterior flag must be 'i' or 'e', not '%1$c'
//
#define DBU_18                           0x000005E3L

//
// MessageId: DBU_19
//
// MessageText:
//
//  DBU_19: Feature %1$ld is locked by another user - cannot associate with edge %2$d
//
#define DBU_19                           0x000005E4L

//
// MessageId: DBU_20
//
// MessageText:
//
//  DBU_20: Feature %1$ld is locked by another user - cannot associate with polygon %2$d
//
#define DBU_20                           0x000005E5L

//
// MessageId: DBU_21
//
// MessageText:
//
//  DBU_21: 'Edge' feature %1$ld is not a line feature
//
#define DBU_21                           0x000005E6L

//
// MessageId: DBU_22
//
// MessageText:
//
//  DBU_22: Edge %1$ld and polygon %2$ld are in different networks
//
#define DBU_22                           0x000005E7L

//
// MessageId: DBU_23
//
// MessageText:
//
//  DBU_23: Missing start edge record for edge %1$ld
//
#define DBU_23                           0x000005E8L

//
// MessageId: DBU_24
//
// MessageText:
//
//  DBU_24: Missing end edge record for edge %1$ld
//
#define DBU_24                           0x000005E9L

//
// MessageId: DBU_25
//
// MessageText:
//
//  DBU_25: Polygon %1$ld is already on left side of edge %2$ld, cannot add it to right side
//
#define DBU_25                           0x000005EAL

//
// MessageId: DBU_26
//
// MessageText:
//
//  DBU_26: Polygon %1$ld is already on right side of edge %2$ld, cannot add it to left side
//
#define DBU_26                           0x000005EBL

//
// MessageId: DBU_27
//
// MessageText:
//
//  DBU_27: Edge %1$ld has left polygon %2$ld. Cannot add polygon %3$ld
//
#define DBU_27                           0x000005ECL

//
// MessageId: DBU_28
//
// MessageText:
//
//  DBU_28: Edge %1$ld has right polygon %2$ld. Cannot add polygon %3$ld
//
#define DBU_28                           0x000005EDL

//
// MessageId: DBU_29
//
// MessageText:
//
//  DBU_29: Edge %1$ld is in polygons %2$ld and %3$ld. Cannot add polygon %4$ld
//
#define DBU_29                           0x000005EEL

//
// MessageId: DBU_30
//
// MessageText:
//
//  DBU_30: Invalid number (%1$d) of polygons to clip.
//
#define DBU_30                           0x000005EFL

//
// MessageId: DBU_31
//
// MessageText:
//
//  DBU_31: Unable to complete clipped polygon %1$d
//
#define DBU_31                           0x000005F0L

//
// MessageId: DBU_32
//
// MessageText:
//
//  DBU_32: Feature %1$ld is locked by another user - cannot complete polygon
//
#define DBU_32                           0x000005F1L

//
// MessageId: DBU_33
//
// MessageText:
//
//  DBU_33: Polygon %1$ld is locked by another user - cannot remove edge %2$d
//
#define DBU_33                           0x000005F2L

//
// MessageId: DBU_34
//
// MessageText:
//
//  DBU_34: Feature %1$ld is locked by another user - cannot remove from polygon %2$d
//
#define DBU_34                           0x000005F3L

//
// MessageId: DBU_35
//
// MessageText:
//
//  DBU_35: Missing start edge record for edge %1$ld
//
#define DBU_35                           0x000005F4L

//
// MessageId: DBU_36
//
// MessageText:
//
//  DBU_36: Missing end edge record for edge %1$ld
//
#define DBU_36                           0x000005F5L

//
// MessageId: DBU_37
//
// MessageText:
//
//  DBU_37: Missing polygon feature %1$ld
//
#define DBU_37                           0x000005F6L

//
// MessageId: DBU_39
//
// MessageText:
//
//  DBU_39: %1$s '%2$s'
//
#define DBU_39                           0x000005F7L

//
// MessageId: DBU_40
//
// MessageText:
//
//  DBU_40: %1$s %2$s
//
#define DBU_40                           0x000005F8L

//
// MessageId: DBU_43
//
// MessageText:
//
//  DBU_43: Left/Right flag must be 'l', 'r' or ' ', not '%1$c'
//
#define DBU_43                           0x000005F9L

//
// MessageId: DBU_44
//
// MessageText:
//
//  DBU_44: Buffer offset is too small for database resolution.
//
#define DBU_44                           0x000005FAL

//
// MessageId: DBU_45
//
// MessageText:
//
//  DBU_45: Feature %1$ld could be found for buffer generation
//
#define DBU_45                           0x000005FBL

//
// MessageId: DBU_46
//
// MessageText:
//
//  DBU_46: Feature with number %1$d has no data points, buffer not generated
//
#define DBU_46                           0x000005FCL

//
// MessageId: DBU_47
//
// MessageText:
//
//  DBU_47: Error or interrupt during generation of a buffer.
//
#define DBU_47                           0x000005FDL

//
// MessageId: DBU_48
//
// MessageText:
//
//  DBU_48: Please wait ...
//
#define DBU_48                           0x000005FEL

//
// MessageId: DBU_49
//
// MessageText:
//
//  DBU_49: Feature %1$ld could be found for buffer generation
//
#define DBU_49                           0x000005FFL

//
// MessageId: DBU_50
//
// MessageText:
//
//  DBU_50: Column %1$s does not exist in table %2$s.
//
#define DBU_50                           0x00000600L

//
// MessageId: DBU_51
//
// MessageText:
//
//  DBU_51: Shared lock table for table %1$s does not exist.
//
#define DBU_51                           0x00000601L

//
// MessageId: DBU_52
//
// MessageText:
//
//  DBU_52: Feature %1$ld not found, cannot lock feature.
//
#define DBU_52                           0x00000602L

//
// MessageId: DBU_53
//
// MessageText:
//
//  DBU_53: Lock number %1$ld does not exist.
//
#define DBU_53                           0x00000603L

//
// MessageId: DBU_54
//
// MessageText:
//
//  DBU_54: Invalid locking request.
//
#define DBU_54                           0x00000604L

//
// MessageId: DBU_55
//
// MessageText:
//
//  DBU_55: Feature %1$ld not found.
//
#define DBU_55                           0x00000605L

//
// MessageId: DBU_56
//
// MessageText:
//
//  DBU_56: Lock %1$ld has an invalid type
//
#define DBU_56                           0x00000606L

//
// MessageId: DBU_57
//
// MessageText:
//
//  DBU_57: Lock %1$ld not found, cannot return row lock type.
//
#define DBU_57                           0x00000607L

//
// MessageId: DBU_58
//
// MessageText:
//
//  DBU_58: unable to allocate dbu_shtbl_cache array, NULL pointer returned.
//
#define DBU_58                           0x00000608L

//
// MessageId: DBU_59
//
// MessageText:
//
//  DBU_59: unable to get row from the cache.
//
#define DBU_59                           0x00000609L

//
// MessageId: DBU_60
//
// MessageText:
//
//  DBU_60: Append to dbu_shtbl_cache failed.
//
#define DBU_60                           0x0000060AL

//
// MessageId: DBU_61
//
// MessageText:
//
//  DBU_61: Shared lock table for table %1$s does not exist.
//
#define DBU_61                           0x0000060BL

//
// MessageId: DBU_62
//
// MessageText:
//
//  DBU_62: Lock number %1$ld does not exist.
//
#define DBU_62                           0x0000060CL

//
// MessageId: DBU_63
//
// MessageText:
//
//  DBU_63: Failed to find table %1$s, cannot continue.
//
#define DBU_63                           0x0000060DL

//
// MessageId: DBU_64
//
// MessageText:
//
//  DBU_64: Invalid lock type for table '%1$s'.
//
#define DBU_64                           0x0000060EL

//
// MessageId: DBU_65
//
// MessageText:
//
//  DBU_65: Shared lock table for table %1$s does not exist.
//
#define DBU_65                           0x0000060FL

//
// MessageId: DBU_66
//
// MessageText:
//
//  DBU_66: Plan %1$ld not found, cannot lock feature.
//
#define DBU_66                           0x00000610L

//
// MessageId: DBU_67
//
// MessageText:
//
//  DBU_67:  Invalid locking request
//
#define DBU_67                           0x00000611L

//
// MessageId: DBU_68
//
// MessageText:
//
//  DBU_68: Cannot add share lock for lock %1$ld
//
#define DBU_68                           0x00000612L

//
// MessageId: DBU_69
//
// MessageText:
//
//  DBU_69: Shared lock table for table %1$s does not exist, cannot unlock lock %2$ld.
//
#define DBU_69                           0x00000613L

//
// MessageId: DBU_70
//
// MessageText:
//
//  DBU_70: Internal error: %1$s - invalid type parameter '%2$s' found, commit cycle not returned.
//
#define DBU_70                           0x00000614L

//
// MessageId: DBU_71
//
// MessageText:
//
//  DBU_71: Format error for environment variable GVC_COMMIT_CYCLE `%1$s'.
//
#define DBU_71                           0x00000615L

//
// MessageId: DBU_72
//
// MessageText:
//
//  DBU_72: Format is GVC_COMMIT_CYCLE [feature] [polygon] [indirect] [feat_sel] [aux].
//
#define DBU_72                           0x00000616L

//
// MessageId: DBU_73
//
// MessageText:
//
//  DBU_73: \twhere feature, polygon, indirect, feat_sel and aux are >= 0.
//
#define DBU_73                           0x00000617L

//
// MessageId: DBU_74
//
// MessageText:
//
//  DBU_74: %1$s  >> commit cycle is %2$s
//
#define DBU_74                           0x00000618L

//
// MessageId: DBU_75
//
// MessageText:
//
//  DBU_75: %1$s  >> commit cycle is %2$s.\n\t\t\t\t(Environment commit cycle %3$d ignored)
//
#define DBU_75                           0x00000619L

//
// MessageId: DBU_76
//
// MessageText:
//
//  DBU_76: Internal error: %1$s - invalid type parameter '%2$s' found, commit cycle not returned.
//
#define DBU_76                           0x0000061AL

//
// MessageId: DBU_77
//
// MessageText:
//
//  DBU_77: Format error for environment variable VISION_COMMIT_CYCLE `%1$s'.
//
#define DBU_77                           0x0000061BL

//
// MessageId: DBU_78
//
// MessageText:
//
//  DBU_78: Format is VISION_COMMIT_CYCLE [feature] [polygon] [indirect] [feat_sel] [aux].
//
#define DBU_78                           0x0000061CL

//
// MessageId: DBU_79
//
// MessageText:
//
//  DBU_79: \twhere feature, polygon, indirect, feat_sel and aux are >= 0.
//
#define DBU_79                           0x0000061DL

//
// MessageId: DBU_80
//
// MessageText:
//
//  DBU_80: 'Auto Commit'
//
#define DBU_80                           0x0000061EL

//
// MessageId: DBU_81
//
// MessageText:
//
//  DBU_81: %1$s  >> commit cycle is %2$s
//
#define DBU_81                           0x0000061FL

//
// MessageId: DBU_82
//
// MessageText:
//
//  DBU_82: %1$s  >> commit cycle is %2$s.\n\t\t\t\t(Environment commit cycle %3$d ignored)
//
#define DBU_82                           0x00000620L

//
// MessageId: DBU_83
//
// MessageText:
//
//  DBU_83: Selection table is missing.
//
#define DBU_83                           0x00000621L

//
// MessageId: DBU_84
//
// MessageText:
//
//  DBU_84: Selection table name `%1$s' is too long.
//
#define DBU_84                           0x00000622L

//
// MessageId: DBU_85
//
// MessageText:
//
//  DBU_85: Selection table `%1$s' is not found.
//
#define DBU_85                           0x00000623L

//
// MessageId: DBU_86
//
// MessageText:
//
//  DBU_86: Table %1$s has no keys.
//
#define DBU_86                           0x00000624L

//
// MessageId: DBU_87
//
// MessageText:
//
//  DBU_87: Table %1$s has no primary key.
//
#define DBU_87                           0x00000625L

//
// MessageId: DBU_88
//
// MessageText:
//
//  DBU_88: %1$s table is missing in database
//
#define DBU_88                           0x00000626L

//
// MessageId: DBU_89
//
// MessageText:
//
//  DBU_89: Connectivity definition '%1$s' for network %2$d not found in Content Directory
//
#define DBU_89                           0x00000627L

//
// MessageId: DBU_90
//
// MessageText:
//
//  DBU_90: Can't find %1$s column in table %2$s.
//
#define DBU_90                           0x00000628L

//
// MessageId: DBU_91
//
// MessageText:
//
//  DBU_91: Table %1$s not found.
//
#define DBU_91                           0x00000629L

//
// MessageId: DBU_92
//
// MessageText:
//
//  DBU_92: Internal error: Can't match primary and foreign keys for table %1$s
//
#define DBU_92                           0x0000062AL

//
// MessageId: DBU_93
//
// MessageText:
//
//  DBU_93: Lock conflict: table %1$s, request %2$s, active lock name `%3$s'
//
#define DBU_93                           0x0000062BL

//
// MessageId: DBU_94
//
// MessageText:
//
//  DBU_94: Version conflict: table %1$s, request %2$s, active version name `%3$s'
//
#define DBU_94                           0x0000062CL

//
// MessageId: DBU_95
//
// MessageText:
//
//  DBU_95: conflict row: conflict %1$s, lock name `%2$s', primary key:
//
#define DBU_95                           0x0000062DL

//
// MessageId: DBU_96
//
// MessageText:
//
//  DBU_96: conflict row: conflict %1$s, lock name `%2$s'
//
#define DBU_96                           0x0000062EL

//
// MessageId: DBU_97
//
// MessageText:
//
//  DBU_97: conflict: conflict %1$s, primary key:
//
#define DBU_97                           0x0000062FL

//
// MessageId: DBU_98
//
// MessageText:
//
//  DBU_98: conflict: conflict %1$s
//
#define DBU_98                           0x00000630L

//
// MessageId: DBU_99
//
// MessageText:
//
//  DBU_99: UPDATE EXCLUSIVE TRANSACTION LOCK
//
#define DBU_99                           0x00000631L

//
// MessageId: DBU_100
//
// MessageText:
//
//  DBU_100: EXCLUSIVE ROW LOCK
//
#define DBU_100                          0x00000632L

//
// MessageId: DBU_101
//
// MessageText:
//
//  DBU_101: VERSION EXCLUSIVE ROW LOCK
//
#define DBU_101                          0x00000633L

//
// MessageId: DBU_102
//
// MessageText:
//
//  DBU_102: SHARED ROW LOCK
//
#define DBU_102                          0x00000634L

//
// MessageId: DBU_103
//
// MessageText:
//
//  DBU_103: SHARED TO EXCLUSIVE LOCK ESCALATION
//
#define DBU_103                          0x00000635L

//
// MessageId: DBU_104
//
// MessageText:
//
//  DBU_104: SHARED TO VERSION EXCLUSIVE LOCK ESCALATION
//
#define DBU_104                          0x00000636L

//
// MessageId: DBU_105
//
// MessageText:
//
//  DBU_105: REMOVE LOCK
//
#define DBU_105                          0x00000637L

//
// MessageId: DBU_106
//
// MessageText:
//
//  DBU_106: NO SHARED LOCK TO ESCALATE
//
#define DBU_106                          0x00000638L

//
// MessageId: DBU_107
//
// MessageText:
//
//  DBU_107: NO CONFLICT
//
#define DBU_107                          0x00000639L

//
// MessageId: DBU_108
//
// MessageText:
//
//  DBU_108: NO LOCK
//
#define DBU_108                          0x0000063AL

//
// MessageId: DBU_109
//
// MessageText:
//
//  DBU_109: NO PRIMARY KEY
//
#define DBU_109                          0x0000063BL

//
// MessageId: DBU_110
//
// MessageText:
//
//  DBU_110: INVALID LOCK TYPE
//
#define DBU_110                          0x0000063CL

//
// MessageId: DBU_111
//
// MessageText:
//
//  DBU_111: NO ACTIVE LOCK
//
#define DBU_111                          0x0000063DL

//
// MessageId: DBU_112
//
// MessageText:
//
//  DBU_112: CONDITIONAL INSERT
//
#define DBU_112                          0x0000063EL

//
// MessageId: DBU_113
//
// MessageText:
//
//  DBU_113: CONDITIONAL UPDATE
//
#define DBU_113                          0x0000063FL

//
// MessageId: DBU_114
//
// MessageText:
//
//  DBU_114: CONDITIONAL DELETE
//
#define DBU_114                          0x00000640L

//
// MessageId: DBU_115
//
// MessageText:
//
//  DBU_115: G_NEXT_VERSION NOT NULL
//
#define DBU_115                          0x00000641L

//
// MessageId: DBU_116
//
// MessageText:
//
//  DBU_116: ILLEGAL DEPENDENCY
//
#define DBU_116                          0x00000642L

//
// MessageId: DBU_117
//
// MessageText:
//
//  DBU_117: ROW EXISTS
//
#define DBU_117                          0x00000643L

//
// MessageId: DBU_118
//
// MessageText:
//
//  DBU_118: VERSION DOESN'T EXIST
//
#define DBU_118                          0x00000644L

//
// MessageId: DBU_119
//
// MessageText:
//
//  DBU_119: PARENT AND CHILD VERSIONS ARE THE SAME
//
#define DBU_119                          0x00000645L

//
// MessageId: DBU_120
//
// MessageText:
//
//  DBU_120: Unknown lock or version request or conflict.
//
#define DBU_120                          0x00000646L

//
// MessageId: DBU_121
//
// MessageText:
//
//  DBU_121: BLOB type (unprintable)
//
#define DBU_121                          0x00000647L

//
// MessageId: DBU_122
//
// MessageText:
//
//  DBU_122: RAW type (unprintable)
//
#define DBU_122                          0x00000648L

//
// MessageId: DBU_123
//
// MessageText:
//
//  DBU_123: Unknown Type
//
#define DBU_123                          0x00000649L

//
// MessageId: DBU_124
//
// MessageText:
//
//  DBU_124: Keytable `%s' was not dropped.
//
#define DBU_124                          0x0000064AL

//
// MessageId: DBU_125
//
// MessageText:
//
//  DBU_125: Panic: memory corrupted while quoting field `%1$s'
//
#define DBU_125                          0x0000064BL

//
// MessageId: DBU_126
//
// MessageText:
//
//  DBU_126: Column '%1$s' does not exist in table '%2$s', can't report lock conflict.
//
#define DBU_126                          0x0000064CL

//
// MessageId: DBU_127
//
// MessageText:
//
//  DBU_127: Insufficient information to report lock conflict.
//
#define DBU_127                          0x0000064DL

//
// MessageId: DBU_128
//
// MessageText:
//
//  DBU_128: Row Processed %1$d.
//
#define DBU_128                          0x0000064EL

//
// MessageId: DBU_129
//
// MessageText:
//
//  DBU_129: EXCLUSIVE TO SHARED LOCK DOWNGRADE
//
#define DBU_129                          0x0000064FL

//
// MessageId: DBU_130
//
// MessageText:
//
//  DBU_130: VERSION EXCLUSIVE TO SHARED LOCK DOWNGRADE
//
#define DBU_130                          0x00000650L

//
// MessageId: DBU_131
//
// MessageText:
//
//  DBU_131: COMMIT LOCK
//
#define DBU_131                          0x00000651L

//
// MessageId: DBU_132
//
// MessageText:
//
//  DBU_132: Invalid lock type '$1%c', expected type '$2%c'.
//
#define DBU_132                          0x00000652L

//
// MessageId: DBU_133
//
// MessageText:
//
//  DBU_133: Invalid shared lock table name '$1%s' in '$2%s'.
//
#define DBU_133                          0x00000653L

//
// MessageId: DBU_134
//
// MessageText:
//
//  DBU_134: '$1%s' is empty.
//
#define DBU_134                          0x00000654L

//
// MessageId: DBU_135
//
// MessageText:
//
//  DBU_135: $1%sfeat_num %2$ld not found.
//
#define DBU_135                          0x00000655L

//
// MessageId: DBU_136
//
// MessageText:
//
//  DBU_136: %1$sInvalid point position (%2$ld) for feature %3$ld.
//
#define DBU_136                          0x00000656L

//
// MessageId: DBU_137
//
// MessageText:
//
//  DBU_137: %1$s%2$s is a null pointer.
//
#define DBU_137                          0x00000657L

//
// MessageId: DBU_138
//
// MessageText:
//
//  DBU_138: %1$sSegment %2$ld is not in feature %3$ld.
//
#define DBU_138                          0x00000658L

//
// MessageId: DBU_139
//
// MessageText:
//
//  DBU_139: $1%sInvalid curve type %2$d.
//
#define DBU_139                          0x00000659L

//
// MessageId: DBU_140
//
// MessageText:
//
//  DBU_140: CAN'T AUTOVERSION IN DIRECT UPDATE MODE
//
#define DBU_140                          0x0000065AL

//
// MessageId: DBU_141
//
// MessageText:
//
//  DBU_141: Can't add edge to polygon in Direct Version Update mode.
//
#define DBU_141                          0x0000065BL

//
// MessageId: DBU_142
//
// MessageText:
//
//  DBU_142: Edge %1$ld not associated.
//
#define DBU_142                          0x0000065CL

//
// MessageId: DBU_143
//
// MessageText:
//
//  DBU_143: Can't remove edge from polygon in Direct Version Update mode.
//
#define DBU_143                          0x0000065DL

//
// MessageId: DBU_144
//
// MessageText:
//
//  DBU_144: Edge %1$ld not removed.
//
#define DBU_144                          0x0000065EL

//
// MessageId: DBU_145
//
// MessageText:
//
//  DBU_145: Either a class value or a class name must be specified.
//
#define DBU_145                          0x0000065FL

//
// MessageId: DBU_146
//
// MessageText:
//
//  DBU_146: Class %1$ld is not associated with table %2$s.
//
#define DBU_146                          0x00000660L

//
// MessageId: DBU_147
//
// MessageText:
//
//  DBU_147: Class %1$ld does not exist.
//
#define DBU_147                          0x00000661L

//
// MessageId: DS_1
//
// MessageText:
//
//  DS_1: ds/arr/get_next: Invalid start address
//
#define DS_1                             0x00000662L

//
// MessageId: DS_2
//
// MessageText:
//
//  DS_2: ds/arr/insert: Invalid input
//
#define DS_2                             0x00000663L

//
// MessageId: DS_3
//
// MessageText:
//
//  DS_3: ds/arr/insert: Bad address (index: %1$d)
//
#define DS_3                             0x00000664L

//
// MessageId: DS_4
//
// MessageText:
//
//  DS_4: ds/arr/sort: Can't initialize the heap
//
#define DS_4                             0x00000665L

//
// MessageId: DS_5
//
// MessageText:
//
//  DS_5: ds_arr_unique: sort failed
//
#define DS_5                             0x00000666L

//
// MessageId: DS_6
//
// MessageText:
//
//  DS_6: %1$s: Invalid input
//
#define DS_6                             0x00000667L

//
// MessageId: DS_7
//
// MessageText:
//
//  DS_7: ds_buf_lsearch: buffer is corrupted (size: %1$d)
//
#define DS_7                             0x00000668L

//
// MessageId: DS_8
//
// MessageText:
//
//  DS_8: ds_buf_sort:  Can't initialize the heap
//
#define DS_8                             0x00000669L

//
// MessageId: DS_9
//
// MessageText:
//
//  DS_9: %1$s: Can't allocate a new block for %2$s
//
#define DS_9                             0x0000066AL

//
// MessageId: DS_10
//
// MessageText:
//
//  DS_10: %1$s: Can't allocate memory pool for %2$s
//
#define DS_10                            0x0000066BL

//
// MessageId: DS_11
//
// MessageText:
//
//  DS_11: ds_list_blk_uniq: block is not sorted.
//
#define DS_11                            0x0000066CL

//
// MessageId: DS_12
//
// MessageText:
//
//  DS_12: List %1$s is not a new list - unique request is ignored
//
#define DS_12                            0x0000066DL

//
// MessageId: DS_13
//
// MessageText:
//
//  DS_13: %1$s: NULL list is supplied
//
#define DS_13                            0x0000066EL

//
// MessageId: DS_14
//
// MessageText:
//
//  DS_14: %1$s: bad magic number
//
#define DS_14                            0x0000066FL

//
// MessageId: DS_15
//
// MessageText:
//
//  DS_15: %1$s: delete operation for (%2$s) is not available yet\n
//
#define DS_15                            0x00000670L

//
// MessageId: DS_16
//
// MessageText:
//
//  DS_16: ds_strf_check: Supplied string `%1$s' is shorter than %2$d characters
//
#define DS_16                            0x00000671L

//
// MessageId: DS_25
//
// MessageText:
//
//  DS_25: ds_xref_bucket_add: merge failed
//
#define DS_25                            0x00000672L

//
// MessageId: DS_26
//
// MessageText:
//
//  DS_26: WARNING SOURCE POOL IS NOT SORTED bucket\n
//
#define DS_26                            0x00000673L

//
// MessageId: DS_27
//
// MessageText:
//
//  DS_27: ds_xref_bucket_split failed
//
#define DS_27                            0x00000674L

//
// MessageId: DS_28
//
// MessageText:
//
//  DS_28: Cross reference (0x%1$x) address mismatch: id: %2$d 0x%3$x 0x%4$x
//
#define DS_28                            0x00000675L

//
// MessageId: DS_29
//
// MessageText:
//
//  DS_29: %1$s: identified type is not valid
//
#define DS_29                            0x00000676L

//
// MessageId: DS_30
//
// MessageText:
//
//  DS_30: Incorrect size must be %1$d is %2$s\n
//
#define DS_30                            0x00000677L

//
// MessageId: DS_31
//
// MessageText:
//
//  DS_31: Incorrect MIN MAX ...................... \n
//
#define DS_31                            0x00000678L

//
// MessageId: DS_32
//
// MessageText:
//
//  DS_32: %1$s failed
//
#define DS_32                            0x00000679L

//
// MessageId: EH_1
//
// MessageText:
//
//  EH_1: Invalid context item was specified
//
#define EH_1                             0x0000067AL

//
// MessageId: EH_2
//
// MessageText:
//
//  EH_2: Error while writing out context data
//
#define EH_2                             0x0000067BL

//
// MessageId: EH_3
//
// MessageText:
//
//  EH_3: %1$sError Id:%2$5d  Package Id: %3$d %4$s
//
#define EH_3                             0x0000067CL

//
// MessageId: EH_4
//
// MessageText:
//
//  EH_4: %1$s%2$s error %3$d
//
#define EH_4                             0x0000067DL

//
// MessageId: EH_5
//
// MessageText:
//
//  EH_5: %1$sClass: db"                    
//
#define EH_5                             0x0000067EL

//
// MessageId: EH_6
//
// MessageText:
//
//  EH_6: %1$sClass: os"                    
//
#define EH_6                             0x0000067FL

//
// MessageId: EH_7
//
// MessageText:
//
//  EH_7: %1$sClass: warning"               
//
#define EH_7                             0x00000680L

//
// MessageId: EH_8
//
// MessageText:
//
//  EH_8: %1$sClass: regular"               
//
#define EH_8                             0x00000681L

//
// MessageId: EH_9
//
// MessageText:
//
//  EH_9: %1$sClass: critical"              
//
#define EH_9                             0x00000682L

//
// MessageId: EH_10
//
// MessageText:
//
//  EH_10: %1$sClass: file"                  
//
#define EH_10                            0x00000683L

//
// MessageId: EH_11
//
// MessageText:
//
//  EH_11: %1$sClass: outside of database extents
//
#define EH_11                            0x00000684L

//
// MessageId: EH_12
//
// MessageText:
//
//  EH_12: %1$sClass: unrecognized class"    
//
#define EH_12                            0x00000685L

//
// MessageId: EH_13
//
// MessageText:
//
//  EH_13: %1$sEmbedded Trail
//
#define EH_13                            0x00000686L

//
// MessageId: EH_14
//
// MessageText:
//
//  EH_14: %1$sEnd of embedded trail
//
#define EH_14                            0x00000687L

//
// MessageId: EH_15
//
// MessageText:
//
//  EH_15: MAIN ERROR TRAIL:
//
#define EH_15                            0x00000688L

//
// MessageId: EH_16
//
// MessageText:
//
//  EH_16: -----------------
//
#define EH_16                            0x00000689L

//
// MessageId: EH_17
//
// MessageText:
//
//  EH_17: END OF MAIN TRAIL
//
#define EH_17                            0x0000068AL

//
// MessageId: EH_18
//
// MessageText:
//
//  EH_18: %1$sContext: \n%2$s
//
#define EH_18                            0x0000068BL

//
// MessageId: EH_19
//
// MessageText:
//
//  EH_19: Cannot open log file for writing: %1$s
//
#define EH_19                            0x0000068CL

//
// MessageId: EH_20
//
// MessageText:
//
//  EH_20: Cannot close log file
//
#define EH_20                            0x0000068DL

//
// MessageId: EH_21
//
// MessageText:
//
//  EH_21: VISION_ERRTRAIL_LIMIT should be at least %1$d. All errors were erased.
//
#define EH_21                            0x0000068EL

//
// MessageId: EH_22
//
// MessageText:
//
//  EH_22: %1$s
//
#define EH_22                            0x0000068FL

//
// MessageId: MSG_1
//
// MessageText:
//
//  MSG_1: Following message truncated: 
//
#define MSG_1                            0x00000690L

//
// MessageId: MSG_2
//
// MessageText:
//
//  MSG_2: Unknown error
//
#define MSG_2                            0x00000691L

//
// MessageId: MSG_3
//
// MessageText:
//
//  MSG_3: System error in %1$s: %2$s (code %3$d)
//
#define MSG_3                            0x00000692L

//
// MessageId: MSG_4
//
// MessageText:
//
//  MSG_4: %1$s: Error in field value type for %2$s.\n
//
#define MSG_4                            0x00000693L

//
// MessageId: MSG_5
//
// MessageText:
//
//  MSG_5: gvc_gvmp: unable to load font
//
#define MSG_5                            0x00000694L

//
// MessageId: NLS_1
//
// MessageText:
//
//  NLS_1: environment variable `%1$s' is invalid
//
#define NLS_1                            0x00000695L

//
// MessageId: NLS_2
//
// MessageText:
//
//  NLS_2: environment variable `LANG' is undefined.
//
#define NLS_2                            0x00000696L

//
// MessageId: NLS_3
//
// MessageText:
//
//  NLS_3: environment variable `LANG' is invalid.
//
#define NLS_3                            0x00000697L

//
// MessageId: NLS_4
//
// MessageText:
//
//  NLS_4: memory corrupted while formating a message.
//
#define NLS_4                            0x00000698L

//
// MessageId: NLS_5
//
// MessageText:
//
//  NLS_5: probably caused by a invalid externalised message.
//
#define NLS_5                            0x00000699L

//
// MessageId: NLS_6
//
// MessageText:
//
//  NLS_6: cannot set locale because ...
//
#define NLS_6                            0x0000069AL

//
// MessageId: OCIDR_1
//
// MessageText:
//
//  OCIDR_1: Cursor not found.
//
#define OCIDR_1                          0x000006DAL

//
// MessageId: RDBI_1
//
// MessageText:
//
//  RDBI_1: Disconnecting while transactions are outstanding...
//
#define RDBI_1                           0x000006DBL

//
// MessageId: RDBI_2
//
// MessageText:
//
//  RDBI_2: ...[%1$s:'%2$s']
//
#define RDBI_2                           0x000006DCL

//
// MessageId: RDBI_3
//
// MessageText:
//
//  RDBI_3: RDBMS: Normal, successful completion.
//
#define RDBI_3                           0x000006DDL

//
// MessageId: RDBI_4
//
// MessageText:
//
//  RDBI_4: RDBMS: Memory allocation failure.
//
#define RDBI_4                           0x000006DEL

//
// MessageId: RDBI_5
//
// MessageText:
//
//  RDBI_5: RDBMS: Table or view already exists.
//
#define RDBI_5                           0x000006DFL

//
// MessageId: RDBI_6
//
// MessageText:
//
//  RDBI_6: RDBMS: Too many connections active - can't establish another.
//
#define RDBI_6                           0x000006E0L

//
// MessageId: RDBI_7
//
// MessageText:
//
//  RDBI_7: RDBMS: Cannot allocate another cursor. Must free one first.
//
#define RDBI_7                           0x000006E1L

//
// MessageId: RDBI_8
//
// MessageText:
//
//  RDBI_8: RDBMS: Select expression or bind variable does not exist.
//
#define RDBI_8                           0x000006E2L

//
// MessageId: RDBI_9
//
// MessageText:
//
//  RDBI_9: RDBMS: Either no row has been fetched, or this is not a 'select for update'.
//
#define RDBI_9                           0x000006E3L

//
// MessageId: RDBI_10
//
// MessageText:
//
//  RDBI_10: RDBMS: Not connected to the specified database.
//
#define RDBI_10                          0x000006E4L

//
// MessageId: RDBI_11
//
// MessageText:
//
//  RDBI_11: RDBMS: SQL syntax error at line %1$d, near '%2$.40s'.
//
#define RDBI_11                          0x000006E5L

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
//  RDBI_13: RDBMS: Lock conflict with another user
//
#define RDBI_13                          0x000006E7L

//
// MessageId: RDBI_14
//
// MessageText:
//
//  RDBI_14: RDBMS: Password file has wrong entry
//
#define RDBI_14                          0x000006E8L

//
// MessageId: RDBI_15
//
// MessageText:
//
//  RDBI_15: RDBMS: You must log in as VISION_OWNER to run this application on VMS
//
#define RDBI_15                          0x000006E9L

//
// MessageId: RDBI_16
//
// MessageText:
//
//  RDBI_16: RDBMS: Failed to locate cursor referenced by current of cursor clause
//
#define RDBI_16                          0x000006EAL

//
// MessageId: RDBI_17
//
// MessageText:
//
//  RDBI_17: RDBMS: Cursor referenced by current of cursor clause must be a for update cursor
//
#define RDBI_17                          0x000006EBL

//
// MessageId: RDBI_19
//
// MessageText:
//
//  RDBI_19: Transaction id: [%1$s] should have stopped sooner
//
#define RDBI_19                          0x000006ECL

//
// MessageId: RDBI_20
//
// MessageText:
//
//  RDBI_20: Illegal (null or empty) transaction id.
//
#define RDBI_20                          0x000006EDL

//
// MessageId: RDBI_21
//
// MessageText:
//
//  RDBI_21: No current database is open; cannot begin transaction '%1$s' .
//
#define RDBI_21                          0x000006EEL

//
// MessageId: RDBI_22
//
// MessageText:
//
//  RDBI_22: Illegal (null or empty) transaction id.
//
#define RDBI_22                          0x000006EFL

//
// MessageId: RDBI_23
//
// MessageText:
//
//  RDBI_23: There are no transactions to end. ['%1$s']
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
//  RDBI_25: RDBMS: Incompatible column type
//
#define RDBI_25                          0x000006F2L

//
// MessageId: RDBI_26
//
// MessageText:
//
//  RDBI_26: RDBMS: End-of-fetch reached
//
#define RDBI_26                          0x000006F3L

//
// MessageId: RDBI_27
//
// MessageText:
//
//  RDBI_27: RDBMS: Failed to locate current of <cursor> clause in SQL statement
//
#define RDBI_27                          0x000006F4L

//
// MessageId: RDBI_28
//
// MessageText:
//
//  RDBI_28: Transaction id: [%1$s] should have stopped sooner.
//
#define RDBI_28                          0x000006F5L

//
// MessageId: RDBI_29
//
// MessageText:
//
//  RDBI_29: Transaction nesting error. %1$c%2$s%3$s%4$s%5$c
//
#define RDBI_29                          0x000006F6L

//
// MessageId: RDBI_30
//
// MessageText:
//
//  RDBI_30: No current database is open; cannot free cursor %1$d
//
#define RDBI_30                          0x000006F7L

//
// MessageId: RDBI_31
//
// MessageText:
//
//  RDBI_31: Transaction id: [%1$s] should have stopped sooner
//
#define RDBI_31                          0x000006F8L

//
// MessageId: SDB_1
//
// MessageText:
//
//  SDB_1: Failed to activate feature %1$d; feature is locked.
//
#define SDB_1                            0x000006F9L

//
// MessageId: SDB_2
//
// MessageText:
//
//  SDB_2: Failed to activate feature %1$d; feature is already active.
//
#define SDB_2                            0x000006FAL

//
// MessageId: SDB_3
//
// MessageText:
//
//  SDB_3: Label point %1$d for current feature is outside the database extent; label not added.
//
#define SDB_3                            0x000006FBL

//
// MessageId: SDB_4
//
// MessageText:
//
//  SDB_4: Warning: expecting an angle (in radians) between 0.0 and 2PI %1$s, received %2$s for label number %3$d of feature %4$ld, converted to %5$s, continuing.
//
#define SDB_4                            0x000006FCL

//
// MessageId: SDB_5
//
// MessageText:
//
//  SDB_5: Input point at position %1$d is outside the database extent, point ignored.
//
#define SDB_5                            0x000006FDL

//
// MessageId: SDB_6
//
// MessageText:
//
//  SDB_6: Resolution of %1$s is too large for the given extent.
//
#define SDB_6                            0x000006FEL

//
// MessageId: SDB_7
//
// MessageText:
//
//  SDB_7: Resolution of %1$s is too fine for the given extent.
//
#define SDB_7                            0x000006FFL

//
// MessageId: SDB_8
//
// MessageText:
//
//  SDB_8: %1$s
//
#define SDB_8                            0x00000700L

//
// MessageId: SDB_9
//
// MessageText:
//
//  SDB_9: Approximate best resolution is %1$s .
//
#define SDB_9                            0x00000701L

//
// MessageId: SDB_10
//
// MessageText:
//
//  SDB_10: Input point for current feature at position %1$d is outside the database extent,\npoint ignored
//
#define SDB_10                           0x00000702L

//
// MessageId: SDB_11
//
// MessageText:
//
//  SDB_11: Feature %1$ld not found.
//
#define SDB_11                           0x00000703L

//
// MessageId: SDB_12
//
// MessageText:
//
//  SDB_12: Feature %1$ld not found.
//
#define SDB_12                           0x00000704L

//
// MessageId: SDB_13
//
// MessageText:
//
//  SDB_13: Feature %1$ld is active, cannot delete it.
//
#define SDB_13                           0x00000705L

//
// MessageId: SDB_14
//
// MessageText:
//
//  SDB_14: Feature %1$ld is locked, cannot delete it.
//
#define SDB_14                           0x00000706L

//
// MessageId: SDB_15
//
// MessageText:
//
//  SDB_15: Spatial database is not initialized for measurement.
//
#define SDB_15                           0x00000707L

//
// MessageId: SDB_16
//
// MessageText:
//
//  SDB_16: Feature %1$ld not found
//
#define SDB_16                           0x00000708L

//
// MessageId: SDB_17
//
// MessageText:
//
//  SDB_17: Version conflict: can't update edge info for feature %1$d
//
#define SDB_17                           0x00000709L

//
// MessageId: SDB_18
//
// MessageText:
//
//  SDB_18: Lock Conflict: Can't update relationship between polygon %1$ld and edge %2$ld
//
#define SDB_18                           0x0000070AL

//
// MessageId: SDB_19
//
// MessageText:
//
//  SDB_19: Version Conflict: Can't update relationship between polygon %1$ld and edge %2$ld .
//
#define SDB_19                           0x0000070BL

//
// MessageId: SDB_20
//
// MessageText:
//
//  SDB_20: Edge for feature %1$d not found.
//
#define SDB_20                           0x0000070CL

//
// MessageId: SDB_21
//
// MessageText:
//
//  SDB_21: Lock Conflict: Can't update relationship between edge %1$ld and polygon %2$ld .
//
#define SDB_21                           0x0000070DL

//
// MessageId: SDB_22
//
// MessageText:
//
//  SDB_22: Version Conflict: Can't update relationship between edge %1$ld and polygon %2$ld .
//
#define SDB_22                           0x0000070EL

//
// MessageId: SDB_23
//
// MessageText:
//
//  SDB_23: Spatial database is not initialized for measurement.
//
#define SDB_23                           0x0000070FL

//
// MessageId: SDB_24
//
// MessageText:
//
//  SDB_24: Spatial database is not initialized for measurement.
//
#define SDB_24                           0x00000710L

//
// MessageId: SDB_25
//
// MessageText:
//
//  SDB_25: Spatial database is not initialized for measurement.
//
#define SDB_25                           0x00000711L

//
// MessageId: SDB_26
//
// MessageText:
//
//  SDB_26: Attempt was made to search a deleted feature.
//
#define SDB_26                           0x00000712L

//
// MessageId: SDB_27
//
// MessageText:
//
//  SDB_27: Failed at point position = (%1$d) for active feature
//
#define SDB_27                           0x00000713L

//
// MessageId: SDB_28
//
// MessageText:
//
//  SDB_28: Could not retrieve database information for database: %1$s .
//
#define SDB_28                           0x00000714L

//
// MessageId: SDB_29
//
// MessageText:
//
//  SDB_29: Failure in sdb_get_feat(%1$ld)
//
#define SDB_29                           0x00000715L

//
// MessageId: SDB_30
//
// MessageText:
//
//  SDB_30: Failure in sdb_get_feati(%1$ld)
//
#define SDB_30                           0x00000716L

//
// MessageId: SDB_31
//
// MessageText:
//
//  SDB_31: Program Interupted by ctrl-C .
//
#define SDB_31                           0x00000717L

//
// MessageId: SDB_32
//
// MessageText:
//
//  SDB_32: Point retrieval direction must be +/- 1, got %1$d for feature %2$ld .
//
#define SDB_32                           0x00000718L

//
// MessageId: SDB_33
//
// MessageText:
//
//  SDB_33: Feature %1$d not found, cannot lock it.
//
#define SDB_33                           0x00000719L

//
// MessageId: SDB_34
//
// MessageText:
//
//  SDB_34: There is no currently active database.
//
#define SDB_34                           0x0000071AL

//
// MessageId: SDB_35
//
// MessageText:
//
//  SDB_35: Schema version (%1$s) is out of date; expecting %2$s.\nYou may run into missing columns or tables.
//
#define SDB_35                           0x0000071BL

//
// MessageId: SDB_36
//
// MessageText:
//
//  SDB_36: Database %1$s is already open.
//
#define SDB_36                           0x0000071CL

//
// MessageId: SDB_37
//
// MessageText:
//
//  SDB_37: Cannot open database %1$s, too many already open.
//
#define SDB_37                           0x0000071DL

//
// MessageId: SDB_38
//
// MessageText:
//
//  SDB_38: Evironment variable '%1$s' set to negative value - ignored.
//
#define SDB_38                           0x0000071EL

//
// MessageId: SDB_39
//
// MessageText:
//
//  SDB_39: Environment variable '%1$s' set to negative value - ignored.
//
#define SDB_39                           0x0000071FL

//
// MessageId: SDB_40
//
// MessageText:
//
//  SDB_40: Unable to obtain definition of '%1$s' .
//
#define SDB_40                           0x00000720L

//
// MessageId: SDB_41
//
// MessageText:
//
//  SDB_41: No original master information available for the active feature.
//
#define SDB_41                           0x00000721L

//
// MessageId: SDB_42
//
// MessageText:
//
//  SDB_42: Invalid value, this will make plans %1$ld, and %2$ld too recursively dependent.
//
#define SDB_42                           0x00000722L

//
// MessageId: SDB_43
//
// MessageText:
//
//  SDB_43: Invalid value, this will make plans %1$ld, and %2$ld too recursively dependent.
//
#define SDB_43                           0x00000723L

//
// MessageId: SDB_44
//
// MessageText:
//
//  SDB_44: ... processed %1$d features (%2$d g_si rows inserted)
//
#define SDB_44                           0x00000724L

//
// MessageId: SDB_45
//
// MessageText:
//
//  SDB_45: Feature %1$ld not found.
//
#define SDB_45                           0x00000725L

//
// MessageId: SDB_46
//
// MessageText:
//
//  SDB_46: Feature %1$ld not found.
//
#define SDB_46                           0x00000726L

//
// MessageId: SDB_47
//
// MessageText:
//
//  SDB_47: Network %1$d not found.
//
#define SDB_47                           0x00000727L

//
// MessageId: SDB_48
//
// MessageText:
//
//  SDB_48: Network %1$d not found.
//
#define SDB_48                           0x00000728L

//
// MessageId: SDB_49
//
// MessageText:
//
//  SDB_49: Feature %1$ld is locked by another user - cannot update text.
//
#define SDB_49                           0x00000729L

//
// MessageId: SDB_50
//
// MessageText:
//
//  SDB_50: Database has not been opened
//
#define SDB_50                           0x0000072AL

//
// MessageId: SDB_51
//
// MessageText:
//
//  SDB_51: Stub for sdb_new_feat called.
//
#define SDB_51                           0x0000072BL

//
// MessageId: SDB_52
//
// MessageText:
//
//  SDB_52: A feature is already active.
//
#define SDB_52                           0x0000072CL

//
// MessageId: SDB_53
//
// MessageText:
//
//  SDB_53: New feat num is %1$ld
//
#define SDB_53                           0x0000072DL

//
// MessageId: SDB_54
//
// MessageText:
//
//  SDB_54: Stub for sdb_activ_feat called.
//
#define SDB_54                           0x0000072EL

//
// MessageId: SDB_55
//
// MessageText:
//
//  SDB_55: There is already an active feature.
//
#define SDB_55                           0x0000072FL

//
// MessageId: SDB_56
//
// MessageText:
//
//  SDB_56: Test stub for del_feat .
//
#define SDB_56                           0x00000730L

//
// MessageId: SDB_57
//
// MessageText:
//
//  SDB_57: stub got rdbi error %1$d .
//
#define SDB_57                           0x00000731L

//
// MessageId: SDB_58
//
// MessageText:
//
//  SDB_58: Stub for sdb_get_feat called.
//
#define SDB_58                           0x00000732L

//
// MessageId: SDB_59
//
// MessageText:
//
//  SDB_59: Stub for sdb_add_point called.
//
#define SDB_59                           0x00000733L

//
// MessageId: SDB_60
//
// MessageText:
//
//  SDB_60: There is no active feature.
//
#define SDB_60                           0x00000734L

//
// MessageId: SDB_61
//
// MessageText:
//
//  SDB_61: Stub for sdb_deact_feat called.
//
#define SDB_61                           0x00000735L

//
// MessageId: SDB_62
//
// MessageText:
//
//  SDB_62: There is no active feature.
//
#define SDB_62                           0x00000736L

//
// MessageId: SDB_63
//
// MessageText:
//
//  SDB_63: stub got rdbi error %1$d
//
#define SDB_63                           0x00000737L

//
// MessageId: SDB_64
//
// MessageText:
//
//  SDB_64: stub got rdbi error %1$d
//
#define SDB_64                           0x00000738L

//
// MessageId: SDB_65
//
// MessageText:
//
//  SDB_65: Feature %1$ld not found.
//
#define SDB_65                           0x00000739L

//
// MessageId: SDB_66
//
// MessageText:
//
//  SDB_66: stub got rdbi error %1$d
//
#define SDB_66                           0x0000073AL

//
// MessageId: SDB_67
//
// MessageText:
//
//  SDB_67: Stub got rdbi error %1$d .
//
#define SDB_67                           0x0000073BL

//
// MessageId: SDB_68
//
// MessageText:
//
//  SDB_68: Cannot end transaction '%1$s' until active feature is deactivated.
//
#define SDB_68                           0x0000073CL

//
// MessageId: SDB_69
//
// MessageText:
//
//  SDB_69: Features %1$ld is active, cannot unlock it.
//
#define SDB_69                           0x0000073DL

//
// MessageId: SDB_70
//
// MessageText:
//
//  SDB_70: Bad argument 'direction = %1$d' passed to sdb_cache_angle .
//
#define SDB_70                           0x0000073EL

//
// MessageId: SDB_71
//
// MessageText:
//
//  SDB_71: Bad argument 'cache_num = %1$d' passed to sdb_cache_angle .
//
#define SDB_71                           0x0000073FL

//
// MessageId: SDB_72
//
// MessageText:
//
//  SDB_72: Bad argument 'point_num = %1$d' passed to sdb_cache_angle .
//
#define SDB_72                           0x00000740L

//
// MessageId: SDB_73
//
// MessageText:
//
//  SDB_73: Unable to allocate memory for extent tree.
//
#define SDB_73                           0x00000741L

//
// MessageId: SDB_74
//
// MessageText:
//
//  SDB_74: Cache 0 not initialized.
//
#define SDB_74                           0x00000742L

//
// MessageId: SDB_75
//
// MessageText:
//
//  SDB_75: Cache 1 not initialized.
//
#define SDB_75                           0x00000743L

//
// MessageId: SDB_76
//
// MessageText:
//
//  SDB_76: Out of memory allocating feature tree structure.
//
#define SDB_76                           0x00000744L

//
// MessageId: SDB_77
//
// MessageText:
//
//  SDB_77: Out of memory allocating feature tree structure.
//
#define SDB_77                           0x00000745L

//
// MessageId: SDB_78
//
// MessageText:
//
//  SDB_78: Unable to get memory for %1$d points (feature %2$d) .
//
#define SDB_78                           0x00000746L

//
// MessageId: SDB_79
//
// MessageText:
//
//  SDB_79: Problem with cache parameter start point
//
#define SDB_79                           0x00000747L

//
// MessageId: SDB_80
//
// MessageText:
//
//  SDB_80:  Problem with cache 'number of points' parameter.
//
#define SDB_80                           0x00000748L

//
// MessageId: SDB_81
//
// MessageText:
//
//  SDB_81: Unable to allocate memory for coincidence structure (%1$d bytes) .
//
#define SDB_81                           0x00000749L

//
// MessageId: SDB_82
//
// MessageText:
//
//  SDB_82: Invalid cache point number.
//
#define SDB_82                           0x0000074AL

//
// MessageId: SDB_83
//
// MessageText:
//
//  SDB_83: Input point at circular arc position '%1$s' is outside the database extent, arc parameters ignored."
//
#define SDB_83                           0x0000074BL

//
// MessageId: SDB_84
//
// MessageText:
//
//  SDB_84: Feature %1$ld is an edge of a polygon. Can't update feature network when in Direct Version Update mode."
//
#define SDB_84                           0x0000074CL

//
// MessageId: SDB_85
//
// MessageText:
//
//  SDB_85: Invalid value for class number: %1$ld.  Must be positive.
//
#define SDB_85                           0x0000074DL

//
// MessageId: SDB_86
//
// MessageText:
//
//  SDB_86: Missing default class for g_master table.
//
#define SDB_86                           0x0000074EL

//
// MessageId: SDB_87
//
// MessageText:
//
//  SDB_87: Feature %1$ld is a polygon. Can't update feature network when in Direct Version Update mode."
//
#define SDB_87                           0x0000074FL

//
// MessageId: SI_1
//
// MessageText:
//
//  SI_1: Too many skip chars specified (max %1$d) .
//
#define SI_1                             0x00000750L

//
// MessageId: SI_2
//
// MessageText:
//
//  SI_2: Invalid MBC format ('4' only allowed at MBC end).
//
#define SI_2                             0x00000751L

//
// MessageId: SI_3
//
// MessageText:
//
//  SI_3: Invalid compressed MBC format (padding).
//
#define SI_3                             0x00000752L

//
// MessageId: SI_4
//
// MessageText:
//
//  SI_4: Invalid compressed MBC format (illegal character).
//
#define SI_4                             0x00000753L

//
// MessageId: SI_5
//
// MessageText:
//
//  SI_5: Invalid MBC format ('4' only allowed at MBC end).
//
#define SI_5                             0x00000754L

//
// MessageId: SI_6
//
// MessageText:
//
//  SI_6: Invalid compressed MBC format (depth exceeds max_depth).
//
#define SI_6                             0x00000755L

//
// MessageId: SI_7
//
// MessageText:
//
//  SI_7: Invalid spatial index extents (min > max).
//
#define SI_7                             0x00000756L

//
// MessageId: SI_8
//
// MessageText:
//
//  SI_8: Invalid spatial index max. depth (%1$d <= 0).
//
#define SI_8                             0x00000757L

//
// MessageId: SI_9
//
// MessageText:
//
//  SI_9: Max range list size must be greater than zero (%1$d).
//
#define SI_9                             0x00000758L

//
// MessageId: SI_10
//
// MessageText:
//
//  SI_10: %1$s
//
#define SI_10                            0x00000759L

//
// MessageId: SI_11
//
// MessageText:
//
//  SI_11: range_2 query extent outside spatial index extents.
//
#define SI_11                            0x0000075AL

//
// MessageId: SI_12
//
// MessageText:
//
//  SI_12: Invalid MBC format (missing root cell).
//
#define SI_12                            0x0000075BL

//
// MessageId: SI_13
//
// MessageText:
//
//  SI_13: Invalid MBC format (illegal character).
//
#define SI_13                            0x0000075CL

//
// MessageId: SI_14
//
// MessageText:
//
//  SI_14: Invalid MBC format (missing root cell).
//
#define SI_14                            0x0000075DL

//
// MessageId: SI_15
//
// MessageText:
//
//  SI_15: Invalid MBC format (illegal character).
//
#define SI_15                            0x0000075EL

//
// MessageId: SI_16
//
// MessageText:
//
//  SI_16: Invalid compressed MBC format.
//
#define SI_16                            0x0000075FL

//
// MessageId: SI_17
//
// MessageText:
//
//  SI_17: Invalid MBC format (missing root cell).
//
#define SI_17                            0x00000760L

//
// MessageId: SI_18
//
// MessageText:
//
//  SI_18: Invalid MBC format (illegal character).
//
#define SI_18                            0x00000761L

//
// MessageId: SI_19
//
// MessageText:
//
//  SI_19: Attempt to free NULL range_list.
//
#define SI_19                            0x00000762L

//
// MessageId: SI_20
//
// MessageText:
//
//  SI_20: Invalid range list min. area (%1$s < 0).
//
#define SI_20                            0x00000763L

//
// MessageId: SI_21
//
// MessageText:
//
//  SI_21: MBC extent outside spatial index extents.
//
#define SI_21                            0x00000764L

//
// MessageId: SI_22
//
// MessageText:
//
//  SI_22: MBC extent outside spatial index extents.
//
#define SI_22                            0x00000765L

//
// MessageId: SU_1
//
// MessageText:
//
//  SU_1: %1$d polygons given for clipping -- 1 or 2 are required
//
#define SU_1                             0x00000766L

//
// MessageId: SU_2
//
// MessageText:
//
//  SU_2: Warning:  no points processed in polygon clip operation.
//
#define SU_2                             0x00000767L

//
// MessageId: SU_3
//
// MessageText:
//
//  SU_3: Unable to allocate Output Loop List
//
#define SU_3                             0x00000768L

//
// MessageId: SU_4
//
// MessageText:
//
//  SU_4: Unable to add to Output Loop List
//
#define SU_4                             0x00000769L

//
// MessageId: SU_5
//
// MessageText:
//
//  SU_5: Unable to allocate Output Loop Left Point List
//
#define SU_5                             0x0000076AL

//
// MessageId: SU_6
//
// MessageText:
//
//  SU_6: Unable to allocate Output Loop Right Point List
//
#define SU_6                             0x0000076BL

//
// MessageId: SU_7
//
// MessageText:
//
//  SU_7: Unable to append point # %1$d to Output Loop Point List
//
#define SU_7                             0x0000076CL

//
// MessageId: SU_8
//
// MessageText:
//
//  SU_8: Polygon clipping error:  unable to continue after a local maximum.
//
#define SU_8                             0x0000076DL

//
// MessageId: SU_9
//
// MessageText:
//
//  SU_9: Warning:    %1$d buffer loops could not be closed.
//
#define SU_9                             0x0000076EL

//
// MessageId: SU_10
//
// MessageText:
//
//  SU_10: Unable to delete from Point List
//
#define SU_10                            0x0000076FL

//
// MessageId: SU_11
//
// MessageText:
//
//  SU_11: Warning:  Polygon clipping failed to find expected intersection of edges.
//
#define SU_11                            0x00000770L

//
// MessageId: SU_12
//
// MessageText:
//
//  SU_12: Unable to allocate space for %1$d buffer points.
//
#define SU_12                            0x00000771L

//
// MessageId: SU_13
//
// MessageText:
//
//  SU_13: Unable to allocate space for %1$d buffer points.
//
#define SU_13                            0x00000772L

//
// MessageId: SU_14
//
// MessageText:
//
//  SU_14: Unable to allocate Buffer Intersection List
//
#define SU_14                            0x00000773L

//
// MessageId: SU_15
//
// MessageText:
//
//  SU_15: Unable to append to Buffer Intersection List after %1$d intersections
//
#define SU_15                            0x00000774L

//
// MessageId: SU_16
//
// MessageText:
//
//  SU_16:   Sorting %1$d buffer intersections...
//
#define SU_16                            0x00000775L

//
// MessageId: SU_17
//
// MessageText:
//
//  SU_17: Unable to allocate interval list
//
#define SU_17                            0x00000776L

//
// MessageId: SU_18
//
// MessageText:
//
//  SU_18: Unable to allocate interval list
//
#define SU_18                            0x00000777L

//
// MessageId: SU_19
//
// MessageText:
//
//  SU_19: Unable to allocate node list
//
#define SU_19                            0x00000778L

//
// MessageId: SU_20
//
// MessageText:
//
//  SU_20:   Computing buffer intervals...
//
#define SU_20                            0x00000779L

//
// MessageId: SU_21
//
// MessageText:
//
//  SU_21: Unable to allocate node connections in buffer
//
#define SU_21                            0x0000077AL

//
// MessageId: SU_22
//
// MessageText:
//
//  SU_22: Unable to allocate node connections in buffer
//
#define SU_22                            0x0000077BL

//
// MessageId: SU_23
//
// MessageText:
//
//  SU_23: Unable to allocate interval points in buffer
//
#define SU_23                            0x0000077CL

//
// MessageId: SU_24
//
// MessageText:
//
//  SU_24:   Sorting %1$d buffer intervals...
//
#define SU_24                            0x0000077DL

//
// MessageId: SU_25
//
// MessageText:
//
//  SU_25: Unable to allocate segment array for buffer
//
#define SU_25                            0x0000077EL

//
// MessageId: SU_26
//
// MessageText:
//
//  SU_26: Warning:  %1$d redundant points were found during buffer segment scan
//
#define SU_26                            0x0000077FL

//
// MessageId: SU_27
//
// MessageText:
//
//  SU_27: Polygon clipping error:  Unknown edge intersection class.
//
#define SU_27                            0x00000780L

//
// MessageId: SU_28
//
// MessageText:
//
//  SU_28: Polygon clipping error:  Unknown edge endpoint class.
//
#define SU_28                            0x00000781L

//
// MessageId: SU_29
//
// MessageText:
//
//  SU_29: Polygon clipping error:  could not find matching edge at a local maximum.
//
#define SU_29                            0x00000782L

//
// MessageId: SU_30
//
// MessageText:
//
//  SU_30: Polygon clipping error:  exceeded boundary edge list.
//
#define SU_30                            0x00000783L

//
// MessageId: SU_31
//
// MessageText:
//
//  SU_31: Unable to allocate edge list
//
#define SU_31                            0x00000784L

//
// MessageId: SU_32
//
// MessageText:
//
//  SU_32: Could not find polygon relationships for feature %1$ld.
//
#define SU_32                            0x00000785L

//
// MessageId: SU_33
//
// MessageText:
//
//  SU_33: Unable to add to edge list
//
#define SU_33                            0x00000786L

//
// MessageId: SU_34
//
// MessageText:
//
//  SU_34: Cannot find edge connected to feature %1$ld.
//
#define SU_34                            0x00000787L

//
// MessageId: SU_35
//
// MessageText:
//
//  SU_35: Encountered wrong edge %1$d on polygon %2$d.
//
#define SU_35                            0x00000788L

//
// MessageId: SU_36
//
// MessageText:
//
//  SU_36: Couldn't find local maximum a polygon loop
//
#define SU_36                            0x00000789L

//
// MessageId: SU_37
//
// MessageText:
//
//  SU_37: Couldn't find local maximum in loop %1$ld of polygon %2$d
//
#define SU_37                            0x0000078AL

//
// MessageId: SU_38
//
// MessageText:
//
//  SU_38: Nonexistent feature number %1$d given for polygon clipping
//
#define SU_38                            0x0000078BL

//
// MessageId: SU_39
//
// MessageText:
//
//  SU_39: Feature %1$d for clipping is not a polygon
//
#define SU_39                            0x0000078CL

//
// MessageId: SU_40
//
// MessageText:
//
//  SU_40: Feature %1$d given for polygon clipping is not a complete polygon
//
#define SU_40                            0x0000078DL

//
// MessageId: SU_41
//
// MessageText:
//
//  SU_41: Couldn't find local minimum in loop %1$ld of polygon %2$d
//
#define SU_41                            0x0000078EL

//
// MessageId: SU_42
//
// MessageText:
//
//  SU_42: Couldn't find local maximum in loop %1$ld of polygon %2$d
//
#define SU_42                            0x0000078FL

//
// MessageId: SU_43
//
// MessageText:
//
//  SU_43: Unable to allocate Local Minima List
//
#define SU_43                            0x00000790L

//
// MessageId: SU_44
//
// MessageText:
//
//  SU_44: Unable to append to Local Minima List
//
#define SU_44                            0x00000791L

//
// MessageId: SU_45
//
// MessageText:
//
//  SU_45: Unable to allocate Point List
//
#define SU_45                            0x00000792L

//
// MessageId: SU_46
//
// MessageText:
//
//  SU_46: Unable to append to Point List
//
#define SU_46                            0x00000793L

//
// MessageId: SU_47
//
// MessageText:
//
//  SU_47: Unable to allocate Scanbeam List
//
#define SU_47                            0x00000794L

//
// MessageId: SU_48
//
// MessageText:
//
//  SU_48: Unable to append to Scanbeam List
//
#define SU_48                            0x00000795L

//
// MessageId: SU_49
//
// MessageText:
//
//  SU_49: Unable to delete from Scanbeam List
//
#define SU_49                            0x00000796L

//
// MessageId: SU_50
//
// MessageText:
//
//  SU_50: Unable to allocate Intersection List
//
#define SU_50                            0x00000797L

//
// MessageId: SU_51
//
// MessageText:
//
//  SU_51: Unable to append to Intersection List
//
#define SU_51                            0x00000798L

//
// MessageId: SU_52
//
// MessageText:
//
//  SU_52: Unable to allocate Active Edge List
//
#define SU_52                            0x00000799L

//
// MessageId: SU_53
//
// MessageText:
//
//  SU_53: Unable to append to Active Edge List
//
#define SU_53                            0x0000079AL

//
// MessageId: SU_54
//
// MessageText:
//
//  SU_54: Unable to allocate Sorted Active Edge List
//
#define SU_54                            0x0000079BL

//
// MessageId: SU_55
//
// MessageText:
//
//  SU_55: Unable to append to Sorted Active Edge List
//
#define SU_55                            0x0000079CL

//
// MessageId: SU_56
//
// MessageText:
//
//  SU_56: Unable to delete from Active Edge List
//
#define SU_56                            0x0000079DL

//
// MessageId: SU_57
//
// MessageText:
//
//  SU_57: Unable to delete from Sorted Active Edge List
//
#define SU_57                            0x0000079EL

//
// MessageId: SU_58
//
// MessageText:
//
//  SU_58: Unable to deallocate Output Loop Left Point List
//
#define SU_58                            0x0000079FL

//
// MessageId: SU_59
//
// MessageText:
//
//  SU_59: Unable to deallocate Output Loop Right Point List
//
#define SU_59                            0x000007A0L

//
// MessageId: SU_60
//
// MessageText:
//
//  SU_60: Unable to delete from Output Loop List
//
#define SU_60                            0x000007A1L

//
// MessageId: SU_61
//
// MessageText:
//
//  SU_61:   %1$s:  report # %2$d;  %3$s %%%% complete    \r
//
#define SU_61                            0x000007A2L

//
// MessageId: SU_62
//
// MessageText:
//
//  SU_62:   %1$s:  report # %2$d;  %3$s %%%% complete
//
#define SU_62                            0x000007A3L

//
// MessageId: SU_63
//
// MessageText:
//
//  SU_63: Input parameter ptrad_def is NULL
//
#define SU_63                            0x000007A4L

//
// MessageId: SU_64
//
// MessageText:
//
//  SU_64: Radius is not positive
//
#define SU_64                            0x000007A5L

//
// MessageId: SU_65
//
// MessageText:
//
//  SU_65: Rotation direction info is lost in converting the representation of a circle into the 3-point representation
//
#define SU_65                            0x000007A6L

//
// MessageId: SU_66
//
// MessageText:
//
//  SU_66: Direction vector can't be zero
//
#define SU_66                            0x000007A7L

//
// MessageId: SU_67
//
// MessageText:
//
//  SU_67: ERROR: Features have to be of line or polygon type !
//
#define SU_67                            0x000007A8L

//
// MessageId: SU_68
//
// MessageText:
//
//  SU_68: ERROR: Intersection between 2 polygons is not implemented !
//
#define SU_68                            0x000007A9L

//
// MessageId: SU_69
//
// MessageText:
//
//  SU_69: Features %1$ld and %2$ld don't intersect. 
//
#define SU_69                            0x000007AAL

//
// MessageId: SU_70
//
// MessageText:
//
//  SU_70: Spatial database is not initialized for measurement. 
//
#define SU_70                            0x000007ABL

//
// MessageId: SU_71
//
// MessageText:
//
//  SU_71: Feature %1$ld does not exist in the database.
//
#define SU_71                            0x000007ACL

//
// MessageId: SU_72
//
// MessageText:
//
//  SU_72: Breaking out of infinite loop in `%1$s'.
//
#define SU_72                            0x000007ADL

//
// MessageId: UT_1
//
// MessageText:
//
//  UT_1: Too many parameters for degree conversion ('%1$s')
//
#define UT_1                             0x000007AEL

//
// MessageId: UT_2
//
// MessageText:
//
//  UT_2: Too many parameters for degree conversion ('%1$s')
//
#define UT_2                             0x000007AFL

//
// MessageId: UT_3
//
// MessageText:
//
//  UT_3: Too many signs found during degree conversion ('%1$s')
//
#define UT_3                             0x000007B0L

//
// MessageId: UT_4
//
// MessageText:
//
//  UT_4: Imbedded sign used incorrectly in degree value ('%1$s')
//
#define UT_4                             0x000007B1L

//
// MessageId: UT_5
//
// MessageText:
//
//  UT_5: Non-numeric character found in degree value ('%1$s')
//
#define UT_5                             0x000007B2L

//
// MessageId: UT_6
//
// MessageText:
//
//  UT_6: Invalid list passed to %1$s
//
#define UT_6                             0x000007B3L

//
// MessageId: UT_7
//
// MessageText:
//
//  UT_7: Unable to allocate chunk header in ut_blist_add
//
#define UT_7                             0x000007B4L

//
// MessageId: UT_8
//
// MessageText:
//
//  UT_8: Unable to allocate chunk in ut_blist_add
//
#define UT_8                             0x000007B5L

//
// MessageId: UT_9
//
// MessageText:
//
//  UT_9: Invalid list passed to %1$s
//
#define UT_9                             0x000007B6L

//
// MessageId: UT_10
//
// MessageText:
//
//  UT_10: Invalid list passed to %1$s
//
#define UT_10                            0x000007B7L

//
// MessageId: UT_11
//
// MessageText:
//
//  UT_11: ut_blist_init: unit size %1$d exceeds the limit %2$d
//
#define UT_11                            0x000007B8L

//
// MessageId: UT_12
//
// MessageText:
//
//  UT_12: ut_blist_init: malloc
//
#define UT_12                            0x000007B9L

//
// MessageId: UT_13
//
// MessageText:
//
//  UT_13: Name must not be null
//
#define UT_13                            0x000007BAL

//
// MessageId: UT_14
//
// MessageText:
//
//  UT_14: Name (%1$s) too long; maximum length is %2$d characters
//
#define UT_14                            0x000007BBL

//
// MessageId: UT_15
//
// MessageText:
//
//  UT_15: Name contains an invalid character: '%1$c'
//
#define UT_15                            0x000007BCL

//
// MessageId: UT_16
//
// MessageText:
//
//  UT_16: Name (%1$s) can only contain one '.'
//
#define UT_16                            0x000007BDL

//
// MessageId: UT_17
//
// MessageText:
//
//  UT_17: invalid number of significant digits - %1$d
//
#define UT_17                            0x000007BEL

//
// MessageId: UT_18
//
// MessageText:
//
//  UT_18: invalid definition.
//
#define UT_18                            0x000007BFL

//
// MessageId: UT_19
//
// MessageText:
//
//  UT_19: invalid definition.
//
#define UT_19                            0x000007C0L

//
// MessageId: UT_20
//
// MessageText:
//
//  UT_20: System error in ut_edit_file: %1$d
//
#define UT_20                            0x000007C1L

//
// MessageId: UT_21
//
// MessageText:
//
//  UT_21: ut_edit_str: tempnam call
//
#define UT_21                            0x000007C2L

//
// MessageId: UT_22
//
// MessageText:
//
//  UT_22: ut_edit_str: fopen(w) call
//
#define UT_22                            0x000007C3L

//
// MessageId: UT_23
//
// MessageText:
//
//  UT_23: ut_edit_str: fopen(r) call
//
#define UT_23                            0x000007C4L

//
// MessageId: UT_24
//
// MessageText:
//
//  UT_24: ut_edit_str: fseek call
//
#define UT_24                            0x000007C5L

//
// MessageId: UT_25
//
// MessageText:
//
//  UT_25: ut_edit_str: malloc call
//
#define UT_25                            0x000007C6L

//
// MessageId: UT_26
//
// MessageText:
//
//  UT_26: ut_edit_str: file larger than expected
//
#define UT_26                            0x000007C7L

//
// MessageId: UT_27
//
// MessageText:
//
//  UT_27: Input line truncated at %1$d characters
//
#define UT_27                            0x000007C8L

//
// MessageId: UT_28
//
// MessageText:
//
//  UT_28: ut_find_file: An input file name must be specified
//
#define UT_28                            0x000007C9L

//
// MessageId: UT_29
//
// MessageText:
//
//  UT_29: ut_find_file: No such directory as %1$s
//
#define UT_29                            0x000007CAL

//
// MessageId: UT_30
//
// MessageText:
//
//  UT_30: temp is = %1$s
//
#define UT_30                            0x000007CBL

//
// MessageId: UT_31
//
// MessageText:
//
//  UT_31: Path is = %1$s
//
#define UT_31                            0x000007CCL

//
// MessageId: UT_32
//
// MessageText:
//
//  UT_32: Unknown user `%1$s'
//
#define UT_32                            0x000007CDL

//
// MessageId: UT_33
//
// MessageText:
//
//  UT_33: The path is = %1$s
//
#define UT_33                            0x000007CEL

//
// MessageId: UT_34
//
// MessageText:
//
//  UT_34: Unknown home directory `%1$$1s'
//
#define UT_34                            0x000007CFL

//
// MessageId: UT_35
//
// MessageText:
//
//  UT_35: ut_ftok: Unable to establish SUITE number, VISION_SUITE_PID not set
//
#define UT_35                            0x000007D0L

//
// MessageId: UT_36
//
// MessageText:
//
//  UT_36: ut_ftok: Unable to allocate any more IPCS resource keys
//
#define UT_36                            0x000007D1L

//
// MessageId: UT_37
//
// MessageText:
//
//  UT_37: ut_ftok: Unable to create temp file '%1$s'.
//
#define UT_37                            0x000007D2L

//
// MessageId: UT_38
//
// MessageText:
//
//  UT_38: ut_ftok: ftok() is unable to access file '%1$s'.
//
#define UT_38                            0x000007D3L

//
// MessageId: UT_39
//
// MessageText:
//
//  UT_39: Invalid arc turn direction.
//
#define UT_39                            0x000007D4L

//
// MessageId: UT_40
//
// MessageText:
//
//  UT_40: Arc start and endpoint cannot be the same.
//
#define UT_40                            0x000007D5L

//
// MessageId: UT_41
//
// MessageText:
//
//  UT_41: Radius too short for arc.
//
#define UT_41                            0x000007D6L

//
// MessageId: UT_42
//
// MessageText:
//
//  UT_42: System error: points do not define an arc
//
#define UT_42                            0x000007D7L

//
// MessageId: UT_43
//
// MessageText:
//
//  UT_43: \nInterrupt detected, EXIT assumed
//
#define UT_43                            0x000007D8L

//
// MessageId: UT_44
//
// MessageText:
//
//  UT_44: End of standard input file, EXIT assumed
//
#define UT_44                            0x000007D9L

//
// MessageId: UT_45
//
// MessageText:
//
//  UT_45: End of response file
//
#define UT_45                            0x000007DAL

//
// MessageId: UT_46
//
// MessageText:
//
//  UT_46: Response file BAD_EXIT record found, terminating with error.
//
#define UT_46                            0x000007DBL

//
// MessageId: UT_47
//
// MessageText:
//
//  UT_47: %s: illegal option -- %c
//
#define UT_47                            0x000007DCL

//
// MessageId: UT_48
//
// MessageText:
//
//  UT_48: %s: option requires an argument -- %c
//
#define UT_48                            0x000007DDL

//
// MessageId: UT_49
//
// MessageText:
//
//  UT_49: Cannot position at record %1$d
//
#define UT_49                            0x000007DEL

//
// MessageId: UT_50
//
// MessageText:
//
//  UT_50: Record not read in completely
//
#define UT_50                            0x000007DFL

//
// MessageId: UT_51
//
// MessageText:
//
//  UT_51: error: invalid record number for ut_grp_rec_rd
//
#define UT_51                            0x000007E0L

//
// MessageId: UT_52
//
// MessageText:
//
//  UT_52: Invalid group number in ut_grp_rec_in
//
#define UT_52                            0x000007E1L

//
// MessageId: UT_53
//
// MessageText:
//
//  UT_53: Invalid record length in ut_grp_rec_in
//
#define UT_53                            0x000007E2L

//
// MessageId: UT_54
//
// MessageText:
//
//  UT_54: Could not allocate memory in ut_grp_rec_in
//
#define UT_54                            0x000007E3L

//
// MessageId: UT_55
//
// MessageText:
//
//  UT_55: Out of in-memory record groups
//
#define UT_55                            0x000007E4L

//
// MessageId: UT_56
//
// MessageText:
//
//  UT_56: Invalid group number for ut_grp_rec_tm
//
#define UT_56                            0x000007E5L

//
// MessageId: UT_57
//
// MessageText:
//
//  UT_57: Invalid record number for ut_grp_recs_wr
//
#define UT_57                            0x000007E6L

//
// MessageId: UT_58
//
// MessageText:
//
//  UT_58: ut_heap_init: unit size %1$d exceeds the limit %2$d
//
#define UT_58                            0x000007E7L

//
// MessageId: UT_59
//
// MessageText:
//
//  UT_59: ut_heap_init: malloc is out of space
//
#define UT_59                            0x000007E8L

//
// MessageId: UT_60
//
// MessageText:
//
//  UT_60: Interpolation - a divide by zero was attempted
//
#define UT_60                            0x000007E9L

//
// MessageId: UT_61
//
// MessageText:
//
//  UT_61: Interpolation - a divide by zero was attempted
//
#define UT_61                            0x000007EAL

//
// MessageId: UT_62
//
// MessageText:
//
//  UT_62: Curve Interpolation - a divide by zero was attempted
//
#define UT_62                            0x000007EBL

//
// MessageId: UT_63
//
// MessageText:
//
//  UT_63: Curve Interpolation - a divide by zero was attempted
//
#define UT_63                            0x000007ECL

//
// MessageId: UT_64
//
// MessageText:
//
//  UT_64: ut_kill: cannot kill process
//
#define UT_64                            0x000007EDL

//
// MessageId: UT_65
//
// MessageText:
//
//  UT_65: ut_list_alloc: passed NULL string list
//
#define UT_65                            0x000007EEL

//
// MessageId: UT_66
//
// MessageText:
//
//  UT_66: ut_list_alloc: can't allocate size of %1$d
//
#define UT_66                            0x000007EFL

//
// MessageId: UT_67
//
// MessageText:
//
//  UT_67: Sign must be in first position.
//
#define UT_67                            0x000007F0L

//
// MessageId: UT_68
//
// MessageText:
//
//  UT_68: Cannot have more than 1 exponent 'e' symbols.
//
#define UT_68                            0x000007F1L

//
// MessageId: UT_69
//
// MessageText:
//
//  UT_69: Cannot have negative exponent with integer data type.
//
#define UT_69                            0x000007F2L

//
// MessageId: UT_70
//
// MessageText:
//
//  UT_70: Cannot have more than 1 decimal points.
//
#define UT_70                            0x000007F3L

//
// MessageId: UT_71
//
// MessageText:
//
//  UT_71: Cannot have a decimal point with an integer value.
//
#define UT_71                            0x000007F4L

//
// MessageId: UT_72
//
// MessageText:
//
//  UT_72: Invalid number.
//
#define UT_72                            0x000007F5L

//
// MessageId: UT_73
//
// MessageText:
//
//  UT_73: Invalid number: no digits
//
#define UT_73                            0x000007F6L

//
// MessageId: UT_74
//
// MessageText:
//
//  UT_74: Invalid number: no digits in exponent
//
#define UT_74                            0x000007F7L

//
// MessageId: UT_75
//
// MessageText:
//
//  UT_75: Number is too large for data type.. 
//
#define UT_75                            0x000007F8L

//
// MessageId: UT_76
//
// MessageText:
//
//  UT_76: Number must be between %1$d and %2$d.
//
#define UT_76                            0x000007F9L

//
// MessageId: UT_77
//
// MessageText:
//
//  UT_77: Number is too large for data type.. 
//
#define UT_77                            0x000007FAL

//
// MessageId: UT_78
//
// MessageText:
//
//  UT_78: Number must be between %1$ld and %2$ld.
//
#define UT_78                            0x000007FBL

//
// MessageId: UT_79
//
// MessageText:
//
//  UT_79: Response files are nested too deeply. Did not open %1$s.
//
#define UT_79                            0x000007FCL

//
// MessageId: UT_80
//
// MessageText:
//
//  UT_80: Couldn't open response file %1$s.
//
#define UT_80                            0x000007FDL

//
// MessageId: UT_81
//
// MessageText:
//
//  UT_81: Couldn't open edit file %1$s 
//
#define UT_81                            0x000007FEL

//
// MessageId: UT_82
//
// MessageText:
//
//  UT_82: Couldn't open input file %1$s 
//
#define UT_82                            0x000007FFL

//
// MessageId: UT_83
//
// MessageText:
//
//  UT_83: Couldn't open output file %1$s 
//
#define UT_83                            0x00000800L

//
// MessageId: UT_84
//
// MessageText:
//
//  UT_84: ut_putenv: can't put environment variables
//
#define UT_84                            0x00000801L

//
// MessageId: UT_85
//
// MessageText:
//
//  UT_85: Cannot position at record %1$d
//
#define UT_85                            0x00000802L

//
// MessageId: UT_86
//
// MessageText:
//
//  UT_86: Record not written completely
//
#define UT_86                            0x00000803L

//
// MessageId: UT_87
//
// MessageText:
//
//  UT_87: Hangup
//
#define UT_87                            0x00000804L

//
// MessageId: UT_88
//
// MessageText:
//
//  UT_88: Interrupt
//
#define UT_88                            0x00000805L

//
// MessageId: UT_89
//
// MessageText:
//
//  UT_89: Quit
//
#define UT_89                            0x00000806L

//
// MessageId: UT_90
//
// MessageText:
//
//  UT_90: Illegal instruction
//
#define UT_90                            0x00000807L

//
// MessageId: UT_91
//
// MessageText:
//
//  UT_91: Trace trap
//
#define UT_91                            0x00000808L

//
// MessageId: UT_92
//
// MessageText:
//
//  UT_92: IOT instruction
//
#define UT_92                            0x00000809L

//
// MessageId: UT_93
//
// MessageText:
//
//  UT_93: System crash imminent
//
#define UT_93                            0x0000080AL

//
// MessageId: UT_94
//
// MessageText:
//
//  UT_94: EMT instruction
//
#define UT_94                            0x0000080BL

//
// MessageId: UT_95
//
// MessageText:
//
//  UT_95: Floating point exception
//
#define UT_95                            0x0000080CL

//
// MessageId: UT_96
//
// MessageText:
//
//  UT_96: Kill
//
#define UT_96                            0x0000080DL

//
// MessageId: UT_97
//
// MessageText:
//
//  UT_97: Bus error
//
#define UT_97                            0x0000080EL

//
// MessageId: UT_98
//
// MessageText:
//
//  UT_98: Segmentation violation
//
#define UT_98                            0x0000080FL

//
// MessageId: UT_99
//
// MessageText:
//
//  UT_99: Bad arg to system call
//
#define UT_99                            0x00000810L

//
// MessageId: UT_100
//
// MessageText:
//
//  UT_100: Write on pipe
//
#define UT_100                           0x00000811L

//
// MessageId: UT_101
//
// MessageText:
//
//  UT_101: Alarm clock
//
#define UT_101                           0x00000812L

//
// MessageId: UT_102
//
// MessageText:
//
//  UT_102: Terminate signal
//
#define UT_102                           0x00000813L

//
// MessageId: UT_103
//
// MessageText:
//
//  UT_103: Urgent condition on IO channel
//
#define UT_103                           0x00000814L

//
// MessageId: UT_104
//
// MessageText:
//
//  UT_104: Stop signal not from tty
//
#define UT_104                           0x00000815L

//
// MessageId: UT_105
//
// MessageText:
//
//  UT_105: Stop signal from tty
//
#define UT_105                           0x00000816L

//
// MessageId: UT_106
//
// MessageText:
//
//  UT_106: Continue a stopped process
//
#define UT_106                           0x00000817L

//
// MessageId: UT_107
//
// MessageText:
//
//  UT_107: Death of a child
//
#define UT_107                           0x00000818L

//
// MessageId: UT_108
//
// MessageText:
//
//  UT_108: Death of a child
//
#define UT_108                           0x00000819L

//
// MessageId: UT_109
//
// MessageText:
//
//  UT_109: Background tty input
//
#define UT_109                           0x0000081AL

//
// MessageId: UT_110
//
// MessageText:
//
//  UT_110: Background tty output
//
#define UT_110                           0x0000081BL

//
// MessageId: UT_111
//
// MessageText:
//
//  UT_111: I/O Signal
//
#define UT_111                           0x0000081CL

//
// MessageId: UT_112
//
// MessageText:
//
//  UT_112: Exceeded CPU time limit
//
#define UT_112                           0x0000081DL

//
// MessageId: UT_113
//
// MessageText:
//
//  UT_113: Exceeded file size limit
//
#define UT_113                           0x0000081EL

//
// MessageId: UT_114
//
// MessageText:
//
//  UT_114: Virtual time alarm
//
#define UT_114                           0x0000081FL

//
// MessageId: UT_115
//
// MessageText:
//
//  UT_115: Window Size changed
//
#define UT_115                           0x00000820L

//
// MessageId: UT_116
//
// MessageText:
//
//  UT_116: User signal 1
//
#define UT_116                           0x00000821L

//
// MessageId: UT_117
//
// MessageText:
//
//  UT_117: User signal 2
//
#define UT_117                           0x00000822L

//
// MessageId: UT_118
//
// MessageText:
//
//  UT_118: Program abort: Signal #%1$d\n
//
#define UT_118                           0x00000823L

//
// MessageId: UT_119
//
// MessageText:
//
//  UT_119: Program abort: %1$s\n
//
#define UT_119                           0x00000824L

//
// MessageId: UT_121
//
// MessageText:
//
//  UT_121: Cannot stat %1$s
//
#define UT_121                           0x00000825L

//
// MessageId: UT_122
//
// MessageText:
//
//  UT_122: File %1$s is not executable: st_mode = %2$o
//
#define UT_122                           0x00000826L

//
// MessageId: UT_123
//
// MessageText:
//
//  UT_123: ut_spawn: fork of %1$s failed; errno = %2$d
//
#define UT_123                           0x00000827L

//
// MessageId: UT_124
//
// MessageText:
//
//  UT_124: ut_spawn: exec of %1$s failed; errno = %2$d
//
#define UT_124                           0x00000828L

//
// MessageId: UT_125
//
// MessageText:
//
//  UT_125: Unable to allocate stack id block
//
#define UT_125                           0x00000829L

//
// MessageId: UT_126
//
// MessageText:
//
//  UT_126: Unable to allocate base pointer list entry
//
#define UT_126                           0x0000082AL

//
// MessageId: UT_127
//
// MessageText:
//
//  UT_127: Unable to allocate stack
//
#define UT_127                           0x0000082BL

//
// MessageId: UT_128
//
// MessageText:
//
//  UT_128: Unable to reallocate stack
//
#define UT_128                           0x0000082CL

//
// MessageId: UT_129
//
// MessageText:
//
//  UT_129: Bad stack id
//
#define UT_129                           0x0000082DL

//
// MessageId: UT_130
//
// MessageText:
//
//  UT_130: Invalid stack size
//
#define UT_130                           0x0000082EL

//
// MessageId: UT_131
//
// MessageText:
//
//  UT_131: Invalid expansion size
//
#define UT_131                           0x0000082FL

//
// MessageId: UT_132
//
// MessageText:
//
//  UT_132: Invalid allocation size
//
#define UT_132                           0x00000830L

//
// MessageId: UT_133
//
// MessageText:
//
//  UT_133: Invalid data size
//
#define UT_133                           0x00000831L

//
// MessageId: UT_134
//
// MessageText:
//
//  UT_134: Stack overflow
//
#define UT_134                           0x00000832L

//
// MessageId: UT_135
//
// MessageText:
//
//  UT_135: Stack is corrupt
//
#define UT_135                           0x00000833L

//
// MessageId: UT_136
//
// MessageText:
//
//  UT_136: Stack underflow
//
#define UT_136                           0x00000834L

//
// MessageId: UT_137
//
// MessageText:
//
//  UT_137: Stack Error - %1$s
//
#define UT_137                           0x00000835L

//
// MessageId: UT_138
//
// MessageText:
//
//  UT_138: Stack Error - %1$s (%2$d bytes)
//
#define UT_138                           0x00000836L

//
// MessageId: UT_139
//
// MessageText:
//
//  UT_139: Stack Error - Unknown error (code %1$d)
//
#define UT_139                           0x00000837L

//
// MessageId: UT_140
//
// MessageText:
//
//  UT_140: Cannot get system time
//
#define UT_140                           0x00000838L

//
// MessageId: UT_141
//
// MessageText:
//
//  UT_141: Failed to set effective user to '%1$s' (%2$d)
//
#define UT_141                           0x00000839L

//
// MessageId: UT_142
//
// MessageText:
//
//  UT_142: Failed to set effective user to '%1$s' (%2$d)
//
#define UT_142                           0x0000083AL

//
// MessageId: UT_143
//
// MessageText:
//
//  UT_143: Failed to look up the password file entry for user '%1$s'
//
#define UT_143                           0x0000083BL

//
// MessageId: UT_144
//
// MessageText:
//
//  UT_144: Failed to look up the password file entry for user id %1$d
//
#define UT_144                           0x0000083CL

//
// MessageId: UT_145
//
// MessageText:
//
//  UT_145: Failed to look up the password file entry for user id %1$d
//
#define UT_145                           0x0000083DL

//
// MessageId: UT_146
//
// MessageText:
//
//  UT_146: ut_v_spawn (fork) of %1$s
//
#define UT_146                           0x0000083EL

//
// MessageId: UT_147
//
// MessageText:
//
//  UT_147: ut_v_spawn (exec) of %1$s
//
#define UT_147                           0x0000083FL

//
// MessageId: UT_148
//
// MessageText:
//
//  UT_148: Date `%1$s' is invalid.
//
#define UT_148                           0x00000840L

//
// MessageId: UT_149
//
// MessageText:
//
//  UT_149: Memory error: overwrite, addr: %1$lx, alloc'ed by: %2$s
//
#define UT_149                           0x00000841L

//
// MessageId: UT_150
//
// MessageText:
//
//  UT_150: Memory_error: freeing free ptr, addr: %1$lx, alloc'ed by %2$s
//
#define UT_150                           0x00000842L

//
// MessageId: UT_151
//
// MessageText:
//
//  UT_151: Memory error: freeing bad pointer, addr: %1$lx
//
#define UT_151                           0x00000843L

//
// MessageId: UT_152
//
// MessageText:
//
//  UT_152: (%1$s) Memory Error - Unable to allocate (%2$d bytes)
//
#define UT_152                           0x00000844L

//
// MessageId: UT_153
//
// MessageText:
//
//  UT_153: Memory error: attempt to realloc memory that wasn't malloc'ed in routine %1$s
//
#define UT_153                           0x00000845L

//
// MessageId: UT_154
//
// MessageText:
//
//  UT_154: Memory error: overwrite, addr: %1$lx, realloc in %2$s
//
#define UT_154                           0x00000846L

//
// MessageId: UT_155
//
// MessageText:
//
//  UT_155: (%1$s) Memory Error - Unable to allocate (%2$d bytes)
//
#define UT_155                           0x00000847L

//
// MessageId: UT_156
//
// MessageText:
//
//  UT_156: Memory error: block chgd after free, addr: %1$lx
//
#define UT_156                           0x00000848L

//
// MessageId: UT_157
//
// MessageText:
//
//  UT_157: Memory error: freeing unfreed block, functions: '%1$s', addr: %2$lx
//
#define UT_157                           0x00000849L

//
// MessageId: UT_158
//
// MessageText:
//
//  UT_158: Memory error: overwrite, addr: %1$lx
//
#define UT_158                           0x0000084AL

//
// MessageId: UT_159
//
// MessageText:
//
//  UT_159: (%1$s) Memory Error - Invalid alloc size (%2$d bytes)
//
#define UT_159                           0x0000084BL

//
// MessageId: UT_160
//
// MessageText:
//
//  UT_160: (%1$s) Memory Error - Unable to allocate (%2$d bytes)
//
#define UT_160                           0x0000084CL

//
// MessageId: UT_161
//
// MessageText:
//
//  UT_161: (%1$s) Memory Error - Invalid alloc size (%2$d bytes)
//
#define UT_161                           0x0000084DL

//
// MessageId: UT_162
//
// MessageText:
//
//  UT_162: (%1$s) Memory Error - NULL realloc pointer (%2$d bytes)
//
#define UT_162                           0x0000084EL

//
// MessageId: UT_163
//
// MessageText:
//
//  UT_163: (%1$s) Memory Error - Invalid alloc size (%2$d bytes)
//
#define UT_163                           0x0000084FL

//
// MessageId: UT_164
//
// MessageText:
//
//  UT_164: Invalid string, possible characters are alphanumeric, '_', and '$'.
//
#define UT_164                           0x00000850L

//
// MessageId: UT_165
//
// MessageText:
//
//  UT_165: ut/fopenp: %1$s
//
#define UT_165                           0x00000851L

//
// MessageId: UT_166
//
// MessageText:
//
//  UT_166: Resolution of %1$s is too large for the given extent.
//
#define UT_166                           0x00000852L

//
// MessageId: UT_167
//
// MessageText:
//
//  UT_167: Resolution of %1$s is too fine for the given extent.
//
#define UT_167                           0x00000853L

//
// MessageId: UT_168
//
// MessageText:
//
//  UT_168: Approximate best resolution is %1$s.
//
#define UT_168                           0x00000854L

//
// MessageId: UT_169
//
// MessageText:
//
//  UT_169: Increment distance delta (%1$s) should be positive.
//
#define UT_169                           0x00000855L

//
// MessageId: UT_170
//
// MessageText:
//
//  UT_170: Radius (%1$s) is not positive.
//
#define UT_170                           0x00000856L

//
// MessageId: UT_171
//
// MessageText:
//
//  UT_171: Dimension n (%1$d) is not positive.
//
#define UT_171                           0x00000857L

//
// MessageId: UT_172
//
// MessageText:
//
//  UT_172: Matrix is NULL.
//
#define UT_172                           0x00000858L

//
// MessageId: UT_173
//
// MessageText:
//
//  UT_173: eindex is NULL.
//
#define UT_173                           0x00000859L

//
// MessageId: UT_174
//
// MessageText:
//
//  UT_174: %1$sInvalid style name: \"%2$s\".
//
#define UT_174                           0x0000085AL

//
// MessageId: UT_175
//
// MessageText:
//
//  UT_175: Stack fault
//
#define UT_175                           0x0000085BL

//
// MessageId: UT_176
//
// MessageText:
//
//  UT_176: String '%1$s' longer than %2$d characters
//
#define UT_176                           0x0000085CL

//
// MessageId: VERS_1
//
// MessageText:
//
//  VERS_1: Invalid parent version number %1$ld .
//
#define VERS_1                           0x0000085DL

//
// MessageId: VERS_2
//
// MessageText:
//
//  VERS_2: Invalid child version number %1$ld .
//
#define VERS_2                           0x0000085EL

//
// MessageId: VERS_3
//
// MessageText:
//
//  VERS_3: Parent and child cannot be the same version (%1$ld) .
//
#define VERS_3                           0x0000085FL

//
// MessageId: VERS_4
//
// MessageText:
//
//  VERS_4: Adding dependency (%1$ld,%2$ld) would cause a cycle in the dependency relationships.
//
#define VERS_4                           0x00000860L

//
// MessageId: VERS_5
//
// MessageText:
//
//  VERS_5: Dependency (%1$ld,%2$ld) is already present or implied by existing dependencies.
//
#define VERS_5                           0x00000861L

//
// MessageId: VERS_6
//
// MessageText:
//
//  VERS_6: Invalid child version number %1$ld .
//
#define VERS_6                           0x00000862L

//
// MessageId: VERS_7
//
// MessageText:
//
//  VERS_7: No active version, cannot create dependency.
//
#define VERS_7                           0x00000863L

//
// MessageId: VERS_8
//
// MessageText:
//
//  VERS_8: Child version %1$ld is not the active version, cannot create dependency.
//
#define VERS_8                           0x00000864L

//
// MessageId: VERS_9
//
// MessageText:
//
//  VERS_9: Child version %1$ld is not the active version, cannot create dependency.
//
#define VERS_9                           0x00000865L

//
// MessageId: VERS_10
//
// MessageText:
//
//  VERS_10: Sequence '%1$s' is not defined, cannot allocate generation number.
//
#define VERS_10                          0x00000866L

//
// MessageId: VERS_11
//
// MessageText:
//
//  VERS_11: Parent version %1$ld for dependency to %2$ld does not exist,\n  cannot create dependency.
//
#define VERS_11                          0x00000867L

//
// MessageId: VERS_12
//
// MessageText:
//
//  VERS_12: Error updating next generation number in sequence table,\n  cannot create dependency.
//
#define VERS_12                          0x00000868L

//
// MessageId: VERS_13
//
// MessageText:
//
//  VERS_13: Can't add version dependency (%1$ld,%2$ld) to cache.
//
#define VERS_13                          0x00000869L

//
// MessageId: VERS_14
//
// MessageText:
//
//  VERS_14: Issuing transaction rollback to recover from previous errors.
//
#define VERS_14                          0x0000086AL

//
// MessageId: VERS_15
//
// MessageText:
//
//  VERS_15: Invalid version dependents query options.
//
#define VERS_15                          0x0000086BL

//
// MessageId: VERS_16
//
// MessageText:
//
//  VERS_16: Version number for dependents query must be greater than 0.
//
#define VERS_16                          0x0000086CL

//
// MessageId: VERS_17
//
// MessageText:
//
//  VERS_17: No version dependents query active.
//
#define VERS_17                          0x0000086DL

//
// MessageId: VERS_18
//
// MessageText:
//
//  VERS_18: No version dependents query active.
//
#define VERS_18                          0x0000086EL

//
// MessageId: VERS_19
//
// MessageText:
//
//  VERS_19: Version %1$d must be greater than 0, cannot calculate dependencies.
//
#define VERS_19                          0x0000086FL

//
// MessageId: VERS_20
//
// MessageText:
//
//  VERS_20: Invalid generation number %1$ld, cannot add new generation
//
#define VERS_20                          0x00000870L

//
// MessageId: VERS_21
//
// MessageText:
//
//  VERS_21: Invalid version numbers (%1$ld,%2$ld) for dependency check.
//
#define VERS_21                          0x00000871L

//
// MessageId: VERS_22
//
// MessageText:
//
//  VERS_22: Invalid sentinel row version (%1$ld,%2$ld) detected.
//
#define VERS_22                          0x00000872L

//
// MessageId: VERS_23
//
// MessageText:
//
//  VERS_23: Invalid key type
//
#define VERS_23                          0x00000873L

//
// MessageId: VERS_24
//
// MessageText:
//
//  VERS_24: Invalid version number %1$d, cannot override N-Versions mode.
//
#define VERS_24                          0x00000874L

//
// MessageId: VERS_25
//
// MessageText:
//
//  VERS_25: Invalid version selection set query options.
//
#define VERS_25                          0x00000875L

//
// MessageId: VERS_26
//
// MessageText:
//
//  VERS_26: No version selection set query active.
//
#define VERS_26                          0x00000876L

//
// MessageId: VERS_27
//
// MessageText:
//
//  VERS_27: No version selection set query active.
//
#define VERS_27                          0x00000877L

//
// MessageId: VERS_28
//
// MessageText:
//
//  VERS_28: Version used cache not initialized
//
#define VERS_28                          0x00000878L

//
// MessageId: VERS_29
//
// MessageText:
//
//  VERS_29: Unable to add `%1$s' to version used cache
//
#define VERS_29                          0x00000879L

//
// MessageId: VERS_30
//
// MessageText:
//
//  VERS_30: Unable to add `%1$s' to version used cache
//
#define VERS_30                          0x0000087AL

//
// MessageId: VERS_31
//
// MessageText:
//
//  VERS_31: Unable to create hash table
//
#define VERS_31                          0x0000087BL

//
// MessageId: VERS_32
//
// MessageText:
//
//  VERS_32: Error updating next generation number in dependency,\n  cannot create dependency.
//
#define VERS_32                          0x0000087CL

//
// MessageId: VERS_33
//
// MessageText:
//
//  VERS_33: Error rolling back dependency from dependency table,\n  cannot create dependency.
//
#define VERS_33                          0x0000087DL

//
// MessageId: VERS_34
//
// MessageText:
//
//  VERS_34: No generation query active.
//
#define VERS_34                          0x0000087EL

//
// MessageId: VERS_35
//
// MessageText:
//
//  VERS_35: Cannot activate scenario set.
//
#define VERS_35                          0x0000087FL

//
// MessageId: VERS_36
//
// MessageText:
//
//  VERS_36: Incompatible base version for current selection set.
//
#define VERS_36                          0x00000880L

//
// MessageId: VERS_37
//
// MessageText:
//
//  VERS_37: All version multibranch is not implemented.
//
#define VERS_37                          0x00000881L

//
// MessageId: VERS_38
//
// MessageText:
//
//  VERS_38: Insert query not implemented in all-version mode.
//
#define VERS_38                          0x00000882L

//
// MessageId: VERS_39
//
// MessageText:
//
//  VERS_39: Insert query not implemented in recent mode.
//
#define VERS_39                          0x00000883L

//
// MessageId: VERS_40
//
// MessageText:
//
//  VERS_40: Cannot enable index for mb recent query.
//
#define VERS_40                          0x00000884L

//
// MessageId: VERS_41
//
// MessageText:
//
//  VERS_41: Cannot do mb recent query. No next version.
//
#define VERS_41                          0x00000885L

//
// MessageId: VERS_42
//
// MessageText:
//
//  VERS_42: Cannot select. No primary key.
//
#define VERS_42                          0x00000886L

//
// MessageId: VERS_43
//
// MessageText:
//
//  VERS_43: Insert query not available for single branch queries.
//
#define VERS_43                          0x00000887L

//
// MessageId: VERS_44
//
// MessageText:
//
//  VERS_44: vctx->db_is_mb is not initialised.
//
#define VERS_44                          0x00000888L

//
// MessageId: VERS_45
//
// MessageText:
//
//  VERS_45: Cannot find version %1$ld.
//
#define VERS_45                          0x00000889L

//
// MessageId: VERS_46
//
// MessageText:
//
//  VERS_46: Parent next_version %1$ld is not a scenario set.
//
#define VERS_46                          0x0000088AL

//
// MessageId: VERS_47
//
// MessageText:
//
//  VERS_47: Version %1$ld conflicts with parent's %2$s %3$ld.
//
#define VERS_47                          0x0000088BL

//
// MessageId: VERS_48
//
// MessageText:
//
//  VERS_48: Base version %1$ld cannot depend on base version %2$ld.
//
#define VERS_48                          0x0000088CL

//
// MessageId: VERS_49
//
// MessageText:
//
//  VERS_49: Invalid version type for selection set.
//
#define VERS_49                          0x0000088DL

//
// MessageId: VERS_50
//
// MessageText:
//
//  VERS_50: Incompatible base version for current selection set.
//
#define VERS_50                          0x0000088EL

//
// MessageId: VERS_51
//
// MessageText:
//
//  VERS_51: Invalid option (mb_insert_clause) for vers_sel_qual.
//
#define VERS_51                          0x0000088FL

//
// MessageId: EH_MEMALLOC_HIST
//
// MessageText:
//
//  Could not allocate memory for a new error history.
//
#define EH_MEMALLOC_HIST                 0x00000890L

//
// MessageId: EH_MEMALLOC_CTXT
//
// MessageText:
//
//  Could not allocate memory for an error context.
//
#define EH_MEMALLOC_CTXT                 0x00000891L

//
// MessageId: EH_MEMALLOC_CTXT_DATA
//
// MessageText:
//
//  Could not allocate memory for error context data.
//
#define EH_MEMALLOC_CTXT_DATA            0x00000892L

//
// MessageId: EH_MEMALLOC_CLASS
//
// MessageText:
//
//  Could not allocate memory for an error class.
//
#define EH_MEMALLOC_CLASS                0x00000893L

//
// MessageId: EH_MEMALLOC_ELEMENT
//
// MessageText:
//
//  Could not allocate memory for an error element.
//
#define EH_MEMALLOC_ELEMENT              0x00000894L

//
// MessageId: EH_NULLPTR
//
// MessageText:
//
//  An unexpected null pointer was passed.
//
#define EH_NULLPTR                       0x00000895L

//
// MessageId: EH_NOMEM
//
// MessageText:
//
//  Out of memory!
//
#define EH_NOMEM                         0x00000896L

//
// MessageId: EH_LOG_ORDER
//
// MessageText:
//
//  The error logging functions were called in the wrong order. \nTheerror must be logged first, followed optionally by classes and/or context.
//
#define EH_LOG_ORDER                     0x00000897L

//
// MessageId: EH_ERROR_NOTFOUND
//
// MessageText:
//
//  The requested error was not found.
//
#define EH_ERROR_NOTFOUND                0x00000898L

//
// MessageId: EH_TRAIL_EMPTY
//
// MessageText:
//
//  No error has been logged yet.
//
#define EH_TRAIL_EMPTY                   0x00000899L

//
// MessageId: EH_MARKER
//
// MessageText:
//
//  End of Error
//
#define EH_MARKER                        0x0000089AL

//
// MessageId: EH_SUBTRAIL_END
//
// MessageText:
//
//  Could not end current embedded list as the current list is the main trail. Possibly the function eh_SubTrailEnd was called out of order.
//
#define EH_SUBTRAIL_END                  0x0000089BL

//
// MessageId: DBGQL_INSERTFAILED
//
// MessageText:
//
//  Row insert failed.
//
#define DBGQL_INSERTFAILED               0x0000089CL

//
// MessageId: NLS_GENERIC
//
// MessageText:
//
//  
//
#define NLS_GENERIC                      0x0000089DL

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
//  Unable to create db
//
#define FDORDBMS_3                       0x000008A0L

//
// MessageId: FDORDBMS_4
//
// MessageText:
//
//  Unable to delete db
//
#define FDORDBMS_4                       0x000008A1L

//
// MessageId: FDORDBMS_5
//
// MessageText:
//
//  Maximum classes exceeded
//
#define FDORDBMS_5                       0x000008A2L

//
// MessageId: FDORDBMS_6
//
// MessageText:
//
//  Error loading class mapping
//
#define FDORDBMS_6                       0x000008A3L

//
// MessageId: FDORDBMS_7
//
// MessageText:
//
//  Class id not found
//
#define FDORDBMS_7                       0x000008A4L

//
// MessageId: FDORDBMS_8
//
// MessageText:
//
//  Class name not found
//
#define FDORDBMS_8                       0x000008A5L

//
// MessageId: FDORDBMS_9
//
// MessageText:
//
//  Unable to open connection
//
#define FDORDBMS_9                       0x000008A6L

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
//  Primary key does not exist
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
//  Expected object property
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
//  Property not found
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
// MessageId: FDORDBMS_32
//
// MessageText:
//
//  Within requires 2 points(4 doubles values)
//
#define FDORDBMS_32                      0x000008BDL

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
//  Table name or Primary key for class '%1$s' does not exist
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
//  Column %1$s not found
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
//  Unable to initialize dbi
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
//  Property '%1$s' not found
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
//  Property '%1$s' not defined for class '%2$s'
//
#define FDORDBMS_59                      0x000008D8L

//
// MessageId: FDORDBMS_60
//
// MessageText:
//
//  Schema not found for class '%1$s'
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
//  Property '%1$s' is not an object proproperty type
//
#define FDORDBMS_65                      0x000008DEL

//
// MessageId: FDORDBMS_66
//
// MessageText:
//
//  Missing or badly defined target class for property '%1$s' of class '%2$s'
//
#define FDORDBMS_66                      0x000008DFL

//
// MessageId: FDORDBMS_67
//
// MessageText:
//
//  Property '%1$s' is an object property and cannot be returned through a basic type; use GetObject
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
// MessageId: FDORDBMS_108
//
// MessageText:
//
//  Expected object property
//
#define FDORDBMS_108                     0x00000909L

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
//  Invalid geometry as spatial constraint
//
#define FDORDBMS_110                     0x0000090BL

//
// MessageId: FDORDBMS_111
//
// MessageText:
//
//  Invalid spatial operation
//
#define FDORDBMS_111                     0x0000090CL


