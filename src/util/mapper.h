#ifndef MAPPER_H
#define MAPPER_H

#include <map>

template <typename T, typename U>
class mapper
{
public:
    mapper(const T& key, const U& val)
    {
        _map[key] = val;
    }

    mapper<T, U>& operator()(const T& key, const U& val)
    {
        _map[key] = val;
        return *this;
    }

    operator std::map<T, U>()
    {
        return _map;
    }

private:
    std::map<T, U> _map;
};

#endif // MAPPER_H
