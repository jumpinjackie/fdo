/*
* Copyright (C) 2007  Haris Kurtagic
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


#ifndef _c_KgMssApplySchema_h
#define _c_KgMssApplySchema_h

class c_KgMssApplySchema : public c_KgMssFdoCommand<FdoIApplySchema>
{
public:
  c_KgMssApplySchema(c_KgMssConnection *Conn);
public:
  ~c_KgMssApplySchema(void);
  
protected:  
    FdoPtr<FdoFeatureSchema>  m_FeatureSchema;    
    FdoPtr<FdoPhysicalSchemaMapping> m_PhysMapping;
    FdoBoolean m_IngoreStates;
    
public:  
  
    /// \brief
    /// Gets the name of the schema to create.
    /// 
    /// \return
    /// Returns the name of the schema to create
    /// 
    FDOKGMSS_API virtual FdoFeatureSchema* GetFeatureSchema();

    /// \brief
    /// Sets the name of the schema to create.
    /// 
    /// \param value 
    /// Input the name of the schema to create
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGMSS_API virtual void SetFeatureSchema(FdoFeatureSchema* value);

    /// \brief
    ///  Gets the FdoPhysicalSchemaMapping used to specify how the schema definition
    /// gets mapped into physical storage.
    /// 
    /// \return
    /// Returns FdoPhysicalSchemaMapping
    /// 
    FDOKGMSS_API virtual FdoPhysicalSchemaMapping* GetPhysicalMapping();

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
    FDOKGMSS_API virtual void SetPhysicalMapping(FdoPhysicalSchemaMapping* value);

    /// \brief
    ///  Indicates whether Execute() will ignore element states 
    /// when applying the feature schema.
    /// 
    /// \return
    /// Returns true if elements states will be ignored, false otherwise. 
    /// 
    ///  
    FDOKGMSS_API virtual FdoBoolean GetIgnoreStates();

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
    FDOKGMSS_API virtual void SetIgnoreStates( FdoBoolean ignoreStates );



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
    FDOKGMSS_API virtual void Execute();
    
    
    // Helper functions
protected:
  void InsertSdoGeomMetadata(FdoString* TableName,FdoGeometricPropertyDefinition* GeomProp);
  void CreateIndex(FdoString* TableName,FdoGeometricPropertyDefinition* GeomProp);
  void CreatePrimaryKey(FdoString* TableName,FdoDataPropertyDefinitionCollection* IdentPropColl);
  void CreateTableSequence(FdoString* TableName);
};

#endif