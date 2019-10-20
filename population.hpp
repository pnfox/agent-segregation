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
        Person(const Person& rhs);
        Person& operator=(const Person& rhs) {return *this;};

        int findNewPosition(std::vector<Person> neighbours);
        void setPosition(float x, float y);
        void setIndex(int i);
        void setRace(bool race);
        void setEmpty(bool e);
        void setHappiness(std::vector<Person> neighbours);
        void setTolerance(float t);
        void update();

        std::tuple<float, float> getPosition() const {
            return std::make_tuple(this->x, this->y);
        };
        float getWidth() const {
            return this->width;
        };
        float getHeight() const {
            return this->height;
        };
        int getIndex() const {
            return this->index;
        };
        bool getRace() const {
            return this->race;
        };
        bool getEmpty() const {
            return this->empty;
        };
        float getHappiness() const {
            return this->happiness;
        };
        float getTolerance() const {
            return this->tolerance;
        };
    
    private:
        int index;
        sf::RectangleShape box;
        bool empty = false;
        bool race = false;
        float happiness = NAN;
        float tolerance = 0;
        float width;
        float height;
};

class Population{

    public:
        std::vector<Person> population;

        Population(float w, float h);

        std::vector<Person> findNeighbours(int i);
        void move(int i);
        Person* getPerson(int i);
        void printNeighbours(int i);

    private:
        int gridSize = 100;
        int numOfRows;
        int numOfColumns;
};

#endif
