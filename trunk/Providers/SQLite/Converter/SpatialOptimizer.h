


struct FeatRec
{
    int id;
    unsigned mortonid;

    bool operator< (const FeatRec& fr)
    {
        return mortonid < fr.mortonid;
    }
};

std::vector<FeatRec>* ComputeMortonOrder(FdoIConnection* con, FdoFeatureClass* fc);
