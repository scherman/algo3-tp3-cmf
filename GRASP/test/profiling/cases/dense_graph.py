import sys
from os import path
sys.path.insert(0, path.abspath(path.join(path.dirname(__file__), '../..')))

from tools.csv_tools import write_csv, rows_to_columns
from tools.profiler import profile_instances, run_algorithm
from tools.graph import DenseGraphBuilder
from utils import *
from itertools import chain, imap


def generator(min_n, max_n, density, cache):
    cached_results = cache_try_load(cache)

    def g():
        for n in range(min_n, max_n + 1):
            if str(n) in cached_results:
                yield (n, cached_results[str(n)])
                continue

            m = n * (n - 1) / 2
            m *= density
            graph = DenseGraphBuilder() \
                .with_n(n) \
                .with_m(int(m)) \
                .build()
            result = (n, stringify(graph))
            cache_save(cache, result)

            yield result

    return g


def extractor(output):
    return output.split()[0]


def calculate_results(min_n, max_n, density, precision, filename):
    results = profile_instances(generator(min_n, max_n, density, filename), precision=precision, runner='grasp', extractor=extractor)

    write_csv("""../results/{filename}.csv""".format(filename=filename),
        chain(
            [['n', 'result', 'time']],
            imap(lambda (iteration, result, time): [iteration, result, time], results)
        )
    )

calculate_results(1, 24, 1.0, 20, 'complete_graph_grasp')
calculate_results(1, 24, 0.8, 20, 'very_dense_graph_grasp')
calculate_results(1, 24, 0.6, 20, 'dense_graph_grasp')
calculate_results(1, 24, 0.4, 20, 'sparse_graph_grasp')