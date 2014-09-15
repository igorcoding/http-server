#ifndef CACHE_H
#define CACHE_H

#include "file.h"

#include <unordered_map>

class cache_entry
{
public:
    cache_entry(file_ptr file, int remove_time);
    ~cache_entry();
    bool check_to_delete();

    file_ptr file() const;

private:
    file_ptr _file;
    int _remove_time;
};

class cache
{
    typedef std::unordered_map<std::string, cache_entry*> cache_type;
public:
    cache(double cache_period);
    ~cache();

    void add(std::string path, file_ptr f);
    file_ptr get(const std::string& path);

private:
    void remove(cache_type::iterator it);

private:
    cache_type _cache;
    int _cache_period;
};

#endif // CACHE_H
