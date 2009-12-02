// 
//  
//  Copyright (C) 2008 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  


#include "stdafx.h"
#include "slt.h"
#include "si_api.h"
#include "SltConversionUtils.h"
#include "SltGeomUtils.h"
#include "SpatialOptimizer.h"
#include "FdoCommonSchemaUtil.h"

typedef FdoIConnection* (*createFunc)();

FdoIConnection* CreateConnection(const wchar_t* provider)
{
#ifdef _WIN32
    HMODULE h = LoadLibrary(provider);
    if (!h) goto fail_load;

    FARPROC f = GetProcAddress(h, "CreateConnection");
    if (!f) goto fail_entry;

    return ((createFunc)f)();
#else
    char mbsprov[PATH_MAX];
    wcstombs(mbsprov, provider);
    
    void* h = dlopen(mbsprov, RTLD_LAZY);
    if (!h) goto fail_load;
    
    void* f = dlsym(h, "CreateConnection");
    if (!f) goto fail_entry;    

    return ((createFunc)f)();
#endif

fail_load:
    printf("Failed to load provider: %ls\n", provider);
    exit(1);

fail_entry:
    printf("Failed to find FDO entry point: %ls\n", provider);
    exit(1);
}

FdoIConnection* GetFdoCon(const wchar_t* srcfile, bool open)
{
    std::wstring connstr;

    _ASSERT(wcslen(srcfile) > 4);

    std::wstring fname = srcfile;
    size_t index = fname.find_last_of(L'.');

    if (index == std::wstring::npos)
    {
        printf ("Invalid input filename.");
        exit(1);
    }

    std::wstring ext = fname.substr(index);
    std::wstring provider;

    if (_wcsicmp(ext.c_str(), L".sdf") == 0)
    {
        provider = L"SdfProvider"; 
        connstr = std::wstring(L"File=") + srcfile + std::wstring(L";ReadOnly=false;");
    }
    else if (_wcsicmp(ext.c_str(), L".shp") == 0)
    {
        provider = L"ShpProvider";
        connstr = std::wstring(L"DefaultFileLocation=") + srcfile + std::wstring(L";");
    }
    else if (_wcsicmp(ext.c_str(), L".sdx") == 0 || wcscmp(ext.c_str(), L".db") == 0)
    {
        provider = L"SQLiteProvider";
        connstr = std::wstring(L"File=") + srcfile + std::wstring(L";");
    }
    else
    {
        printf("Can only connect to .sdf and .shp files.\n");
        exit(1); //ouch
    }

    FdoIConnection* conn = CreateConnection(provider.c_str());

    //open an FDO connection to the file
    conn->SetConnectionString(connstr.c_str());

    if (open)
        conn->Open();

    return conn;
}

void PopulateSRSTable(FdoIConnection* dcon, FdoIConnection* con)
{
    //populate the spatial_ref_sys table
    FdoPtr<FdoIGetSpatialContexts> sc = (FdoIGetSpatialContexts*) con->CreateCommand(FdoCommandType_GetSpatialContexts);
    FdoPtr<FdoISpatialContextReader> scr = sc->Execute();

    FdoPtr<FdoICreateSpatialContext> csc = (FdoICreateSpatialContext*)dcon->CreateCommand(FdoCommandType_CreateSpatialContext);
    
    int id = 1;

    while (scr->ReadNext())
    {
        csc->SetCoordinateSystem(scr->GetCoordinateSystem());
        csc->SetCoordinateSystemWkt(scr->GetCoordinateSystemWkt());
        csc->SetName(scr->GetName());

        csc->Execute();
    }
}


void CreateFeatureTable(FdoIConnection* dcon, FdoFeatureClass* fc, FdoIInsert* insert, int& autoGenIndex)
{
    autoGenIndex = -1;

    FdoPtr<FdoIApplySchema> as = (FdoIApplySchema*)dcon->CreateCommand(FdoCommandType_ApplySchema);

    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"SQLite Schema", NULL);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();

    FdoPtr<FdoClassDefinition> cloned = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(fc);

    classes->Add(cloned);

    //change the geometry property spatial context association
    //from name to SRID
    if (cloned->GetClassType() == FdoClassType_FeatureClass)
    {
        FdoPtr<FdoGeometricPropertyDefinition> gpd = ((FdoFeatureClass*)cloned.p)->GetGeometryProperty();
        gpd->SetSpatialContextAssociation(gpd->GetSpatialContextAssociation());
    }

    as->SetFeatureSchema(schema);
    as->Execute();

    //also populate the insert
    FdoPtr<FdoPropertyValueCollection> pvc = insert->GetPropertyValues();

    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();

    for (int i=0; i<pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);

        FdoPropertyValue* pv = FdoPropertyValue::Create(pd->GetName(), NULL);
        pvc->Add(pv);

        if (pd->GetPropertyType() == FdoPropertyType_DataProperty)
        {
            FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)(pd.p);

            if (dpd->GetIsAutoGenerated())
            {
                //if this is the autogenerated ID, we will
                //leave the property value as null when inserting
                autoGenIndex = i;
            }

            FdoDataType dt = dpd->GetDataType();

            switch (dt)
            {
            case FdoDataType_BLOB:
                {
                    FdoPtr<FdoBLOBValue> bv = FdoBLOBValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Boolean:
                {
                    FdoPtr<FdoBooleanValue> bv = FdoBooleanValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Byte:
                {
                    FdoPtr<FdoByteValue> bv = FdoByteValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_CLOB:
                {
                    FdoPtr<FdoCLOBValue> bv = FdoCLOBValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_DateTime:
                {
                    FdoPtr<FdoDateTimeValue> bv = FdoDateTimeValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Decimal:
                {
                    FdoPtr<FdoDecimalValue> bv = FdoDecimalValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Double:
                {
                    FdoPtr<FdoDoubleValue> bv = FdoDoubleValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Int16:
                {
                    FdoPtr<FdoInt16Value> bv = FdoInt16Value::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Int32:
                {
                    FdoPtr<FdoInt32Value> bv = FdoInt32Value::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Int64:
                {
                    FdoPtr<FdoInt64Value> bv = FdoInt64Value::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_Single:
                {
                    FdoPtr<FdoSingleValue> bv = FdoSingleValue::Create();
                    pv->SetValue(bv);
                }
                break;
            case FdoDataType_String:
                {
                    FdoPtr<FdoStringValue> bv = FdoStringValue::Create();
                    pv->SetValue(bv);
                }
                break;
            }
        }
        else
        {   
            FdoPtr<FdoGeometryValue> bv = FdoGeometryValue::Create();
            pv->SetValue(bv);
        }
    }
}


void BindPropVals(FdoPropertyValueCollection* props, FdoIFeatureReader* src, int autoGenIndex)
{
    int count = props->GetCount();

    for (int i=0; i<count; i++)
    {
        FdoPtr<FdoPropertyValue> pv = props->GetItem(i);
        FdoString* name = FdoPtr<FdoIdentifier>(pv->GetName())->GetName();
        FdoPtr<FdoValueExpression> ve = pv->GetValue();
        FdoLiteralValue* lv = (FdoLiteralValue*)(ve.p);

        if (lv->GetLiteralValueType() == FdoLiteralValueType_Data)
        {
            FdoDataValue* dv = (FdoDataValue*)lv;
            FdoDataType dt = dv->GetDataType();

            //check if we need to bind null
            if (i == autoGenIndex || src->IsNull(name))
            {
                dv->SetNull();
                continue;
            }

            switch (dt)
            {
            case FdoDataType_BLOB:
                {
                    FdoPtr<FdoLOBValue> lobv = src->GetLOB(name);
                    FdoPtr<FdoByteArray> ba = lobv->GetData();
                    ((FdoBLOBValue*)dv)->SetData(ba);
                }
                break;
            case FdoDataType_Boolean:
                {
                    ((FdoBooleanValue*)dv)->SetBoolean(src->GetBoolean(name));
                }
                break;
            case FdoDataType_Byte:
                {
                    ((FdoByteValue*)dv)->SetByte(src->GetByte(name));
                }
                break;
            case FdoDataType_CLOB:
                {
                    FdoPtr<FdoLOBValue> lobv = src->GetLOB(name);
                    FdoPtr<FdoByteArray> ba = lobv->GetData();
                    ((FdoCLOBValue*)dv)->SetData(ba);
                }
                break;
            case FdoDataType_DateTime:
                {
                    ((FdoDateTimeValue*)dv)->SetDateTime(src->GetDateTime(name));
                }
                break;
            case FdoDataType_Decimal:
                {
                    ((FdoDecimalValue*)dv)->SetDecimal(src->GetDouble(name));
                }
                break;
            case FdoDataType_Double:
                {
                    ((FdoDoubleValue*)dv)->SetDouble(src->GetDouble(name));
                }
                break;
            case FdoDataType_Int16:
                {
                    ((FdoInt16Value*)dv)->SetInt16(src->GetInt16(name));
                }
                break;
            case FdoDataType_Int32:
                {
                    ((FdoInt32Value*)dv)->SetInt32(src->GetInt32(name));
                }
                break;
            case FdoDataType_Int64:
                {
                    ((FdoInt64Value*)dv)->SetInt64(src->GetInt64(name));
                }
                break;
            case FdoDataType_Single:
                {
                    ((FdoSingleValue*)dv)->SetSingle(src->GetSingle(name));
                }
                break;
            case FdoDataType_String:
                {
                    ((FdoStringValue*)dv)->SetString(src->GetString(name));
                }
                break;
            }
        }
        else
        {
            FdoGeometryValue* gv = (FdoGeometryValue*)lv;

            if (src->IsNull(name))
                gv->SetNullValue();
                
            FdoPtr<FdoByteArray> arr = src->GetGeometry(name);
            gv->SetGeometry(arr);
        }
    }
}

void ConvertFDOToSDFX(const wchar_t* src, const wchar_t* dst, bool optimize)
{
    FdoPtr<FdoIConnection> con;
    FdoPtr<FdoIConnection> dcon;

    printf("Converting to SQLite Spatial format. \n");
    printf("%ls ---> %ls\n\n", src, dst);
    
    //open the FDO connection
    try
    {
        con = GetFdoCon(src, true);
        dcon = GetFdoCon(dst, false);
    }
    catch (FdoException* e)
    {
        printf ("Failed to open connection, because: %ls\n", e->GetExceptionMessage());
        e->Release();
        exit(1);
    }


    //create the destination database file
    FdoPtr<FdoICreateDataStore> cds = (FdoICreateDataStore*)dcon->CreateCommand(FdoCommandType_CreateDataStore);
    FdoPtr<FdoIDataStorePropertyDictionary> dspd = cds->GetDataStoreProperties();
    dspd->SetProperty(L"File", dst);
    cds->Execute();

    //open the connection to destination
    dcon->Open();

    //make the spatial_ref_sys table
    PopulateSRSTable(dcon, con);

    //get the feature class
    //TODO: make this a loop over all feature classes
    FdoPtr<FdoIDescribeSchema> ds = (FdoIDescribeSchema*)con->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = ds->Execute();
    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();

    for (int q=0; q<classes->GetCount(); q++)
    {
        FdoPtr<FdoFeatureClass> fc = (FdoFeatureClass*)classes->GetItem(q);

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)dcon->CreateCommand(FdoCommandType_Insert);

        //add class to the schema
        int autoGenIndex;
        CreateFeatureTable(dcon, fc, insert, autoGenIndex);

        FdoPtr<FdoPropertyValueCollection> pvc = insert->GetPropertyValues();
        insert->SetFeatureClassName(fc->GetName());

        const wchar_t* idname = NULL;

        if (autoGenIndex != -1)
        {
            FdoPtr<FdoPropertyValue> pv = pvc->GetItem(autoGenIndex);
            idname = FdoPtr<FdoIdentifier>(pv->GetName())->GetName();
        }
        else
        {
            //case where identity property is not marked
            //as autogenerated -- it still must be an int.
            FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
            FdoPtr<FdoDataPropertyDefinition> idprop = idpdc->GetItem(0);
            idname = idprop->GetName();
        }

        //now prepare the insert query -- we will use parameters so that 
        //we don't have to parse it all the time;

        FdoPtr<FdoISelect> select = (FdoISelect*)con->CreateCommand(FdoCommandType_Select);
        select->SetFeatureClassName(fc->GetName());

        int count = 0;

        if (optimize)
        {
            //add the property names to the list of the Select command.
            //This will significantly speed up initialization of SQLite provider feature readers,
            //in the case we are doing spatial optimization and we are creating lots of readers
            FdoPtr<FdoIdentifierCollection> pnames = select->GetPropertyNames();

            for (int i=0; i<pvc->GetCount(); i++)
            {
                FdoPtr<FdoPropertyValue> pv = pvc->GetItem(i);
                FdoPtr<FdoIdentifier> id = pv->GetName();
                pnames->Add(id);
            }

            //if we are reordering, we must mark the identity
            //property as autogenerated, in order to ensure that
            //SQLite inserts results in IDs that are ordered sequentially.
            //So, even if the ID is not marked as autogenerated, we will
            //clobber it.
            if (autoGenIndex == -1)
            {
                FdoPtr<FdoPropertyValue> pv = pvc->FindItem(idname);
                autoGenIndex = pvc->IndexOf(pv);
            }

            //spatial reordering -- make use of the Morton order set
            std::vector<FeatRec>* recs = ComputeMortonOrder(con, fc);  

            //make the FID filter for the source data
            FdoPtr<FdoIdentifier> idid = FdoIdentifier::Create(idname);
            FdoPtr<FdoInt32Value> idval = FdoInt32Value::Create(0);
            FdoPtr<FdoFilter> fidfilter = FdoComparisonCondition::Create(
                idid, FdoComparisonOperations_EqualTo, idval);

            select->SetFilter(fidfilter);

            for (int i=0; i<recs->size(); i++)
            {
                FeatRec& fr = (*recs)[i];
                idval->SetInt32(fr.id);

                FdoPtr<FdoIFeatureReader> rdr = select->Execute();
                
                rdr->ReadNext();

                //bind property values from reader to the SQL parameters
                BindPropVals(pvc, rdr, autoGenIndex);

                //do the insert
                insert->Execute()->Release();

                count++;

                if (count % 8192 == 0)
                {
                    printf ("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b# processed: %d", count);
                }

                rdr->Close();
            }
        }
        else
        {
            //no spatial reordering -- just run through a full table select
            FdoPtr<FdoIFeatureReader> rdr = select->Execute();

            while (rdr->ReadNext())
            {
                //bind property values from reader to the SQL parameters
                BindPropVals(pvc, rdr, autoGenIndex);

                //do the insert
                insert->Execute()->Release();

                count++;

                if (count % 8192 == 0)
                {
                    printf ("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b# processed: %d", count);
                }
            }

            rdr->Close();
        }


        printf ("\n\nTotal feature count : %d\n", count);
    }

    con->Close();
    dcon->Close();
}


bool DiffCB(FdoInt64 id, const wchar_t* attrname, void* userData)
{
    printf("ID: %d Attr: %ls different. \n", (int)id, attrname);
    return true;
}


//======================================================================
//
//  Performance testing
//
//======================================================================

FdoFilter* CreateBoundingBoxFilter(const wchar_t* gpname, double minx, double miny, double maxx, double maxy)
{
    //the FGF polygon for the spatial query
    FgfPolygon empty;

    try
    {
		empty.p[0] = minx; empty.p[1] = miny;
		empty.p[2] = maxx; empty.p[3] = miny;
		empty.p[4] = maxx; empty.p[5] = maxy;
		empty.p[6] = minx; empty.p[7] = maxy;
		empty.p[8] = minx; empty.p[9] = miny;

		FdoPtr<FdoByteArray> fgf = FdoByteArray::Create((FdoByte*)&empty, sizeof(empty));

		//set up the spatial filter
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(fgf);
		FdoSpatialCondition* cond = FdoSpatialCondition::Create(
			gpname, FdoSpatialOperations_EnvelopeIntersects, gv);

		return cond;
	}
    catch (FdoException* e)
    {
		FdoString* message = e->GetExceptionMessage();
        printf("%ls\n", message);
        exit(1);
    }

	return NULL;
}

FdoFilter* CreateIntersectsFilter(const wchar_t* gpname, double minx, double miny, double maxx, double maxy)
{
    //the FGF polygon for the spatial query
    FgfPolygon empty;

    try
    {
		empty.p[0] = minx; empty.p[1] = miny;
		empty.p[2] = maxx; empty.p[3] = miny;
		empty.p[4] = maxx; empty.p[5] = maxy;
		empty.p[6] = minx; empty.p[7] = maxy;
		empty.p[8] = minx; empty.p[9] = miny;

		FdoPtr<FdoByteArray> fgf = FdoByteArray::Create((FdoByte*)&empty, sizeof(empty));

		//set up the spatial filter
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(fgf);
		FdoSpatialCondition* cond = FdoSpatialCondition::Create(
			gpname, FdoSpatialOperations_Intersects, gv);

		return cond;
	}
    catch (FdoException* e)
    {
		FdoString* message = e->GetExceptionMessage();
        printf("%ls\n", message);
        exit(1);
    }

	return NULL;
}

void TestPerformance(const wchar_t* filename)
{
    printf ("Testing file: %ls\n", filename);

    FdoPtr<FdoIConnection> con = GetFdoCon(filename, true);
    
    //get the schema
    FdoPtr<FdoIDescribeSchema> ds = (FdoIDescribeSchema*)con->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = ds->Execute();

    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();

    printf ("Feature Schema: %ls\n", schema->GetName());

    std::wstring fcname;
    std::wstring geomname;

    for (int i=0; i<classes->GetCount(); i++)
    {
        FdoPtr<FdoClassDefinition> fc = classes->GetItem(i);

        if (!i) fcname = fc->GetName();

        printf ("\tClass %ls\n", fc->GetName());
        printf ("\tProperties: %ls\n", fc->GetName());
        
        FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();

        for (int j=0; j<pdc->GetCount(); j++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(j);

            printf("\t\tProperty %ls :\n", pd->GetName());

            FdoPropertyType ptype = pd->GetPropertyType();

            if (ptype == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)pd.p;
            }
            else if (ptype == FdoPropertyType_GeometricProperty)
            {
                if (!i) geomname = pd->GetName();

                FdoGeometricPropertyDefinition* gpd = (FdoGeometricPropertyDefinition*)pd.p;
                printf ("sc assoc: %ls\n", gpd->GetSpatialContextAssociation());
            }
        }

        printf ("\t====\n\n");
    }

/*
    FdoPtr<FdoIGetSpatialContexts> gsc = (FdoIGetSpatialContexts*)con->CreateCommand(FdoCommandType_GetSpatialContexts);
    FdoPtr<FdoISpatialContextReader> scrdr = gsc->Execute();

    while (scrdr->ReadNext())
    {
        printf ("sc name %ls\n", scrdr->GetName());

        FdoPtr<FdoByteArray> ba  = scrdr->GetExtent();

        FgfPolygon* ext = (FgfPolygon*)ba->GetData();
    }
*/
/*
    FdoPtr<FdoISQLCommand> select = (FdoISQLCommand*)con->CreateCommand(FdoCommandType_SQLCommand);
    std::wstring sql = L"SELECT * FROM " + fcname + L" WHERE sin(CRN_RAD) > 0";
    select->SetSQLStatement(sql.c_str());
*/
    /*
    FdoPtr<FdoISelectAggregates> sa = (FdoISelectAggregates*)con->CreateCommand(FdoCommandType_SelectAggregates);
    sa->SetDistinct(true);
    FdoPtr<FdoIdentifierCollection> aggprops = sa->GetPropertyNames();
    
    FdoPtr<FdoIdentifier> iii = FdoIdentifier::Create(L"GENUS");
    aggprops->Add(iii);

    sa->SetFeatureClassName(fcname.c_str());


    FdoPtr<FdoIDataReader> rdr = sa->Execute();

    while (rdr->ReadNext())
    {
        printf("%ls\n", rdr->GetString(L"GENUS"));
    }
    */
/*
   FdoPtr<FdoISelectAggregates> sa = (FdoISelectAggregates*) con->CreateCommand(FdoCommandType_SelectAggregates);
   
   FdoPtr<FdoIdentifierCollection> aggprops = sa->GetPropertyNames();
   aggprops->Add(FdoComputedIdentifier::Create(L"mymax", FdoExpression::Parse(L"max(CRN_RAD)")));
    
   sa->SetFeatureClassName(fcname.c_str());
   
   FdoPtr<FdoIDataReader> drdr = sa->Execute();
   
   while (drdr->ReadNext())
   {
       double val = drdr->GetDouble(L"mymax");
       printf ("maxval val: %lf\n", val);
   } 
   
   drdr->Close();
*/

/*
    FdoPtr<FdoISelect> sss = (FdoISelect*)con->CreateCommand(FdoCommandType_Select);
    sss->SetFeatureClassName(L"canhiway");
    //FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"OGC_FID=1 OR OGC_FID=2");
    FdoPtr<FdoFilter> filter = CreateIntersectsFilter(geomname.c_str(), -180, -90, 180, 90);
    sss->SetFilter(filter);

    //FdoPtr<FdoIdentifierCollection> ssprops = sss->GetPropertyNames();
    //FdoPtr<FdoIdentifier> sid = FdoIdentifier::Create(L"LEGAL_PID");
    //ssprops->Add(sid);
    FdoPtr<FdoIFeatureReader> rdr = sss->Execute();

    FILE* fp = fopen ("c:\\props.txt", "w");
    while (rdr->ReadNext())
    {
        if (!rdr->IsNull(L"highway"))
        {
            FdoString* val = rdr->GetString(L"highway");
            int len;
            const FdoByte* geom = rdr->GetGeometry(L"GEOMETRY", &len);
            //FgfPolygon* poly = (FgfPolygon*)geom;
            fprintf(fp, "%ls\n", val);
        }
    }
    fclose(fp);
*/

    //get the overall extent
    double minx = DBL_MAX;
    double miny = DBL_MAX;
    double maxx = -DBL_MAX;
    double maxy = -DBL_MAX;

    FdoPtr<FdoISelectAggregates> agg = (FdoISelectAggregates*)con->CreateCommand(FdoCommandType_SelectAggregates);
    agg->SetFeatureClassName(fcname.c_str());
    FdoPtr<FdoIdentifierCollection> props = agg->GetPropertyNames();

    std::wstring exps = std::wstring(L"SpatialExtents(") + geomname + L")";
    FdoPtr<FdoExpression> exp = FdoExpression::Parse(exps.c_str());
    FdoPtr<FdoComputedIdentifier> se = FdoComputedIdentifier::Create(L"Extents", exp);
    props->Add(se);

    std::wstring expc = std::wstring(L"Count()");
    exp = FdoExpression::Parse(expc.c_str());
    se = FdoComputedIdentifier::Create(L"TheCount", exp);
    props->Add(se);


    FdoPtr<FdoIDataReader> extrdr = agg->Execute();
    extrdr->ReadNext();

    FdoPtr<FdoByteArray> ba = extrdr->GetGeometry(L"Extents");

    FgfPolygon* poly = (FgfPolygon*)ba->GetData();

    minx = poly->p[0];
    miny = poly->p[1];
    maxx = poly->p[4];
    maxy = poly->p[5];

    extrdr->Close();

    printf ("Bounds : %lf %lf %lf %lf\n", minx, miny, maxx, maxy);

    double min[2];
    double max[2];
    double factor = 0.49;
    min[0] = minx + (maxx - minx)*factor;
    min[1] = miny + (maxy - miny)*factor;
    max[0] = maxx - (maxx - minx)*factor;
    max[1] = maxy - (maxy - miny)*factor;

    FdoPtr<FdoISelect> select = (FdoISelect*)con->CreateCommand(FdoCommandType_Select);
    select->SetFeatureClassName(fcname.c_str());

    FdoPtr<FdoFilter> bbox0 = CreateBoundingBoxFilter(geomname.c_str(), min[0], min[1],max[0], max[1]);
    //select->SetFilter(bbox0);

    //FdoPtr<FdoFilter> somefilter = FdoFilter::Parse(L"GENUS=CONCAT('hello', 7)");
    //FdoPtr<FdoBinaryLogicalOperator> op = FdoBinaryLogicalOperator::Create(
    //    bbox0, FdoBinaryLogicalOperations_And, somefilter);
    //select->SetFilter(op);

    printf("Spatial filter: %ls\n", bbox0->ToString());
    
/*
    FdoPtr<FdoIdentifierCollection> idprops = select->GetPropertyNames();
    FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(geomname.c_str());
    idprops->Add(id);
    id = FdoIdentifier::Create(L"GENUS");
    idprops->Add(id);
    id = FdoIdentifier::Create(L"SCI_NM");
    idprops->Add(id);
    id = FdoIdentifier::Create(L"TREE_ID");
    idprops->Add(id);
    id = FdoIdentifier::Create(L"MAINT");
    idprops->Add(id);
    id = FdoIdentifier::Create(L"DEADWOOD");
    idprops->Add(id);
    id = FdoIdentifier::Create(L"CROWN_AREA");
    idprops->Add(id);
    id = FdoIdentifier::Create(L"CROWN_VOL");
    idprops->Add(id); 
    id = FdoIdentifier::Create(L"CRN_RAD");
    idprops->Add(id);
*/
    int blab = 0;
    int fcount = 0;
    const wchar_t* sp[30];
    double rp[30];
    clock_t t0;
    clock_t t1;


    blab = 0;
    fcount = 0;
    t0 = clock();

    for (int i=0; i<1; i++)
    {
        FdoPtr<FdoIFeatureReader> rdr = (FdoIFeatureReader*)(select->Execute());
        
        while (rdr.p->ReadNext())
        {
            int count = 0;
            const unsigned char* geom = rdr.p->GetGeometry(geomname.c_str(), &count);

          // printf ("%d,", rdr->GetInt32(L"FeatId"));
/*
			if (!rdr->IsNull(L"GENUS")) sp[0] = rdr->GetString(L"GENUS");
			if (!rdr->IsNull(L"SCI_NM")) sp[1] = rdr->GetString(L"SCI_NM");
			if (!rdr->IsNull(L"TREE_ID")) sp[3] = rdr->GetString(L"TREE_ID");
			if (!rdr->IsNull(L"MAINT")) sp[4] = rdr->GetString(L"MAINT");

            rp[0] += rdr->GetDouble(L"DEADWOOD");
            rp[1] += rdr->GetDouble(L"CROWN_AREA");
            rp[2] += rdr->GetDouble(L"CROWN_VOL");
            rp[3] += rdr->GetDouble(L"CRN_RAD");
*/
            blab += count;
            fcount++;
        }

        rdr->Close();
    }

    t1 = clock();

    printf ("blab is %d, fcount is %d,  in %d\n", blab, fcount, t1 - t0);
}


void TestDelete()
{
    FdoPtr<FdoIConnection> con = GetFdoCon(L"E:\\carbon\\p4\\GBU\\Metropolis_0\\Output\\Release\\Bin\\Data\\Cache\\test9999\\CityDatabase.db", true);

    FdoPtr<FdoISelect> sel = (FdoISelect*)con->CreateCommand(FdoCommandType_Select);

    sel->SetFeatureClassName(L"TerrainPoints");

    FdoPtr<FdoIFeatureReader> rdr = sel->Execute();

    rdr->ReadNext();
    rdr->Close();

    FdoPtr<FdoIDelete> del = (FdoIDelete*)con->CreateCommand(FdoCommandType_Delete);

    del->SetFeatureClassName(L"TerrainPoints");

    del->SetFilter(L"DATA_SOURCE_ID=1");

    int items = del->Execute();

    printf ("delete : %d\n", items);
}


void Usage()
{
    printf ("\n FDO SQLite Converter \n");
    printf ("~~~~~~~~~~~~~~~~~~~~~~\n");
    printf ("Usage: \n");
    printf ("SQLiteConverter [optimize] source.xxx destination.xxx\n\n");
    printf ("source      : Full path to source file (sdf, shp or sqlite db)\n");
    printf ("destination : Full path to destination file (sdf or sqlite db)\n");
    printf ("optimize    : Optional flag, indicating whether to optimize spatial order\n");
    printf ("              of features, for faster BBOX query (relevant for sqlite output only)\n");
}


int main(int argc, char* argv[])
{
    std::vector<std::wstring> wargv;

    for (int i=0; i<argc; i++)
    {
        wchar_t tmp[MAX_PATH];
        mbstowcs(tmp, argv[i], MAX_PATH);
        wargv.push_back(std::wstring(tmp));
    }

    if (argc == 3)
    {
        if (wcscmp(wargv[1].c_str(), L"test") == 0)
        {
            //TestDelete();
            TestPerformance(wargv[2].c_str());
        }
        else
        {
            try 
            {
                clock_t t0 = clock();
                ConvertFDOToSDFX(wargv[1].c_str(), wargv[2].c_str(), false);
                clock_t t1 = clock();
                printf ("Conversion time: %d\n", t1 - t0);
            }
            catch (FdoException* e)
            {
                printf ("Conversion failed : %ls\n", e->GetExceptionMessage());
                return 1;
            }
        }

        return 0;
    }
    else if (argc == 4)
    {
        if (wcscmp(wargv[1].c_str(), L"optimize") == 0)
        {
            try
            {
                clock_t t0 = clock();
                ConvertFDOToSDFX(wargv[2].c_str(), wargv[3].c_str(), true);
                clock_t t1 = clock();
                printf ("Conversion time: %d\n", t1 - t0);
            }
            catch (FdoException* e)
            {
                printf ("Conversion failed : %ls\n", e->GetExceptionMessage());
                return 1;
            }
            
            return 0;
        }
        if (wcscmp(wargv[1].c_str(), L"diff") == 0)
        {
            /*
            try
            {
                clock_t t0 = clock();
                CompareFiles(wargv[2].c_str(), wargv[3].c_str());
                clock_t t1 = clock();
                printf ("Comparison time: %d\n", t1 - t0);
            }
            catch (FdoException* e)
            {
                printf ("Comparison failed : %ls\n", e->GetExceptionMessage());
                return 1;
            }
            
            return 0;
            */
        }
        else
        {
            printf("Unknown command line parameters.\n");
            exit(1);
        }
    }
    else
    {
        Usage();
    }

    return 1;
}

