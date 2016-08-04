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
#ifndef SDFFEATURECOMMAND_H
#define SDFFEATURECOMMAND_H

#include "SdfCommand.h"

//Base class for all commands. Defines common functionality and
//behavior.
template <class FEATURECOMMAND> class SdfFeatureCommand : public SdfCommand<FEATURECOMMAND>
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a command using the specified connection
        SdfFeatureCommand(SdfConnection* connection)
            : SdfCommand<FEATURECOMMAND>(connection)
        {
            m_className = NULL;
            m_filter = NULL;
        }

    protected:
        // default destructor
        virtual ~SdfFeatureCommand()
        {
            FDO_SAFE_RELEASE(m_className);
            FDO_SAFE_RELEASE(m_filter);
        }


    //-------------------------------------------------------
    // FdoIFeatureCommand implementation
    //-------------------------------------------------------

    public:
        // Gets the name of the class to be operated upon as an Identifier.
        SDF_API virtual FdoIdentifier* GetFeatureClassName()
        {
            // addref the class name because we own it
            FDO_SAFE_ADDREF(m_className);
            return m_className;
        }

        // Sets the name of the class to be operated upon as an Identifier.
        SDF_API virtual void SetFeatureClassName(FdoIdentifier* value)
        {
            FDO_SAFE_RELEASE(m_className);

            m_className = value;

            FDO_SAFE_ADDREF(m_className);
        }

        // Sets the name of the class to be operated upon as an Identifier.
        SDF_API virtual void SetFeatureClassName(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_className);

            if (value != NULL)
                m_className = FdoIdentifier::Create(value);
        }

        // Gets the filter as a Filter tree.
        SDF_API virtual FdoFilter* GetFilter()
        {
            // addref the filter because we own it
            FDO_SAFE_ADDREF(m_filter);
            return m_filter;
        }

        // Sets the filter as a Filter tree.
        SDF_API virtual void SetFilter(FdoFilter* value)
        {
            FDO_SAFE_RELEASE(m_filter);

            m_filter = value;

            FDO_SAFE_ADDREF(m_filter);
        }

        // Sets the filter as a Filter tree, using the supplied
        // filter expression string.
        SDF_API virtual void SetFilter(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_filter);

            if (value != NULL)
                m_filter = FdoFilter::Parse(value);
        }

    protected:

        FdoIdentifier* m_className;
        FdoFilter* m_filter;
};

#endif

