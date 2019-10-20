#include <SFML/Graphics.hpp>
#include "population.hpp"

using namespace std;

int main(int argc, char *argv[]) {

    const sf::VideoMode screenSize = sf::VideoMode(800, 800, 24);
    sf::RenderWindow window(screenSize, "SFML");
    const float frameRate = 30.0;
    window.setFramerateLimit(frameRate);
    window.setActive(false);
    window.requestFocus();

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.f/frameRate);

    Population pop = Population(800, 800);
    bool startAgents = false;

    // Main Loop here
    while(window.isOpen())
    {

        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        if(timeSinceLastUpdate < timePerFrame)
            continue;

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                const sf::Keyboard::Key keycode = event.key.code;
                if(keycode == sf::Keyboard::Space)
                {
                    startAgents = true;
                }
            }
        }

        window.clear();

        if(startAgents)
        {
            // Perform agent calculation
            for(Person p: pop.population){
                if(p.getEmpty())
                    continue;
                pop.move(p.getIndex());
            }
        }

        // Draw population on screen
        for(Person p : pop.population){
            window.draw(p.getBox());
        }

        window.display();

        timeSinceLastUpdate = sf::Time::Zero;
    }

    return 0;
}
