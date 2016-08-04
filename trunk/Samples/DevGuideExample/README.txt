FDO DevGuideExample README.txt
Initial version written on January 13, 2005
Updated February 3, 2005
Update February 9, 2005
Update March 1, 2005

The ExerciseFdoApi program contains code used as example code in the chapters
of the "Autodesk FDO API Developer's Guide". This example code uses hard-coded
values as method arguments. The program also contains code driven by data
values contained in configuration files. The first data set submitted for
use by this program is the set contained in the Open GIS Consortium document
"Conformance Test Guidelines for OpenGIS Simple Features Specification for
SQL, Revision 1.0" (OpenGIS Project Document 98-046r1).

This README contains the following topics:
1. Principles of construction for the ExerciseFdoApi program
2. High-level view of operations implemented for this release
3. Usage notes
4. A more detailed description of program operation
5. Files in this distribution
6. Setting up the build environment
7. Command-line compile string
8. Setting up the runtime environment


PRINCIPLES OF CONSTRUCTION FOR THE ExerciseFdoApi PROGRAM

1. Maintain sample code with hard-coded values for illustrating API method
   calls in the chapters of the "Autodesk FDO API Developer's Guide". 
2. Maintain driver code that takes data sets from configuration files.
   The general intent is to simulate retrieving the values from a user
   as well as to enable the use of large data sets of varying complexity.
3. Make the program run no matter what provider it is connected to.
   Use the capabilities API to make this work.
4. Use well-known data sets from the public domain.
5. Operate on the data sets according to a defined set of use cases.


HIGH-LEVEL VIEW OF OPERATIONS IMPLEMENTED FOR THIS RELEASE

1. Connect to a datastore using either of the two supported providers
   (FDO Provider for Oracle or  FDO Provider for ArcSDE)
2. Execute the commands that use hard-code values. These commands are
   ApplySchema, DescribeSchema, Insert, Update, Delete, and DestroySchema.
3. Create the feature schema specified in the Open GIS Consortium document
   "Conformance Test Guidelines for OpenGIS Simple Features Specification
    for SQL, Revision 1.0", OpenGIS Project Document 98-046r1
   In the case of ArcSDE, the schema is created using ArcSDE tools because
   ArcSDE does not support the FdoIApplySchema command.
   ArcSDE schema creation is described in the FDO Provider for ArcSDE appendix
   in the "Autodesk FDO API Developer's Guide".
4. Populate the schema with the row values specified in 98-046r1.


USAGE NOTES

ExerciseFdoApi.exe takes 7 arguments. The usage text is:
"Usage: ExerciseFdoApi <provider>.data <logFileName>
			doSampleFeatureSchema=<T|F> doCapabilities=<T|F>
			doPartialSchema=<T|F> doXmlSchema=<T|F>
			undoXmlSchema=<T|F>
			doInsertDelete=<T|F>

The first argument is the name of a provider data file.
There are 2 provider data files:
1. OracleProvider.data
2. ArcSDEProvider.data
The data files contain configuration data, such as connection information,
which is provider-specific.

The second argument is the name of a log file. The program does extensive logging. You can enter any name.

The rest of the arguments are flags which govern the execution of blocks of code within the program. The program sets them to false. If you want to make any of them operational, set the value to 'T'.

The doSampleFeatureSchema flag governs the excecution of the code set out in the chapters of the "Autodesk FDO API Developer's Guide".

The doCapabilities flag governs the execution of the code which queries the capabilities of the current provider.

The doPartialSchema flag governs the execution of the code which creates a schema using FDO method calls.

The doXmlSchema flag governs the execution of code which creates a schema from an xml schema file.

The undoXmlSchema flag governs the destruction of the schema created from the xml schema file.

The doInsertDelete flag governs the insertion of data read from the OGC98046r1.insert.ArcSDE file by the ArcSDE provider or read from the OGC98046r1.insert file by the Oracle provider and the deletion of that same data as specified in the OGC98046r1.delete file. The reason for the provider-specific insert files is that ArcSDE supports only one geometry per feature class, and the buildings class in the OGC 98046r1 document has two geometries. Oracle permits the inclusion of two geometries in a feature class; one is the feature geometry, and the other is just another property.

FDO Provider for Oracle
Before you run the program using OracleProvider.data as the first argument, you must edit this file and provide values for Service, Username, and Password. The Service value must be an Oracle Net Service Name. This service name must appear in an entry in a tnsnames.ora file in the ...Network/Admin directory in your Oracle instance or client installation. Before running the program, you must connect to this service using the SysAdmin tool to create your Username and Password plus a DataStore called "ogc980461". The SysAdmin Create DataStore operation choice is number 1. Other than the DataStore name, you can accept the defaults. F1 Help is available for each operation.

FDO Provider for ArcSDE
Before you run the program using ArcSDEProvider.data as the first argument, you must edit this file and provide values for Server, Instance, Username, and Password. The server value is the name of the machine hosting the ArcSDE Server. The Instance property acts as an index into the services file. This file is located in C:\WINDOWS\System32\drivers\etc. Assuming that the instance name is "esri_sde", an entry would look something like this:
"esri_sde	5151/tcp	#ArcSDE Server Listening Port"

The Username and Password values must be valid in the data source to which the ArcSDE Server is connected. The username must have global insert, update, select, and delete privileges in the target data source. This is because the program performs these operations on the SAMPLEFEATURESCHEMA and OGC980461FS schemas, and these schema names have the status of user names in the data store. The FDO Provider for ArcSDE connects through an ArcSDE 9.0 client on the user's host to an ArcSDE 8.3 Server on a network host and the ArcSDE Server connects to the data source, for example, an Oracle database.

Installing the ArcSDE9.0 client components required for the operation of the FDP Provider for ArcSDE on your machine is an action distinct from installing the FDO SDK. This action is described in the FDO Provider for ArcSDE appendix in the "Autodesk FDO API Developer's Guide".

Since the FDO Provider for ArcSDE does not support the ApplySchema command, you must create the schema using the ArcSDE tools. This process is described in the FDO Provider for ArcSDE appendix in the "Autodesk FDO API Developer's Guide". Note in the program that schema, class, and property names are uppercase when the provider is ArcSDE. This is because the schema was created using the ArcSDE command-line tools operating on an Oracle database.


A MORE DETAILED DESCRIPTION OF PROGRAM OPERATION

The program does the following:
1. Gets the name of the provider it must connect to from the provider data file
2. Verifies that the provider named in the config file is registered
3. Logs the list of providers in the registry
4. Creates a connection object for the target provider
5. Gets the connection properties dictionary associated with the connection object. The dictionary  can be queried to determine what properties are currently required in order to make a connection. The program gets the values for the required properties from the provider data files.
6. Attempts to open a connection to the provider. The first attempt results in a pending state.
7.  Queries the dictionary, if the connection is in a pending state, to determine what properties are required in order to make a connection. The first connection attempt causes the datastore property to be identified as required. It also caused the datastore property to be populated with values. The program goes to the provider data file to get the value to choose and looks for this value in the list retrieved from the dictionary. Once the additional required properties are set, the program attempts to complete the connection.
8. Logs the provider capabilities, once the connection is complete.
9. Creates the commands to be used for the SampleFeatureSchema operations
10. if provider supports the DestroySchema command, destroy the SampleFeatureSchema
11. if provider supports the ApplySchema command, apply the SampleFeatureSchema and write it to an xml file
12. if provider supports the DescribeSchema command, describe the schema
13. Destroys the SampleFeatureSchema, if provider supports the DestroySchema command.
14. Reads the schema from the xml file and applies it, if provider supports the ApplySchema command.
15. Creates an instance of SampleFeatureClass class by inserting integer and string and polygon
16. Changes the value of the string in the SampleFeatureClass object
17. Queries the schema so it returns the SampleFeatureClass object created in
step 9
18. Deletes the SampleFeatureClass object
19. Destroys the SampleFeatureSchema, if provider supports the DestroySchema command.
20. Creates the commands which will be used in the OGC980461FS schema operations
21. Creates part of the OGC980461FS schema by reading name/value pairs from a file, if provider supports the ApplySchema command.
22. Writes the schema in GML format to an XML file
23. Describes the schema
24. if the provider supports the DesctroySchema command, destroy the schema
25. if the provider supports the ApplySchema command, create the schema by reading from the XML file created in step 22
26. describe the schema
27. Destroys the SampleFeatureSchema, if provider supports the DestroySchema command.
28. Creates the entire OGC980461FS schema by reading from an XML file that was created manually, if the provider supports the ApplySchema command.
29. Describes the schema
30. Inserts the data read from the OGC98046r1.insert or OGC98046r1.insert.ArcSDE file
31. Deletes the data specified in the filters in the OGC98046r1.delete file.
32. Destroys the SampleFeatureSchema, if provider supports the DestroySchema command.
33. Closes the connection, releases resources, and terminates.

FILES IN THIS DISTRIBUTION
The Developer's Guide sample code is part of the ExerciseFdoApi project,
which consists of the following files:
ArcSDEProvider.data - configuration data used for connection to FDO Provider for ArcSDE
conversions.cpp - wchar_t to char
conversions.h
Dialog.cpp - dialog simulation for sample code in the "Establishing a Connection" chapter in the "Autodesk FDO API Developer's Guide".
Dialog.h
ExerciseFdoApi.cpp - main
ExerciseFdoApi.vcproj - Visual Studio C++ project file
ExerciseFdoCapabilities.cpp - methods not directly supported by FDO
ExerciseFdoCapabilities.h
ExerciseFdoConnection.cpp - connection utilities
ExerciseFdoConnection.h
ExerciseFdoSchema.cpp - schema utilities
ExerciseFdoSchema.h
ExerciseFdoUtilities.cpp - currently one for printing exception info
ExerciseFdoUtilities.h
fdoinfo.cpp - log capability and schema info
fdoinfo.h
Logger.cpp - logging utility
Logger.h
Menu.cpp - simulate menu for sample code in the "Establishing a Connection" chapter in the "Autodesk FDO API Developer's Guide".
Menu.h
Ogc98046FeatureSchema.xsd - manually created OGC980461FS schema in GML format
OGC98046r1.delete - name/value pairs that define filters used for deleting feature objects
OGC98046r1.insert - name/value pairs that define insertion data for Oracle provider
OGC98046r1.insert.ArcSDE - name/value pairs that define insertion data for ArcSDE provider
OGC98046r1.schema - name/value pairs that define schema in OGC doc 98-046r1
OracleProvider.data - configuration data used for connection to Oracle provider
ParseSchemaDefinitionFile.cpp - derived class for parsing files containing key/value data defining schemas
ParseSchemaDefinitionFile.h
ParseSchemaFilterFile.cpp - derived class for parsing files containing key/value data defining filters
ParseSchemaFilterFile.h
ParseSchemaInsertionFile.cpp - derived class for parsing files containing key/value data defining insertion data
ParseSchemaInsertionFile.h
ParseSchemaOperationFile.cpp - base class for parsing files containing key/value data defining schemas, insertion data, and delete filter definitions
ParseSchemaOperationFile.h
ProviderConfigInfo.cpp - used for parsing <provider>.data file
ProviderConfigInfo.h
README.txt - the file that you are reading
sdetoolDevGuideExample.bat - batch file for creating SAMPLEFEATURESCHEMA and OGC980461FS schema in ArcSDE
stdafx.cpp - machinery for created predefined headers
stdafx.h
strToFdoType.cpp - convert strings to enumerated type values
strToFdoType.h

SETTING UP THE BUILD ENVIRONMENT

Set up the build environment as instructed in the FDO API Build Environment section in the "Before You Begin" chapter in the "Autodesk FDO API Developer's Guide".

SETTING UP THE RUNTIME ENVIRONMENT

Set up the runtime environment as instructed in the FDO API Runtime Environment section in the "Before You Begin" chapter in the "Autodesk FDO API Developer's Guide".
				-end-
