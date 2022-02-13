/*

BlackJack.cpp
Made By Will Hawthorne
2/7/2022

A function that allows the user to play BlackJack with the function acting as the dealer
*/

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>


using namespace std;

//Outputs a string of what the card inputed is
string dealCard(int cardNumber, bool *shoe) {
	cardNumber = cardNumber % 52;
	int suitNumber = floor(cardNumber / 13);
	int cardValue = cardNumber % 13;
	string suit;
	switch (suitNumber) {
	case 0 :
		suit = "Clubs";
		break;
	case 1 :
		suit = "Diamonds";
		break;
	case 2 : 
		suit = "Hearts";
		break;
	case 3 :
		suit = "Spades";
		break;
	}
	switch (cardValue) {
	case 10 :
		return "Jack of " + suit;
		break;
	case 0 :
		return "Ace of " + suit;
		break;
	case 11 :
		return "Queen of " + suit;
		break;
	case 12 :
		return "King of " + suit;
		break;
	default :
		return std::to_string(cardValue + 1) + " of " + suit;
		break;
	}
}

//evaluates the card and adds it to the hand. Alters ace value if needed
int cardEval(int cardNumber, int handVal, int &ace) {
	cardNumber = cardNumber % 13;
	int temp;
	if (cardNumber > 8) {
		temp = handVal + 10;
		if (temp > 21 && ace > 0) {
			ace--;
			return handVal;
		}
		else return temp;
	}
	else if (cardNumber > 0) {
		temp = handVal + cardNumber + 1;
		if (temp > 21 && ace > 0) {
			ace--;
			return temp - 10;
		}
		else return temp;
	}
	else if (handVal < 11) {
		return handVal + 11;
	}
	else {
		temp = handVal + 1;
		if (temp > 21 && ace > 0) {
			ace--;
			return temp - 10;
		}
		else return temp;
	}
}

//draws a random, unused card, prints the card, then returns the card value
int draw(bool *shoe, int decks) {
	int cardNumber;
	do {

		cardNumber = rand() % (52 * decks);
	} while (!shoe[cardNumber]);

	string card = dealCard(cardNumber, shoe);
	shoe[cardNumber] = false;
	/*
	std::cout << cardNumber << endl;
	for (int i = 0; i < 52; i++) {
		std::cout << shoe[i];
	}*/
	std::cout << card << endl;
	return cardNumber;
}

//if possible, it outputs a card that makes the dealer tie or beat the player
int specialDraw(bool *shoe, int playerHand, int dealerHand, int& ace) {
	int cardNumber;
	int value;
	bool cardExist;
	int handDif = playerHand - dealerHand;
	if ((handDif) > 11) {
		return draw(shoe, 1);
	}
	else {
		do {
			do {
				if (handDif == 11) {
					//std::cout << "Need Ace" << endl;
					cardNumber = (rand() % 4) * 13;
				}
				else {
					cardNumber = (rand() % (11 - handDif)) + handDif;
					//std::cout << "drawing " << cardNumber << endl;
					if (cardNumber == 10) {
						cardNumber = ((rand() % 4) + 9) + (rand() % 4) * 13;
					}
					else if (cardNumber == 11) {
						cardNumber = (rand() % 4) * 13;
					}
					else {
						cardNumber = (cardNumber - 1) + (rand() % 4) * 13;
					}
				}
				cardExist = shoe[cardNumber];

				//std::cout << "cardExist: " << cardExist << endl;
			} while (!cardExist);
			value = cardEval(cardNumber, 0, ace);
		} while (dealerHand + value > 21);// || (playerHand - dealerHand - value) > 0);
	}

	string card = dealCard(cardNumber, shoe);
	std::cout << card << endl;

	return cardNumber;

}

//Dealing function for hard mode. Guarantees that the dealer wins
int hardDeal(int winnings, bool shoe[], int decks, ostream& fout) {
	std::cout << endl;
	int dealerHand = 0;
	int playerHand = 0;
	int dealerAce = 0;
	int playerAce = 0;
	bool bj = false;
	bool dealerBJ = false;

	for (int i = 0; i < 3; i++) {

		int cardNumber = draw(shoe, decks);

		if (i == 0) {
			std::cout << "[]" << endl << endl << endl;
			if ((cardNumber % 13) == 0) dealerAce++;
			dealerHand = cardEval(cardNumber, dealerHand, dealerAce);
		}
		else {
			if ((cardNumber % 13) == 0) playerAce++;
			playerHand = cardEval(cardNumber, playerHand, playerAce);
		}

	}

	//Player draws blackjack
	if (playerHand == 21) {
		std::cout << "Congratulations, you got Blackjack! As long as I don't also have BlackJak, you win $3" << endl;
		bj = true;
	}

	//Hit or Stay Loop
	bool bust = false;
	bool stay = false;
	do {
		std::cout << "Would you like to hit or stay?" << endl;
		string response;
		cin >> response;
		if (response == "hit") {
			/*int cardNumber;
			do {

				cardNumber = rand() % sizeof(shoe);
			} while (shoe[cardNumber]);

			string card = dealCard(cardNumber, shoe);
			shoe[cardNumber] = false;
			std::cout << card << endl;*/

			int cardNumber = draw(shoe, 1);
			if ((cardNumber % 13) == 0) playerAce++;
			playerHand = cardEval(cardNumber, playerHand, playerAce);

			if (playerHand > 21) {
				bust = true;
				std::cout << "That's " << playerHand << ". You busted." << endl;
				return -2;
			}

		}
		else if (response == "stay") {
			stay = true;
		}
	} while (!bust && !stay);

	std::cout << endl << "My other card is a ";
	int j = 0;

	//Initiates specialDraw to guarantee the dealer draws the exact card that they need
	do {
		if (j > 0) std::cout << "I draw a ";

		int cardNumber = specialDraw(shoe, playerHand, dealerHand, dealerAce);

		if ((cardNumber % 13) == 0) dealerAce++;

		dealerHand = cardEval(cardNumber, dealerHand, dealerAce);

		if (j == 0 && dealerHand == 21) dealerBJ = true;

		j++;
	} while (dealerHand < playerHand);

	if (dealerBJ && bj) {
		bj = false;
		cout << "Looks like I have Blackjack, too. Sorry." << endl;
	}
	else if (dealerBJ) {
		cout << "I've got Blackjack, so I automatically win" << endl;
		fout << "Dealer Hand: " << dealerHand << endl << "Player Hand: " << playerHand << endl;
		fout << "Dealer wins with Blackjack" << endl;
		return -2;
	}

	std::cout << endl << "Dealer Hand: " << dealerHand << endl << "Player Hand: " << playerHand << endl;
	fout << "Dealer Hand: " << dealerHand << endl << "Player Hand: " << playerHand << endl;

	//results
	if (dealerHand > 21) {
		std::cout << "I busted, so you win!" << endl;
		fout << "Player Wins" << endl;
		return 2;
	}
	else if (dealerHand < playerHand) {
		std::cout << "Great Job! You Win!" << endl;
		fout << "Player Wins" << endl;
		return 2;
	}
	else if (dealerHand > playerHand) {
		std::cout << "You played well, but the House always wins" << endl;
		fout << "Dealer Wins" << endl;
		return -2;
	}
	else {
		std::cout << "We pushed, so we both keep our money." << endl;
		fout << "It's a tie" << endl;
		return 0;
	}

	return 0;
}

//Deals and executes a normal hand of blackjack
int normalDeal(int winnings, bool *shoe, int decks, ostream& fout) {
	std::cout << endl;
	int dealerHand = 0;
	int playerHand = 0;
	int dealerAce = 0;
	int playerAce = 0;
	bool bj = false;

	//Deals the first 3 cards (but not the dealer's second card)
	for (int i = 0; i < 3; i++) {

		int cardNumber = draw(shoe, decks);

		if (i == 0) {
			std::cout << "[]" << endl << endl << endl;
			if ((cardNumber % 13) == 0) dealerAce++;
			dealerHand = cardEval(cardNumber, dealerHand, dealerAce);
		}
		else {
			if ((cardNumber % 13) == 0) playerAce++;
			playerHand = cardEval(cardNumber, playerHand, playerAce);
		}
		
	}


	//Player draws blackjack
	if (playerHand == 21) {
		std::cout << "Congratulations, you got Blackjack! As long as I don't also have BlackJak, you win $3" << endl;
		bj = true;
	}

	//Hit or Stay Loop
	bool bust = false;
	bool stay = false;
	do {
		std::cout << "Would you like to hit or stay?" << endl;
		string response;
		cin >> response;
		if (response == "hit") {
			/*int cardNumber;
			do {

				cardNumber = rand() % sizeof(shoe);
			} while (shoe[cardNumber]);

			string card = dealCard(cardNumber, shoe);
			shoe[cardNumber] = false;
			std::cout << card << endl;*/

			int cardNumber = draw(shoe, decks);
			if ((cardNumber % 13) == 0) playerAce++;
			playerHand = cardEval(cardNumber, playerHand, playerAce);

			if (playerHand > 21) {
				bust = true;
				std::cout << "That's " << playerHand << ". You busted." << endl;
				return -2;
			}

		}
		else if (response == "stay") {
			stay = true;
		}
	} while (!bust && !stay);

	//Dealers second card
	/*int cardNumber;
	do {

		cardNumber = rand() % sizeof(shoe);
	} while (shoe[cardNumber]);

	string card = dealCard(cardNumber, shoe);
	shoe[cardNumber] = false;*/
	

	//Dealers Hit or Stay (soft 16)
	std::cout << endl << "My other card is a ";

	int cardNumber = draw(shoe, decks);
	if ((cardNumber % 13) == 0) dealerAce++;
	dealerHand = cardEval(cardNumber, dealerHand, dealerAce);

	if (dealerHand == 21 && bj) {
		bj = false;
		cout << "Looks like I have Blackjack, too. Sorry." << endl;
	}
	else if (dealerHand == 21) {
		cout << "I've got Blackjack, so I automatically win" << endl;
		fout << "Dealer Hand: " << dealerHand << endl << "Player Hand: " << playerHand << endl;
		fout << "Dealer wins with Blackjack" << endl;
		return -2;
	}
	else if (bj) {
		cout << "That's not BlackJack, so you win!" << endl;
		fout << "Dealer Hand: " << dealerHand << endl << "Player Hand: " << playerHand << endl;
		fout << "Player wins with Blackjack" << endl;
		return 3;
	}

	//dealer hits until they have at least 17
	while (dealerHand < 17) {
		/*int cardNumber;
		do {

			cardNumber = rand() % sizeof(shoe);
		} while (shoe[cardNumber]);

		string card = dealCard(cardNumber, shoe);
		shoe[cardNumber] = false;
		std::cout << "I draw a " << card << endl;*/

		std::cout << "I draw a ";

		int cardNumber = draw(shoe, decks);
		if ((cardNumber % 13) == 0) dealerAce++;
		dealerHand = cardEval(cardNumber, dealerHand, dealerAce);
	}

	std::cout << endl << "Dealer Hand: " << dealerHand << endl << "Player Hand: " << playerHand << endl;
	fout << "Dealer Hand: " << dealerHand << endl << "Player Hand: " << playerHand << endl;
	//results
	if (dealerHand > 21) {
		std::cout << "I busted, so you win!" << endl;
		fout << "Player Wins" << endl;
		return 2;
	}
	else if (dealerHand < playerHand) {
		std::cout << "Great Job! You Win!" << endl;
		fout << "Player Wins" << endl;
		return 2;
	}
	else if (dealerHand > playerHand) {
		std::cout << "You played well, but the House always wins" << endl;
		fout << "Dealer Wins" << endl;
		return -2;
	}
	else {
		std::cout << "We pushed, so we both keep our money." << endl;
		fout << "It's a tie" << endl;
		return 0;
	}

	return 0;
}


int main() {
	//dVector <bool> shoe; // Represents all the possible cards in the shoe
	bool shoe[520];
	int decks = 1;
	unsigned int cardsDealt = 0;
	unsigned int shuffleCard;
	bool hardMode = true;
	int winnings = 0;

	ofstream fout("log.txt", ios::out);
	fout << "New Game Start" << endl;

	string input;
	int confirm;

	srand(time(nullptr));

	//Welcomes the player, gets pregame information from them, and initiates the game
	std::cout << "Welcome to the Blackjack table. I'm your dealer, Will Hawthorne." << endl;
	

	//bool shoe[52 * decks] = { true };


	for (int i = 0; i < 52; i++) {
		shoe[i] = true;
	}

	/*do {
		std::cout << "Would you like to play on \"easy\" or \"hard\" mode?" << endl;

		getline(cin, input);
		hardMode = input == "hard";

	} while (!hardMode && input != "easy");*/

	if (hardMode) {
		std::cout << "You are playing on Hard Mode" << endl;
		fout << "Playing on Hard Mode" << endl;
		std::cout << endl << "Hard Mode reshuffles after every hand and uses an AI dealer that is able to play like a dealer with the current best strategies used by professional players. Good Luck." << endl;
	}

	//Sets up the size of the shoe and places the shuffleCard
	if (!hardMode) {
		do {
			std::cout << "How many Decks would you like me to use?" << endl;
			std::cout  << "Please choose between 1 and 10 decks:" << endl;

			//cin >> decks; Does not confirm type, user can cause an error

			getline(cin, input);
			stringstream(input) >> decks;

			std::cout << "That's " << decks << " decks. Is that correct?" << endl << "Input 1 to continue, or 0 to choose again:" << endl;

			getline(cin, input);
			stringstream(input) >> confirm;

		} while ((decks < 1 || decks > 10) && confirm == 1);

		//shoe.resize(52 * decks);

		for (int i = 52; i < 52 * decks; i++) {
			shoe[i] = true;
		}

		shuffleCard = round((double)(rand() % 5 + 85) / 100 * 52 * decks); //randomly places the shuffle card in the last 10-15% of the shoe

	}



	//Starts the game cycle and ends when the user types "quit"
	while (input != "quit") {
		std::cout << endl << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl << "You're current winnings are: " << winnings << endl;
		fout << "Winnings: " << winnings << endl;

		std::cout << "Type quit if you want to walk away from the table. Otherwise, type anything else" << endl;
		cin >> input;
		//getline(cin, input);
		//stringstream(input) >> confirm;

		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (input == "quit") break;
		
		if (hardMode) {
			if (rand() % 100 < 50) {
				winnings += normalDeal(winnings, shoe, decks, fout);
			}
			else {
				winnings += hardDeal(winnings, shoe, decks, fout);
			}
		}
		else {
			if (cardsDealt >= shuffleCard) {

				std::cout << "Reshuffling the deck(s)" << endl;
				for (int i = 0; i < 52 * decks; i++) {
					shoe[i] = true;
				}
			}
			winnings += normalDeal(winnings, shoe, decks, fout);
		}

	}
	fout << "End of Game Session" << endl;
	fout.close();

	return 0;
}
