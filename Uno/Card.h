#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <algorithm>
#include <queue>
#include <set>
#include <random>
#include <cstdlib>

using namespace std;

typedef long long ll;

enum class Color { RED, GREEN, BLUE, YELLOW , BLACK};
enum class Value { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, SKIP, REVERSE, DRAW_TWO,UNO_FLIP };

string colorToString(Color c) {
    switch (c) {
        case Color::RED: return "\033[31mRed\033[0m";     // Red text
        case Color::GREEN: return "\033[32mGreen\033[0m"; // Green text
        case Color::BLUE: return "\033[34mBlue\033[0m";   // Blue text
        case Color::YELLOW: return "\033[33mYellow\033[0m"; // Yellow text
        case Color::BLACK: return "Black";
        default: return "Unknown";
    }
}

string valueToString(Value v) {
    switch (v) {
        case Value::ZERO:    return "0";
        case Value::ONE:     return "1";
        case Value::TWO:     return "2";
        case Value::THREE:   return "3";
        case Value::FOUR:    return "4";
        case Value::FIVE:    return "5";
        case Value::SIX:     return "6";
        case Value::SEVEN:   return "7";
        case Value::EIGHT:   return "8";
        case Value::NINE:    return "9";
        case Value::SKIP:    return "Skip";
        case Value::REVERSE: return "Reverse";
        case Value::UNO_FLIP:return "UNO Flip";
        case Value::DRAW_TWO:return "Draw Two";
        default:             return "Unknown";
    }
}

bool flip = false;

class Card {
public:
    Color color;
    Value value;
    Color flipcolor;
    Value flipvalue;

    Card(Color c, Value v , Color fc , Value fv) : color(c), value(v) , flipcolor(fc), flipvalue(fv) {}

    void print() {
        if(!flip)
            cout << colorToString(color) << " " << valueToString(value) << "\033[0m" << endl;
        else 
            cout << colorToString(flipcolor) << " " << valueToString(flipvalue) << "\033[0m" << endl;
    }
};
#endif
