#ifndef POPULATION_H
#define POPULATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>

class Person{

    public:
        float x;
        float y;
        sf::RectangleShape getBox() {
            return this->box;
        };

        Person(float x, float y);

        void setPosition(float x, float y);
        void setRace(bool race);
        void setEmpty(bool e);

    private:
        sf::RectangleShape box;
        bool empty = false;
};

class Population{

    public:
        std::vector<Person> population;

        Population(float w, float h);

    private:
        int gridSize = 100;
        int numOfRows;
        int numOfColumns;
};

#endif
