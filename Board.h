//
//  Board.hpp
//  Project3
//
//  Created by Drew Wan on 5/19/23.
//

#ifndef Board_h
#define Board_h

#include <iostream>
#include <vector>
#include "Side.h"


class Board {
public:
    Board(int nHoles, int nInitialBeansPerHole);
    
    //Board(const Board& other);
    //~Board();
    //Board& operator=(const Board& rhs);
    
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
  
    int m_nHoles;
    
    std :: vector<int> m_nBeansNorth;
    std :: vector<int> m_nBeansSouth;
    
    bool sowBeans(Side s, int hole, Side& endSide, int& endHole);
    int getNextHole(int currentHole, Side currentSide);
//    int m_totalHoles;
//    int m_nBeansNorth;
//    int m_nBeansSouth;
//    int* board1;
//    int* board2;
    
    
};

#endif /* Board_h */
