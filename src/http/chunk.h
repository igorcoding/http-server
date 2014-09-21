#ifndef CHUNK_H
#define CHUNK_H

#include <cstring>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class chunk
{
public:
    typedef chunk* ptr;

    chunk(const char* buf, size_t size)
        : _data(nullptr),
          _size(size),
          _immediate_delete(true)
    {
        _data = new char[_size];
        memcpy(_data, buf, _size);
    }

    ~chunk()
    {
        delete[] _data;
    }

    const char* data()
    {
        return _data;
    }

    size_t size()
    {
        return _size;
    }

    bool is_immediate_delete()
    {
        return _immediate_delete;
    }

    void postpone_delete()
    {
        _immediate_delete = false;
    }

    static chunk::ptr merge_chunks(const std::vector<chunk::ptr>& chunks);

private:
    char* _data;
    size_t _size;
    bool _immediate_delete;
};

#endif // CHUNK_H
