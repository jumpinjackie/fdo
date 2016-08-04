#include "MappedFile.h"
#include <time.h>

#ifndef VECTOR_MF_H
#define VECTOR_MF_H

#define MAPPED_CNT_PAGES (16)
#define MAPPED_RECS_PERPAGE (64*1024)
#define MAX_MEMSIZE_VECT (MAPPED_CNT_PAGES*MAPPED_RECS_PERPAGE)

template<class _Ty >class vectormf
{
public:
typedef MappedFile<_Ty, MAPPED_RECS_PERPAGE> VectorMappedFile;
private:
    std::vector<_Ty>* _vector;
    VectorMappedFile* _mapFile;
public:
	vectormf()
	{
        _vector = new std::vector<_Ty>();
        _mapFile = NULL;
	}
    ~vectormf()
	{
        if (_vector != NULL)
            delete _vector;
        if (_mapFile != NULL)
        {
            _mapFile->close();
            delete _mapFile;
        }
        _vector = NULL;
        _mapFile = NULL;
	}
    void push_back(const _Ty& val)
    {
        setsize(size()+1, val);
    }
    size_t size()
    {
        return (_vector != NULL) ? _vector->size() : _mapFile->numrecs();
    }

    _Ty& at(size_t idx) const
    {
        return (_vector != NULL) ? _vector->at(idx) : *(_Ty*)_mapFile->load_noaddref(idx);
    }
    
    _Ty& at(size_t idx)
    {
        return (_vector != NULL) ? _vector->at(idx) : *(_Ty*)_mapFile->load_noaddref(idx);
    }
    
    _Ty& operator[](size_t idx) const
    {
        return (_vector != NULL) ? (*_vector)[idx] : *(_Ty*)_mapFile->load_noaddref(idx);
    }
    
    _Ty& operator[](size_t idx)
    {
        return (_vector != NULL) ? (*_vector)[idx] : *(_Ty*)_mapFile->load_noaddref(idx);
    }
    void setsize(size_t sz, const _Ty& defaultVal)
    {
        // we keep max 1048576 records in memory
        if (sz > MAX_MEMSIZE_VECT)
        {
            if (_mapFile == NULL)
            {
                std::wstring seed = L"vector";
                _mapFile = new VectorMappedFile(MAPPED_CNT_PAGES);
                _mapFile->createTempFile(seed);
                for (size_t idx = 0; idx<_vector->size(); idx++)
                {
                    _Ty* node = (_Ty*)_mapFile->load_newrec(idx);
                    *node = (*_vector)[idx];
                    _mapFile->release(node);
                }
                delete _vector;
                _vector = NULL;
            }
            if (sz >= _mapFile->numrecs())
            {
                IndexType cnt = _mapFile->numrecs();
                for (size_t idx = cnt; idx<sz; idx++)
                {
                    _Ty* node = (_Ty*)_mapFile->load_newrec(idx);
                    *node = defaultVal;
                    _mapFile->release(node);
                }
            }
        }
        else
        {
            if (sz >= _vector->size())
                _vector->insert(_vector->end(), sz-_vector->size(), defaultVal);
        }
    }
};
typedef vectormf<FdoInt64> VectorMF;

#endif //VECTOR_MF_H