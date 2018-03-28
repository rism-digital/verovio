//
// Copyright 1998-2000 by Craig Stuart Sapp, All Rights Reserved.
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr  5 13:07:18 PDT 1998
// Last Modified: Sat Mar  1 09:27:49 PST 2014 Implemented with STL.
// Filename:      Options.h
// Web Address:   https://github.com/craigsapp/optionlib/blob/master/include/Options.h
// Documentation: http://sig.sapp.org/doc/classes/Options
// Syntax:        C++11
//
// Description:   Interface to command-line options.
//

#ifndef _OPTIONS_H_INCLUDED
#define _OPTIONS_H_INCLUDED

#include <vector>
#include <map>
#include <string>

using namespace std;

class Option_register {
   public:
                   Option_register    (void);
                   Option_register    (const string& aDefinition, char aType,
                                       const string& aDefaultOption);
                   Option_register    (const string& aDefinition, char aType,
                                       const string& aDefaultOption,
                                       const string& aModifiedOption);
                  ~Option_register    ();

     void          clearModified      (void);
     const string& getDefinition      (void);
     const string& getDefault         (void);
     const string& getOption          (void);
     const string& getModified        (void);
     const string& getDescription     (void);
     int           isModified         (void);
     char          getType            (void);
     void          reset              (void);
     void          setDefault         (const string& aString);
     void          setDefinition      (const string& aString);
     void          setDescription     (const string& aString);
     void          setModified        (const string& aString);
     void          setType            (char aType);
     ostream&      print              (ostream& out);

   protected:
      string       definition;
      string       description;
      string       defaultOption;
      string       modifiedOption;
      int          modifiedQ;
      char         type;

};



class Options {
   public:
                      Options           (void);
                      Options           (int argc, char** argv);
                     ~Options           ();

      int             argc              (void) const;
      const vector<string>& argv        (void) const;
      int             define            (const string& aDefinition);
      int             define            (const string& aDefinition,
                                         const string& description);
      const string&   getArg            (int index);
      const string&   getArgument       (int index);
      int             getArgCount       (void);
      int             getArgumentCount  (void);
      const vector<string>& getArgList  (void);
      const vector<string>& getArgumentList (void);
      int             getBoolean        (const string& optionName);
      string          getCommand        (void);
      const string&   getCommandLine    (void);
      string          getDefinition     (const string& optionName);
      double          getDouble         (const string& optionName);
      char            getFlag           (void);
      char            getChar           (const string& optionName);
      float           getFloat          (const string& optionName);
      int             getInt            (const string& optionName);
      int             getInteger        (const string& optionName);
      string          getString         (const string& optionName);
      char            getType           (const string& optionName);
      int             optionsArg        (void);
      ostream&        print             (ostream& out);
      ostream&        printOptionList   (ostream& out);
      ostream&        printOptionListBooleanState(ostream& out);
      void            process           (int error_check = 1, int suppress = 0);
      void            process           (int argc, char** argv,
                                            int error_check = 1,
                                            int suppress = 0);
      void            reset             (void);
      void            xverify           (int argc, char** argv,
                                            int error_check = 1,
                                            int suppress = 0);
      void            xverify           (int error_check = 1,
                                            int suppress = 0);
      void            setFlag           (char aFlag);
      void            setModified       (const string& optionName,
                                         const string& optionValue);
      void            setOptions        (int argc, char** argv);
      void            appendOptions     (int argc, char** argv);
      void            appendOptions     (const string& strang);
      void            appendOptions     (const vector<string>& argv);
      ostream&        printRegister     (ostream& out);
      int             isDefined         (const string& name);

   protected:
      int                      options_error_check;  // for verify command
      int                      oargc;
      vector<string>           oargv;
      string                   commandString;
      char                     optionFlag;
      vector<string>           argument;

      vector<Option_register*> optionRegister;
      map<string, int>         optionList;

      int                      processedQ;
      int                      suppressQ;       // prevent the --options option
      int                      optionsArgument; // indicates --options present

      vector<string>           extraArgv;
      vector<string>           extraArgv_strings;

      int         getRegIndex             (const string& optionName);
      int         optionQ                 (const string& aString, int& argp);
      int         storeOption             (int gargp, int& position,
                                             int& running);

};

#define OPTION_BOOLEAN_TYPE   'b'
#define OPTION_CHAR_TYPE      'c'
#define OPTION_DOUBLE_TYPE    'd'
#define OPTION_FLOAT_TYPE     'f'
#define OPTION_INT_TYPE       'i'
#define OPTION_STRING_TYPE    's'
#define OPTION_UNKNOWN_TYPE   'x'



#endif  /* _OPTIONS_H_INCLUDED */

