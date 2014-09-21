#include "chunk.h"

chunk::ptr chunk::merge_chunks(const std::vector<chunk::ptr>& chunks)
{
    if (chunks.size() == 1) {
        chunks[0]->postpone_delete();
        return chunks[0];
    }

    size_t total_size = 0;
    for (auto& c : chunks) {
        total_size += c->size();
    }

    char* merged = new char[total_size];
    size_t offset = 0;
    for (auto c : chunks) {
        memcpy(merged + offset, c->data(), c->size());
        offset += c->size();
    }

    auto ch = new chunk(merged, total_size);
    delete[] merged;
    merged = nullptr;
    return ch;
}
