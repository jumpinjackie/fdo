//
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
//  
#include <FdoStd.h>
#include <Fdo/Xml/SpatialContextFlags.h>
#include "SCReadHandler.h"

// Spatial Context reading is done by a state machine that reads each spatial context
// and its sub-elements. Errors are logged if the sub-elements are not in the allowed
// order.
// This reader handler receives element start and end SAX events. The state transitions
// are made as these events are received.

// The following is the definition for each entry in the state transition table. 

typedef struct ParseState {
    FdoInt32                currentState;       // the current state
    FdoBoolean              isStart;            // true - starting an XML element
                                                // false - ending an XML element.
    FdoString*              nextElement;        // the element being started or ended.
                                                // "*" is a wildcard that determines the 
                                                // transition when no transition matches
                                                // the current element.
    FdoXmlFlags::ErrorLevel  errorLevel;        // Allowable error level for the transition.
                                                // If the current error level is higher than 
                                                // this level then the transition is not allowed.
    FdoInt32                nextState;          // the next state based on the previous members.
} ParseState;

// The state transition table. See the switch statement below for a description
// of these states. 

ParseState parseStates[] = {
    {0, true, L"DerivedCRS", FdoXmlFlags::ErrorLevel_High, 2},
    {0, true, L"*", FdoXmlFlags::ErrorLevel_High, 1},
    {0, false, L"*", FdoXmlFlags::ErrorLevel_High, -1},
    {1, true, L"DerivedCRS", FdoXmlFlags::ErrorLevel_High, 2},
    {1, true, L"*", FdoXmlFlags::ErrorLevel_High, 100},
    {1, false, L"*", FdoXmlFlags::ErrorLevel_High, 1},
    {2, true, L"metaDataProperty", FdoXmlFlags::ErrorLevel_High, 23},
    {2, true, L"remarks", FdoXmlFlags::ErrorLevel_High, 3},
    {2, true, L"srsName", FdoXmlFlags::ErrorLevel_High, 5},
    {3, false, L"remarks", FdoXmlFlags::ErrorLevel_High, 4},
    {4, true, L"srsName", FdoXmlFlags::ErrorLevel_High, 5},
    {5, false, L"srsName", FdoXmlFlags::ErrorLevel_High, 6},
    {6, true, L"validArea", FdoXmlFlags::ErrorLevel_High, 7},
    {7, true, L"boundingBox", FdoXmlFlags::ErrorLevel_High, 8},
    {8, true, L"pos", FdoXmlFlags::ErrorLevel_High, 9},
    {9, false, L"pos", FdoXmlFlags::ErrorLevel_High, 10},
    {10, true, L"pos", FdoXmlFlags::ErrorLevel_High, 11},
    {11, false, L"pos", FdoXmlFlags::ErrorLevel_High, 12},
    {12, false, L"boundingBox", FdoXmlFlags::ErrorLevel_High, 13},
    {13, false, L"validArea", FdoXmlFlags::ErrorLevel_High, 14},
    {14, true, L"baseCRS", FdoXmlFlags::ErrorLevel_High, 15},
    {15, false, L"baseCRS", FdoXmlFlags::ErrorLevel_High, 16},
    {15, true, L"WKTCRS", FdoXmlFlags::ErrorLevel_High, 28},
    {16, true, L"definedByConversion", FdoXmlFlags::ErrorLevel_High, 17},
    // derivedCRSType, usesCS and DerivedCRS are optional when error level is not High
    {16, true, L"derivedCRSType", FdoXmlFlags::ErrorLevel_Normal, 19},
    {16, true, L"usesCS", FdoXmlFlags::ErrorLevel_Normal, 21},
    {16, false, L"DerivedCRS", FdoXmlFlags::ErrorLevel_Normal, -1},
    {17, false, L"definedByConversion", FdoXmlFlags::ErrorLevel_High, 18},
    {18, true, L"derivedCRSType", FdoXmlFlags::ErrorLevel_High, 19},
    {18, true, L"usesCS", FdoXmlFlags::ErrorLevel_Normal, 21},
    {18, false, L"DerivedCRS", FdoXmlFlags::ErrorLevel_Normal, -1},
    {19, false, L"derivedCRSType", FdoXmlFlags::ErrorLevel_High, 20},
    {20, true, L"usesCS", FdoXmlFlags::ErrorLevel_High, 21},
    {20, false, L"DerivedCRS", FdoXmlFlags::ErrorLevel_Normal, -1},
    {21, false, L"usesCS", FdoXmlFlags::ErrorLevel_High, 22},
    {22, false, L"DerivedCRS", FdoXmlFlags::ErrorLevel_High, -1},
    /* 
     * States 23 to 27 handle FDO-specific metadata 
     * Todo: think about putting these states in their own SAX Handler.
    */
    {23, true, L"GenericMetaData", FdoXmlFlags::ErrorLevel_High, 24},
    {23, false, L"metaDataProperty", FdoXmlFlags::ErrorLevel_High, 2},
    {23, true, L"*", FdoXmlFlags::ErrorLevel_Normal, 23},
    {23, false, L"*", FdoXmlFlags::ErrorLevel_Normal, 23},
    {24, false, L"GenericMetaData", FdoXmlFlags::ErrorLevel_High, 23},
    {24, true, L"SCExtentType", FdoXmlFlags::ErrorLevel_High, 25},
    {24, true, L"XYTolerance", FdoXmlFlags::ErrorLevel_High, 26},
    {24, true, L"ZTolerance", FdoXmlFlags::ErrorLevel_High, 27},
    {24, true, L"*", FdoXmlFlags::ErrorLevel_Normal, 24},
    {24, false, L"*", FdoXmlFlags::ErrorLevel_Normal, 24},
    {25, false, L"SCExtentType", FdoXmlFlags::ErrorLevel_High, 24},
    {26, false, L"XYTolerance", FdoXmlFlags::ErrorLevel_High, 24},
    {27, false, L"ZTolerance", FdoXmlFlags::ErrorLevel_High, 24},
    /* 
     * States 28 to 33 handle a WKT coordinate system specification 
     * Todo: think about putting these states in their own SAX Handler.
    */
    {28, true, L"srsName", FdoXmlFlags::ErrorLevel_High, 29},
    {29, false, L"srsName", FdoXmlFlags::ErrorLevel_High, 30},
    {30, true, L"WKT", FdoXmlFlags::ErrorLevel_High, 31},
    {31, false, L"WKT", FdoXmlFlags::ErrorLevel_High, 32},
    {32, false, L"WKTCRS", FdoXmlFlags::ErrorLevel_High, 33},
    {33, false, L"baseCRS", FdoXmlFlags::ErrorLevel_High, 16},
    {100, true, L"DerivedCRS", FdoXmlFlags::ErrorLevel_High, 2},
    {100, true, L"*", FdoXmlFlags::ErrorLevel_High, 100},
    {100, false, L"*", FdoXmlFlags::ErrorLevel_High, 100},
    {-1, true, L"", FdoXmlFlags::ErrorLevel_High, -1},
    {-2, false, L"DerivedCRS", FdoXmlFlags::ErrorLevel_High, -1},
    {-2, true, L"*", FdoXmlFlags::ErrorLevel_High, -2},
    {-2, false, L"*", FdoXmlFlags::ErrorLevel_High, -2},
// -3 is the end of state table indicator. It must always be the last
// entry in this table.
    {-3, false, L"*", FdoXmlFlags::ErrorLevel_High, -3}
};


void FdoXmlSCReadHandler::Setup( FdoXmlFlags* pXmlFlags )
{
    mXmlFlags = FDO_SAFE_ADDREF( pXmlFlags );

    // Initialize all members to the "not read" state.
    mSCName = L"";
    mDescription = L"";
    mCsysID = L"";
    mCoordSysName = L"";
    mCoordSysWkt = L"";
    mExtentType = FdoSpatialContextExtentType_Static;

    mXYTolerance = 0.0;
    mZTolerance = 0.0;

    mState = 0;
    mPrevStart = false;
    mPrevElement = L"";
    mStopParse = false;
    mCsysFound = false;

    mElementPrefix = L"";
}

FdoXmlSaxHandler* FdoXmlSCReadHandler::doTransition( 
    FdoBoolean isStart, 
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name,
    FdoXmlAttributeCollection* atts
)
{
    FdoInt32 i;
    FdoInt32 nextState = -2;
    FdoXmlAttributeP attr;
    FdoXmlSaxHandler* ret = NULL;
    FdoXmlReaderP reader = context->GetReader();

    // Given the current state, find the transition for the current element.
    // The first element must be gml:DerivedCRS.
    //
    if ( (mState != 0) || (wcscmp( uri, FdoXml::mGmlUri)) == 0 ) {
        for ( i = 0; parseStates[i].currentState != -3; i++ ) {
            if ( (parseStates[i].currentState == mState) &&
                (parseStates[i].isStart == isStart) &&
                // Skip transitions not allowed at the current error level.
                (parseStates[i].errorLevel <= mXmlFlags->GetErrorLevel()) &&
                (wcscmp( parseStates[i].nextElement, name ) == 0) ) {
            
                nextState = parseStates[i].nextState;
                break;
            }
        }
    }

    if ( nextState == -2 ) {
        // No transition found. Check if we have a default transition for the 
        // current state.
        for ( i = 0; parseStates[i].currentState != -3; i++ ) {
            if ( (parseStates[i].currentState == mState) &&
                (parseStates[i].isStart == isStart) &&
                (parseStates[i].errorLevel <= mXmlFlags->GetErrorLevel()) &&
                (wcscmp( parseStates[i].nextElement, L"*" ) == 0) ) {
                
                nextState = parseStates[i].nextState;
                break;
            }
        }
    }

    switch ( nextState ) {

    // -1 is the end state. Either we read a spatial context or reached the 
    // end of the document. Either way, tell the XML parser to quit.
    case -1:
        mStopParse = true;
        break;

    // -2 is the error state. There is a syntax or semantic error in the current
    // spatial context
    case -2:
        if ( mState != -2 ) {
            // If we just entered the error state, log an error. Otherwise, just continue
            // to read past the current spatial context. 
            if ( mPrevStart ) {
                if ( isStart ) {
                    // We read the start of an element and then encountered an 
                    // unexpected first sub-element.
                    context->AddError( 
                        FdoExceptionP(
                            FdoException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SC_9_BADSUBELEMENT),
                                    (FdoString*) mSCName,
                                    name,
                                    (FdoString*) (mElementPrefix + mPrevElement)
                                )
                            )
                        )
                    );
                }
                else {
                    // We read the start of an element and then its end, without 
                    // encountering any sub-elements. The element also has mandatory
                    // sub-elements.
                    context->AddError( 
                        FdoExceptionP(
                            FdoException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SC_12_EMPTYELEMENT),
                                    (FdoString*) mSCName,
                                    (FdoString*) (mElementPrefix + mPrevElement)
                                )
                            )
                        )
                    );
                }
            }
            else {
                if ( isStart ) {
                    // We read the end of one element and then the start of 
                    // another. Therefore, this must be an unexpected next element
                    // error.
                    context->AddError( 
                        FdoExceptionP(
                            FdoException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SC_10_BADNEXTELEMENT),
                                    (FdoString*) mSCName,
                                    name,
                                    (FdoString*) (mElementPrefix + mPrevElement)
                                )
                            )
                        )
                    );
                }
                else {
                    // We read the end of an element and then the end of its
                    // containing element. Therefore, we're missing an expected
                    // next element inside the containing element.

                    // This loop finds the expected next element and logs the 
                    // the error.
                    for ( i = 0; parseStates[i].currentState != -3; i++ ) {
                        if ( (parseStates[i].currentState == mState) &&
                            (parseStates[i].isStart == true) ) {
                        
                            context->AddError( 
                                FdoExceptionP(
                                    FdoException::Create(
                                        FdoException::NLSGetMessage(
                                            FDO_NLSID(SC_11_MISSINGNEXTELEMENT),
                                            (FdoString*) mSCName,
                                            parseStates[i].nextElement,
                                            (FdoString*) (mElementPrefix + mPrevElement)
                                        )
                                    )
                                )
                            );
                            break;
                        }
                    }

                    if ( parseStates[i].currentState == -3 ) {
                        // This is not a error in the XML document. It indicates
                        // a logic problem in the state transition table.
                        context->AddError( 
                            FdoExceptionP(
                                FdoException::Create(
                                    FdoException::NLSGetMessage(
                                        FDO_NLSID(SC_2_NOSTATETRANSITION),
                                        (FdoString*) mPrevElement
                                    )
                                )
                            )
                        );
                    }
                }
            }
        }

        // if the end of a DerivedCRS generated an error, then stop parsing,
        // since we've reached the end of this spatial context.
        if ( !isStart && (wcscmp(name, L"DerivedCRS") == 0) )
            mStopParse = true;

        break;

    // State 1 looks for the start of the spatial context. When in state 1, the current
    // XML element is not a spatial context (DerivedCRS).
    case 1:
        // The handling of the first XML element depends on whether we're reading
        // the first spatial context. 
        // If reading the first spatial context, the we assume that the spatial contexts
        // are sub-elements of the current element. If reading the next spatial context
        // then the spatial context is assumed to be after the current element and
        // the current element is skipped.
        if ( isStart && !mFirst ) {
            mSkipHandler = FdoXmlSkipElementHandler::Create();
            ret = mSkipHandler;
        }

        // Stop parsing if we passed the first XML element without finding any 
        // spatial contexts.
        if ( mFirst && !isStart )
            mStopParse = true;

        break;

    // State 2 handles the start of the DerivedCRS. It handles its attributes.
    case 2:
        // Get the gml:id
        attr = atts->FindItem( FdoXml::mGmlUri + L":id" );

        if ( attr ) {
            mID = mXmlFlags->GetNameAdjust() ?
                (FdoString*) reader->DecodeName(attr->GetValue()) :
                attr->GetValue();
        }

        break;

    // The following states handle the start of sub-elements with simple content,
    // They set up a SAX handler to read this content.
    case 3:
    case 5:
    case 9:
    case 11:
    case 25:
    case 26:
    case 27:
    case 29:
    case 31:
        mContentHandler = FdoXmlCharDataHandler::Create();
        ret = mContentHandler;
        break;

    // State 4 handles the end of gml:remarks. The remarks content is the 
    // spatial context description
    case 4:
        mDescription = mContentHandler->GetString();
        break;

    // State 6 handles the end of gml:srsName. The srsName content is the 
    // spatial context name.
    case 6:
        mSCName = mContentHandler->GetString();

        if ( mSCName.GetLength() == 0 ) {
            // Name is mandatory
            context->AddError( 
                FdoExceptionP(
                    FdoException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SC_8_BADELEMENT),
                            L"",
                            L"srsName",
                            L""
                        )
                    )
                )
            );
        }
        else {
            if ( mID != mSCName && (mXmlFlags->GetErrorLevel() == FdoXmlFlags::ErrorLevel_High) )
                // ID is also mandatory when doing strict error checking. ID must 
                // match name.
                context->AddError( 
                    FdoExceptionP(
                        FdoException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SC_7_NAMEIDDIFF),
                                (FdoString*) mSCName,
                                (FdoString*) mID
                            )
                        )
                    )
                );
        }
        break;

    // State 10 handles the first gml:pos element, which contains the lower left
    // extent corner.
    case 10:
        // Get the extents from the content handler
        if ( swscanf(mContentHandler->GetString(), L"%lf %lf", &mMinX, &mMinY) != 2 ) {
            // Error if failed to get both X and Y
            context->AddError( 
                FdoExceptionP(
                    FdoException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SC_8_BADELEMENT),
                            (FdoString*) mSCName,
                            name,
                            mContentHandler->GetString()
                        )
                    )
                )
            );
        }
        break;

    // State 10 handles the first gml:pos element, which contains the upper right
    // extent corner.
    case 12:
        if ( swscanf(mContentHandler->GetString(), L"%lf %lf", &mMaxX, &mMaxY) != 2 ) {
            context->AddError( 
                FdoExceptionP(
                    FdoException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SC_8_BADELEMENT),
                            (FdoString*) mSCName,
                            name,
                            mContentHandler->GetString()
                        )
                    )
                )
            );
        }
        break;

    // State 15 handles the start of the baseCRS element, which contains the
    // coordinate system name.
    case 15:
        // Coordinate system can be a reference (xlink:href attribute) or
        // WKT definition (gml:metaDataProperty sub-element).
        attr = atts->FindItem( FdoXml::mXlinkUri + L":href" );

        if ( attr && (wcslen(attr->GetValue()) > 0) ) {
            mCsysFound = true;
            // Coordinate system is a reference to a name.

            // Check if it is the default dummy rectangular FDO coordinate system.
            // If it is, just leave coordinate system name blank.
            if ( (wcscmp( attr->GetValue(), L"http://fdo.osgeo.org/crs#default_cartesian") != 0) &&
                 (wcscmp( attr->GetValue(), L"http://www.autodesk.com/isd/crs#default_cartesian") != 0) ) 
            {
                // It isn't, parse the coordinate system name out of the URI.
                // It is the part of the URI after the '#'.
                
                FdoStringsP tokens = FdoStringCollection::Create( FdoStringP(attr->GetValue()), L"#" );
                if ( tokens->GetCount() != 2 ) {
                    context->AddError( 
                        FdoExceptionP(
                            FdoException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SC_6_BADATTRIBUTE),
                                    (FdoString*) mSCName,
                                    name,
                                    L"xlink:href",
                                    attr->GetValue()
                                )
                            )
                        )
                    );
                }
                else {
                    mCoordSysName = tokens->GetString(1);
                }
            }
        }

        break;

    // State 16 handles the end of the baseCRS element.
    case 16:
        // Error if baseCRS did not define an FDO coordinate system.
        if ( !mCsysFound ) 
            context->AddError( 
                FdoExceptionP(
                    FdoException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SC_14_EMPTYBASECRS),
                            (FdoString*) mSCName
                        )
                    )
                )
            );
        break;

    // State 17 handles the start of the DefinedByConversion element. This element
    // is required by GML but doesn't contain anything used by FDO. Therefore, it 
    // is validated when doing strict error checking, and ignored otherwise.
    case 17:
        if ( mXmlFlags->GetErrorLevel() == FdoXmlFlags::ErrorLevel_High ) {
            attr = atts->FindItem( FdoXml::mXlinkUri + L":href" );

            if ( !attr || (wcslen(attr->GetValue())==0) ) {
                context->AddError( 
                    FdoExceptionP(
                        FdoException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SC_5_MISSINGATTRIBUTE),
                                (FdoString*) mSCName,
                                name,
                                L"xlink:href"
                            )
                        )
                    )
                );
            }
            else {
                // Only the following coordinate conversion is currently supported.
                if ( (wcscmp(attr->GetValue(), L"http://fdo.osgeo.org/coord_conversions#identity") != 0) &&
                     (wcscmp(attr->GetValue(), L"http://www.autodesk.com/isd/coord_conversions#identity") != 0)
                ) {
                    context->AddError( 
                        FdoExceptionP(
                            FdoException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SC_6_BADATTRIBUTE),
                                    (FdoString*) mSCName,
                                    name,
                                    L"xlink:href",
                                    attr->GetValue()
                                )
                            )
                        )
                    );
                }
            }
        }
        break;

    // State 19 handles the start of a DerivedCrsType element.
    case 19:
        // Make sure right codeSpace when doing strict error checking
        if ( mXmlFlags->GetErrorLevel() == FdoXmlFlags::ErrorLevel_High ) {
            attr = atts->FindItem( L"codeSpace" );

            if ( !attr || (wcslen(attr->GetValue())==0) ) {
                context->AddError( 
                    FdoExceptionP(
                        FdoException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SC_5_MISSINGATTRIBUTE),
                                (FdoString*) mSCName,
                                name,
                                L"codeSpace"
                            )
                        )
                    )
                );
            }
            else {
                // Only the following codeSpace currently allowed.
                if ( (wcscmp(attr->GetValue(), L"http://fdo.osgeo.org/crs_types") != 0) &&
                     (wcscmp(attr->GetValue(), L"http://www.autodesk.com/isd/crs") != 0)
                ) {
                    context->AddError( 
                        FdoExceptionP(
                            FdoException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SC_6_BADATTRIBUTE),
                                    (FdoString*) mSCName,
                                    name,
                                    L"codeSpace",
                                    attr->GetValue()
                                )
                            )
                        )
                    );
                }
            }
        }

        // Handle the DerivedCrsType content.
        mContentHandler = FdoXmlCharDataHandler::Create();
        ret = mContentHandler;
        break;

    // State 20 handles the end of the DerivedCRSType element. The FDO API currently
    // doesn't expose whether a spatial context is geographic or not. Therefore, assume
    // all spatial contexts are geographic and log an error for ones that are not.
    // More DerivedCRSType's will be supported in the future when more coordinate 
    // system handling is added to the FDO API.
    case 20:
        if ( wcscmp(mContentHandler->GetString(), L"geographic" ) != 0 ) {
            context->AddError( 
                FdoExceptionP(
                    FdoException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SC_8_BADELEMENT),
                            (FdoString*) mSCName,
                            name,
                            mContentHandler->GetString()
                        )
                    )
                )
            );
        }
        break;

    // State 17 handles the start of the UsesCS element. This element
    // is required by GML but doesn't contain anything used by FDO. Therefore, it 
    // is validated when doing strict error checking, and ignored otherwise.
    case 21:
        if ( mXmlFlags->GetErrorLevel() == FdoXmlFlags::ErrorLevel_High ) {
            attr = atts->FindItem( FdoXml::mXlinkUri + L":href" );

            if ( !attr || (wcslen(attr->GetValue())==0) ) {
                context->AddError( 
                    FdoExceptionP(
                        FdoException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SC_5_MISSINGATTRIBUTE),
                                (FdoString*) mSCName,
                                name,
                                L"xlink:href"
                            )
                        )
                    )
                );
            }
            else {
                // Only the following coordinate system currently allowed.
                if ( (wcscmp(attr->GetValue(), L"http://fdo.osgeo.org/cs#default_cartesian") != 0)   &&
                     (wcscmp(attr->GetValue(), L"http://www.autodesk.com/isd/cs#default_cartesian") != 0) 
                ) {
                    context->AddError( 
                        FdoExceptionP(
                            FdoException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SC_6_BADATTRIBUTE),
                                    (FdoString*) mSCName,
                                    name,
                                    L"xlink:href",
                                    attr->GetValue()
                                )
                            )
                        )
                    );
                }
            }
        }
        break;

    /* case 24 handle FDO-specific metadata */

    case 24:
        // Metadata type depends on current state
        switch (mState) {
        // Extent type
        case 25:
        // Get the spatial context extent type if present
            if ( wcscmp(mContentHandler->GetString(), L"dynamic") == 0 ) {
                mExtentType = FdoSpatialContextExtentType_Dynamic;
            }
            else {
                context->AddError( 
                    FdoExceptionP(
                        FdoException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SC_8_BADELEMENT),
                                (FdoString*) mID,
                                L"fdo:SCExtentType",
                                mContentHandler->GetString()
                            )
                        )
                    )
                );
            }
            break;

        // XY Tolerance
        case 26:
            mXYTolerance = FdoStringP(mContentHandler->GetString()).ToDouble();

            break;

        // Z Tolerance
        case 27:
            mZTolerance = FdoStringP(mContentHandler->GetString()).ToDouble();
            break;
        }
        
        break;


    // State 28 handles the start of an fdo:WKTCRS. It handles its attributes.
    case 28:
        mCsysFound = true;

        // The element prefix is used in error messages to identity the proper
        // nested sub-element within the gml:DerivedCRS. Need to set it since 
        // current element (WKTCRS) is nested down 2 levels.
        mElementPrefix = L"gml:baseCRS/fdo:";

        // Get the gml:id
        attr = atts->FindItem( FdoXml::mGmlUri + L":id" );

        if ( attr ) {
            mCsysID = mXmlFlags->GetNameAdjust() ?
                (FdoString*) reader->DecodeName(attr->GetValue()) :
                attr->GetValue();
        }

        break;

    // State 30 handles the end of gml:baseCRS/fdo:WKTCRS/gml:srsName. The srsName content is the 
    // spatial context's coordinate system name.
    case 30:
        // change the element prefix since we're now nested at 3 levels below
        // gml:DerivedCRS.
        mElementPrefix = L"gml:baseCRS/fdo:WKTCRS/";

        mCoordSysName = mContentHandler->GetString();

        if ( mCoordSysName.GetLength() == 0 ) {
            // Name is mandatory
            context->AddError( 
                FdoExceptionP(
                    FdoException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SC_8_BADELEMENT),
                            (FdoString*) mSCName,
                            L"gml:baseCRS/fdo:WKTCRS/gml:srsName",
                            L""
                        )
                    )
                )
            );
        }
        else {
            if ( mCsysID != mCoordSysName && (mXmlFlags->GetErrorLevel() == FdoXmlFlags::ErrorLevel_High) )
                // ID is also mandatory when doing strict error checking. ID must 
                // match name.
                context->AddError( 
                    FdoExceptionP(
                        FdoException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SC_13_NAMEIDDIFF),
                                (FdoString*) mSCName,
                                (FdoString*) mCsysID
                            )
                        )
                    )
                );
        }
        break;

    // State 32 handles the end of gml:baseCRS/fdo:WKTCRS/fdo:WKT. The WKT content is the 
    // spatial context's coordinate definition in well known text format.
    case 32:
        mCoordSysWkt = mContentHandler->GetString();

        if ( mCoordSysWkt.GetLength() == 0 ) {
            // WKT coordinate system must be defined
            context->AddError( 
                FdoExceptionP(
                    FdoException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SC_8_BADELEMENT),
                            (FdoString*) mSCName,
                            L"gml:baseCRS/fdo:WKTCRS/fdo:WKT",
                            L""
                        )
                    )
                )
            );
        }
        break;

    case 33:
        // element nesting level is back down to 1 so don't need to prefix 
        // element names in error messages.
        mElementPrefix = L"";
        break;

    // State 100 handles the skipping of elements. An FDO XML document can contain a 
    // mixture of spatial context, feature schema, and schema override elements. This
    // state skips over the non-spatialcontext elements.
    case 100:
        mSkipHandler = FdoXmlSkipElementHandler::Create();
        ret = mSkipHandler;
        break;

    }

    // Move on to the next state.
    mState = nextState;
    // Keep track of the previous transition for error reporting.
    mPrevStart = isStart;
    mPrevElement = name;

    return ret;
}

