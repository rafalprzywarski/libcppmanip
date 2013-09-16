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
        When I run two method extractions for "std::cout << 2" with name "eg" and for "std::cout << 1" with name "ef"
        Then final source code should contain:
        """
        void g()
        {
            eg();
        }
        """
        And final source code should contain:
        """
        void f()
        {
            ef();
        }
        """
