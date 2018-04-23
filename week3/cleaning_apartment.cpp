/*
 * Coursera/Advanced Algorithms and Complexity/Week 3/Problem 2:
 * Cleaning the Apartment (reduce Hamiltonian path to SAT)
 * Grading: 'Good job! (Max time used: 0.03/2.00, max memory used: 9904128/536870912.)'
 * Respect Coursera Honor Code
 * Copyright © 2018. All rights reserved
 */

/*
 * Useful links: https://www.csie.ntu.edu.tw/~lyuu/complexity/2011/20111018.pdf
 */

#include <iostream>
#include <vector>
#include <cstdio>

using std::cin;
using std::ios;
using std::vector;

struct Hamiltonian_path_to_SAT
{

  Hamiltonian_path_to_SAT(int n, int m)
      : clauses_num(0), vertices_num(n), matrix(n, vector<bool>(n, false)), data(n, vector<int>(n))
  {
    for (int i = 0, cnt = 0; i < vertices_num; ++i)
    {
      for (int j = 0; j < vertices_num; ++j)
      {
        data[i][j] = ++cnt;
      }
    }
  }

  void print_SAT_formula(const int max_clauses_num) noexcept
  {
    clauses_stream.reserve(max_clauses_num * 3);

    each_vertext_in_path();
    each_vertext_in_path_only_once();
    each_path_position_occupied();
    verteces_occupy_diff_positions();
    nonadjacent_vertices_nonadjacent_in_path();

    printf("%d %d \n%s", clauses_num, vertices_num * vertices_num, clauses_stream.c_str());
  }

  void each_vertext_in_path() noexcept
  {
    for (int i = 0; i < vertices_num; ++i, ++clauses_num)
    {
      for (int j = 0; j < vertices_num; ++j)
      {
        clauses_stream += std::to_string(data[i][j]) + " ";
      }
      clauses_stream += "0\n";
    }
  }

  void each_vertext_in_path_only_once() noexcept
  {
    for (int i = 0; i < vertices_num; ++i)
    {
      for (int j = 0; j < vertices_num; ++j)
      {
        for (int k = i + 1; k < vertices_num; ++k, ++clauses_num)
        {
          clauses_stream += std::to_string(-data[i][j]) + " " + std::to_string(-data[k][j]) + " 0\n";
        }
      }
    }
  }

  void each_path_position_occupied() noexcept
  {
    for (int i = 0; i < vertices_num; ++i, ++clauses_num)
    {
      for (int j = 0; j < vertices_num; ++j)
      {
        clauses_stream += std::to_string(data[j][i]) + " ";
      }
      clauses_stream += "0\n";
    }
  }

  void verteces_occupy_diff_positions() noexcept
  {
    for (int i = 0; i < vertices_num; ++i)
    {
      for (int j = 0; j < vertices_num; ++j)
      {
        for (int k = j + 1; k < vertices_num; ++k, ++clauses_num)
        {
          clauses_stream += std::to_string(-data[i][j]) + " " + std::to_string(-data[i][k]) + " 0\n";
        }
      }
    }
  }

  void nonadjacent_vertices_nonadjacent_in_path() noexcept
  {
    for (int i = 0; i < vertices_num; ++i)
    {
      for (int j = 0; j < vertices_num; ++j)
      {
        if (!matrix[i][j] && j != i)
        {
          for (int k = 0; k < vertices_num - 1; ++k, ++clauses_num)
          {
            clauses_stream += std::to_string(-data[i][k]) + " " + std::to_string(-data[j][k + 1]) + " 0\n";
          }
        }
      }
    }
  }

  unsigned int clauses_num;
  const unsigned int vertices_num;
  vector<vector<bool>> matrix;
  vector<vector<int>> data;
  std::string clauses_stream;
};

int main()
{
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;

  Hamiltonian_path_to_SAT converter(n, m);

  for (int k = 0; k < m; ++k)
  {
    int i, j;
    cin >> i >> j;
    converter.matrix[i - 1][j - 1] = true;
    converter.matrix[j - 1][i - 1] = true;
  }

  converter.print_SAT_formula(120000);

  return 0;
}
