#include "Engine.h"
#include "Particle.h"

int main()
{
    // Create an Engine instance.
    Engine engine;

    // Create a Particle for unit testing.
    // The tests are provided to check your code.
    std::cout << "Starting Particle unit tests..." << std::endl;
    Particle p(engine.getWindow(), 4, Vector2i(engine.getWindow().getSize().x / 2, engine.getWindow().getSize().y / 2));
    p.unitTests();
    std::cout << "Unit tests complete. Starting engine..." << std::endl;

    // Run the engine (start the game loop).
    engine.run();

    // The program exits here when the engine stops.
    return 0;
}
