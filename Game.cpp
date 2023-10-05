



#include "Game.h"
#include "Player.h"
#include "Board.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north): m_board(b), m_sPlayer(south), m_nPlayer(north) {

}

void Game::display() const {
    //display
    //adding lines to see each move easier
    cout << "-----------------------------------" << endl;
    cout << "North Player: " << m_nPlayer->name() << endl;
    cout << "  ";
    for (int i = 1; i < m_board.holes() + 1; i++) {
        cout << m_board.beans(NORTH, i) << " ";
    }
    cout << endl << m_board.beans(NORTH, 0);
   
    for (int i = 0; i <= 2 * m_board.holes(); i++) {
        cout << " ";
    }
    cout  << m_board.beans(SOUTH, 0) << endl;
    cout << "  ";
    for (int i = 1; i < m_board.holes() + 1; i++) {
        cout << m_board.beans(SOUTH, i) << " ";
    }
    cout << endl;
    cout << "South Player: " << m_sPlayer->name() << endl << endl;
    cout << "-----------------------------------" << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    //checking if winner is north or south or if game is over
    int northBeans = m_board.beans(NORTH, 0);
    int southBeans = m_board.beans(SOUTH, 0);
    if (m_board.beansInPlay(SOUTH) == 0 || m_board.beansInPlay(NORTH) == 0) {
        over = true;
        if (northBeans != southBeans) {
            hasWinner = true;
            if (northBeans > southBeans) {
                winner = NORTH;
            } else {
                winner = SOUTH;
            }
                
        } else {
            hasWinner = false;
        }
            
    } else {
        over = false;
    }
        
}

bool Game::move(Side s) {
    Player* curr;
    
    if (s == NORTH) {
        curr = m_nPlayer;
    } else {
        curr = m_sPlayer;
    }
    int move = curr->chooseMove(m_board, s);
    
    while (m_board.beans(s, move) == 0) {
        move = curr->chooseMove(m_board, s);
    }
    
    //what player chose
    cout << curr->name() << " chooses " << move << endl;
    Side endSide;
    int endHole;
    bool over;
    bool hasWinner;
    Side winner;
    m_board.sow(s, move, endSide, endHole);
    status(over, hasWinner, winner);
    display();

   
    //sweep beans if over
    if (over) {
        //display();
        for (int i = 1; i <= m_board.holes(); i++) {
            m_board.moveToPot(opponent(s), i, opponent(s));
        }
        
        display();
        std :: cout << "Sweeping remaining beans into pots." << std :: endl;
        return false;
    }

    while ((endHole == 0 || endHole == m_board.holes() + 1) && endSide == s) {
        //display();
        move = curr->chooseMove(m_board, s);
        while (m_board.beans(s, move) == 0) {
            move = curr->chooseMove(m_board, s);
        }
        cout << curr->name() << " chooses " << move << endl;
        m_board.sow(s, move, endSide, endHole);
        
       
        //sweep beans if over
        status(over, hasWinner, winner);
        if (over) {
            display();
            for (int i = 1; i <= m_board.holes(); i++) {
                m_board.moveToPot(opponent(s), i, opponent(s));
            }
            std :: cout << "Sweeping remaining beans into pots." << std :: endl;
            //display();
            return false;
        }
        display();
    }

    //sweep all of opposite into their pot or capture and game not over
    if (endSide == s && endHole != 0) {
        if (m_board.beans(endSide, endHole) - 1 == 0 && m_board.beans(opponent(endSide), endHole) != 0) {
            m_board.moveToPot(opponent(endSide), endHole,s);
            m_board.moveToPot(endSide, endHole, s);
            m_board.setBeans(endSide, endHole, 0);
            m_board.setBeans(opponent(endSide), endHole, 0);
            std :: cout << "Captured Beans into " << curr -> name() << "'s pot." << std :: endl;
            display();
        }
    }
   
    //sweep beans if over
    status(over, hasWinner, winner);
    if (over) {
        display();
        for (int i = 1; i <= m_board.holes(); i++) {
            m_board.moveToPot(opponent(s), i, opponent(s));
            m_board.moveToPot((s), i, (s)); //added not sure if necessary
        }
        std :: cout << "Sweeping remaining beans into pots." << std :: endl;
        return false;
    }
    return true;
}

void Game::play() {
    bool over = false;
    bool hasWinner;
    Side winner;
    Side currSide = SOUTH;
    status(over, hasWinner, winner);
    display();
    while (!over) { //game is not over
        move(currSide);
        
        if (!m_nPlayer->isInteractive() && !m_sPlayer->isInteractive()) {
            std::cout << "Press ENTER to continue" << endl;
            std::cin.ignore(1000, '\n');
        }

        //display();
        currSide = opponent(currSide);
        status(over, hasWinner, winner);
        
    }
   
    status(over, hasWinner, winner);
    display();
    
    
    if (hasWinner == true) {
        if(winner == NORTH) {
            //print out winner
            cout << "Winner is " + m_nPlayer->name() << endl;
            //cout <<  m_sPlayer ->name() <<" is Loser" << endl;
        } else if (winner == SOUTH){
            //print out winner
            cout << "Winner is " + m_sPlayer->name() << endl;
            //cout <<  m_nPlayer ->name() <<" is Loser" << endl;
        }
            
            
    } else {
        cout << "Tie" << endl;
    }
    
}

int Game::beans(Side s, int hole) const {
    if (hole < 0 || hole > m_board.holes()) {
        return -1;
    }
    return m_board.beans(s, hole);
}



