//
//  main.cpp
//  Project3
//
//  Created by Drew Wan on 5/19/23.
//

#include <iostream>
#include "Board.h"
#include "Player.h"
#include "Game.h"


#include <cassert>

using namespace std;


void displayTestBoard(const Board& m_board) {
    std :: cout << "  ";
    for (int i = 1; i < m_board.holes() + 1; i++) {
       std :: cout << m_board.beans(NORTH, i) << " ";
    }
    std :: cout << std :: endl << m_board.beans(NORTH, 0);
    for (int i = 0; i <= 2 * m_board.holes(); i++) {
        std :: cout << " ";
    }
    std :: cout  << m_board.beans(SOUTH, 0) << std :: endl;
    std :: cout << "  ";
    for (int i = 1; i < m_board.holes() + 1; i++) {
        std :: cout << m_board.beans(SOUTH, i) << " ";
    }
    std :: cout << std :: endl;
}



void testBoard() {
    // Test constructor and initialization
    Board b(6, 4);
    assert(b.holes() == 6);
    assert(b.totalBeans() == 2 * (6 * 4 + 0));
    assert(b.beans(NORTH, 0) == 0);
    assert(b.beans(SOUTH, 0) == 0);
    for (int i = 1; i <= b.holes(); i++) {
        assert(b.beans(NORTH, i) == 4);
        assert(b.beans(SOUTH, i) == 4);
    }
    displayTestBoard(b);


    // Test sow() and beansInPlay()
    assert(b.beansInPlay(NORTH) == 24);
    assert(b.beansInPlay(SOUTH) == 24);
    Side endSide;
    int endHole;
   
    b.sow(NORTH, 3, endSide, endHole);
    assert(b.beans(NORTH, 3) == 0);
    assert(b.beans(SOUTH, 1) == 5);
    assert(b.beansInPlay(NORTH) == 22);
    assert(b.beansInPlay(SOUTH) == 25);
    displayTestBoard(b);

    // Test moveToPot() and beans()
    b.moveToPot(NORTH, 2, NORTH);
    assert(b.beans(NORTH, 2) == 0);
    assert(b.beans(NORTH, 0) == 6);
    assert(b.beansInPlay(NORTH) == 17);
    assert(b.totalBeans() == 48);
    displayTestBoard(b);
}


void testGame() {
    // Create a test board
    Board board(6, 4);
    
    // Create test players
    Player* southPlayer = new BadPlayer("South");
    Player* northPlayer = new SmartPlayer("North");
    
    // Create a game
    Game game(board, southPlayer, northPlayer);
    
    // Test display function
    game.display();
    
    // Test status function
    bool over, hasWinner;
    Side winner;
    game.status(over, hasWinner, winner);
    assert(!over);  // Game should not be over yet
    assert(!hasWinner);  // There should be no winner yet
    
    // Test move function
    bool validMove = game.move(SOUTH);
    assert(validMove);  // A valid move should be made
    
    // Test beans function
    assert(game.beans(NORTH, 0) == 0);  // Number of beans in North's pot should be 0
    
    // Clean up
    delete southPlayer;
    delete northPlayer;
    
    std :: cout << "testGame Passed " << std :: endl;
}


void doBoardTests()
    {
        Board b(3, 2);
        assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
                        b.beans(SOUTH, POT) == 0  &&  b.beansInPlay(SOUTH) == 6);
        b.setBeans(SOUTH, 1, 1);
        b.moveToPot(SOUTH, 2, SOUTH);
        assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
                    b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
            b.beansInPlay(SOUTH) == 3);
        Side es;
        int eh;
        b.sow(SOUTH, 3, es, eh);
        assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
                    b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
            b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    }

void doPlayerTests()
    {
        HumanPlayer hp("Marge");
        assert(hp.name() == "Marge"  &&  hp.isInteractive());
        BadPlayer bp("Homer");
        assert(bp.name() == "Homer"  &&  !bp.isInteractive());
        SmartPlayer sp("Lisa");
        assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
        Board b(3, 2);
        b.setBeans(SOUTH, 2, 0);
        cout << "=========" << endl;
        int n = hp.chooseMove(b, SOUTH);
        cout << "=========" << endl;
        assert(n == 1  ||  n == 3);
        n = bp.chooseMove(b, SOUTH);
        assert(n == 1  ||  n == 3);
        n = sp.chooseMove(b, SOUTH);
        assert(n == 1  ||  n == 3);
    }


void doGameTests()
    {
        BadPlayer bp1("Bart");
        BadPlayer bp2("Homer");
        Board b(3, 0);
        b.setBeans(SOUTH, 1, 2);
        b.setBeans(NORTH, 2, 1);
        b.setBeans(NORTH, 3, 2);
        Game g(b, &bp1, &bp2);
        bool over;
        bool hasWinner;
        Side winner;
          //    Homer
          //   0  1  2
          // 0         0
          //   2  0  0
          //    Bart
        g.status(over, hasWinner, winner);
        assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
        g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
        g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

        g.move(SOUTH);
          //   0  1  0
          // 0         3
          //   0  1  0
        g.status(over, hasWinner, winner);
        assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
        g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

        g.move(NORTH);
          //   1  0  0
          // 0         3
          //   0  1  0
        g.status(over, hasWinner, winner);
        assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
        g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

        g.move(SOUTH);
          //   1  0  0
          // 0         3
          //   0  0  1
        g.status(over, hasWinner, winner);
        assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
        g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

        g.move(NORTH);
          //   0  0  0
          // 1         4
          //   0  0  0
        g.status(over, hasWinner, winner);
        assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
        g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
        g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
        assert(hasWinner && winner == SOUTH);
    }



int main() {
//    testBoard();
//    testGame();
//    doGameTests();
//    doPlayerTests();
//    doBoardTests();
//    std :: cout << "passed" << std :: endl;
    
    
    SmartPlayer hp("Smart");
    BadPlayer bp("Bad");
    Board b(6, 4);
    Game g(b, &hp, &bp);
    g.play();
//
    
}

