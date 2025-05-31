#ifndef GRAPHREPRESENTATION_H
#define GRAPHREPRESENTATION_H

class GraphRepresentation
{
public:
    // Virtual destructor makes the class polymorphic
    virtual ~GraphRepresentation() = default;
    // Pure virtual functions that derived classes must implement - marked as const
    [[nodiscard]] virtual int getVertexCount() const = 0;
    [[nodiscard]] virtual DefinitelyNotAVector<Edge> toEdgeList() const = 0;

    // Optional: Add other common interface methods
    virtual void addEdge(int from, int to, int weight) = 0;

};

#endif //GRAPHREPRESENTATION_H
