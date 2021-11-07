//
//  clause_node.cpp
//  DPLLHighPerfomance
//
//  Created by Евгений Сударский on 03.11.2021.
//

#include "clause_node.hpp"

ClauseNode::Builder& ClauseNode::Builder::add(int literal){
    this->literals.emplace_back(literal);
    return *this;
}

ClauseNode::Builder& ClauseNode::Builder::reset(){
    this->literals.clear();
    return *this;
}

ClauseNode* ClauseNode::Builder::build(){
//    std::sort(literals.begin(), literals.end(), [](int a,int b) { return std::abs(a) < std::abs(b); } );
    return new ClauseNode( this->literals ) ;
}

bool ClauseNode::Builder::isEmpty() const {
    return this->literals.empty();
}


/* ----------------Clause Node------------------- */

ClauseNode::ClauseNode(ClauseNode& node) {
    for(auto literal = node.begin(); literal != node.end(); ++literal ) {
        this->literals.push_back(*literal);
    }
    mark = node.mark;
    markStep = node.markStep;
}

ClauseNode::ClauseNode(std::vector<int>& lit) {
    for(auto literal = lit.begin(); literal != lit.end(); ++literal) {
        this->literals.push_back(*literal);
    }
}

int ClauseNode::getStep() {
    return markStep;
}

std::vector<int>::iterator ClauseNode::begin() {
    return this->literals.begin();
}

std::vector<int>::iterator ClauseNode::end() {
    return this->literals.end();
}

bool ClauseNode::isTrue() {
    return this->mark;
}

bool ClauseNode::isTrueOnStepOrMarkFalse(int step) {
    if (markStep == step) {
        mark = false;
    }
    return mark;
}

void ClauseNode::setMark(bool _mark, int step) {
    this->mark = _mark;
    markStep = _mark ? step : -1;
}

int ClauseNode::amountOfLiterals() {
    return (int)literals.size();
}
