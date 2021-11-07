//
//  dmacs_parcer.cpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#include "dmacs_parcer.hpp"

LogicFunction* DMACSParser::parse(const std::string& path) {
    
    std::ifstream inputStream(path);
    
    if( !inputStream.is_open() ) return NULL;//throw std::runtime_error("could not open file at " + path);
    
    std::vector<ClauseNode*> logicFunction;
    
    std::string cnf;
    
    int amountOfLiterals = 0;
    int amountOfClauses = 0;
    
    //проваливаемся до параметров
    for(std::string line; std::getline(inputStream, line);) {
        
        if(line.empty() || line[0] == 'c') continue;
        
        if (line[0] != 'p') {
//            return NULL;
            throw std::runtime_error("The 'p' symbol was expected while parcing, but find line: " + line);
        } else {
            
            char p;
            
            std::stringstream deconstructStream(line);
            
            deconstructStream >> p >> cnf >> amountOfLiterals >> amountOfClauses;
            
            if(cnf != "cnf") {
//                return NULL;
                throw std::runtime_error("No cnf found or file end found, cnf state: " + cnf);
            }
            
            break;
        }
    }
    
    auto clauseNodeBuileder = new ClauseNode::Builder();
    
    for(std::string line; std::getline(inputStream, line);) {
    
        if(line.empty()|| line[0] == 'c' || line[0] == '%') continue;
        
        if (line[0] == 'p') {
//            return NULL;
            throw std::runtime_error("Found unexpected parameter symbol at line: " + line);
        }
        
        int literal;
        
        std::stringstream deconstructionStream(line);
        
        while(deconstructionStream >> literal && literal != 0) clauseNodeBuileder->add(literal);

        if(literal == 0) {
            if (!clauseNodeBuileder->isEmpty()){
                logicFunction.push_back(clauseNodeBuileder->build());
                clauseNodeBuileder->reset();
            }
        }
    }
    
    return new LogicFunction(logicFunction, amountOfLiterals);
}
