//
//  clause_node.hpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#ifndef clause_node_hpp
#define clause_node_hpp

#include <unordered_map>
#include <vector>
#include <tuple>

#include "literal.hpp"

//todo::оптимизация на основе сортированной клаузы
//тогда для зачем???
class ClauseNode{
public:
    class Builder {
    public:
        Builder& add(int literal);
        bool isEmpty() const;
        Builder& reset();
        ClauseNode* build();
    private:
        std::vector<int> literals;
    };
    
    ClauseNode(ClauseNode& node);
    
    std::vector<int>::iterator begin();
    std::vector<int>::iterator end();
    
    bool isTrue();
    bool isTrueOnStepOrMarkFalse(int step);
    void setMark(bool _mark,int step);
    
    int amountOfLiterals();
    
    int getStep();
    
    size_t amountOfUnmarkedLiterals;
private:
    ClauseNode(std::vector<int>& lit);
    std::vector<int> literals;
    
    bool mark = false;
    int markStep = -1;
};

#endif /* clause_node_hpp */
