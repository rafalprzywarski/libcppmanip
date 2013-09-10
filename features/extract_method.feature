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
    @done
    Scenario: Referenced local variables should be passed to the extracted function
        Given source code:
        """
        void other1(int);
        void other2();
        void other3(int, float);
        void original()
        {
            int i = 7;
            float f = 9.5f;
            other1(i);
            other2();
            other3(i + 5, f * 2);
        }
        """
        When I run method extraction from "other1(i)" to "other3(i + 5" with name "with_args"
        Then final source code should contain:
        """
        void with_args(int i, float f)
        {
            other1(i);
            other2();
            other3(i + 5, f * 2);
        }
        """
        And final source code should contain:
        """
        with_args(i, f);
        """
    @wip
    Scenario: Extraction of local variables used after the selected block should prevent the extraction
        Given source code:
        """
        void other3(int, int);
        void original()
        {
            int i = 7, j = 9;
            i += 2;
            j += 5;
            other3(i, j);
        }
        """
        When I run method extraction from "int i = 7" to "j += 5" with name "bad"
        Then it should fail with a message "Cannot extract 'bad'. Following variables are in use after the selected statements: i, j"
        And original code should not change
