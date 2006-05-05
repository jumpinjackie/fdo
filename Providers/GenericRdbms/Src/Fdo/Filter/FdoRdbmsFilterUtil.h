#ifndef FDORDBMSFILTERUTIL_H
#define FDORDBMSFILTERUTIL_H    1
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
 *
 */


class FdoRdbmsFilterUtil
{
public:
    //
    // Constructors/Destructor
    FdoRdbmsFilterUtil();
    ~FdoRdbmsFilterUtil();

    //
    // Given a class name of the form <main class_name>.<obj prop> and a filter based on the obj_class property name,
    // This method will modify the filter to use property relative to the main class and return a new class name that
    // represent the main class.
    static FdoIdentifier *ConvertFilterToMainClass( FdoIdentifier *className, FdoFilter  *filter );

    //
    // This method undo what ConvertFilterToMainClass did and reset the filter to its original content.
    static void ConvertFilterToObjectClass( FdoIdentifier *className, FdoFilter  *filter );

    //
    // This helper method is used to get the feature ids from simple filters. It can be used for optimization reason to avoid making
    // a round trip to the database to fetch feature ids which are already provided in the filter.
    // Only 2 filter forms can be used so far: "featidcol=<val>" and "featidcol in (val1,val2,...,valn)"
    static  bool FeatIdsFromFilter( const wchar_t *featIdName, FdoFilter  *filter, int **FeatIdArray, int *count );

    // The following two functions return the feature id property for the given class.
    // If the class is a feature class, then the returned collection has a single element
    // which is the feature id column.
    // The caller is responsible for releasing this list.
    // If the class is not a feature class then NULL is returned.
    static FdoIdentifierCollection* GetFeatIdSelList( DbiConnection *dbi_connection, const wchar_t *className );

    static FdoIdentifierCollection* GetFeatIdSelList( const FdoSmLpClassDefinition* classDefinition );

	static FdoIdentifierCollection* GetIdentPropList( const FdoSmLpClassDefinition* classDefinition );
};

#endif // FDORDBMSFILTERUTIL_H
