#include "cache.h"

cache::cache()
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
    _cache[path] = f;
}

file_ptr cache::get(const std::string& path)
{
    auto it = _cache.find(path);
    if (it == _cache.end())
        return nullptr;
    return it->second;
}
