#pragma once

#include <map>
#include <vector>

class Flow
{
public:
    Flow();

    void addVertex(const void* ptr);
    void addEdge(const void* src, const void* dst);
    void setVertexAsSource(const void* ptr);

    bool flow(std::map<const void*, size_t>* flowOrder = nullptr) const;

private:
    bool doesVertexExist(const void* vertex, std::map<const void*, std::vector<const void*>>::iterator* it = nullptr);

    std::map<const void*, std::vector<const void*>> m_edges;
    std::vector<const void*> m_sourceVertices;
};
