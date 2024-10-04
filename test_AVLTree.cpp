#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "AVLTree.h"

TEST_CASE("AVL Tree Map", "[avl_tree_map]") {
    AvlTree<int, std::string> map;

    SECTION("Insertion and Access") {
        map.insert(5, "five");
        map.insert(3, "three");
        map.insert(7, "seven");

        REQUIRE(map.contains(5));
        REQUIRE(map.contains(3));
        REQUIRE(map.contains(7));
        REQUIRE_FALSE(map.contains(2));
        REQUIRE_FALSE(map.contains(4));
        REQUIRE_FALSE(map.contains(6));

        REQUIRE(map.at(5) == "five");
        REQUIRE(map.at(3) == "three");
        REQUIRE(map.at(7) == "seven");

        REQUIRE(map[5] == "five");
        REQUIRE(map[3] == "three");
        REQUIRE(map[7] == "seven");
    }

    SECTION("Insertion and Removal") {
        map.insert(5, "five");
        map.insert(3, "three");
        map.insert(7, "seven");
        map.insert(1, "one");
        map.insert(9, "nine");

        REQUIRE(map.contains(5));
        REQUIRE(map.contains(3));
        REQUIRE(map.contains(7));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(9));

        map.remove(3);
        REQUIRE_FALSE(map.contains(3));
        REQUIRE(map.contains(5));
        REQUIRE(map.contains(7));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(9));

        map.remove(7);
        REQUIRE_FALSE(map.contains(3));
        REQUIRE(map.contains(5));
        REQUIRE_FALSE(map.contains(7));
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(9));

        map.remove(1);
        REQUIRE_FALSE(map.contains(3));
        REQUIRE(map.contains(5));
        REQUIRE_FALSE(map.contains(7));
        REQUIRE_FALSE(map.contains(1));
        REQUIRE(map.contains(9));

        map.remove(9);
        REQUIRE_FALSE(map.contains(3));
        REQUIRE(map.contains(5));
        REQUIRE_FALSE(map.contains(7));
        REQUIRE_FALSE(map.contains(1));
        REQUIRE_FALSE(map.contains(9));

        map.remove(5);
        REQUIRE(map.isEmpty());
    }

    SECTION("Update Value") {
        map.insert(5, "five");
        REQUIRE(map.at(5) == "five");

        map.insert(5, "updated_five");
        REQUIRE(map.at(5) == "updated_five");
    }

    SECTION("Exception Handling") {
        REQUIRE_THROWS_AS(map.at(3), std::out_of_range);

        map.insert(5, "five");
        REQUIRE_NOTHROW(map.at(5));

        map.remove(5);
        REQUIRE_THROWS_AS(map.at(5), std::out_of_range);
    }

    SECTION("Empty Check") {
        REQUIRE(map.isEmpty());

        map.insert(5, "five");
        REQUIRE_FALSE(map.isEmpty());

        map.remove(5);
        REQUIRE(map.isEmpty());
    }

    SECTION("Operator[]") {
        REQUIRE(map.isEmpty());

        map[3] = "three";
        REQUIRE(map.contains(3));
        REQUIRE(map.at(3) == "three");

        map[3] = "updated_three";
        REQUIRE(map.at(3) == "updated_three");

        map[7] = "seven";
        REQUIRE(map.contains(7));
        REQUIRE(map.at(7) == "seven");
    }

    SECTION("Const Access") {
        map.insert(5, "five");
        map.insert(3, "three");
        map.insert(7, "seven");

        const auto &const_map = map;

        REQUIRE(const_map.contains(5));
        REQUIRE(const_map.contains(3));
        REQUIRE(const_map.contains(7));

        REQUIRE(const_map.at(5) == "five");
        REQUIRE(const_map.at(3) == "three");
        REQUIRE(const_map.at(7) == "seven");

        REQUIRE_THROWS_AS(const_map.at(2), std::out_of_range);
    }

    SECTION("Copy Constructor and Assignment Operator") {
        map.insert(5, "five");
        map.insert(3, "three");
        map.insert(7, "seven");

        AvlTree<int, std::string> copy_constructed(map);
        REQUIRE(copy_constructed.contains(5));
        REQUIRE(copy_constructed.contains(3));
        REQUIRE(copy_constructed.contains(7));

        AvlTree<int, std::string> copy_assigned;
        copy_assigned = map;
        REQUIRE(copy_assigned.contains(5));
        REQUIRE(copy_assigned.contains(3));
        REQUIRE(copy_assigned.contains(7));
    }
}