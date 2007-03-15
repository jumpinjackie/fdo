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
#include "stdafx.h"
#include "PostGisProvider.h"
#include "FilterProcessor.h"

#include <cassert>

namespace fdo { namespace postgis {

FilterProcessor::FilterProcessor()
{
}

FilterProcessor::~FilterProcessor()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDelete interface
///////////////////////////////////////////////////////////////////////////////

void FilterProcessor::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIFilerProcessor interface
///////////////////////////////////////////////////////////////////////////////

void FilterProcessor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& op)
{
}

void FilterProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& op)
{
}

void FilterProcessor::ProcessComparisonCondition(FdoComparisonCondition& op)
{
}

void FilterProcessor::ProcessInCondition(FdoInCondition& cond)
{
}

void FilterProcessor::ProcessNullCondition(FdoNullCondition& cond)
{
}

void FilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& cond)
{
}

void FilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& cond)
{
}

}} // namespace fdo::postgis
