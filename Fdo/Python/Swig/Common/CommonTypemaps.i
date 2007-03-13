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

%include std_wstring.i 

/* -------------------------------------------------------------
 * PYTHON typemaps
 *	Description:	These typemaps will convert between wchar_t*
 *					and Python strings
 * -------------------------------------------------------------
 */
 

/* Convert Python string to FdoStringP */
%typemap (python, in) FdoStringP
{
	// Convert Python string to FdoStringP
	$1 = FdoStringP(utils_PyObjectToWideChar($input));
}

/* Convert FdoStringP to Python string */
%typemap (python, out) FdoStringP	
{
	$result = utils_WideCharToPyObject((const wchar_t*)$1); // Convert FdoStringP to Python string	
}


 /* -------------------------------------------------------------
 * PYTHON typemaps
 *	Description:	These typemaps will convert between FdoInt*
 *					and Python numbers
 * -------------------------------------------------------------
 */

/* Convert the Fdo integers into python long */
%typemap (in) _int8, _int16, _int32, int8_t, int16_t, int32_t, FdoInt8, FdoInt16, FdoInt32, FdoSize = int;
%typemap (out) _int8, _int16, _int32, int8_t, int16_t, int32_t, FdoInt8, FdoInt16, FdoInt32, FdoSize = int;

/* Convert the Fdo long integers into python long long  */
%typemap (in) _int64, int64_t, FdoInt64 = long long;
%typemap (out) _int64, int64_t, FdoInt64 = long long;


/* -------------------------------------------------------------
 * PYTHON typemaps
 *	Description:	These typemaps will convert between wchar_t**
 *					and Python Tuples
 * -------------------------------------------------------------
 */
/* Convert wchar_t** to Python Tuples  */
%typemap (python, out) wchar_t**
{
	// Convert wchar_t** to Python Tuples		
	int arrayLength = *arg2;
	const wchar_t** array = $1;

	if ( array == NULL ) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	// Create the Python Tuple.
	$result = PyTuple_New( arrayLength );
	if ($result == NULL) {
		PyErr_SetString(PyExc_RuntimeError, "Internal error.  Cannot create tuple in 'wchar_t** out' typemap.");
		goto fail;
	}

	// And populate the tuple from the C++ array
	for (int index = 0; index < arrayLength; index++) {
		PyObject *newElement = SWIG_FromWCharPtr( array[index] );	
		if (PyTuple_SetItem($result, index, newElement) != 0) {
			PyErr_SetString(PyExc_RuntimeError, "Internal error.  Cannot build tuple in 'wchar_t** out' typemap.");
			goto fail;			
		}
	}	
}

/* -------------------------------------------------------------
 * PYTHON typechecking
 *	Description:	These typechecks are for wchar_t, wchar_t*,
 *					and FdoStringP.
 *					This will ensure that method overloading works
 *					properly with wchar_t* parameters
 * -------------------------------------------------------------
 */


%typecheck(SWIG_TYPECHECK_STRING) FdoStringP {
	$1 = PyString_Check($input) ? 1 : 0;
}

%typecheck(SWIG_TYPECHECK_INTEGER) FdoInt8, FdoInt16, FdoInt32, FdoSize = int;

%typecheck(SWIG_TYPECHECK_INTEGER) FdoInt8, FdoInt16, FdoInt32, FdoSize = int;

%typecheck(SWIG_TYPECHECK_INTEGER) FdoInt64 = long long;

%typecheck(SWIG_TYPECHECK_INTEGER) FdoInt64 = long long;




/* -------------------------------------------------------------
 * PYTHON typemaps
 *	Description:	These typemaps will convert between C++
 *					arrays to Python Lists
 * -------------------------------------------------------------
 */

%include "typemaps.i"
%apply int& OUTPUT { FdoInt32& count };
%apply int& OUTPUT { FdoInt32& length };
%apply int& OUTPUT { FdoInt32& size };


/* Convert FdoDictionary* to Python Dictionary */
%typemap(out) FdoDictionary* {	
	// Translate the FdoDictionary to a Python dictionary
	$result = utils_FdoDictionaryToPyDict($1);
}


/* Convert StringCollection* to Python List */
%typemap(out) StringCollection* {	
	// Translate the StringCollection to a Python list
	$result = utils_StringCollectionToPyList($1);
}
