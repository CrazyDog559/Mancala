


#include "Board.h"
#include <vector>
#include <iostream>
using namespace std;

#include "Side.h"

//Two vectors: One for the North board and one for the South board
Board::Board(int nHoles, int nInitialBeansPerHole) :m_nHoles(nHoles) {
    
    //if nHoles less than one, set to 1
    if (nHoles < 1) {
        nHoles = 1;
    }
    //if nInitialBeans less than 1, set to 0.
    if (nInitialBeansPerHole < 1) {
        nInitialBeansPerHole = 0;
    }
    //Set the pots
    m_nBeansNorth.push_back(0);
    m_nBeansSouth.push_back(0);

    for (int i = 0; i < m_nHoles; i++) {
        m_nBeansNorth.push_back(nInitialBeansPerHole);
        m_nBeansSouth.push_back(nInitialBeansPerHole);
    }
}
//ended up not needed copy constructor
//Board::Board (const Board &other) {
//    m_nHoles = other.m_nHoles;
//    
//    for(int i = 0; i < other.m_nBeansNorth.size();i++) {
//        m_nBeansNorth.push_back(other.m_nBeansNorth[i]);
//        m_nBeansSouth.push_back(other.m_nBeansSouth[i]);
//    }
//}

//return m_nHoles
int Board::holes() const {
    return m_nHoles;
}

int Board::beans(Side s, int hole) const
{
    if (hole < 0 || hole > m_nHoles){
        return -1;
    }
    //return north beans
    if (s == NORTH) {
        return m_nBeansNorth[hole];
    }
    //return south beans
    return m_nBeansSouth[hole];
}

int Board::beansInPlay(Side s) const {
    //how many beans are on the board
    int nIndicatedSide = 0;
    if (s == NORTH) {
        for (int i = 1; i < holes() + 1; i++) {
            nIndicatedSide += m_nBeansNorth[i];
        }
    }

    if (s == SOUTH) {
        for (int i = 1; i < holes() + 1; i++) {
            nIndicatedSide += m_nBeansSouth[i];
        }
    }
    return nIndicatedSide;
}

int Board::totalBeans() const {
    //the total beans
    return (beansInPlay(NORTH) + beansInPlay(SOUTH) + beans(NORTH, 0) + beans(SOUTH, 0));
    
}


bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    if (hole <= 0 || hole > m_nHoles || beans(s, hole) == 0) {
        return false;
    } else {
        int currBeans = beans(s, hole);

        if(s == NORTH) {
            m_nBeansNorth[hole] = 0;
        } else {
            m_nBeansSouth[hole] = 0;
        }
        
        Side currSide = s;
        int currHole = hole;
        if (currSide == NORTH) {
            currHole--;
        } else {
            currHole++;
        }
        
        while (currBeans > 0) {
            if (currHole == 0 || currHole == m_nHoles + 1) {
                //if cuurent hole is a pot update the pot and change side
                if (currSide == NORTH) {
                    if (s == NORTH) {
                        m_nBeansNorth[POT]++;
                        currBeans--;
                    }
                    if (currBeans > 0) {
                        currHole = 1;
                        currSide = SOUTH;
                    }
                } else {
                    if (s == SOUTH) {
                        m_nBeansSouth[POT]++;
                        currBeans--;
                    }
                    if (currBeans > 0) {
                        currHole = m_nHoles;
                        currSide = NORTH;
                    }
                }

            } else {
                //sow the beans to the next hole on the cuurent side
                if (currSide == NORTH) {
                    m_nBeansNorth[currHole]++;
                } else {
                    m_nBeansSouth[currHole]++;
                }
                currBeans--;
                if (currBeans > 0) {
                    //move to the next hole
                    if (currSide == NORTH) {
                        currHole--;
                    } else {
                        currHole++;
                    }
                        
                }
            }
        }
        //update the final hole and side after the sow
        endSide = currSide;
        endHole = currHole;
        return true;
    }
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (hole <= 0 || hole > m_nHoles) {
        return false;
    }
    //move beans from specifed hole ot the potOwner
    if (potOwner == NORTH && s == NORTH) {
        m_nBeansNorth[0] += m_nBeansNorth[hole];
        m_nBeansNorth[hole] = 0;
    } else if (potOwner == NORTH && s == SOUTH){
        m_nBeansNorth[0] += m_nBeansSouth[hole];
        m_nBeansSouth[hole] = 0;
    } else if (potOwner == SOUTH && s == NORTH) {
        m_nBeansSouth[0] += m_nBeansNorth[hole];
        m_nBeansNorth[hole] = 0;
    } else if (potOwner == SOUTH && s == SOUTH) {
        m_nBeansSouth[0] += m_nBeansSouth[hole];
        m_nBeansSouth[hole] = 0;
    }

    return true;
}

bool Board::setBeans(Side s, int hole, int beans) {
    if (hole < 0 || hole > m_nHoles || beans < 0) {
        return false;
    }
    //set number of beans in the specifed hole for the given side
    if (s == NORTH){
        m_nBeansNorth[hole] = beans;
        return true;
    }

    m_nBeansSouth[hole] = beans;
    return true;
}


//failed sow
/*
bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    if (hole <= 0 || hole > m_nHoles || beans(s, hole) == 0) {
        return false;
    }
    
    //North Side
    if (s == NORTH) {
        //Counter for the total number of beans to be dropped
        int beansNorth = beans(NORTH, hole);
        m_nBeansNorth[hole] = 0;
        int north = hole - 1;

        //Sow beans into each hole except the opponent's pot
        while (beansNorth > 0) {
            //Sowing on the North side
            for ( ; north >= 0; north--) {
                m_nBeansNorth[north]++;
                beansNorth--;

                if (beansNorth == 0) {
                    endSide = NORTH;
                    endHole = north;
                    break;
                }
            }

            if (endSide == NORTH && endHole == north) {
                break;
            }
               

            north = m_nHoles;    //Reset the hole to the last hole if we return to the North side

            int southCount = -10;    //Initialized to an arbitrary value
            //Sowing on the South side
            for (int i = 1; i < m_nHoles + 1; i++) {
                m_nBeansSouth[i]++;
                beansNorth--;

                if (beansNorth == 0) {
                    endSide = SOUTH;
                    endHole = i;
                    southCount = i;
                    break;
                }
            }

            if (endSide == SOUTH && endHole == southCount) {
                break;
            }
                
        }
    } else if (s == SOUTH) {
        //Counter for total number of beans to be deposited
        int beansSouth = beans(SOUTH, hole);
        m_nBeansSouth[hole] = 0;
        int south = hole + 1;
        int first = 1;        //This is used to sow into the pot for the first time we run through

        //Sow beans into each hole except opponent's pot
        while (beansSouth > 0)
        {
            //Sowing on the south side

            for ( ; south < m_nHoles + 1; south++)
            {
                m_nBeansSouth[south]++;
                beansSouth--;

                if (beansSouth == 0)
                {
                    //If we are at the South pot, break out of this loop
                    if (south == m_nHoles + 1)
                        break;
                    endSide = SOUTH;
                    endHole = south;
                    break;
                }
            }

            //Break out of the while loop if we end on a regular South hole
            if (endSide == SOUTH && endHole == south)
                break;

            //Fill in the pot when you sow for the first time
            if (first == 1)
            {
                m_nBeansSouth[0]++;
                beansSouth--;
                first++;
                
                if (beansSouth == 0)
                {
                    south = 0;
                    endSide = SOUTH;
                    endHole = south;
                    break;
                }
            }

            south = 1;    //Reset the hole to the first hole if we return to the south side

            //This condition is invoked if we end on South's Pot
            if (beansSouth == 0)
            {
                endSide = SOUTH;
                endHole = south;
                break;
            }

            int northTracker = -10;    //Initialized to arbitrary value
            //Sowing on the north side
            for (int i = m_nHoles; i >= 1; i--)
            {
                m_nBeansNorth[i]++;
                beansSouth--;

                if (beansSouth == 0)
                {
                    endSide = NORTH;
                    endHole = i;
                    northTracker = i;
                    break;
                }
            }

            //Break out of the while loop if we end on the North side
            if (endSide == NORTH && endHole == northTracker)
                break;
        }
    }

    return true;
}
*/
