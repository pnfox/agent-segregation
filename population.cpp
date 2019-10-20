#include "population.hpp"

Person::Person(float x, float y)
{
    sf::RectangleShape b(sf::Vector2f(x, y));
    this->width = x;
    this->height = y;
    b.setOutlineThickness(2);
    b.setOutlineColor(sf::Color(20, 20, 200));
    this->box = b;
}

Person::Person(const Person& rhs)
{
    sf::RectangleShape b(sf::Vector2f(rhs.getWidth(), rhs.getHeight()));
    this->width = rhs.getWidth();
    this->height = rhs. getHeight();
    b.setOutlineThickness(2);
    b.setOutlineColor(sf::Color(20, 20, 200));
    this->box = b;

    auto [ x, y ] = rhs.getPosition(); // C++17 binding
    this->setPosition(x, y);
    this->setIndex(rhs.getIndex());
    this->setRace(rhs.getRace());
    this->setEmpty(rhs.getEmpty());
    this->setTolerance(rhs.getTolerance());
}

void Person::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
    this->box.setPosition(x, y);
}

void Person::setIndex(int i)
{
    this->index = i;
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
        this->box.setFillColor(sf::Color(200,0,0));
}

void Person::setHappiness(std::vector<Person> neighbours)
{
    // Given a persons race, tolerance and neighbours
    // Calcualte it´s happiness
    int totalWithSameRace = 0;
    int totalWithDifferentRace = 0;
    int totalNeighbours = neighbours.size();

    if(this->getEmpty())
    {
        this->happiness = NAN;
        return;
    }

    for(Person p: neighbours)
    {
        if(p.getEmpty())
            continue;

        if(p.getRace() == this->race)
            totalWithSameRace++;
        else
            totalWithDifferentRace++;
    }
    
    this->happiness = (float) (totalWithSameRace) / (float) totalNeighbours;
    
}

void Person::setTolerance(float t)
{
    this->tolerance = t;
}

/*
 * Move person if he is unhappy with neighbours and there is an empty space
 * nearby
 */
int Person::findNewPosition(std::vector<Person> neighbours)
{
    /*std::cout << "Inside newPosition()" << std::endl;*/
    int newPosition = -1;

    if(!std::isnan(this->happiness) && !this->getEmpty())
    {
        /*std::cout << "NaN Happiness and Empty" << std::endl;*/
        this->setHappiness(neighbours);
    }

    // Doesn´t need to move, person is happy
    if(this->happiness >= this->tolerance)
    {
        /*std::cout << "Happiness >= tolerance" << std::endl;*/
        return -2;
    }

    for(Person p : neighbours)
    {
        // Move to empty space
        if(p.getEmpty())
            newPosition = p.getIndex();
    }

    return newPosition;
}

Population::Population(float w, float h)
{
    // Create a grid, each space representing a person or
    // empty space
    this->gridSize = 500;

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
    std::bernoulli_distribution race(0.5);
    std::bernoulli_distribution person(0.95);

    for(int i = 0; i < gridSize; i++)
    {
        // Create person and place in grid
        Person p = Person(width, height);
        p.setPosition(std::fmod(i*width, w), std::floor(i/numOfColumns)*height);
        p.setIndex(i);
        if(person(gen))
        {
            p.setRace(race(gen));
	    if(p.getRace())
	        p.setTolerance(0.5); // white squares tolerance
	    else
	        p.setTolerance(0.5); // grey squares tolerance
	}
        else
            p.setEmpty(true);
        this->population.insert(this->population.end(), p);
    }
    
    for(int i = 0; i < gridSize; i++)
    {
        this->population[i].setHappiness(this->population);
    }

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

    /*std::cout << "Neighbours of " << i << ":" << std::endl;
    for( int n : neighbours )
    {
        std::cout << n << ", ";
    }
    std::cout << std::endl;*/

    // Convert neighbours int vector into person vector
    std::vector<Person> n;
    for( int i : neighbours )
    {
        n.insert(n.end(), this->population[i]);
    }

    return n;
}

/*
 * Swap person i with person j in std::vector
 */
void Population::move(int i)
{
    Person* personMoving = this->getPerson(i);
    /*std::cout << "Index of person: " << personMoving->getIndex() << std::endl;*/

    if(personMoving->getEmpty())
        return;

    // See if we can move this Person in the population
    int j = personMoving->findNewPosition(this->findNeighbours(personMoving->getIndex()));

    if(j == -1) // no empty space available but unhappy
    {
        this->population[i].setEmpty(true);
	return;
    }

    if(j == -2) // person is happy
        return;

    /*std::cout << "Moving " << i << " to " << j << std::endl;*/
    if(!this->population[j].getEmpty())
        std::cerr << "Warning: Moving to non-empty space" << std::endl;

    Person copy = this->population[i];

    this->population[i].setRace(this->population[j].getRace());
    this->population[j].setRace(copy.getRace());

    this->population[i].setEmpty(this->population[j].getEmpty());
    this->population[j].setEmpty(copy.getEmpty());

    this->population[i].setTolerance(this->population[j].getTolerance());
    this->population[j].setTolerance(copy.getTolerance());

    this->population[i].setHappiness(this->population);
    this->population[j].setHappiness(this->population);
}

Person* Population::getPerson(int i)
{
    if(i < 0 || i > this->population.size())
        return NULL;

    Person* person = this->population.data();

    for(int j = 0; j < i; j++)
    {
        person++;
    }

    return person;
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
