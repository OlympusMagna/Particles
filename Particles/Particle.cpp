#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints)
{
    m_ttl = TTL;
    m_numPoints = numPoints;
    m_radiansPerSec = static_cast<float>(rand()) / (RAND_MAX) * static_cast<float>(M_PI);

    m_cartesianPlane.setCenter(0.f, 0.f);
    m_cartesianPlane.setSize(static_cast<float>(target.getSize().x), -1.0f * static_cast<float>(target.getSize().y));

    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    m_vx = static_cast<float>(rand() % 401 + 100);
    if (rand() % 2) {
        m_vx *= -1;
    }
    m_vy = static_cast<float>(rand() % 401 + 100);
    if (rand() % 2) {
        m_vy *= -1;
    }

    m_color1 = Color::White;
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

    float theta = static_cast<float>(rand()) / (RAND_MAX) * (static_cast<float>(M_PI) / 2.0f);
    float dTheta = 2.0f * static_cast<float>(M_PI) / (numPoints > 1 ? (numPoints -1) : 1) ;

    for (int j = 0; j < numPoints; j++)
    {
        float r = static_cast<float>(rand() % 61 + 20);
        float dx = r * std::cos(theta);
        float dy = r * std::sin(theta);
        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray lines(TriangleFan, m_numPoints + 1);

    sf::Vector2i centerPixelCoords = target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
    lines[0].position = sf::Vector2f(static_cast<float>(centerPixelCoords.x), static_cast<float>(centerPixelCoords.y));
    lines[0].color = m_color1;

    for (int j = 1; j <= m_numPoints; j++)
    {
        // m_A stores world coordinates for each vertex
        sf::Vector2f vertexWorldCoords(
            static_cast<float>(m_A(0, j - 1)),
            static_cast<float>(m_A(1, j - 1))
        );
        sf::Vector2i vertexPixelCoords = target.mapCoordsToPixel(vertexWorldCoords, m_cartesianPlane);
        lines[j].position = sf::Vector2f(static_cast<float>(vertexPixelCoords.x), static_cast<float>(vertexPixelCoords.y));
        lines[j].color = m_color2;
    }
    target.draw(lines, states);
}

void Particle::update(float dt)
{
    m_ttl -= dt;

    if (m_ttl > 0) {
        rotate(dt * m_radiansPerSec);
        scale(SCALE);

        float dx = m_vx * dt;
        m_vy -= G * dt;
        float dy = m_vy * dt;
        translate(dx, dy);
    }
}

void Particle::translate(double xShift, double yShift)
{
    TranslationMatrix T(xShift, yShift, m_A.getCols());
    m_A = T + m_A;

    m_centerCoordinate.x += static_cast<float>(xShift);
    m_centerCoordinate.y += static_cast<float>(yShift);
}

void Particle::rotate(double theta)
{
    sf::Vector2f originalCenter = m_centerCoordinate;
    // Move vertices so the center of rotation is at the origin
    for(int j=0; j < m_A.getCols(); ++j) {
        m_A(0,j) -= originalCenter.x;
        m_A(1,j) -= originalCenter.y;
    }

    RotationMatrix R(theta);
    m_A = R * m_A;

    // Move vertices back to the original center
    for(int j=0; j < m_A.getCols(); ++j) {
        m_A(0,j) += originalCenter.x;
        m_A(1,j) += originalCenter.y;
    }
}

void Particle::scale(double c)
{
    sf::Vector2f originalCenter = m_centerCoordinate;
    // Move vertices so the center of scaling is at the origin
     for(int j=0; j < m_A.getCols(); ++j) {
        m_A(0,j) -= originalCenter.x;
        m_A(1,j) -= originalCenter.y;
    }

    ScalingMatrix S(c);
    m_A = S * m_A;

    // Move vertices back to the original center
    for(int j=0; j < m_A.getCols(); ++j) {
        m_A(0,j) += originalCenter.x;
        m_A(1,j) += originalCenter.y;
    }
}

bool Particle::almostEqual(double a, double b, double eps)
{
    return std::fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;
    using namespace std;
    using namespace Matrices;

    cout << "Testing RotationMatrix constructor..." << endl;
    double rot_theta = M_PI / 4.0;
    RotationMatrix r(rot_theta);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(rot_theta))
        && almostEqual(r(0, 1), -sin(rot_theta))
        && almostEqual(r(1, 0), sin(rot_theta))
        && almostEqual(r(1, 1), cos(rot_theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor..." << endl;
    ScalingMatrix sm(1.5);
    if (sm.getRows() == 2 && sm.getCols() == 2
        && almostEqual(sm(0, 0), 1.5)
        && almostEqual(sm(0, 1), 0)
        && almostEqual(sm(1, 0), 0)
        && almostEqual(sm(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor..." << endl;
    int nCols_for_test = m_A.getCols();
    TranslationMatrix t(5, -5, nCols_for_test);
    bool translationMatrixTestPassed = true;
    if (t.getRows() == 2 && t.getCols() == nCols_for_test) {
        for (int j = 0; j < nCols_for_test; ++j) {
            if (!almostEqual(t(0, j), 5) || !almostEqual(t(1, j), -5)) {
                translationMatrixTestPassed = false;
                break;
            }
        }
    } else {
        translationMatrixTestPassed = false;
    }

    if (translationMatrixTestPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing Particles..." << endl;
    cout << "Testing Particle initial m_centerCoordinate..." << endl;
    // Create a Particle with a known mouse position for reliable testing.
    // Use (0,0) and a cartesian plane centered at (0,0).
    sf::RenderWindow testWindow(sf::VideoMode(1920, 1080), "Test Window");
    Vector2i testPosition(0, 0);
    Particle testParticle(testWindow, 4, testPosition);

    if (almostEqual(testParticle.m_centerCoordinate.x, 0.0) && almostEqual(testParticle.m_centerCoordinate.y, 0.0))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed: m_centerCoordinate is (" << testParticle.m_centerCoordinate.x << "," << testParticle.m_centerCoordinate.y << ")." << endl;
    }

    cout << "Applying one rotation of 90 degrees about the particle's center..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    if (m_A.getCols() == initialCoords.getCols()) {
        for (int j = 0; j < initialCoords.getCols(); j++)
        {
            // Rotation around (cx, cy) by 90 deg: (x,y) -> (cx - (y-cy), cy + (x-cx))
            double expected_x = m_centerCoordinate.x - (initialCoords(1, j) - m_centerCoordinate.y);
            double expected_y = m_centerCoordinate.y + (initialCoords(0, j) - m_centerCoordinate.x);

            if (!almostEqual(m_A(0, j), expected_x) || !almostEqual(m_A(1, j), expected_y))
            {
                cout << "Failed mapping for rotation: ";
                cout << "Point " << j << ": Original (" << initialCoords(0, j) << ", " << initialCoords(1, j) << ")";
                cout << " ==> New (" << m_A(0, j) << ", " << m_A(1, j) << ")";
                cout << " Expected (" << expected_x << ", " << expected_y << ")" << endl;
                rotationPassed = false;
            }
        }
    } else {
        cout << "Failed rotation test: Column count mismatch." << endl;
        rotationPassed = false;
    }

    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }
    m_A = initialCoords; // Restore for next test

    cout << "Applying a scale of 0.5 around particle's center..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    if (m_A.getCols() == initialCoords.getCols()) {
        for (int j = 0; j < initialCoords.getCols(); j++)
        {
            // Scaling (x,y) by c relative to (cx,cy): (cx + c*(x-cx), cy + c*(y-cy))
            double expected_x = m_centerCoordinate.x + 0.5 * (initialCoords(0, j) - m_centerCoordinate.x);
            double expected_y = m_centerCoordinate.y + 0.5 * (initialCoords(1, j) - m_centerCoordinate.y);

            if (!almostEqual(m_A(0, j), expected_x) || !almostEqual(m_A(1, j), expected_y))
            {
                cout << "Failed mapping for scale: ";
                cout << "Point " << j << ": Original (" << initialCoords(0, j) << ", " << initialCoords(1, j) << ")";
                cout << " ==> New (" << m_A(0, j) << ", " << m_A(1, j) << ")";
                cout << " Expected (" << expected_x << ", " << expected_y << ")" << endl;
                scalePassed = false;
            }
        }
    } else {
        cout << "Failed scale test: Column count mismatch." << endl;
        scalePassed = false;
    }

    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }
    m_A = initialCoords; // Restore for next test

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    sf::Vector2f initialCenter = m_centerCoordinate;
    translate(10, 5);
    bool translatePassed = true;
    if (!almostEqual(m_centerCoordinate.x, initialCenter.x + 10) || !almostEqual(m_centerCoordinate.y, initialCenter.y + 5)) {
        cout << "Failed: Center not translated correctly." << endl;
        cout << "Initial Center: (" << initialCenter.x << ", " << initialCenter.y << ")" << endl;
        cout << "New Center: (" << m_centerCoordinate.x << ", " << m_centerCoordinate.y << ")" << endl;
        cout << "Expected Center: (" << initialCenter.x + 10 << ", " << initialCenter.y + 5 << ")" << endl;
        translatePassed = false;
    }
    if (m_A.getCols() == initialCoords.getCols()) {
        for (int j = 0; j < initialCoords.getCols(); j++)
        {
            if (!almostEqual(m_A(0, j), initialCoords(0, j) + 10) || !almostEqual(m_A(1, j), initialCoords(1, j) + 5))
            {
                cout << "Failed mapping for translation: ";
                cout << "Point " << j << ": Original (" << initialCoords(0, j) << ", " << initialCoords(1, j) << ")";
                cout << " ==> New (" << m_A(0, j) << ", " << m_A(1, j) << ")";
                cout << " Expected (" << initialCoords(0, j) + 10 << ", " << initialCoords(1, j) + 5 << ")" << endl;
                translatePassed = false;
            }
        }
    } else {
         cout << "Failed translation test: Column count mismatch." << endl;
        translatePassed = false;
    }

    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7 (Note: Particle origin test corrected)" << endl;
}
