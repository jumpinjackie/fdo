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

#ifndef ARCSDELONGTRANSACTIONREADER_H
#define ARCSDELONGTRANSACTIONREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDELongTransactionReader :
    public virtual FdoILongTransactionReader
{
    ArcSDEConnection* mConnection;
    SE_VERSIONINFO* mInfo;
    LONG mCount;
    LONG mIndex; // current record index
    // cached values:
    FdoString* mName;
    FdoString* mDescription;
    FdoString* mOwner;

public:
    ArcSDELongTransactionReader (ArcSDEConnection* connection, LONG count, SE_VERSIONINFO* info);
    virtual ~ArcSDELongTransactionReader (void);

    virtual void Dispose();

    // Prevent the use of the Assignment Operation by defining it and not implemeting it.
    // DO NOT IMPLEMENT
    ArcSDELongTransactionReader & operator= (const ArcSDELongTransactionReader &right);

    // clean up cached values
    void reset ();
    // check for closed or exhausted
    void validate ();

    //
    // FdoILongTransactionReader interface
    //

    /// <summary>Gets the name of the long transaction currently being read.</summary>
    /// <returns>Returns the name of the long transaction.</returns> 
    virtual FdoString* GetName ();

    /// <summary>Gets the description of the long transaction currently being read.</summary>
    /// <returns>Returns the description of the long transaction.</returns> 
    virtual FdoString* GetDescription ();

    /// <summary>Gets the owner of the long transaction currently being read.</summary>
    /// <returns>Returns the owner name.</returns> 
    virtual FdoString* GetOwner ();

    /// <summary>Provides access to the direct descendent long transactions 
    /// for the long transaction being currently read. The caller can use the
    /// returned reader to get to the next level of descendents if required.
    /// The returned reader is empty if there are no descendents for the long
    /// transction.</summary>
    /// <returns>Returns a long transaction reader with the direct descendents
    /// for the long transaction currently being read.</returns> 
    virtual FdoILongTransactionReader* GetChildren();

    /// <summary>Provides access to the direct ascendant long transactions for
    /// the long transaction being currently read. The caller can use the 
    /// returned reader to get to the next level of ascendants if required.
    /// The returned reader is empty if there are no ascendants for the long
    /// transaction</summary>
    /// <returns>Returns a long transaction reader with the direct ascendants 
    /// for the long transaction currently being read.</returns> 
    virtual FdoILongTransactionReader* GetParents();

    /// <summary>Gets the creation date of the long transaction currently being read.</summary>
    /// <returns>Returns the date when the long transaction was created.</returns> 
    virtual FdoDateTime GetCreationDate ();

    /// <summary>Returns true if the long transaction currently being read is the
    /// active long transaction.</summary>
    /// <returns>Returns true if the long transaction is active.</returns> 
    virtual bool IsActive ();

    virtual bool IsFrozen ();

    /// <summary>Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext ();

    /// <summary>Closes the FdoILongTransactionReader object, freeing any resources it may
    /// be holding.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Close ();
};

#endif // ARCSDELONGTRANSACTIONREADER_H

