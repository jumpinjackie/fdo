/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include "ElemAttributeSet.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetRoot.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemAttributeSet::ElemAttributeSet(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			lineNumber,
			columnNumber,
			StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET),
	m_qname(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_qname = constructionContext.createXalanQName(
				atts.getValue(i),
				stylesheetTree.getNamespaces(),
				getLocator());
		}
		else if(!(processUseAttributeSets(constructionContext, aname, atts, i) ||
					isAttrOK(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(m_qname == 0)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::TemplateMustHaveAttribute_2Param,
				Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING,
				Constants::ATTRNAME_NAME),
			0,
			this);
	}
	else if (m_qname->isValid() == false)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::TemplateHasIllegalAttribute_2Param
				,Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING
				,Constants::ATTRNAME_NAME),
				0,
				this);
	}
}



ElemAttributeSet::~ElemAttributeSet()
{
}



const XalanDOMString&
ElemAttributeSet::getElementName() const
{
	return Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING;
}



void
ElemAttributeSet::execute(StylesheetExecutionContext&	executionContext) const
{
	typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex		SetAndRestoreCurrentStackFrameIndex;
	typedef StylesheetExecutionContext::ElementRecursionStackPusher				ElementRecursionStackPusher;

	// This will push and pop the stack automatically...
	ElementRecursionStackPusher		thePusher(executionContext, this);

	// Make sure only global variables are visible during execution...
	SetAndRestoreCurrentStackFrameIndex		theSetAndRestore(
					executionContext, 
					executionContext.getGlobalStackFrameIndex());

	ElemUse::execute(executionContext);

	const ElemTemplateElement*	attr = getFirstChildElem();

	while(0 != attr)
	{
		attr->execute(executionContext);

		attr = attr->getNextSiblingElem();
	}
}



void
ElemAttributeSet::addToStylesheet(
			StylesheetConstructionContext&	/* constructionContext */,
			Stylesheet&						theStylesheet)
{
	theStylesheet.getStylesheetRoot().addAttributeSet(*this);
}



bool
ElemAttributeSet::childTypeAllowed(int	xslToken) const
{
	bool	fResult = false;

	switch(xslToken)
	{
	case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE:
		fResult = true;
		break;
		
	default:
		break;
	}

	return fResult;
}



XALAN_CPP_NAMESPACE_END
