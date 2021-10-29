#include "pch.h"

typedef sf::Event sfe;
typedef sf::Keyboard sfk;

#define M_PI 3.14

struct Spherical
{
    float distance, theta, phi;
    Spherical(float gdistance, float gtheta, float gphi) : distance(gdistance), theta(gtheta), phi(gphi) { }
    float getX() { return distance * cos(phi) * cos(theta); }
    float getY() { return distance * sin(phi); }
    float getZ() { return distance * cos(phi) * sin(theta); }
};

Spherical camera(3.0f, 1.0f, 0.2f);
sf::Vector3f pos(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rot(0.0f, 0.0f, 0.0f);

float* pos_offsets[3] = { &pos.x, &pos.y, &pos.z };
float* scale_offsets[3] = { &scale.x, &scale.y, &scale.z };
float* rot_offsets[3] = { &rot.x, &rot.y, &rot.z };

void initOpenGL(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}

void reshapeScreen(sf::Vector2u size)
{
    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawScene()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Spherical north_of_camera(camera.distance, camera.theta, camera.phi + 0.01f);
    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
        0.0, 0.0, 0.0,
        north_of_camera.getX(), north_of_camera.getY(), north_of_camera.getZ());

    //uklad
    glBegin(GL_LINES);
    //Osie ukladu
    glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, 1.0, 0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
    glEnd();

    //Linie przerywane
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(-1.0, 0, 0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, -1.0, 0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, -1.0);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    //transformacje
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(rot.x, 1, 0, 0);
    glRotatef(rot.y, 0, 1, 0);
    glRotatef(rot.z, 0, 0, 1);
    glScalef(scale.x, scale.y, scale.z);

    //szescian
    glLineWidth(2.0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    for (unsigned char i = 0; i < 2; i++)
        for (unsigned char j = 0; j < 2; j++)
        {
            glVertex3f(-0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j, -0.3f); glVertex3f(-0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j, 0.3f);
            glVertex3f(-0.3f, -0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j); glVertex3f(0.3f, -0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j);
            glVertex3f(-0.3f + 0.6f * (i ^ j), -0.3f, -0.3f + 0.6f * j); glVertex3f(-0.3f + 0.6f * (i ^ j), 0.3f, -0.3f + 0.6f * j);
        }
    glEnd();
    glLineWidth(1.0);
    //trojkat
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.3f, 0.3f, 0.3f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.3f, -0.3f, 0.3f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.3f, 0.3f, -0.3f);
    glEnd();
}

int main()
{
    bool running = true;
    sf::ContextSettings context(24, 0, 0, 4, 5);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dice Kingdom", 7U, context);
    sf::Clock deltaClock;

    ImGui::SFML::Init(window);

    window.setVerticalSyncEnabled(true);

    reshapeScreen(window.getSize());
    initOpenGL();

    while (running)
    {
        sfe event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sfe::Closed || (event.type == sfe::KeyPressed && event.key.code == sfk::Escape)) running = false;
            if (event.type == sfe::Resized) reshapeScreen(window.getSize());
        }

        if (sfk::isKeyPressed(sfk::Left)) { camera.theta -= 0.0174533f; if (camera.theta < 0.0f) camera.theta = 0.0f; }
        if (sfk::isKeyPressed(sfk::Right)) { camera.theta += 0.0174533f; if (camera.theta > (float)(2.0 * M_PI)) camera.theta = (float)(2.0 * M_PI); }
        if (sfk::isKeyPressed(sfk::Up)) { camera.phi += 0.0174533f;  if (camera.phi > (float)M_PI) camera.phi = (float)M_PI; }
        if (sfk::isKeyPressed(sfk::Down)) { camera.phi -= 0.0174533f; if (camera.phi < 0.0f) camera.phi = 0.0f; }

        drawScene();

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Camera");
        ImGui::SliderFloat("R", &camera.distance, 0.5f, 10.0f);
        ImGui::SliderAngle("theta", &camera.theta, 0.0f, 360.0f);
        ImGui::SliderAngle("phi", &camera.phi, 0.0f, 180.0f);
        ImGui::End();

        ImGui::Begin("Transformations");
        ImGui::SliderFloat3("Position", *pos_offsets, -3.0f, 3.0f);
        ImGui::SliderFloat3("Scale", *scale_offsets, -2.0f, 2.0f);
        ImGui::SliderFloat3("Rotation", *rot_offsets, -180.0f, 180.0f);
        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}
