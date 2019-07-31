#include "population.hpp"

Person::Person(float x, float y)
{
    sf::RectangleShape b(sf::Vector2f(x, y));
    b.setOutlineThickness(2);
    b.setOutlineColor(sf::Color(20, 20, 200));
    this->box = b;
}

void Person::setPosition(float x, float y)
{
    this->box.setPosition(x, y);
}

void Person::setRace(bool race)
{
    if(race)
        this->box.setFillColor(sf::Color(255,255,255));
    else
        this->box.setFillColor(sf::Color(60,60,60));
}

void Person::setEmpty(bool e)
{
    this->empty = e;

    if(e)
        this->box.setFillColor(sf::Color(0,0,0));
}

Population::Population(float w, float h)
{
    // Create a grid, each space representing a person or
    // empty space
    this->gridSize = 100;

    this->numOfColumns = 1;
    this->numOfRows = gridSize / numOfColumns;
    double width, height;

   for(int i = 0; i < gridSize; i++)
    {
        if(std::remainder(gridSize, i) == 0 && (i - (gridSize / i)) < 2)
        {
            this->numOfColumns = i;
            this->numOfRows = gridSize / numOfColumns;
        }
    }

    this->numOfRows = gridSize / numOfColumns;

    // set width and height of each persons rectangleShape
    width = w / numOfColumns;
    height = h / numOfRows;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::bernoulli_distribution race(0.75);
    std::bernoulli_distribution person(0.8);

    for(int i = 0; i < gridSize; i++)
    {
        // Create person and place in grid
        Person p = Person(width, height);
        p.setPosition(std::fmod(i*width, w), std::floor(i/numOfColumns)*height);
        if(person(gen))
            p.setRace(race(gen));
        else
            p.setEmpty(true);
        this->population.insert(this->population.end(), p);
    }
}
