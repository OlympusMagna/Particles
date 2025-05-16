#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
private:
    // The main game window
    RenderWindow m_Window;

    // Collection of particles
    vector<Particle> m_particles;
    bool m_isMousePressed; // Tracks if the mouse button is pressed

    // Private methods for game logic
    void input();          // Handles user input
    void update(float dtAsSeconds); // Updates game state
    void draw();           // Renders the scene

public:
    // Engine constructor
    Engine();

    // Starts the main game loop
    void run();

    // Provides access to the game window
    RenderWindow& getWindow() { return m_Window; }
};
