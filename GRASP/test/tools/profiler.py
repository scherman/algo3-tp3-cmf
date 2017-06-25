from subprocess32 import Popen, PIPE, STDOUT
from os import path
import resource

project_root = path.abspath(path.join(path.dirname(__file__), '../..'))


def run_algorithm(runner, stdin, args):
    runner_file = path.join(project_root, "bin", runner)
    process = Popen([runner_file] + args, stdout=PIPE, stdin=PIPE, stderr=None)

    # Measurement sandwich
    pre_run_program_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime
    result = process.communicate(input=bytearray(stdin))
    post_run_program_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime

    stdout = result[0]
    algorithm_time = post_run_program_time - pre_run_program_time

    return stdout, algorithm_time


def profile_algorithm(instance, runs, runner, extractor):
    times_list = []
    result_list = []
    for i in range(0, runs):
        (run_result, run_time) = run_algorithm(runner, instance[0], instance[1:])
        times_list.append(run_time)
        result_list.append(extractor(run_result))

    return average(result_list), average(times_list)


def average(ls):
    return reduce(lambda x, y: x + y, ls) / len(ls)


def process_result(previous_n, n_results, n_times):
    print "Processing " + str(previous_n)
    return previous_n, average(n_results), average(n_times)


def profile_instances(generator, precision, runner, extractor):
    previous_n = -1
    n_times = []
    n_results = []
    for (current_n, instance) in generator():
        if previous_n >= 0 and previous_n != current_n:
            yield process_result(previous_n, n_results, n_times)
            del n_times[:]

        previous_n = current_n

        (average_result, average_run_time) = profile_algorithm(instance, precision, runner, extractor)
        n_times.append(average_run_time)
        n_results.append(average_result)

    yield process_result(previous_n, n_results, n_times)
