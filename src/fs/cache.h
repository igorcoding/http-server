#ifndef CACHE_H
#define CACHE_H

#include "file.h"

#include <unordered_map>

class cache
{
public:
    cache();
    ~cache();

    void add(std::string path, file_ptr f);
    file_ptr get(const std::string& path);

private:
    std::unordered_map<std::string, file_ptr> _cache;
};

#endif // CACHE_H
