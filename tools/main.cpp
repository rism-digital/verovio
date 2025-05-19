/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <sys/stat.h>

#ifndef _WIN32
#include <getopt.h>
#else
#include "win_getopt.h"
#endif

//----------------------------------------------------------------------------

#include "options.h"
#include "toolkit.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "jsonxx.h"

// Some redundant code to get basenames
// and remove extensions
// possible that it is not in std??
struct MatchPathSeparator {
    bool operator()(char ch) const { return ch == '\\' || ch == '/'; }
};

std::string Basename(const std::string &pathname)
{
    return std::string(std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base(), pathname.end());
}

std::string RemoveExtension(const std::string &filename)
{
    std::string::const_reverse_iterator pivot = std::find(filename.rbegin(), filename.rend(), '.');
    return (pivot == filename.rend()) ? filename : std::string(filename.begin(), pivot.base() - 1);
}

bool DirExists(const std::string &dir)
{
    struct stat st;
    return (stat(dir.c_str(), &st) == 0) && (((st.st_mode) & S_IFMT) == S_IFDIR);
}

void DisplayVersion()
{
    std::cout << "Verovio " << vrv::GetVersion() << std::endl;
}

bool OptionExists(const std::string &option, int argc, char **argv, std::string &badOption)
{
    for (int i = 0; i < argc; ++i) {
        if (!strncmp(option.c_str(), argv[i], option.size())) return true;
        if (option.rfind(argv[i], 0) == 0) badOption = argv[i];
    }
    return false;
}

option OptionStruct(vrv::Option *option, const std::map<vrv::Option *, std::string> &optionNames)
{
    return { optionNames.at(option).c_str(), option->IsArgumentRequired() ? required_argument : no_argument, NULL,
        option->GetShortOption() };
}

int main(int argc, char **argv)
{
    std::string infile;
    std::string svgdir;
    std::string outfile;
    std::string outformat = "svg";
    bool stdOutput = false;

    bool allPages = false;
    std::optional<int> page;
    bool showVersion = false;

    // Create the toolkit instance without loading the font because
    // the resource path might be specified in the parameters
    // The fonts will be loaded later with Resources::InitFonts()
    vrv::Toolkit toolkit(false);

    vrv::Options *options = toolkit.GetOptionsObj();

    // map storing lower-case option names
    std::map<vrv::Option *, std::string> optionNames
        = { { &options->m_allPages, vrv::FromCamelCase(options->m_allPages.GetKey()) },
              { &options->m_inputFrom, vrv::FromCamelCase(options->m_inputFrom.GetKey()) },
              { &options->m_help, vrv::FromCamelCase(options->m_help.GetKey()) },
              { &options->m_logLevel, vrv::FromCamelCase(options->m_logLevel.GetKey()) },
              { &options->m_outfile, vrv::FromCamelCase(options->m_outfile.GetKey()) },
              { &options->m_page, vrv::FromCamelCase(options->m_page.GetKey()) },
              { &options->m_resourcePath, vrv::FromCamelCase(options->m_resourcePath.GetKey()) },
              { &options->m_scale, vrv::FromCamelCase(options->m_scale.GetKey()) },
              { &options->m_outputTo, vrv::FromCamelCase(options->m_outputTo.GetKey()) },
              { &options->m_version, vrv::FromCamelCase(options->m_version.GetKey()) },
              { &options->m_xmlIdSeed, vrv::FromCamelCase(options->m_xmlIdSeed.GetKey()) } };

    // TODO: Use std::to_array to get rid of the explicit array dimension, once MSVC supports it
    // See the final comment in https://github.com/rism-digital/verovio/pull/4023
    static const std::array<option, 13> baseOptions = {
        OptionStruct(&options->m_allPages, optionNames), //
        OptionStruct(&options->m_inputFrom, optionNames), //
        OptionStruct(&options->m_help, optionNames), //
        OptionStruct(&options->m_logLevel, optionNames), //
        OptionStruct(&options->m_outfile, optionNames), //
        OptionStruct(&options->m_page, optionNames), //
        OptionStruct(&options->m_resourcePath, optionNames), //
        OptionStruct(&options->m_scale, optionNames), //
        OptionStruct(&options->m_outputTo, optionNames), //
        OptionStruct(&options->m_version, optionNames), //
        OptionStruct(&options->m_xmlIdSeed, optionNames), //
        // standard input - long options only or - as filename
        { "stdin", no_argument, 0, 'z' }, //
        { 0, 0, 0, 0 } //
    };

    const vrv::MapOfStrOptions *params = options->GetItems();

    if (argc < 2) {
        std::cerr << "Expected one input file but found none." << std::endl << std::endl;
        toolkit.PrintOptionUsage("", std::cout);
        exit(1);
    }

    std::vector<option> longOptions;
    std::vector<std::unique_ptr<std::string>> longOptionNames;
    for (const auto &param : *params) {
        // Double check that back and forth convertion is correct
        assert(vrv::ToCamelCase(vrv::FromCamelCase(param.first)) == param.first);

        longOptions.emplace_back();

        // It is crucial to keep the camel cased name alive, since we store a pointer to it in longOptions
        const std::string name = vrv::FromCamelCase(param.first);
        longOptionNames.push_back(std::make_unique<std::string>(name));
        longOptions.back().name = longOptionNames.back()->c_str();

        vrv::OptionBool *optBool = dynamic_cast<vrv::OptionBool *>(param.second);
        longOptions.back().has_arg = (optBool) ? no_argument : required_argument;
        longOptions.back().flag = NULL;
        longOptions.back().val = 0;
    }

    // Concatenate the base options
    longOptions.insert(longOptions.end(), baseOptions.begin(), baseOptions.end());

    int c;
    std::string key;
    int optionIndex = 0;
    vrv::Option *opt = NULL;
    vrv::OptionBool *optBool = NULL;
    std::string resourcePath = toolkit.GetResourcePath();
    while ((c = getopt_long(argc, argv, "ab:f:h:l:o:p:r:s:t:vx:z", longOptions.data(), &optionIndex)) != -1) {
        switch (c) {
            case 0:
                key = longOptions[optionIndex].name;
                opt = params->at(vrv::ToCamelCase(key));
                optBool = dynamic_cast<vrv::OptionBool *>(opt);
                if (std::string badOption; !OptionExists("--" + key, argc, argv, badOption)) {
                    vrv::LogError("Unrecognized option %s has been skipped.", badOption.c_str());
                    continue;
                }

                if (optBool) {
                    optBool->SetValue(true);
                }
                else if (opt) {
                    if (!opt->SetValue(optarg)) {
                        vrv::LogWarning("Setting option %s with %s failed, default value used",
                            longOptions[optionIndex].name, optarg);
                    }
                }
                else {
                    vrv::LogError("Something went wrong with option %s", longOptions[optionIndex].name);
                    exit(1);
                }
                break;

            case 'a': allPages = true; break;

            case 'f':
                if (!toolkit.SetInputFrom(std::string(optarg))) {
                    exit(1);
                };
                break;

            case 'l': vrv::EnableLog(vrv::StrToLogLevel(std::string(optarg))); break;

            case 'o': outfile = std::string(optarg); break;

            case 'p': page = atoi(optarg); break;

            case 'r': resourcePath = optarg; break;

            case 't':
                outformat = std::string(optarg);
                toolkit.SetOutputTo(std::string(optarg));
                break;

            case 's':
                if (!options->m_scale.SetValue(optarg)) {
                    vrv::LogWarning("Setting scale with %s failed, default value used", optarg);
                }
                break;

            case 'v': showVersion = true; break;

            case 'x':
                if (!options->m_xmlIdSeed.SetValue(optarg)) {
                    vrv::LogWarning("Setting xml id seed with %s failed, default value used", optarg);
                }
                vrv::Object::SeedID(options->m_xmlIdSeed.GetValue());
                break;

            case 'z':
                if (!strcmp(longOptions[optionIndex].name, "stdin")) {
                    infile = "-";
                }
                break;

            case 'h':
                toolkit.PrintOptionUsage(optarg, std::cout);
                exit(0);
                break;

            case '?':
                toolkit.PrintOptionUsage("", std::cout);
                exit(1);
                break;

            default: break;
        }
    }
    options->Sync();

    toolkit.SetLocale();

    if (showVersion) {
        DisplayVersion();
        exit(0);
    }

    // Start the clock if desired
    if (options->m_showRuntime.GetValue()) {
        toolkit.InitClock();
    }

    std::cerr << infile;
    if (optind <= argc - 1) {
        infile = std::string(argv[optind]);
    }
    else if (infile != "-") {
        std::cerr << "Incorrect number of arguments: expected one input file but found none." << std::endl << std::endl;
        toolkit.PrintOptionUsage("base", std::cout);
        exit(1);
    }

    // Make sure the user uses a valid Resource path
    // Save many headaches for empty SVGs
    if (!DirExists(resourcePath)) {
        std::cerr << "The resource path " << resourcePath << " could not be found; please use -r option." << std::endl;
        exit(1);
    }

    // Load the music font from the resource directory
    if (!toolkit.SetResourcePath(resourcePath)) {
        std::cerr << "The music font could not be loaded; please check the contents of the resource directory."
                  << std::endl;
        exit(1);
    }

    const std::vector<std::string> outformats = { "mei", "mei-basic", "mei-pb", "mei-facs", "svg", "midi", "timemap",
        "expansionmap", "humdrum", "hum", "pae", "mei-pb-serialized" };
    if (std::find(outformats.begin(), outformats.end(), outformat) == outformats.end()) {
        std::cerr << "Output format (" << outformat
                  << ") can only be 'mei', 'mei-basic', 'mei-pb', mei-facs', 'svg', 'midi', 'timemap', 'expansionmap', "
                     "'humdrum', 'hum', 'pae', or , 'mei-pb-serialized'."
                  << std::endl;
        exit(1);
    }

    // Make sure we provide a file name or output to std output with std input
    if ((infile == "-") && (outfile.empty())) {
        std::cerr << "Standard input can be used only with standard output or output filename." << std::endl;
        exit(1);
    }

    // Hardcode svg ext for now
    if (outfile.empty()) {
        outfile = RemoveExtension(infile);
    }
    else if (outfile == "-") {
        // vrv::EnableLog(false);
        stdOutput = true;
    }
    else {
        outfile = RemoveExtension(outfile);
    }

    // Skip the layout for MIDI and timemap output by setting --breaks to none
    if ((outformat == "midi") || (outformat == "timemap") || (outformat == "expansionmap")) {
        toolkit.SetOptions("{'breaks': 'none'}");
    }

    // Load the std input or load the file
    if (!((toolkit.GetOutputTo() == vrv::HUMDRUM) && (toolkit.GetInputFrom() == vrv::MEI))) {
        if (infile == "-") {
            std::ostringstream dataStream;
            for (std::string line; getline(std::cin, line);) {
                dataStream << line << std::endl;
            }
            if (!toolkit.LoadData(dataStream.str())) {
                std::cerr << "The input could not be loaded." << std::endl;
                exit(1);
            }
        }
        else {
            if (!toolkit.LoadFile(infile)) {
                std::cerr << "The file '" << infile << "' could not be opened." << std::endl;
                exit(1);
            }
        }
    }

    if (page && (toolkit.GetOutputTo() != vrv::HUMDRUM)) {
        // Check the page range
        if (*page > toolkit.GetPageCount()) {
            std::cerr << "The page requested (" << *page << ") is not in the page range (max is "
                      << toolkit.GetPageCount() << ")." << std::endl;
            exit(1);
        }
        if (*page < 1) {
            std::cerr << "The page number has to be greater than 0." << std::endl;
            exit(1);
        }
    }

    if (outformat == "svg") {
        const int from = page ? *page : 1;
        const int to = allPages ? toolkit.GetPageCount() : from;

        for (int p = from; p <= to; ++p) {
            std::string curOutfile = outfile;
            if (from < to) {
                curOutfile += vrv::StringFormat("_%03d", p);
            }
            curOutfile += ".svg";
            if (stdOutput) {
                std::cout << toolkit.RenderToSVG(p);
            }
            else if (!toolkit.RenderToSVGFile(curOutfile, p)) {
                std::cerr << "Unable to write SVG to " << curOutfile << "." << std::endl;
                exit(1);
            }
            else {
                std::cerr << "Output written to " << curOutfile << "." << std::endl;
            }
        }
    }

    else if (outformat == "hummidi") {
        std::string humdata;
        if (infile == "-") {
            std::ostringstream inputData;
            for (std::string line; getline(std::cin, line);) {
                inputData << line << std::endl;
            }
            if (inputData.str().empty()) {
                std::cerr << "The input could not be loaded." << std::endl;
                exit(1);
            }
            humdata = inputData.str();
        }
        else {
            std::ifstream instream(infile.c_str());
            if (!instream.is_open()) {
                return 1;
            }

            instream.seekg(0, std::ios::end);
            std::streamsize fileSize = (std::streamsize)instream.tellg();
            instream.clear();
            instream.seekg(0, std::ios::beg);

            // read the file into the std::string:
            humdata.resize(fileSize, 0);
            instream.read(humdata.data(), fileSize);
        }

        const std::string base64Midi = toolkit.ConvertHumdrumToMIDI(humdata);
        if (stdOutput) {
            std::cout << base64Midi << std::endl;
        }
        else {
            std::cerr << "Humdrum-MIDI to file not yet implemented." << std::endl;
            // outfile += ".mid";
            // smf::MidiFile outputfile;
            // outputfile.readBase64(base64midi);
            // outputfile.write(outfile);
        }
    }
    else if (outformat == "midi") {
        outfile += ".mid";
        if (stdOutput) {
            std::cerr << "Midi cannot write to standard output." << std::endl;
            exit(1);
        }
        else if (!toolkit.RenderToMIDIFile(outfile)) {
            std::cerr << "Unable to write MIDI to " << outfile << "." << std::endl;
            exit(1);
        }
        else {
            std::cerr << "Output written to " << outfile << "." << std::endl;
        }
    }
    else if (outformat == "timemap") {
        outfile += ".json";
        if (stdOutput) {
            std::string output;
            std::cout << toolkit.RenderToTimemap(options->m_timemapOptions.GetValue());
        }
        else if (!toolkit.RenderToTimemapFile(outfile, options->m_timemapOptions.GetValue())) {
            std::cerr << "Unable to write timemap to " << outfile << "." << std::endl;
            exit(1);
        }
        else {
            std::cerr << "Output written to " << outfile << "." << std::endl;
        }
    }
    else if (outformat == "expansionmap") {
        outfile += "-em.json";
        if (stdOutput) {
            std::string output;
            std::cout << toolkit.RenderToExpansionMap();
        }
        else if (!toolkit.RenderToExpansionMapFile(outfile)) {
            std::cerr << "Unable to write expansionmap to " << outfile << "." << std::endl;
            exit(1);
        }
        else {
            std::cerr << "Output written to " << outfile << "." << std::endl;
        }
    }
    else if (outformat == "humdrum" || outformat == "hum") {
        if (toolkit.GetInputFrom() == vrv::MEI) {
            std::string meidata;

            if (infile == "-") {
                std::ostringstream inputData;
                for (std::string line; getline(std::cin, line);) {
                    inputData << line << std::endl;
                }
                if (inputData.str().empty()) {
                    std::cerr << "The input could not be loaded." << std::endl;
                    exit(1);
                }
                meidata = inputData.str();
            }
            else {
                std::ifstream instream(infile.c_str());
                if (!instream.is_open()) {
                    return 1;
                }

                instream.seekg(0, std::ios::end);
                std::streamsize fileSize = (std::streamsize)instream.tellg();
                instream.clear();
                instream.seekg(0, std::ios::beg);

                // read the file into the std::string:
                meidata.resize(fileSize, 0);
                instream.read(meidata.data(), fileSize);
            }

            // Output will be accessible from toolkit.GetHumdrum():
            toolkit.ConvertMEIToHumdrum(meidata);
        }
        else if (toolkit.GetInputFrom() == vrv::HUMDRUM) {

            std::string humdata;
            if (infile == "-") {
                std::ostringstream inputData;
                for (std::string line; getline(std::cin, line);) {
                    inputData << line << std::endl;
                }
                if (inputData.str().empty()) {
                    std::cerr << "The input could not be loaded." << std::endl;
                    exit(1);
                }
                humdata = inputData.str();
            }
            else {
                std::ifstream instream(infile.c_str());
                if (!instream.is_open()) {
                    return 1;
                }

                instream.seekg(0, std::ios::end);
                std::streamsize fileSize = (std::streamsize)instream.tellg();
                instream.clear();
                instream.seekg(0, std::ios::beg);

                // read the file into the std::string:
                humdata.resize(fileSize, 0);
                instream.read(humdata.data(), fileSize);
            }
            // Output will be accessible from toolkit.GetHumdrum():
            toolkit.ConvertHumdrumToHumdrum(humdata);
        }

        outfile += ".krn";
        if (stdOutput) {
            toolkit.GetHumdrum(std::cout);
        }
        else {
            if (!toolkit.GetHumdrumFile(outfile)) {
                std::cerr << "Unable to write Humdrum to " << outfile << "." << std::endl;
                exit(1);
            }
            else {
                std::cerr << "Output written to " << outfile << "." << std::endl;
            }
        }
    }
    else if (outformat == "pae") {
        outfile += ".pae";
        if (stdOutput) {
            std::string output;
            std::cout << toolkit.RenderToPAE();
        }
        else if (!toolkit.RenderToPAEFile(outfile)) {
            std::cerr << "Unable to write PAE to " << outfile << "." << std::endl;
            exit(1);
        }
        else {
            std::cerr << "Output written to " << outfile << "." << std::endl;
        }
    }
    else {
        std::string params;
        if (outformat == "mei-pb-serialized") {
            params = "{'serialized': true }";
            outfile += ".xml";
        }
        else {
            const char *scoreBased = (outformat == "mei-pb") ? "false" : "true";
            const char *basic = (outformat == "mei-basic") ? "true" : "false";
            const char *removeIds = (options->m_removeIds.GetValue()) ? "true" : "false";
            const char *generateFacs = (outformat == "mei-facs") ? "true" : "false";
            outfile += ".mei";
            params = page ? vrv::StringFormat(
                                "{'scoreBased': %s, 'basic': %s, 'pageNo': %d, 'removeIds': %s, 'generateFacs': %s}",
                                scoreBased, basic, *page, removeIds, generateFacs)
                          : vrv::StringFormat("{'scoreBased': %s, 'basic': %s, 'removeIds': %s, 'generateFacs': %s}",
                                scoreBased, basic, removeIds, generateFacs);
        }
        if (stdOutput) {
            std::cout << toolkit.GetMEI(params);
        }
        else if (!toolkit.SaveFile(outfile, params)) {
            std::cerr << "Unable to write MEI to " << outfile << "." << std::endl;
            exit(1);
        }
        else {
            std::cerr << "Output written to " << outfile << "." << std::endl;
        }
    }

    // Display runtime if desired
    if (options->m_showRuntime.GetValue()) {
        toolkit.LogRuntime();
    }

    return 0;
}
