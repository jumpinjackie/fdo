#ifndef FDORDBMSLPJOINTREENODE_H
#define FDORDBMSLPJOINTREENODE_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include "Table.h"
#include "JoinTreeNodeCollection.h"

// A node in a join tree. A join tree shows all tables that need to be included
// in a select in order to select a given set of properties. 
// Join trees are build by FdoRdbmsLpClassDefinition::GetJoinTree().

class FdoRdbmsLpJoinTreeNode :
	public FdoRdbmsSmSchemaElement
{
public:

    /// Create a new join tree node from a table.
	//
    /// Parameters:
    /// 	pLpTable: the table. 
	FdoRdbmsLpJoinTreeNode(const FdoRdbmsLpTable* pLpTable);

    /// Get the table for this join tree node.
	const FdoRdbmsLpTable* GetTable(void) const;

    /// Get the source (child) join tree nodes. These represent the 
    /// next tables to join in on the way to reaching all given properties.
	FdoRdbmsLpJoinTreeNodeCollection* GetSourceNodes(void);

    /// Gets the list of properties not in the join tree.
    /// This list is only set for the root node.
    /// See FdoRdbmsLpClassDefinition::GetJoinTree() for more details.
	FdoStringsP GetMissingProperties();

    /// Sets the table for this join tree node.
	void SetTable(const FdoRdbmsLpTable* pLpTable);

protected:
    /// Refcounted classes can't be directly destroyed.
	~FdoRdbmsLpJoinTreeNode(void);

private:
	const FdoRdbmsLpTable* mpTable;
	FdoRdbmsLpJoinTreeNodeCollection mSourceNodes;
	FdoStringsP mMissingProperties;
};

#endif


