#ifndef OGR_FILTER_IDENTIFIER_EXTRACTOR_H
#define OGR_FILTER_IDENTIFIER_EXTRACTOR_H

#include "Fdo.h"

class OgrFilterIdentifierExtractor : public FdoIFilterProcessor,
                                     public FdoIExpressionProcessor  
{
public:
    OgrFilterIdentifierExtractor();
    ~OgrFilterIdentifierExtractor();

    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    virtual void ProcessInCondition(FdoInCondition& filter);

    virtual void ProcessNullCondition(FdoNullCondition& filter);

    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);

    virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

    virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    virtual void ProcessFunction(FdoFunction& expr);

    virtual void ProcessIdentifier(FdoIdentifier& expr);

    virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

    virtual void ProcessParameter(FdoParameter& expr);

    virtual void ProcessBooleanValue(FdoBooleanValue& expr);

    virtual void ProcessByteValue(FdoByteValue& expr);

    virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    virtual void ProcessDecimalValue(FdoDecimalValue& expr);

    virtual void ProcessDoubleValue(FdoDoubleValue& expr);

    virtual void ProcessInt16Value(FdoInt16Value& expr);

    virtual void ProcessInt32Value(FdoInt32Value& expr);

    virtual void ProcessInt64Value(FdoInt64Value& expr);

    virtual void ProcessSingleValue(FdoSingleValue& expr);

    virtual void ProcessStringValue(FdoStringValue& expr);

    virtual void ProcessBLOBValue(FdoBLOBValue& expr);

    virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    virtual void ProcessGeometryValue(FdoGeometryValue& expr);

    FdoIdentifierCollection* GetIdentifiers();

protected:
    virtual void Dispose() { delete this; }

private:
    void AddIdentifier(FdoIdentifier* ident);

    FdoPtr<FdoIdentifierCollection> m_identifiers;
};

#endif //OGR_FILTER_IDENTIFIER_EXTRACTOR_H