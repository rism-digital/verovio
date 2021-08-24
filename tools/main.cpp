/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include <assert.h>
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

#define HELP_TABS 32

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

std::string fromCamelCase(const std::string &s)
{
    std::regex regExp1("(.)([A-Z][a-z]+)");
    std::regex regExp2("([a-z0-9])([A-Z])");

    std::string result = s;
    result = std::regex_replace(result, regExp1, "$1-$2");
    result = std::regex_replace(result, regExp2, "$1-$2");

    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toCamelCase(const std::string &s)
{
    std::istringstream iss(s);
    std::string token;
    std::string result;

    while (getline(iss, token, '-')) {
        token[0] = toupper(token[0]);
        result += token;
    }

    result[0] = tolower(result[0]);

    return result;
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

void display_option(vrv::Option *option)
{
    std::string option_str = " ";

    if (option->GetShortOption()) {
        option_str.append("-");
        option_str.push_back(option->GetShortOption());
        option_str.append(", ");
    }

    if (!option->GetKey().empty()) {
        option_str.append("--");
        option_str.append(fromCamelCase(option->GetKey()));
    }

    const vrv::OptionDbl *optDbl = dynamic_cast<const vrv::OptionDbl *>(option);
    const vrv::OptionInt *optInt = dynamic_cast<const vrv::OptionInt *>(option);
    const vrv::OptionIntMap *optIntMap = dynamic_cast<const vrv::OptionIntMap *>(option);
    const vrv::OptionString *optString = dynamic_cast<const vrv::OptionString *>(option);
    const vrv::OptionArray *optArray = dynamic_cast<const vrv::OptionArray *>(option);
    const vrv::OptionBool *optBool = dynamic_cast<const vrv::OptionBool *>(option);

    if (optDbl) {
        option_str.append(" <f>");
    }
    else if (optInt) {
        option_str.append(" <i>");
    }
    else if (optString) {
        option_str.append(" <s>");
    }
    else if (optArray) {
        option_str.append("* <s>");
    }
    else if (!optBool) {
        option_str.append(" <s>");
    }

    if (option_str.size() < HELP_TABS) {
        option_str.insert(option_str.end(), HELP_TABS - option_str.size(), ' ');
    }
    else {
        option_str.append("\t");
    }

    std::cout << option_str << option->GetDescription();

    if (optInt && (optInt->GetDefault() != optInt->GetMin()) && (optInt->GetDefault() != optInt->GetMax())) {
        std::cout << " (default: " << optInt->GetDefault();
        std::cout << "; min: " << optInt->GetMin();
        std::cout << "; max: " << optInt->GetMax() << ")";
    }
    if (optDbl && (optDbl->GetDefault() != optDbl->GetMin()) && (optDbl->GetDefault() != optDbl->GetMax())) {
        std::cout << std::fixed << " (default: " << optDbl->GetDefault();
        std::cout << std::fixed << "; min: " << optDbl->GetMin();
        std::cout << std::fixed << "; max: " << optDbl->GetMax() << ")";
    }
    if (optString) {
        std::cout << " (default: \"" << optString->GetDefault() << "\")";
    }
    if (optIntMap) {
        std::cout << " (default: \"" << optIntMap->GetDefaultStrValue()
                  << "\"; other values: " << optIntMap->GetStrValuesAsStr(true) << ")";
    }
    std::cout << std::endl;
}

void display_version()
{
    std::cout << "Verovio " << vrv::GetVersion() << std::endl;
}

void display_usage()
{
    std::cout.precision(2);

    display_version();
    std::cout << std::endl << "Example usage:" << std::endl << std::endl;
    std::cout << " verovio [-s scale] [-r resource-path] [-o outfile] infile" << std::endl << std::endl;

    // These need to be kept in alphabetical order:
    // -options with both short and long forms first
    // -then options with long forms only
    // -then debugging options

    // Options with both short and long forms
    vrv::Options options;

    std::cout << "Options (marked as * are repeatable)" << std::endl;
    const std::vector<vrv::Option *> *baseOptions = options.GetBaseOptions();

    // All other option groups
    for (auto const &option : *baseOptions) {
        display_option(option);
    }

    std::vector<vrv::OptionGrp *> *grps = options.GetGrps();

    for (auto const &grp : *grps) {

        // Options with long forms only
        std::cout << std::endl << grp->GetLabel() << std::endl;
        const std::vector<vrv::Option *> *options = grp->GetOptions();

        for (auto const &option : *options) {
            display_option(option);
        }
    }
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
    int show_help = 0;
    int show_version = 0;

    // Create the toolkit instance without loading the font because
    // the resource path might be specified in the parameters
    // The fonts will be loaded later with Resources::InitFonts()
    vrv::Toolkit toolkit(false);

    if (argc < 2) {
        std::cerr << "Expected one input file but found none." << std::endl << std::endl;
        display_usage();
        exit(1);
    }

    static struct option base_options[] = { //
        { "all-pages", no_argument, 0, 'a' }, //
        { "from", required_argument, 0, 'f' }, //
        { "help", no_argument, 0, 'h' }, //
        { "outfile", required_argument, 0, 'o' }, //
        { "page", required_argument, 0, 'p' }, //
        { "resources", required_argument, 0, 'r' }, //
        { "scale", required_argument, 0, 's' }, //
        { "to", required_argument, 0, 't' }, //
        { "version", no_argument, 0, 'v' }, //
        { "xml-id-seed", required_argument, 0, 'x' }, //
        // standard input - long options only or - as filename
        { "stdin", no_argument, 0, 'z' }, //
        { 0, 0, 0, 0 }
    };

    int baseSize = sizeof(base_options) / sizeof(option);

    vrv::Options *options = toolkit.GetOptions();
    const vrv::MapOfStrOptions *params = options->GetItems();
    int mapSize = (int)params->size();

    struct option *long_options;
    int i = 0;
    long_options = (struct option *)malloc(sizeof(struct option) * (baseSize + mapSize));

    // A vector of string for storing names as const char* for long_options
    std::vector<std::string> optNames;
    optNames.reserve(mapSize);

    vrv::MapOfStrOptions::const_iterator iter;
    for (iter = params->begin(); iter != params->end(); ++iter) {
        // Double check that back and forth convertion is correct
        assert(toCamelCase(fromCamelCase(iter->first)) == iter->first);

        optNames.push_back(fromCamelCase(iter->first));
        long_options[i].name = optNames.at(i).c_str();
        vrv::OptionBool *optBool = dynamic_cast<vrv::OptionBool *>(iter->second);
        long_options[i].has_arg = (optBool) ? no_argument : required_argument;
        long_options[i].flag = 0;
        long_options[i].val = 0;
        i++;
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
    int seed = 0;
    std::string key;
    int option_index = 0;
    vrv::Option *opt = NULL;
    vrv::OptionBool *optBool = NULL;
    while ((c = getopt_long(argc, argv, "ab:f:ho:p:r:s:t:vx:z", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                key = long_options[option_index].name;
                opt = params->at(toCamelCase(key));
                optBool = dynamic_cast<vrv::OptionBool *>(opt);

                // Handle deprecated options
                /*
                if (key == "condense-encoded") {
                    vrv::LogWarning("Option --condense-encoded is deprecated; use --condense encoded instead");
                    options->m_condense.SetValue("encoded");
                    break;
                }
                */

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

            case 'o': outfile = std::string(optarg); break;

            case 'p': page = atoi(optarg); break;

            case 'r': vrv::Resources::SetPath(optarg); break;

            case 't':
                outformat = std::string(optarg);
                toolkit.SetOutputTo(std::string(optarg));
                break;

            case 's':
                if (!toolkit.SetScale(atoi(optarg))) {
                    exit(1);
                }
                break;

            case 'v': show_version = 1; break;

            case 'x':
                seed = atoi(optarg);
                options->m_xmlIdSeed.SetValue(seed);
                vrv::Object::SeedUuid(seed);
                break;

            case 'z':
                if (!strcmp(long_options[option_index].name, "stdin")) {
                    infile = "-";
                }
                break;

            case 'h':
                display_usage();
                exit(0);
                break;

            case '?':
                display_usage();
                exit(1);
                break;

            default: break;
        }
    }
    options->Sync();

    if (show_version) {
        display_version();
        exit(0);
    }

    if (show_help) {
        display_usage();
        exit(0);
    }

    std::cerr << infile;
    if (optind <= argc - 1) {
        infile = std::string(argv[optind]);
    }
    else if (infile != "-") {
        std::cerr << "Incorrect number of arguments: expected one input file but found none." << std::endl << std::endl;
        display_usage();
        exit(1);
    }

    // Make sure the user uses a valid Resource path
    // Save many headaches for empty SVGs
    if (!dir_exists(vrv::Resources::GetPath())) {
        std::cerr << "The resources path " << vrv::Resources::GetPath() << " could not be found; please use -r option."
                  << std::endl;
        exit(1);
    }

    // Load the music font from the resource directory
    if (!vrv::Resources::InitFonts()) {
        std::cerr << "The music font could not be loaded; please check the contents of the resource directory."
                  << std::endl;
        exit(1);
    }

    // Load a specified font
    if (!vrv::Resources::SetFont(options->m_font.GetValue())) {
        std::cerr << "Font '" << options->m_font.GetValue() << "' could not be loaded." << std::endl;
        exit(1);
    }

    if ((outformat != "svg") && (outformat != "mei") && (outformat != "midi") && (outformat != "timemap")
        && (outformat != "humdrum") && (outformat != "hum") && (outformat != "pae") && (outformat != "pb-mei")) {
        std::cerr << "Output format (" << outformat
                  << ") can only be 'mei', 'pb-mei', 'svg', 'midi', 'humdrum' or 'pae'." << std::endl;
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
            std::cout << toolkit.RenderToTimemap();
        }
        else if (!toolkit.RenderToTimemapFile(outfile)) {
            std::cerr << "Unable to write MIDI to " << outfile << "." << std::endl;
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
        const char *scoreBased = (outformat == "mei") ? "true" : "false";
        const char *removeIds = (options->m_removeIds.GetValue()) ? "true" : "false";
        outfile += ".mei";
        if (all_pages) {
            std::string params = vrv::StringFormat("{'scoreBased': %s, 'removeIds': %s}", scoreBased, removeIds);
            if (std_output) {
                std::string output;
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
        else {
            std::string params
                = vrv::StringFormat("{'scoreBased': %s, 'pageNo': %d, 'removeIds': %s}", scoreBased, page, removeIds);
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

    free(long_options);
    return 0;
}
