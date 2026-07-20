/////////////////////////////////////////////////////////////////////////////
// Runtime font performance benchmark
/////////////////////////////////////////////////////////////////////////////

#include "toolkit.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

struct Arguments {
    std::string fixture;
    std::string resources;
    int iterations = 30;
    int coldIterations = -1;
    int warmups = 5;
    int warmBatch = 1000;
};

double Milliseconds(Clock::time_point begin, Clock::time_point end)
{
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

double Median(std::vector<double> values)
{
    std::sort(values.begin(), values.end());
    const size_t middle = values.size() / 2;
    if (values.size() % 2) return values[middle];
    return (values[middle - 1] + values[middle]) / 2.0;
}

void PrintSamples(const std::vector<double> &values)
{
    std::cout << '[';
    for (size_t i = 0; i < values.size(); ++i) {
        if (i) std::cout << ',';
        std::cout << std::fixed << std::setprecision(6) << values[i];
    }
    std::cout << ']';
}

Arguments ParseArguments(int argc, char **argv)
{
    Arguments arguments;
    for (int i = 1; i < argc; ++i) {
        const std::string name = argv[i];
        if ((name == "--fixture") && (i + 1 < argc))
            arguments.fixture = argv[++i];
        else if ((name == "--resources") && (i + 1 < argc))
            arguments.resources = argv[++i];
        else if ((name == "--iterations") && (i + 1 < argc))
            arguments.iterations = std::atoi(argv[++i]);
        else if ((name == "--cold-iterations") && (i + 1 < argc))
            arguments.coldIterations = std::atoi(argv[++i]);
        else if ((name == "--warmups") && (i + 1 < argc))
            arguments.warmups = std::atoi(argv[++i]);
        else if ((name == "--warm-batch") && (i + 1 < argc))
            arguments.warmBatch = std::atoi(argv[++i]);
        else {
            std::cerr << "Unknown or incomplete argument: " << name << '\n';
            std::exit(2);
        }
    }
    if (arguments.fixture.empty() || arguments.resources.empty() || (arguments.iterations < 1)
        || (arguments.warmups < 0) || (arguments.warmBatch < 1)) {
        std::cerr << "Usage: font-runtime-benchmark --fixture FILE --resources DIR "
                     "[--iterations 30] [--warmups 5] [--warm-batch 1000]\n";
        std::exit(2);
    }
    if (arguments.coldIterations < 0) arguments.coldIterations = arguments.iterations;
    if (arguments.coldIterations < 1) std::exit(2);
    return arguments;
}

bool Prepare(vrv::Toolkit &toolkit, const Arguments &arguments)
{
    if (!toolkit.SetResourcePath(arguments.resources)) return false;
    if (!toolkit.SetOptions("{\"breaks\":\"encoded\",\"header\":\"none\",\"footer\":\"none\"}")) return false;
    return toolkit.LoadFile(arguments.fixture);
}

} // namespace

int main(int argc, char **argv)
{
    const Arguments arguments = ParseArguments(argc, argv);
    std::vector<double> cold;
    std::vector<double> warm;
    cold.reserve(arguments.iterations);
    warm.reserve(arguments.iterations);
    size_t svgBytes = 0;

    for (int i = 0; i < arguments.coldIterations; ++i) {
        const auto begin = Clock::now();
        vrv::Toolkit toolkit(false);
        if (!Prepare(toolkit, arguments)) {
            std::cerr << toolkit.GetLog() << '\n';
            return 1;
        }
        const std::string svg = toolkit.RenderToSVG(1);
        const auto end = Clock::now();
        if (svg.empty()) return 1;
        svgBytes = svg.size();
        cold.push_back(Milliseconds(begin, end));
    }

    vrv::Toolkit toolkit(false);
    if (!Prepare(toolkit, arguments)) {
        std::cerr << toolkit.GetLog() << '\n';
        return 1;
    }
    for (int i = 0; i < arguments.warmups; ++i) {
        if (toolkit.RenderToSVG(1).empty()) return 1;
    }
    for (int i = 0; i < arguments.iterations; ++i) {
        const auto begin = Clock::now();
        std::string svg;
        for (int batch = 0; batch < arguments.warmBatch; ++batch) {
            svg = toolkit.RenderToSVG(1);
            if (svg.empty()) return 1;
        }
        const auto end = Clock::now();
        svgBytes = svg.size();
        warm.push_back(Milliseconds(begin, end) / arguments.warmBatch);
    }

    std::cout << "{\"fixture\":\"" << arguments.fixture << "\",\"iterations\":" << arguments.iterations
              << ",\"warm_batch\":" << arguments.warmBatch << ",\"cold_ms\":{";
    std::cout << "\"median\":" << std::fixed << std::setprecision(6) << Median(cold) << ",\"samples\":";
    PrintSamples(cold);
    std::cout << "},\"warm_ms\":{";
    std::cout << "\"median\":" << std::fixed << std::setprecision(6) << Median(warm) << ",\"samples\":";
    PrintSamples(warm);
    std::cout << "},\"svg_bytes\":" << svgBytes << "}\n";
    return 0;
}
