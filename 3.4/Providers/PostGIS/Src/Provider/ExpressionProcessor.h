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
#ifndef FDOPOSTGIS_EXPRESSIONPROCESSOR_H_INCLUDED
#define FDOPOSTGIS_EXPRESSIONPROCESSOR_H_INCLUDED

#include <string>
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

/// Definition of processor for expression tree nodes.
/// The processor translates FdoExpression hierarchy to the SQL equivalent sytanx
/// supported by PostgreSQL DBMS and PostGIS engine.
///
class ExpressionProcessor : public FdoIExpressionProcessor
{
public:

    /// Type of FDO smart pointer for Connection class.
    typedef FdoPtr<ExpressionProcessor> Ptr;
    
    /// Constructor.
    //ExpressionProcessor(std::wstring& buffer);
    ExpressionProcessor();
    
    //
    // FdoIExpressionProcessor
    //
    
 	/// Process the FdoBinaryExpression passed in as an argument.
 	/// \param
 	/// expre [in] -
 	///
    void ProcessBinaryExpression(FdoBinaryExpression& expr);
    
 	/// Process the FdoUnaryExpression passed in as an argument.
 	/// \param
 	/// expre [in] -
 	///
    void ProcessUnaryExpression(FdoUnaryExpression& expr);

 	/// Process node of function type passed in as an argument.
    void ProcessFunction(FdoFunction& expr);

    /// Process node of identifier type passed in as an argument.
    void ProcessIdentifier(FdoIdentifier& expr);
     	
    /// Process node of type of computed identifier passed in as an argument.
    void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

    /// Process node of type of parameter passed in as an argument.
    void ProcessParameter(FdoParameter& expr);

    /// Process node of boolean data type passed in as an argument.
    void ProcessBooleanValue(FdoBooleanValue& expr);

    /// Process node of byte data type passed in as an argument.
    void ProcessByteValue(FdoByteValue& expr);

    /// Process node of data and time data type passed in as an argument.
    void ProcessDateTimeValue(FdoDateTimeValue& expr);

    /// Process node of numeric/decimal data type passed in as an argument.
    void ProcessDecimalValue(FdoDecimalValue& expr);

    /// Process node of double data type passed in as an argument.
    void ProcessDoubleValue(FdoDoubleValue& expr);

    /// Process 2-bytes long integral data type passed in as an argument.
    void ProcessInt16Value(FdoInt16Value& expr);

    /// Process 4-bytes long integral data type passed in as an argument.
    void ProcessInt32Value(FdoInt32Value& expr);

    /// Process 8-bytes long integral data type passed in as an argument.
    void ProcessInt64Value(FdoInt64Value& expr);

    /// Process single-precision data type passed in as an argument.
    void ProcessSingleValue(FdoSingleValue& expr);

    /// Process node of string data type passed in as an argument.
    void ProcessStringValue(FdoStringValue& expr);

    /// Process node of BLOB data type passed in as an argument.
    void ProcessBLOBValue(FdoBLOBValue& expr);

    /// Process node of CLOB data type passed in as an argument.
    void ProcessCLOBValue(FdoCLOBValue& expr);

    /// Process node of geometry data type passed in as an argument. 
    void ProcessGeometryValue(FdoGeometryValue& expr);

    //
    // ExpressionProcessor interface
    //

    /// Get textual representation of parsed expression.
    /// The function releases currently stored expression text,
    /// and leaves internal buffer empty, and ready for next expression to process.
    /// That's also the reason the function is non-const.
    ///
    /// \param
    /// exprText [out] - buffer that will be filled with expression text.
    ///
    void ReleaseExpressionText(std::string& exprText);

    std::string ReleaseBuffer();

protected:
    
    /// Destructor.
    virtual ~ExpressionProcessor();

    //
    // FdoIDisposable interface
    //
    
    void Dispose();
    
private:

    //
    // Private data members
    //

    // Textual representation of the expresion.
    std::string mBuffer;

    //
    // Private operations
    //

    template <typename T>
    std::string GetValueAsString(T const& value);

};


///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

template <typename T>
std::string ExpressionProcessor::GetValueAsString(T const& value)
{
    try
    {
        std::string str(boost::lexical_cast<std::string>(value));
        return str;
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE(L"ExpressionProcessor can not fetch expression value");
        FDOLOG_WRITE("ERROR: %s", e.what());

        throw FdoExpressionException::Create(
            L"Conversion of expression value to string failed.");
    }
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_EXPRESSIONPROCESSOR_H_INCLUDED
