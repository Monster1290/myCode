//
//  main.cpp
//  interpolation
//
//  Created by Александр on 10.04.16.
//  Copyright © 2016 Alexander Besedin. All rights reserved.
//
//  Составление полинома Лагранжа по заданным точкам в читаемом для человека виде.

#include <iostream>
#include <vector>
using namespace std;

const int numOfPoints = 3;


float Combibation(int numOfElementsInCombination, vector<float> valueOfX, int currIndex, int indexForSubscript) {
    if (numOfElementsInCombination == 0) {
        return 1;
    }
    if (currIndex == indexForSubscript) {
        return Combibation(--numOfElementsInCombination , valueOfX, currIndex, ++indexForSubscript);
    } else {
        return valueOfX[indexForSubscript] * Combibation(--numOfElementsInCombination, valueOfX, currIndex, ++indexForSubscript);
    }
}

float sumOfCombiations(int numOfElementsInCombination, int numOfElements, int currIndex, vector<float> valueOfX) {
    if (numOfElementsInCombination == numOfElements - 1) {
        float oneCombination = 1;
        for (int i = 1; i <= numOfElements; i++) {
            if (i == currIndex) {
                continue;
            }
            oneCombination *=valueOfX[i];               // Находим произведение всех Х-ов
        }
        return oneCombination;    // Убираем оттуда элемент текущего i-того
    }
    if (numOfElementsInCombination == 0) {
        return 1;
    }
    int sum = 0;
    for (int index = 1; index <= numOfElements; index++) {
        if (index == currIndex) {
            continue;
        }
        float combination = Combibation(numOfElementsInCombination, valueOfX, currIndex, index);
        sum += combination;
    }
    return sum;
}

float getConstant(int index, int numOfElements, vector<vector<float>> func) {
    float denominator = 1;
    for (int i = 1; i <= numOfElements; i++) {
        if (i == index) {
            continue;
        }
        denominator *= func[0][index] - func[0][i];
    }
    return (func[1][index] / denominator);
}

void printLagrangePolynomial(vector<float> multipliers) {
    for (int i = numOfPoints - 1; i >= 0; i--) {
        cout << multipliers[i];
        switch (i) {
            case 0:
                break;
                
            case 1:
                cout << "X";
                break;
                
            default:
                cout << "X^" << i;
                break;
        }
        cout << " + ";
    }
}

int main() {
    vector<vector<float>> func =
    {
        {0, 0, 1, 5},
        {0, 2, 3, 147}
    };
    float constants[numOfPoints + 1];
    for (int i = 1; i <= numOfPoints; i++) {
        constants[i] = getConstant(i, numOfPoints, func);
    }
    int sign = numOfPoints % 2 == 1 ? 1 : -1;
    vector<float> multipliers;
    for (int i = 1; i <= numOfPoints; i++) {
        float multiplier = 0;
        for (int j = 1; j <= numOfPoints; j++) {
            multiplier += constants[j] * sumOfCombiations(numOfPoints - i, numOfPoints, j, func[0]);
        }
        multiplier *= sign;
        multipliers.push_back(multiplier);
        sign *= -1;
    }
    printLagrangePolynomial(multipliers);
    
    return 0;
}





















