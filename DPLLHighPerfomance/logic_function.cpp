//
//  logic_function.cpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#include "logic_function.hpp"

LogicFunction::LogicFunction(std::vector<ClauseNode*>& lf, int _amountOfLiterals) {
    this->literalMarks = new int[_amountOfLiterals+1]();
    this->literalMarksStepsChanged = new int[_amountOfLiterals+1]();
    this->probPureLiteralsArray = new char[_amountOfLiterals+1];
    this->unitLiteralsArray = new int[_amountOfLiterals+1];
    this->unitLiteralsArrayCache = new int[_amountOfLiterals+1];
    this->amountOfLiterals = _amountOfLiterals;
    this->positiveClauses = new std::vector<ClauseNode*>*[_amountOfLiterals+1];
    this->negativeClauses = new std::vector<ClauseNode*>*[_amountOfLiterals+1];
    
    for(int i = 1; i <= _amountOfLiterals; i++) {
        this->positiveClauses[i] = new std::vector<ClauseNode*>();
        this->negativeClauses[i] = new std::vector<ClauseNode*>();
    }
    
    for(auto clause = lf.begin(); clause != lf.end(); ++clause){
        this->logicFunction.push_back(*clause);
        
        for(auto literal = (*clause)->begin(); literal != (*clause)->end(); ++literal) {
            int term = std::abs(*literal);
//            if (*literal == -24) {
//                std::cout<<"some interesting literal"<<std::endl;
//            }
            if (*literal > 0) {
                this->positiveClauses[term]->push_back(*clause);
            } else {
                this->negativeClauses[term]->push_back(*clause);
            }
        }
    }
}


LogicFunction::~LogicFunction() {
    delete literalMarks;
    delete literalMarksStepsChanged;
    delete unitLiteralsArray;
    delete unitLiteralsArrayCache;
    delete probPureLiteralsArray;
    
    for(int i = 1; i <= amountOfLiterals; i++) {
        positiveClauses[i]->clear();
        delete positiveClauses[i];
        negativeClauses[i]->clear();
        delete negativeClauses[i];
    }
    
    delete positiveClauses;
    delete negativeClauses;
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        delete *clause;
    }
    logicFunction.clear();
}

void LogicFunction::printMarkedLiterals() {
    std::cout<<"Marked literals: ";
    for(int i = 1; i <= amountOfLiterals; i++) {
        if(literalMarks[i] != UNMARKED) {
            std::cout<<" [ "<<i<<" : "<<(literalMarks[i] == TRUEL ? "TRUE" : "FALSE")<<" ]";
        }
    }
    std::cout<<std::endl<<std::endl;
}

int* LogicFunction::getResult() {
    return literalMarks;
}

int LogicFunction::getAmountOfLiterals() {
    return amountOfLiterals;
}

bool LogicFunction::isCorrect() {
    
    int counter = 0;
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        
        if((*clause)->isTrue()) continue;
        
        counter = 0;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            
            int term = std::abs(*literal);
            
            if ((literalMarks[term] == TRUEL && *literal < 0) || (literalMarks[term] == FALSEL && *literal > 0)) {
                counter++;
            }
            
        }
        
        if (counter == (*clause)->amountOfLiterals()) {
            /* START LOG */
//            std::cout<<"incorrect on clause: "<<clause-logicFunction.begin()<< "  ";
//
//            for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
//
//                int term = std::abs(*literal);
//
//                std::cout<< " [ "<<*literal
//                << " : "
//                <<(literalMarks[term] == TRUEL ? "TRUE" : (literalMarks[term] == FALSEL ? "FALSE" : "UNMARKED"))
//                << " ]";
//            }
//            std::cout<< std::endl;
//
            /* END LOG */
            
            return false;
        }
    }
    
    return true;
}


bool LogicFunction::isCorrectFast() {
    auto clause = logicFunction.begin();
    
    while(clause != logicFunction.end()) {
        if((*clause)->isTrue()) goto END_INTERNAL_LOOP;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            
            if(literalMarks[std::abs(*literal)] == UNMARKED) goto END_INTERNAL_LOOP;
        }
        
        return false;
    
    END_INTERNAL_LOOP:
                ++clause;
    }
    
    return true;
}

bool LogicFunction::isCorrectOnVector() {
    
    int counter = 0;
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        
        counter = 0;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            
            int term = std::abs(*literal);
            
            if ((literalMarks[term] == TRUEL && *literal < 0) || (literalMarks[term] == FALSEL && *literal > 0)) {
                counter++;
            }
            
        }
        
        if (counter == (*clause)->amountOfLiterals()) {
            /* START LOG */
//            std::cout<<"incorrect on clause: "<<clause-logicFunction.begin()<< "  ";
//
//            for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
//
//                int term = std::abs(*literal);
//
//                std::cout<< " [ "<<*literal
//                << " : "
//                <<(literalMarks[term] == TRUEL ? "TRUE" : (literalMarks[term] == FALSEL ? "FALSE" : "UNMARKED"))
//                << " ]";
//            }
//            std::cout<< std::endl;
//
            /* END LOG */
            
            return false;
        }
    }
    
    return true;
}

bool LogicFunction::isCorrect(int term, int posibleState) {
    
    int startState =literalMarks[term] ;
    literalMarks[term] = posibleState;
    
    int counter = 0;
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        
        if((*clause)->isTrue()) continue;
        
        counter = 0;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            
            int term = std::abs(*literal);
            
            if ((literalMarks[term] == TRUEL && *literal < 0) || (literalMarks[term] == FALSEL && *literal > 0)) {
                counter++;
            }
            
        }
        
        if (counter == (*clause)->amountOfLiterals()) {
            /* START LOG */
//            std::cout<<"incorrect on clause: "<<clause-logicFunction.begin()<< "  ";
//
//            for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
//
//                int term = std::abs(*literal);
//
//                std::cout<< " [ "<<*literal
//                << " : "
//                <<(literalMarks[term] == TRUEL ? "TRUE" : (literalMarks[term] == FALSEL ? "FALSE" : "UNMARKED"))
//                << " ]";
//            }
//            std::cout<< std::endl;
//
            /* END LOG */
            
            literalMarks[term] = startState;
            
            return false;
        }
    }
    
    
    literalMarks[term] = startState;
    
    return true;
}

bool LogicFunction::isCorrectFast(int term, int posibleState) {
    int startState = literalMarks[term] ;
    literalMarks[term] = posibleState;
    
    auto clause = logicFunction.begin();
    
    while(clause != logicFunction.end()) {
        if((*clause)->isTrue()) goto END_INTERNAL_LOOP;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            
            if(literalMarks[std::abs(*literal)] == UNMARKED) goto END_INTERNAL_LOOP;
        }
        
        literalMarks[term] = startState;
        return false;
    
    END_INTERNAL_LOOP:
                ++clause;
    }
    
    literalMarks[term] = startState;
    return true;
}


void LogicFunction::updateState(int term) {
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        
        if((*clause)->isTrue()) continue;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            int termInternal = std::abs(*literal);
            if(term == termInternal) {
                if ((literalMarks[termInternal] == TRUEL && *literal > 0) ||
                    (literalMarks[termInternal] == FALSEL && *literal < 0)) {
                    (*clause)->setMark(true,step);
                    break;
                }
            }
        }
    }
}

std::optional<int> LogicFunction::getFirstUnmarkedLiteralIfExistTrue() {
    return getFirstUnmarkedLiteralIfExistInternal(TRUEL);
}

std::optional<int> LogicFunction::getFirstUnmarkedLiteralIfExistFalse() {
    return getFirstUnmarkedLiteralIfExistInternal(FALSEL);
}

std::optional<int> LogicFunction::getFirstUnmarkedLiteralIfExistInternal(int mark) {
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        
        if ((*clause)->isTrue()) continue;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            int term = std::abs(*literal);
            if(literalMarks[term] == UNMARKED) {
                literalMarks[term] = mark;
                literalMarksStepsChanged[term] = step;
                updateState(term);
                return  term;
            }
        }
    }
    
    return std::nullopt;
}

std::optional<int> LogicFunction::getFirstUnmarkedLiteralIfExistFalseHighPerfomance() {
    return getFirstUnmarkedLiteralIfExistInternalHighPerfomance(FALSEL);
}

std::optional<int> LogicFunction::getFirstUnmarkedLiteralIfExistTrueHighPerfomance() {
    return getFirstUnmarkedLiteralIfExistInternalHighPerfomance(TRUEL);
}

std::optional<int> LogicFunction::getFirstUnmarkedLiteralIfExistInternalHighPerfomance(int mark) {
    for(int i = 1; i <= amountOfLiterals; i++) {
        if (literalMarks[i] == UNMARKED) {
            literalMarks[i] = mark;
            literalMarksStepsChanged[i] = step;
            updateState(i);
            return i;
        }
    }
    
    return std::nullopt;
}

void LogicFunction::markLiteral(int term, bool _mark) {
    literalMarks[term] = _mark ? TRUEL : FALSEL;
    literalMarksStepsChanged[term] = step;
    updateState(term);
}

void LogicFunction::remarkLiteral(int term, bool _mark) {
    literalMarks[term] = _mark ? TRUEL : FALSEL;
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        
        if((*clause)->isTrueOnStepOrMarkFalse(step)) continue;
        
        for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
            int termInternal = std::abs(*literal);
            if(term == termInternal) {
                if ((literalMarks[termInternal] == TRUEL && *literal > 0) ||
                    (literalMarks[termInternal] == FALSEL && *literal < 0)) {
                    (*clause)->setMark(true,step);
                    break;
                }
            }
        }
    }
}

void LogicFunction::nextStep() {
    ++step;
}

void LogicFunction::prevStep() {
    rollbackSteps(1);
}

void LogicFunction::rollbackSteps(int amountOfSteps) {
    int stepsLowerBound = step - amountOfSteps;
    
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
        if((*clause)->getStep() > stepsLowerBound) {
            (*clause)->setMark(false,-1);
        }
    }
    
    for(int i = 0; i <= amountOfLiterals; i++) {
        if(literalMarksStepsChanged[i] > stepsLowerBound){
            literalMarks[i] = UNMARKED;
            literalMarksStepsChanged[i] = NOT_MODIFIED;
        }
    }
    
    step -= amountOfSteps;
}

void LogicFunction::findPureLiterals() {

    probPureLiterals.clear();
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause){
        
        if ((*clause)->isTrue()) continue;
        
        for(auto literal = (*clause)->begin(); literal != (*clause)->end(); ++literal){
            
            int term = std::abs(*literal);
            //todo:: перейти на массив вместо мапы???
            if(literalMarks[term] == UNMARKED) {
                
                auto mapLiteral = probPureLiterals.find(term);
                
                if (mapLiteral == probPureLiterals.end()) {
                    probPureLiterals[term] = *literal < 0 ? NEGATIVE : POSITIVE;
                } else {
                    
                    switch (mapLiteral->second) {
                        case DELETED:
                            break;
                        case POSITIVE:
                            if (*literal < 0) {
                                probPureLiterals[term] = DELETED_OLD;
                            }
                            break;
                        case NEGATIVE:
                            if (*literal > 0) {
                                probPureLiterals[term] = DELETED_OLD;
                            }
                            break;
                    }
                }
            }
        }
    }
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause){
        
        if ((*clause)->isTrue()) continue;
        
        for(auto literal = (*clause)->begin(); literal != (*clause)->end(); ++literal){
            int term = std::abs(*literal);
            if(probPureLiterals[term] != DELETED_OLD) {
                if (*literal > 0) {
                    literalMarks[term] = TRUEL;
                } else {
                    literalMarks[term] = FALSEL;
                }
                literalMarksStepsChanged[term] = step;
                (*clause)->setMark(true,step);
            }
        }
        
    }
}

void LogicFunction::findPureLiteralsHighPerfomance() {
    
    memset(probPureLiteralsArray,CNOT_CHEKED,(amountOfLiterals+1)*sizeof(char));
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause){
        
        if ((*clause)->isTrue()) continue;
        
        for(auto literal = (*clause)->begin(); literal != (*clause)->end(); ++literal){
            
            int term = std::abs(*literal);
            
            if(literalMarks[term] == UNMARKED) {
                
                switch (probPureLiteralsArray[term]) {
                    case CDELETED:
                        break;
                    case CNOT_CHEKED:
                        probPureLiteralsArray[term] = *literal > 0 ? CPOSITIVE : CNEGATIVE;
                        break;
                    case CPOSITIVE:
                        probPureLiteralsArray[term] = *literal > 0 ? CPOSITIVE : CDELETED;
                        break;
                    case CNEGATIVE:
                        probPureLiteralsArray[term] = *literal < 0 ? CNEGATIVE : CDELETED;
                        break;
                }
            }
        }
    }
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause){
        
        if ((*clause)->isTrue()) continue;
        
        for(auto literal = (*clause)->begin(); literal != (*clause)->end(); ++literal){
            int term = std::abs(*literal);
            if(probPureLiteralsArray[term] > CNOT_CHEKED) {
                
                literalMarks[term] = *literal > 0 ? TRUEL : FALSEL;
                literalMarksStepsChanged[term] = step;
                (*clause)->setMark(true,step);
            }
        }
        
    }
}

void LogicFunction::findPureLiteralsHighPerfomanceMap() {
    memset(probPureLiteralsArray,CNOT_CHEKED,(amountOfLiterals+1)*sizeof(char));
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause){
        
        if ((*clause)->isTrue()) continue;
        
        for(auto literal = (*clause)->begin(); literal != (*clause)->end(); ++literal){
            
            int term = std::abs(*literal);
            
            if(literalMarks[term] == UNMARKED) {
                
                switch (probPureLiteralsArray[term]) {
                    case CDELETED:
                        break;
                    case CNOT_CHEKED:
                        probPureLiteralsArray[term] = *literal > 0 ? CPOSITIVE : CNEGATIVE;
                        break;
                    case CPOSITIVE:
                        probPureLiteralsArray[term] = *literal > 0 ? CPOSITIVE : CDELETED;
                        break;
                    case CNEGATIVE:
                        probPureLiteralsArray[term] = *literal < 0 ? CNEGATIVE : CDELETED;
                        break;
                }
            }
        }
    }
    
    for(int i = 1; i <= amountOfLiterals; i++) {
        if (probPureLiteralsArray[i] == CPOSITIVE) {
            for(auto clause = positiveClauses[i]->begin(); clause != positiveClauses[i]->end(); ++ clause) {
                if ((*clause)->isTrue()) continue;
                else (*clause)->setMark(true, step);
            }
            literalMarks[i] = TRUEL;
            literalMarksStepsChanged[i] = step;
        } else if(probPureLiteralsArray[i] == CNEGATIVE) {
            for(auto clause = negativeClauses[i]->begin(); clause != negativeClauses[i]->end(); ++ clause) {
                if ((*clause)->isTrue()) continue;
                else (*clause)->setMark(true, step);
            }
            literalMarks[i] = FALSEL;
            literalMarksStepsChanged[i] = step;
        }
    }
}

int LogicFunction::amountOfUnmarkedLiteralInClauseAndGetIfSingle(int cl) {
    auto clause = this->logicFunction[cl];
    
    int counter = 0;
    int term = 0;
    int first = 0;
    
    for(auto literl = clause->begin(); literl != clause->end(); ++literl) {
        term = std::abs(*literl);
        if(literalMarks[term] == UNMARKED) {
            first = *literl;
            ++counter;
        }
    }
    return counter == 1 ? first : 0;
}

//todo::перенести на счетчик элементов в клаузе
int LogicFunction::amountOfUnmarkedLiteralInClauseAndGetIfSingleHighPerfomance(ClauseNode* clause) {
    //todo fix something
    int counter = 0;
    int term = 0;
    int first = 0;
    int mark = 0;
    
    for(auto literal = clause->begin(); literal != clause->end(); ++literal) {
        term = std::abs(*literal);
        mark = literalMarks[term];
        if( mark == UNMARKED) {
            first = *literal;
            if((++counter) > 1) return 0;
        } else if ((mark == TRUEL && *literal > 0) || (mark == FALSEL && *literal < 0)) {
                return 0;
        }
    }
    return first;
}

void LogicFunction::unitPropagation() {
    int clauseIndex = 0;
    
    int posibleLiteral = 0;
    int term = 0;
    
    while(clauseIndex < this->logicFunction.size()) {
        
        auto clause = this->logicFunction[clauseIndex];
        
        if(clause->isTrue()) {
            clauseIndex++;
            continue;
        }
        
        posibleLiteral = amountOfUnmarkedLiteralInClauseAndGetIfSingle(clauseIndex);
        
        if(posibleLiteral != 0) {
            
            term = std::abs(posibleLiteral);
            
            literalMarks[term] = posibleLiteral > 0 ? TRUEL : FALSEL;
            literalMarksStepsChanged[term] = step;
            clause->setMark(true,step);
            
            updateState(term);
            
            clauseIndex = 0;
            
        } else {
            clauseIndex ++;
        }
        
    }
}

void LogicFunction::unitPropagationHighPerf() {
    bool isAnyContains = true;
    
    int posibleLiteral = 0;
    int term = 0;
    
    while(isAnyContains) {
        
        isAnyContains = false;
        unitLiterals.clear();
        
        for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause) {
            
            
            if((*clause)->isTrue()) continue;
            
            
            posibleLiteral = amountOfUnmarkedLiteralInClauseAndGetIfSingleHighPerfomance(*clause);
            
            if(posibleLiteral != 0) {
                
                term = std::abs(posibleLiteral);
                
                literalMarks[term] = posibleLiteral > 0 ? TRUEL : FALSEL;
                literalMarksStepsChanged[term] = step;
                (*clause)->setMark(true,step);
                
                unitLiterals.insert(term);
                
                isAnyContains = true;
            }
            
        }
        
        if (unitLiterals.empty()) return;
        
        for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
            
            if((*clause)->isTrue()) continue;
            
            for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
                
                term = std::abs(*literal);
                //todo:: перейти на массив???
                if(unitLiterals.find(term) != unitLiterals.end()) {
                    if ((literalMarks[term] == TRUEL && *literal > 0) ||
                        (literalMarks[term] == FALSEL && *literal < 0)) {
                        (*clause)->setMark(true,step);
                        break;
                    }
                }
            }
        }
        
    }
}

void LogicFunction::unitPropagationHighPerfArray() {
    bool isAnyContains = true;
    
    int posibleLiteral = 0;
    int term = 0;
    
    while(isAnyContains) {
        
        isAnyContains = false;
        memset(unitLiteralsArray, 0, (amountOfLiterals+1)*sizeof(int));
        
        for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause) {
            
            if((*clause)->isTrue()) continue;
                 
            posibleLiteral = amountOfUnmarkedLiteralInClauseAndGetIfSingleHighPerfomance(*clause);
            
            //todo::branching remove 
            if(posibleLiteral != 0) {
                
                term = std::abs(posibleLiteral);
                
                literalMarks[term] = posibleLiteral > 0 ? TRUEL : FALSEL;
                literalMarksStepsChanged[term] = step;
                (*clause)->setMark(true,step);
                
                unitLiteralsArray[term] = 1;
                
                isAnyContains = true;
            }
            
        }
        
        if (!isAnyContains) return;
        
        for(auto clause = logicFunction.begin(); clause != logicFunction.end(); ++clause){
            
            if((*clause)->isTrue()) continue;
            
            for(auto literal = (*clause)->begin();literal != (*clause)->end(); ++literal) {
                
                term = std::abs(*literal);
                
                
                if ((unitLiteralsArray[term] != 0) &&
                    ((literalMarks[term] == TRUEL && *literal > 0) ||
                     (literalMarks[term] == FALSEL && *literal < 0))) {
                    
                    (*clause)->setMark(true,step);
                    break;
                }
            }
        }
    }
}

void LogicFunction::unitPropagationHighPerfArrayMap() {
    bool isAnyContains = true;
    
    int posibleLiteral = 0;
    int term = 0;
    
    while(isAnyContains) {
        
        isAnyContains = false;
        memset(unitLiteralsArray, 0, (amountOfLiterals+1)*sizeof(int));
        
        for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause) {
            
            if((*clause)->isTrue()) continue;
                 
            posibleLiteral = amountOfUnmarkedLiteralInClauseAndGetIfSingleHighPerfomance(*clause);
            
            //todo::branching remove
            if(posibleLiteral != 0) {
                
                term = std::abs(posibleLiteral);
                
                literalMarks[term] = posibleLiteral > 0 ? TRUEL : FALSEL;
                literalMarksStepsChanged[term] = step;
                (*clause)->setMark(true,step);
                
                unitLiteralsArray[term] = 1;
                
                isAnyContains = true;
            }
            
        }
        
        if (!isAnyContains) return;
        
        //поидее можно избаавиться от while который большой и сделать 2 цикла первый общий - второй только на затронутые клаузы проверить обратный знак
        
        for(int i = 1; i <= amountOfLiterals; i++) {
            if (unitLiteralsArray[i] != 0) {
                
                if (literalMarks[i] == TRUEL) {
                    for(auto clause = positiveClauses[i]->begin(); clause != positiveClauses[i]->end(); ++ clause) {
                        if ((*clause)->isTrue()) continue;
                        else (*clause)->setMark(true, step);
                    }
                } else if(literalMarks[i] == FALSEL) {
                    for(auto clause = negativeClauses[i]->begin(); clause != negativeClauses[i]->end(); ++ clause) {
                        if ((*clause)->isTrue()) continue;
                        else (*clause)->setMark(true, step);
                    }
                }
            }
        }
    }
}

void LogicFunction::unitPropagationHighPerfArrayMapRetryHighPerf() {
    bool isAnyContains = true;
    
    int posibleLiteral = 0;
    int term = 0;
    bool isMemsetNeeded = true;
    
    isAnyContains = false;
    memset(unitLiteralsArray, 0, (amountOfLiterals+1)*sizeof(int));
    
    for(auto clause = logicFunction.begin(); clause != logicFunction.end() ; ++clause) {
        
        if((*clause)->isTrue()) continue;
        
        posibleLiteral = amountOfUnmarkedLiteralInClauseAndGetIfSingleHighPerfomance(*clause);
        
        //todo::branching remove
        if(posibleLiteral != 0) {
            
            term = std::abs(posibleLiteral);
            
            literalMarks[term] = posibleLiteral > 0 ? TRUEL : FALSEL;
            literalMarksStepsChanged[term] = step;
            (*clause)->setMark(true,step);
            
            unitLiteralsArray[term] = 1;
            
            isAnyContains = true;
        }
        
    }
    
    if (!isAnyContains) return;
    
    //поидее можно избаавиться от while который большой и сделать 2 цикла первый общий - второй только на затронутые клаузы проверить обратный знак
    
    UPDATE_STATE:
    isAnyContains = false;
    isMemsetNeeded = true;
    for(int i = 1; i <= amountOfLiterals; i++) {
        if (unitLiteralsArray[i] != 0) {
            
            if (literalMarks[i] == TRUEL) {
                for(auto clause = positiveClauses[i]->begin(); clause != positiveClauses[i]->end(); ++ clause) {
                    if ((*clause)->isTrue()) continue;
                    else (*clause)->setMark(true, step);
                }
            } else if(literalMarks[i] == FALSEL) {
                for(auto clause = negativeClauses[i]->begin(); clause != negativeClauses[i]->end(); ++ clause) {
                    if ((*clause)->isTrue()) continue;
                    else (*clause)->setMark(true, step);
                }
            }
        }
    }
    
    memcpy(unitLiteralsArrayCache,unitLiteralsArray,amountOfLiterals*sizeof(int));
    
    for(int i = 1; i <= amountOfLiterals; i++) {
        if (unitLiteralsArrayCache[i] != 0) {
            
            auto changedClauses = literalMarks[i] == TRUEL ? negativeClauses[i] : positiveClauses[i];
            
            for(auto clause = changedClauses->begin(); clause != changedClauses->end(); ++ clause) {
                if ((*clause)->isTrue()) continue;
                
                posibleLiteral = amountOfUnmarkedLiteralInClauseAndGetIfSingleHighPerfomance(*clause);
                
                //todo::branching remove
                if(posibleLiteral != 0) {
                    if (isMemsetNeeded) {
                        //fuckedup
                        isMemsetNeeded = false;
                        memset(unitLiteralsArray, 0, (amountOfLiterals+1)*sizeof(int));
                    }
                    term = std::abs(posibleLiteral);
                    
                    literalMarks[term] = posibleLiteral > 0 ? TRUEL : FALSEL;
                    literalMarksStepsChanged[term] = step;
                    (*clause)->setMark(true,step);
                    
                    unitLiteralsArray[term] = 1;
                    
                    isAnyContains = true;
                }
                
            }
            
        }
    }
    if (isAnyContains) goto UPDATE_STATE;
    //todo::merge to 1 cycle
    
}
