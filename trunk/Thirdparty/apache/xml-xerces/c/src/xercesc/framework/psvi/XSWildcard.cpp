/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
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
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log: XSWildcard.cpp,v $
 * Revision 1.5  2003/11/21 17:34:04  knoaman
 * PSVI update
 *
 * Revision 1.4  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.3  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/util/StringPool.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSWildcard: Constructors and Destructor
// ---------------------------------------------------------------------------
XSWildcard::XSWildcard(SchemaAttDef* const  attWildCard,
                       XSAnnotation* const  annot,
                       XSModel* const       xsModel,
                       MemoryManager* const manager)
    : XSObject(XSConstants::WILDCARD, xsModel, manager)
    , fConstraintType(NSCONSTRAINT_ANY)
    , fProcessContents(PC_STRICT)
    , fNsConstraintList(0)
    , fAnnotation(annot)
{
    XMLAttDef::AttTypes attType = attWildCard->getType();
    if (attType == XMLAttDef::Any_Other)
    {
        fConstraintType = NSCONSTRAINT_NOT;
        fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(1, true, manager);
        fNsConstraintList->addElement
        (
            XMLString::replicate(fXSModel->getURIStringPool()->getValueForId(
                    attWildCard->getAttName()->getURI()), manager)
        );
    }
    else if (attType == XMLAttDef::Any_List)
    {
        fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
        ValueVectorOf<unsigned int>* nsList = attWildCard->getNamespaceList();
        if (nsList)
        {
            unsigned int nsListSize = nsList->size();
            if (nsListSize)
            {
                fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(nsListSize, true, manager);
                for (unsigned int i=0; i < nsListSize; i++)
                {
                    fNsConstraintList->addElement
                    (
                        XMLString::replicate
                        (
                            fXSModel->getURIStringPool()->getValueForId
                            (
                                nsList->elementAt(i)
                            )
                            , manager
                        )
                    );
                }
            }
        }
    }

    XMLAttDef::DefAttTypes attDefType = attWildCard->getDefaultType();
    if (attDefType == XMLAttDef::ProcessContents_Skip)
        fProcessContents = PC_SKIP;
    else if (attDefType == XMLAttDef::ProcessContents_Lax)
        fProcessContents = PC_LAX;
}

XSWildcard::XSWildcard(const ContentSpecNode* const elmWildCard,
                       XSAnnotation* const annot,
                       XSModel* const xsModel,
                       MemoryManager* const manager)
    : XSObject(XSConstants::WILDCARD, xsModel, manager)
    , fConstraintType(NSCONSTRAINT_ANY)
    , fProcessContents(PC_STRICT)
    , fNsConstraintList(0)
    , fAnnotation(annot)
{
    ContentSpecNode::NodeTypes nodeType = elmWildCard->getType();
    if ((nodeType & 0x0f) == ContentSpecNode::Any_Other)
    {
        fConstraintType = NSCONSTRAINT_NOT;
        if (nodeType == ContentSpecNode::Any_Other_Lax)
            fProcessContents = PC_LAX;
        else if (nodeType == ContentSpecNode::Any_Other_Skip)
            fProcessContents = PC_SKIP;
    }
    else if ((nodeType & 0x0f) == ContentSpecNode::Any_NS)
    {
        fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
        if (nodeType == ContentSpecNode::Any_NS_Lax)
            fProcessContents = PC_LAX;
        else if (nodeType == ContentSpecNode::Any_NS_Skip)
            fProcessContents = PC_SKIP;
    }
    else if (nodeType == ContentSpecNode::Any_NS_Choice)
    {
        fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
        ContentSpecNode::NodeTypes anyType = elmWildCard->getFirst()->getType();

        if (anyType == ContentSpecNode::Any_NS_Lax)
            fProcessContents = PC_LAX;
        else if (anyType == ContentSpecNode::Any_NS_Skip)
            fProcessContents = PC_SKIP;

        fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(4, true, manager);
        buildNamespaceList(elmWildCard);
    }
    // must be any
    else
    {
        if (nodeType == ContentSpecNode::Any_Lax)
            fProcessContents = PC_LAX;
        else if (nodeType == ContentSpecNode::Any_Skip)
            fProcessContents = PC_SKIP;
    }

    if (fConstraintType == NSCONSTRAINT_NOT
        || (fConstraintType == NSCONSTRAINT_DERIVATION_LIST
            && !fNsConstraintList))
    {
        fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(1, true, manager);
        fNsConstraintList->addElement
        (
             XMLString::replicate(fXSModel->getURIStringPool()->getValueForId(
                     elmWildCard->getElement()->getURI()), manager)
        );
    }
}

XSWildcard::~XSWildcard() 
{
    if (fNsConstraintList) 
        delete fNsConstraintList;
}

// ---------------------------------------------------------------------------
//  XSWildcard: helper methods
// ---------------------------------------------------------------------------
void XSWildcard::buildNamespaceList(const ContentSpecNode* const rootNode)
{
    ContentSpecNode::NodeTypes nodeType = rootNode->getType();
    if (nodeType == ContentSpecNode::Any_NS_Choice)
    {
        buildNamespaceList(rootNode->getFirst());
        buildNamespaceList(rootNode->getSecond());
    }
    else
    {
        fNsConstraintList->addElement
        (
            XMLString::replicate(fXSModel->getURIStringPool()->getValueForId(
                    rootNode->getElement()->getURI()), fMemoryManager)
        );
    }
}

XERCES_CPP_NAMESPACE_END
