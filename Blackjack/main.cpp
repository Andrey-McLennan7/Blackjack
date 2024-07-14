#include <iostream> // for std::cout and std::cin
#include <ctime> // for time()
#include "Blackjack.h"

int main()
{
	srand(time(0));

	DisplayIntroduction();

	DisplayMenu();

	return 0;
}