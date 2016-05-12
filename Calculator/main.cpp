//
//  main.cpp
//  Calculator
//
//  Created by Александр on 04.03.16.
//  Copyright © 2016 Alexander Besedin. All rights reserved.
//

#include <iostream>
#include <stack>
#include <string>

using namespace std;

void pushNumFromStr(string *stringNum, stack<float> *numStack) {
    float num;
    if (stringNum->empty() == false) {
        num = atof(stringNum->c_str());
        numStack->push(num);
        *stringNum = "";
    }
}

float calc(float op1, float op2, char operation) {
    switch (operation) {
        case '+':
            return op1 + op2;
            break;
            
        case '-':
            return op2 - op1;
            break;
            
        case  '*':
            return op1 * op2;
            break;
            
        case '/':
            return op2 / op1;
            break;
            
        default:
            return NULL;
    }
}

void doCalculation(stack<float> *fromNumStack, stack<char> *opStack) {
    float calculatedNum;
    float op1 = fromNumStack->top();
    fromNumStack->pop();
    float op2 = fromNumStack->top();
    fromNumStack->pop();
    char operation = opStack->top();
    opStack->pop();
    calculatedNum = calc(op1, op2, operation);
    fromNumStack->push(calculatedNum);
}

//  Функция которая определяет разницу в проритетах между входной оператор op1 и оператор op2 на вершине стэка.
//  Обозначение возращаемых значений: E - Equal, L - Lower, H - Higher, e - error.
char Difference(char op1, char op2) {
    int priority1;
    int priority2;
    switch (op1)
    {
        case '/':
        case '*':
            priority1 = 1;
            break;
        case '+':
        case '-':
            priority1 = 0;
    }
    switch (op2)
    {
        case '/':
        case '*':
            priority2 = 1;
            break;
            
        case '+':
        case '-':
            priority2 = 0;
            break;
            
        case '(':
            priority2 = -1;
            break;
    }
    if (priority1 - priority2 < 0) {
        return 'L';
    } else {
        if (priority1 - priority2 > 0) {
            return 'H';
        } else {
            return 'E';
        }
    }
}

void pushOperation(char operation, stack<char> *toOpStack, stack<float> *withUseNumStack) {
    stack<char> *opStack = toOpStack;
    stack<float> *numStack = withUseNumStack;
    char difference = ' ';
    if (opStack->empty()) {
        difference = 'H';
    } else {
        difference = Difference(operation, opStack->top());
    }
    while (difference != 'H') {
        doCalculation(numStack, opStack);
        if (opStack->empty() == false) {
            difference = Difference(operation, opStack->top());
        } else {
            difference = 'H';
        }
    }
    opStack->push(operation);
}

int main() {
    
    stack<char> opStack;
    stack<float> numStack;
    
    string calculateIt;
    getline(cin, calculateIt);
    
    string stringNum = "";
    
    char symbol = ' ';
    for (int i = 0; symbol != '\0'; i++) {
        if (calculateIt[i] == ' ') {
            calculateIt.erase(i, 1);
            symbol = calculateIt[i];
        } else {
            symbol = calculateIt[i];
        }
        
        switch (symbol) {
            case '+':
            case '-':
            case '*':
            case '/':
                pushNumFromStr(&stringNum, &numStack);
                pushOperation(symbol, &opStack, &numStack);
                break;

            case '0'...'9':
            case '.':
                stringNum += symbol;
                break;
            
            case '(':
                opStack.push(symbol);
                break;
                
            case ')':
                pushNumFromStr(&stringNum, &numStack);
                while (opStack.top() != '(') {
                    doCalculation(&numStack, &opStack);
                }
                opStack.pop();
                break;
        }

    }
    pushNumFromStr(&stringNum, &numStack);
    while (opStack.empty() != true) {
        doCalculation(&numStack, &opStack);
    }
    printf("%s = %f\n", calculateIt.c_str(), numStack.top());
    return 0;
}
