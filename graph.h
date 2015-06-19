#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

struct Edge {
    Node *ns[2];
    Triangle *ts[2];

    std::vector<Edge *> adj;

    void flip();

    int side(const Point &p);
    int inside(const Point &p, const Triangle *triangle);

  private:
    void find_triangle_indices(int &inds[]);
    void flip_adjacent_edges(int &inds[]);
    void flip_adjacent_nodes(int &inds[]);
    void replace_triangle(const Triangle *next, const Triangle *old);
};

struct Point {
    double x;
    double y;

    double cross2d(const Point &p);

    Point (double x, double y);
};

struct Node {
    Point loc;

    std::vector<Edge *> edges;

    Node (const Point &p);
};

struct Triangle {
    Node *ns[3];
    Edge *es[3];

    bool contains(const Point &p);

    int find_edge(const Edge *e);
};

#endif

