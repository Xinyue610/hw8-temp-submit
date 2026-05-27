#include "graph.h"

void error(string msg)
{
    cerr << "Error: " << msg << endl;
}

// DisjointSet
DisjointSet::DisjointSet(int numVertices)
{
    subsets.resize(numVertices);

    for (Vertex v = 0; v < numVertices; ++v)
        makeSet(v);
}

void DisjointSet::makeSet(Vertex x)
{
    subsets[x].parent = x;
}

Vertex DisjointSet::findSet(Vertex x){
	if (subsets[x].parent != x)
        subsets[x].parent = findSet(subsets[x].parent);
	return subsets[x].parent;
}
void DisjointSet::unionSets(Vertex x, Vertex y){
	Vertex rootX = findSet(x);
    Vertex rootY = findSet(y);

    if (rootX != rootY)
        subsets[rootY].parent = rootX;
}
// Graph
Graph Graph::sort_edges() const{
	Graph sortedGraph = *this;
    sort(sortedGraph.begin(), sortedGraph.end(),
         [](const Edge& a, const Edge& b)
         {
             return a.weight < b.weight;
         });

    return sortedGraph;
}
VertexList Graph::edges_from(Vertex vertex) const{
	VertexList neighbors;

    for (const Edge& e : *this){
		if (e.u == vertex){
			neighbors.push_back(e.v);
		}
    }
    return neighbors;
}

// Kruskal's MST
EdgeList Kruskals(const Graph& G){
	EdgeList mst;
    Graph sortedGraph = G.sort_edges();
    DisjointSet ds(G.numVertices);

    for (const Edge& e : sortedGraph)
    {
        Vertex rootU = ds.findSet(e.u);
        Vertex rootV = ds.findSet(e.v);

        if (rootU != rootV)
        {
            mst.push_back(e);
            ds.unionSets(e.u, e.v);
        }
    }

    return mst;
}
int sum_weights(EdgeList const& L){
	int total = 0;
	for (const Edge& e : L)
        total += e.weight;

    return total;
}

// input
void file_to_graph(string filename, Graph& G){
	ifstream in(filename);
	if (!in)
        error("unable to open file " + filename);

    in >> G;
}
// DFS
VertexList dfs(const Graph& graph, Vertex startVertex){
	VertexList result;
    vector<bool> visited(graph.numVertices, false);
    stack<Vertex> s;

    visited[startVertex] = true;
    s.push(startVertex);

    while (!s.empty()){
		Vertex v = s.top();
        s.pop();
		result.push_back(v);
		VertexList neighbors = graph.edges_from(v);
		for (Vertex w : neighbors){
			if (!visited[w]){
				visited[w] = true;
                s.push(w);
			}
		}
	}
	return result;
}
//BFS
VertexList bfs(const Graph& graph, Vertex startVertex){
	VertexList result;
    vector<bool> visited(graph.numVertices, false);
    queue<Vertex> q;
	visited[startVertex] = true;
    q.push(startVertex);

	while(!q.empty()){
		Vertex v = q.front();
        q.pop();
		result.push_back(v);
		VertexList neighbors = graph.edges_from(v);
		for (Vertex w : neighbors){
			if (!visited[w]){
				visited[w] = true;
                q.push(w);
            }
        }
    }
	return result;
}

string get_arg(int argc, char* argv[], string def){
    if (argc > 1)
        return argv[1];

    return def;
}
