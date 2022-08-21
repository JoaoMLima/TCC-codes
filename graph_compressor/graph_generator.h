int getRandom(int limit) {
    return (rand() % limit + limit) % limit;
}

/*
 * Returns a random simple graph represented by a adjacency matrix.
 * "seed" should be the seed used to randomize the generation of the graph
 * "n" should be the number of nodes on the graph
 * "m" should be the number of edges on the graph, between 0 and n*(n-1)
 */
vector<vb> random_graph(int seed, int n, int m) {
    srand(seed);
    vector<vb> ret(n, vb(n));
    oset<pii> edges_set;
    rep(i, 0, n) {
        rep(j, 0, n) {
            if (i != j) edges_set.insert({i, j});
        }
    }
    //trace(sz(edges_set));
    rep(i, 0, m) {
        int ran = getRandom(sz(edges_set));
        auto drawn_edge = *edges_set.find_by_order(ran);
        edges_set.erase(drawn_edge);
        //trace(ran, drawn_edge, sz(edges_set));
        ret[drawn_edge.first][drawn_edge.second] = true;
    }
    return ret;
}