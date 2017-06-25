import sys
import inspect
from os import path
sys.path.insert(0, path.abspath(path.join(path.dirname(__file__), '../..')))

from tools.csv_tools import write_csv, rows_to_columns
from tools.profiler import profile_instances, run_algorithm
from tools.graph import DenseGraphBuilder
from utils import *

def generator():
    from_cache = cache_try_load('_example')

    def g():
        graph = DenseGraphBuilder() \
            .with_n(20) \
            .with_m(190) \
            .build()
        result = (1, stringify(graph))
        cache_save('_example', result)
        yield result

    return from_cache if from_cache is not None else g

print run_algorithm(generator()().next()[1], 'exact')
