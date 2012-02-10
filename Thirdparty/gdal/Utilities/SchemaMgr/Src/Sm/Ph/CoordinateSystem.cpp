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

#include "stdafx.h"
#include <Sm/Ph/CoordinateSystem.h>
#include <FdoCommonOSUtil.h>

// Transition table and enums for parsing WKT

// Various types of tokens related to finding Axis assignments.
enum wkt_tokens {
    WKT_TOKEN_QUOTE = 0,
    WKT_TOKEN_LBRACKET,
    WKT_TOKEN_RBRACKET,
    WKT_TOKEN_COMMA,
    WKT_TOKEN_AXIS,
    WKT_TOKEN_TERM,
    WKT_TOKEN_X,
    WKT_TOKEN_Y,
    WKT_TOKEN_NORTH,
    WKT_TOKEN_SOUTH,
    WKT_TOKEN_EAST,
    WKT_TOKEN_WEST,
    WKT_TOKEN_OTHER
};

// Parsing states
enum wkt_states {
    WKT_STATE_NORMAL = 0,       // Currently in a part of the WKT we want to keep.
    WKT_STATE_LITERAL,          // Currently inside a literal string.
    WKT_STATE_SKIP,             // Currently at an Axis assignment to skip.
    WKT_STATE_END,              // Done parsing.
};

// Actions
enum wkt_actions {
    WKT_ACTION_NONE = 0,
    WKT_ACTION_PUSHLITERAL,
    WKT_ACTION_POPLITERAL,
    WKT_ACTION_PUSHBRACKET,
    WKT_ACTION_POPBRACKET,
    WKT_ACTION_AXIS,
    WKT_ACTION_COMMA,
    WKT_ACTION_XAXIS,
    WKT_ACTION_YAXIS,
    WKT_ACTION_DIRN,
    WKT_ACTION_TERM
};

// Transition Table:
//   1st dimension lists current states
//   2nd dimension lists various tokens
//   Values are the actions to take based on current state and current token being read from WKT.
static int wkt_transition[3][18] = {
                   /* QUOTE,                  LBRACKET,               RBRACKET,              COMMA,             AXIS,               TERM,             X                 Y                 NORTH             SOUTH             EAST              WEST               OTHER */
    /* NORMAL  */   { WKT_ACTION_PUSHLITERAL, WKT_ACTION_PUSHBRACKET, WKT_ACTION_POPBRACKET, WKT_ACTION_NONE,   WKT_ACTION_AXIS,    WKT_ACTION_TERM,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE},
    /* LITERAL */   { WKT_ACTION_POPLITERAL,  WKT_ACTION_NONE,        WKT_ACTION_NONE,       WKT_ACTION_NONE,   WKT_ACTION_NONE,    WKT_ACTION_TERM,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE,  WKT_ACTION_NONE},
    /* SKIP */      { WKT_ACTION_PUSHLITERAL, WKT_ACTION_PUSHBRACKET, WKT_ACTION_POPBRACKET, WKT_ACTION_COMMA,  WKT_ACTION_NONE,    WKT_ACTION_TERM,  WKT_ACTION_XAXIS, WKT_ACTION_YAXIS, WKT_ACTION_DIRN,  WKT_ACTION_DIRN,  WKT_ACTION_DIRN,  WKT_ACTION_DIRN,  WKT_ACTION_NONE}
};

FdoSmPhCoordinateSystem::FdoSmPhCoordinateSystem(
    FdoSmPhMgrP pMgr,
    FdoString* name,
    FdoString* description,
    FdoInt64 srid,
    FdoString* Wkt
) : 
    FdoSmPhSchemaElement((FdoString *)name, description, pMgr),
	mSrid(srid)
{
    // Strip out any axis assignments from the WKT.
    ParseWkt(Wkt);
}

FdoSmPhCoordinateSystem::~FdoSmPhCoordinateSystem(void)
{
}

FdoString* FdoSmPhCoordinateSystem::GetWkt()
{
    return mWkt;
}

FdoInt64 FdoSmPhCoordinateSystem::GetSrid()
{
	return mSrid;
}

void FdoSmPhCoordinateSystem::ParseWkt( FdoStringP inWKT )
{
    FdoStringsP outWKT = FdoStringCollection::Create();

    // Initialize parser to start of WKT string, with Normal state.

    FdoString* ptr = (FdoString*) inWKT;
    FdoInt32  idx = 0;
    int state = WKT_STATE_NORMAL;
    int prevState = state;

    FdoInt32  bracketLevel = 0;
    FdoInt32  axisBracketLevel = 0;

    int xdirn = WKT_TOKEN_EAST;
    int ydirn = WKT_TOKEN_NORTH;
    int* dirn = NULL;

    for ( ; state != WKT_STATE_END; idx++)
    {
        // Determine current token from string at current position in the WKT.

        int token = WKT_TOKEN_OTHER;

        if ( (state == WKT_STATE_SKIP) && (FdoCommonOSUtil::wcsnicmp(&ptr[idx], L"\"X\"", 3) == 0) ) 
            token = WKT_TOKEN_X;
        else if ( (state == WKT_STATE_LITERAL) && (prevState == WKT_STATE_SKIP) && (wcsncmp(&ptr[idx], L"(X)", 3) == 0) ) 
            token = WKT_TOKEN_X;
        else if ( (state == WKT_STATE_SKIP) && (FdoCommonOSUtil::wcsnicmp(&ptr[idx], L"\"Y\"", 3) == 0) ) 
            token = WKT_TOKEN_Y;
        else if ( (state == WKT_STATE_LITERAL) && (prevState == WKT_STATE_SKIP) && (wcsncmp(&ptr[idx], L"(Y)", 3) == 0) ) 
            token = WKT_TOKEN_Y;
        else if ( (state == WKT_STATE_SKIP) && (wcsncmp(&ptr[idx], L"NORTH", 5) == 0) ) 
            token = WKT_TOKEN_NORTH;
        else if ( (state == WKT_STATE_SKIP) && (wcsncmp(&ptr[idx], L"SOUTH", 5) == 0) ) 
            token = WKT_TOKEN_SOUTH;
        else if ( (state == WKT_STATE_SKIP) && (wcsncmp(&ptr[idx], L"EAST", 4) == 0) ) 
            token = WKT_TOKEN_EAST;
        else if ( (state == WKT_STATE_SKIP) && (wcsncmp(&ptr[idx], L"WEST", 4) == 0) ) 
            token = WKT_TOKEN_WEST;
        else if ( ptr[idx] == '"' ) 
            token = WKT_TOKEN_QUOTE;
        else if ( ptr[idx] == '[' ) 
            token = WKT_TOKEN_LBRACKET;
        else if ( ptr[idx] == ']' ) 
            token = WKT_TOKEN_RBRACKET;
        else if ( ptr[idx] == 0 ) 
            token = WKT_TOKEN_TERM;
        else if ( wcsncmp(&ptr[idx], L",AXIS", 5) == 0 ) 
            token = WKT_TOKEN_AXIS;
        else if ( ptr[idx] == ',' ) 
            token = WKT_TOKEN_COMMA;

        // Determine action from current state and token.
        int action = wkt_transition[state][token];

        // Perform the action.
        switch ( action ) 
        {
        case WKT_ACTION_PUSHLITERAL:
            // At start of literal string.
            prevState = state;
            state = WKT_STATE_LITERAL;
            break;

        case WKT_ACTION_POPLITERAL:
            // At end of literal string, go back to previous state.
            state = prevState;
            break;

        case WKT_ACTION_PUSHBRACKET:
            // [ bracket, track the nesting level.
            bracketLevel++;
            break;

        case WKT_ACTION_POPBRACKET:
            // ] bracket, track the nesting level.
            bracketLevel--;
            if ( (state == WKT_STATE_SKIP) && (bracketLevel < axisBracketLevel) ) 
            {
                // We're currently skipping an axis assignment but the bracket 
                // nesting level is now lower that at the start of the axis.
                // This means that we've reached the end of the axis assignment
                // and can stop skipping characters.

                // Move position past characters to skip.
                ptr = &ptr[idx];
                idx = -1;
                state = WKT_STATE_NORMAL;
            }
            break;

        case WKT_ACTION_AXIS:
            if ( bracketLevel == 1 )
            {
                // Starting an Axis assigment. Write out WKT up to this point
                // and go into the skip state.
                outWKT->Add( FdoStringP(ptr,true).Mid(0,idx) );

                // remember current bracket nesting level. This level will increase
                // as brackets inside the axis assignment are encountered. When
                // the bracket level drops below this level, we've reached the
                // end of the axis assignment.
                axisBracketLevel = bracketLevel;
                state = WKT_STATE_SKIP;
                dirn = NULL;
            }
            break;

        case WKT_ACTION_XAXIS:
            // Current axis assignment is for X.
            dirn = &xdirn;
            idx += 2;
            break;

        case WKT_ACTION_YAXIS:
            // Current axis assignment is for Y.
            dirn = &ydirn;
            idx += 2;
            break;

        case WKT_ACTION_DIRN:
            // Axis direction (North, South, East or West) encountered.
            if ( dirn ) 
                (*dirn) = token;
            break;

        case WKT_ACTION_COMMA:
            if ( bracketLevel == axisBracketLevel ) 
            {
                // We're currently skipping an axis assignment but the bracket 
                // nesting level is now the same at the start of the axis.
                // This means that we've reached the comma ending the axis assignment
                // and can stop skipping characters.

                // Move position past characters to skip.
                ptr = &ptr[idx + 1];
                idx = -1;
                state = WKT_STATE_NORMAL;
            }
            break;

        case WKT_ACTION_TERM:
            // We're done. Write out the last part of the WKT.
            outWKT->Add( ptr );
            state = WKT_STATE_END;
            break;
        }
    }

    // Put the WKT back together.
    mWkt = outWKT->ToString(L"");

    // Adjust transformation matrices based on axis directions
    if ( xdirn == WKT_TOKEN_WEST || xdirn == WKT_TOKEN_SOUTH ) 
    {
        mTransformOut[0][0] = -mTransformOut[0][0];
    }

    if ( ydirn == WKT_TOKEN_WEST || ydirn == WKT_TOKEN_SOUTH ) 
    {
        mTransformOut[1][1] = -mTransformOut[1][1];
    }

    if ( xdirn == WKT_TOKEN_NORTH || xdirn == WKT_TOKEN_SOUTH ) 
    {
        mTransformOut = mTransformOut.SwapXY();
    }

    mTransformIn = mTransformOut.Inverse();
}

void FdoSmPhCoordinateSystem::TransformOut( double& x, double& y, double& z)
{
    Vector in( x, y, z );
    Vector out = mTransformOut * in;
    out.Get( x, y, z );
}

void FdoSmPhCoordinateSystem::TransformIn( double& x, double& y, double& z)
{
    Vector out( x, y, z );
    Vector in = mTransformIn * out;
    in.Get( x, y, z );
}

FdoSmPhCoordinateSystem::Vector::Vector()
{
    Set( 0, 0, 0 );
}

FdoSmPhCoordinateSystem::Vector::Vector( double x, double y, double z )
{
    Set( x, y, z );
}

FdoSmPhCoordinateSystem::Vector::~Vector()
{
}

void FdoSmPhCoordinateSystem::Vector::Get( double& x, double& y, double& z )
{
    x = mVector[0];
    y = mVector[1];
    z = mVector[2];
}

void FdoSmPhCoordinateSystem::Vector::Set( double x, double y, double z )
{
    mVector[0] = x;
    mVector[1] = y;
    mVector[2] = z;
}

void FdoSmPhCoordinateSystem::Vector::Set( Vector& src )
{
    for ( int i = 0; i < 3; i++ ) 
        mVector[i] = src[i];
}

double& FdoSmPhCoordinateSystem::Vector::operator[]( int index )
{
    return mVector[index];
}

FdoSmPhCoordinateSystem::Vector& FdoSmPhCoordinateSystem::Vector::operator=(Vector src)
{
    Set(src);

    return(*this);
}

double FdoSmPhCoordinateSystem::Vector::operator*(Vector rhs)
{
    double out = 0;

    for ( int j = 0; j < 3; j++ ) 
    {
        out += mVector[j] * rhs[j];
    }

    return out;
}

FdoSmPhCoordinateSystem::Matrix::Matrix( bool identity )
{
    if ( identity ) 
    {
        for ( int i = 0; i < 3; i++ ) 
            mMatrix[i][i] = 1;
    }
}

FdoSmPhCoordinateSystem::Matrix::~Matrix()
{
}

FdoSmPhCoordinateSystem::Matrix FdoSmPhCoordinateSystem::Matrix::SwapXY()
{
    Matrix out(false);
    Matrix flip(false);

    // Flip X and Y
    flip[1][0] = 1;
    flip[0][1] = 1;

    // Keep Z the same
    flip[2][2] = 1;

    out = flip * (*this);

    return out;
}

void FdoSmPhCoordinateSystem::Matrix::Set( Matrix& src )
{
    for ( int i = 0; i < 3; i++ ) 
        mMatrix[i] = src[i];
}

FdoSmPhCoordinateSystem::Matrix FdoSmPhCoordinateSystem::Matrix::Inverse()
{
    Matrix out = (*this);

    // Not completely correct but good enough for matrices that convert axis assignments.
    out[0][1] = mMatrix[1][0];
    out[1][0] = mMatrix[0][1];

    return out;
}

FdoSmPhCoordinateSystem::Vector& FdoSmPhCoordinateSystem::Matrix::operator[]( int index )
{
    return mMatrix[index];
}

FdoSmPhCoordinateSystem::Matrix& FdoSmPhCoordinateSystem::Matrix::operator=(FdoSmPhCoordinateSystem::Matrix src)
{
    Set(src);

    return(*this);
}

FdoSmPhCoordinateSystem::Matrix FdoSmPhCoordinateSystem::Matrix::operator*(FdoSmPhCoordinateSystem::Matrix rhs)
{
    Matrix out(false);

    for ( int j = 0; j < 3; j++ ) 
    {
        for ( int i = 0; i < 3; i++ )
        {
            for ( int k = 0; k < 3; k++ ) 
                out[j][i] += mMatrix[j][k] * rhs[k][i];
        }
    }

    return out;
}

FdoSmPhCoordinateSystem::Vector FdoSmPhCoordinateSystem::Matrix::operator*(FdoSmPhCoordinateSystem::Vector rhs)
{
    Vector out;

    for ( int j = 0; j < 3; j++ ) 
    {
        out[j] = mMatrix[j] * rhs;
    }

    return out;
}


