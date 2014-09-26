#include "cache.h"

#include <ctime>
#include <iostream>
#include <boost/make_shared.hpp>

cache::cache(double cache_period)
    : _cache_period(cache_period * 60)
{
}

cache::~cache()
{
    std::cout << "Cache size: " << _cache.size() << ". Deleting cache\n";
    for (auto& kv : _cache) {
        std::cout << kv.first << "\n";
        delete kv.second;
    }
}

void cache::add(std::string path, file::ptr f)
{
    _cache[path] = f;
}

file::ptr cache::get(const std::string& path)
{
    auto it = _cache.find(path);
    if (it == _cache.end())
        return nullptr;
    if (check_to_delete(it->second)) {
        remove(it);
        return nullptr;
    }
    return it->second;
}

int cache::count_expires()
{
    time_t t = time(nullptr);
    auto t_sec = static_cast<int>(t) + _cache_period;
    return t_sec;
}

void cache::remove(cache_type::iterator it)
{
    delete it->second;
    _cache.erase(it);
}


bool cache::check_to_delete(file::ptr f)
{
    time_t time_now = time(nullptr);
    auto time_now_sec = static_cast<int>(time_now);
    return time_now_sec >= f->get_expires();
}

