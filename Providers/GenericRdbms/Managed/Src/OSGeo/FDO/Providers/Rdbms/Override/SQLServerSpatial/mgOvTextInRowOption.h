/*
* (C) Copyright 2005 by Autodesk, Inc. All Rights Reserved.
*
* By using this code, you are agreeing to the terms and conditions of
* the License Agreement included in the documentation for this code.
*
* AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
* CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
* IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
* DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
* DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
*
* Use, duplication, or disclosure by the U.S. Government is subject
* to restrictions set forth in FAR 52.227-19 (Commercial Computer
* Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
* (Rights in Technical Data and Computer Software), as applicable.
*
*
*/

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL

/// <summary>Enumerated type representing the possible text-in-row options for SQL Server overrides</summary>
/// <param name="OvTextInRowOption_InRow">Text, ntext and image data are stored within the row's 8K page.</param>
/// <param name="OvTextInRowOption_NotInRow">Text, ntext and image data are not stored within the row's 8K page.</param>
/// <param name="OvTextInRowOption_Default">The text-in-row override is inherited from somewhere else.</param>
public __value enum OvTextInRowOption 
{
	OvTextInRowOption_InRow = SqlServerOvTextInRowOption_InRow,
    OvTextInRowOption_NotInRow = SqlServerOvTextInRowOption_NotInRow,
	OvTextInRowOption_Default = SqlServerOvTextInRowOption_Default
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
