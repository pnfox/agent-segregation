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
    this->x = x;
    this->y = y;
    this->box.setPosition(x, y);
}

void Person::setRace(bool race)
{
    this->race = race;
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

void Person::setHappiness(std::vector<Person> neighbours)
{
    // Given a persons race, tolerance and neighbours
    // Calcualte it´s happiness
    int totalWithSameRace;
    for(Person p: neighbours)
    {
        if(p.getRace() == this->race && !p.getEmpty())
            totalWithSameRace++;
    }
    
    std::cout << "Total with same race " << totalWithSameRace << std::endl;
    
}

Population::Population(float w, float h)
{
    // Create a grid, each space representing a person or
    // empty space
    this->gridSize = 100;

    this->numOfColumns = 1;
    this->numOfRows = gridSize / numOfColumns;
    float width, height;

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
    std::bernoulli_distribution race(0.6);
    std::bernoulli_distribution person(0.85);

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
    
    this->population[3].setHappiness(findNeighbours(3));
}

std::vector<Person> Population::findNeighbours(int i)
{
    // Given the ith person in the grid, find its neighbours

    bool top = false;
    bool right = false;
    bool left = false;
    bool bottom = false;

    std::vector<int> neighbours;
    
    if( i <= (this->numOfColumns-1) )
        top = true;
    if( (this->numOfRows-1)*this->numOfColumns <= i && (i <= this->numOfRows*this->numOfColumns) )
        bottom = true;
    
    for( int j = 1; j <= this->numOfRows; j++ )
    {
        if( i == (this->numOfColumns*j - 1) )
            right = true;
        if( i == (this->numOfColumns*j) || i == 0)
            left = true;
    }

    if(left)
        neighbours.insert(neighbours.end(), i + 1);
    if(right)
        neighbours.insert(neighbours.end(), i - 1);
    // Top left corner
    if (top && left)
    {
        neighbours.insert(neighbours.end(), i + this->numOfColumns);
        neighbours.insert(neighbours.end(), i + this->numOfColumns + 1);
    }
    // Top right corner
    else if( top && right )
    {
        neighbours.insert(neighbours.end(), i + this->numOfColumns);
        neighbours.insert(neighbours.end(), i + this->numOfColumns - 1);
    }
    else if( bottom && (left || right) )
    {
        neighbours.insert(neighbours.end(), i - this->numOfColumns);
    }

    // top or bottom, not in corners
    if( top && !right && !left )
    {
        neighbours.insert(neighbours.end(), i - 1);
        neighbours.insert(neighbours.end(), i + 1);
        neighbours.insert(neighbours.end(), i + this->numOfColumns - 1);
        neighbours.insert(neighbours.end(), i + this->numOfColumns);
        neighbours.insert(neighbours.end(), i + this->numOfColumns + 1);
    }
    else if (bottom && !right && !left)
    {
        neighbours.insert(neighbours.end(), i - 1);
        neighbours.insert(neighbours.end(), i + 1);
        neighbours.insert(neighbours.end(), i - this->numOfColumns - 1);
        neighbours.insert(neighbours.end(), i - this->numOfColumns);
        neighbours.insert(neighbours.end(), i - this->numOfColumns + 1);
    }
    // left or right, not in corners
    else if( (left || right) && !top && !bottom)
    {
        neighbours.insert(neighbours.end(), i - this->numOfColumns);
        neighbours.insert(neighbours.end(), i + this->numOfColumns);
    }

    // not near a border
    if( !top && !right && !left && !bottom )
    {
        neighbours.insert(neighbours.end(), i - this->numOfColumns - 1);
        neighbours.insert(neighbours.end(), i - this->numOfColumns); // top neighbour
        neighbours.insert(neighbours.end(), i - this->numOfColumns + 1);
        neighbours.insert(neighbours.end(), i - 1); // left neighbour
        neighbours.insert(neighbours.end(), i + 1); // right neighbour
        neighbours.insert(neighbours.end(), i + this->numOfColumns - 1);
        neighbours.insert(neighbours.end(), i + this->numOfColumns); // bottom neighbour
        neighbours.insert(neighbours.end(), i + this->numOfColumns + 1);
    }

    std::cout << "Neighbours of " << i << ":" << std::endl;
    for( int n : neighbours )
    {
        std::cout << n << ", ";
    }
    std::cout << std::endl;

    // Convert neighbours int vector into person vector
    std::vector<Person> n;
    for( int i : neighbours )
    {
        n.insert(n.end(), this->population[i]);
    }

    return n;
}


/*void Population::printNeighbours(int i) 
{
    std::vector<Person> neighbours = this->findNeighbours(i);
    std::cout << "Neighbours of " << i << ":" << std::endl;
    for( Person n : neighbours )
    {
        std::cout << n << ", ";
    }
    std::cout << std::endl;
}*/
