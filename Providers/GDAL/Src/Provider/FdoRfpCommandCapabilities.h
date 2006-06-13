/*
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpCommandCapabilities.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPCOMMANDCAPABILITIES_H
#define FDORFPCOMMANDCAPABILITIES_H

#ifdef _WIN32
#pragma once
#endif


//
// Implementation of FdoICommandCapabilities
//
class FdoRfpCommandCapabilities : public FdoICommandCapabilities
{
//
// Data members
//
private:
	wchar_t **mCommands;
	
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:	
	~FdoRfpCommandCapabilities(void);
	virtual void Dispose();
public:
    FdoRfpCommandCapabilities(void);
	
// 
// Exposed functions
//
public:
    //Returns an array of the CommandTypes supported by the feature provider.
    virtual int* GetCommands(int& size);

    // Returns true if commands support parameterization, false otherwise.
    virtual bool SupportsParameters();

    // Returns true if the feature provider supports command execution timeout.
    virtual bool SupportsTimeout();

    /// <summary>Determines if the feature provider can use expressions for properties with Select and SelectAggregate commands.</summary>
    /// <returns>Returns true if the feature provider supports select expressions.</returns> 
    virtual bool SupportsSelectExpressions() { return false; }

    /// <summary>Determines if simple functions can be used in Select and SelectAggregates command. 
    /// Aggregate functions can only be used in the SelectAggregates command. Capability for using SelectAggregates is found in 
    /// CommandCapabilities.</summary>
    /// <returns>Returns true if the feature provider supports select simple functions.</returns> 
    virtual bool SupportsSelectFunctions() { return false; }

    /// <summary>Determines if Distinct can be used with SelectAggregates. This can be true only 
    /// if the SelectAggregates command is supported by the provider.</summary>
    /// <returns>Returns true if the feature provider supports select distinct.</returns> 
    virtual bool SupportsSelectDistinct() { return false; }

    /// <summary>Determines  if ordering is available in the Select and SelectAggregates command.</summary>
    /// <returns>Returns true if the feature provider supports select ordering.</returns> 
    virtual bool SupportsSelectOrdering() { return false; }

    /// <summary>Determines if a grouping criteria is available in the SelectAggregates command. 
    /// This can be true only if the SelectAggregates command is supported by the provider. Note that aggregate 
    /// functions can be supported without also supporting grouping criteria (but not vice versa).</summary>
    /// <returns>Returns true if the feature provider supports select grouping.</returns> 
    virtual bool SupportsSelectGrouping() { return false; }

};

#endif // FDORFPCOMMANDCAPABILITIES_H
