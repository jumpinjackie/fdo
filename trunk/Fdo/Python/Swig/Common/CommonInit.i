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

%{
	// ----------- Includes from CommonInit.i ------------
	#include "FdoStd.h"
	#include "Common/utils.h"
	#include "Common/StringBuffer.h"
	#include <new>
	#include <string.h>
	#include "Fdo.h"
	#include "Common/FdowStd.h"
	#include "Common/FdoProxyConnectionManager.h"

	// PyObjects representing Fdo exceptions
	PyObject *pFdoException;
	PyObject *pFdoCommandException;
	PyObject *pFdoConnectionException;
	PyObject *pFdoExpressionException;
	PyObject *pFdoFilterException;
	PyObject *pFdoSchemaException;
	PyObject *pFdoXmlException;
	PyObject *pFdoSpatialContextMismatchException;

	// Add any new FDO exception pointers here.
%}



/* Handle operator overloading for all classes*/

%rename(__divide__) *::operator/; 
%rename(__multiply__) *::operator*; 
%rename(__subtract__) *::operator-; 
%rename(__add__) *::operator+; 
%rename(__assign__) *::operator=; 
%rename(__equals__) *::operator==; 
%rename(__notequals__) *::operator!=;




/* -------------------------------------------------------------
 * Wrapper initialization
 *	Description:	These commands are inserted during the initialization
 *					of the wrapper.  The FdoException classes are
 *					reFdotered here, and single pointers to the exceptions
 *					are created.
 * -------------------------------------------------------------
 */
 
%init %{
	// Custom initialization code from FdoExceptions.i
	// Create and initialize Python exception pointers used to represent
	// FDO pointers 
	pFdoException = PyErr_NewException( "FDOw.FdoException", NULL, NULL );
	if (pFdoException != NULL ) {		
		PyDict_SetItemString( d, "FdoException", pFdoException );
	}

	pFdoException = PyErr_NewException( "FDOw.FdoException", NULL, NULL );
	if (pFdoException != NULL ) {		
		PyDict_SetItemString( d, "FdoException", pFdoException );
	}	

	pFdoCommandException = PyErr_NewException( "FDOw.FdoCommandException", NULL, NULL );
	if (pFdoCommandException != NULL ) {
		PyDict_SetItemString( d, "FdoCommandException", pFdoCommandException );
	}

	pFdoConnectionException = PyErr_NewException( "FDOw.FdoConnectionException", NULL, NULL );
	if (pFdoConnectionException != NULL ) {
		PyDict_SetItemString( d, "FdoConnectionException", pFdoConnectionException );
	}

	pFdoExpressionException = PyErr_NewException( "FDOw.FdoExpressionException", NULL, NULL );
	if (pFdoExpressionException != NULL ) {
		PyDict_SetItemString( d, "FdoExpressionException", pFdoExpressionException );
	}

	pFdoFilterException = PyErr_NewException( "FDOw.FdoFilterException", NULL, NULL );
	if (pFdoFilterException != NULL ) {
		PyDict_SetItemString( d, "FdoFilterException", pFdoFilterException );
	}

	pFdoSchemaException = PyErr_NewException( "FDOw.FdoSchemaException", NULL, NULL );
	if (pFdoSchemaException != NULL ) {
		PyDict_SetItemString( d, "FdoSchemaException", pFdoSchemaException );
	}

	pFdoXmlException = PyErr_NewException( "FDOw.FdoXmlException", NULL, NULL );
	if (pFdoXmlException != NULL ) {
		PyDict_SetItemString( d, "FdoXmlException", pFdoXmlException );
	}

	pFdoSpatialContextMismatchException = PyErr_NewException( "FDOw.FdoSpatialContextMismatchException", NULL, NULL );
	if (pFdoSpatialContextMismatchException != NULL ) {
		PyDict_SetItemString( d, "FdoSpatialContextMismatchException", pFdoSpatialContextMismatchException );
	}
	// End custom initialization code from Main.i
%}

/* -------------------------------------------------------------
 *  Description:    Ignore these constructs in the API
 * -------------------------------------------------------------
 */

%ignore FdoException::Dispose();
%ignore FdoException::NLSGetMessage;
