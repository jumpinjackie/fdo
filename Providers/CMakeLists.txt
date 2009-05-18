# Here we just call Providers subdir based on availability
# using optional macros

if( WITH_PROVIDERS )
	foreach( provider ${WITH_PROVIDERS} )
		macro_optional_add_subdirectory( ${provider} )
	endforeach( provider ${WITH_PROVIDERS} )
endif( WITH_PROVIDERS )
