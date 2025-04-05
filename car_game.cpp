#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>


// Window size
const int WIDTH = 500;
const int HEIGHT = 700;

// Car position
float carX = 200;
const float carY = 100;
const float carWidth = 50;
const float carHeight = 80;

// Obstacles
struct Obstacle {
    float x, y;
    float width, height;
};

std::vector<Obstacle> obstacles;
float obstacleSpeed = 5.0f;
int score = 0;

// Function to draw a rectangle
void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Initialize OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    srand(time(0));
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw car (red)
    glColor3f(1.0, 0.0, 0.0);
    drawRectangle(carX, carY, carWidth, carHeight);
    
    // Draw obstacles (white)
    glColor3f(1.0, 1.0, 1.0);
    for (const auto& obs : obstacles) {
        drawRectangle(obs.x, obs.y, obs.width, obs.height);
    }
    
    glutSwapBuffers();
}

// Update function (moves obstacles)
void update(int value) {
    for (auto& obs : obstacles) {
        obs.y -= obstacleSpeed;
        
        // Collision detection
        if (obs.y < carY + carHeight && obs.y + obs.height > carY &&
            obs.x < carX + carWidth && obs.x + obs.width > carX) {
            std::cout << "Game Over! Score: " << score << std::endl;
            exit(0);
        }
    }
    
    // Remove obstacles that go off-screen
    obstacles.erase(
        remove_if(obstacles.begin(), obstacles.end(), [](Obstacle& obs) { return obs.y < 0; }),
        obstacles.end()
    );
    
    // Spawn new obstacles
    if (rand() % 50 == 0) {
        obstacles.push_back({static_cast<float>(rand() % (WIDTH - 50)), HEIGHT, 50, 50});
        score++;
    }
    
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

// Keyboard input
void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_LEFT && carX > 0)
        carX -= 20;
    if (key == GLUT_KEY_RIGHT && carX < WIDTH - carWidth)
        carX += 20;
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("2D Car Game");
    
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(30, update, 0);
    glutMainLoop();
    
    return 0;
}
