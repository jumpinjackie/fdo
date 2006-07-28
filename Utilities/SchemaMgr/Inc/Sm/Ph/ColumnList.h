#ifndef FDOSMPHCOLUMNLIST_H
#define FDOSMPHCOLUMNLIST_H		1
// 

//
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Mgr.h>

class FdoSmPhColumnList : public FdoStringCollection
{
public:
    /// \brief
    /// Constructs a new empty column list
    /// 
    /// \return
    /// Returns FdoSmPhColumnList
    /// 
	static FdoSmPhColumnList* Create(FdoSmPhMgrP mgr);

    /// \brief
    /// Creates a copy of column list
    /// 
    /// \param src 
    /// Input the source list
    /// 
    /// \return
    /// Returns FdoSmPhColumnList
    /// 
	static FdoSmPhColumnList* Create( FdoSmPhMgrP mgr, const FdoStringCollection& src);

    /// \brief
    /// Creates a copy of column list
    /// 
    /// \param src 
    /// Input pointer to the column list
    /// 
    /// \return
    /// Returns FdoSmPhColumnList
    /// 
	static FdoSmPhColumnList* Create( FdoSmPhMgrP mgr, const FdoStringCollection* src);

    /// \brief
    /// Creates a column list that is tokenize from a string.
    /// The collection contains an element for each token.
    /// 
    /// \param inString 
    /// Input the string to tokenize.
    /// \param delimiters 
    /// Input list of single character token delimiters.
    /// 
    /// \return
    /// Returns FdoStringCollection
    /// 
	static FdoSmPhColumnList* Create( FdoSmPhMgrP mgr, const FdoStringP& inString, const FdoString* delimiters );

    FdoSmPhMgrP GetManager();

    FdoStringP GetDbString( FdoInt32 idx );

    /// \brief
    /// Concatenates the strings in this collection.
    /// 
    /// \param separator 
    /// Input separate each collection string with this separator string.
    /// 
    /// \return
    /// Returns the concatenation if all strings in this collection.
    /// 
    /// Returns a concatenation of all the strings in this collection, 
    /// separated by the given separator.
	FdoStringP ToString( const FdoString* separator = L", " );

protected:
/// \cond DOXYGEN-IGNORE
    FdoSmPhColumnList(void) {}
	FdoSmPhColumnList(FdoSmPhMgrP mgr);
	FdoSmPhColumnList(FdoSmPhMgrP mgr, const FdoStringCollection& src);
	FdoSmPhColumnList(FdoSmPhMgrP mgr, const FdoStringCollection* src);
	FdoSmPhColumnList(FdoSmPhMgrP mgr, const FdoStringP& inString, const FdoString* delimiters );

	virtual ~FdoSmPhColumnList(void);

/// \endcond
    virtual void Dispose()
    {
        delete this;
    }

private:
    // Back pointer, so no refcount held. Otherwise circular reference introduced.
    FdoSmPhMgr* mMgr;
};

typedef FdoPtr<FdoSmPhColumnList> FdoSmPhColumnListP;
#if 0
/// \brief
/// FdoStringsP is a FdoPtr on FdoStringCollection, provided for convenience.
class FdoStringsP : public FdoPtr<FdoStringCollection>
{
public:
    FdoStringsP() {}
    FdoStringsP( const FdoStringsP& src ) 
        : FdoPtr<FdoStringCollection>(src)
    {}

    FdoStringsP( FdoStringCollection* src )
        : FdoPtr<FdoStringCollection>(src)
    {}

    ~FdoStringsP() {}
    /// \brief
    /// Copies a string collection
    /// 
    /// \param src 
    /// Input the source collection
    /// 
    /// \return
    /// Returns the copy FdoStringCollection
    /// 
	GIS_API FdoStringCollection* operator=( FdoStringCollection* src ); 

    /// \brief
    /// Concatenates two string collections
    /// 
    /// \param coll2 
    /// Input collection to append to the end of this collection
    /// 
    /// \return
    /// Returns the concatenated FdoStringCollection
    /// 
	GIS_API const FdoStringsP operator+( const FdoStringsP coll2 ) const;

    /// \brief
    /// Concatenates a string collection and a string
    /// 
    /// \param str2 
    /// Input stromg to append to the end of this collection
    /// 
    /// \return
    /// Returns the concatenated FdoStringCollection
    /// 
	GIS_API const FdoStringsP operator+( const FdoStringP str2 ) const;

    /// \brief
    /// Concatenates two string collections
    /// 
    /// \param coll2 
    /// Input collection to append to the end of this collection
    /// 
    /// \return
    /// Returns the concatenated FdoStringCollection
    /// 
	GIS_API FdoStringsP operator+=( FdoStringsP coll2 );

    /// \brief
    /// Concatenates a string collection and a string
    /// 
    /// \param str2 
    /// Input string to append to the end of this collection
    /// 
    /// \return
    /// Returns the concatenated FdoStringCollection
    /// 
	GIS_API FdoStringsP operator+=( FdoStringP str2 );

};
#endif

#endif


