#ifndef UNO_H
#define UNO_H

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
#include "player.h"
#include "card.h"

using namespace std;

typedef long long ll;

class Game {
private:
    vector<Player> players;
    stack<Card> deck;
    stack<Card> draw;

    void initializeDeck() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j <= 12; ++j) {
                deck.push(Card(static_cast<Color>(i), static_cast<Value>(j) ,static_cast<Color>(3 - i), static_cast<Value>(12 - j)));
            }
        }
        deck.push(Card(static_cast<Color>(4), static_cast<Value>(13),static_cast<Color>(4), static_cast<Value>(13)));
        deck.push(Card(static_cast<Color>(4), static_cast<Value>(13),static_cast<Color>(4), static_cast<Value>(13)));
        deck.push(Card(static_cast<Color>(4), static_cast<Value>(13),static_cast<Color>(4), static_cast<Value>(13)));
        deck.push(Card(static_cast<Color>(4), static_cast<Value>(13),static_cast<Color>(4), static_cast<Value>(13)));
        vector<Card> elements;
        while (!deck.empty()) {
            elements.push_back(deck.top());
            deck.pop();
        }
        random_device rd;
        mt19937 g(rd());
        shuffle(elements.begin(), elements.end(), g);
        for (const auto& element : elements) {
            deck.push(element);
        }
    }

    void initializePlayers(int n) {
        for (int i = 0; i < n; i++) {
            players.emplace_back("Player" + to_string(i));
        }
        for (auto& player : players) {
            for (int i = 0; i < 7; ++i) {
                player.drawCard(deck);
            }
        }
    }

    stack<Card> shuffleDeck(stack<Card> & deck) {
        vector<Card> elements;
        while (!deck.empty()) {
            elements.push_back(deck.top());
            deck.pop();
        }
        stack<Card> res;
        random_device rd;
        mt19937 g(rd());
        shuffle(elements.begin(), elements.end(), g);
        for (const auto& element : elements) {
            res.push(element);
        }
        return res;
    }

public :
    Game() {
        initializeDeck();
        int n;
        cout << "Chose numbers of Players: " ;
        cin >> n;
        initializePlayers(n);
        system("cls");
    }

    void Monitor() {
        for(int i = 1 ;i < players.size();i ++) {
            cout << "Player " << i << " have " << players[i].hand.size() << " cards" << endl; 
        }
    }

    bool choosingCard(int x , int & t , int & k) {
        if(deck.empty()) {
            deck = shuffleDeck(draw);
            draw.push(deck.top());
            deck.pop();
        }
        if (x == -1) {
            players[t].drawCard(deck);
            return false;
        }
        if(colorToString(players[t].hand[x].color) == "Black") {
            draw.push(players[t].hand[x]);
            players[t].playCard(x);
            t += k;
            flip = !flip;
        }
        else if(draw.top().color == players[t].hand[x].color && valueToString(players[t].hand[x].value) == "Skip") {
            draw.push(players[t].hand[x]);
            players[t].playCard(x);
            t += k;
            if(t >= players.size()) {
                t = 0;
            }
            if(t < 0) {
                t = players.size() - 1;
            }
        }
        else if(draw.top().color == players[t].hand[x].color && valueToString(players[t].hand[x].value) == "Reverse") {
            k *= -1;
            draw.push(players[t].hand[x]);
            players[t].playCard(x);
        }
        else if(draw.top().color == players[t].hand[x].color && valueToString(players[t].hand[x].value) == "Draw Two") {
            draw.push(players[t].hand[x]);
            if(t + k < players.size() && t + k >= 0) {
                players[t + k].drawCard(deck);
                players[t + k].drawCard(deck);
            }
            else {
                if(k == -1) {
                    players[players.size() - 1].drawCard(deck);
                    players[players.size() - 1].drawCard(deck);
                }
                else {
                    players[0].drawCard(deck);
                    players[0].drawCard(deck);
                }
            }
            players[t].playCard(x);
        }
        else if (draw.top().color == players[t].hand[x].color || draw.top().value == players[t].hand[x].value) {
            draw.push(players[t].hand[x]);
            players[t].playCard(x);
        }
        else {
           // cout << "Error!" << endl;
            return false;
        }
        t += k;
        if(t < 0) {
            t = players.size() - 1;
        }
        if (t >= players.size()) {
            t = 0;
        }
        return true;
    }

    void play() {
        draw.push(deck.top());
        int k = 1, t = 0;
        deck.pop();
        while (true) {
            if(t != 0) {
                bool flag = false;
                for(int x = 0;x < players[t].hand.size();x ++) {
                    if(choosingCard(x , t , k)) {
                        flag = true;
                        break;
                    }
                    else {
                        continue;
                    }
                }
                while (!flag) {
                    players[t].drawCard(deck);
                    if(choosingCard(players[t].hand.size() - 1 ,t,k)) {
                        flag = true;
                        break;
                    }
                    else {
                        continue;
                    }
                }
                continue;
            }
            Monitor();
            draw.top().print();
            cout << "Chose card in your hand Player " + to_string(t) + "(-1 for to draw card):" << endl;
            players[t].print();
            string s;
            cin >> s;
            int x = atoi(s.c_str());
            if(x == -1) {
                players[t].drawCard(deck);
                system("cls");
                continue;
            }
            while(true) {
                if(choosingCard(x ,  t , k)) {
                    break;
                }
                else {
                    cout << "Error!" << endl;
                    break;
                }
            }
            system("cls");
        }
    }
};

int main() {
    Game game;
    game.play();
    return 0;
}

#endif
