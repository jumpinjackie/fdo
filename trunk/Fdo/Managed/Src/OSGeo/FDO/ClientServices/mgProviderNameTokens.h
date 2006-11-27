/*
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
*/

#pragma  once

class FdoProviderNameTokens;

class FdoStringsP;
class FdoVectorP;

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

public __gc class ProviderNameTokens : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public private:
	inline FdoProviderNameTokens* GetImpObj();

protected:
	System::Void ReleaseUnmanagedObject();

public:
	ProviderNameTokens(System::IntPtr unmanaged, System::Boolean autoDelete)
		: Disposable(unmanaged, autoDelete)
	{
	}

    /// \brief
    /// Tokenize a full provider name
    /// The name is usually of the form "[Company].[Provider].[Version]",
    /// e.g. "Autodesk.Oracle.3.2".
    /// 
    /// \param names 
    /// Input the provider name to tokenize
    /// 
    /// \return
    /// Returns FdoProviderNameTokens
    /// 
	ProviderNameTokens(System::String* name);

    /// \brief
    /// Gets all of the tokens in this provider name
    /// 
    /// \return
    /// Returns the collection of tokens. 
    ///   Element 0 is the company
    ///   Element 1 is the unqualified name
    ///   the rest of the elements are the individual parts of the version number.
    /// 
	System::String *GetNameTokens()[];

    /// \brief
    /// Gets all of the version number components in this provider name
    /// 
    /// \return
    /// Returns the collection of version number components. For
    /// example, if the full provider name is "Autodesk.Oracle.3.1" then 
    /// {2,1} is returned.
    /// 
	System::Double GetVersionTokens()[];

    /// \brief
    /// Gets the local (unqualified) name of this provider.
    /// 
    /// \return
    /// Returns the local name ( without company and version ). For
    /// example, if the full provider name is "Autodesk.Oracle.3.1" then 
    /// "Oracle" is returned.
    /// 
	System::String *GetLocalName();
private:
	static System::String *FdoStringsToStringArray(const FdoStringsP &sa)[];
	static System::Double FdoVectorToDoubleArrary(const FdoVectorP &da)[];
};

END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES


