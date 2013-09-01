Feature: As a developer I want to extract code into methods to make my code more readable
    @done
    Scenario: Free function without arguments
        Given source code:
        """
        #include <iostream>
        void someFunc()
        {
            std::cout << "counting" << std::endl;
            for (int i = 0; i < 10; ++i)
                std::cout << " " << i;
            std::cout << std::endl;
        }
        """
        When I run method extraction from "for (" to "<< i;" with name "runLoop"
        Then final source code should contain:
        """
        void runLoop()
        {
            for (int i = 0; i < 10; ++i)
                std::cout << " " << i;
        }
        """
        And final source code should contain:
        """
        void someFunc()
        {
            std::cout << "counting" << std::endl;
            runLoop();
            std::cout << std::endl;
        }
        """
    @done
    Scenario: Partial selection should work as full selection
        Given source code:
        """
        void f(int);
        
        void func()
        {
            for (int i = 0; i < 5; ++i)
            {
                f(i);
            }
            f(10);
            f(20);
        }
        """
        When I run method extraction from "i);" to "f(2" with name "partial"
        Then final source code should contain:
        """
        void func()
        {
            partial();
        }
        """
        And final source code should contain:
        """
        void partial()
        {
            for (int i = 0; i < 5; ++i)
            {
                f(i);
            }
            f(10);
            f(20);
        }
        """
    @done
    Scenario: Extracted function should be inserted before original function
        Given source code:
        """
        #include <iostream>
        void f(int);
        void complex()
        {
            f(1);
            std::cout << 2;
            f(2);
        }
        void other();
        """
        When I run method extraction for "cout" with name "print"
        Then final source code should be:
        """
        #include <iostream>
        void f(int);
        void print()
        {
            std::cout << 2;
        }
        void complex()
        {
            f(1);
            print();
            f(2);
        }
        void other();
        """
        