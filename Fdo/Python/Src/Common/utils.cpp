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


#include "stdafx.h"
#include "Common/utils.h"

// ------------------------------------------------------------------------
// Function Name: utils_PyObjectToWideChar
// Description:   Translates a Python string (as a PyObject) into a 
//                wide character string.
// 
// Parameters:    pPyStrin  g (mandatory input)
//					- The input Python string. No typechecking is performed.
//
//
// Returns:       A fully allocated wchar_t* array with the message.
//                Returns NULL if the PyObject == PyNone.
// ------------------------------------------------------------------------
wchar_t* utils_PyObjectToWideChar( PyObject* pPyString ) {
	int strLength;	
	char* pIntermediateString = NULL;	
	wchar_t* pWideCharString = NULL;
	
	if (pPyString == Py_None ) {
		return NULL;
	}
	else {
		// Convert PyString into char*. According to Python docs, do NOT
		// delete pIntermediateString.
		pIntermediateString = PyString_AsString(pPyString);

		/* Convert char* to wchar_t */
		strLength = PyUnicode_GET_SIZE(pPyString);
		pWideCharString = new wchar_t[ strLength + 1 ];
		if ( !ut_utf8_to_unicode( pIntermediateString, pWideCharString, strLength ) ) {
			pWideCharString = NULL;
		}
	}
	return pWideCharString;
}



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
PyObject* utils_WideCharToPyObject( const wchar_t* pString ) {
	PyObject *retObject = NULL;
	size_t length;
	char* pIntermediate;

	if (pString == NULL) {		
		Py_INCREF(Py_None);
		retObject = Py_None;
	}
	else {			
		// Convert from wchar_t* to char* 		
		// Using multiplier of 3 to support maximum size of unicode to byte translation		
		length = wcslen( pString );		
		pIntermediate = new char[ ((int)(length) + 1) * 3 ];						
	    if( ! ut_utf8_from_unicode( pString, pIntermediate, (int)length*3 ) )
            retObject = NULL;               
            
		// Convert from char* to python	string
		retObject = PyString_FromStringAndSize( pIntermediate, (int)length );		
		delete [] pIntermediate;		
	}	
	return retObject;
}



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
//
// Returns:       The Python list containing PyStrings.
//                Returns Py_None if pString is NULL.
// ------------------------------------------------------------------------
PyObject* utils_WideCharArrayToPyList( const wchar_t** pStringArray, const long arrayLength )
{
	PyObject *retList = NULL;

	// Check input parameters
	if ((arrayLength < 0) || (pStringArray == NULL)) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	// Create and populate the python list from the C++ array
	retList = PyList_New(arrayLength);
	for (int i = 0; i < arrayLength; i++) {
		PyObject *o = utils_WideCharToPyObject( pStringArray[i] );
		PyList_SetItem(retList,i,o);
	}

	return retList;
}



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
PyObject* utils_FdoDictionaryToPyDict( const FdoDictionary *pFdoDictionary ) {
	if (pFdoDictionary == NULL) {	
		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject *pPyDictionary = PyDict_New();
	FdoDictionaryElement *pDictItem;
	size_t stringLength;
	char *pDictKey;
	PyObject *pDictValue;
	int errNum = 0;

	for (int index=0; index < pFdoDictionary->GetCount(); index++) {
		// Get the dictionary item
		pDictItem = pFdoDictionary->GetItem(index);

		// Translate the dictionary item key from wchar_t* to char*
		stringLength = wcslen(pDictItem->GetName());
		pDictKey = new char[int(stringLength) + 1];
		errNum = ut_utf8_from_unicode(pDictItem->GetName(), pDictKey, int(stringLength) + 1);
		if (!errNum) {
			return Py_None;
		}
		
		// Translate the dictionary item value from wchar_t* to PyString*
		pDictValue = utils_WideCharToPyObject( pDictItem->GetValue() );		
		if (pDictValue == NULL) {
			return Py_None;
		}

		// Insert the dictionary item into the Python dictionary
		errNum = PyDict_SetItemString( pPyDictionary, pDictKey, pDictValue );
		if (errNum == -1) {
			return Py_None;
		}

		// Cleanup
		delete [] pDictKey;
		pDictItem->Release();
	}
	return pPyDictionary;
}

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
// Remarks:			pStringCollection is not destroyed in this function. 
// ------------------------------------------------------------------------
PyObject* utils_StringCollectionToPyList( const FdoStringCollection* pStringCollection ) {
	if ( pStringCollection == NULL ) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	PyObject *pyList = PyList_New(pStringCollection->GetCount());
	
	for( int index=0; index < pStringCollection->GetCount(); index++ ){				
		PyObject *o = utils_WideCharToPyObject( pStringCollection->GetString(index) );
		PyList_SetItem(pyList,index,o);	
	}

	return pyList;
}


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
PyObject* utils_intArrayToPyList( const int* array, const long arrayLength ) {
	PyObject* newList = NULL;
	int status = 0;

	if ( array == NULL ) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	// Create the Python List..
	newList = PyList_New( arrayLength );

	// And populate the list from the C++ array
	for (int index = 0; index < arrayLength; index++) {
		PyObject *newArrayElement = PyInt_FromLong( (long)array[index] );
		status = PyList_SetItem(newList, index, newArrayElement);
	}

	return newList;
}

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
PyObject* utils_intArrayToPyTuple( const int* array, const long arrayLength ) {
	PyObject* newTuple = NULL;
	int status = 0;

	if ( array == NULL ) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	// Create the Python Tuple..
	newTuple = PyTuple_New( arrayLength );
	if (newTuple == NULL) {
		throw ( L"utils_intArrayToPyTuple failed. Cannot create PyTuple" );
	}

	// And populate the list from the C++ array
	for (int index = 0; index < arrayLength; index++) {
		PyObject *newArrayElement = PyInt_FromLong( (long)array[index] );
		status = PyTuple_SetItem(newTuple, index, newArrayElement);	
	}

	return newTuple;
}




/************************************************************************
 *  Name                                                                *
 *     ut_utf8_to_unicode - Convert a UTF8 string to a UNICODE string.  *
 *     ut_utf8_from_unicode - Convert a UNICODE string to a UTF8 string.*
 *                                                                      *  
 *  Synopsis                                                            *
 *                                                                      *  
 *  Description                                                         *
 *     This is a utility functions to convert a string from UTF8 to     *
 *     wide char UNICODE string. And from UNICODE to UTF8.              *
 *                                                                      *  
 *   Parameters                                                         *
 *                                                                      *  
 *   Function value                                                     *
 *     True if successful; false if severe error.                       *
*************************************************************************/




typedef struct
{
  int     cmask;
  int     cval;
  int     shift;
  long    lmask;
  long    lval;
} Tab;

static Tab tab[] =
{
  0x80,  0x00,   0*6,    0x7F,           0,              /* 1 byte sequence */
  0xE0,  0xC0,   1*6,    0x7FF,          0x80,           /* 2 byte sequence */
  0xF0,  0xE0,   2*6,    0xFFFF,         0x800,          /* 3 byte sequence */
  0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,        /* 4 byte sequence */
  0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,       /* 5 byte sequence */
  0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000,      /* 6 byte sequence */
  0,                                                     /* end of table    */
};

static int UTF8_to_Unicode ( wchar_t *p, char *s, size_t n )
{
  long l;
  int c0, c, nc;
  Tab *t;

  if ( s == 0 )
    return 0;

  nc = 0;
  if ( (int)n <= nc )
    return -1;
  c0 = *s & 0xff;
  l = c0;
  for ( t = tab; t->cmask; t++ ) {
    nc++;
    if ( ( c0 & t->cmask ) == t->cval ) {
      l &= t->lmask;
      if ( l < t->lval )
        return -1;
      *p = (short)l;
      return nc;
    }
    if ( (int)n <= nc )      return -1;
    s++;
    c = ( *s ^ 0x80 ) & 0xFF;
    if ( c & 0xC0 )
      return -1;
    l = ( l << 6 ) | c;
  }
  return -1;
}

static int Unicode_to_UTF8 ( char *s, wchar_t wc )
{
  long l;
  int c, nc;
  Tab *t;

  if (s == 0 )
    return 0;

  l = wc;
  nc = 0;
  for ( t=tab; t->cmask; t++ ) {
    nc++;
    if ( l <= t->lmask ) {
      c = t->shift;
      *s = (char)(t->cval | ( l >> c ));
      while ( c > 0 ) {
        c -= 6;
        s++;
        *s = (char)(0x80 | ( ( l >> c ) & 0x3F ));
      }
      return nc;
    }
  }
  return -1;
}

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
int ut_utf8_to_unicode(
        const char  *   str_in,
        wchar_t *   Wtext,
        int         out_size)
{
    int     i,j;
    int     length;

        length = (int) strlen(str_in);
        for(j=0,i=0; j<length && i<out_size; j++,i++) {
            if (UTF8_to_Unicode ( &Wtext[i], (char *)&str_in[j], 4 ) < 0 )  {
                return FALSE;
            }
            if (Wtext[i] > 2047) j++;
            if (Wtext[i] > 127) j++;
        }
        Wtext[i] = 0;

    return TRUE;
}


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
int ut_utf8_from_unicode(
    const wchar_t   *       Wtext,
    char    *       str_out,
    int         out_max_size)
{
    int i,j;

    for( i=0,j=0; Wtext[i] && j<out_max_size; i++,j++)  {
        if( Unicode_to_UTF8( &str_out[j], Wtext[i] ) < 0 ) {
                return FALSE;
        }
        if (Wtext[i] > 127) j++;
        if (Wtext[i] > 2047) j++;
    }
    str_out[j] = '\0';

    return TRUE;
}



/* To avoid empty translation unit warnings */

int ut_utf8_to_unicode_dummy()
{
	return 0;
}

int ut_utf8_charlen(const char *utf8String)
{
    long wChar;
    int cFirst, cUtf8, nCount;
    Tab *table_entry;

    if ( utf8String == 0 )
        return 0;

    nCount = 0;
    while (*utf8String)
    {
        cFirst = *utf8String & 0xff;
        wChar = cFirst;

        for ( table_entry = tab; table_entry->cmask; table_entry++ )
        {
            if ( ( cFirst & table_entry->cmask ) == table_entry->cval )
            {
                wChar &= table_entry->lmask;
                if ( wChar < table_entry->lval )
                    return -1; //error
                break;
            }

            utf8String++;
            cUtf8 = ( *utf8String ^ 0x80 ) & 0xFF;
            if ( cUtf8 & 0xC0 )
                return -1; //error
            wChar = ( wChar << 6 ) | cUtf8;
        }
        utf8String++;
        nCount++;
    }
    return nCount;
}
