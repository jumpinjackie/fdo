#include "FDORFP.h"
#include "FdoRfpKeyColor.h"

FdoRfpKeyColor::FdoRfpKeyColor(void)
{
}

FdoRfpKeyColor::~FdoRfpKeyColor(void)
{
}

bool FdoRfpKeyColorCollection::Contains(FdoRfpKeyColorP& val)
{
	bool bFound = false;
	int i;
	for (i=0; i<GetCount(); i++)
	{
		FdoRfpKeyColorP key = GetItem(i);
		if (key->type == val->type)
		{
			if (key->type == FdoRfpKeyColor::kData1 && val->value.data1 == key->value.data1
				|| key->type == FdoRfpKeyColor::kData8 && val->value.data8 == key->value.data8
				|| key->type == FdoRfpKeyColor::kData16 && val->value.data16 == key->value.data16
				|| key->type == FdoRfpKeyColor::kData32 && val->value.data32 == key->value.data32
				|| key->type == FdoRfpKeyColor::kData64 && val->value.data64 == key->value.data64)
			{
				bFound = true;
				break;
			}
		}
	}
	return bFound;
}

FdoInt32 FdoRfpKeyColorCollection::Add(FdoRfpKeyColorP& val)
{
	if (!Contains(val))
		return FdoCollection<FdoRfpKeyColor, FdoException>::Add(val);
	else
		return GetCount()-1;
}

void FdoRfpKeyColorCollection::Add(FdoRfpKeyColorsP& coll)
{
	int i;
	for (i=0; i<coll->GetCount(); i++)
	{
		FdoRfpKeyColorP key = coll->GetItem(i);
		Add(key);
	}
}

FdoRfpKeyColorCollection* FdoRfpKeyColorCollection::Create()
{
	return new FdoRfpKeyColorCollection();
}
