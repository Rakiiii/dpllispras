//
//  sat_solver.cpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#include "sat_solver.hpp"

const bool firstBranch = true;
const bool secondBranch = false;

std::optional<int*> SATSolver::solveTest(LogicFunction &logicFunction) {
    bool isCorrect = false;
    int amountOfRollbackSteps = 1;
    
    
    std::stack<Pair> literalStack;
    
    Pair lastLiteral(-1,firstBranch);
    
    while (true) {

        logicFunction.nextStep();
        
        logicFunction.findPureLiteralsHighPerfomance();
        
        logicFunction.unitPropagationHighPerfArray();
        
        if(!logicFunction.isCorrect()) {
            
            isCorrect = false;
            amountOfRollbackSteps = 1;
            
            while (!isCorrect) {
                
                while (literalStack.top().branch != firstBranch) {
                    
                    literalStack.pop();
                    
                    amountOfRollbackSteps +=2;
                    
                    if (literalStack.empty()) {
                        return std::nullopt;
                    }
                }
                logicFunction.rollbackSteps(amountOfRollbackSteps+1);
                
                isCorrect = logicFunction.isCorrect(literalStack.top().literal, FALSEL);
                
                amountOfRollbackSteps = 2;
            }
            //logicFunction.prevStep();
            
            logicFunction.nextStep();
            
            lastLiteral.literal = literalStack.top().literal;
            lastLiteral.branch = secondBranch;
            
            logicFunction.markLiteral(lastLiteral.literal, secondBranch);
            
            literalStack.pop();
            literalStack.push(Pair(lastLiteral));
            
            continue;
        }
        
        logicFunction.nextStep();
        
        auto literal = logicFunction.getFirstUnmarkedLiteralIfExistTrue();
        
        if (!logicFunction.isCorrect()) {
            
            logicFunction.prevStep();
            
            logicFunction.nextStep();
            
            logicFunction.markLiteral(*literal, secondBranch);
            
            if (logicFunction.isCorrect()) continue;
            
            isCorrect = false;
            //todo:: iscorrect?
            amountOfRollbackSteps = 2;
            
            while (!isCorrect) {
                
                while (literalStack.top().branch != firstBranch) {
                    
                    literalStack.pop();
                    
                    amountOfRollbackSteps += 2;
                    
                    if (literalStack.empty()) {
                        return std::nullopt;
                    }
                }
                
                logicFunction.rollbackSteps(amountOfRollbackSteps+1);
                
                isCorrect = logicFunction.isCorrect(literalStack.top().literal, FALSEL);
                
                amountOfRollbackSteps = 2;
            }
            
            logicFunction.prevStep();
            logicFunction.nextStep();
            lastLiteral.literal = literalStack.top().literal;
            lastLiteral.branch = secondBranch;
            
            logicFunction.markLiteral(lastLiteral.literal, secondBranch);
            
            literalStack.pop();
            literalStack.push(Pair(lastLiteral));
            
            continue;
            
        } else {
            if (!literal) return logicFunction.getResult();
            
            lastLiteral.literal = *literal;
            lastLiteral.branch = firstBranch;
            literalStack.push(Pair(lastLiteral));
        }
    }

}
 
std::optional<int*> SATSolver::solve(LogicFunction &logicFunction) {
    
    std::stack<Pair> literalStack;
    
    Pair lastLiteral(-1,firstBranch);
    
    while (true) {
        
//        std::cout<<"/---------------Next Step------------------/"<<std::endl;
        
        if (lastLiteral.literal != -1) {
            literalStack.push(Pair(lastLiteral));
        }
        
        logicFunction.nextStep();
        
//        std::cout<<"/---------------Find pure------------------/"<<std::endl;
        logicFunction.findPureLiterals();
//        logicFunction.printMarkedLiterals();
//        std::cout<<"/---------------Find unit proc------------------/"<<std::endl;
        logicFunction.unitPropagation();
//        logicFunction.unitPropagationHighPerf();
//        logicFunction.printMarkedLiterals();
        
        if(!logicFunction.isCorrect()) {
//            std::cout<<"/---------------Find unit proc incorrect------------------/"<<std::endl;
            logicFunction.prevStep();
            
//            std::cout<<"/---------------Rollback start------------------/"<<std::endl;
            while (literalStack.top().branch != firstBranch || !logicFunction.isCorrect(literalStack.top().literal,FALSEL)) {
                
//                std::cout<<"/---------------Rollbacked: "<<literalStack.top().literal<<" ------------------/"<<std::endl;
                literalStack.pop();
                
                logicFunction.prevStep();
                logicFunction.prevStep();
                
                if (literalStack.empty()) {
                    return std::nullopt;
                }
            }
            
//            std::cout<<"/---------------Rollback end------------------/"<<std::endl;
            logicFunction.prevStep();
            
            logicFunction.nextStep();
            lastLiteral.literal = literalStack.top().literal;
            lastLiteral.branch = secondBranch;
            
//            std::cout<<"/---------------Next literal "<<lastLiteral.literal<<" barnch false "<<"------------------/"<<std::endl;
            
            //todo add correct check
            logicFunction.markLiteral(lastLiteral.literal, secondBranch);
//            logicFunction.printMarkedLiterals();
            
            literalStack.pop();
            
            continue;
        }
        
        logicFunction.nextStep();
        
//        std::cout<<"/---------------Find next literal------------------/"<<std::endl;
        auto literal = logicFunction.getFirstUnmarkedLiteralIfExistTrue();
        
        if (!logicFunction.isCorrect()) {
            
//            std::cout<<"/---------------Incorrect on next "<< *literal <<" ------------------/"<<std::endl;
            logicFunction.prevStep();
            logicFunction.nextStep();
            
            logicFunction.markLiteral(*literal, secondBranch);
            
            if (logicFunction.isCorrect()) continue;
            
            logicFunction.prevStep();
            
//            std::cout<<"/---------------Rollback start------------------/"<<std::endl;
            while (literalStack.top().branch != firstBranch || !logicFunction.isCorrect(literalStack.top().literal,FALSEL)) {
                
                literalStack.pop();
                
                logicFunction.prevStep();
                logicFunction.prevStep();
                
                if (literalStack.empty()) {
                    return std::nullopt;
                }
            }
            
//            std::cout<<"/---------------Rollback end------------------/"<<std::endl;
            
            logicFunction.prevStep();
            logicFunction.nextStep();
            lastLiteral.literal = literalStack.top().literal;
            lastLiteral.branch = secondBranch;
            
//            std::cout<<"/---------------Next literal "<<lastLiteral.literal<<" barnch false "<<"------------------/"<<std::endl;
            
            logicFunction.markLiteral(lastLiteral.literal, secondBranch);
            
            literalStack.pop();
            
            continue;
            
        } else {
            if (!literal) return logicFunction.getResult();
            
            lastLiteral.literal = *literal;
            lastLiteral.branch = firstBranch;
            
//            std::cout<<"/---------------Next literal "<<lastLiteral.literal<<" barnch true "<<"------------------/"<<std::endl;
        }
    }
}

//todo::move from stack to pair of arrays
std::optional<int*> SATSolver::solveHighPerfomance(LogicFunction &logicFunction) {
    
    std::stack<Pair> literalStack;
    
    Pair lastLiteral(-1,firstBranch);
    
    while (true) {
        
        if (lastLiteral.literal != -1) {
            literalStack.push(Pair(lastLiteral));
        }

        logicFunction.nextStep();
        
        logicFunction.findPureLiteralsHighPerfomance();
        
        logicFunction.unitPropagationHighPerfArray();
        
        if(!logicFunction.isCorrect()) {
            
            bool isCorrect = false;
            int amountOfRollbackSteps = 1;
            
            while (!isCorrect) {
                
                while (literalStack.top().branch != firstBranch) {
                    
                    literalStack.pop();
                    
                    amountOfRollbackSteps +=2;
                    
                    if (literalStack.empty()) {
                        return std::nullopt;
                    }
                }
                logicFunction.rollbackSteps(amountOfRollbackSteps+1);
                
                isCorrect = logicFunction.isCorrect(literalStack.top().literal, FALSEL);
                
                amountOfRollbackSteps = 2;
            }
            //logicFunction.prevStep();
            
            logicFunction.nextStep();
            
            lastLiteral.literal = literalStack.top().literal;
            lastLiteral.branch = secondBranch;
            
            logicFunction.markLiteral(lastLiteral.literal, secondBranch);
            
            literalStack.pop();
            
            continue;
        }
        
        logicFunction.nextStep();
        
        auto literal = logicFunction.getFirstUnmarkedLiteralIfExistTrue();
        
        if (!logicFunction.isCorrect()) {
            
            logicFunction.prevStep();
            
            logicFunction.nextStep();
            
            logicFunction.markLiteral(*literal, secondBranch);
            
            if (logicFunction.isCorrect()) continue;
            
            bool isCorrect = false;
            //todo:: iscorrect?
            int amountOfRollbackSteps = 2;
            
            while (!isCorrect) {
                
                while (literalStack.top().branch != firstBranch) {
                    
                    literalStack.pop();
                    
                    amountOfRollbackSteps += 2;
                    
                    if (literalStack.empty()) {
                        return std::nullopt;
                    }
                }
                
                logicFunction.rollbackSteps(amountOfRollbackSteps+1);
                
                isCorrect = logicFunction.isCorrect(literalStack.top().literal, FALSEL);
                
                amountOfRollbackSteps = 2;
            }
            
            logicFunction.prevStep();
            logicFunction.nextStep();
            lastLiteral.literal = literalStack.top().literal;
            lastLiteral.branch = secondBranch;
            
            logicFunction.markLiteral(lastLiteral.literal, secondBranch);
            
            literalStack.pop();
            
            continue;
            
        } else {
            if (!literal) return logicFunction.getResult();
            
            lastLiteral.literal = *literal;
            lastLiteral.branch = firstBranch;
//            literalStack.push(Pair(lastLiteral));
        }
    }
}

//переход на специализацию когда сначала переберается ветка true, смена first и second branch местами логику не изменит
//для изменения порядка перебора стоит поменять логику внутри while отката
//todo::подумать и отдебажить способ избавиться от стека и до пары в виже lastLiteral
std::optional<int*> SATSolver::solveHighPerfomanceNoStack(LogicFunction &logicFunction) {
    int *literalStack = new int[logicFunction.getAmountOfLiterals()];
    bool *literalBranchStack = new bool[logicFunction.getAmountOfLiterals()];
    
    int stackCup = -1;
    
    while (true) {
        
        logicFunction.nextStep();
        
//        logicFunction.findPureLiteralsHighPerfomance();
        logicFunction.findPureLiteralsHighPerfomanceMap();
        
//        logicFunction.unitPropagationHighPerfArray();
//        logicFunction.unitPropagationHighPerfArrayMap();
        logicFunction.unitPropagationHighPerfArrayMapRetryHighPerf();
        
        if(!logicFunction.isCorrectFast()) {
            
            if(stackCup == -1) {
                return std::nullopt;
            }
            
            bool isCorrect = false;
            int amountOfRollbackSteps = 1;
            
            while (!isCorrect) {
                
                while (!literalBranchStack[stackCup]) {
                    
                    --stackCup;
                    
                    amountOfRollbackSteps +=2;
                    
                    if (stackCup == -1) {
                        return std::nullopt;
                    }
                }
                
                logicFunction.rollbackSteps(amountOfRollbackSteps+1);
                
                isCorrect = logicFunction.isCorrectFast(literalStack[stackCup], FALSEL);
                
                amountOfRollbackSteps = 1;
            }
            
            logicFunction.nextStep();
            
            logicFunction.markLiteral(literalStack[stackCup], secondBranch);
            
            literalBranchStack[stackCup] = secondBranch;
            
            
            continue;
        }
        
        logicFunction.nextStep();
        
        auto literal = logicFunction.getFirstUnmarkedLiteralIfExistTrue();
        
        if (!logicFunction.isCorrectFast()) {
            //todo::надо ли это делать так, может стоит просто временно помарчить литералы???
            /* ??? START ??? */
//            logicFunction.prevStep();
//
//            logicFunction.nextStep();
//
//            logicFunction.markLiteral(*literal, secondBranch);
            logicFunction.remarkLiteral(*literal, secondBranch);
            
            if (logicFunction.isCorrectFast()) continue;
            /* ??? END ??? */
            
            if(stackCup == -1) {
                return std::nullopt;
            }
            
            bool isCorrect = false;
            
            int amountOfRollbackSteps = 2;
            
            while (!isCorrect) {
                
                while (!literalBranchStack[stackCup]) {
                    
                    amountOfRollbackSteps +=2;
                    
                    if ((--stackCup) == -1) {
                        return std::nullopt;
                    }
                }
                
                //todo::написать метод который проверяет корректность и ролбэчит в 1 методе
                //todo::может не ролбэчить а опять же временно марчить
                logicFunction.rollbackSteps(amountOfRollbackSteps);
                
                isCorrect = logicFunction.isCorrectFast(literalStack[stackCup], FALSEL);
                
                amountOfRollbackSteps = 1;
            }
            
            //todo::как-то оптимизировать этот финт ушами а то хреново выглядит
            // Возможно стоит делать откат + ремарка а срузк помарчить новым вариантом
            // в таком варианте надо пробегать 1 раз по всем клайзам и летралам
            // если клауза помарчена на нынешнем степе нынешнем шагом то инвертим
            // если не опмарчена то проверяем наличие то бежим до нужного литерала
            logicFunction.prevStep();
            logicFunction.nextStep();
            
            logicFunction.markLiteral(literalStack[stackCup], secondBranch);
            
            literalBranchStack[stackCup] = secondBranch;
            
            continue;
            
        } else {
            if (!literal) return logicFunction.getResult();
            
            ++stackCup;
            literalStack[stackCup] = *literal;
            literalBranchStack[stackCup] = firstBranch;
        }
    }
}
