//
//  dmacs_parcer.hpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#ifndef dmacs_parcer_hpp
#define dmacs_parcer_hpp

#include "clause_node.hpp"
#include "logic_function.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

class DMACSParser {
public:
    static LogicFunction* parse(const std::string &path);
};

#endif /* dmacs_parcer_hpp */
