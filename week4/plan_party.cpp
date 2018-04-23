/*
 * Coursera/Advanced Algorithms and Complexity/Week 4/Problem 2:
 * Plan a Fun Party (find maximum independent set in tree using dinamic programming)
 * Grading: 'Good job! (Max time used: 0.07/1.00, max memory used: 22724608/536870912.)'
 * Respect Coursera Honor Code
 * Copyright © 2018. All rights reserved
 */

#include <iostream>
#include <vector>

struct vertex
{
  int weight;
  std::vector<int> children;
};

using graph = std::vector<vertex>;
using sum = std::vector<int>;

graph read_tree() noexcept
{
  std::ios::sync_with_stdio(false);

  int vertices_count;
  std::cin >> vertices_count;

  graph tree(vertices_count);

  for (int i = 0; i < vertices_count; ++i)
  {
    std::cin >> tree[i].weight;
  }

  for (int i = 1, from, to; i < vertices_count; ++i)
  {
    std::cin >> from >> to;
    tree[from - 1].children.emplace_back(to - 1);
    tree[to - 1].children.emplace_back(from - 1);
  }

  return tree;
}

int compute_recursively(const graph &tree, const int vertex, sum &s, const int parent) noexcept
{
  if (s[vertex] == -1)
  {
    if (tree[vertex].children.empty())
    {
      s[vertex] = tree[vertex].weight;
    }
    else
    {
      auto m1 = tree[vertex].weight;
      for (auto u : tree[vertex].children)
      {
        if (u == parent)
          continue;
        for (auto w : tree[u].children)
        {
          if (w != vertex)
          {
            m1 += compute_recursively(tree, w, s, u);
          }
        }
      }
      auto m0 = 0;
      for (auto u : tree[vertex].children)
      {
        if (u != parent)
        {
          m0 += compute_recursively(tree, u, s, vertex);
        }
      }

      s[vertex] = std::max(m1, m0);
    }
  }

  return s[vertex];
}

int compute_max_independent_set(const graph &tree) noexcept
{
  size_t size = tree.size();
  if (size == 0)
    return 0;

  auto s = sum(size, -1);

  return compute_recursively(tree, 0, s, -1);
}

int main()
{
  const graph tree = read_tree();
  std::cout << compute_max_independent_set(tree) << std::endl;
  return 0;
}
