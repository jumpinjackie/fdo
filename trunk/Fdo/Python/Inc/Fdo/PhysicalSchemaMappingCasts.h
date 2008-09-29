// File :  //otwqa/Providers/qa/tools/FdoPythonWrappers/inc/Common/PhysicalSchemaMappingCasts.h
//
// (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
//
// By using this code, you are agreeing to the terms and conditions of
// the License Agreement included in the documentation for this code.
//
// AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
// CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
// IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
// DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
// DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
// Use, duplication, or disclosure by the U.S. Government is subject
// to restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//
// Modification History
// Date       Author          Description
// ---------- --------------- ------------------------------
// 2005-11-30 Sheldon Regular     Initial Release
//
//


#include "Common/FdowStd.h"
#include "Fdo.h"
#include <wchar.h>
#include "Python.h"
#include <string.h>
#include <stdlib.h>

#ifndef FALSE
#   define  FALSE   (0)
#endif

#ifndef TRUE
#   define  TRUE    (1)
#endif

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalSchemaMappingToFdoOracleOvPhysicalSchemaMapping
// Description:   Casts an FdoPhysicalSchemaMapping to an  
//                FdoOracleOvPhysicalSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalSchemaMapping Object
//
//
// Returns:       A FdoOracleOvPhysicalSchemaMapping
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-11-30  Sheldon Regular    Initial Release
//
// ------------------------------------------------------------------------
//FdoOracleOvPhysicalSchemaMapping* FdoPhysicalSchemaMappingToFdoOracleOvPhysicalSchemaMapping( FdoPhysicalSchemaMapping* mapping );

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalSchemaMappingToFdoXmlSchemaMapping
// Description:   Casts an FdoPhysicalSchemaMapping to an  
//                FdoXmlSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalSchemaMapping Object
//
//
// Returns:       A FdoXmlSchemaMapping
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-11-30  Sheldon Regular    Initial Release
//
// ------------------------------------------------------------------------
inline FdoXmlSchemaMapping* FdoPhysicalSchemaMappingToFdoXmlSchemaMapping( FdoPhysicalSchemaMapping* mapping )
{
	return static_cast<FdoXmlSchemaMapping *>(mapping);
}

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalSchemaMappingToFdoOdbcOvPhysicalSchemaMapping
// Description:   Casts an FdoPhysicalSchemaMapping to an  
//                FdoOdbcOvPhysicalSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalSchemaMapping Object
//
//
// Returns:       A FdoOdbcOvPhysicalSchemaMapping
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-11-30  Sheldon Regular    Initial Release
//
// ------------------------------------------------------------------------
//FdoOdbcOvPhysicalSchemaMapping* FdoPhysicalSchemaMappingToFdoOdbcOvPhysicalSchemaMapping( FdoPhysicalSchemaMapping* mapping );

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalSchemaMappingToFdoRfpPhysicalSchemaMapping
// Description:   Casts an FdoPhysicalSchemaMapping to an  
//                FdoRfpPhysicalSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalSchemaMapping Object
//
//
// Returns:       A FdoRfpPhysicalSchemaMapping
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-11-30  Sheldon Regular    Initial Release
//
// ------------------------------------------------------------------------
//FdoRfpPhysicalSchemaMapping* FdoPhysicalSchemaMappingToFdoRfpPhysicalSchemaMapping( FdoPhysicalSchemaMapping* mapping );

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalSchemaMappingToFdoShpPhysicalSchemaMapping
// Description:   Casts an FdoPhysicalSchemaMapping to an  
//                FdoShpPhysicalSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalSchemaMapping Object
//
//
// Returns:       A FdoShpPhysicalSchemaMapping
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-11-30  Sheldon Regular    Initial Release
//
// ------------------------------------------------------------------------
//FdoShpPhysicalSchemaMapping* FdoPhysicalSchemaMappingToFdoShpPhysicalSchemaMapping( FdoPhysicalSchemaMapping* mapping );

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalSchemaMappingToFdoWmsOvPhysicalSchemaMapping
// Description:   Casts an FdoPhysicalSchemaMapping to an  
//                FdoWmsOvPhysicalSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalSchemaMapping Object
//
//
// Returns:       A FdoWmsOvPhysicalSchemaMapping
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-11-30  Sheldon Regular    Initial Release
//
// ------------------------------------------------------------------------
//FdoWmsOvPhysicalSchemaMapping* FdoPhysicalSchemaMappingToFdoWmsOvPhysicalSchemaMapping( FdoPhysicalSchemaMapping* mapping );

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalElementMappingToFdoPhysicalClassMapping
// Description:   Casts an FdoPhysicalElementMapping an  
//                FdoPhysicalClassMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalElementMapping  Object
//
//
// Returns:       A FdoPhysicalClassMapping Object
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-12-07  Jake Zhong     Initial Release
//
// ------------------------------------------------------------------------
inline FdoPhysicalClassMapping * FdoPhysicalElementMappingToFdoPhysicalClassMapping( FdoPhysicalElementMapping * mapping )
{
	return static_cast<FdoPhysicalClassMapping *>(mapping);
}


// ------------------------------------------------------------------------
// Function Name: FdoPhysicalElementMappingToFdoPhysicalPropertyMapping(
// Description:   Casts an FdoPhysicalElementMapping an  
//                FdoPhysicalPropertyMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalElementMapping  Object
//
//
// Returns:       A FdoPhysicalPropertyMapping Object
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-12-07  Jake Zhong     Initial Release
//
// ------------------------------------------------------------------------
inline FdoPhysicalPropertyMapping * FdoPhysicalElementMappingToFdoPhysicalPropertyMapping( FdoPhysicalElementMapping * mapping )
{
	return static_cast<FdoPhysicalPropertyMapping *>(mapping);
}

// ------------------------------------------------------------------------
// Function Name: FdoPhysicalElementMappingToFdoPhysicalSchemaMapping
// Description:   Casts an FdoPhysicalElementMapping an  
//                FdoPhysicalSchemaMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalElementMapping  Object
//
//
// Returns:       A FdoPhysicalSchemaMapping Object
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-12-07  Jake Zhong     Initial Release
//
// ------------------------------------------------------------------------
inline FdoPhysicalSchemaMapping * FdoPhysicalElementMappingToFdoPhysicalSchemaMapping( FdoPhysicalElementMapping * mapping )
{
	return static_cast<FdoPhysicalSchemaMapping *>(mapping);
}


// ------------------------------------------------------------------------
// Function Name: FdoPhysicalElementMappingToFdoXmlElementMapping
// Description:   Casts an FdoPhysicalElementMapping an  
//                FdoXmlElementMapping
// 
// Parameters:    mapping (mandatory input)
//					- A FdoPhysicalElementMapping  Object
//
//
// Returns:       A FdoXmlElementMapping Object
//
// Revision History:
// Date:       Name:          Description:
// ------------------------------------------------------------------------
// 2005-12-07  Jake Zhong     Initial Release
//
// ------------------------------------------------------------------------
inline FdoXmlElementMapping * FdoPhysicalElementMappingToFdoXmlElementMapping( FdoPhysicalElementMapping * mapping )
{
	return static_cast<FdoXmlElementMapping *>(mapping);
}

