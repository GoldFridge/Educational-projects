#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <algorithm>
#include <queue>
#include <set>
#include <random>
#include "card.h"

using namespace std;

class Player {
public :
    string name;
    vector<Card> hand;

    Player(const string& n) : name(n) {}

    void print() {
        for (int i = 0; i < hand.size(); i++) {
            cout << "[" << i << "]";
            hand[i].print();
        }
    }

    void drawCard(stack<Card> & deck) {
        if (!deck.empty()) {
            hand.push_back(deck.top());
            deck.pop();
        }
    }

    void playCard(int x) {
        hand.erase(hand.begin() + x);
    }

    bool hasUno() {
        return hand.size() == 2;
    }
    
    bool isOut() {
        return hand.empty();
    }
};
#endif
