#ifndef FDORDBMSSQLBUILDER_H
#define FDORDBMSSQLBUILDER_H 1
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <Fdo/Schema/ClassDefinition.h>
#include <Fdo/Expression/JoinCriteriaCollection.h>
#include <vector>

// Class used only to detect if the property values collection has changed between 
// two or more consecutive inserts. No other special handling is done here
class FdoRdbmsPropertyValueCollection : public FdoPropertyValueCollection
{
private:
    bool m_collChanged;
protected:
    FdoRdbmsPropertyValueCollection()
    {
        m_collChanged = false;
    }
    virtual ~FdoRdbmsPropertyValueCollection()
    {
    }

    virtual void Dispose()
    {
        delete this;
    }
public:
    static FdoRdbmsPropertyValueCollection* Create()
    {
        return new FdoRdbmsPropertyValueCollection();
    }
    virtual void SetItem(FdoInt32 index, FdoPropertyValue* value)
    {
        m_collChanged = true;
        FdoCollection<FdoPropertyValue, FdoCommandException>::SetItem(index, value);
    }
    virtual FdoInt32 Add(FdoPropertyValue* value)
    {
        m_collChanged = true;
        return FdoCollection<FdoPropertyValue, FdoCommandException>::Add(value);
    }
    virtual void Insert(FdoInt32 index, FdoPropertyValue* value)
    {
        m_collChanged = true;
        FdoCollection<FdoPropertyValue, FdoCommandException>::Insert(index, value);
    }
    virtual void Clear()
    {
        m_collChanged = true;
        FdoCollection<FdoPropertyValue, FdoCommandException>::Clear();
    }
    virtual void Remove(const FdoPropertyValue* value)
    {
        m_collChanged = true;
        FdoCollection<FdoPropertyValue, FdoCommandException>::Remove(value);
    }
    virtual void RemoveAt(FdoInt32 index)
    {
        m_collChanged = true;
        FdoCollection<FdoPropertyValue, FdoCommandException>::RemoveAt(index);
    }
    bool GetCollectionChanged()
    {
        return m_collChanged;
    }
    void SetCollectionChanged(bool value)
    {
        m_collChanged = value;
    }
};

class FdoRdbmsFdoParameterValueCollection : public FdoParameterValueCollection
{
private:
    bool m_collChanged;
protected:
    FdoRdbmsFdoParameterValueCollection()
    {
        m_collChanged = false;
    }
    virtual ~FdoRdbmsFdoParameterValueCollection()
    {
    }

    virtual void Dispose()
    {
        delete this;
    }
public:
    static FdoRdbmsFdoParameterValueCollection* Create()
    {
        return new FdoRdbmsFdoParameterValueCollection();
    }
    virtual void SetItem(FdoInt32 index, FdoParameterValue* value)
    {
        m_collChanged = true;
        FdoCollection<FdoParameterValue, FdoCommandException>::SetItem(index, value);
    }
    virtual FdoInt32 Add(FdoParameterValue* value)
    {
        m_collChanged = true;
        return FdoCollection<FdoParameterValue, FdoCommandException>::Add(value);
    }
    virtual void Insert(FdoInt32 index, FdoParameterValue* value)
    {
        m_collChanged = true;
        FdoCollection<FdoParameterValue, FdoCommandException>::Insert(index, value);
    }
    virtual void Clear()
    {
        m_collChanged = true;
        FdoCollection<FdoParameterValue, FdoCommandException>::Clear();
    }
    virtual void Remove(const FdoParameterValue* value)
    {
        m_collChanged = true;
        FdoCollection<FdoParameterValue, FdoCommandException>::Remove(value);
    }
    virtual void RemoveAt(FdoInt32 index)
    {
        m_collChanged = true;
        FdoCollection<FdoParameterValue, FdoCommandException>::RemoveAt(index);
    }
    bool GetCollectionChanged()
    {
        return m_collChanged;
    }
    void SetCollectionChanged(bool value)
    {
        m_collChanged = value;
    }
};

struct NameOrderingPair
{
    NameOrderingPair(FdoIdentifier* nm, FdoOrderingOption opt)
        : name(nm), option(opt)
    {}

    FdoIdentifier* name;
    FdoOrderingOption option;
};

class FdoRdbmsSqlBuilder : public virtual FdoIExpressionProcessor, public virtual FdoIFilterProcessor
{
protected:
    FdoRdbmsConnection* m_fdoConn;
public:
    FdoRdbmsSqlBuilder(FdoRdbmsConnection* conn)
    {
        m_fdoConn = conn; // no add ref!
    }

    // in case it contains unsupported functions this method will return NULL
    virtual FdoString* ToSelectSqlString(FdoIdentifier* mainClass, FdoIdentifier* alias, FdoFilter* filter, FdoIdentifierCollection* selectList, 
        const std::vector<NameOrderingPair>& ordering, FdoJoinCriteriaCollection* joinCriteria) = 0;

    // in case it contains unsupported functions this method will return NULL
    virtual FdoString* ToUpdateFilterSqlString(FdoIdentifier* mainClass, FdoFilter* filter) = 0;

    virtual FdoString* ToAggregateSelectSqlString(FdoIdentifier* mainClass, FdoIdentifier* alias, FdoFilter* filter, FdoIdentifierCollection* selectList,
                                                bool bDistinct, FdoOrderingOption eOrderingOption, FdoIdentifierCollection* ordering, 
                                                FdoFilter* grFilter, FdoIdentifierCollection* grouping, FdoParameterValueCollection* parmValues, 
                                                FdoJoinCriteriaCollection* joinCriteria) = 0;
    virtual void SetParameterValues (FdoParameterValueCollection* params) = 0;
    virtual std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* GetUsedParameterValues() = 0;
};

#endif // FDORDBMSSQLBUILDER_H
