#ifndef CHUNK_H
#define CHUNK_H

#include <cstring>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class chunk
{
public:
    typedef boost::shared_ptr<chunk> ptr;

    chunk(const char* buf, size_t size)
        : _data(nullptr),
          _size(size)
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

    static chunk::ptr merge_chunks(const std::vector<chunk::ptr>& chunks);

private:
    char* _data;
    size_t _size;
};

#endif // CHUNK_H
