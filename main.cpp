#include "Menu/MainMenu.h"
#include "Tests/TestRunner.h"
#include <iostream>

void showUsageOptions()
{
    std::cout << "\n=== Performance Testing Options ===" << std::endl;
    std::cout << "1. Run all tests (comprehensive)" << std::endl;
    std::cout << "2. Run MST tests only" << std::endl;
    std::cout << "3. Run Shortest Path tests only" << std::endl;
    std::cout << "4. Run Max Flow tests only" << std::endl;
    std::cout << "5. Show main menu" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose option: ";
}

void runPerformanceTests()
{
    int choice;
    int generationCount = 50; // Default number of test runs per configuration
    
    std::cout << "Enter number of test runs per configuration (default 50): ";
    std::cin >> generationCount;
    if (generationCount <= 0) generationCount = 50;
    
    while (true)
    {
        showUsageOptions();
        std::cin >> choice;
        
        switch (choice)
        {
            case 1:
                std::cout << "\nRunning comprehensive performance tests..." << std::endl;
                TestRunner::Run(generationCount);
                break;
                
            case 2:
                std::cout << "\nRunning MST algorithm tests..." << std::endl;
                TestRunner::RunForProblemType(GraphProblemType::MST, generationCount);
                break;
                
            case 3:
                std::cout << "\nRunning Shortest Path algorithm tests..." << std::endl;
                TestRunner::RunForProblemType(GraphProblemType::SHORTEST_PATH, generationCount);
                break;
                
            case 4:
                std::cout << "\nRunning Max Flow algorithm tests..." << std::endl;
                TestRunner::RunForProblemType(GraphProblemType::MAX_FLOW, generationCount);
                break;
                
            case 5:
                return; // Go back to main menu
                
            case 0:
                std::cout << "Exiting..." << std::endl;
                exit(0);
                
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
        
        if (choice >= 1 && choice <= 4)
        {
            std::cout << "\n=== Next Steps ===" << std::endl;
            std::cout << "To generate charts, run:" << std::endl;
            std::cout << "  cd Python" << std::endl;
            std::cout << "  python Type1ChartGenerator.py" << std::endl;
            std::cout << "  python Type2ChartGenerator.py" << std::endl;
            std::cout << std::endl;
        }
    }
}

int main()
{
    std::cout << "=== Graph Algorithms Performance Analysis ===" << std::endl;
    std::cout << "This program tests various graph algorithms and generates performance data." << std::endl;
    
    int choice;
    std::cout << "\nChoose mode:" << std::endl;
    std::cout << "1. Run Performance Tests" << std::endl;
    std::cout << "2. Interactive Menu" << std::endl;
    std::cout << "Choice: ";
    std::cin >> choice;
    
    switch (choice)
    {
        case 1:
            runPerformanceTests();
            break;
            
        case 2:
            {
                MainMenu menu;
                menu.show();
            }
            break;
            
        default:
            std::cout << "Invalid choice. Starting interactive menu..." << std::endl;
            MainMenu menu;
            menu.show();
            break;
    }
    
    return 0;
}