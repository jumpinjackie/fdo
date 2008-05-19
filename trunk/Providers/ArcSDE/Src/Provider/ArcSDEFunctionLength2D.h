//
// 
// Copyright (C) 2004-2007  Autodesk, Inc.
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
//
#ifndef ARCSDEFUNCTIONLENGTH2D_H
#define ARCSDEFUNCTIONLENGTH2D_H

#include <Functions/Geometry/FdoFunctionLength2D.h>
#include <sdetype.h>

// ============================================================================
// The class FdoFunctionArea2D implements the Expression Engine function AREA.
// ============================================================================

class ArcSDEFunctionLength2D : public FdoFunctionLength2D
{
    public:

        // Function to create an instance of this class.
        static ArcSDEFunctionLength2D *Create (ArcSDEConnection* connection, SE_COORDREF coordRef);

		// Evaluate 
        virtual FdoLiteralValue *Evaluate (FdoLiteralValueCollection *literal_values);

	protected:

		// Constructors
		ArcSDEFunctionLength2D ();
		ArcSDEFunctionLength2D (ArcSDEConnection* connection, SE_COORDREF coordRef);

		// Destructor
		~ArcSDEFunctionLength2D ();

    private:
		FdoPtr<ArcSDEConnection> mConnection;
		SE_COORDREF				 mCoordRef;
}; 
#endif
