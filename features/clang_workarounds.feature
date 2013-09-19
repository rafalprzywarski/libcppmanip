Feature: As I developer I want to use this library despite clang sortcomings
    @done
    Scenario: Extract method should run more than once
        Given source code:
        """
        #include <iostream>
        void f()
        {
            std::cout << 1;
        }
        void g()
        {
            std::cout << 2;
        }
        """
        When I run two function extractions for "std::cout << 2" with name "eg" and for "std::cout << 1" with name "ef"
        Then there should be a replacement with "eg();"
        And there should be a replacement with "ef();"
