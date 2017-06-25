import os
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
            parsed_line = line.split(' ', 1)
            instance_key = parsed_line[0]
            instance_content = literal_eval(parsed_line[1])
            instances[instance_key] = instance_content

    return instances

def cache_save(key, instance):
    if not os.path.exists('./.cache'):
        os.makedirs('./.cache')

    with open(os.path.join('./.cache', '_' + key), 'a') as cache:
        cache.write("""{key} {content} \n""".format(key=instance[0], content=repr(instance[1])))

    with open(os.path.join('./.cache', '_' + key + '.debug'), 'w') as cache:
        cache.write(instance[1])
