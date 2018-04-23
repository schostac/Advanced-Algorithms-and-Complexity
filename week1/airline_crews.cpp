/*
 * Coursera/Advanced Algorithms and Complexity/Week 1/Problem 2:
 * Assigning Airline Crews to Flights (maximum bipartite matching)
 * Grading: 'Good job! (Max time used: 0.01/1.00, max memory used: 9134080/536870912.)'
 * Respect Coursera Honor Code
 * Copyright © 2018. All rights reserved
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdio.h>

using std::numeric_limits;
using std::queue;
using std::size_t;
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

  const size_t flights;

public:
  explicit FlowGraph(size_t n, size_t m, size_t f)
      : graph(n), flights(f)
  {
    edges.reserve(m * 2);
  }

  inline void add_edge(int from, int to, int capacity)
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

  inline size_t size() const
  {
    return graph.size();
  }

  inline size_t get_flights() const
  {
    return flights;
  }

  inline const vector<size_t> &get_ids(int from) const
  {
    return graph[from];
  }

  inline const Edge &get_edge(size_t id) const
  {
    return edges[id];
  }

  inline void add_flow(size_t id, int flow)
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
  int n, m;
  std::cin >> n >> m;

  FlowGraph graph(n + m + 2, m + n + 2, n);

  /* Construct FlowGraph from data given for bipartite graph
     * by adding source and sink.
     */

  /* Edges from vertices on the left of bipartite graph to the source */
  for (int i = 0; i < n; ++i)
  {
    graph.add_edge(0, i + 1, 1);
  }

  /* Edges of verteces from left to right of bipartite graph */
  for (int i = 1; i <= n; ++i)
  {
    for (int j = 0; j < m; ++j)
    {
      int bit;
      std::cin >> bit;
      if (bit == 1)
      {
        graph.add_edge(i, n + j + 1, 1);
      }
    }
  }

  /* Edges from vertices on the right of bipartite graph to the sink */
  for (int i = n + 1; i <= m + n; ++i)
  {
    graph.add_edge(i, n + m + 1, 1);
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

void print_MBM(FlowGraph &graph, const size_t flights)
{
  for (int i = 0; i < flights; ++i)
  {
    int job = -1;
    for (auto id : graph.get_ids(i + 1))
    {
      const FlowGraph::Edge &e = graph.get_edge(id);
      if (e.flow == 1)
      {
        job = e.to - flights;
        break;
      }
    }
    printf("%d ", job);
  }
  printf("%s", "\n");
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  FlowGraph graph = read_data();

  max_flow(graph, 0, graph.size() - 1);
  print_MBM(graph, graph.get_flights());
  return 0;
}
