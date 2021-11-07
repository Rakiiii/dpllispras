//
//  main.cpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#include <iostream>
#include <chrono>
#include "sat_solver.hpp"
#include "dmacs_parcer.hpp"

int main(int argc, const char * argv[]) {
    std::vector<std::string> argList(argv, argv + argc);

    auto argsPath = argList.begin();
    ++argsPath;
    for(;argsPath != argList.end(); ++ argsPath){
        std::string path = *argsPath;
        
        LogicFunction *logicFunction = DMACSParser::parse(path);
        
        std::cout << "/---------------Start DPLL-----------/"<<std::endl<< std::endl;
        
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        
        auto startTime = high_resolution_clock::now();
//        auto result = SATSolver::solveTest(*logicFunction);
//        auto result = SATSolver::solveHighPerfomance(*logicFunction);
        auto result = SATSolver::solveHighPerfomanceNoStack(*logicFunction);
        auto endTime = high_resolution_clock::now();
        
        auto duration = duration_cast<milliseconds>(endTime - startTime);
        
        std::cout<<"Operation longs: "<<duration.count()<<std::endl<<std::endl;
        
        if(result) {
            std::cout << "SAT: ";
            for(int i = 1; i <= logicFunction->getAmountOfLiterals();i++) std::cout << (*result)[i] << " ";
            std::cout << std::endl;
            
            std::cout<<"isCorrect: "<<(logicFunction->isCorrectOnVector() ? "CORRECT" : "INCORRECT")<<std::endl;
        } else {
            std::cout << "UNSAT" << std::endl;
        }
    }
    return 0;
}
