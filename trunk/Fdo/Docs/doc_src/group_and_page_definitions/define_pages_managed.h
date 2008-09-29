/*! \mainpage Introduction
<div class="mainpage">
<!-- This is the index.htm page that the user first sees. -->
<p>The Feature Data Objects (FDO) application 
programming interface (API) is a set of APIs used to 
manipulate, define, and analyze geospatial information.
</p>
<p>FDO supports the retrieving and updating of spatial and non-spatial 
GIS feature data through a rich classification model that is 
based on OpenGIS and ISO standards.
</p>
<p>From the perspective of a client application user: the FDO API 
provides consistent access to feature data, 
whether it comes from a CAD-based data source, or from a 
relational data store that supports rich classification. To 
achieve this, FDO supports a model that can readily follow 
the capabilities of each data source, allowing for consumer 
applications functionality that is tailored to match that of 
the data source. For example, some data sources may support 
spatial queries, while others do not. Also, a flexible 
metadata model is required in FDO, allowing clients to adapt 
to the underlying feature schema exposed by each data source.
</p>
<p>From the perspective of a client application engineer: the 
FDO API provides a common, general purpose abstraction layer 
for accessing geospatial data from a variety of data sources. 
The API is, in part, an interface specification of the 
abstraction layer. A provider, such as FDO Provider for SDF, 
is an implementation of the interface for a specific type of 
data source, which in this example is an implementation for 
OSGeo's spatial database format (SDF). The API can support 
the standard data store manipulation operations, such as 
querying, updating, versioning, locking, and others, only 
when the underlying data store has the same capabilities 
for these operations.
</p>
<h2>
  Architecture
</h2>
<p>
The high-level architecture of FDO comprises feature providers 
that plug into an application to expose a consistent feature 
schema and a consistent interface for interacting with geospatial 
feature data, regardless of the underlying data store. The 
client applications interact through the various FDO interfaces, 
such as IConnection or ICommand, using an FDO interface layer, 
or feature access manager.
</p>
<p>
FDO is assembled in or consists of conceptual packages of similar 
functionality. This packaging is reflected in the substructure of 
the FDO SDK 'includes' folder. FDO commands, provider-specific 
commands, and connections/capabilities provide access to native 
data stores through each specific FDO provider. Schema management 
(using XML), client services, and filters/expressions are all 
provider-independent packages that connect directly to the FDO API.
</p>
<p>
For more information, see <i>The Essential FDO</i> (FET_TheEssentialFDO.pdf) 
and the <i>FDO Developer's Guide</i> (FDG_FDODevGuide.pdf).
</p>
\image html FDO_architecture.png
<br>
</div>
*/
/// \defgroup OSGeoFDO OSGeo::FDO
/// \defgroup OSGeoFDOClientServices OSGeo::FDO::ClientServices
/// \defgroup OSGeoFDOCommands OSGeo::FDO::Commands
/// \defgroup OSGeoFDOCommandsFeature OSGeo::FDO::Commands::Feature
/// \defgroup OSGeoFDOCommandsDataStore OSGeo::FDO::Commands::DataStore
/// \defgroup OSGeoFDOCommandsLocking OSGeo::FDO::Commands::Locking
/// \defgroup OSGeoFDOCommandsLongTransaction OSGeo::FDO::Commands::LongTransaction
/// \defgroup OSGeoFDOCommandsSchema OSGeo::FDO::Commands::Schema
/// \defgroup OSGeoFDOCommandsSpatialContext OSGeo::FDO::Commands::SpatialContext
/// \defgroup OSGeoFDOCommandsSQL OSGeo::FDO::Commands::SQL
/// \defgroup OSGeoFDOCommandsUnitOfMeasure OSGeo::FDO::Commands::UnitOfMeasure
/// \defgroup OSGeoFDOConnections OSGeo::FDO::Connections
/// \defgroup OSGeoFDOConnectionsCapabilities OSGeo::FDO::Connections::Capabilities
/// \defgroup OSGeoFDOExpression OSGeo::FDO::Expression
/// \defgroup OSGeoFDOFilter OSGeo::FDO::Filter
/// \defgroup OSGeoFDORaster OSGeo::FDO::Raster
/// \defgroup OSGeoFDOSchema OSGeo::FDO::Schema
/// \defgroup OSGeoFDOXml OSGeo::FDO::Xml
/// \defgroup OSGeoFDOCommon OSGeo::FDO::Common
/// \defgroup OSGeoFDOCommonGML212 OSGeo::FDO::Common::GML212
/// \defgroup OSGeoFDOCommonIo OSGeo::FDO::Common::Io
/// \defgroup OSGeoFDOCommonXml OSGeo::FDO::Common::Xml
/// \defgroup OSGeoFDOCommonXsl OSGeo::FDO::Common::Xsl
/// \defgroup OSGeoFDOGeometry OSGeo::FDO::Geometry
/// \defgroup OSGeoFDORuntime OSGeo::FDO::Runtime
/// \defgroup OSGeoFDOSpatial OSGeo::FDO::Spatial
