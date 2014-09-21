#ifndef CACHE_H
#define CACHE_H

#include "file.h"

#include <unordered_map>
#include <mutex>

class cache
{
    typedef std::unordered_map<std::string, file::ptr> cache_type;
public:
    cache(double cache_period);
    ~cache();

    void add(std::string path, file::ptr f);
    file::ptr get(const std::string& path);

    int count_expires();

private:
    void remove(cache_type::iterator it);
    bool check_to_delete(file::ptr f);

private:
    cache_type _cache;
    int _cache_period;
};

#endif // CACHE_H
