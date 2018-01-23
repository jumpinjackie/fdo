//ExerciseFdoUtilities.cpp
// 
// 
// Copyright (C) 2008  Autodesk, Inc.
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

#include "stdafx.h"
#include "ExerciseFdoUtilities.h"

ExerciseFdoUtilities::ExerciseFdoUtilities(Logger * log,
										   Conversions * convert) {
	this->log = log;
	this->convert = convert;
}

void
ExerciseFdoUtilities::PrintException( FdoException* e, FdoBoolean stripLineNo)
{
	FdoPtr<FdoException> currE = e;
	// Add ref to prevent smart pointer from destroying exception.
	FDO_SAFE_ADDREF(e);

	while ( currE ) {
        FdoString* pMessage = NULL;
        if ( stripLineNo ) 
            // The following removes the function and line number info from error messages.
            pMessage = wcschr( currE->GetExceptionMessage(), ')' );

		if ( pMessage )
            pMessage++;
        else
            pMessage = currE->GetExceptionMessage();

		log->write( convert->wchar_tStarToCharStar(pMessage) );
		currE = currE->GetCause();
	}
}

// example input "52 18 66 23 73 9 48 6 52 18"
// also "52.0 18.3 66 23.6 73 9.0 48.2 52 18"
double *
ExerciseFdoUtilities::stringToDoubleArray(char * str, int  * count) {
	double arr[128];
	double * arrPtr = NULL;
	int elementCount = 0;
	char * value = NULL;
	if (strlen(str)) {
		value = strtok(str, " ");
		while (value != NULL) {
			arr[elementCount] = atof(value);
			elementCount++;
			value = strtok((char*)NULL, " ");
		}
	}
	if (elementCount) {
		arrPtr = new double[elementCount];
		for(int i=0; i<elementCount; i++) {
			arrPtr[i] = arr[i];
		}
		*count = elementCount;
	}
	return arrPtr;
}

