#ifndef SQLSERVEROVTEXTINROWOPTION_H
#define SQLSERVEROVTEXTINROWOPTION_H
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
 *
 */

#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>

/// \brief
/// Enumerated type representing the possible text-in-row options for SQL Server overrides
/// <param name="SqlServerOvTextInRowOption_InRow">
/// Text, ntext and image data are stored within the row's 8K page.
/// </param>
/// <param name="SqlServerOvTextInRowOption_notInRow">
/// Text, ntext and image data are not stored within the row's 8K page.
/// </param>
/// <param name="SqlServerOvTextInRowOption_Default">
/// The text-in-row override is inherited from somewhere else.
/// </param>
enum SqlServerOvTextInRowOption
{
    SqlServerOvTextInRowOption_InRow,
    SqlServerOvTextInRowOption_NotInRow,
    SqlServerOvTextInRowOption_Default,
};

/// \brief
/// XML string constants corresponding to the enumerated type SqlServerOvTextInRowOption.
namespace FdoSqlServerOverrides
{
    static const FdoString* TextInRowOption_InRow       = L"InRow";
    static const FdoString* TextInRowOption_NotInRow    = L"NotInRow";
    static const FdoString* TextInRowOption_Default     = L"Default";
}

static SqlServerOvTextInRowOption TextInRow_StringToEnum(FdoString* textInRowString, FdoXmlSaxContext* pContext = NULL)
{
    SqlServerOvTextInRowOption ret;

    if (0==wcscmp(textInRowString, FdoSqlServerOverrides::TextInRowOption_InRow))
        ret = SqlServerOvTextInRowOption_InRow;
    else if (0==wcscmp(textInRowString, FdoSqlServerOverrides::TextInRowOption_NotInRow))
        ret = SqlServerOvTextInRowOption_NotInRow;
    else if (0==wcscmp(textInRowString, FdoSqlServerOverrides::TextInRowOption_Default))
        ret = SqlServerOvTextInRowOption_Default;
    else
    {
        ret = SqlServerOvTextInRowOption_Default;
		if (pContext != NULL)
		{
	        FdoPtr<FdoException> e = FdoException::Create(L"TODO");
		    pContext->AddError(e);
		}
    }

    return ret;
}

static FdoString* TextInRow_EnumToString(SqlServerOvTextInRowOption textInRowEnum)
{
    if (textInRowEnum == SqlServerOvTextInRowOption_InRow)
        return FdoSqlServerOverrides::TextInRowOption_InRow;
    else if (textInRowEnum == SqlServerOvTextInRowOption_NotInRow)
        return FdoSqlServerOverrides::TextInRowOption_NotInRow;
    else if (textInRowEnum == SqlServerOvTextInRowOption_Default)
        return FdoSqlServerOverrides::TextInRowOption_Default;
    else
        throw FdoException::Create(L"TODO");
}


#endif  // SQLSERVEROVTEXTINROWOPTION_H


