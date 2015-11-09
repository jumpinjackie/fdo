#include "stdafx.h"
#include "OgrFilterIdentifierExtractor.h"

OgrFilterIdentifierExtractor::OgrFilterIdentifierExtractor()
{
    m_identifiers = FdoIdentifierCollection::Create();
}

OgrFilterIdentifierExtractor::~OgrFilterIdentifierExtractor()
{
    m_identifiers = NULL;
}

void OgrFilterIdentifierExtractor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();
    left->Process(this);
    right->Process(this);
}

void OgrFilterIdentifierExtractor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> op = filter.GetOperand();
    op->Process(this);
}

void OgrFilterIdentifierExtractor::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();
    left->Process(this);
    right->Process(this);
}

void OgrFilterIdentifierExtractor::ProcessInCondition(FdoInCondition& filter)
{
    FdoPtr<FdoIdentifier> ident = filter.GetPropertyName();   
    AddIdentifier(ident);
}

void OgrFilterIdentifierExtractor::ProcessNullCondition(FdoNullCondition& filter)
{
    FdoPtr<FdoIdentifier> ident = filter.GetPropertyName();
    AddIdentifier(ident);
}

void OgrFilterIdentifierExtractor::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    FdoPtr<FdoIdentifier> ident = filter.GetPropertyName();
    AddIdentifier(ident);
}

void OgrFilterIdentifierExtractor::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
    FdoPtr<FdoIdentifier> ident = filter.GetPropertyName();
    AddIdentifier(ident);
}

void OgrFilterIdentifierExtractor::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    FdoPtr<FdoExpression> left = expr.GetLeftExpression();
    FdoPtr<FdoExpression> right = expr.GetRightExpression();
    left->Process(this);
    right->Process(this);
}

void OgrFilterIdentifierExtractor::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    FdoPtr<FdoExpression> uex = expr.GetExpression();
    uex->Process(this);
}

void OgrFilterIdentifierExtractor::ProcessFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> args = expr.GetArguments();
    for (FdoInt32 i = 0; i < args->GetCount(); i++)
    {
        FdoPtr<FdoExpression> arg = args->GetItem(i);
        arg->Process(this);
    }
}

void OgrFilterIdentifierExtractor::ProcessIdentifier(FdoIdentifier& expr)
{
    std::wstring name;
    name += expr.GetName();
    FdoPtr<FdoIdentifier> ident = FdoIdentifier::Create(name.c_str());
    AddIdentifier(ident);
}

void OgrFilterIdentifierExtractor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    FdoPtr<FdoExpression> ex = expr.GetExpression();
    ex->Process(this);
}

void OgrFilterIdentifierExtractor::ProcessParameter(FdoParameter& expr)
{
    
}

void OgrFilterIdentifierExtractor::ProcessBooleanValue(FdoBooleanValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessByteValue(FdoByteValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessDecimalValue(FdoDecimalValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessDoubleValue(FdoDoubleValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessInt16Value(FdoInt16Value& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessInt32Value(FdoInt32Value& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessInt64Value(FdoInt64Value& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessSingleValue(FdoSingleValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessStringValue(FdoStringValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessBLOBValue(FdoBLOBValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessCLOBValue(FdoCLOBValue& expr)
{

}

void OgrFilterIdentifierExtractor::ProcessGeometryValue(FdoGeometryValue& expr)
{

}

void OgrFilterIdentifierExtractor::AddIdentifier(FdoIdentifier* ident)
{
    if (NULL != ident)
    {
        FdoString* name = ident->GetName();
        if (m_identifiers->IndexOf(name) < 0)
        {
            m_identifiers->Add(ident);
        }
    }
}

FdoIdentifierCollection* OgrFilterIdentifierExtractor::GetIdentifiers()
{
    return FDO_SAFE_ADDREF(m_identifiers.p);
}
