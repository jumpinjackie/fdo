

struct sqlite3;

//mapping of FdoSpatialOperations to extension function names
static const char* g_spatial_op_map[] = 
{
"geom_contains",
"geom_crosses",
"geom_disjoint",
"geom_equals",
"geom_intersects",
"geom_overlaps",
"geom_touches",
"geom_within",
"geom_coveredby",
"geom_inside",
"geom_bbox"
};


void RegisterExtensions (sqlite3* db);
