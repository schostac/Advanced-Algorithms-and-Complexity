/*
* Coursera/Advanced Algorithms and Complexity/Week 1/Problem 3(advanced):
* Stock Charts (maximum flow and path cover, etc)
* Grading: 'Good job! (Max time used: 0.01/2.00, max memory used: 8937472/536870912.)'
* Respect Coursera Honor Code
* Copyright © 2018. All rights reserved
*/

/*
 * Useful to read: http://mradwan.github.io/algorithms/2014/05/02/flows-cuts-and-matchings/
*/

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

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

public:
  explicit FlowGraph(size_t n)
      : graph(n)
  {
    edges.reserve(n / 2);
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

vector<vector<int>> read_data(size_t num_stocks, size_t num_points)
{
  vector<vector<int>> stock_data(num_stocks, vector<int>(num_points));

  for (int i = 0; i < num_stocks; ++i)
    for (int j = 0; j < num_points; ++j)
    {
      std::cin >> stock_data[i][j];
    }

  return stock_data;
}

FlowGraph construct_graph(size_t num_stocks, size_t num_points)
{

  vector<vector<int>> stock_data = read_data(num_stocks, num_points);

  FlowGraph graph(num_stocks * 2 + 2);

  /*
    *  Create a DAG from the given charts, where each vertex corresponds to a chart, vertex V_i has
    *  an edge going to V_j if each kth point in the ith chart is less than the kth point in the jth chart,
    *  the answer then is the minimum path cover in that DAG.
    */

  /* Edges from vertices on the left of bipartite graph to the source */
  for (int i = 0; i < num_stocks; ++i)
  {
    graph.add_edge(0, i + 1, 1);
  }

  /* Edges of verteces from left to right of bipartite graph */
  for (int i = 0; i < num_stocks; ++i)
  {
    int cur_stock = i;

    for (int j = 0; j < num_stocks; ++j)
    {
      if (j == cur_stock)
      {
        continue;
      }

      bool each_less{true};

      for (int k = 0; k < num_points; ++k)
      {
        if (stock_data[i][k] >= stock_data[j][k])
        {
          each_less = false;
          break;
        }
      }

      if (each_less)
      {
        graph.add_edge(i + 1, num_stocks + j + 1, 1);
      }
    }
  }

  /* Edges from vertices on the right of bipartite graph to the sink */
  for (int i = num_stocks + 1; i <= num_stocks * 2; ++i)
  {
    graph.add_edge(i, num_stocks * 2 + 1, 1);
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

void max_flow(FlowGraph &graph, int s, int t)
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
}

int min_overlaid_charts(const FlowGraph &graph, int num_stocks)
{

  int min_path_cover{0};

  for (int i = 1; i <= num_stocks; ++i)
  {
    for (auto id : graph.get_ids(i))
    {
      const FlowGraph::Edge &e = graph.get_edge(id);
      if (e.flow > 0)
      {
        ++min_path_cover;
        break;
      }
    }
  }

  return (num_stocks - min_path_cover);
}

int main()
{
  std::ios_base::sync_with_stdio(false);

  size_t num_stocks, num_points;
  std::cin >> num_stocks >> num_points;

  FlowGraph graph = construct_graph(num_stocks, num_points);

  max_flow(graph, 0, graph.size() - 1);
  std::cout << min_overlaid_charts(graph, num_stocks) << std::endl;

  return 0;
}
