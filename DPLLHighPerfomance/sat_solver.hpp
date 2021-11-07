//
//  sat_solver.hpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#ifndef sat_solver_hpp
#define sat_solver_hpp

#include "logic_function.hpp"
#include <optional>
#include <stack>
#include <iostream>

class Pair {
public:
    Pair(int l,bool b) : literal(l), branch(b) {}
    int literal;
    bool branch;
};

class SATSolver {
public:
    static std::optional<int*> solveTest(LogicFunction& logicFunction);
    static std::optional<int*> solve(LogicFunction& logicFunction);
    static std::optional<int*> solveHighPerfomance(LogicFunction& logicFunction);
    static std::optional<int*> solveHighPerfomanceNoStack(LogicFunction& logicFunction);
// static void log(std::vector<std::tuple<int, std::vector<int>*>>* changedLiterals);
};

#endif /* sat_solver_hpp */
