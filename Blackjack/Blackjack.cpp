#include <iostream> // for std::cout and std::cin
#include <iomanip> // for std::setw()
#include <ctime> // for time()
#include <Windows.h> // for Sleep()
#include "Blackjack.h"

void DisplayIntroduction()
{
	std::cout << "--------------------" << std::endl;
	std::cout << "Welcome to Blackjack" << std::endl;
	std::cout << "--------------------\n" << std::endl;

	std::cout << "Blackjack is a popular American casino game, now found throughout the world. It is a banking game in\n" <<
				 "which the aim of the player is to achieve a hand whose points total nearer to 21 than the banker's hand, but\n" <<
				 "without exceeding 21.\n" << std::endl;

	std::cout << "The aim of the game is to accumulate a higher point total than the dealer, but without going over 21. You\n" <<
				 "compute your score by adding the values of your individual cards.\n" << std::endl;

	std::cout << "The cards 2 through 10 have their face value, J, Q, and K are worth 10 points each, and the Ace is worth\n" <<
				 "either 1 or 11 points (player's choice).\n" << std::endl;

	system("pause");

	system("cls");
}

void DisplayMenu()
{
	int choice{ -1 };
	char input[80]{ 0 };
	bool debug_game{ false };
	bool exit_flag{ true };

	do
	{
		std::cout << "---------" << std::endl;
		std::cout << "Blackjack" << std::endl;
		std::cout << "---------\n" << std::endl;
		std::cout << "1.Play\n2.Instructions\n3.Debug\n4.Quit\n" << std::endl;

		std::cout << "Choose one of the menu options:> ";
		std::cin.getline(&input[0], 80, '\n');

		choice = ValidateUserInput(&input[0], 1);

		switch (choice)
		{
		case 1:
			play(debug_game);
			break;
		case 2:
			instructions();
			break;
		case 3:
			debug_game = debug();
			break;
		case 4:
			quit();
			exit_flag = false;
			break;
		default:
			if (choice > 4 || choice == 0)
			{
				system("cls");
				std::cout << "ERROR:INCORRECT RANGE; MUST BE BETWEEN 1 AND 4\n" << std::endl;
				system("pause");
				system("cls");
			}
			break;
		}

		choice = -1;

	} while (exit_flag);
}

int ValidateUserInput(char _input[], int _ID)
{
	int choice{ -1 };
	int len{ 0 };
		
	len = strlen(&_input[0]); // check the player input length

	if (1 == len)
	{
		switch (_ID) // based on which user input needs to be validated, check the value id
		{
		case 1: // in the case of 1, validate the user's numeric input

			if (isdigit(_input[0]))
			{
				choice = atoi(&_input[0]); // store the decimal if the player's input is numeric
			}
			else
			{
				system("cls");
				std::cout << "ERROR:INCORRECT CHARACTER; MUST BE NUMERIC\n" << std::endl;
				system("pause");
			}

			break;

		case 2: // in the case of 2, validate the user's alphabetic input

			if (isalpha(_input[0]))
			{
				if (_input[0] != 'Y' && _input[0] != 'y' && _input[0] != 'N' && _input[0] != 'n')
				{
					system("cls");
					std::cout << "ERROR:INCORRECT VALUE; MUST BE EITHER Y/y OR N/n\n" << std::endl;
					system("pause");
				}
				else
				{
					choice = (int)_input[0]; // store the decimal value of the character
				}
			}
			else
			{
				system("cls");
				std::cout << "ERROR:INCORRECT VALUE; MUST BE ALPHABETIC\n" << std::endl;
				system("pause");
			}

			break;
		}
	}
	else
	{
		system("cls");
		std::cout << "ERROR:INCORRECT LENGTH\n" << std::endl;
		system("pause");
	}

	system("cls");

	return choice;
}

void play(bool& _debug_game)
{
	Card deck[52]; // declare an array of 52 cards in a deck

	Player player; // structure vairable representing the player
	Player dealer; // structure variable representing the dealer

	bool exit_flag_1{ true }; // declare the LCV for the overall game loop

	player.pot = 100; // start the game with a pot of 100 for the player

	InitialiseDeck(&deck[0]); // create the deck

	while (exit_flag_1)
	{
		bool exit_flag_2{ true }; // declare the LCV for the nested loop

		ShuffleDeck(&deck[0]);

		RecieveHand(&deck[0], player, dealer);

		BetHand(player);

		do
		{
			DisplayGameInterface(player, dealer, _debug_game);

			if (player.size < (sizeof(player.hand) / sizeof(Card)))
			{
				AddCardToPlayer(deck, player, exit_flag_2);
			}
			else
			{
				std::cout << "No more cards can be added\n" << std::endl;
				exit_flag_2 = false;

				system("pause");
				system("cls");
			}

		} while (exit_flag_2); // repeat showing the currect game state if the player's input in incorrect

		exit_flag_2 = true; // reuse for the second nested loop

		do
		{
			DisplayGameInterface(player, dealer, true);

			Sleep(500);

			TrackDealerHits(&deck[0], player, dealer, exit_flag_2);

			system("cls");

		} while (exit_flag_2);

		exit_flag_2 = true;

		DisplayGameInterface(player, dealer, true);

		CompareHands(player, dealer);

		do
		{
			DisplayGameInterface(player, dealer, true);

			ContinueGame(player, dealer, exit_flag_1, exit_flag_2);

		} while (exit_flag_2); // repeat showing the currect game state if the player's input in incorrect
	}
}

void instructions()
{
	std::cout << "------------------------" << std::endl;
	std::cout << "The deal and \"blackjack\"" << std::endl;
	std::cout << "------------------------\n" << std::endl;

	std::cout << "At the start of every blackjack game, the players must first enter their bets. Then the dealer deals two cards\n" <<
				 "to each player, including himself. The players' cards are normally dealt face up, while the dealer has one face\n" <<
				 "down (called the \"hole card\") and one face up.\n" << std::endl;

	std::cout << "The best possible blackjack hand is an opening deal of an ace with any ten-point card. This is called a\n" <<
				 "\"blackjack\", or a natural 21, and the player holding this automatically wins unless the dealer also has a\n" <<
				 "blackjack. If the player and the dealer each have a blackjack, the result is a draw (push) for that player. If the\n" <<
				 "dealer has a blackjack and the player is NOT holding a blackjack, you lose your bet.\n" << std::endl;

	std::cout << "-----------------" << std::endl;
	std::cout << "The player's turn" << std::endl;
	std::cout << "-----------------\n" << std::endl;

	std::cout << "After the cards have been dealt, the game goes on with the players taking action first. Players can either keep their hand as it is (stand),\n" <<
				 "or add more cards into their hand from the deck (hit). Until the player decides that their hand is strong enough to up against the dealer's\n" <<
				 "hand (or until they go over 21, in which case the player immediately looses (busts)). In the end, players can either go another round (where\n" <<
				 "the pot they either lost or won remains the same), or they can quit and return to the main menu (where all the progress is lost and reset\n" <<
				 "when they enter the play again)\n" << std::endl;

	std::cout << "-----------------" << std::endl;
	std::cout << "The dealer's turn" << std::endl;
	std::cout << "-----------------\n" << std::endl;

	std::cout << "When players finished their actions, either decided to stand or busted, the dealer turns over their hidden hole card. If the dealer\n" <<
				 "has a natural 21 (blackjack) with their two cards, the player automatically looses, unless they also have a blackjack, in which\n" <<
				 "case it is a draw (push) - the bet is returned to the player.\n" << std::endl;

	std::cout << "--------" << std::endl;
	std::cout << "Showdown" << std::endl;
	std::cout << "--------\n" << std::endl;

	std::cout << "The player wins if their total card value is greater than the dealer's, has 21, or the dealer goes bust, in which case the player gets\n" <<
				 "double the amount they betted. The player draws with the dealer, if they have the same value as the dealer (blackjack or normal values),\n" <<
				 "they get their pot returned. Overwise, the player looses their pot, if they score lower than the dealer, the dealer has 21, or they go bust\n" << std::endl;

	system("pause");

	system("cls");
}

bool debug()
{
	char input[80];
	int choice{ -1 };
	bool debug_game{ false };
	bool exit_flag{ true };

	do
	{
		std::cout << "Set the debug option to true?\n" << std::endl;
		std::cout << "Enter 0 (false) or 1 (true)>: ";
		std::cin.getline(&input[0], 80, '\n');

		choice = ValidateUserInput(input, 1);

		switch (choice)
		{
		case 0:
			debug_game = false; // set the debug option to false
			exit_flag = false; // exit the outer loop
			break;
		case 1:
			debug_game = true; // set the debug option to true
			exit_flag = false; // exit the outer loop
			break;
		default:
			if (choice > 1)
			{
				system("cls");
				std::cout << "ERROR:INCORRECT RANGE; MUST BE EITHER 0 OR 1\n" << std::endl;
				system("pause");
				system("cls");
			}
			break;
		}

		choice = -1;

	} while (exit_flag);

	return debug_game;
}

void quit()
{
	system("cls");
	std::cout << "Goodbye\n" << std::endl;
	system("pause");
}

void InitialiseDeck(Card _deck[])
{
	int tracker{ 2 }; // keep track of every iteration of cards in the deck - card value

	for (int i = 0; i < 52; i++) // initialise values
	{
		if (tracker < 10)
		{
			_deck[i].value = tracker;
			tracker++;
		}
		else
		{
			if (tracker < 14)
			{
				_deck[i].value = 10;
				tracker++;
			}
			else
			{
				_deck[i].value = 11;
				tracker = 2;
			}
		}
	}

	for (int i = 0; i < 52; i++) // initialise suits
	{
		if (i < 13)
		{
			strcpy_s(_deck[i].suit, "Hearts");
		}
		else if (i < 26)
		{
			strcpy_s(_deck[i].suit, "Diamonds");
		}
		else if (i < 39)
		{
			strcpy_s(_deck[i].suit, "Clubs");
		}
		else if (i < 52)
		{
			strcpy_s(_deck[i].suit, "Spades");
		}
	}

	tracker = 0; // keep track of every iteration of cards in the deck - card name

	for (int i = 0; i < 52; i++, tracker++) // initialise names
	{
		switch (tracker)
		{
		case 0:
			strcpy_s(_deck[i].name, "Two");
			break;
		case 1:
			strcpy_s(_deck[i].name, "Three");
			break;
		case 2:
			strcpy_s(_deck[i].name, "Four");
			break;
		case 3:
			strcpy_s(_deck[i].name, "Five");
			break;
		case 4:
			strcpy_s(_deck[i].name, "Six");
			break;
		case 5:
			strcpy_s(_deck[i].name, "Seven");
			break;
		case 6:
			strcpy_s(_deck[i].name, "Eight");
			break;
		case 7:
			strcpy_s(_deck[i].name, "Nine");
			break;
		case 8:
			strcpy_s(_deck[i].name, "Ten");
			break;
		case 9: 
			strcpy_s(_deck[i].name, "Jack");
			break;
		case 10:
			strcpy_s(_deck[i].name, "Queen");
			break;
		case 11:
			strcpy_s(_deck[i].name, "King");
			break;
		case 12:
			strcpy_s(_deck[i].name, "Ace");
			tracker = -1;
			break;
		}
	}
}

void ShuffleDeck(Card _deck[])
{
	int shuffle{ 0 };

	shuffle = rand() % 10 + 1; // shuffle the deck a couple of times based on a random value between 1 and 10

	for (int i = 0; i < shuffle; i++)
	{
		for (int j = 0; j < 52; j++)
		{
			Card temp_card; // temporally store the values of card i

			temp_card = _deck[j];

			int rnd = rand() % 52; // generate a random number from 0 to 51 to choose a random card in the deck

			while (rnd == j)
			{
				rnd = rand() % 52; // repeat again if rnd generates the same value as j
			}

			// overwrite the original values with the randomly selected component in the array
			_deck[j] = _deck[rnd];

			// overwrite the original randomly selected values with the selected component stored in temp_card
			_deck[rnd] = temp_card;
		}
	}
}

void RecieveHand(Card _deck[], Player& _player, Player& _dealer)
{
	for (int i = 0; i < 2; i++)
	{
		// give the player the first two cards in the deck (0, 1)
		_player.hand[i] = _deck[i];

		// give the dealer the third and the fourth (2, 3)
		_dealer.hand[i] = _deck[i + 2];
	}

	_player.size = 2;
	_dealer.size = 2;

	for (int i = 0; i < 2; i++)
	{
		// calculate the player's and the dealer's hand values
		_player.hand_value += _player.hand[i].value;
		_dealer.hand_value += _dealer.hand[i].value;
	}
}

void BetHand(Player& _player)
{
	int bet{ 0 };

	do
	{
		std::cout << "Your pot: " << _player.pot << std::endl;

		std::cout << "\nEnter your bet:> ";
		std::cin >> bet;

		system("cls");

		if (0 == bet)
		{
			std::cout << "ERROR:YOU MUST ENTER A BET\n" << std::endl;
			system("pause");
			system("cls");
		}
		else if (bet < 0)
		{
			std::cout << "ERROR:YOU CAN'T BET LESS THAN 0\n" << std::endl;
			system("pause");
			system("cls");
		}
		else if (bet > _player.pot)
		{
			std::cout << "ERROR:YOU CAN'T BET MORE THAN YOUR TOTAL POT\n" << std::endl;
			system("pause");
			system("cls");
		}

	} while (0 == bet || bet < 0 || bet > _player.pot);

	_player.bet = bet;
	_player.pot -= _player.bet;
}

void DisplayGameInterface(Player& _player, Player& _dealer, bool _debug_game)
{
	std::cout << "-----------" << std::endl;
	std::cout << "Game begins" << std::endl;
	std::cout << "-----------\n" << std::endl;

	std::cout << "--------------------------" << std::endl;

	std::cout << "Dealer's hand:" << std::endl;

	for (int i = 0; i < _dealer.size; i++)
	{
		if (1 == i) // display hole card
		{
			if (_debug_game) // show the dealer's 'hole card' if the user set the debug to true
			{
				std::cout << "\nCard 2: Name: " << _dealer.hand[1].name << std::endl;
				std::cout << std::setw(14) << "Suit: " << _dealer.hand[1].suit << std::endl;
				std::cout << std::setw(15) << "Value: " << _dealer.hand[1].value << std::endl;
			}
			else // overwise do not show the dealer's 'hole card'
			{
				std::cout << "\nCard 2: Name:???" << std::endl;
				std::cout << std::setw(16) << "Suit:???" << std::endl;
				std::cout << std::setw(17) << "Value:???" << std::endl;
			}
		}
		else // display the first and other drawn cards
		{
			std::cout << "\nCard " << i + 1 << ": Name: " << _dealer.hand[i].name << std::endl;
			std::cout << std::setw(14) << "Suit: " << _dealer.hand[i].suit << std::endl;
			std::cout << std::setw(15) << "Value: " << _dealer.hand[i].value << std::endl;
		}
	}

	if (_debug_game)
	{
		std::cout << "\nTotal cards value: " << _dealer.hand_value << std::endl;
	}
	else
	{
		std::cout << "\nTotal cards value:???" << std::endl;
	}

	std::cout << "--------------------------" << std::endl;

	std::cout << "Your hand:\n" << std::endl;

	for (int i = 0; i < _player.size; i++)
	{
		std::cout << "Card " << i + 1 << ": Name: " << _player.hand[i].name << std::endl;
		std::cout << std::setw(14) << "Suit: " << _player.hand[i].suit << std::endl;
		std::cout << std::setw(15) << "Value: " << _player.hand[i].value << '\n' << std::endl;
	}

	std::cout << "Pot: " << _player.pot << std::endl;
	std::cout << "Bet: " << _player.bet << std::endl;

	std::cout << "\nTotal cards value: " << _player.hand_value << std::endl;

	std::cout << "--------------------------\n" << std::endl;
}

void AddCardToPlayer(Card _deck[], Player& _player, bool& _exit_flag)
{
	char choice{ -1 };
	char input[80]{ 0 };

	std::cin.ignore(500, '\n');
	
	std::cout << "Would you like to add another card?" << std::endl;
	std::cout << "Enter (Y/y)es or (N/n)o:> ";

	std::cin.getline(&input[0], 80, '\n');

	choice = (char)ValidateUserInput(&input[0], 2);

	system("cls");

	switch (choice)
	{
	case 'Y':
	case 'y':

		_player.hand[_player.size] = _deck[_player.size + 2];

		_player.hand_value += _player.hand[_player.size].value;

		_player.size++; // initialise a new size for the player's hand

		break;
	case 'N':
	case 'n':
		_exit_flag = false; // exit the outer loop with no addition to hand
		break;
	}
}

void TrackDealerHits(Card _deck[], Player& _player, Player& _dealer, bool& _exit_flag)
{
	if (_dealer.hand_value >= 17) // stop adding cards once the dealer's hand value is 17 or higher
	{
		if (_dealer.hand_value == 21) // the dealer will not bother adding more cards to their hand
		{							  // if the player's hand value is equal to 21
			_exit_flag = false;
		}
		else
		{
			if (_player.hand_value >= 21) // the dealer will not bother adding more cards to their hand
			{							  // if the player's hand value is greater than 21
				_exit_flag = false;
			}
			else
			{
				if (_dealer.hand_value < _player.hand_value) // the dealer will attempt to beat or draw with the player
				{
					AddCardToDealer(&_deck[0], _player, _dealer);
				}
				else // the dealer will stop adding more cards to their hand if
				{    // their hand value is greater than or equal to the player's

					_exit_flag = false;
				}
			}
		}
	}
	else // overwise they'll keep adding more cards into their hand until
	{    // they meet the condition of having a hand value of 17 or higher

		AddCardToDealer(&_deck[0], _player, _dealer);
	}
}

void AddCardToDealer(Card _deck[], Player& _player, Player& _dealer)
{
	_dealer.hand[_dealer.size] = _deck[_dealer.size + _player.size]; // add the cards after the ones the player the player received

	_dealer.hand_value += _dealer.hand[_dealer.size].value;

	_dealer.size++;
}

void CompareHands(Player& _player, Player& _dealer)
{
	if (_player.hand_value < 21)
	{
		if (_dealer.hand_value < 21)
		{
			if (_player.hand_value > _dealer.hand_value)
			{
				std::cout << "YOU WIN!\n" << std::endl;
				_player.bet *= 2;
				_player.pot += _player.bet;
				_player.bet = 0;
			}
			else if (_player.hand_value == _dealer.hand_value)
			{
				std::cout << "DRAW!\n" << std::endl;
				_player.pot += _player.bet;
				_player.bet = 0;
			}
			else
			{
				std::cout << "YOU LOOSE!\n" << std::endl;
				_player.bet = 0;
			}
		}
		else if (_dealer.hand_value > 21)
		{
			std::cout << "YOU WIN!\n" << std::endl;
			_player.bet *= 2;
			_player.pot += _player.bet;
			_player.bet = 0;
		}
		else
		{
			std::cout << "YOU LOOSE!\n" << std::endl;
			_player.bet = 0;
		}
	}
	else if (_player.hand_value > 21)
	{
		std::cout << "YOU LOOSE!\n" << std::endl;
		_player.bet = 0;
	}
	else
	{
		if (_dealer.hand_value == 21)
		{
			std::cout << "DRAW!\n" << std::endl;
			_player.pot += _player.bet;
			_player.bet = 0;
		}
		else
		{
			std::cout << "YOU WIN!\n" << std::endl;
			_player.bet *= 2;
			_player.pot += _player.bet;
			_player.bet = 0;
		}
	}

	system("pause");
	system("cls");
}

void ContinueGame(Player& _player, Player& _dealer, bool& _exit_flag_1, bool& _exit_flag_2)
{
	if (0 == _player.pot)
	{
		_exit_flag_2 = false; // exit the outer loop
		_exit_flag_1 = false; // exit the game loop

		std::cout << "You have no pot to bet.\n" << std::endl;
		std::cout << "GAME OVER!\n" << std::endl;
		
		system("pause");
		system("cls");
	}
	else
	{
		char choice{ -1 };
		char input[80]{ 0 };

		std::cout << "Do you want to go another round?" << std::endl;
		std::cout << "Enter (Y/y)es or (N/n)o:> ";
		std::cin.getline(&input[0], 80, '\n');

		choice = (char)ValidateUserInput(&input[0], 2);

		switch (choice)
		{
		case 'N':
		case 'n':
			_exit_flag_2 = false; // exit the outer loop
			_exit_flag_1 = false; // exit the game loop
			break;
		case 'Y':
		case 'y':

			for (int i = 0; i < 11; i++)
			{
				memset(_player.hand[i].name, 0, 10);
				memset(_player.hand[i].suit, 0, 10);
				_player.hand[i].value = 0;
				
				memset(_dealer.hand[i].name, 0, 10);
				memset(_dealer.hand[i].suit, 0, 10);
				_dealer.hand[i].value = 0;
			}

			_player.size = 0; // reset the player's hand total
			_dealer.size = 0; // reset the dealer's hand total

			_player.hand_value = 0; // reset the player's hand value

			_dealer.hand_value = 0; // reset the dealer's hand value

			_exit_flag_2 = false; // exit the outer loop

			break;
		}

		system("cls");
	}
}