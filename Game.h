//
//  Game.hpp
//  Project3
//
//  Created by Drew Wan on 5/19/23.
//

#ifndef Game_h
#define Game_h
#include <iostream>
#include <vector>
#include "Board.h"
#include "Player.h"
#include "Side.h"
//class Board;
//class Player;


class Game {
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;
private:
    Board m_board;
    Player* m_sPlayer;
    Player* m_nPlayer;
    
    //Side m_Side;
};

#endif /* Game_h */
