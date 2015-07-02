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

#include "stdafx.h"
#include "FdoWfsOgcScalarCapabilities.h"


FdoWfsOgcScalarCapabilities::FdoWfsOgcScalarCapabilities() : m_comparisonOperators(0),
                        m_logicalOperators(false), m_simpleArithmetic(false)
{
}

FdoWfsOgcScalarCapabilities::~FdoWfsOgcScalarCapabilities()
{
}

FdoWfsOgcScalarCapabilities* FdoWfsOgcScalarCapabilities::Create()
{
    return new FdoWfsOgcScalarCapabilities();
}

void FdoWfsOgcScalarCapabilities::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoWfsOgcScalarCapabilities::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;
    try 
    {
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));

        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);
        switch (myContext->StateScalar_Capabilities())
        {
        case 0:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Logical_Operators) == 0)
                    m_logicalOperators = true;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Comparison_Operators) == 0)
                    myContext->SetStateScalar_Capabilities(1);
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Arithmetic_Operators) == 0)
                    myContext->SetStateScalar_Capabilities(2);
                // For WFS 1.1
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::LogicalOperators) == 0)
                    m_logicalOperators = true;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::ComparisonOperators) == 0)
                    myContext->SetStateScalar_Capabilities(3);
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::ArithmeticOperators) == 0)
                    myContext->SetStateScalar_Capabilities(4);
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::Scalar_Capabilities));

                break;
            }
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Simple_Comparisons) == 0)
                    m_comparisonOperators |= ComparisonOperators_Simple;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Like) == 0)
                    m_comparisonOperators |= ComparisonOperators_Like;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::NullCheck) == 0)
                    m_comparisonOperators |= ComparisonOperators_NullCheck;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Between) == 0)
                    m_comparisonOperators |= ComparisonOperators_Between;
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::Comparison_Operators));
                break;
            }
        case 2:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Simple_Arithmetic) == 0)
                    m_simpleArithmetic = true;
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::Arithmetic_Operators));

                break;
            }
        case 3:
            {
                // WFS 1.1.0 uses <ogc:ComparisonOperator>...</ogc:ComparisonOperator> to define 
                // the supported comparison operator.
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::ComparisonOperator) == 0)
                {
                    mXmlContentHandler = FdoXmlCharDataHandler::Create();
                    pRet = mXmlContentHandler;
                }
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::ComparisonOperator));

                break;
            }
        case 4:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::SimpleArithmetic) == 0)
                    m_simpleArithmetic = true;
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoWfsGlobals::ArithmeticOperators));

                break;
            }
        default:
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;

}

FdoBoolean FdoWfsOgcScalarCapabilities::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
    {
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));

        FdoWfsCapabilitiesSaxContext* myContext = static_cast<FdoWfsCapabilitiesSaxContext*>(context);
        switch (myContext->StateScalar_Capabilities())
        {
        case 0:
            break;
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Comparison_Operators) == 0)
                    myContext->SetStateScalar_Capabilities(0);
                break;
            }
        case 2:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::Arithmetic_Operators) == 0)
                    myContext->SetStateScalar_Capabilities(0);
                break;
            }
        case 3:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::ComparisonOperator) == 0)
                {
                    if (mXmlContentHandler != NULL)
                    {
                        FdoStringP name = mXmlContentHandler->GetString();

                        // FDO condition type doesn't distinguish equal, not equal, greater than, less than, 
                        // greater than or equal to, or less than or equal to. So once finding one of those
                        // comparision operations, we assume all of them are supported.
                        if (name == FdoWfsGlobals::LessThan ||
                            name == FdoWfsGlobals::GreaterThan ||
                            name == FdoWfsGlobals::LessThanEqualTo ||
                            name == FdoWfsGlobals::GreaterThanEqualTo ||
                            name == FdoWfsGlobals::EqualTo ||
                            name == FdoWfsGlobals::NotEqualTo)
                            m_comparisonOperators |= ComparisonOperators_Simple;
                        else if (name == FdoWfsGlobals::Like)
                            m_comparisonOperators |= ComparisonOperators_Like;
                        else if (name == FdoWfsGlobals::NullCheck)
                            m_comparisonOperators |= ComparisonOperators_NullCheck;
                        else if (name == FdoWfsGlobals::Between)
                            m_comparisonOperators |= ComparisonOperators_Between;
                    }
                }
                else if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::ComparisonOperators) == 0)
                    myContext->SetStateScalar_Capabilities(0);

                break;
            }
        case 4:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoWfsGlobals::ArithmeticOperators) == 0)
                    myContext->SetStateScalar_Capabilities(0);
                break;
            }
        default:
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return ret;
}



