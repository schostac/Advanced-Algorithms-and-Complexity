/*
 * Coursera/Advanced Algorithms and Complexity/Week 3/Problem 3 (Advanced):
 * Advertisement Budget Allocation (convert Integer Linear Programming to SAT)
 * Grading: 'Good job! (Max time used: 0.02/2.00, max memory used: 13828096/536870912.)'
 * Respect Coursera Honor Code
 * Copyright © 2018. All rights reserved
 */

#include <ios>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <bitset>
#include <cmath>
#include <cassert>

using namespace std;

struct ConvertILPToSat
{
  vector<vector<int>> A;
  vector<int> b;

  ConvertILPToSat(int n, int m) : A(n, vector<int>(m)), b(n) {}

  void printEquisatisfiableSatFormula()
  {
    int cnt = 0;
    for (int i = 0; i < A.size(); ++i)
    {
      const auto &row = A[i];

      int n = std::count_if(row.begin(), row.cend(), [](const auto a) { return a != 0; });

      for (int j = 0, s = pow(2, n); j < s; ++j)
      {
        combinations = j;

        int sum = 0, comb = 0;
        for (const auto x : row)
        {
          if (x != 0 && combinations[comb++])
          {
            sum += x;
          }
        }

        if (sum > b[i])
        {
          bool is_clause = false;
          for (int k = 0, comb = 0; k < row.size(); ++k)
          {
            if (row[k] != 0)
            {
              clauses_stream += combinations[comb] ? (std::to_string(-(k + 1)) + ' ') : (std::to_string(k + 1) + ' ');
              ++comb;
              is_clause = true;
            }
          }
          if (is_clause)
          {
            clauses_stream += "0 \n";
            ++cnt;
          }
        }
      }
    }

    if (cnt == 0)
    {
      cnt++;
      clauses_stream += "1 -1 0\n";
    }
    cout << cnt << " " << (A[0].size()) << endl;
    cout << clauses_stream.c_str();
  }

  std::bitset<3> combinations;
  std::string clauses_stream;
};

int main()
{
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  ConvertILPToSat converter(n, m);
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      cin >> converter.A[i][j];
    }
  }
  for (int i = 0; i < n; i++)
  {
    cin >> converter.b[i];
  }

  converter.printEquisatisfiableSatFormula();

  return 0;
}
