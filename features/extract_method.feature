Feature: As a developer I want to extract code into method to make my code more readable
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
        Then final source code should be:
        """
        void runLoop()
        {
            for (int i = 0; i < 10; ++i)
                std::cout << " " << i;
        }
        void someFunc()
        {
            std::cout << "counting" << std::endl;
            runLoop();
            std::cout << std::endl;
        }
        """
