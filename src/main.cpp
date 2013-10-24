#include <iostream>

//For generateDeck()
#include <random>
#include <unordered_set>

//For standard containers used through the program.
#include <stack>
#include <vector>
#include <iterator>

#include "../headers/card.h"

using std::vector;
using std::cout;
using std::endl;

//An optimized function to generate cards, O(n)
std::stack<Card*> generateDeck()
{
	//Facilitate an O(1) lookup while generating
	std::unordered_set<Card*, Hash, isEq> hashOfCards;
	std::stack<Card*> deck;

	hashOfCards.max_load_factor(.5);

	// Seed with a real random value, if available
	std::random_device rd;
 
	std::mt19937 rng(rd());

	std::uniform_int_distribution<int> face(1, 13);
	std::uniform_real_distribution<double> suit(0.0, 39.0);

	for(int i = 0; i < 52; ++i)
	{
		Suit s;
		int num;

		//GARBAGE VALUE
		Card tempCard(Suit::DIAMONDS, 0);

		do
		{
			num = face(rng);
			s = intToSuit(int( ceil( suit(rng)/10.0 ) ) );

			//Construct a card to test against our hash set
			tempCard = Card(s, num);
		}while(hashOfCards.find(&tempCard) != hashOfCards.end());

		//Keep track of the cards we have made
		hashOfCards.insert(new Card(s, num));
		//If the card isn't in our deck, insert!
		deck.push(new Card(s, num));
	}

	//Clean up!
	for(Card * c: hashOfCards)
		delete c;

	return deck;
}

bool isLegalMove(vector<Card*>::iterator toGrab, vector<Card*> destination, bool toFound)
{
	if(toFound)
	{
		//There is already a card on the foundation
		if(destination.size() != 0)
		{
			//The source card is not one greater than the top destination card.
			if((*toGrab)->getNumber() != destination.back()->getNumber() + 1)
				return false;
		}
		else
		{
			//We can only move an ace to an empty tableau
			if((*toGrab)->getNumber() != 1)
				return false;
		}
	}
	else
	{
		if(destination.size() != 0)
		{
			//The source card isn't one less than the top destion card.
			if((*toGrab)->getNumber() + 1 != destination.back()->getNumber())
				return false;

			//They aren't the same suit!
			if((*toGrab)->getSuit() != destination.back()->getSuit())
				return false;
		}
		else
		{
			//We can only move a king to an empty tableau
			if( (*toGrab)->getNumber() != 13 )
				return false;
		}
	}

	return true;
}

void mover (vector<Card*> * source, vector<Card*> * destination, vector<Card*>::iterator * toGrab, bool toFound)
{
	//If the move we are attemtping to perform is allowed by the rules...
	if(isLegalMove(*toGrab, *destination, toFound))
	{
		//Add the source cards starting from toGrab to the destination vector
  		for(vector<Card*>::iterator i = (*toGrab); i != source->end(); i = std::next(i))
  			destination->push_back(*i);

  		//Take the cards out of the source
  		while((*toGrab) != source->end())
  			source->pop_back();

  		//Make sure the card is visible.
  		if(source->size() != 0)
  			source->back()->setFaceUp(true);
	}
	else
		cout << "Illegal Move" << endl;
}

void printField(vector<vector<Card*>> foundations, vector<vector<Card*>> tableau, vector <Card*> discard)
{
	for(int i = 0; i < 4; ++i)
	{
		if(foundations[i].size() != 0)
		{
			cout << "Foundation " << i + 1 << " has: " 
						<< foundations[i].back()->toString() 
						<< " as its top card." << endl;
		}
		else
			cout <<"No cards foundation " << i + 1 << "." << endl;
	}
	
	cout << "\n\n" << endl;

	for(int i = 0; i < 7; ++i)
	{
		cout << tableau[i].size() << " cards in tableau " << i + 1 << endl;
		
		//Print the column of cards top to bottom
		if(tableau[i].size() != 0)
			for(Card * c: tableau[i])
				cout << c->toString() << endl;
		
		cout << "\n" << endl;
	}

	cout << "\n\n" << endl;

	if(discard.size() != 0)
		cout << "The top card of the discard pile is: " << discard.back()->toString() << endl;
	else
		cout << "Discard is empty." << endl;
}

void drawFromDeck(std::stack<Card*> *deck, vector <Card*> *discard)
{
	if(!deck->empty())
	{
		discard->push_back(deck->top());
		discard->back()->setFaceUp(true);
		deck->pop();
	}
	else
		cout << "The deck is empty." endl;
}

void setupField(vector<vector<Card*>> * foundations, vector<vector<Card*>> * tableau, std::stack<Card*> * deck)
{
	tableau->resize(7);
	foundations->resize(4);

	//Put the cards on the proper foundations
	for(int i = 0; i < 7; ++i)
	{
		for(unsigned int j = 0; j < 7; ++j)
		{
			if((*tableau)[j].size() <= j)
			{
				//It's an ace, put it on a foundation
				if(deck->top()->getNumber() == 1)
				{
					//Make sure it goes in an empty slot
					for(int i = 0; i < 4; ++i)
					{
						if((*foundations)[i].size() == 0)
						{
							(*foundations)[i].push_back(deck->top());
							(*foundations)[i].back()->setFaceUp(true);
							break;
						}
					}
				}
				//Otherwise, continue putting it on the tableau
				else
				{
					(*tableau)[j].push_back(deck->top());
				}
				//Take the card out of the deck
				deck->pop();
			}
		}
	}

	//Make the cards at the bottom of the tableau visible.
	for(int i = 0; i < 7; ++i)
		(*tableau)[i].back()->setFaceUp(true);
}

int main()
{
	//Our "deck" of cards
	std::stack<Card*> deck = generateDeck();
	
	vector<Card*> discard;

	//The four slots for aces
	vector<vector<Card*>> foundations;
	//The seven columns of cards
	vector<vector<Card*>> tableau;

	setupField(&foundations, &tableau, &deck);

	while(true)
	{
		printField(foundations, tableau, discard);

		std::string action;
		cout << "What to do? (draw, move, quit)" << endl;
		std::cin >> action;

		if(action.compare("quit") == 0 || action.compare("Quit") == 0)
			break;

		if(action.compare("draw") == 0 || action.compare("Draw") == 0)
		{
			drawFromDeck(&deck, &discard);
			printField(foundations, tableau, discard);
			continue;
		}

		std::string src;
		unsigned int sourceIndex;
		cout << "Where to take from (discard[0, 0] or tableau [1, 7])" << endl;
		std::cin >> src;
		std::cin >> sourceIndex;

		//Keep zero indexed.
		--sourceIndex;

		std::string dst;
		unsigned int destinationIndex;
		cout << "Where to move to (foundation[1, 4] or tableau[1, 7])?" << endl;
		std::cin >> dst;
		std::cin >> destinationIndex;

		//Keep zero indexed.
		--destinationIndex;

		//We are taking from a tableau
		if(src.compare("tableau") == 0 || src.compare("Tableau") == 0)
		{
			//How many cards does the user wish to move?
			int numFromBack;
			cout << "How many cards from the bottom should attempt to be grabbed?" << endl;
			std::cin >> numFromBack;

			//Check for sane input.
			if(numFromBack < 0)
			{
				cout << "Please enter a number greater than or equal to 0." << endl;
				continue;
			}
			
			//Get an iterator to the back of the tableau of the user's choosing.
			vector<Card*>::iterator toGrab = std::prev(tableau[sourceIndex].end());
			
			bool possible = true;

			//Move the iterator to the user designated card
			for(int i = 0; i < numFromBack; ++i)
			{
				//The iterator has reached the beginning of the array, it can go no further.
				if(std::distance(toGrab, tableau[sourceIndex].begin()) == 0)
				{
					possible = false;
					break;
				}

				//The card is face down, we can't point the iterator here.
				if((*toGrab)->getFaceUp() == false)
				{
					possible = false;
					break;
				}

				//Move the iterator back one.
				toGrab = std::prev(toGrab);
			}
			if(!possible)
			{
				cout << "You can't grab from there!" << endl;
				continue;
			}

			//Moving to a foundation.
			if(dst.compare("foundation") == 0 || dst.compare("Foundation") == 0)
				mover(&(tableau[sourceIndex]), &(foundations[destinationIndex]), &toGrab, true);
			
			//Moving to the tableau.
			if(dst.compare("tableau") == 0 || dst.compare("Tableau") == 0)
				mover(&(tableau[sourceIndex]), &(tableau[destinationIndex]), &toGrab, false);
		}
		else if(src.compare("discard") == 0 || src.compare("Discard") == 0)
		{
			//You can only ever use the "top" card of the discard.
			vector<Card*>::iterator toGrab = std::prev(discard.end());
			
			//We are moving to a foundation
			if(dst.compare("foundation") == 0 || dst.compare("Foundation") == 0)
				mover(&(discard), &(foundations[destinationIndex]), &toGrab, true);
			
			//We are moving to the tableau
			if(dst.compare("tableau") == 0 || dst.compare("Tableau") == 0)
				mover(&(discard), &(tableau[destinationIndex]), &toGrab, false);
		}
	}

	//Clean up the deck
	while(!deck.empty())
	{
		delete deck.top();
		deck.top() = nullptr;
		deck.pop();
	}

	//Clean up the discard
	while(!discard.empty())
	{
		delete discard.back();
		discard.back() = nullptr;
		discard.pop_back();
	}

	//Clean up the tableau
	for(unsigned int i = 0; i < tableau.size(); ++i)
	{
		//Clear the ith tableau
		while(!tableau[i].empty())
		{
			delete tableau[i].back();
			tableau[i].back() = nullptr;
			tableau[i].pop_back();
		}
	}

	//Clean up the foundations
	for(unsigned int i = 0; i < foundations.size(); ++i)
	{
		while(!foundations[i].empty())
		{
			delete foundations[i].back();
			foundations[i].back() = nullptr;
			foundations[i].pop_back();
		}
	}

	return 0;
}