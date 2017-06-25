from subprocess32 import Popen, PIPE, STDOUT
from os import path
import resource

project_root = path.abspath(path.join(path.dirname(__file__), '../..'))

def run_algorithm(instance, runner):
    runner_file = path.join(project_root, "bin", runner)
    process = Popen([runner_file], stdout=PIPE, stdin=PIPE, stderr=None)
    process_input = instance

    # Measurement sandwich
    pre_run_program_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime
    result = process.communicate(input=bytearray(process_input))
    post_run_program_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime

    stdout = result[0]
    algorithm_time = post_run_program_time - pre_run_program_time

    return stdout, algorithm_time


def profile_algorithm(instance, runs, runner):
    results_list = []
    run_result = None
    for i in range(0, runs):
        (run_result, run_time) = run_algorithm(instance, runner)
        results_list.append(run_time)
    return run_result, average(results_list)


def average(ls):
    return reduce(lambda x, y: x + y, ls) / len(ls)


def process_result(previous_n, last_result, n_times, extractor):
    print "Processing " + str(previous_n)
    print last_result
    return previous_n, extractor(last_result), average(n_times)


def profile_instances(generator, precision, runner, extractor):
    previous_n = -1
    n_times = []
    last_result = None
    for (current_n, instance) in generator():
        if previous_n >= 0 and previous_n != current_n:
            yield process_result(previous_n, last_result, n_times, extractor)
            del n_times[:]

        previous_n = current_n

        (last_result, average_run_time) = profile_algorithm(instance, precision, runner)
        n_times.append(average_run_time)

    yield process_result(previous_n, last_result, n_times, extractor)
