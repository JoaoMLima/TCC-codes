// #include "../utils/template.h"
// #include "../utils/trace.h"

struct segtree_graph {
    int n = 0, m = 0;
    int real_n = 0, real_m = 0;
    int root_1 = 0, root_2 = 0;
    vector<vector<int>> adjacency_list;
    vector<bool> used;
    vector<int> L;
    vector<int> R;

    int addRealVertex() {
        adjacency_list.push_back(vector<int>());
        L.push_back(-1);
        R.push_back(-1);
        used.push_back(false);
        return real_n++;
    }

    void addRealEdge(int a, int b) {
        // trace("addRealEdge:", a, b);
        assert(a >= 0 && a < sz(adjacency_list));
        adjacency_list[a].push_back(b);
        // trace("addRealEdge:", a, b);
        real_m++;
    }

    int buildSegTree(int l, int r) {
        // trace("l:",l, "r:",r);
        if (l != r) {
            int id = addRealVertex();
            // trace(id);
            int mid = (l+r)/2;
            assert(id >= 0 && id < sz(L));
            int x = buildSegTree(l, mid);
            assert(id >= 0 && id < sz(R));
            int y = buildSegTree(mid+1, r);

            L[id] = x;
            R[id] = y;
            // trace("teste",id, L[id], R[id], l, mid, r);
            addRealEdge(id, L[id]);
            addRealEdge(id, R[id]);
            // trace("->", l, r, id);
            return id;
        } else {
            // trace("->", l, r, l);
            return l;
        }
    }

    int buildSegTreeRev(int l, int r) {
        // trace("l:",l, "r:",r);
        if (l != r) {
            int id = addRealVertex();
            // trace(id);
            int mid = (l+r)/2;
            assert(id >= 0 && id < sz(L));
            int x = buildSegTreeRev(l, mid);
            assert(id >= 0 && id < sz(R));
            int y = buildSegTreeRev(mid+1, r);
            L[id] = x;
            R[id] = y;
            // trace("teste",id, L[id], R[id], l, mid, r, x);
            addRealEdge(L[id], id);
            addRealEdge(R[id], id);
            // trace("->", l, r, id);
            return id;
        } else {
            // trace("->", l, r, l);
            return l;
        }
    }

    void build(int _n) {
        n = _n;
        for(int i = 0; i < n; i++) {
            addRealVertex();
            used[i] = true;
        }
        root_1 = buildSegTree(0, n-1);
        root_2 = buildSegTreeRev(0, n-1);
    }

    void addEdgeIn(int id, int l, int r, int i, int j, int aux_id) {
        if (l > r || l > j || r < i) return;
        if (l >= i && r <= j) {
            addRealEdge(id, aux_id);
            used[id] = true;
            used[aux_id] = true;
        } else {
            int mid = (l+r)/2;
            addEdgeIn(L[id], l, mid, i, j, aux_id);
            addEdgeIn(R[id], mid+1, r, i, j, aux_id);
        }
    }

    void addEdgeOut(int id, int l, int r, int i, int j, int aux_id) {
        if (l > r || l > j || r < i) return;
        if (l >= i && r <= j) {
            addRealEdge(aux_id, id);
            used[id] = true;
            used[aux_id] = true;
        } else {
            int mid = (l+r)/2;
            addEdgeOut(L[id], l, mid, i, j, aux_id);
            addEdgeOut(R[id], mid+1, r, i, j, aux_id);
        }
    }

    void addEdgeIn2(int id, int l, int r, int i, int j, int x, int y) {
        if (l > r || l > j || r < i) return;
        if (l >= i && r <= j) {
            addEdgeOut(root_1, 0, n-1, x, y, id);
        } else {
            int mid = (l+r)/2;
            addEdgeIn2(L[id], l, mid, i, j, x, y);
            addEdgeIn2(R[id], mid+1, r, i, j, x, y);
        }
    }

    int countEdges(int l, int r, int i, int j) {
        if (l > r || l > j || r < i) return 0;
        if (l >= i && r <= j) {
            return 1;
        } else {
            int mid = (l+r)/2;
            return countEdges(l, mid, i, j) + countEdges(mid+1, r, i, j);
        }
    }

    void addEdge(int l_in, int r_in, int l_out, int r_out) {
        m += (r_in - l_in + 1) * (r_out - l_out + 1);
        
        
        int countEdges1 = countEdges(0, n-1, l_in, r_in);
        int countEdges2 = countEdges(0, n-1, l_out, r_out);
        int cost1 = countEdges1*countEdges2;
        int cost2 = countEdges1+countEdges2+1;
        if (cost1 <= cost2) {
            addEdgeIn2(root_2, 0, n-1, l_in, r_in, l_out, r_out);
        } else {
            int id = addRealVertex();
            addEdgeIn(root_2, 0, n-1, l_in, r_in, id);
            addEdgeOut(root_1, 0, n-1, l_out, r_out, id);
        }
    }

    void removeFromGraph(int v, int &qtd_n, int &qtd_m) {
        if (v == -1) return;
        if (!used[v]) {
            qtd_n--;
            qtd_m -= 2;
            removeFromGraph(L[v], qtd_n, qtd_m);
            removeFromGraph(R[v], qtd_n, qtd_m);
        }
    }

    // This will count just the non-ghost vertices plus the vertices and edges that are used by some interval-to-interval edges. 
    pii count_necessary_parts() {
        int ret_n = real_n, ret_m = real_m;
        removeFromGraph(root_1, ret_n, ret_m);
        removeFromGraph(root_2, ret_n, ret_m);
        return {ret_n, ret_m};
    }
};