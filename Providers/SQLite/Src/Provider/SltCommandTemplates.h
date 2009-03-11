// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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

//Base class for all commands. Defines common functionality and
//behavior.
template <class COMMAND> class SltCommand : public COMMAND
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        SltCommand(SltConnection* connection)
        {
            m_connection = connection;
            m_connection->AddRef();
        }

    protected:
        // default destructor
        virtual ~SltCommand()
        {
            m_connection->Release();
        }

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        virtual void Dispose() { delete this; }

    //-------------------------------------------------------
    // FdoICommand implementation
    //-------------------------------------------------------

    public:
        virtual FdoIConnection* GetConnection()
        {
            m_connection->AddRef();
            return m_connection;
        }

        virtual FdoITransaction* GetTransaction()           { return NULL; }
        virtual void SetTransaction(FdoITransaction* value) { }
        virtual FdoInt32 GetCommandTimeout()                { return 0;}
        virtual void SetCommandTimeout(FdoInt32 value)      { }
        virtual FdoParameterValueCollection* GetParameterValues(){ return NULL; }
        virtual void Prepare()                              { }
        virtual void Cancel()                               { }

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    protected:
        SltConnection* m_connection;

};

//Base class  template for all feature commands. Defines common functionality and
//behavior.
template <class FEATURECOMMAND> class SltFeatureCommand : public SltCommand<FEATURECOMMAND>
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        SltFeatureCommand(SltConnection* connection) 
            : SltCommand<FEATURECOMMAND>(connection)
        {
            m_className = NULL;
            m_filter = NULL;
        }

    protected:
        virtual ~SltFeatureCommand()
        {
            FDO_SAFE_RELEASE(m_className);
            FDO_SAFE_RELEASE(m_filter);
        }

    //-------------------------------------------------------
    // FdoIFeatureCommand implementation
    //-------------------------------------------------------

    public:
        virtual FdoIdentifier* GetFeatureClassName()
        {
            return FDO_SAFE_ADDREF(m_className);
        }

        virtual void SetFeatureClassName(FdoIdentifier* value)
        {
            FDO_SAFE_RELEASE(m_className);
            m_className = value;
            if (m_className) m_className->AddRef();
        }

        virtual void SetFeatureClassName(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_className);
            if (value != NULL)
                m_className = FdoIdentifier::Create(value);
        }

        virtual FdoFilter* GetFilter()
        {
            return FDO_SAFE_ADDREF(m_filter);
        }

        virtual void SetFilter(FdoFilter* value)
        {
            FDO_SAFE_RELEASE(m_filter);
            m_filter = value;
            if (m_filter) m_filter->AddRef();
        }

        virtual void SetFilter(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_filter);
            if (value != NULL)
                m_filter = FdoFilter::Parse(value);
        }

    protected:

        FdoIdentifier*  m_className;
        FdoFilter*      m_filter;
};
