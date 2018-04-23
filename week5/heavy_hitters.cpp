/*
 * Coursera/Advanced Algorithms and Complexity/Week 5 (optional)/Problem 1:
 * Nice vs Naughty (Heavy Hitters Problem using Count Sketch streaming algorithm)
 * Grading: 'Good job! (Max time used: 0.99/4.00, max memory used: 1609728/10485760.)'
 * Respect Coursera Honor Code
 * Copyright © 2018. All rights reserved
 */

/*
 * Useful links:
 * - https://stackoverflow.com/questions/6811351/explaining-the-count-sketch-algorithm
 * - https://en.wikipedia.org/wiki/Universal_hashing
 * - https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch
 * - https://www.cse.iitb.ac.in/~nutan/courses/cs602-13/notes/lec6.pdf
 */

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <random>

struct hash_function
{
  long operator()(long x) const noexcept { return ((a * x + b) % p) % m; }

  long a, b, p, m;
};

struct sign_function
{
  static std::default_random_engine gen;
  static std::uniform_int_distribution<int> dist;

  sign_function() noexcept : hf{dist(gen), dist(gen), 10000019, 1000} {}

  int operator()(long x) const noexcept { return hf(x) > 500 ? 1 : -1; }

  hash_function hf;
};

std::default_random_engine sign_function::gen = std::default_random_engine();
std::uniform_int_distribution<int> sign_function::dist = std::uniform_int_distribution<int>(10, 100);

struct funcs
{
  hash_function hf;
  sign_function sf;
};

struct count_sketch
{
  count_sketch(long n) noexcept : b{(int)log(n) * 50},
                                  t{(int)log(n) + 1},
                                  data{std::vector<std::vector<long>>(t, std::vector<long>(b))},
                                  ith_data{std::vector<long>(data.size())}
  {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> dist(3, 50);

    long p = 10e8 + 19;
    functions.reserve(data.size());

    for (auto i = 0; i < t; ++i)
    {
      functions.push_back(funcs{hash_function{dist(gen), dist(gen), p, b}, sign_function()});
    }
  }

  void update(long i, long frq = 1) noexcept
  {
    for (auto j = 0u; j < data.size(); ++j)
    {
      data[j][functions[j].hf(i)] += (functions[j].sf(i) * frq);
    }
  }

  long estimate(long i) noexcept
  {
    for (auto j = 0u; j < data.size(); ++j)
    {
      ith_data[j] = data[j][functions[j].hf(i)] * functions[j].sf(i);
    }

    return median();
  }

private:
  long median() noexcept
  {
    std::nth_element(ith_data.begin(), ith_data.begin() + ith_data.size() / 2, ith_data.end());
    return *std::next(ith_data.begin(), ith_data.size() / 2);
  }

  const int b, t;

  /* b - number of buckets; large enough to reduce variance, i.e. estimation error,
     *     or as large as posssible given memory constraints
     * t - number of hash functions; log(n) + 1
     */

  std::vector<funcs> functions;
  std::vector<std::vector<long>> data;
  std::vector<long> ith_data;
};

int main()
{
  long n, t, id, value;
  scanf("%ld %ld", &n, &t);

  count_sketch algo(n);

  for (long i = 0; i < n; ++i)
  {
    scanf("%ld %ld", &id, &value);
    algo.update(id, value);
  }

  for (long i = 0; i < n; ++i)
  {
    scanf("%ld %ld", &id, &value);
    algo.update(id, -value);
  }

  int num_queries = 0;
  scanf("%d", &num_queries);

  for (int q = 0, query = 0; q < num_queries; ++q)
  {
    scanf("%d", &query);
    printf("%c ", (algo.estimate(query) >= t) ? '1' : '0');
  }

  printf("\n");

  return 0;
}
