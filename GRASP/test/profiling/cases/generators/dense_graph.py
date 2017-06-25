import sys
from os import path
sys.path.insert(0, path.abspath(path.join(path.dirname(__file__), '../../..')))

from tools.graph import DenseGraphBuilder
from utils import stringify


def generate_instance(n, density):
    m = n * (n - 1) / 2
    m *= density
    graph = DenseGraphBuilder() \
        .with_n(n) \
        .with_m(int(m)) \
        .build()

    return stringify(graph)
