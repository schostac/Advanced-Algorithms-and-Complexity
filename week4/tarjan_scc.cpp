#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

struct Tarjan_SCCs
{

  Tarjan_SCCs(vector<vector<int>> adj_lst_graph)
      : graph{std::move(adj_lst_graph)},
        index{vector<int>(graph.size(), -1)},
        lowlink{vector<int>(graph.size(), -1)},
        on_stack{vector<bool>(graph.size(), false)}
  {
  }

  void process_vertex(int v)
  {
    vector<int> scc;

    index[v] = lowlink[v] = t++;
    S.push(v);
    on_stack[v] = true;

    for (auto w : graph[v])
    {
      if (index[w] == -1)
      {
        process_vertex(w);
        lowlink[v] = std::min(lowlink[v], lowlink[w]);
      }
      else if (on_stack[w])
      {
        lowlink[v] = std::min(lowlink[v], index[w]);
      }
    }

    if (lowlink[v] == index[v])
    {
      int w = 0;
      do
      {
        w = S.top();
        S.pop();
        on_stack[w] = false;
        scc.push_back(w + 1);
      } while (w != v);
    }

    if (!scc.empty())
    {
      scc_set.emplace_back(std::move(scc));
    }
  }

  vector<vector<int>> topolog_SCCs_sets()
  {
    for (int i = 0; i < graph.size(); ++i)
    {
      if (index[i] == -1)
      {
        process_vertex(i);
      }
    }

    return std::move(scc_set);
  }

  int t{0};

  vector<vector<int>> graph, scc_set;
  vector<int> index, lowlink;
  vector<bool> on_stack;
  stack<int> S;
};

int main()
{
  size_t n, m;
  std::cin >> n >> m;

  vector<vector<int>> adj_lst_graph(n);

  for (size_t i = 0; i < m; ++i)
  {
    int x, y;
    std::cin >> x >> y;
    adj_lst_graph[x - 1].emplace_back(y - 1);
  }

  Tarjan_SCCs tscc(std::move(adj_lst_graph));
  auto scc_set = tscc.topolog_SCCs_sets();

  for (auto &s : scc_set)
  {
    for (auto v : s)
    {
      cout << v << ' ';
    }
    cout << endl;
  }
}
