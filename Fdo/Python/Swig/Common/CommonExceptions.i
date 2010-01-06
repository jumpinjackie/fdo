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


// -------------------------------------------------------------
// PYTHON exceptions
//	Description:	The standard global exception handler for
//					all FDO method calls.  This exception handler
//					will try to catch all known FDO exceptions,
//					and throw the corresponding Python exception
//					proxy class.
//  
//                  Any new FDO exception classes must have its
//                  own "catch" construct, and should be added here.
//                  Note: each exception has a global pointer,
//                  and it should be declared in CommonInit.i
//
// -------------------------------------------------------------

 %exception {
	try {
		$action
	}
	catch ( FdoExpressionException *ex ) {
		PyErr_SetObject(pFdoExpressionException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoExpressionException, 1) );
		goto fail;
	}
	catch ( FdoFilterException *ex ) {
		PyErr_SetObject(pFdoFilterException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoFilterException, 1) );
		goto fail;
	}
	catch ( FdoSchemaException *ex ) {
		PyErr_SetObject(pFdoSchemaException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoSchemaException, 1) );
		goto fail;
	}
	catch ( FdoCommandException *ex ) {
		PyErr_SetObject(pFdoCommandException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoCommandException, 1) );
		goto fail;
	}
	catch ( FdoConnectionException *ex ) {
		PyErr_SetObject(pFdoConnectionException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoConnectionException, 1) );
		goto fail;
	}	
	catch ( FdoXmlException *ex) {
		PyErr_SetObject(pFdoXmlException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoXmlException, 1) );
		goto fail;
	}
	catch (FdoSpatialContextMismatchException *ex) {
		PyErr_SetObject(pFdoSpatialContextMismatchException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoSpatialContextMismatchException, 1) );
		goto fail;
	}
	catch ( FdoException *ex ) {
		PyErr_SetObject(pFdoException, SWIG_NewPointerObj((void *) ex, SWIGTYPE_p_FdoException, 1) );
		goto fail;
	}
	catch ( wchar_t *wcharMsg ) {
		PyErr_SetObject(PyExc_RuntimeError, SWIG_FromWCharPtr(wcharMsg));
		goto fail;
	}
	catch ( char *errMsg ) {
		PyErr_SetString(PyExc_RuntimeError, errMsg);
		goto fail;
	}
	catch (...) {
		PyErr_SetString(PyExc_RuntimeError, "Unknown exception occurred.");
		goto fail;
	}
}


 /* -------------------------------------------------------------
 * Extend FdoException
 *	Description:	Add the __str__ method to FdoException.
 *					This makes FDO exceptions more "pythonic" by
 *					providing a default string represntation of
 *					an FdoException instance.
 * -------------------------------------------------------------
 */
 %extend FdoException {	
	 FdoString* __str__() {		
		// Function: __str__
		// Description: The __str__ method is the standard	Python method to return
		//				a string representation of an object instance, and is analgous
		//				to Java's 'ToString()' methods.  This method will support
		//				a more 'Pythonic' way of handling exception and exception messages.
		// Parameters:  None
		// Returns:		A FdoString* represntation of the exception.  
		
		FdoString *returnVal = NULL;
		StringBuffer *buffer = new StringBuffer(self->GetExceptionMessage());		
		FdoException* exceptionCause = self->GetCause();

		while (exceptionCause) {
			buffer->append(L"\n\tException cause: ");
			buffer->append(exceptionCause->GetExceptionMessage());
			exceptionCause = exceptionCause->GetCause();
		}	
		
		returnVal = buffer->toString();
		delete buffer;
		return returnVal;
	 }
 };


