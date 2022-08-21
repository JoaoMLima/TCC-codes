#include "max_independent_set.h"


enum vertex_type
{
        FREE,
        BLOCKED,
        CONCAVE_LU, // concave that can connect to left and up
        CONCAVE_LD, // concave that can connect to left and down 
        CONCAVE_RU, // concave that can connect to right and up
        CONCAVE_RD, // concave that can connect to right and down
};



vertex_type get_type(int i,  int j, vector<vb> &g) {
    if (i == 0 || j == 0 || i == sz(g) || j == sz(g)) return BLOCKED;
    int qtdblocked = 0;
    if (!g[i-1][j-1]) qtdblocked++;
    if (!g[i-1][j]) qtdblocked++;
    if (!g[i][j-1]) qtdblocked++;
    if (!g[i][j]) qtdblocked++;

    if (qtdblocked == 0) return FREE;
    else if (qtdblocked == 1) {
        if (!g[i-1][j-1]) return CONCAVE_RD;
        if (!g[i-1][j]) return CONCAVE_LD;
        if (!g[i][j-1]) return CONCAVE_RU;
        else return CONCAVE_LU;
    } else return BLOCKED;
}

bool cross(pair<pii, pii> horizontal, pair<pii, pii> vertical) {
    int i = horizontal.first.first, j = vertical.first.second;
    if (i >= vertical.first.first && i <= vertical.second.first && j >= horizontal.first.second && j <= horizontal.second.second) {
        return true;
    }
    return false;
}


/*
 * Returns a list of edges from interval to interval from a graph.
 * Graph "g" should be a graph represented as a adjacency matrix
 */
vector<pair<pii, pii>> graph_compression(vector<vb> g) {

    // can i go to down or right given some position?
    vector<vector<pair<bool, bool>>> is_rectangle_edge(sz(g), vector<pair<bool, bool>>(sz(g)));
    vector<pair<pii, pii>> rectangles;




    // Here, vertex_types is referring to the type of the vertices on the polygonal representation of the adjacency matrix.
    vector<vector<vertex_type>> vertex_types(sz(g)+1, vector<vertex_type>(sz(g)+1));
    vector<pair<pii, pii>> horizontals;
    vector<pair<pii, pii>> verticals;

    rep(i, 0, sz(g)) {
        rep(j, 0, sz(g)) {
            if (i == sz(g)-1 || !g[i+1][j]) {
                is_rectangle_edge[i][j].first = true;
            } else {
                is_rectangle_edge[i][j].first = false;
            }

            if (j == sz(g)-1 || !g[i][j+1]) {
                is_rectangle_edge[i][j].second = true;
            } else {
                is_rectangle_edge[i][j].second = false;
            }
        }
    }

    rep(i, 0, sz(g)+1) {
        rep(j, 0, sz(g)+1) {
            vertex_types[i][j] = get_type(i, j, g);
            int k;
            if (vertex_types[i][j] == CONCAVE_LU) {
                k = i-1; //test up
                while(vertex_types[k][j] == FREE) k--;
                if (vertex_types[k][j] == CONCAVE_LD || vertex_types[k][j] == CONCAVE_RD) {
                    verticals.push_back({{k, j}, {i, j}});
                }

                k = j-1; // test left
                while(vertex_types[i][k] == FREE) k--;
                if (vertex_types[i][k] == CONCAVE_RU || vertex_types[i][k] == CONCAVE_RD) {
                    horizontals.push_back({{i, k}, {i, j}});
                }
            } else if (vertex_types[i][j] == CONCAVE_LD) {
                // k = i+1; //test down
                // while(vertex_types[k][j] == FREE) k++;
                // if (vertex_types[k][j] == CONCAVE_LU || vertex_types[k][j] == CONCAVE_RU) {
                //     verticals.push_back({{i, j}, {k, j}});
                // }

                k = j-1; // test left
                while(vertex_types[i][k] == FREE) k--;
                if (vertex_types[i][k] == CONCAVE_RU || vertex_types[i][k] == CONCAVE_RD) {
                    horizontals.push_back({{i, k}, {i, j}});
                }
            } else if (vertex_types[i][j] == CONCAVE_RU) {
                k = i-1; //test up
                while(vertex_types[k][j] == FREE) k--;
                if (vertex_types[k][j] == CONCAVE_LD || vertex_types[k][j] == CONCAVE_RD) {
                    verticals.push_back({{k, j}, {i, j}});
                }

                // k = j+1; // test right
                // while(vertex_types[i][k] == FREE) k++;
                // if (vertex_types[i][k] == CONCAVE_LU || vertex_types[i][k] == CONCAVE_LD) {
                //     horizontals.push_back({{i, j}, {i, k}});
                // }
            } else if (vertex_types[i][j] == CONCAVE_RD) {
                // k = i+1; //test down
                // while(vertex_types[k][j] == FREE) k++;
                // if (vertex_types[k][j] == CONCAVE_LU || vertex_types[k][j] == CONCAVE_RU) {
                //     verticals.push_back({{i, j}, {k, j}});
                // }

                // k = j+1; // test right
                // while(vertex_types[i][k] == FREE) k++;
                // if (vertex_types[i][k] == CONCAVE_LU || vertex_types[i][k] == CONCAVE_LD) {
                //     horizontals.push_back({{i, j}, {i, k}});
                // }
            }
        }
    }
    trace("Encontradas", sz(horizontals), "cordas horizontais!");
    trace("Encontradas", sz(verticals), "cordas verticais!");
    int edge_count = 0;
    vector<vi> bin_g(sz(horizontals)); 
    rep(i, 0, sz(horizontals)) {
        rep(j, 0, sz(verticals)) {
            if (cross(horizontals[i], verticals[j])) {
                bin_g[i].push_back(j);
                edge_count++;
            }
        }
    }
    // trace(horizontals);
    // trace(verticals);
    trace("Criada estrutura para o max independent set com", sz(horizontals)+sz(verticals), "vértices e", edge_count, "arestas!");

    vi independent_set = max_independent_set(bin_g, sz(horizontals), sz(verticals));

    trace("Max independent set rodou!");
    

    for(int chord : independent_set) {
        if (chord < sz(horizontals)) {
            auto horizontal = horizontals[chord];
            rep(i, horizontal.first.second, horizontal.second.second) {
                is_rectangle_edge[horizontal.first.first-1][i].first = true;
            }
        } else {
            auto vertical = verticals[chord-sz(horizontals)];
            rep(i, vertical.first.first, vertical.second.first) {
                is_rectangle_edge[i][vertical.first.second-1].second = true;
            }
        }
    }
    
    vector<vb> vis(sz(g), vb(sz(g)));

    rep(i, 0, sz(g)) {
        rep(j, 0, sz(g)) {
            if (g[i][j] && !vis[i][j]) {
                int k = i, l = j;
                while(k != sz(g)-1 && !vis[k+1][j] && !is_rectangle_edge[k][j].first) k++;
                while(l != sz(g)-1 && !vis[i][l+1] && !is_rectangle_edge[i][l].second) l++;
                rectangles.push_back({{i, k}, {j, l}});
                rep(x, i, k+1) {
                    rep(y, j, l+1) {
                        vis[x][y] = true;
                    }
                }
            }
        }
    }

    return rectangles;
}