import sys
import inspect
from os import path
sys.path.insert(0, path.abspath(path.join(path.dirname(__file__), '../..')))

from tools.csv_tools import write_csv, rows_to_columns
from tools.profiler import profile_instances, run_algorithm
from tools.graph import DenseGraphBuilder
from utils import *


_me = 'complete_graph'


def generator(min_n, max_n):
    cached_results = cache_try_load(_me)

    def g():
        for n in range(min_n, max_n + 1):
            if str(n) in cached_results:
                yield (n, cached_results[str(n)])
                continue

            m = n * (n - 1) / 2
            graph = DenseGraphBuilder() \
                .with_n(n) \
                .with_m(m) \
                .build()
            result = (n, stringify(graph))
            cache_save(_me, result)

            yield result

    return g


def calculate_results(min_n, max_n, presicion):
    write_csv("""../results/{filename}.csv""".format(filename=_me),
        [['n', 'time']] +
        rows_to_columns([
            [n for n in range(min_n, max_n + 1)],
            profile_instances(generator(min_n, max_n), precision=presicion, runner='exact')
        ])
    )


calculate_results(5, 12, 20)
