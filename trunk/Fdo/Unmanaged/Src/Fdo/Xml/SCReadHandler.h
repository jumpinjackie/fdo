#ifndef FDO_XML_SCREADHANDLER_H_
#define FDO_XML_SCREADHANDLER_H_
//
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Commands/SpatialContext/SpatialContextExtentType.h>

// SCReadHandler is a helper class for FdoXmlSpatialContextReader
// It handles the parsing of the spatial contexts, using a state machine.
//
// This class is not part of the FDO. It only intended to be used by
// FdoXmlSpatialContextReader.

class FdoXmlSCReadHandler : 
    public FdoIDisposable
{
friend class FdoXmlSpatialContextReader;

public:

    // Creates the handler
    static FdoXmlSCReadHandler* Create()
    {
        return new FdoXmlSCReadHandler();
    }

    // Initializes the handler. Must be called before reading each spatial context.
    void Setup( FdoXmlFlags* pXmlFlags );

    // Decodes any escaped characters in Spatial Context and Coordinate System names. 
    FdoStringP DecodeName ( FdoStringP name, FdoXmlReader* reader );

protected:
    FdoXmlSCReadHandler() {}
    ~FdoXmlSCReadHandler() {}

    virtual void Dispose()
    {
        delete this;
    }

private:
    // Move to the next state, based on the given transition.
    // Returns the SAX Handler for the current element's sub-elements.
    // Returns NULL if this SAX Handler will be used.
    virtual FdoXmlSaxHandler* doTransition( 
        FdoBoolean isStart,                 // true - starting an element, false - ending an element
        FdoXmlSaxContext* context,          // SAX parsing context info
        FdoString* uri,                     // element namespace
        FdoString* name,                    // element local name
        FdoXmlAttributeCollection* atts     // element attributes, set only if isStart=true
    );

    // SAX Handler for skipping an XML element
    FdoXmlSkipElementHandlerP mSkipHandler;
    // SAX Handler for reading an element's simple content
    FdoXmlCharDataHandlerP mContentHandler;

    // The following hold the spatial context that was read.
    // When mSCName is blank, no spatial context has been read.
    FdoStringP              mID;
    FdoStringP              mCsysID;
    FdoStringP              mSCName;
    FdoStringP              mDescription;
    FdoStringP              mCoordSysName;
    FdoStringP              mCoordSysWkt;

    FdoSpatialContextExtentType mExtentType;
    double                  mMinX, mMinY, mMaxX, mMaxY;

    FdoDouble               mXYTolerance;
    FdoDouble               mZTolerance;

    // The following keep track of the current state.
    FdoBoolean              mFirst;                 // true if the next element to read is the first (root) element in the document
    FdoXmlFlagsP            mXmlFlags;
    FdoInt32                mState;
    FdoStringP              mPrevElement;
    FdoBoolean              mPrevStart;
    FdoBoolean              mStopParse;
    FdoBoolean              mCsysFound;

    FdoStringP              mElementPrefix;
};


#endif

