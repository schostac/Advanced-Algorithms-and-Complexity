# python3

# Coursera/Advanced Algorithms and Complexity/Week 3/Problem 1:
# Assign Frequencies to the Cells of a GSM Network (reduce graph 3 coloring to SAT)
# Grading: 'Good job! (Max time used: 0.05/5.00, max memory used: 8990720/1073741824.)'
# Respect Coursera Honor Code
# Copyright © 2018. All rights reserved

# Useful links: https://research.ijcaonline.org/encc/number1/encc004.pdf

n, m = map(int, input().split())
edges = [list(map(int, input().split())) for i in range(m)]
K = 3


def print_SAT_formula():

    C, V, cnt = K*len(edges) + n, n*K, 1
    print("{0} {1}".format(C, V))

    for i in range(1, n+1):
        print("{0} {1} {2} 0".format(cnt, cnt+1, cnt+2))
        cnt += 3

    for edge in edges:
        print("{0} {1} 0".format(-((edge[0]-1)*K+1), -((edge[1]-1)*K+1)))
        print("{0} {1} 0".format(-((edge[0]-1)*K+2), -((edge[1]-1)*K+2)))
        print("{0} {1} 0".format(-((edge[0]-1)*K+3), -((edge[1]-1)*K+3)))


print_SAT_formula()
