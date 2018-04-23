/*
 * Coursera/Advanced Algorithms and Complexity/Week 2/Problem 2:
 * Optimal Diet Problem (linear programming)
 * Grading: 'Good job! (Max time used: 0.12/1.00, max memory used: 9748480/536870912.)'
 * Respect Coursera Honor Code
 * Copyright © 2018. All rights reserved
 */

#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

const long double EPS = 1e-3;
const long double INF = 1.0e+9;

using Column = std::vector<long double>;
using Row = std::vector<long double>;
using Matrix = std::vector<Row>;

struct Equation
{
  Matrix a;
  Column b;
};

struct Position
{
  short column;
  short row;
};

Position select_pivot(const Matrix &a, std::vector<bool> &used_rows, std::vector<bool> &used_columns)
{
  // first, select the first free element.
  Position pivot_element{0, 0};
  while (used_rows[pivot_element.row])
    ++pivot_element.row;
  while (used_columns[pivot_element.column])
    ++pivot_element.column;

  // then, amongs free elements, select a pivot with the largest absolute value
  long double max = 0.0;
  for (int from = pivot_element.row, size = a.size(); from < size; ++from)
  {
    if (std::fabs(max) - std::fabs(a[from][pivot_element.column]) < EPS)
    {
      max = a[from][pivot_element.column];
      pivot_element.row = from;
    }
  }

  return pivot_element;
}

inline void swap_lines(Matrix &a, Column &b, std::vector<bool> &used_rows, Position &pivot_element)
{
  std::swap(a[pivot_element.column], a[pivot_element.row]);
  std::swap(b[pivot_element.column], b[pivot_element.row]);
  std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
  pivot_element.row = pivot_element.column;
}

int back_substitution(Matrix &a, Column &b)
{
  for (int i = a.size() - 1; i; --i)
  {
    long double v = b[i];
    for (int j = 0; j != i; ++j)
    {
      b[j] -= a[j][i] * v;
    }
  }

  return 0;
}

inline void scale_pivot(Matrix &a, Column &b, const Position &pivot_element)
{
  const long double divisor = a[pivot_element.row][pivot_element.column];
  const int size = a.size();

  for (int j = pivot_element.column; j < size; ++j)
  {
    a[pivot_element.row][j] /= divisor;
  }

  b[pivot_element.row] /= divisor;
}

void process_pivot(Matrix &a, Column &b, const Position &pivot_element)
{
  const int size = a.size();
  long double multiple{0.0};

  scale_pivot(a, b, pivot_element);

  for (int i = pivot_element.row + 1; i < size; ++i)
  {
    multiple = a[i][pivot_element.column];
    for (int j = pivot_element.column; j < size; ++j)
    {
      a[i][j] -= (a[pivot_element.row][j] * multiple);
    }
    b[i] -= (b[pivot_element.row] * multiple);
  }
}

inline void mark_pivot_used(const Position &pivot, std::vector<bool> &used_rows, std::vector<bool> &used_columns)
{
  used_rows[pivot.row] = true;
  used_columns[pivot.column] = true;
}

pair<int, Column> solve_equation(Equation equation)
{
  Matrix &a = equation.a;
  Column &b = equation.b;

  if (a.empty())
  {
    return {};
  }

  std::vector<bool> used_columns(a.size(), false);
  std::vector<bool> used_rows(a.size(), false);

  for (int steps = a.size(); steps; --steps)
  {
    Position pivot_element = select_pivot(a, used_rows, used_columns);
    if (a[pivot_element.row][pivot_element.column] == 0)
    {
      break;
    }
    swap_lines(a, b, used_rows, pivot_element);
    process_pivot(a, b, pivot_element);
    mark_pivot_used(pivot_element, used_rows, used_columns);
  }

  int code = back_substitution(a, b);

  return {code, std::move(b)};
}

vector<vector<int>> get_subsets(const vector<int> &set, int m)
{
  const int n = pow(2, set.size());
  vector<vector<int>> subsets;
  std::bitset<32> bset;

  for (int i = 0; i < n; ++i)
  {
    bset = bset.to_ulong() + 1l;
    std::vector<int> subset;

    int cnt = 0;
    for (int j = 0; j < set.size(); ++j)
    {
      if (bset[set.size() - 1 - j])
      {
        subset.push_back(set[j]);
        if (++cnt > m)
        {
          break;
        }
      }
    }

    if (cnt == m)
    {
      subsets.emplace_back(std::move(subset));
    }
  }

  return subsets;
}

vector<Column> solve_all_equations(int m, const Matrix &A, const vector<long double> &b)
{
  vector<Column> solutions;
  vector<int> nums(A.size());

  int s{0};
  generate(nums.begin(), nums.end(), [&s] { return s++; });
  auto subsets = get_subsets(nums, m);

  if (A.size() == 1)
  {
    subsets.emplace_back(0);
  }

  for (const auto &sub : subsets)
  {

    Matrix mat;
    Column col;

    for (auto j : sub)
    {
      mat.push_back(A[j]);
      col.push_back(b[j]);
    }

    Equation eq{std::move(mat), std::move(col)};

    auto code_and_sol = solve_equation(eq);

    if (code_and_sol.first == 0 && !code_and_sol.second.empty())
    {
      solutions.emplace_back(std::move(code_and_sol.second));
    }
  }

  return solutions;
}

inline void prepare(int &n, const int m, Matrix &A, vector<long double> &b)
{
  // enusre matrix MxM
  while (n < m)
  {
    A.emplace_back(m, 0);
    b.emplace_back(0);
    ++n;
  }

  // add equation to check for infnity
  A.emplace_back(vector<long double>(m, 1));
  b.emplace_back(1.0e+9);
  ++n;

  // add equations to check that amount are positive
  for (int k = 0; k < m; ++k)
  {
    vector<long double> vec(m, 0.0);
    vec[k] = -1;
    A.emplace_back(std::move(vec));
    b.emplace_back(-0.0);
    ++n;
  }
}

pair<int, vector<long double>>
solve_diet_problem(int n, int m, Matrix A, vector<long double> b, vector<long double> c)
{
  prepare(n, m, A, b);
  vector<Column> solutions = solve_all_equations(m, A, b);

  if (solutions.size() == 0)
  {
    return {-1, {}}; // no solution
  }

  int sol_index = -1;
  long double largest_pleasure = -(std::numeric_limits<long double>::max() / 2);

  // check solutions
  for (int i = 0; i < solutions.size(); ++i)
  {

    auto &sol = solutions[i];
    bool satisfied{true};

    for (int j = 0; j < n; ++j)
    {

      long double constraint = b[j];
      long double sum{0.0};

      for (int k = 0; k < m; ++k)
      {
        sum += A[j][k] * sol[k];
      }

      if (sum - constraint > EPS)
      {
        satisfied = false;
        break;
      }
    }

    long double pleasure{0.0};
    for (int k = 0; k < m; ++k)
    {
      pleasure += sol[k] * c[k];
    }

    if (satisfied && pleasure > largest_pleasure)
    {
      largest_pleasure = pleasure;
      sol_index = i;
    }
  }

  if (sol_index == -1)
  {
    return {-1, {}}; // no solution
  }

  auto &sol = solutions[sol_index];
  if (std::accumulate(sol.begin(), sol.end(), (long double)0.0) + EPS >= INF)
  {
    return {1, {}}; // infinity
  }

  return {0, std::move(sol)};
}

int main()
{
  std::ios_base::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;

  Matrix A(n, vector<long double>(m));

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      cin >> A[i][j];
    }
  }

  vector<long double> b(n);
  for (int i = 0; i < n; i++)
  {
    cin >> b[i];
  }

  vector<long double> c(m);
  for (int i = 0; i < m; i++)
  {
    cin >> c[i];
  }

  auto ans = solve_diet_problem(n, m, std::move(A), std::move(b), std::move(c));

  switch (ans.first)
  {
  case -1:
    printf("No solution\n");
    break;
  case 0:
    printf("Bounded solution\n");
    for (int i = 0; i < m; i++)
    {
      printf("%.18Lf%c", ans.second[i], " \n"[i + 1 == m]);
    }
    break;
  case 1:
    printf("Infinity\n");
    break;
  }
  return 0;
}
