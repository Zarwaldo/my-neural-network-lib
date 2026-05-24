#include <network/Flow.h>

#include <stdexcept>

Flow::Flow()
    : m_edges()
    , m_sourceVertices()
{}

void
Flow::addVertex(const void* ptr)
{
    if (doesVertexExist(ptr))
    {
        throw std::runtime_error("Flow::addVertex: Vertex already exists.");
    }

    m_edges[ptr] = {};
}

void
Flow::addEdge(const void* src, const void* dst)
{
    std::map<const void*, std::vector<const void*>>::iterator it;
    if (!doesVertexExist(src, &it))
    {
        throw std::runtime_error("Flow::addEdge: Source vertex does not exist.");
    }

    if (!doesVertexExist(dst))
    {
        throw std::runtime_error("Flow::addEdge: Destination vertex does not exist.");
    }

    std::vector<const void*>& destinations = it->second;
    if (std::find(destinations.begin(), destinations.end(), dst) != destinations.end())
    {
        throw std::runtime_error("Flow::addEdge: Edge already exists.");
    }

    destinations.push_back(dst);
}

void
Flow::setVertexAsSource(const void* ptr)
{
    if (!doesVertexExist(ptr))
    {
        throw std::runtime_error("Flow::setVertexAsSource: Vertex does not exist.");
    }

    if (std::find(m_sourceVertices.begin(), m_sourceVertices.end(), ptr) != m_sourceVertices.end())
    {
        throw std::runtime_error("Flow::setVertexAsSource: Vertex is already a source vertex.");
    }

    m_sourceVertices.push_back(ptr);
}

bool
Flow::flow(std::map<const void*, size_t>* resultingFlowOrder) const
{
    std::map<const void*, size_t> flowOrder;
    for (const std::pair<const void*, std::vector<const void*>>& pair : m_edges)
    {
        flowOrder[pair.first] = 0;
    }

    for (const void* const& sourceVertex : m_sourceVertices)
    {
        flowOrder[sourceVertex] = 1;
    }

    size_t time = 1;
    bool newVerticesExplored = true;

    while (newVerticesExplored)
    {
        newVerticesExplored = false;

        for (const std::pair<const void*, std::vector<const void*>>& pair : m_edges)
        {
            const void* const& source = pair.first;
            if (flowOrder[source] != time)
            {
                continue;
            }

            const std::vector<const void*>& dests = pair.second;
            for (const void* const& dest : dests)
            {
                if (flowOrder[dest] > 0)
                {
                    continue;
                }

                flowOrder[dest] = time + 1;
                newVerticesExplored = true;
            }
        }

        time++;
    }

    if (resultingFlowOrder != nullptr)
    {
        *resultingFlowOrder = flowOrder;
    }

    for (const std::pair<const void*, size_t>& pair : flowOrder)
    {
        if (pair.second == 0)
        {
            return false;
        }
    }

    return true;
}

bool
Flow::doesVertexExist(const void* vertex, std::map<const void*, std::vector<const void*>>::iterator* iterator)
{
    std::map<const void*, std::vector<const void*>>::iterator it = std::find_if(
        m_edges.begin(),
        m_edges.end(),
        [vertex](const std::pair<const void*, std::vector<const void*>>& pair) {
            return pair.first == vertex;
        });

    if (iterator != nullptr)
    {
        *iterator = it;
    }

    return it != m_edges.end();
}
