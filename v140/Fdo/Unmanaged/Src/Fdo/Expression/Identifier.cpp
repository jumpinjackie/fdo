// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <wctype.h>
#include <Fdo/Expression/Identifier.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"

// defined in Lex.cpp.
bool IsKeyWord(FdoString* word);

// Constructs a default instance of an Identifier.
FdoIdentifier* FdoIdentifier::Create()
{
    return new FdoIdentifier();
}

// Constructs an instance of an Identifier using the specified arguments.
FdoIdentifier* FdoIdentifier::Create(FdoString* text)
{
    return new FdoIdentifier(text);
}

FdoIdentifier::~FdoIdentifier()
{
	FdoStringUtility::ClearString(m_text);
    FdoStringUtility::ClearString(m_name);
    FdoStringUtility::ClearString(m_schemaName);
    if (m_scope != NULL)
    {
        for (FdoInt32 i = 0; i < m_scopelen; i++)
        {
			if (m_scope[i] != NULL)
	            delete [] m_scope[i];
        }
        delete[] m_scope;
	}
}

void FdoIdentifier::Dispose()
{
    delete this;
}

// Constructs a default instance of an Identifier.
FdoIdentifier::FdoIdentifier()
{
    m_text = NULL;
    m_name = NULL;
    m_scope = NULL;
    // create an empty string
    m_schemaName = new wchar_t[1];
    m_schemaName[0] = '\0';
    m_scopelen = 0;
    generateScope = true;
	generateName = true;
    generateSchemaName = true;
}

// Constructs an instance of an Identifier using the specified arguments.
FdoIdentifier::FdoIdentifier(FdoString* text)
{
    m_text = FdoStringUtility::MakeString(text);
    m_name = NULL;
    m_scope = NULL;
    // create an empty string
    m_schemaName = new wchar_t[1];
    m_schemaName[0] = '\0';
    m_scopelen = 0;
    generateScope = true;
	generateName = true;
    generateSchemaName = true;
}

// Gets the full text of the identifier.
FdoString* FdoIdentifier::GetText()
{
	return m_text;
}

// Sets the full text of the identifier.
void FdoIdentifier::SetText(FdoString* value)
{
    FdoStringUtility::ClearString(m_text);
    m_text = FdoStringUtility::MakeString(value);
    generateScope = true;
	generateName = true;
    generateSchemaName = true;
}

// Gets just the name of the identifier, with any scope stripped off.
FdoString* FdoIdentifier::GetName()
{
    if (generateName)
    {
		if (m_name != NULL)
            FdoStringUtility::ClearString(m_name);

        generateName = false;
        if (m_text != NULL)
        {
            const wchar_t*	temp;
            const wchar_t*	name = NULL;

            temp = FdoStringUtility::FindCharacter(m_text, L':');
            if( temp == NULL )
                temp = FdoStringUtility::FindCharacter(m_text, L'.');
            else
            {
                name = &temp[1];
                temp = FdoStringUtility::FindCharacter(name, L'.');
                if( temp == NULL )
                {
                    m_name = FdoStringUtility::MakeString(name);
                    return m_name;
                }
            }
            while (temp != NULL)
            {
                name = temp;
                name = &name[1];
                temp = FdoStringUtility::FindCharacter(name, L'.');
            }

            if (name != NULL)
            {
                m_name = FdoStringUtility::MakeString(name);
            }
            else if (m_text != NULL)
		    {
                m_name = FdoStringUtility::MakeString(m_text);
		    }
        }
    }
	return m_name;
}

// Gets the scope of the identifier as an array of Strings.
FdoString** FdoIdentifier::GetScope(FdoInt32& length)
{
    if ( ! generateScope )
	{
		length = m_scopelen;
		return (FdoString**)m_scope;
	}
    
	if (m_scope != NULL)
	{
		for (FdoInt32 i = 0; i < m_scopelen; i++)
		{
			delete [] m_scope[i];
		}
		delete[] m_scope;
		m_scope = NULL;
	}

    if (m_text == NULL)
    {
        length = m_scopelen = 0;
        return (FdoString**)m_scope;
    }

    generateScope = false;
    wchar_t** scope;
	const wchar_t* name, *temp;
    FdoInt32 scopelen = 0;

    temp = FdoStringUtility::FindCharacter(m_text, L'.');
    // Finds number of scopes first, so array does not have to be
    // continuously reallocated later.
    while (temp != NULL)
    {
        scopelen++;
        name = temp;
        name = &name[1];
        temp = FdoStringUtility::FindCharacter(name, L'.');
    }
    if (scopelen != 0)
    {
        length = m_scopelen = scopelen;
        FdoInt32 len;
        const wchar_t* oldname = m_text;
        scope = new wchar_t*[scopelen];

        temp = FdoStringUtility::FindCharacter(m_text, L':');
        if( temp == NULL )
            temp = FdoStringUtility::FindCharacter(m_text, L'.');
        else
        {
            oldname = &temp[1];
            temp = FdoStringUtility::FindCharacter(&temp[1], L'.');
        }
        scopelen = 0;
        while (temp != NULL)
        {
            name = temp;
            name = &name[1];
            len = (FdoInt32)(FdoStringUtility::StringLength(oldname) - FdoStringUtility::StringLength(name));
            scope[scopelen] = new wchar_t[len];
            FdoStringUtility::SubstringCopy(scope[scopelen], oldname, len - 1);
            scope[scopelen][len-1] = '\0';
            temp = FdoStringUtility::FindCharacter(name, L'.');
            oldname = name;
            scopelen++;
        }
        //scope[scopelen] = new wchar_t[wcslen(name) + 1];
        //wcscpy(scope[scopelen], name);
        m_scope = scope;
        return (FdoString**)m_scope;
    }
    else
	{
		length = 0;
        return NULL;
	}
   
}

// Gets the schema name part of the identifier.
FdoString* FdoIdentifier::GetSchemaName()
{
    if (generateSchemaName)
    {
        FdoStringUtility::ClearString(m_schemaName);

        generateSchemaName = false;
        if (m_text != NULL)
        {
            const wchar_t*	temp;
            const wchar_t*	name = NULL;

            temp = FdoStringUtility::FindCharacter(m_text, L':');
            if (temp != NULL)
            {
                size_t len = FdoStringUtility::StringLength(m_text) - FdoStringUtility::StringLength(temp);
                m_schemaName = new wchar_t[len+1];
                FdoStringUtility::SubstringCopy(m_schemaName, m_text, len);
                m_schemaName[len] = '\0';
            }
            else
            {
                // create an empty string
                m_schemaName = new wchar_t[1];
                m_schemaName[0] = '\0';
            }
        }
        else
        {
            // create an empty string
            m_schemaName = new wchar_t[1];
            m_schemaName[0] = '\0';
        }
    }
	return m_schemaName;
}

// Overrides Expression.Process to pass the Identifier to the appropriate
// expression processor operation.
void FdoIdentifier::Process(FdoIExpressionProcessor* p)
{
    p->ProcessIdentifier(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoIdentifier::ToString()
{
    FdoStringUtility::ClearString(m_toString);

    if (GetText() == NULL)
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_16_NULLIDENTIFIER)));
        throw pExcept;
    }

	bool			bQuote = false;
	const wchar_t*	wzText = GetText();

	// first check if text is starts with an alpha character.
	// Quote the string if not.
	if (!iswalpha(*wzText))
		bQuote = true;
	wzText++;

	// next check if consists of only alpha characters, numbers, underscores and dots.
	// Quote the string if not.
	while (!bQuote && *wzText)
	{
		if (!iswalnum(*wzText) && *wzText != '_' && *wzText != '.')
			bQuote = true;	// will force us out of the loop

		wzText++;
	}

	// Then check if the text is a keyword from the parse grammer
	// Quote the string if it is.
	if (!bQuote && IsKeyWord(GetText()))
		bQuote = true;
    
	// return quoted string if necessary
	if (bQuote)
	{
		m_toString = FdoStringUtility::QuoteString(GetText(), L'"');
		return m_toString;
	}
	else
	    return GetText();
}

