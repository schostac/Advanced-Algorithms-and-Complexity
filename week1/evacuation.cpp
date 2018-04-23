/*
 * Coursera/Advanced Algorithms and Complexity/Week 1/Problem 1:
 * Evacuating People (Edmonds–Karp algorithm for computing maximum flow)
 * Grading: 'Good job! (Max time used: 0.02/1.00, max memory used: 9236480/536870912.)'
 * Respect Coursera Honor Code
 * Copyright © 2018. All rights reserved
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using std::numeric_limits;
using std::queue;
using std::vector;

/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */
class FlowGraph
{
public:
  struct Edge
  {
    int from, to, capacity, flow;
  };

private:
  /* List of all - forward and backward - edges */
  vector<Edge> edges;

  /* These adjacency lists store only indices of edges in the edges list */
  vector<vector<size_t>> graph;

public:
  explicit FlowGraph(size_t n, size_t m)
      : graph(n)
  {
    edges.reserve(m * 2);
  }

  void add_edge(int from, int to, int capacity)
  {
    /* Note that we first append a forward edge and then a backward edge,
         * so all forward edges are stored at even indices (starting from 0),
         * whereas backward edges are stored at odd indices in the list edges */
    Edge forward_edge = {from, to, capacity, 0};
    Edge backward_edge = {to, from, 0, 0};
    graph[from].push_back(edges.size());
    edges.push_back(forward_edge);
    graph[to].push_back(edges.size());
    edges.push_back(backward_edge);
  }

  size_t size() const
  {
    return graph.size();
  }

  const vector<size_t> &get_ids(int from) const
  {
    return graph[from];
  }

  const Edge &get_edge(size_t id) const
  {
    return edges[id];
  }

  void add_flow(size_t id, int flow)
  {
    /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
         * due to the described above scheme. On the other hand, when we have to get a "backward"
         * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
         * should be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through! */
    edges[id].flow += flow;
    edges[id ^ 1].flow -= flow;
  }
};

FlowGraph read_data()
{
  int vertex_count, edge_count;
  std::cin >> vertex_count >> edge_count;

  FlowGraph graph(vertex_count, edge_count);

  for (int i = 0; i < edge_count; ++i)
  {
    int u, v, capacity;
    std::cin >> u >> v >> capacity;
    graph.add_edge(u - 1, v - 1, capacity);
  }

  return graph;
}

void BFS(const FlowGraph &graph, int s, int t, vector<int> &pred)
{
  queue<int> q;
  q.push(s);

  std::fill(pred.begin(), pred.end(), -1);

  while (!q.empty())
  {

    int cur = q.front();
    q.pop();

    for (auto id : graph.get_ids(cur))
    {

      const FlowGraph::Edge &e = graph.get_edge(id);

      if (pred[e.to] == -1 && e.capacity > e.flow && e.to != s)
      {
        pred[e.to] = id;
        q.push(e.to);
      }
    }
  }
}

int max_flow(FlowGraph &graph, int s, int t)
{
  int flow = 0;

  /* Contains predecessors of a vertex to get
     * the path and calculate minimum flow thereon. */
  vector<int> pred(graph.size());

  do
  {

    BFS(graph, s, t, pred);

    if (pred[t] != -1)
    {
      int min_flow = numeric_limits<int>::max();

      /* Find minimal flow on the path from BFS. */
      for (int u = pred[t]; u != -1; u = pred[graph.get_edge(u).from])
      {
        min_flow = std::min(min_flow, graph.get_edge(u).capacity - graph.get_edge(u).flow);
      }

      /* Update flow in original and residual graphs along the path from BFS*/
      for (int u = pred[t]; u != -1; u = pred[graph.get_edge(u).from])
      {
        graph.add_flow(u, min_flow);
      }

      flow += min_flow;
    }

  } while (pred[t] != -1);

  return flow;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  FlowGraph graph = read_data();

  std::cout << max_flow(graph, 0, graph.size() - 1) << "\n";
  return 0;
}
