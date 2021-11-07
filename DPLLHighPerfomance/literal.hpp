//
//  literal.hpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#ifndef literal_hpp
#define literal_hpp

const int FALSEL = -1;
const int TRUEL = 1;
const int UNMARKED = 0;
const int NOT_MODIFIED = -1;

class LiteralNode{
public:
    int mark;
    int modificationStep;
    
    LiteralNode(int m,int s) : mark(m), modificationStep(s) { };
    LiteralNode(LiteralNode& n) : mark(n.mark), modificationStep(n.modificationStep) {};
    LiteralNode() : mark(UNMARKED), modificationStep(NOT_MODIFIED) {};
};

#endif /* literal_hpp */
