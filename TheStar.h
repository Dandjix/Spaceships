#pragma once
#include "TarotCard.h"
#include <string>

class TheStar : public TarotCard
{
	std::string getDescription() override
	{
		return "- My Amy, what is this, some kind of hotline ? - All entities take 10x damage, weapons always drop loaded from enemies. You cannot store weapons in the inventory. You cannot reload weapons.";
	}


	std::string getTitle() override
	{
		return "The Star";
	}


	int getIndex() override
	{
		return 17;
	}

};