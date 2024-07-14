#pragma once

#ifndef _BLACKJACK_H_
#define _BLACKJACK_H_

struct Card
{
	char name[10]{ 0 };
	char suit[10]{ 0 };
	int value{ 0 };
};

struct Player
{
	Card hand[11];
	int size = 0;
	int pot{ 0 };
	int bet{ 0 };
	int hand_value{ 0 };
};

void DisplayIntroduction();
// Output welcome text on the screen

void DisplayMenu();
// Output menu options for the player to select

int ValidateUserInput(char _input[], int _ID);
// Pre: user must input any value (valid or erroneous)
//		the ID must be either 1 for numeric validation or
//		2 for character validation
// Post: the function will return the correct data to the caller
//		 overwise, it will display an error message

void play(bool& _debug_game);
// Post: the function will run a series of functions before displaying
//		 the interactive interface.

void instructions();
// Output game rules and instrcutions text on the screen

bool debug();
// Output debug option text on the screen

void quit();
// Output a parting message on the screen

void InitialiseDeck(Card _deck[]);
// Pre: declare an array of sctructured variables of type Card
// Post: initialise every card in the deck (including their members)

void ShuffleDeck(Card _deck[]);
// Pre: each structured element within an array is initialised (including their members)
// Post: each structured element within an array will be switched at random locations
//		 within the same array

void RecieveHand(Card _deck[], Player& _player, Player& _dealer);
// Pre: each structured element within an array is randomised
//		varibales of type Player must be declared for the player and the dealer
// Post: the player and the dealer will be assigned with random cards from the deck

void BetHand(Player& _player);
// Pre: the player will have an initialised pot
// Post: the function will return a the amount the player bets

void DisplayGameInterface(Player& _player, Player& _dealer, bool _debug_game);
// Pre: the player and the dealer will have cards within their hands
//		debug must be set to true for the dealer's hole card to be displayed
//		even at the start of the game
// Post: output the formatted interface using text and game view data

void AddCardToPlayer(Card _deck[], Player& _player, bool& _exit_flag);
// Pre: the player will have cards within their hand
// Post: the player will have more cards in their hand

void TrackDealerHits(Card _deck[], Player& _player, Player& _dealer, bool& _exit_flag);
// Post: stop the dealer from drawing more cards when their hand value is
//		 greater than or equal to 17

void AddCardToDealer(Card _deck[], Player& _player, Player& _dealer);
// Pre: the dealer will have cards within their hand
// Post: the dealer will have more cards in their hand

void CompareHands(Player& _player, Player& _dealer);
// Pre: the player and the dealer will have two cards within their hands
// Post: the player either wins, loses, or ends in a draw depending on
//		 the player's and dealer's hands

void ContinueGame(Player& _player, Player& _dealer, bool& _exit_flag_1, bool& _exit_flag_2);
// Post: if the player selects yes for another play, then all the referenced parameters
//		 and the structured variables' member fields will be reset (apart from the player's
//		 pot value and the first exit flag)
//
//		 overwise, the exit flag that controls the game's overall loop will be set to exit
//		 the loop and the player will retrun to the game menu (all local variables will be deleted)

#endif // !_BLACKJACK_H_