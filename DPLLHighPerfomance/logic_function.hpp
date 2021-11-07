//
//  logic_function.hpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#ifndef logic_function_hpp
#define logic_function_hpp

#include <optional>
#include <set>
#include <iostream>
#include <cstring>
#include "clause_node.hpp"


const int DELETED = -1;
const int NOT_CHEKED = -2;
const int DELETED_OLD = 0;
const int POSITIVE = 1;
const int NEGATIVE = 2;

const char CPOSITIVE = 'd';
const char CNEGATIVE = 'c';
const char CDELETED = 'a';
const char CNOT_CHEKED = 'b';

class LogicFunction {
public:
    LogicFunction(std::vector<ClauseNode*>& lf, int _amountOfLiterals);
    ~LogicFunction();
    
    void printMarkedLiterals();
    
    std::optional<int> getFirstUnmarkedLiteralIfExistFalse();
    std::optional<int> getFirstUnmarkedLiteralIfExistTrue();
    std::optional<int> getFirstUnmarkedLiteralIfExistFalseHighPerfomance();
    std::optional<int> getFirstUnmarkedLiteralIfExistTrueHighPerfomance();
    void markLiteral(int term,bool _mark);
    void remarkLiteral(int term, bool _mark);
    
    bool isCorrect();
    bool isCorrectFast();
    bool isCorrect(int term,int posibleState);
    bool isCorrectFast(int term,int posibleState);
    bool isCorrectOnVector();
    
    void findPureLiterals();
    void findPureLiteralsHighPerfomance();
    void findPureLiteralsHighPerfomanceMap();
    void unitPropagation();
    void unitPropagationHighPerf();
    void unitPropagationHighPerfArray();
    void unitPropagationHighPerfArrayMap();
    void unitPropagationHighPerfArrayMapRetryHighPerf();
    
    void nextStep();
    void prevStep();
    void rollbackSteps(int amountOfSteps);
    
    int* getResult();
    int getAmountOfLiterals();
private:
    int amountOfLiterals;
    std::vector<ClauseNode*> logicFunction;
    int *literalMarks;
    int *literalMarksStepsChanged;
    int step = 0;
    
    std::vector<ClauseNode*>** positiveClauses;
    std::vector<ClauseNode*>** negativeClauses;
    
    std::unordered_map<int, int> probPureLiterals{};
    char *probPureLiteralsArray;
    std::set<int> unitLiterals{};
    int *unitLiteralsArray;
    int *unitLiteralsArrayCache;
    
    std::optional<int> getFirstUnmarkedLiteralIfExistInternal(int mark);
    std::optional<int> getFirstUnmarkedLiteralIfExistInternalHighPerfomance(int mark);
    
    void updateState(int term);
    
    int amountOfUnmarkedLiteralInClauseAndGetIfSingle(int cl);
    int amountOfUnmarkedLiteralInClauseAndGetIfSingleHighPerfomance(ClauseNode* clause);
};

#endif /* logic_function_hpp */
