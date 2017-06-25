from random import random
from graph_tool.all import *
from itertools import imap
from exceptions import ValueError


def _weighted_choice(weights):
    r = random()

    range_start = 0.0
    for i, weight in enumerate(weights):
        range_end = range_start + weight
        if range_start <= r < range_end:
            return i

        range_start = range_end

    raise ValueError("Not a valid weight array")


class DenseGraphGeneration:
    def __init__(self, n, m):
        self.m = m
        self.g = Graph()
        self.g.set_directed(False)

        for vertex in range(0, n):
            self.g.add_vertex()

        self._update_deg_list()

    def _update_deg_list(self):
        self.deg_list = self.g.degree_property_map('total').get_array()

    def format_degrees(self, v1):
        v1_neighbours = [self.g.vertex_index[neighbour] for neighbour in self.g.vertex(v1).all_neighbours()]

        def vertex_degree((v2, degree)):
            if v2 != v1 and v2 not in v1_neighbours:
                return degree + 1
            else:
                return 0

        return imap(vertex_degree, enumerate(self.deg_list))

    def format_degrees_a(self):
        n = self.g.num_vertices()
        def vertex_degree((v1, degree)):
            if self.g.vertex(v1).out_degree() == n - 1:
                return 0
            else:
                return degree + 1

        return imap(vertex_degree, enumerate(self.deg_list))

    def _add_edge(self):
        v1_degrees = list(self.format_degrees_a())
        v1_degree_sum = sum(v1_degrees)
        v1_choices = imap(lambda deg: float(deg) / float(v1_degree_sum), v1_degrees)
        v1 = _weighted_choice(v1_choices)

        v2_degrees = list(self.format_degrees(v1))
        v2_degree_sum = sum(v2_degrees)
        v2_choices = imap(lambda deg: float(deg) / float(v2_degree_sum), v2_degrees)
        v2 = _weighted_choice(v2_choices)

        v1 = self.g.vertex(v1)
        v2 = self.g.vertex(v2)
        self.g.add_edge(v1, v2)
        self._update_deg_list()

    def run(self):
        for edge in range(0, self.m):
            self._add_edge()

        return self.g


class DenseGraphBuilder:
    def __init__(self):
        self.n = 0
        self.m = 0

    def with_n(self, n):
        self.n = n
        return self

    def with_m(self, m):
        self.m = m
        return self


    def build(self):
        generator = DenseGraphGeneration(self.n, self.m)
        return generator.run()

