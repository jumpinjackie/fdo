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

class SltConnection;

//spatial context reader -- returned when executing get spatial contexts command
class SltSpatialContextReader : public FdoISpatialContextReader
{
    //-------------------------------------------------------
    // Constructors / Destructor
    //-------------------------------------------------------

    public:

        SltSpatialContextReader (SltConnection* conn);

    protected:

        virtual ~SltSpatialContextReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

        void Dispose();

    //-------------------------------------------------------
    // FdoISpatialContextReader implementation
    //-------------------------------------------------------

    public:

        SLT_API virtual FdoString*                  GetName();
        SLT_API virtual FdoString*                  GetDescription();
        SLT_API virtual FdoString*                  GetCoordinateSystem();
        SLT_API virtual FdoString*                  GetCoordinateSystemWkt();
        SLT_API virtual FdoSpatialContextExtentType GetExtentType();
        SLT_API virtual FdoByteArray*               GetExtent();
        SLT_API virtual const double                GetXYTolerance();
        SLT_API virtual const double                GetZTolerance();
        SLT_API virtual const bool                  IsActive();
        SLT_API virtual bool                        ReadNext();

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
        SltConnection*  m_connection;
        sqlite3_stmt*   m_pStmt;
        std::wstring    m_wkt;
        std::wstring    m_cs;
        std::wstring    m_name;
        bool            m_bHasScName;
        bool            m_bToleranceSupp;
};

