# agent-segregation
Implemention of the Schelling segregation model in C++ using SFML

In 1971, the American economist Thomas Schelling created an agent-based model  His model of segregation showed
that even when agents didn't mind being surrounded or living by agents of a different race,
they would still choose to segregate themselves from other agents over time.

I implemented this in C++ mainly because I wanted to learn this language but I was also studying agent-based models
at the time too. SFML is used for creating window and drawing simulation.

### Prerequisites

* C++17 (can be run with lower versions if line 22:population.cpp changed)
* [SFML](https://github.com/SFML/SFML)

### Usage
Run `make` and then `./final`

This is not a perfect implementation by any means. It´s not multithreaded, doesn´t use a GPU and currently
needs to be recompiled everytime parameters are tweaked.

### Before Simulation
![Image of Simulation](https://i.imgur.com/3VPrY1v.png)

### After Simulation
![Image of Simulation](https://i.imgur.com/TwLf2HQ.png)
