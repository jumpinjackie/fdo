// ExerciseFdoUtilities.h

#ifndef _EXERCISEFDOUTILITIES_H_
#define _EXERCISEFDOUTILITIES_H_
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

#include <FdoCommon.h>
#include "Logger.h"
#include "conversions.h"

class ExerciseFdoUtilities {
public:
	ExerciseFdoUtilities(Logger * log, Conversions * convert);
   // Print an exception, and all it's cause exceptions to log.
   void PrintException( FdoException* e, FdoBoolean stripLineNo = false );
   double * stringToDoubleArray(char * str, int * count);
private:
   Logger * log;
   Conversions * convert;
   char errString[256];
};

#endif

