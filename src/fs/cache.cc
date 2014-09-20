#include "cache.h"

#include <ctime>

cache::cache(double cache_period)
    : _cache_period(cache_period * 60)
{
}

cache::~cache()
{
    std::cout << "deleting cache\n";
    for (auto& kv : _cache) {
        delete kv.second;
    }
}

void cache::add(std::string path, file_ptr f)
{
    time_t t = time(nullptr);
    auto t_ms = static_cast<int>(t) + _cache_period;
    _cache[path] = new cache_entry(f, t_ms);
}

file_ptr cache::get(const std::string& path)
{
    auto it = _cache.find(path);
    if (it == _cache.end())
        return nullptr;
    if (it->second->check_to_delete()) {
        remove(it);
        return nullptr;
    }
    return it->second->file();
}

void cache::remove(cache_type::iterator it)
{
    delete it->second;
    _cache.erase(it);
}


cache_entry::cache_entry(file_ptr file, int remove_time)
{
    _file = file;
    _remove_time = remove_time;
}

cache_entry::~cache_entry()
{
//    delete _file;
//    _file = nullptr;
}

bool cache_entry::check_to_delete()
{
    time_t time_now = time(nullptr);
    auto time_now_ms = static_cast<int>(time_now);
    return time_now_ms >= _remove_time;
}

file_ptr cache_entry::file() const
{
    return _file;
}
