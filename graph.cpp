#include "graph.h"

int Edge::side(const Point &p) {
    Point along (es[1]->loc.x - es[0]->loc.x, es[1]->loc.y - es[0]->loc.y);

    double cval = along.cross2d(along);

    return cval > 0;
}

int Edge::inside(const Point &p, const Triangle *triangle) {
    int eside = this->side(p);
    int tside = ts[1] == triangle;

    return eside == tside;
}

void Edge::flip() {
    if (!ts[0] || !ts[1]) return;

    int inds[2];
    
    find_triangle_indices(inds);
    flip_adjacent_edges(inds);
    flip_adjacent_nodes(inds);
}

void Edge::find_triangle_indices(int &inds[]) {
    inds[0] = ts[0].find_edge(this);
    inds[1] = ts[1].find_edge(this);
}

void Edge::flip_adjacent_edges(int &inds[]) {
    Edge *esc[4];
    for (int i=0;i<4;++i) {
        int tri = i >> 1;
        esc[i] = ts[tri].es[(inds[tri]+(i&1)+1)%3];
    }
    ts[0].es[0] = this;
    ts[1].es[0] = this;

    for (int i=0;i<4;++i) {
        ts[tri].es[(i&1)+1] = esc[(i+3)%4];
    }

    esc[3]->flip_triangle(ts[1], ts[0]);
    esc[1]->flip_triangle(ts[0], ts[1]);
}

void Edge::flip_adjacent_nodes(int &inds[]) {
    Node *nsc[4] = {ns[0], ts[0].ns[(inds[0]+1)%3], ns[0], ts[1].ns[(inds[1]+1)%3]};

    ts[0].ns[0] = nsc[0];
    ts[0].ns[1] = nsc[1];
    ts[0].ns[2] = nsc[3];
    ts[1].ns[0] = nsc[2];
    ts[1].ns[1] = nsc[3];
    ts[1].ns[2] = nsc[1];

    std::remove(ns[0]->adj.begin(), ns[0]->adj.end(), this);      
    std::remove(ns[1]->adj.begin(), ns[1]->adj.end(), this);

    ns[0] = nsc[3];
    ns[1] = nsc[1];

    ns[0]->adj.push_back(this);
    ns[1]->adj.push_back(this);
}

void Edge::replace_triangle(const Triangle *next, const Triangle *old) {
    if (ts[0] == old) ts[0] = next;
    if (ts[1] == old) ts[1] = next;
}

Point::Point(double x, double y) : x(x), y(y) { }

double Point::cross2d(const Point &p) {
    return this->x * p.y - this->y * p.x;
}

Node::Node(Point &p) : loc(p.x, p.y) {}

bool Triangle::contains(const Point &p) {
    return es[0]->inside(p, this) && es[1]->inside(p, this) && es[2]->inside(p, this);
}

int Triangle::find_edge(const Edge *e) {
    for (int i=0;i<3;++i) {
        if (es[i] == e) return i;
    }
    return -1;
}

