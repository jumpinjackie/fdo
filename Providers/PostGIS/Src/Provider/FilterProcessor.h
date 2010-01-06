//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_FILTERPROCESSOR_H_INCLUDED
#define FDOPOSTGIS_FILTERPROCESSOR_H_INCLUDED

#include "ExpressionProcessor.h"
#include <string>

namespace fdo { namespace postgis {

/// Definition of a filter tree processor.
/// It declares a process operation for each concrete class in the filter hierarchy.
///
class FilterProcessor : public FdoIFilterProcessor
{
public:

    /// Type of FDO smart pointer for Connection class.
    typedef FdoPtr<FilterProcessor> Ptr;
    
    /// Constructor.
    FilterProcessor();

    FilterProcessor(int srid);
    
    //
    // FdoIFilterProcessor
    //
    
 	/// Process the FdoBinaryLogicalOperator passed in as an argument.
 	/// \param
 	/// op [in] -
 	///
    void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& op);
    
 	/// Process the FdoUnaryLogicalOperator passed in as an argument.
 	/// \param
 	/// op [in] -
 	///
    void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& op);

 	/// Process the FdoComparisonCondition passed in as an argument.
 	/// \param
 	/// cond [in] -
 	///
    void ProcessComparisonCondition(FdoComparisonCondition& cond);
 	
 	/// Processe the FdoInCondition passed in as an argument.
 	/// \param
 	/// cond [in] -
 	///
    void ProcessInCondition(FdoInCondition& cond);
 	
 	/// Process the FdoNullCondition passed in as an argument.
 	/// \param
 	/// cond [in] -
 	///
    void ProcessNullCondition(FdoNullCondition& cond);
 	
 	/// Process the FdoSpatialCondition passed in as an argument.
 	/// \param
 	/// cond [in] -
 	///
    void ProcessSpatialCondition(FdoSpatialCondition& cond);
 	
 	/// Process the FdoDistanceCondition passed in as an argument. 
 	/// \param
 	/// cond [in] -
 	///
    void ProcessDistanceCondition(FdoDistanceCondition& cond);
    
    //
    // FilterProcessor interface
    //

    std::string const& GetFilterStatement() const;

protected:
    
    /// Destructor.
    virtual ~FilterProcessor();

    //
    // FdoIDisposable interface
    //
    
    void Dispose();
    
private:

    //
    // Private data members
    //

    ExpressionProcessor::Ptr mExprProc;

    // Used as argument of GeomFromWKB, during construction of geometries.
    std::string mSRID;

    // Internal buffer of filtering statement.
    std::string mStatement;

    //
    // Private operations
    //

    // NOTE: Due to the fact the const-correctness is broken,
    // it's not possible to use const references for input
    // parameters of validation functions.

    void ValidateBinaryOperator(FdoBinaryLogicalOperator& op);
    void ValidateUnaryOperator(FdoUnaryLogicalOperator& op);
    void ValidateComparisonCondition(FdoComparisonCondition& cond);
  
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_FILTERPROCESSOR_H_INCLUDED
