#pragma once
#include "Matrices.h"
#include <SFML/Graphics.hpp>

const float G = 2000;      // Gravity strength
const float TTL = 3.0;  // Particle's lifespan (Time To Live)
const float SCALE = 0.999; // Scaling factor per frame

using namespace Matrices;
using namespace sf;

class Particle : public Drawable
{
public:
    Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition);
    virtual void draw(RenderTarget& target, RenderStates states) const override;
    void update(float dt);
    float getTTL() { return m_ttl; }

    // Unit testing functions
    bool almostEqual(double a, double b, double eps = 0.0001);
    void unitTests();

private:
    float m_ttl;           // Remaining life
    int m_numPoints;       // Number of vertices
    Vector2f m_centerCoordinate; // Particle's center
    float m_radiansPerSec;   // Rotation speed
    float m_vx;              // Horizontal velocity
    float m_vy;              // Vertical velocity
    View m_cartesianPlane;   // View for coordinate mapping
    Color m_color1;          // Center color
    Color m_color2;          // Vertex color
    Matrix m_A;              // Matrix for vertex coordinates

    // Rotate particle counter-clockwise by theta radians
    void rotate(double theta);

    // Scale particle size by factor c
    void scale(double c);

    // Move particle by (xShift, yShift)
    void translate(double xShift, double yShift);
};
