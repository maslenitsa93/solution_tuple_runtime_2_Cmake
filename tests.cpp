#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "runtime_tuple.h"

#include <utility>

class Empty {};

TEST_CASE( "any", "[any]" )
{
    runtime::Any a;
    runtime::Any b;

    REQUIRE( a.is_null() );
    REQUIRE( a.not_null() == false);
    REQUIRE( b.is_null() );

    a = 5;
    REQUIRE( a.is_null() == false);
    REQUIRE( a.not_null());
    REQUIRE( a.is<Empty>() == false);
    REQUIRE_NOTHROW(int(a));
    REQUIRE_THROWS_AS(Empty(a),std::bad_cast);
    REQUIRE( b.to_string() == "null" );
    REQUIRE( a.to_string() == "5" );
    REQUIRE( runtime::Any(Empty()).to_string() == "not provide");

}

TEST_CASE( "tuple", "[tuple]" )
{
    runtime::tuple a,b{1,"test"};
    REQUIRE( a.size() == 0 );
    REQUIRE( a.to_string() == "{}" );
    REQUIRE_THROWS_AS(a.get(2),std::out_of_range);
    REQUIRE( a != b );
    REQUIRE( a < b );
    REQUIRE( (a < a) == false);
    REQUIRE( b.to_string() == "{1,test}");
    REQUIRE( b == b );
    REQUIRE_THROWS_AS(b.get<Empty>(0),std::bad_cast);
    REQUIRE_THROWS_AS(b.get<std::string>(1),std::bad_cast);
    REQUIRE( b.get<int>(0) == 1 );
}
