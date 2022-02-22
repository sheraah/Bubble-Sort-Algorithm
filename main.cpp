#include<iostream>
#include<SFML/Graphics.hpp>
#include<vector>

sf::RenderWindow window({ 1500,800 }, "Sort", sf::Style::Close);
sf::Event evnt;
sf::Color LineColor = sf::Color::Cyan;
std::vector<sf::RectangleShape> Lines;
std::vector<sf::RectangleShape> SortedLines;
constexpr float LineWidth = 6.f;
constexpr float BetweenLines = 2.f;
const float MaxLineHeight = float(window.getSize().y) - 10.f;
const float MinLineHeight = 300.f;
const float MoveSpeed = 10000.f;

bool Start = false;
bool Moving = false;

size_t index = 0;

int RandNumBetween(double n1, double n2)
{
	int result = rand() % int(n1);
	while(result < n2)
		result = rand() % int(n1);

	return result;
}

void resetMoving()
{
	for (size_t i = index + 1; i < Lines.size(); i++)
		Lines[i].move(-LineWidth, 0.f);

	SortedLines.push_back(Lines[index]);
	Lines.erase(Lines.begin() + index);
	Moving = false;
}

void update()
{
	if (!Moving)
	{
		float Biggest = Lines[0].getSize().y;
		index = 0;
		for (size_t i = 1; i < Lines.size(); i++)
		{
			if (Lines[i].getSize().y >= Biggest)
			{
				Biggest = Lines[i].getSize().y;
				index = i;
			}
		}

		Moving = true;
	}
	
	if (Moving)
	{

		if (SortedLines.size() > 0)
		{
			if (Lines[index].getPosition().x + LineWidth < SortedLines[SortedLines.size() - 1].getPosition().x)
				Lines[index].move(MoveSpeed, 0.f);
			else
			{
				Lines[index].setPosition(SortedLines[SortedLines.size() - 1].getPosition().x - LineWidth, Lines[index].getPosition().y);
				resetMoving();
			}
		}
	    
		else
		{
			if (Lines[index].getPosition().x + LineWidth < window.getSize().x - LineWidth)
				Lines[index].move(MoveSpeed, 0.f);
			else
			{
				Lines[index].setPosition(window.getSize().x - LineWidth * 0.6, Lines[index].getPosition().y);
				resetMoving();
			}
		}
	}	

}

int main()
{
	srand(unsigned int(time(0)));
	window.setFramerateLimit(60);

	for (auto i = 0; i < window.getSize().x / LineWidth; i++)
	{
		sf::RectangleShape shape({ LineWidth - BetweenLines, float(RandNumBetween(MaxLineHeight, MinLineHeight)) });
		shape.setPosition(i * LineWidth + BetweenLines, window.getSize().y - shape.getSize().y);
		shape.setFillColor(LineColor);
		Lines.push_back(shape);
	}

	while (window.isOpen())
	{
		while (window.pollEvent(evnt))
			if (evnt.type == sf::Event::Closed) window.close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			Start = true;
		if(Start && Lines.size() > 0)
		 update();

		window.clear(sf::Color::White);
		
		for (const auto& i : Lines)
			window.draw(i);

		for (const auto& i : SortedLines)
			window.draw(i);

		window.display();
	}
}