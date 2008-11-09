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

#ifndef ARCSDEFEATUREREADER_H
#define ARCSDEFEATUREREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDELockConflictReader;

class ArcSDEFeatureReader :
    public virtual ArcSDEReader
{
    typedef ArcSDEReader superclass;

    friend class ArcSDELockConflictReader;
    friend class ArcSDESelectCommand;

protected:
    FdoPtr<FdoClassDefinition> mClassDefPruned;
    FdoPtr<FdoFilter> mFilter;
    ArcSDELockConflictReader* mConflictReader;
    FdoLockStrategy mLockStrategy;
    FdoLockType mLockType;
    bool mInitialized;
    SE_LOG mLog;

public:
    ArcSDEFeatureReader (ArcSDEConnection* connection, FdoClassDefinition* definition, FdoFilter* filter, FdoIdentifierCollection *propertiesToSelect);
    virtual ~ArcSDEFeatureReader (void);

    virtual void Dispose();

    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEFeatureReader & operator= (const ArcSDEFeatureReader &right);

    /// <summary>Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.</summary>
    /// <returns>Returns the class definition object.</returns> 
    FdoClassDefinition* GetClassDefinition ();

    /// <summary>Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.</summary>
    /// <returns>Returns the depth</returns> 
    FdoInt32 GetDepth ();

    /// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the nested feature reader</returns> 
    FdoIFeatureReader* GetFeatureObject (FdoString* propertyName);

    /// <summary>Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    bool ReadNext ();

    /// <summary>Closes the FdoIFeatureReader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
    void Close ();

protected:
	ArcSDEFeatureReader() {};
    void PrepareStream ();
	bool ContainsSDEValidExpressionsOnly (bool& filterValid, bool& selectListValid);
    ColumnDefinition* createColumnDef(int column, SE_COLUMN_DEF* definition, wchar_t* propertyName, wchar_t* funcName = NULL);
    ColumnDefinition* getColumnDef (const wchar_t* identifier);
    void getColumnDefs ();
    void SetLockConflictReader (ArcSDELockConflictReader* conflicts);
    ArcSDELockConflictReader* GetLockConflictReader ();
    void SetLockStrategy (FdoLockStrategy strategy);
    FdoLockStrategy GetLockStrategy ();
    void SetLockType (FdoLockType type);
    FdoLockType GetLockType ();
};

#endif // ARCSDEFEATUREREADER_H

