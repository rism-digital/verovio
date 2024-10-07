/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

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

std::string basename(std::string const &pathname)
{
    return std::string(std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base(), pathname.end());
}

std::string removeExtension(std::string const &filename)
{
    std::string::const_reverse_iterator pivot = std::find(filename.rbegin(), filename.rend(), '.');
    return pivot == filename.rend() ? filename : std::string(filename.begin(), pivot.base() - 1);
}

bool dir_exists(std::string dir)
{
    struct stat st;
    if ((stat(dir.c_str(), &st) == 0) && (((st.st_mode) & S_IFMT) == S_IFDIR)) {
        return true;
    }
    else {
        return false;
    }
}

void display_version()
{
    std::cout << "Verovio " << vrv::GetVersion() << std::endl;
}

bool optionExists(const std::string &option, int argc, char **argv, std::string &badOption)
{
    for (int i = 0; i < argc; ++i) {
        if (!strncmp(option.c_str(), argv[i], option.size())) return true;
        if (option.rfind(argv[i], 0) == 0) badOption = argv[i];
    }
    return false;
}

option optionStruct(vrv::Option *option, const std::map<vrv::Option *, std::string> &optionNames)
{
    return { optionNames.at(option).c_str(), option->IsArgumentRequired() ? required_argument : no_argument, 0,
        option->GetShortOption() };
}

int main(int argc, char **argv)
{
    std::string infile;
    std::string svgdir;
    std::string outfile;
    std::string outformat = "svg";
    bool std_output = false;

    int all_pages = 0;
    int page = 1;
    int show_version = 0;

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

    static struct option base_options[] = { //
        optionStruct(&options->m_allPages, optionNames), //
        optionStruct(&options->m_inputFrom, optionNames), //
        optionStruct(&options->m_help, optionNames), //
        optionStruct(&options->m_logLevel, optionNames), //
        optionStruct(&options->m_outfile, optionNames), //
        optionStruct(&options->m_page, optionNames), //
        optionStruct(&options->m_resourcePath, optionNames), //
        optionStruct(&options->m_scale, optionNames), //
        optionStruct(&options->m_outputTo, optionNames), //
        optionStruct(&options->m_version, optionNames), //
        optionStruct(&options->m_xmlIdSeed, optionNames), //
        // standard input - long options only or - as filename
        { "stdin", no_argument, 0, 'z' }, //
        { 0, 0, 0, 0 }
    };

    int baseSize = sizeof(base_options) / sizeof(option);

    const vrv::MapOfStrOptions *params = options->GetItems();
    int mapSize = (int)params->size();

    if (argc < 2) {
        std::cerr << "Expected one input file but found none." << std::endl << std::endl;
        toolkit.PrintOptionUsage("", std::cout);
        exit(1);
    }

    struct option *long_options;
    int i = 0;
    long_options = (struct option *)malloc(sizeof(struct option) * (baseSize + mapSize));

    // A vector of string for storing names as const char* for long_options
    std::vector<std::string> optNames;
    optNames.reserve(mapSize);

    vrv::MapOfStrOptions::const_iterator iter;
    for (iter = params->begin(); iter != params->end(); ++iter) {
        // Double check that back and forth convertion is correct
        assert(vrv::ToCamelCase(vrv::FromCamelCase(iter->first)) == iter->first);

        optNames.push_back(vrv::FromCamelCase(iter->first));
        long_options[i].name = optNames.at(i).c_str();
        vrv::OptionBool *optBool = dynamic_cast<vrv::OptionBool *>(iter->second);
        long_options[i].has_arg = (optBool) ? no_argument : required_argument;
        long_options[i].flag = 0;
        long_options[i].val = 0;
        ++i;
    }

    // Concatenate the base options
    assert(i == mapSize);
    for (; i < mapSize + baseSize; ++i) {
        long_options[i].name = base_options[i - mapSize].name;
        long_options[i].has_arg = base_options[i - mapSize].has_arg;
        long_options[i].flag = base_options[i - mapSize].flag;
        long_options[i].val = base_options[i - mapSize].val;
    }

    int c;
    std::string key;
    int option_index = 0;
    vrv::Option *opt = NULL;
    vrv::OptionBool *optBool = NULL;
    std::string resourcePath = toolkit.GetResourcePath();
    while ((c = getopt_long(argc, argv, "ab:f:h:l:o:p:r:s:t:vx:z", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                key = long_options[option_index].name;
                opt = params->at(vrv::ToCamelCase(key));
                optBool = dynamic_cast<vrv::OptionBool *>(opt);
                if (std::string badOption; !optionExists("--" + key, argc, argv, badOption)) {
                    vrv::LogError("Unrecognized option %s has been skipped.", badOption.c_str());
                    continue;
                }

                if (optBool) {
                    optBool->SetValue(true);
                }
                else if (opt) {
                    if (!opt->SetValue(optarg)) {
                        vrv::LogWarning("Setting option %s with %s failed, default value used",
                            long_options[option_index].name, optarg);
                    }
                }
                else {
                    vrv::LogError("Something went wrong with option %s", long_options[option_index].name);
                    exit(1);
                }
                break;

            case 'a': all_pages = 1; break;

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

            case 'v': show_version = 1; break;

            case 'x':
                if (!options->m_xmlIdSeed.SetValue(optarg)) {
                    vrv::LogWarning("Setting xml id seed with %s failed, default value used", optarg);
                }
                vrv::Object::SeedID(options->m_xmlIdSeed.GetValue());
                break;

            case 'z':
                if (!strcmp(long_options[option_index].name, "stdin")) {
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

    if (show_version) {
        display_version();
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
    if (!dir_exists(resourcePath)) {
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
        "expansionmap", "humdrum", "hum", "pae" };
    if (std::find(outformats.begin(), outformats.end(), outformat) == outformats.end()) {
        std::cerr << "Output format (" << outformat
                  << ") can only be 'mei', 'mei-basic', 'mei-pb', mei-facs', 'svg', 'midi', 'timemap', 'expansionmap', "
                     "'humdrum', 'hum', or 'pae'."
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
        outfile = removeExtension(infile);
    }
    else if (outfile == "-") {
        // vrv::EnableLog(false);
        std_output = true;
    }
    else {
        outfile = removeExtension(outfile);
    }

    // Skip the layout for MIDI and timemap output by setting --breaks to none
    if ((outformat == "midi") || (outformat == "timemap") || (outformat == "expansionmap")) {
        toolkit.SetOptions("{'breaks': 'none'}");
    }

    // Load the std input or load the file
    if (!((toolkit.GetOutputTo() == vrv::HUMDRUM) && (toolkit.GetInputFrom() == vrv::MEI))) {
        if (infile == "-") {
            std::ostringstream data_stream;
            for (std::string line; getline(std::cin, line);) {
                data_stream << line << std::endl;
            }
            if (!toolkit.LoadData(data_stream.str())) {
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

    if (toolkit.GetOutputTo() != vrv::HUMDRUM) {
        // Check the page range
        if (page > toolkit.GetPageCount()) {
            std::cerr << "The page requested (" << page << ") is not in the page range (max is "
                      << toolkit.GetPageCount() << ")." << std::endl;
            exit(1);
        }
        if (page < 1) {
            std::cerr << "The page number has to be greater than 0." << std::endl;
            exit(1);
        }
    }

    int from = page;
    int to = page + 1;
    if (all_pages) {
        to = toolkit.GetPageCount() + 1;
    }

    if (outformat == "svg") {
        int p;
        for (p = from; p < to; ++p) {
            std::string cur_outfile = outfile;
            if (all_pages) {
                cur_outfile += vrv::StringFormat("_%03d", p);
            }
            cur_outfile += ".svg";
            if (std_output) {
                std::cout << toolkit.RenderToSVG(p);
            }
            else if (!toolkit.RenderToSVGFile(cur_outfile, p)) {
                std::cerr << "Unable to write SVG to " << cur_outfile << "." << std::endl;
                exit(1);
            }
            else {
                std::cerr << "Output written to " << cur_outfile << "." << std::endl;
            }
        }
    }

    else if (outformat == "hummidi") {
        std::string humdata;
        if (infile == "-") {
            std::ostringstream input_data;
            for (std::string line; getline(std::cin, line);) {
                input_data << line << std::endl;
            }
            if (input_data.str().empty()) {
                std::cerr << "The input could not be loaded." << std::endl;
                exit(1);
            }
            humdata = input_data.str();
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

        std::string base64midi = toolkit.ConvertHumdrumToMIDI(humdata);
        if (std_output) {
            std::cout << base64midi << std::endl;
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
        if (std_output) {
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
        if (std_output) {
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
        if (std_output) {
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
                std::ostringstream input_data;
                for (std::string line; getline(std::cin, line);) {
                    input_data << line << std::endl;
                }
                if (input_data.str().empty()) {
                    std::cerr << "The input could not be loaded." << std::endl;
                    exit(1);
                }
                meidata = input_data.str();
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
                std::ostringstream input_data;
                for (std::string line; getline(std::cin, line);) {
                    input_data << line << std::endl;
                }
                if (input_data.str().empty()) {
                    std::cerr << "The input could not be loaded." << std::endl;
                    exit(1);
                }
                humdata = input_data.str();
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
        if (std_output) {
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
        if (std_output) {
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
        const char *scoreBased = (outformat == "mei-pb") ? "false" : "true";
        const char *basic = (outformat == "mei-basic") ? "true" : "false";
        const char *removeIds = (options->m_removeIds.GetValue()) ? "true" : "false";
        const char *generateFacs = (outformat == "mei-facs") ? "true" : "false";
        outfile += ".mei";
        if (all_pages) {
            std::string params
                = vrv::StringFormat("{'scoreBased': %s, 'basic': %s, 'removeIds': %s, 'generateFacs': %s}", scoreBased,
                    basic, removeIds, generateFacs);
            if (std_output) {
                std::string output;
                std::cout << toolkit.GetMEI(params);
            }
            else if (!toolkit.SaveFile(outfile, params)) {
                exit(1);
            }
            else {
                std::cerr << "Output written to " << outfile << "." << std::endl;
            }
        }
        else {
            std::string params = vrv::StringFormat(
                "{'scoreBased': %s, 'basic': %s, 'pageNo': %d, 'removeIds': %s, 'generateFacs': %s}", scoreBased, basic,
                page, removeIds, generateFacs);
            if (std_output) {
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
    }

    // Display runtime if desired
    if (options->m_showRuntime.GetValue()) {
        toolkit.LogRuntime();
    }

    free(long_options);
    return 0;
}
