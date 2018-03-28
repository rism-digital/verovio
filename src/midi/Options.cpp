//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr  5 13:07:18 PDT 1998
// Last Modified: Sat Mar  1 09:31:01 PST 2014 Implemented with STL.
// Filename:      Options.cpp
// Web Address:   https://github.com/craigsapp/optionlib/blob/master/include/Options.h
// Documentation: http://sig.sapp.org/doc/classes/Options
// Syntax:        C++11
//
// Description:   Interface to command-line options.
//

#include "Options.h"

#include <stdlib.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <algorithm>

using namespace std;


///////////////////////////////////////////////////////////////////////////
//
// Option_register class function definitions.
//


//////////////////////////////
//
// Option_register::Option_register -- Constructor.
//

Option_register::Option_register(void) {
   type = 's';
   modifiedQ = 0;
}


Option_register::Option_register(const string& aDefinition, char aType,
      const string& aDefaultOption) {
   type = 's';
   modifiedQ = 0;
   setType(aType);
   setDefinition(aDefinition);
   setDefault(aDefaultOption);
}

Option_register::Option_register(const string& aDefinition, char aType,
      const string& aDefaultOption, const string& aModifiedOption) {
   type = 's';
   modifiedQ = 0;
   setType(aType);
   setDefinition(aDefinition);
   setDefault(aDefaultOption);
   setModified(aModifiedOption);
}



//////////////////////////////
//
// Option_register::~Option_register -- Destructor.
//

Option_register::~Option_register() {
   // do nothing
}



//////////////////////////////
//
// Option_register::clearModified -- Clear any changes in the option value.
//

void Option_register::clearModified(void) {
   modifiedOption.clear();
   modifiedQ = 0;
}



//////////////////////////////
//
// Option_register::getDefinition -- Returns the initial definition.
//	string used to define this entry.
//

const string& Option_register::getDefinition(void) {
   return definition;
}



//////////////////////////////
//
// Option_register::getDescription -- Return the textual description
//      of the entry.
//

const string& Option_register::getDescription(void) {
   return description;
}



//////////////////////////////
//
// Option_register::getDefault --  Return the default value string.
//

const string& Option_register::getDefault(void) {
   return defaultOption;
}



//////////////////////////////
//
// Option_register::getModified -- Return the modified option string.
//

const string& Option_register::getModified(void) {
   return modifiedOption;
}



//////////////////////////////
//
// Option_register::isModified -- Return true if option has been
//    set on the command-line.
//

int Option_register::isModified(void) {
   return modifiedQ;
}



//////////////////////////////
//
// Option_register::getType -- Return the data type of the option.
//

char Option_register::getType(void) {
   return type;
}



//////////////////////////////
//
// Option_register::getOption -- return the modified option
//  	or the default option if no modified option.
//

const string& Option_register::getOption(void) {
   if (isModified()) {
      return getModified();
   } else {
      return getDefault();
   }
}



//////////////////////////////
//
// Option_register::reset -- deallocate space for all
//	strings in object.  (but default string is set to "")
//

void Option_register::reset(void) {
   definition.clear();
   defaultOption.clear();
   modifiedOption.clear();
}



//////////////////////////////
//
// Option_register::setDefault -- Set the default value.
//

void Option_register::setDefault(const string& aString) {
   defaultOption = aString;
}



//////////////////////////////
//
// Option_register::setDefinition -- Set the option definition.
//

void Option_register::setDefinition(const string& aString) {
   definition = aString;
}



//////////////////////////////
//
// Option_register::setDescription -- Set the textual description.
//

void Option_register::setDescription(const string& aString) {
   description = aString;
}



//////////////////////////////
//
// Option_register::setModified -- Set the modified value.
//

void Option_register::setModified(const string& aString) {
   modifiedOption = aString;
   modifiedQ = 1;
}



//////////////////////////////
//
// Option_register::setType -- Set the option type.
//

void Option_register::setType(char aType) {
   type = aType;
}



//////////////////////////////
//
// Option_register::print -- Print the state of the option registery entry.
//     Useul for debugging.
//

ostream& Option_register::print(ostream& out) {
   out << "definition:\t"     << definition     << endl;
   out << "description:\t"    << description    << endl;
   out << "defaultOption:\t"  << defaultOption  << endl;
   out << "modifiedOption:\t" << modifiedOption << endl;
   out << "modifiedQ:\t\t"    << modifiedQ      << endl;
   out << "type:\t\t"         << type           << endl;
   return out;
};




///////////////////////////////////////////////////////////////////////////
//
// Options class function definitions.
//

//////////////////////////////
//
// Options::Options -- Constructor.
//

Options::Options(void) {
   oargc               = -1;
   suppressQ           =  0;
   processedQ          =  0;
   optionsArgument     =  0;
   options_error_check =  1;
   optionFlag          = '-';

   extraArgv.reserve(100);
   extraArgv_strings.reserve(100);
}


Options::Options(int argc, char** argv) {
   oargc               = -1;
   suppressQ           =  0;
   processedQ          =  0;
   optionsArgument     =  0;
   options_error_check =  1;
   optionFlag          = '-';

   extraArgv.reserve(100);
   extraArgv_strings.reserve(100);

   setOptions(argc, argv);
}



//////////////////////////////
//
// Options::~Options -- Destructor.
//

Options::~Options() {
   reset();
}



//////////////////////////////
//
// Options::argc -- returns the argument count as input from main().
//

int Options::argc(void) const {
   return oargc;
}



//////////////////////////////
//
// Options::argv -- returns the arguments strings as input from main().
//

const vector<string>& Options::argv(void) const {
   return oargv;
}



//////////////////////////////
//
// Options::define -- store an option definition in the registry.  Option
//     definitions have this sructure:
//        option-name|alias-name1|alias-name2=option-type:option-default
// option-name :: name of the option (one or more character, not including
//      spaces or equal signs.
// alias-name  :: equivalent name(s) of the option.
// option-type :: single charater indicating the option data type.
// option-default :: default value for option if no given on the command-line.
//

int Options::define(const string& aDefinition) {
   Option_register* definitionEntry = NULL;

   // Error if definition string doesn't contain an equals sign
   auto location = aDefinition.find("=");
   if (location == string::npos) {
      cerr << "Error: no \"=\" in option definition: " << aDefinition << endl;
      exit(1);
   }

   string aliases = aDefinition.substr(0, location);
   string rest    = aDefinition.substr(location+1);
   string otype   = rest;
   string ovalue  = "";

   location = rest.find(":");
   if (location != string::npos) {
      otype  = rest.substr(0, location);
      ovalue = rest.substr(location+1);
   }

   // Remove anyspaces in the option type field
   otype.erase(remove_if(otype.begin(), otype.end(), ::isspace), otype.end());

   // Option types are only a single charater (b, i, d, c or s)
   if (otype.size() != 1) {
      cerr << "Error: option type is invalid: " << otype
           << " in option definition: " << aDefinition << endl;
      exit(1);
   }

   // Check to make sure that the type is known
   if (otype[0] != OPTION_STRING_TYPE  &&
       otype[0] != OPTION_INT_TYPE     &&
       otype[0] != OPTION_FLOAT_TYPE   &&
       otype[0] != OPTION_DOUBLE_TYPE  &&
       otype[0] != OPTION_BOOLEAN_TYPE &&
       otype[0] != OPTION_CHAR_TYPE ) {
      cerr << "Error: unknown option type \'" << otype[0]
           << "\' in defintion: " << aDefinition << endl;
      exit(1);
   }

   // Set up space for a option entry in the registry
   definitionEntry = new Option_register(aDefinition, otype[0], ovalue);

   auto definitionIndex = optionRegister.size();

   // Store option aliases
   string optionName;
   unsigned int i;
   aliases += '|';
   for (i=0; i<aliases.size(); i++) {
      if (::isspace(aliases[i])) {
         continue;
      } else if (aliases[i] == '|') {
         if (isDefined(optionName)) {
            cerr << "Option \"" << optionName << "\" from definition:" << endl;
            cerr << "\t" << aDefinition << endl;
            cerr << "is already defined in: " << endl;
            cerr << "\t" << getDefinition(optionName) << endl;
            exit(1);
         }
         if (optionName.size() > 0) {
            optionList[optionName] = definitionIndex;
         }
         optionName.clear();
      } else {
         optionName += aliases[i];
      }
   }

   // Store definition in registry and return its indexed location.
   // This location will be used to link option aliases to the main
   // command name.
   optionRegister.push_back(definitionEntry);
   return definitionIndex;
}


int Options::define(const string& aDefinition, const string& aDescription) {
   int index = define(aDefinition);
   optionRegister[index]->setDescription(aDescription);
   return index;
}



//////////////////////////////
//
// Options::isDefined -- Return true if option is present in registry.
//

int Options::isDefined(const string& name) {
   if (optionList.find(name) == optionList.end()) {
      return 0;
   } else {
      return 1;
   }
}



//////////////////////////////
//
// Options::getArg -- returns the specified argument.
//	argurment 0 is the command name.
//

const string& Options::getArg(int index) {
   if (index < 0 || index >= (int)argument.size()) {
      cerr << "Error: argument " << index << " does not exist." << endl;
      exit(1);
   }
   return argument[index];
}

// Alias:

const string& Options::getArgument(int index) {
   return getArg(index);
}



//////////////////////////////
//
// Options::getArgCount --  number of arguments on command line.
//	does not count the options or the command name.
//

int Options::getArgCount(void) {
   return argument.size() - 1;
}

// Alias:

int Options::getArgumentCount(void) {
   return getArgCount();
}



//////////////////////////////
//
// Options::getArgList -- return a string vector of the arguments
//     after the options have been parsed out of it.
//

const vector<string>& Options::getArgList(void) {
   return argument;
}

// Alias:

const vector<string>& Options::getArgumentList(void) {
   return getArgList();
}



//////////////////////////////
//
// Options::getBoolean --  returns true if the option was
//	used on the command line.
//

int Options::getBoolean(const string& optionName) {
   int index = getRegIndex(optionName);
   if (index < 0) {
      return 0;
   }
   return optionRegister[index]->isModified();
}



//////////////////////////////
//
// Options::getCommand -- returns argv[0] (the first string
//     in the original argv list.
//

string Options::getCommand(void) {
   if (argument.size() == 0) {
      return "";
   } else {
      return argument[0];
   }
}



//////////////////////////////
//
// Options::getCommandLine -- returns a string which contains the
//     command-line call to the program.  Deal with spaces in arguments...
//

const string& Options::getCommandLine(void) {
   if (commandString.size()) {
      return commandString;
   }

   commandString = oargv[0];

   int i;
   for (i=1; i<oargc; i++) {
      commandString += " ";
      commandString += oargv[i];
   }

   return commandString;
}




//////////////////////////////
//
// Options::getDefinition -- returns the definition for the specified
//      option name.  Returns empty string if there is no entry for
//      the option name.  spaces count in the input option name.
//

string Options::getDefinition(const string& optionName) {
   auto it = optionList.find(optionName);
   if (it == optionList.end()) {
      return "";
   } else {
      return optionRegister[it->second]->getDefinition();
   }
}



//////////////////////////////
//
// Options::getDouble -- returns the double float associated
//	with the given option.  Returns 0 if there is no
//	number associated with the option.
//

double Options::getDouble(const string& optionName) {
   return strtod(getString(optionName).c_str(), (char**)NULL);
}



//////////////////////////////
//
// Options::getChar -- Return the first character in the option string;
//      If the length is zero, then return '\0'.
//

char Options::getChar(const string& optionName) {
   return getString(optionName).c_str()[0];
}



//////////////////////////////
//
// Options::getFloat -- Return the floating point number
//	associated with the given option.
//

float Options::getFloat(const string& optionName) {
   return (float)getDouble(optionName);
}



//////////////////////////////
//
// Options::getInt -- Return the integer argument.  Can handle
//	hexadecimal, decimal, and octal written in standard
//	C syntax.
//

int Options::getInt(const string& optionName) {
   return (int)strtol(getString(optionName).c_str(), (char**)NULL, 0);
}

int Options::getInteger(const string& optionName) {
   return getInt(optionName);
}



//////////////////////////////
//
// Options::getString -- Return the option argument string.
//

string Options::getString(const string& optionName) {
   int index = getRegIndex(optionName);
   if (index < 0) {
      return "UNKNOWN OPTION";
   } else {
      return optionRegister[index]->getOption();
   }
}



//////////////////////////////
//
// Options::optionsArg -- Return true if --options is present
//    on the command line, otherwise returns false.
//

int Options::optionsArg(void) {
   return optionsArgument;
}



//////////////////////////////
//
// Options::print -- Print a list of the defined options.
//

ostream& Options::print(ostream& out) {
   for (unsigned int i=0; i<optionRegister.size(); i++) {
      out << optionRegister[i]->getDefinition() << "\t"
           << optionRegister[i]->getDescription() << endl;
   }
   return out;
}



//////////////////////////////
//
// Options::reset -- Clear all defined options.
//

void Options::reset(void) {
   unsigned int i;
   for (i=0; i<optionRegister.size(); i++) {
      delete optionRegister[i];
      optionRegister[i] = NULL;
   }
   optionRegister.clear();

   argument.clear();
   commandString.clear();
   extraArgv.clear();
   extraArgv_strings.clear();

   oargc = -1;
   oargv.clear();
}



//////////////////////////////
//
// Options::getFlag -- Set the character which is usually set to a dash.
//

char Options::getFlag(void) {
   return optionFlag;
}



//////////////////////////////
//
// Options::setFlag -- Set the character used to indicate an
//	option.  For unix this is usually '-', in MS-DOS,
//	this is usually '/';  But the syntax of the Options
//	class is for Unix-style options.
//

void Options::setFlag(char aFlag) {
   optionFlag = aFlag;
}



//////////////////////////////
//
// Options::setModified --
//

void Options::setModified(const string& optionName, const string& aString) {
   int index = getRegIndex(optionName);
   if (index < 0) {
      return;
   }

   optionRegister[getRegIndex(optionName)]->setModified(aString);
}




//////////////////////////////
//
// Options::setOptions --  Store the input list of options.
//

void Options::setOptions(int argc, char** argv) {
   processedQ = 0;

   extraArgv.resize(argc);
   extraArgv_strings.resize(argc);
   int oldsize = 0;

   int i;
   for (i=0; i<argc; i++) {
      extraArgv_strings[i+oldsize] = argv[i];
      extraArgv[i] = extraArgv_strings[i];
   }

   oargc  = extraArgv.size();
   oargv  = extraArgv;
}



//////////////////////////////
//
// Options::appendOptions -- Add argc and argv data to the current
//      list residing inside the Options class variable.
//

void Options::appendOptions(int argc, char** argv) {

   processedQ = 0;

   // data used to be stored directly here:
   //gargc = argc;
   //gargv = argv;
   // but now gets interfaced to: extraArgv and extraArgv_strings:

   int oldsize = extraArgv.size();
   extraArgv.resize(oldsize + argc);
   extraArgv_strings.resize(oldsize + argc);

   int i;
   for (i=0; i<argc; i++) {
      extraArgv_strings[i+oldsize] = argv[i];
      extraArgv[i+oldsize] = extraArgv_strings[i+oldsize];
   }

   oargc = extraArgv.size();
   oargv = extraArgv;
}


void Options::appendOptions(const vector<string>& argv) {
   processedQ = 0;

   int oldsize = extraArgv.size();
   extraArgv.resize(oldsize + argv.size());
   extraArgv_strings.resize(oldsize + argv.size());

   unsigned int i;
   for (i=0; i<argv.size(); i++) {
      extraArgv_strings[i+oldsize] = argv[i];
      extraArgv[i+oldsize] = extraArgv_strings[i+oldsize];
   }

   oargc = extraArgv.size();
   oargv = extraArgv;
}



//////////////////////////////
//
// Options::appendOptions -- parse the string like command-line arguments.
//   Either double or single quotes can be used to encapsulate
//   a command-line token.  If double quotes are used to encapsulate,
//   then you will not have to back quote single quotes inside the
//   token string, but you will have to backslash double quotes:
//      "-T \"\"" but "-T ''"
//   Likewise for single quotes in reverse with double quotes:
//      '-T \'\'' is equal to: '-T ""'
//

void Options::appendOptions(const string& strang) {
   int i;
   int doublequote = 0;
   int singlequote = 0;

   vector<string> tokens;
   vector<string> tempargv;
   string tempvalue;

   tokens.reserve(100);
   tempargv.reserve(100);
   tempvalue.reserve(1000);

   char ch;

   int length = strang.size();
   for (i=0; i<length; i++) {

      if (!singlequote && (strang[i] == '"')) {
         if ((i>0) && (strang[i-1] != '\\')) {
            doublequote = !doublequote;
            if (doublequote == 0) {
               // finished a doublequoted section of data, so store
               // even if it is the empty string
               ch = '\0';
               tempvalue += (ch);
               tokens.push_back(tempvalue);
               tempvalue.clear();
               continue;
            } else {
               // don't store the leading ":
               continue;
            }
         }
      } else if (!doublequote && (strang[i] == '\'')) {
         if ((i>0) && (strang[i-1] != '\\')) {
            singlequote = !singlequote;
            if (singlequote == 0) {
               // finished a singlequote section of data, so store
               // even if it is the empty string
               ch = '\0';
               tempvalue += ch;
               tokens.push_back(tempvalue);
               tempvalue.clear();
               continue;
            } else {
               // don't store the leading ":
               continue;
            }
         }
      }


      if ((!doublequote && !singlequote) && std::isspace(strang[i])) {
         if (tempvalue.size() > 0) {
            tempvalue += ch;
            tokens.push_back(tempvalue);
            tempvalue.clear();
         }
      } else {
         ch = strang[i];
         tempvalue += ch;
      }
   }
   if (tempvalue.size() > 0) {
      tokens.push_back(tempvalue);
      tempvalue.clear();
   }

   // now that the input string has been chopped up into pieces,
   // assemble the argv structure

   tempargv.reserve(tokens.size());
   for (i=0; i<(int)tempargv.size(); i++) {
      tempargv[i] = tokens[i];
   }

   // now store the argv and argc data in opts:

   // now store the parsed command-line simulated tokens
   // for actual storage:
   appendOptions(tempargv);
}



//////////////////////////////
//
// Options:getType -- Return the type of the option.
//

char Options::getType(const string& optionName) {
   int index = getRegIndex(optionName);
   if (index < 0) {
      return -1;
   } else {
      return optionRegister[getRegIndex(optionName)]->getType();
   }
}



//////////////////////////////
//
// Options::process -- Same as xverify.
//   	default values: error_check = 1, suppress = 0;
//

void Options::process(int argc, char** argv, int error_check, int suppress) {
   setOptions(argc, argv);
   xverify(error_check, suppress);
}


void Options::process(int error_check, int suppress) {
   xverify(error_check, suppress);
}



//////////////////////////////
//
// Options::xverify --
//	default value: error_check = 1, suppress = 0;
//

void Options::xverify(int error_check, int suppress) {
   options_error_check = error_check;
   int gargp = 1;
   int optionend = 0;

   if (suppress) {
      suppressQ = 1;
   } else {
      suppressQ = 0;
   }

   // if calling xverify again, must remove previous argument list.
   if (argument.size() != 0) {
      argument.clear();
   }

   char* tempargument;
   tempargument = new char[oargv[0].size()+1];
   strcpy(tempargument, oargv[0].c_str());
   argument.push_back(tempargument);

   int oldgargp;
   int position = 0;
   int running = 0;
   while (gargp < oargc && optionend == 0) {
      if (optionQ(oargv[gargp], gargp)) {
         oldgargp = gargp;
         gargp = storeOption(gargp, position, running);
         if (gargp != oldgargp) {
            running = 0;
            position = 0;
         }
      } else {
         if (oargv[gargp].size() == 2 && oargv[gargp][0] == getFlag() &&
            oargv[gargp][2] == getFlag() ) {
               optionend = 1;
            gargp++;
            break;
         } else {                          // this is an argument
            tempargument = new char[oargv[gargp].size()+1];
            strcpy(tempargument, oargv[gargp].c_str());
            argument.push_back(tempargument);
            gargp++;
         }
      }
   }

   while (gargp < oargc) {
      tempargument = new char[oargv[gargp].size()+1];
      strcpy(tempargument, oargv[gargp].c_str());
      argument.push_back(tempargument);
      gargp++;
   }

}


void Options::xverify(int argc, char** argv, int error_check, int suppress) {
   setOptions(argc, argv);
   xverify(error_check, suppress);
}




///////////////////////////////////////////////////////////////////////////
//
// private functions
//


//////////////////////////////
//
// Options::getRegIndex -- returns the index of the option associated
//	with this name.
//

int Options::getRegIndex(const string& optionName) {
   if (suppressQ && (optionName == "options")) {
         return -1;
   }

   if (optionName == "options") {
      print(cout);
      exit(0);
   }


   auto it = optionList.find(optionName);
   if (it == optionList.end()) {
      if (options_error_check) {
         cerr << "Error: unknown option \"" << optionName << "\"." << endl;
         print(cout);
         exit(1);
      } else {
         return -1;
      }
   } else {
      return it->second;
   }
}



//////////////////////////////
//
// Options::optionQ --  returns true if the string is an option
//	"--" is not an option, also '-' is not an option.
//	aString is assumed to not be NULL.
//

int Options::optionQ(const string& aString, int& argp) {
   if (aString[0] == getFlag()) {
      if (aString[1] == '\0') {
         argp++;
         return 0;
      } else if (aString[1] == getFlag()) {
         if (aString[2] == '\0') {
            argp++;
            return 0;
         } else {
            return 1;
         }
      } else {
         return 1;
      }
   } else {
      return 0;
   }
}



//////////////////////////////
//
// Options::storeOption --
//

#define OPTION_FORM_SHORT     0
#define OPTION_FORM_LONG      1
#define OPTION_FORM_CONTINUE  2

int Options::storeOption(int gargp, int& position, int& running) {
   int optionForm;
   char tempname[1024];
   char optionType = '\0';

   if (running) {
      optionForm = OPTION_FORM_CONTINUE;
   } else if (oargv[gargp][1] == getFlag()) {
      optionForm = OPTION_FORM_LONG;
   } else {
      optionForm = OPTION_FORM_SHORT;
   }

   switch (optionForm) {
      case OPTION_FORM_CONTINUE:
         position++;
         tempname[0] = oargv[gargp][position];
         tempname[1] = '\0';
         optionType = getType(tempname);
         if (optionType != OPTION_BOOLEAN_TYPE) {
            running = 0;
            position++;
         }
         break;
      case OPTION_FORM_SHORT:
         position = 1;
         tempname[0] = oargv[gargp][position];
         tempname[1] = '\0';
         optionType = getType(tempname);
         if (optionType != OPTION_BOOLEAN_TYPE) {
            position++;
         }
         break;
      case OPTION_FORM_LONG:
         position = 2;
         while (oargv[gargp][position] != '=' &&
               oargv[gargp][position] != '\0') {
            tempname[position-2] = oargv[gargp][position];
            position++;
         }
         tempname[position-2] = '\0';
         optionType = getType(tempname);
         if (optionType == -1) {         // suppressed --options option
            optionsArgument = 1;
            break;
         }
         if (oargv[gargp][position] == '=') {
            if (optionType == OPTION_BOOLEAN_TYPE) {
               cerr << "Error: boolean variable cannot have any options: "
                    << tempname << endl;
               exit(1);
            }
            position++;
         }
         break;
   }

   if (optionType == -1) {              // suppressed --options option
      optionsArgument = 1;
      gargp++;
      position = 0;
      return gargp;
   }

   if (oargv[gargp][position] == '\0' &&
         optionType != OPTION_BOOLEAN_TYPE) {
      gargp++;
      position = 0;
   }

   if (optionForm != OPTION_FORM_LONG && optionType == OPTION_BOOLEAN_TYPE &&
         oargv[gargp][position+1] != '\0') {
      running = 1;
   } else if (optionType == OPTION_BOOLEAN_TYPE &&
         oargv[gargp][position+1] == '\0') {
      running = 0;
   }

   if (gargp >= oargc) {
      cerr << "Error: last option requires a parameter" << endl;
      exit(1);
   }
   setModified(tempname, &oargv[gargp][position]);

   if (!running) {
      gargp++;
   }
   return gargp;
}



//////////////////////////////
//
// Options::printOptionList --
//

ostream& Options::printOptionList(ostream& out) {
   for (auto it = optionList.begin(); it != optionList.end(); it++) {
      out << it->first << "\t" << it->second << endl;
   }
   return out;
}



//////////////////////////////
//
// Options::printOptionBooleanState --
//

ostream& Options::printOptionListBooleanState(ostream& out) {
   for (auto it = optionList.begin(); it != optionList.end(); it++) {
      out << it->first << "\t"
          << optionRegister[it->second]->isModified() << endl;
   }
   return out;
}



//////////////////////////////
//
// Options::printRegister --
//

ostream& Options::printRegister(ostream& out) {
   for (auto it = optionRegister.begin(); it != optionRegister.end(); it++) {
      (*it)->print(out);
   }
   return out;
}



