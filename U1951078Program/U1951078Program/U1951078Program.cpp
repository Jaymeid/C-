#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include<string>
using namespace std;

#define HAND_SIZE 8
#define DECK_MAX_SIZE 98

void display_game(int* piles, int* hand)
{
    printf("Play Piles:\n1:A[%d], 2:A[%d], 3:D[%d], 4:D[%d]\n",
        piles[0], piles[1], piles[2], piles[3]);

    printf("Player hand:\n");
    for (int n = 0; n < HAND_SIZE; n++)
    {
        printf("%d:[%d], ", n + 1, hand[n]);
    }
    printf("\n");
}

void ascendingPiles(int cardDecision, int playPiles[4], int pileSelected, int playerHand[HAND_SIZE], int playDeck[DECK_MAX_SIZE], int& playIndex, int cardsSelected[8], int& numberPlayed, bool exists) { //Factored into a method so the main game did not have repeated code
    int cardPlayed = playerHand[cardDecision - 1];
    if (cardPlayed > playPiles[pileSelected - 1] || cardPlayed == playPiles[pileSelected - 1] - 10 && cardPlayed != -1) { //Checks if card is 10 less than either of the ascending piles
        if (exists == false) { //Wont allow user to place card if it has already been picked in the same turn
            playPiles[pileSelected - 1] = cardPlayed; //Places current card on the selected pile
            if (playIndex != 98) {
                playerHand[cardDecision - 1] = playDeck[playIndex]; //Replaces the card in the hand with a new card from the play deck
                playIndex++;
            }
            else {
                playerHand[cardDecision - 1] = -1; //If play deck is finished relpace the card with -1 to signify the hand is emptying
            }
            cardsSelected[cardPlayed - 1] = cardDecision;
            numberPlayed++;
            //Ensures next card taken from playdeck isnt the same card
        }
        else {
            printf("\nCannot play the same card twice in one round\n");
        }
    }
    else {
        printf("Place a card in ascending order or directly 10 less than the current card\n");
    }
}

void descendingPiles(int cardDecision, int playPiles[4], int pileSelected, int playerHand[HAND_SIZE], int playDeck[DECK_MAX_SIZE], int& playIndex, int cardsSelected[8], int& numberPlayed, bool exists) {
    int cardPlayed = playerHand[cardDecision - 1];
    if (cardPlayed < playPiles[pileSelected - 1] || cardPlayed == playPiles[pileSelected - 1] + 10 && cardPlayed != -1) { //Is the card more than the value in selected pile
        if (exists == false) {
            playPiles[pileSelected - 1] = cardPlayed;
            if (playIndex != 98) {
                playerHand[cardDecision - 1] = playDeck[playIndex];
                playIndex++;
            }
            else {
                playerHand[cardDecision - 1] = -1;
            }
            cardsSelected[cardPlayed] = cardDecision;
            numberPlayed++;
            if (playIndex < 98) {
                playIndex++;
            }
        }
        else {
            printf("\nCannot play the same card twice in one round\n");
        }
    }
    else {
        printf("Place a card in descending order or directly 10 more than the current card\n");
    }
}

int main(int argc, const char** argv)
{
    //initialisation
    int deck[DECK_MAX_SIZE];
    int hand[8];
    int piles[4];
    int deck_top = 0;
    int current_turn = 1;
    int cardsSelected[8];
    int playerScore;
    int amountOfCardsToPick = 2;
    bool invalidInput = true;

    int playDeckIndex = HAND_SIZE;
    srand(time(0)); //Seed a random number based on the time


    piles[0] = piles[1] = 1;
    piles[2] = piles[3] = 100;

    bool finished = false;
    bool turnFinished = false;


    for (int n = 0; n < DECK_MAX_SIZE; n++)
    {
        deck[n] = n + 2;
    }

    for (int card = 0; card < DECK_MAX_SIZE; card++) { // Randomize deck
        int index = rand() % DECK_MAX_SIZE;
        int temp = deck[card];
        deck[card] = deck[index]; //swap randomized card with card in order
        deck[index] = temp;
    }






    for (int index = 0; index < HAND_SIZE; index++) { // give player a deck of cards equivalent to hand size
        hand[index] = deck[index];
        deck[index] = -1; //replace card that was removed from deck and put in hand to -1. Signifies it is not available to be picked again
    }

    //Main game loop
    while (!finished)
    {
        display_game(piles, hand);
        turnFinished = false;
        char cDecision = 'a'; //initialised so compiler doesnt have a melt down
        int cardDecision = 0;
        int card;
        char pDecision = 'a';
        int pileDecision = 0;
        int cardsPlayed = 0;
        char endTurnDecision;
        bool exists = false;
        bool isInteger = false;

        for (int i = 0; i < HAND_SIZE; i++) { //resets the cards that have been selected before the next turn begins
            cardsSelected[i] = false;
        }


        if (playDeckIndex != 98) { //If the play deck is empty then make the minimum number of cards played to be 1
            amountOfCardsToPick = 2;
        }
        else {
            amountOfCardsToPick = 1;
        }

        while (!turnFinished) { //While loop which seperates the game into turns
            do {
                printf("When you are ready to play your turn please select a card number [%d] to be played\nPlease select a card within your handsize\n", cardsPlayed + 1);
                cin >> cDecision;
            } while (isdigit(cDecision) == false && (int)cDecision < 58); //Make sure the input given is a number
            cardDecision = ((int)cDecision - 48); // Account for ASCII offset to make it an actual digit. Offset is 49 but in this case 48 is used to make it useable in the arrays as an index


            if (cardDecision <= 0 || cardDecision > HAND_SIZE) { //Ensures that the user doesnt pick a number which is outside of their hand size
                printf("Please select a number between 1 and your hand size\n");
            }
            else {
                card = hand[cardDecision]; //get the actual value of the card in the hand
                do {
                    printf("Which play pile would you like to place your card\n\n");
                    cin >> pDecision;
                } while (isdigit(pDecision) == false); //Make sure user enters a number as a pile choice
                pileDecision = (int)pDecision - 48;
                exists = std::find(std::begin(cardsSelected), std::end(cardsSelected), cardDecision) != std::end(cardsSelected); //Checks if the card has been selected during this turn. Gives the illusion that cards will only be picked up after turn finished
                if (exists == true) { //If already has been picked prompt the user and ask for the current selection again
                    printf("\nCannot play the same card twice in one round\n");
                }
                else {
                    if (pileDecision < 1 || pileDecision > 4) {
                        printf("Please select a pile between 1 and 4\n");
                    }
                    else {
                        if (pileDecision == 1 || pileDecision == 2) {
                            ascendingPiles(cardDecision, piles, pileDecision, hand, deck, playDeckIndex, cardsSelected, cardsPlayed, exists);
                        }
                        else {
                            descendingPiles(cardDecision, piles, pileDecision, hand, deck, playDeckIndex, cardsSelected, cardsPlayed, exists);
                        }
                        cardsSelected[cardsPlayed] = cardDecision; //Add the card to an array which holds all the cards selected this turn
                    }
                }


                if (cardsPlayed >= amountOfCardsToPick) { //If minimum number of cards played is reach ask if the user wishes to end the turn
                    printf("\nDo you wish to end your turn?(Y/N)\nIf you wish to end the game press 9\n");
                    cin >> endTurnDecision;
                    if (toupper(endTurnDecision) == 'Y') {
                        printf("\nTurn ended\n");
                        turnFinished = true;
                        invalidInput = false;
                    }
                    else if (toupper(endTurnDecision) == 'N') {
                        printf("\nTurn continued\n");
                        invalidInput = false;
                    }
                    else if (endTurnDecision == '9') {
                        turnFinished = true;
                        finished = true;
                    }

                }
                bool empty;
                for (int index = 0; index < HAND_SIZE; index++) {
                    if (hand[index] != -1) { //if anything in the array is not -1 then the hand is not empty thus game is not aced
                        empty = false;
                        break;
                    }
                    else {
                        empty = true;
                    }
                }
                if (playDeckIndex == 98) { //Check if play deck is 'Empty' 
                    if (empty == true) {
                        finished = true;
                        printf("\n\nCongratulations! You have aced the game!\n\n");
                    }
                    else {
                        printf("\nPlay deck is empty!\n");
                    }
                }

            }

        }

        int unplayableCards = 0;
        //Turn finishes here

        for (int index = 0; index < HAND_SIZE; index++) {
            if (hand[index] < piles[0] && hand[index] < piles[1]) { //Is card greater than both ascending piles
                if (hand[index] != piles[0] - 10 && hand[index] != piles[1] - 10 && hand[index] != piles[2] - 10 && hand[index] != piles[3] - 10) { //Is the card unable to be played with the -10 rule
                    unplayableCards++;
                }
            }
            if (hand[index] > piles[2] && hand[index] > piles[3]) {
                if (hand[index] != piles[2] + 10 && hand[index] != piles[3] + 10 && hand[index] != piles[1] + 10 && hand[index] != piles[2] + 10) {
                    unplayableCards++;
                }
            }
        }

        if (unplayableCards == HAND_SIZE * 2) {
            printf("\nAll cards are unplayable. Game is finished\n");
            finished = true;
        }
        //Read an input from key for menu selection
        int option = getc(stdin);

        switch (option)
        {
        case 'q':
        case 'Q':
            finished = true;
            break;
        default:
            break;
        }
    }

    playerScore = (98 - playDeckIndex) + HAND_SIZE;

    std::cout << "Final score: " << playerScore;
}
