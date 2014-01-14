// -*- mode: c++; c-basic-offset: 4; -*-

// Author: Hong Jiang <hong@hjiang.net>
// Include a few sanity tests from https://github.com/isubiker/mljson/
// Include a few samples from http://www.json.org

#include <cassert>
#include <sstream>
#include <string>
#include <iostream>

#include "jsonxx.h"

namespace jsonxx {
    extern bool parse_string(std::istream& input, String& value);
    extern bool parse_number(std::istream& input, Number& value);
    extern bool match(const char* pattern, std::istream& input);
}

bool is_asserting() {
    bool asserting = false;
    JSONXX_ASSERT( asserting |= true );
    return asserting;
}

struct custom_type {};      // Used in a test elsewhere

int main(int argc, const char **argv) {

    if( !is_asserting() ) {
        std::cout << "JSONXX_ASSERT(...) is not working. Assertions are disabled somehow. Tests aborted." << std::endl;
        return -1;
    }

    bool verbose = false;
    if( argc > 1 && (std::string(argv[1]) == "-v")) {
        verbose = true;
    }

#   define TEST_ARRAY(...)  TEST( Array().parse(istringstream(#__VA_ARGS__).str()) )
#   define TEST_OBJECT(...) TEST( Object().parse(istringstream(#__VA_ARGS__).str()) )
#   define TEST(...) do { JSONXX_ASSERT( __VA_ARGS__ ); \
    if( verbose ) std::cout << "[OK] " #__VA_ARGS__ << std::endl; \
    } while(0)

    using namespace jsonxx;
    using namespace std;
    {
        string teststr("\"field1\"");
        string value;
        istringstream input(teststr);
        TEST(parse_string(input, value));
        TEST(value == "field1");
    }
    if( !Strict )
    {
        string teststr("'field1'");
        string value;
        istringstream input(teststr);
        TEST(parse_string(input, value));
        TEST(value == "field1");
    }
    {
        string teststr("\"  field1\"");
        string value;
        istringstream input(teststr);
        TEST(parse_string(input, value));
        TEST(value == "  field1");
    }
    if( !Strict )
    {
        string teststr("'  field1'");
        string value;
        istringstream input(teststr);
        TEST(parse_string(input, value));
        TEST(value == "  field1");
    }
    {
        string teststr("  \"field1\"");
        string value;
        istringstream input(teststr);
        TEST(parse_string(input, value));
        TEST(value == "field1");
    }
    if( !Strict )
    {
        string teststr("  'field1'");
        string value;
        istringstream input(teststr);
        TEST(parse_string(input, value));
        TEST(value == "field1");
    }
    {
        // 'escaped text to unescaped text' test
        string teststr("\"\\b\\f\\n\\r\\t\\u0014\\u0002\"");
        string value;
        istringstream input(teststr);
        TEST(parse_string(input, value));
        TEST( value == "\b\f\n\r\t\xe\x2" );
    }
    {
        string teststr("6");
        istringstream input(teststr);
        Number value;
        TEST(parse_number(input, value));
        TEST(value == 6);
    }
    {
        string teststr(" }");
        istringstream input(teststr);
        TEST(match("}", input));
    }
    {
        string teststr("{ \"field1\" : 6 }");
        istringstream input(teststr);
        Object o;
        TEST(o.parse(input));
    }
    {
        string teststr("{ \"field1 : 6 }");
        istringstream input(teststr);
        Object o;
        TEST(!o.parse(input));
    }
    {
        string teststr("6");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<Number>());
        TEST(v.get<Number>() == 6);
    }
    {
        string teststr("+6");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<Number>());
        TEST(v.get<Number>() == 6);
    }
    {
        string teststr("-6");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<Number>());
        TEST(v.get<Number>() == -6);
    }
    {
        string teststr("asdf");
        istringstream input(teststr);
        Value v;
        TEST(!v.parse(input));
    }
    {
        string teststr("true");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<Boolean>());
        TEST(v.get<Boolean>());
    }
    {
        string teststr("false");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<Boolean>());
        TEST(!v.get<Boolean>());
    }
    {
        string teststr("null");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<Null>());
        TEST(!v.is<Boolean>());
    }
    {
        string teststr("\"field1\"");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<String>());
        TEST("field1" == v.get<String>());
        ostringstream stream;
        stream << v;
        TEST(stream.str() == "\"field1\"");
    }
    if( !Strict )
    {
        string teststr("'field1'");
        istringstream input(teststr);
        Value v;
        TEST(v.parse(input));
        TEST(v.is<String>());
        TEST("field1" == v.get<String>());
        ostringstream stream;
        stream << v;
        TEST(stream.str() == "\"field1\"");
    }
    {
        string teststr("[\"field1\", 6]");
        istringstream input(teststr);
        Array a;
        TEST(a.parse(input));
        TEST(a.has<String>(0));
        TEST("field1" == a.get<String>(0));
        TEST(a.has<Number>(1));
        TEST(6 == a.get<Number>(1));
        TEST(!a.has<Boolean>(2));
    }
    {
        string teststr(
                "{"
                "  \"foo\" : 1,"
                "  \"bar\" : false,"
                "  \"person\" : {\"name\" : \"GWB\", \"age\" : 60},"
                "  \"data\": [\"abcd\", 42, 54.7]"
                "}"
                       );
        istringstream input(teststr);
        Object o;
        TEST(o.parse(input));
        TEST(1 == o.get<Number>("foo"));
        TEST(o.has<Boolean>("bar"));
        TEST(o.has<Object>("person"));
        TEST(o.get<Object>("person").has<Number>("age"));
        TEST(o.has<Array>("data"));
        TEST(o.get<Array>("data").get<Number>(1) == 42);
        TEST(o.get<Array>("data").get<String>(0) == "abcd");
        TEST(o.get<Array>("data").get<Number>(2) - 54.7 < 1e-6 ||
             - o.get<Array>("data").get<Number>(2) + 54.7 < 1e-6 );
        TEST(!o.has<Number>("data"));
    }
    {
        string teststr("{\"bar\": \"a\\rb\\nc\\td\", \"foo\": true}");
        istringstream input(teststr);
        Object o;
        TEST(o.parse(input));
        TEST(o.has<String>("bar"));
        TEST(o.get<String>("bar") == "a\rb\nc\td");
    }
    {
        string teststr("[ ]");
        istringstream input(teststr);
        ostringstream output;
        Array root;
        TEST(root.parse(input));
        output << root;
    }

    {
        string teststr("{}");
        istringstream input(teststr);
        Object o;
        TEST(o.parse(input));
    }

    {
        string teststr("{\"attrs\":{}}");
        istringstream input(teststr);
        Object o;
        TEST(o.parse(input));
        TEST(o.has<Object>("attrs"));
    }

    {
        string teststr("{\"place\":{\"full_name\":\"Limburg, The Netherlands\""
                       ",\"attributes\":{},\"name\":\"Limburg\","
                       "\"place_type\":\"admin\",\"bounding_box\":{"
                       "\"type\":\"Polygon\",\"coordinates\":"
                       "[[[5.5661376,50.750449],[6.2268848,50.750449],"
                       "[6.2268848,51.7784841],[5.5661376,51.7784841]]]},"
                       "\"url\":\"http:\\/\\/api.twitter.com\\/1\\/geo\\/id\\/"
                       "4ef0c00cbdff9ac8.json\",\"country_code\":\"NL\","
                       "\"id\":\"4ef0c00cbdff9ac8\","
                       "\"country\":\"The Netherlands\"}}");
        istringstream input(teststr);
        Object o;
        TEST(o.parse(input));
        TEST(o.has<Object>("place"));
        TEST(o.get<Object>("place").has<Object>("attributes"));
    }

    if( !Strict )
    {
        #define QUOTE(...) #__VA_ARGS__
        string input = QUOTE(
        {
          "name/surname":"John Smith",
          'alias': 'Joe',
          "address": {
            "streetAddress": "21 2nd Street",
            "city": "New York",
            "state": "NY",
            "postal-code": 10021,
          },
          "phoneNumbers": [
            "212 555-1111",
            "212 555-2222",
          ],
          "additionalInfo": null,
          "remote": false,
          "height": 62.4,
          "ficoScore": " > 640",
        }
        );

        string sample_output = QUOTE(
        <?xml version="1.0" encoding="UTF-8"?>
        <json:object xsi:schemaLocation="http://www.datapower.com/schemas/json jsonx.xsd"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:json="http://www.ibm.com/xmlns/prod/2009/jsonx">
          <json:string name="name/surname">John Smith</json:string>
          <json:string name="alias">Joe</json:string>
          <json:object name="address">
            <json:string name="streetAddress">21 2nd Street</json:string>
            <json:string name="city">New York</json:string>
            <json:string name="state">NY</json:string>
            <json:number name="postal-code">10021</json:number>
          </json:object>
          <json:array name="phoneNumbers">
            <json:string>212 555-1111</json:string>
            <json:string>212 555-2222</json:string>
          </json:array>
          <json:null name="additionalInfo" />
          <json:boolean name="remote">false</json:boolean>
          <json:number name="height">62.4</json:number>
          <json:string name="ficoScore">&gt; 640</json:string>
        </json:object>
        );

        Object o;
        if( o.parse(input) ) {
            if( verbose ) {
                cout << o.xml(JSONx) << endl;            // XML output, JSONx flavor
                cout << o.xml(JXML) << endl;             // XML output, JXML flavor
                cout << o.xml(JXMLex) << endl;           // XML output, JXMLex flavor
                cout << o.xml(TaggedXML) << endl;        // XML output, tagged XML flavor
            }
        } else {
            TEST( !"provided JSON is valid. jsonxx::Object::operator<<() is broken!" );
        }

        TEST( jsonxx::validate(input) );
    }

    // Four samples from www.json.org
    TEST_OBJECT( {
        "name": "Jack (\"Bee\") Nimble",
        "format": {
            "type":       "rect",
            "width":      1920,
            "height":     1080,
            "interlace":  false,
            "frame rate": 24
        }
    } );
    TEST_OBJECT( { "color": "blue", "closed": true, "points": [[10,10], [20,10], [20,20], [10,20]] } );
    TEST_ARRAY( [ [0, -1, 0], [1, 0, 0], [0, 0, 1] ] );
    TEST_ARRAY( ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"] );

    // Empty/raw datatypes
    TEST_ARRAY( [true, false, null, [], {}] );

    // Various numbers
    TEST_ARRAY( [[-1], [1.2]] );

    // General array with all data types
    TEST_ARRAY( ["hello", 0, [], {}, null, false, true] );

    // Nested objects
    TEST_OBJECT( {"foo":1, "bar":{"baz":2, "yaz":3}} );

    // Nested objects with trailing key/value
    TEST_OBJECT( {"foo":1, "em":{"a":"b"}, "bar":"aa"} );

    // Nested arrays
    TEST_ARRAY( [1, 2, 3, [4, 5, [7, 8, 9], 6]] );

    // Nested arrays with trailing values
    TEST_ARRAY( [1, 2, 3, [4, 5, [7, 8, 9], 6], 10] );

    // UTF-8
    TEST_OBJECT( {"text":"は 2010/11/4 at 5:50 AM に 6'45\"/km のペースで 8.42 km を走りました http://go.nike.com/9rlcovd"} );

    // Escaped UTF-8
    TEST_OBJECT( {"text":"\u3050\u3089\u307e\u3041\u3067\u3061\u3085\u306d\u2665\u304a\u306f\u3088\u3046\u3067\u3059\uff01"} );

    // Empty values
    TEST_OBJECT( {"foo":"", "bar":""} );

    // Escaped quotes
    TEST_OBJECT( {"foo":"\"bar\""} );

    // Newline
    TEST_OBJECT( {"foo":"bar\nbaz"} );

    // Could use more tests around escaping odd characters in key names that can't be used as an element name
    // Escaping invalid xml element names
    TEST_OBJECT( {"f•o":"bar"} );

    // Escaping our invalid xml element name escaping
    TEST_OBJECT( {"_foo":"bar"} );

    // Empty key name (about to check in a fix)
    TEST_OBJECT( {"":"bar"} );

    // Trailing commas (if permissive mode is enabled)
    if( !jsonxx::Strict ) {
        TEST_ARRAY( [ true, 42, 54.7, ] );
        TEST_OBJECT( { "hello": "world",} );
    }

    // Single-quoted strings (if permissive mode is enabled)
    if( !jsonxx::Strict ) {
        TEST_OBJECT( { 'single-quoted-strings': 'are "handy"' } );
    }

    {
        jsonxx::Array a;
        a << true;
        a << false;
        a << 'A';
        a << 65;
        a << 65L;
        a << 65LL;
        a << 65U;
        a << 65UL;
        a << 65ULL;
        a << 65.f;
        a << 65.0;
        a << jsonxx::Value( "hello world" );
        a << std::string("hello world");
        a << "hello world";

        TEST( jsonxx::Array().parse( a.json() ) );   // self-evaluation
        TEST( validate( a.json() ) );                // self-evaluation
    }

    {
        jsonxx::Object o;
        o << "hello" << "world";
        o << "hola" << "mundo";
        TEST( o.get<String>("hello") == "world" );
        TEST( o.get<String>("hola") == "mundo" );
        o << "hello" << "mundo";
        o << "hola" << "world";
        TEST( o.get<String>("hello") == "mundo" );
        TEST( o.get<String>("hola") == "world" );
    }

    {
        // Generate JSON document dinamically
        jsonxx::Array a;
        a << jsonxx::Null();           // C++11: 'a << nullptr' is preferred
        a << 123;
        a << "hello \"world\"";
        a << 3.1415;
        a << 99.95f;
        a << 'h';

        jsonxx::Object o;
        o << "key1" << "value";
        o << "key2" << 123;
        o << "key3" << a;

        a << o;

        TEST( jsonxx::Array().parse( a.json() ) );   // self-evaluation
        TEST( validate( a.json() ) );                // self-evaluation
    }

    {
        // Generate JSON document dinamically
        jsonxx::Object o1;
        o1 << "key1" << "value 1";
        o1 << "key2" << 123;

        jsonxx::Object o2;
        o2 << "key3" << "value 2";
        o2 << "key4" << 456;

        o1 << "key3" << o2;

        TEST( jsonxx::Object().parse( o1.json() ) );  // self-evaluation
        TEST( validate( o1.json() ) );                // self-evaluation
    }

    {
        jsonxx::Array a;
        a << true;
        a << false;

        jsonxx::Object o;
        o << "number" << 123;
        o << "string" << "hello world";
        o << "boolean" << false;
        o << "null" << NULL;
        o << "array" << a;
        o << "object" << jsonxx::Object("child", "object");
        o << "undefined" << custom_type();

        if( verbose ) {
            cout << o.write(JSON) << std::endl;        // same than o.json()
            cout << o.write(JSONx) << std::endl;       // same than o.xml()
            cout << o.write(JXML) << std::endl;        // same than o.xml(JXML)
            cout << o.write(JXMLex) << std::endl;      // same than o.xml(JXMLex)
        }

        TEST( jsonxx::Object().parse( o.json() ) );   // self-evaluation
        TEST( validate( o.json() ) );                 // self-evaluation
    }

    {
        // recursion test
        jsonxx::Array a;
        a << 123;
        a << "hello world";
        a << a;
        TEST( a.size() == 4 );
    }

    {
        // recursion test
        jsonxx::Object o;
        o << "number" << 123;
        o << "string" << "world";
        o << "recursion" << o;
        TEST( o.size() == 3 );
    }

    if( !jsonxx::Strict )
    {
        // C++ style comments test
        string teststr(
                "//this is comment #1 {\n"
                "{"
                "  \"foo\" : 1,"
                "  \"bar\" : false, //this is comment #2\n"
                "  \"person\" : {\"name //this shall not be removed\" : \"GWB\", \"age\" : 60},"
                "  \"data\": [\"abcd\", 42, 54.7]"
                "} //this is comment #3"
       );
        jsonxx::Object o;
        TEST( o.parse(teststr) );
    }

    cout << "All tests ok." << endl;
    return 0;
}
