#include "Engine.h"

Engine::Engine()
{
    // Call create on m_Window to populate the RenderWindow member variable
    // You can assign a custom resolution or you can call VideoMode::getDesktopMode() 
    m_Window.create(VideoMode::getDesktopMode(), "Particles");
    m_isMousePressed = false; // Initialize the mouse press state
}

void Engine::run()
{
    // Construct a local Clock object to track Time per frame
    // The time differential (dt) since the last frame will be passed into update
    Clock clock;

    // Construct a local Particle to be used for the unit tests. 
    // The tests will be given to you, and you can use them to check your progress as you go. 
    // Use the following code exactly:
    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, Vector2i(m_Window.getSize().x / 2, m_Window.getSize().y / 2));
    p.unitTests();
    cout << "Unit tests complete. Starting engine..." << endl;

    // After the unit tests are complete, create your game loop
    // Loop while m_Window is open
    while (m_Window.isOpen())
    {
        // Restart the clock (this will return the time elapsed since the last frame)
        Time dt = clock.restart();

        // Convert the clock time to seconds
        float dtAsSeconds = dt.asSeconds();

        // Call input
        input();

        // Call update
        update(dtAsSeconds);

        // Call draw
        draw();
    }
}

void Engine::input()
{
    // Poll the Windows event queue
    Event event;
    while (m_Window.pollEvent(event))
    {
        // Handle the Escape key pressed and closed events so your program can exit
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
            {
                m_Window.close();
            }
        }
        if (event.type == Event::Closed)
        {
            m_Window.close();
        }

        // Handle the left mouse button pressed event
        if (event.type == Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == Mouse::Left)
            {
                m_isMousePressed = true; // Set the flag to true when the mouse button is pressed
                // Create particles at the initial mouse press position
                for (int i = 0; i < 5; i++)
                {
                    int numPoints = rand() % 26 + 25;
                    m_particles.push_back(Particle(m_Window, numPoints, Mouse::getPosition(m_Window)));
                }
            }
        }

        // Handle the left mouse button released event
        if (event.type == Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == Mouse::Left)
            {
                m_isMousePressed = false; // Set the flag to false when the mouse button is released
            }
        }
    }

    // Create particles while the left mouse button is held down
    if (m_isMousePressed)
    {
        for (int i = 0; i < 5; i++)
        {
            int numPoints = rand() % 26 + 25;
            m_particles.push_back(Particle(m_Window, numPoints, Mouse::getPosition(m_Window)));
        }
    }
}

void Engine::update(float dtAsSeconds)
{
    // The general idea here is to loop through m_particles and call update on each Particle in the vector 
    // whose ttl (time to live) has not expired
    // If a particle's ttl has expired, it must be erased from the vector
    // This is best done with an iterator-based for-loop
    // Don't automatically increment the iterator for each iteration
    for (auto it = m_particles.begin(); it != m_particles.end(); )
    {
        if (it->getTTL() > 0.0)
        {
            // Call update on that Particle
            // Pass in the time differential since the last frame (dt)
            it->update(dtAsSeconds);
            // increment the iterator
            ++it;
        }
        else
        {
            // erase the element the iterator points to
            it = m_particles.erase(it);
            // erase returns an iterator that points to the next element after deletion, or end if it is the end of the vector
            // Assign the iterator to this return value
            // Do not increment the iterator (if you do you might increment past the end of the vector after you delete the last element)
        }
    }
}

void Engine::draw()
{
    // Clear the last frame
    m_Window.clear();

    // Draw all the particles
    for (auto const& particle : m_particles)
    {
        m_Window.draw(particle);
    }

    // End the current frame and display its contents on screen
    m_Window.display();
}

