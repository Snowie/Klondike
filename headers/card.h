#ifndef CARD_H
#define CARD_H
#include <cmath>
#include <string>
#include <sstream>

enum class Suit
{
	HEARTS = 1, SPADES = 2, CLUBS = 3, DIAMONDS = 4
};

Suit intToSuit(int);

std::string suitToString(Suit);

class Card
{
	public:
		Card(Suit, int);

		Suit getSuit() const;

		bool getFaceUp() const;

		int getHash() const;

		int getNumber() const;

		//Return a textual representation of our card
		std::string toString() const;

		//Checks if two cards are equivalent
		bool operator== (const Card &) const;

		void setFaceUp(bool);

	private:
		Suit suit;
		int number;
		bool faceUp;
};

//Struct to be used for a hash functor in unordered_set
struct Hash 
{
	size_t operator()(const Card * c) const 
	{
		return c->getHash();
	}
};

//Struct to be used for an equality functor in unordered_set
struct isEq {
    bool operator() ( Card * lhs, Card * rhs ) const
    {
    	return (*lhs) == (*rhs);
    }
};

#endif // CARD_H