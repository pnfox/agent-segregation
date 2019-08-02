#ifndef POPULATION_H
#define POPULATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

class Person{

    public:
        // x and y positions
        float x;
        float y;
        // get box for drawing to screen
        sf::RectangleShape getBox() {
            return this->box;
        };

        Person(float x, float y);

        void setPosition(float x, float y);
        void setRace(bool race);
        void setEmpty(bool e);
        void setHappiness(std::vector<Person> neighbours);

        bool getRace() {
            return this->race;
        };
        bool getEmpty() {
            return this->empty;
        };
    
    private:
        sf::RectangleShape box;
        bool empty = false;
        bool race = false;
        float happiness = 0;
};

class Population{

    public:
        std::vector<Person> population;

        Population(float w, float h);

        std::vector<Person> findNeighbours(int i);
        void printNeighbours(int i);

    private:
        int gridSize = 100;
        int numOfRows;
        int numOfColumns;
};

#endif
