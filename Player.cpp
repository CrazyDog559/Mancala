//
//  Player.cpp
//  Project3
//
//  Created by Drew Wan on 5/19/23.
//

#include "Player.h"






Player :: Player(std::string name): m_name(name){
    
}
std::string Player :: name() const {
    return m_name;
}


bool Player :: isInteractive() const {
    return false;
}
Player :: ~Player() {
    
}


//Human Player
HumanPlayer:: HumanPlayer(std::string name) : Player(name) {
    
}
HumanPlayer::~HumanPlayer() {
    
}
bool HumanPlayer :: isInteractive() const {
    return true;
}
int HumanPlayer :: chooseMove(const Board& b, Side s) const {
    //User Pick a hole
    std :: cout << "Select a hole, " << name() << ": ";
    int move = 0;
    
    //reprompt if there is another move
    
    while (!false) {
        std :: cin >> move;
        
        if (move <= 0 || move > b.holes()) {
            std :: cout << "The hole number must be from 1 to " << b.holes() << "." << std:: endl;
        } else if(b.beans(s, move) == 0) {
            std :: cout << "There are no beans in that hole." << std:: endl;
            std :: cout << "Select a hole, " << name() << ": ";
        } else {
            break;
        }
    }
    return move;
}


//BadPlayer

BadPlayer::BadPlayer(std:: string name) : Player(name) {
    
}
BadPlayer :: ~BadPlayer() {
    
}


int BadPlayer :: chooseMove(const Board& b, Side s) const {

    //picking far right hole
    
    if (b.beansInPlay(s) > 0) {
        for (int i = b.holes(); i != 0; i--) {
            if(b.beans(s,i) > 0) {
                return i;
            }
        }
    }
    return -1;
    
}





//SmartPlayer

SmartPlayer :: SmartPlayer(std :: string name) : Player(name) {
    
}

SmartPlayer :: ~SmartPlayer() {
    
}

int SmartPlayer::chooseMove(const Board& b, Side s) const {
    int bestHole = -1;
    int depth = 7;  //set to 8 to hit the timer cap
    int value;

    Timer timer;
    
    timer.start();
    
    double timerLimit = 4990;
    
    minimax(b, s, bestHole, value, depth, timer, timerLimit);

    // Stop the timer
    double elapsedTime = timer.elapsed();

    // Adjust the depth if the evaluation took less than 5 seconds
//    while (elapsedTime < 4990 && depth < 7) {
//        depth++;
//        // Restart the timer
//        Timer t;
//        t.start();
//        minimax(b, s, bestHole, value, depth);
//
//        // Stop the timer
//
//
//        if (elapsedTime >= 4000) {
//            std::cout << "Depth: " << depth << std::endl;
//            std::cout << "Elapsed Time: " << elapsedTime << "ms" << std::endl;
//            return bestHole;
//        }
//        elapsedTime = timer.elapsed() + t.elapsed();
//    }

    std::cout << "Elapsed Time: " << elapsedTime << "ms" << std::endl;
    return bestHole;
}


void SmartPlayer::minimax(const Board& b, Side s, int& bestHole, int& value, int depth, Timer &t, double tLimit) const {
    
    if (tLimit <= t.elapsed()) {
        value = evaluate(b);
        bestHole = -1;
        return;
    }
    if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0 || depth == 0) {
        // Game over, return the evaluation
        value = evaluate(b);
        bestHole = -1;
        return;
    } else {
        if (s == SOUTH) {
            value = INT_MIN; //set to lowest possible value
            int gValue; //compare to value
            int best_hole = -1;
            int firstHole = -1;
            //find first move if no good move
            for (int i = 1; i <= b.holes(); i++) {
                if (b.beans(s, i) != 0) {
                    firstHole = i;
                    break;
                }
            }
            //for every possible move
            for (int i = 1; i <= b.holes(); i++) {
                if (b.beans(s, i) != 0) {
                    Board copy(b);
                    Side endside;
                    int endhole;
                    
                    //double timeLimit = 4900;
                    //double startTime = t.elapsed();
                    
                    copy.sow(s, i, endside, endhole); //sow a copy of the board
                    if (!capture(s, endside, endhole, copy)) { //check if captured
                        minimax(copy, SOUTH, best_hole, gValue, depth, t, tLimit); //if not captured use recursion to make another move
                    } else {
                        minimax(copy, NORTH, best_hole, gValue, depth - 1, t, tLimit); //see what the opponenet would do
                    }
                    
                    //if not good holes do the first one
                    if (i == firstHole) {
                        bestHole = i;
                    }
                    //if greater, and not losing
                    if (gValue > value) {
                        value = gValue;
                        bestHole = i;
                    }
                    
//                    tLimit -= (t.elapsed() - startTime);
//                    if (tLimit <= 0)
//                        tLimit = 0;
                   
                }
            }
            return;
        } else if (s == NORTH) {
            value = INT_MAX; //set to max value
            int gValue;
            int best_hole = -1;
            int firstHole = -1;
            //for if there are no good moves
            for (int i = 1; i <= b.holes(); i++) {
                if (b.beans(s, i) != 0) {
                    firstHole = i;
                    break;
                }
            }
            //checking all moves
            for (int i = 1; i <= b.holes(); i++) {
                if (b.beans(s, i) != 0) {
                    Board copy(b);
                    Side endside;
                    int endhole;
                    //double startTime = t.elapsed();
                    //double timeLimit = 4900;
                    copy.sow(s, i, endside, endhole); //copy of the board
                    //if it captured
                    if (!capture(s, endside, endhole, copy)) {
                        minimax(copy, NORTH, best_hole, gValue, depth,t, tLimit); //recursion to move again
                    } else {
                        minimax(copy, SOUTH, best_hole, gValue, depth - 1, t, tLimit); //checking counter move
                    }
                    
                    //setting to firsthole if no good moves
                    if (i == firstHole) {
                        bestHole = i;
                    }
                    //if less than value and not gonna lose and better than current
                    if (gValue < value) {
                        value = gValue;
                        bestHole = i;
                    }
                    
//                    tLimit -= (t.elapsed() - startTime);
//                    if (tLimit <= 0)
//                        tLimit = 0;
                   
                }
            }
            return;
        }
    }
}




int SmartPlayer::evaluate(Board b) const {
    //Playing around with the evaluate to find a better strat
    //return b.beans(SOUTH, 0) - b.beans(NORTH, 0);
    return (b.beans(SOUTH, 0) - b.beans(NORTH, 0) + 1  * (b.beansInPlay(SOUTH) - b.beansInPlay(NORTH)));
}

bool SmartPlayer::capture(Side startSide, Side endSide, int endHole, Board& newBoard) const {
    if (endSide == startSide && endHole == 0 && newBoard.beansInPlay(startSide) != 0) {
        return false;
    } else if (endSide == startSide && newBoard.beans(endSide, endHole) == 1 && newBoard.beans(opponent(endSide), endHole) != 0) {
        newBoard.moveToPot(NORTH, endHole, startSide);
        newBoard.moveToPot(SOUTH, endHole, startSide);
        return true;
    } else {
        return true;
    }
}

    
    
    
 















