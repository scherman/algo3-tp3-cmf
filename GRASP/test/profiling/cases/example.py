import sys
import inspect
from os import path
sys.path.insert(0, path.abspath(path.join(path.dirname(__file__), '../..')))

from tools.csv_tools import write_csv, rows_to_columns
from tools.profiler import profile_instances, run_algorithm
from tools.graph import DenseGraphBuilder
from utils import *

def generator():
    def g():
        graph = DenseGraphBuilder() \
            .with_n(3) \
            .with_m(1) \
            .build()
        result = (1, stringify(graph))
        yield result

    return g

print run_algorithm(generator()().next()[1], 'exact')
