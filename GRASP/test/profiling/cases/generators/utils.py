import os
import sys
from os import path
sys.path.insert(0, path.abspath(path.join(path.dirname(__file__), '../../..')))

from tools.csv_tools import write_csv, rows_to_columns
from tools.profiler import profile_instances, run_algorithm
from itertools import chain, imap
from ast import literal_eval


def lines(graph):
    n = graph.num_vertices()
    m = graph.num_edges()
    yield """{n} {m}""".format(n=n, m=m)

    for edge in graph.edges():
        v1 = graph.vertex_index[edge.source()]
        v2 = graph.vertex_index[edge.target()]
        yield """{v1} {v2}""".format(v1=v1, v2=v2)


def stringify(graph):
    return '\n'.join(lines(graph))


def cache_try_load(key):
    instances = dict()

    if not os.path.exists(os.path.join('./.cache', '_' + key)):
        return instances

    with open(os.path.join('./.cache', '_' + key), 'r') as cache:
        for line in cache:
            instance_parts = line.split('\'')
            instance_key = instance_parts[0].strip()
            instance_content = literal_eval('\'' + instance_parts[1] + '\'')
            instance_args = instance_parts[2].split(' ')
            instances[instance_key] = (instance_key, [instance_content] + instance_args)

    return instances


def cache_save(key, instance):
    if not os.path.exists('./.cache'):
        os.makedirs('./.cache')

    with open(os.path.join('./.cache', '_' + key), 'a') as cache:
        cache.write("""{key} {content} {args}\n""".format(
            key=instance[0], content=repr(instance[1][0]), args=' '.join(instance[1][1:])))

    with open(os.path.join('./.cache', '_' + key + '.debug'), 'w') as cache:
        cache.write(instance[1][0])


def cache_wrapper(instances, generate, case_name):
    cached_results = cache_try_load(case_name)

    def g():
        for instance_key in instances:
            if str(instance_key) in cached_results:
                yield cached_results[str(instance_key)]
                continue

            instance = generate(instance_key)
            cache_save(case_name, instance)
            yield instance

    return g


def extractor(output):
    return int(output.split()[0])


def calculate_results(variable, instances, generate, runner, precision, case_name):
    results = profile_instances(
        cache_wrapper(instances, generate, case_name),
        precision=precision,
        runner=runner,
        extractor=extractor
    )

    write_csv(
        """../results/{filename}.csv""".format(filename=case_name),
        chain(
            [[variable, 'result', 'time']],
            imap(lambda (iteration, result, time): [iteration, result, time], results)
        )
    )