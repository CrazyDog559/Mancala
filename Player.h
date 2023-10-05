//
//  Player.hpp
//  Project3
//
//  Created by Drew Wan on 5/19/23.
//

#ifndef Player_h
#define Player_h
#include <iostream>
#include <chrono>
#include <climits>
#include "Side.h"
#include "Board.h"
class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

class Player {
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    std :: string m_name;
    Board* m_board;
    
};

class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name);
    ~HumanPlayer();
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player {
public:
    BadPlayer(std::string name);
    ~BadPlayer();
   
    virtual int chooseMove(const Board& b, Side s) const;
    
};

class SmartPlayer : public Player {
public:
    SmartPlayer(std::string name);
    ~SmartPlayer();

    virtual int chooseMove(const Board& b, Side s) const;
    
private:
   
//    int evaluateBoard(const Board& b, Side s) const;
//    int minimax(const Board& b, Side currentPlayer, Side maximizingPlayer, Side minimizingPlayer, int depth) const;
    
    void minimax(const Board& b, Side s, int& bestHole, int& bestMove, int depth, Timer &t, double tLimit) const;
    //int minimax(const Board& b, Side s, int& bestHole, int& bestMove, int depth, int alpha, int beta) const;
   
    int evaluate(Board b) const;
    bool capture(Side startSide, Side endSide, int endHole, Board& newBoard) const;
   
};








#endif /* Player_h */
