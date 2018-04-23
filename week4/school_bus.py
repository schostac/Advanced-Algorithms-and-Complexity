# python3

import itertools
import sys

"""
 Coursera/Advanced Algorithms and Complexity/Week 4/Problem 3:
 School Bus (Traveling Salesman Person problem by DP (Held-Karp algorithm))
 Grading: 'Good job! (Max time used: 3.16/45.00, max memory used: 123478016/536870912.)'
 Respect Coursera Honor Code
 Copyright © 2018. All rights reserved.
"""

"""
 - https://en.wikipedia.org/wiki/Held%E2%80%93Karp_algorithm
 - https://optimization.mccormick.northwestern.edu/index.php/Traveling_salesman_problems
 - https://www.youtube.com/watch?v=-JjA4BLQyqE&t=534s
"""

INF = 10 ** 9


def read_data():
    n, m = map(int, input().split())
    graph = [[INF] * n for _ in range(n)]
    for _ in range(m):
        u, v, weight = map(int, input().split())
        u -= 1
        v -= 1
        graph[u][v] = graph[v][u] = weight
    return graph


class TSP:
    def __init__(self):
        self.dists = read_data()
        self.n = len(self.dists)
        self.C = {}

    def optimal_tour(self):
        for k in range(1, self.n):
            self.C[(1 << k, k)] = (self.dists[0][k], 0)

        #  for i in range(2, 1<<(n-1)):
        #      if i and (i & (i-1)):
        #          subset = [i+1 for i, v in enumerate(bin(i)[:1:-1]) if int(v)]
        #          for k in subset:
        #              C[(i*2, k)] = min((C[(i*2 ^ (1 << k), m)][0] + dists[m][k], m) for m in subset if m and m != k)

        for size in range(2, self.n):
            for subset in itertools.combinations(range(1, self.n), size):
                bits = 0
                for bit in subset:
                    bits |= 1 << bit
                for k in subset:
                    prev = bits ^ (1 << k)
                    self.C[(bits, k)] = min((self.C[(prev, m)][0] + self.dists[m][k], m) for m in subset if m != k)

        return self.backtrack_optimal()

    def backtrack_optimal(self):
        bits = (2**self.n - 1) - 1
        opt, parent = min((self.C[(bits, k)][0] + self.dists[k][0], k) for k in range(1, self.n))
        if opt >= INF:
            return opt, []
        tour = [1]
        for i in range(self.n - 1):
            tour.append(parent+1)
            tmp = parent
            _, parent = self.C[(bits, parent)]
            bits = bits ^ (1 << tmp)
        return opt, tour

if __name__ == '__main__':
    tsp = TSP()
    result = tsp.optimal_tour()
    if result[0] >= INF:
        print(-1)
    else:
        print(result[0])
        print(*result[1])
