#include "../headers/card.h"

Suit intToSuit(int x)
{
	if(x == 1)
		return Suit::HEARTS;
	
	if(x == 2)
		return Suit::SPADES;
	
	if(x == 3)
		return Suit::CLUBS;
	
	//It's nothing else, it must be Diamonds	
	return Suit::DIAMONDS;
}

std::string suitToString(Suit s)
{
	if(s == Suit::HEARTS)
		return "Hearts";
	
	if(s == Suit::SPADES)
		return "Spades";
	
	if(s == Suit::CLUBS)
		return "Clubs";
	
	return "Diamonds";
}

Card::Card(Suit suit, int number)
{
	this->suit = suit;
	this->number = number;
	this->faceUp = false;
}

Suit Card::getSuit() const
{
	return suit;
}

void Card::setFaceUp(bool set)
{
	faceUp = set;
}

bool Card::getFaceUp() const
{
	return faceUp;
}

int Card::getNumber() const
{
	return number;
}

int Card::getHash() const
{
	return pow((39*number), double(int(suit)*number*7.0));
}

std::string Card::toString() const
{
	//Nothing should be able to know about face down cards.
	if(!getFaceUp())
		return "Face down card";

	std::stringstream toRet;

	if(number < 11 && number > 1)
		toRet << number << " of " << suitToString(suit);
	else
	{
		if(number == 1)
			toRet <<  "Ace of " << suitToString(suit);

		if(number == 11)
			toRet <<  "Jack of " << suitToString(suit);

		if(number == 12)
			toRet <<  "Queen of " << suitToString(suit);

		if(number == 13)
			toRet <<  "King of " << suitToString(suit);
	}

	return toRet.str();	
}

bool Card::operator==(const Card & other) const
{
	return (this->getSuit() == other.getSuit()) 
			&& 
			(this->getNumber() == other.getNumber());
}