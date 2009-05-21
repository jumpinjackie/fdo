/*
* Copyright (C) 2006  SL-King d.o.o
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
*/
#include "StdAfx.h"
#include "c_KgKmlApplySchema.h"
#include "c_FdoKml_API.h"


c_KgKmlApplySchema::c_KgKmlApplySchema(c_KgKmlConnection *Conn)
  : c_KgKmlFdoCommand(Conn)
{
  m_IngoreStates = true;
}

c_KgKmlApplySchema::~c_KgKmlApplySchema(void)
{
}


/// \brief
/// Gets the name of the schema to create.
/// 
/// \return
/// Returns the name of the schema to create
/// 
FdoFeatureSchema* c_KgKmlApplySchema::GetFeatureSchema()
{
  return FDO_SAFE_ADDREF(m_FeatureSchema.p);
}

/// \brief
/// Sets the name of the schema to create.
/// 
/// \param value 
/// Input the name of the schema to create
/// 
/// \return
/// Returns nothing
/// 
void c_KgKmlApplySchema::SetFeatureSchema(FdoFeatureSchema* Schema)
{
  m_FeatureSchema = Schema;
  FDO_SAFE_ADDREF(m_FeatureSchema.p);
}

/// \brief
///  Gets the FdoPhysicalSchemaMapping used to specify how the schema definition
/// gets mapped into physical storage.
/// 
/// \return
/// Returns FdoPhysicalSchemaMapping
/// 
FdoPhysicalSchemaMapping* c_KgKmlApplySchema::GetPhysicalMapping()
{
  return FDO_SAFE_ADDREF(m_PhysMapping.p);
}

/// \brief
/// Sets the FdoPhysicalSchemaMapping used to specify how the schema definition
/// gets mapped into physical storage.
/// 
/// \param value 
/// Input the FdoPhysicalSchemaMapping
/// 
/// \return
/// Returns nothing
/// 
void c_KgKmlApplySchema::SetPhysicalMapping(FdoPhysicalSchemaMapping* PhysMap)
{
  m_PhysMapping = PhysMap;
  FDO_SAFE_ADDREF(m_PhysMapping.p);
}

/// \brief
///  Indicates whether Execute() will ignore element states 
/// when applying the feature schema.
/// 
/// \return
/// Returns true if elements states will be ignored, false otherwise. 
/// 
///  
FdoBoolean c_KgKmlApplySchema::GetIgnoreStates()
{
  return m_IngoreStates;
}

/// \brief
/// Changes the handling of element states by Execute().
/// 
/// \param ignoreStates 
/// Input True: Execute() will ignore the 
/// element states on the Feature Schema elements. Instead, it 
/// will additively merge the Feature Schema into the current DataStore.
/// False: Execute() will use the element states to determine whether
/// each element is inserted, modified or deleted.
/// 
/// \return
/// Returns nothing
/// 
void c_KgKmlApplySchema::SetIgnoreStates( FdoBoolean IgnoreStates )
{
  m_IngoreStates = IgnoreStates;
}



/// \brief
/// Executes the ApplySchema command that creates metadata and physical
/// storage for the schema. An exception is thrown if the schema already
/// exists or if a schema already exists and the feature provider only
/// supports a single schema. If schema changes include deletion of classes
/// or the schema itself, then those classes (or the schema, if it is marked
/// as deleted) must not contain any instance data. If they do, an exception
/// will be thrown.
/// Implementors must call FdoFeatureSchema::AcceptChanges() when Execute() succeeds.
/// It must be called after the schema has been successfully applied, in
/// order to synchronize the states of the schema elements.
/// 
/// \return
/// Returns nothing
/// 
void c_KgKmlApplySchema::Execute()
{
  if( !m_IngoreStates ) 
    throw FdoException::Create(L"c_KgKmlApplySchema::Execute Supports only IgnoreStates='true'");
    
  if( !m_FeatureSchema.p ) return;
 
// Check if class exist in schema
  FdoPtr<FdoClassCollection> class_coll = m_FeatureSchema->GetClasses();
  if( class_coll->GetCount() > 0)
  {
    FdoPtr<FdoClassDefinition> class_def = class_coll->GetItem(0);
    
    FdoPtr<FdoClassDefinition> newclass = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(class_def);
    
    FdoPtr<c_KgKmlFdoData> kmlfdodata = m_Connection->GetKmlFdoData();
    kmlfdodata->SetFdoSchema(m_FeatureSchema);
  }
  
  
    
}//endof c_KgKmlApplySchema::Execute

