from functools import partial
from generators.dense_graph import generate_instance
from generators.utils import calculate_results


def generate(key):
    return key, [generate_instance(key, density=0.5), '--search', '1']

calculate_results(
    variable='n',
    instances=[n for n in range(1, 24 + 1)],
    generate=generate,
    runner='grasp',
    precision=20,
    case_name='example'
)
