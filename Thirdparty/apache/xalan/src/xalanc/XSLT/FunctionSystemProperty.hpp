/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(FUNCTIONSYSTEMPROPERTY_HEADER_GUARD_1357924680)
#define FUNCTIONSYSTEMPROPERTY_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// Implementation of the XSLT function sytsem-property().
//
class XALAN_XSLT_EXPORT FunctionSystemProperty : public Function
{
public:

    typedef Function    ParentType;

    FunctionSystemProperty(MemoryManager& theManager);

    FunctionSystemProperty( const FunctionSystemProperty& other, MemoryManager& theManager);

    virtual
    ~FunctionSystemProperty();

    // These methods are inherited from Function ...

    virtual XObjectPtr
    execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg,
            const Locator*          locator) const;

    using ParentType::execute;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual FunctionSystemProperty*
#endif
    clone(MemoryManager& theManager) const;

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString& theResult) const;

private:

    FunctionSystemProperty(const FunctionSystemProperty&);

    // Not implemented...
    FunctionSystemProperty&
    operator=(const FunctionSystemProperty&);

    bool
    operator==(const FunctionSystemProperty&) const;


    // Data members...
    const XalanDOMString    m_xsltNamespaceURI;
    const XalanDOMString    m_versionPropertyString;
    const XalanDOMString    m_vendorPropertyString;
    const XalanDOMString    m_vendorURLPropertyString;
    const XalanDOMString    m_vendorString;
    const XalanDOMString    m_vendorURLString;
};



XALAN_CPP_NAMESPACE_END



#endif  // FUNCTIONSYSTEMPROPERTY_HEADER_GUARD_1357924680
