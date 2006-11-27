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
#ifndef FDOCOMMONQUERYAGGREGATOR_H
#define FDOCOMMONQUERYAGGREGATOR_H

#include <FdoCommonPropertyIndex.h>
#include <FdoCommonFilterExecutor.h>



struct AggregateStats
{
    double   doubleVal;
    double   doubleValAccumulator;
    FdoInt64 int64Val;
    FdoPtr<FdoIGeometry> geomVal;
    bool     isNull;
};


/// \brief
/// Overloads evaluation of functions with support for aggregate functions
/// like Min and Max.
class FdoCommonQueryAggregator : public FdoCommonFilterExecutor
{

public:

    FdoCommonQueryAggregator(FdoIConnection *conn, FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds, FdoIFeatureReader* reader, FdoArray<FdoFunction*>* aggrFunctions);
    ~FdoCommonQueryAggregator();

protected:

    void Dispose();

public:


    //--------------------------------------------------------------------
    /// FdoIExpressionProcessor
    //--------------------------------------------------------------------

    virtual void ProcessFunction(FdoFunction& expr);

public:

    //-----------------------------------------------------------------
    /// FdoCommonQueryAggregator
    //-----------------------------------------------------------------
    
    FdoPropertyValueCollection* RunQuery();

    /// Helper methods:
    static bool IsAggregateFunction(FdoString* name);


private:

    FdoPtr<FdoIConnection> m_connection;
    std::vector<AggregateStats> m_aggrStats;
    int m_aggrStatsIndex;
    bool m_EvaluatingAggregates;
    FdoPtr<FdoClassDefinition> m_originalClassDef;
    FdoPtr<FdoIdentifierCollection> m_selectedIds;
    FdoPtr< FdoArray<FdoFunction*> > m_aggrFunctions;
};

#endif // FDOCOMMONQUERYAGGREGATOR_H


