#include <iostream>

#include "runtime_tuple.h"

class C{};
int main()
{
    std::cout << std::boolalpha;
    std::string s1 = "foo";

    runtime::Any a1 = s1;
    runtime::Any a2 = 5;
    runtime::Any a3 = C{};
    std::vector<runtime::tuple> v;

    std::cout<<"Before sort \n";
    v.push_back({ 2, std::string("zaz"), -0.1 });
    v.push_back({ 1, std::string("foo"), 100.1 });
    v.push_back({ 3, std::string("baz"), -0.1 });
    v.push_back({ 2, std::string("baz"), -0.1,std::string("another value") });
    v.push_back({ 1, std::string("foo"), 99.9,8 });
    v.push_back({a1,a2,a3});
    for (const auto &el: v)
        std::cout << el.to_string()<<'\n';

    std::sort(v.begin(), v.end());
    std::cout<<"After sort \n";
    for (const auto &el: v)
        std::cout << el.to_string()<<'\n';

    std::cout<<toString(v[0].get<int>(0))<<'\n';
    std::cout<<toString(v[1].get(0))<<'\n';

}
