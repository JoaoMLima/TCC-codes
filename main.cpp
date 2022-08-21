#include "utils/template.h"
#include "utils/trace.h"
#include "graph_compressor/graph_compressor.h"
#include "graph_compressor/graph_generator.h"
#include "graph_compressor/segtree_graph.h"


int main() {
    int n = 6;
    int m = 15;
    int seed = 449644;

    auto graph = random_graph(seed, n, m);
    trace(graph);
    trace("Grafo criado!");
    auto comp = graph_compression(graph);
    trace(sz(comp));
    trace(comp);

    segtree_graph g;
    g.build(n);
    for (auto rect : comp) {
        g.addEdge(rect.first.first, rect.first.second, rect.second.first, rect.second.second);
    }

    auto count_vertices_edges = g.count_necessary_parts();

    trace("Com compressão em retângulos:",g.n, g.m, count_vertices_edges.first, count_vertices_edges.second);

    return 0;
}