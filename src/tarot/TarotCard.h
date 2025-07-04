#pragma once
#include <string>

//see https://en.wikipedia.org/wiki/Tarot_card_reading#Trumps
//trumps -> "Tarot de Marseille"

/// <summary>
/// cards are difficulty modifiers, like skulls in halo. You can activate and deactivate through Starnet on the terminal on most ships
/// </summary>
class TarotCard
{
public:
	virtual std::string getDescription() = 0;
	virtual std::string getTitle() = 0;
	virtual int getIndex() = 0;
	std::string getFrontImage();
	std::string getBackImage();
};