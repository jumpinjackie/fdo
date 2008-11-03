/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ARCSDEPROVIDER_H
#define ARCSDEPROVIDER_H

// General descriptive items of ArcSDE Provier:
#define ARCSDE_PROVIDER_NAME						L"OSGeo.ArcSDE.3.3"
#define ARCSDE_PROVIDER_DEFAULT_DISPLAY_NAME	"OSGeo FDO Provider for ArcSDE"
#define ARCSDE_PROVIDER_DEFAULT_DESCRIPTION		"Read/write access to an ESRI ArcSDE-based data store, using Oracle and SQL Server."
#define ARCSDE_PROVIDER_VERSION					L"3.3.0.0"
#define ARCSDE_FDO_VERSION						L"3.3.0.0"

// Hardcode datastore names:
#define ArcSDEDefaultDataStore L"Default Datastore"

// Connection property strings:
const wchar_t CONNECTIONPROPERTY_SERVER[] = L"Server";
const wchar_t CONNECTIONPROPERTY_INSTANCE[] = L"Instance";
const wchar_t CONNECTIONPROPERTY_DATASTORE[] = L"Datastore";
const wchar_t CONNECTIONPROPERTY_USERNAME[] = L"Username";
const wchar_t CONNECTIONPROPERTY_PASSWORD[] = L"Password";

// Non-"well-known" function/argument names:
#define ARCSDE_FUNCTION_DOUBLE_ARGUMENT_NAME L"dblValue"


// ArcObjects/ArcSDE system tables/users:
#define SDE_USER                                  L"SDE"
#define SDE_USER_MULTIBYTE                         "SDE"
#define OPTIONAL_SYSTEM_TABLE_PREFIX              L"SDE_"
#define OPTIONAL_SYSTEM_TABLE_PREFIX_MULTIBYTE     "SDE_"
#define DBO_USER                                  L"DBO"
#define DBO_USER_MULTIBYTE                         "DBO"
#define ARCOBJECT_PREFIX                          L"GDB_"
#define SDE_SYSTEM_TABLE_STATE_LINEAGES           L"STATE_LINEAGES"
#define SDE_SYSTEM_TABLE_STATE_LINEAGES_MULTIBYTE  "STATE_LINEAGES"

// To bracket various condition and expressions
#define                OPEN_PAREN                        L" ("
#define                CLOSE_PAREN                       L") "

// For logical operations
#define                LOGICAL_AND                       L" AND "
#define                LOGICAL_OR                        L" OR "
#define                LOGICAL_NOT                       L" NOT "

// For comparison operation
#define                EQUAL_OP                          L" = "
#define                NOT_EQUAL_OP                      L" != "
#define                GREATER_THAN_OP                   L" > "
#define                GREATER_OR_EQUAL_OP               L" >= "
#define                LESS_THAN_OP                      L" < "
#define                LESS_OR_EQUAL_OP                  L" <= "
#define                LIKE_OP                           L" LIKE "
#define                ESCAPE                            L" ESCAPE '\\' "


// For arthmetic operation
#define                ARITHMETIC_PLUS                   L" + "
#define                ARITHMETIC_MINUS                  L" - "
#define                ARITHMETIC_MULT                   L" * "
#define                ARITHMETIC_DIV                    L" / "

// For In operation
#define                IN_OP                             L" IN "
#define                IN_SEPARATOR                      L" , "

// For Is Null operation
#define                IS_NULL_OP                        L" IS NULL "

// Misc
#define                WHERE                             L" WHERE "
#define                QUOTE                             L"'"
#define                UNARY_MINUS                       L" -"
#define                COMMA                             L","
#define                SPACE                             L" "

// For Datetime literals
#define                ORACLE_TODATE                     L" TO_DATE"
#define                ORACLE_DATEFORMAT                 L"YYYY/MM/DD"  // Same as MS SQL Server
#define                ORACLE_TIMEFORMAT                 L"HH24:MI:SS"
#define                ORACLE_DATEFORMAT_PRINTF          L"%04d/%02hd/%02hd"
#define                ORACLE_TIMEFORMAT_PRINTF          L"%02hd:%02hd:%02.0f"
#define                ORACLE_NLS_AMERICAN               L"NLS_LANGUAGE=American"
#define                SQLSERVER_DATEFORMAT              L"YYYY/MM/DD"  // Same as Oracle
#define                SQLSERVER_TIMEFORMAT              L"HH:MM:SS:III"
#define                SQLSERVER_DATEFORMAT_PRINTF       L"%04d/%02hd/%02hd"
#define                SQLSERVER_TIMEFORMAT_PRINTF       L"%02hd:%02hd:%02.0f:%03d"
#define                INFORMIX_DATEFORMAT               L"YYYY-MM-DD"
#define                INFORMIX_TIMEFORMAT               L"HH:MI:SS.FFFFF"
#define                INFORMIX_DATEFORMAT_PRINTF        L"%04d-%02hd-%02hd"
#define                INFORMIX_TIMEFORMAT_PRINTF        L"%02hd:%02hd:%02.5f"
#define                INFORMIX_SUFFIX_DATE              L"YEAR TO DAY"
#define                INFORMIX_SUFFIX_TIME              L"HOUR TO FRACTION(5)"
#define                INFORMIX_SUFFIX_DATETIME          L"YEAR TO FRACTION(5)"
#define                DB2_DATEFORMAT_PRINTF             L"%04d-%02hd-%02hd"
#define                DB2_TIMEFORMAT_PRINTF             L"%02hd:%02hd:%02.0f"
#define                DB2_DATETIMEFORMAT_PRINTF         L"%04d-%02hd-%02hd %02hd:%02hd:%02.6f"
#define                DB2_PREFIX_DATE                   L"DATE"
#define                DB2_PREFIX_TIME                   L"TIME"
#define                DB2_PREFIX_DATETIME               L"TIMESTAMP"


// For ORDER BY clause:
#define                ORDERBY_CLAUSE                    L" ORDER BY "
#define                ORDERBY_SEPARATOR                 L" , "
#define                ORDERBY_ASCENDING                 L" ASC "
#define                ORDERBY_DESCENDING                L" DESC "



// For unhandled SDE column types:
#define                FDODATATYPE_UNHANDLEDSDETYPE      (-2)


#endif // ARCSDEPROVIDER_H

