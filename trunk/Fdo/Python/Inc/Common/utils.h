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

#include "FdowStd.h"
#include "Fdo.h"
#include <wchar.h>
#include <Python.h>
#include <string.h>
#include <stdlib.h>

#ifndef FALSE
#   define  FALSE   (0)
#endif

#ifndef TRUE
#   define  TRUE    (1)
#endif

// ------------------------------------------------------------------------
// Function Name: utils_PyObjectToWideChar
// Description:   Translates a Python string (as a PyObject) into a 
//                wide character string.
// 
// Parameters:    pPyString (mandatory input)
//					- The input Python string. No typechecking is performed.
//
//
// Returns:       A fully allocated wchar_t* array with the message.
//                Returns NULL if the pPyString == PyNone.
// ------------------------------------------------------------------------
wchar_t* utils_PyObjectToWideChar( PyObject* pPyString );



// ------------------------------------------------------------------------
// Function Name: utils_WideCharToPyObject
//
// Description:   Translates a wide character string into a Python string.
// 
// Parameters:    pString (mandatory input)
//					- The wide character string
//
// Returns:       The Python object representing the string.
//                Returns Py_None if pString is NULL.
// ------------------------------------------------------------------------
PyObject* utils_WideCharToPyObject( const wchar_t* pString );


// ------------------------------------------------------------------------
// Function Name: utils_WideCharArrayToPyList
//
// Description:   Translates a wide character double pointer into a Python list.
// 
// Parameters:    pStringArray (mandatory input)
//					- The wide character string array
//
//				  arrayLength  (mandatory input)
//                  - Length of the array
//
// Returns:       The Python list containing PyStrings.
//                Returns Py_None if pString is NULL.
// ------------------------------------------------------------------------
PyObject* utils_WideCharArrayToPyList( const wchar_t** pStringArray, const long arrayLength );

// ------------------------------------------------------------------------
// Function Name: utils_FdoDictionaryToPyDict
//
// Description:   Translates a Fdo FdoDictionary to a Python Dictionary
// 
// Parameters:    pFdoDictionary (mandatory input)
//					- The original FDO FdoDictionary
//
// Returns:       The Python dictionary containing PyStrings.
//                Returns Py_None if pString is NULL.
// ------------------------------------------------------------------------
PyObject* utils_FdoDictionaryToPyDict( const FdoDictionary *pFdoDictionary );


// ------------------------------------------------------------------------
// Function Name: utils_StringCollectionToPyList
//
// Description:   Translates an FDO String Collection to a Python list.
// 
// Parameters:    pStringCollection (mandatory input)
//					- The FDO String Collection
//
// Returns:       The Python List containing a copy of the string collection.
//
// Remarks:		  pStringCollection is not destroyed in this function. 
// ------------------------------------------------------------------------
PyObject* utils_StringCollectionToPyList( const FdoStringCollection* pStringCollection );


// ------------------------------------------------------------------------
// Function Name: utils_intArrayToPyList
//
// Description:   Translates an array of integers to a Python list.
// 
// Parameters:    array (mandatory input)
//					- the integer array
//
//					arrayLength (mandatory input)
//					- number of array elements
//
// Returns:       The Python List containing a copy of the integer values.
//
// Remarks:			array is not destroyed in this function. 
// ------------------------------------------------------------------------
PyObject* utils_intArrayToPyList( const int* array, const long arrayLength );


// ------------------------------------------------------------------------
// Function Name: utils_intArrayToPyTuple
//
// Description:   Translates an array of integers to a Python tuple.
// 
// Parameters:    array (mandatory input)
//					- the integer array
//
//					arrayLength (mandatory input)
//					- number of array elements
//
// Returns:       The Python Tuple containing a copy of the integer values.
//
// Remarks:       The original array is not destroyed in this function. 
// ------------------------------------------------------------------------
PyObject* utils_intArrayToPyTuple( const int* array, const long arrayLength );


// ------------------------------------------------------------------------
// Function Name: ut_utf8_from_unicode
//
// Description:   Convert a UTF8 string from a UNICODE string.
// 
// Parameters:    
//                Wtext (mandatory input)
//                  - The unicode string that will be transformed into UTF8
//               
//                str_out (mandatory output)
//					- the result of the Unicode to UT8 conversion
//
//                out_max_size (mandatory input)
//                  - the size of the output string
//
// Returns:       Returns TRUE on success.  FALSE on failure.
// ------------------------------------------------------------------------
int ut_utf8_from_unicode(const wchar_t *Wtext, char* str_out, int out_max_size);


int ut_utf8_charlen(const char *str_in);


// ------------------------------------------------------------------------
// Function Name: ut_utf8_to_unicode
//
// Description:   Convert a UTF8 string to a UNICODE string.
// 
// Parameters:    str_in (mandatory input)
//					- the input utf8 string
//
//                Wtext (mandatory output)
//                  - the result of the conversion from UTF8 to UNICODE
//
//                out_size (mandatory input)
//                  - the size of the output string
//
// Returns:       Returns TRUE on success.  FALSE on failure.
// ------------------------------------------------------------------------
int ut_utf8_to_unicode( const char* str_in, wchar_t* Wtext, int out_size);

