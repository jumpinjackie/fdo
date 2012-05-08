# Here we just call Providers subdir based on availability
# using optional macros
# If you need add a new provider, just add dirname on the 
# PROVIDERS list below

set( PROVIDERS 
	#ArcSDE
	GenericRdbms
	#KingKML
	#KingOracle
	#PostGIS
	SHP
	WMS
	GDAL
	#KingInformix
	#KingMsSqlSpatial
	OGR
	SDF
	SQLite
	WFS
	)

foreach( PRV ${PROVIDERS} )
	string(TOUPPER ${PRV} UP_PRV) 
	string(TOLOWER ${PRV} DO_PRV) 
	if( WITH_${PRV} OR WITH_${UP_PRV} OR WITH_${DO_PRV} OR ALL_PROVIDERS )
		macro_optional_add_subdirectory( ${PRV} )
		message( STATUS "${PRV} provider is enabled" )
	endif( WITH_${PRV} OR WITH_${UP_PRV} OR WITH_${DO_PRV} OR ALL_PROVIDERS )
endforeach( PRV ${PROVIDERS} )
