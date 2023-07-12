#include <catch.hpp>
#include <iostream>
#include <scope_guard/scope_guard.hpp>

struct point {

    point() = default;

    point(int x_, int y_)
        : x {x_},
          y {y_} {
    }

    int x {};
    int y {};
};

static bool free_called {false};

void free_delete(point& p) {
    delete &p;
    free_called = true;
}

template<typename T>
class default_deleter {
public:
    void operator()(T& object) {
        delete &object;
        called = true;
    }

    bool called {false};
};

TEST_CASE("Scope Guard Tests", "[unit tests]") {
    auto ptr = new point {33, 27};
    SECTION("Constructing a scope_guard with an lambda") {

        bool delete_called {false};

        {
            allstdcpp::guard::scope_guard<point> sg {*ptr, [&delete_called](point& p) {
                                                         delete &p;
                                                         delete_called = true;
                                                     }};
        }

        REQUIRE(delete_called);
    }
    SECTION("Constructing a scope_guard with an functor") {

        default_deleter<point> deleter {};

        allstdcpp::guard::scope_guard<point> sg {*ptr, deleter};
        sg.destroy();

        auto d = sg.deleter();
        auto t = d.target<default_deleter<point>>();

        REQUIRE(t != nullptr);
        REQUIRE(t->called);
    }
    SECTION("Constructing a scope_guard with an free function") {

        free_called = false;
        allstdcpp::guard::scope_guard<point> sg {*ptr, free_delete};
        { allstdcpp::guard::scope_guard<point> sg {*ptr, free_delete}; }

        REQUIRE(free_called);
    }
    SECTION("A scope_guard with release called wont destroy the object") {
        free_called = false;
        {
            allstdcpp::guard::scope_guard<point> sg {*ptr, free_delete};
            sg.release();
        }
        REQUIRE_FALSE(free_called);
    }
}
