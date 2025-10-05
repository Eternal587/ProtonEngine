//
//  main.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/5/25.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cmath>
#include <array>
#define GL_SILENCE_DEPRECATION

class Cube {
public:
    float x;
    float y;
    float z;
    float l;
    float w;
    float h;
};

double cx = 0;
double cy = 0;
double cz = 0;
double cangle = 0;

float forwardZ = -sin(cangle);
float forwardX = -cos(cangle);

/// 3d Projection x

double tdpx(double x, double z, double fov) {
    
    /// Defining the aspect ratio and fov for 3d calulations
    
    int width  = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    double aspectratio = (width / height);
    double f = (1 / tan(fov/2));
    
    double px = (aspectratio * f * (x * cos(cangle) + z * sin(cangle)))/ ((0-x) *  sin(cangle) + z * cos(cangle));
    
    return px;
}

/// 3d Projection y

double tdpy(double x, double y, double z, double fov) {
    double f = (1 / tan(fov/2));
    
    double py = (f * y)/ ((0-x) *  sin(cangle) + z * cos(cangle));
    
    return py;
}

void handleKeys(unsigned char key, int x, int y) {
    
    float forwardX = sin(cangle);
    float forwardZ = cos(cangle);
    float speed = 1;
    
    switch (key) {
        case 27:  // Escape key ASCII code
            std::cout << "Escape pressed. Exiting...\n";
            exit(0);  // Exit the program
            break;

        case 'w':
            cz++;
            glutPostRedisplay(); // Ask GLUT to redraw
            break;

        case 'a':
            cx--;
            glutPostRedisplay();
            break;

        case 's':
            cz--;
            glutPostRedisplay();
            break;
        case 'd':
            cx++;
            glutPostRedisplay();
            break;
        case ' ':
            cy++;
            glutPostRedisplay();
            break;
        case 'c':
            cy--;
            glutPostRedisplay();
            break;
        case ',':
            cangle = cangle + 0.1;;
            glutPostRedisplay();
            std::cout << "Camera Angle: " << cangle << "\nForwardX: " << forwardX << "\nForwardZ: " << forwardZ << std::endl;
            break;
        case '/':
            cangle = cangle - 0.1;
            glutPostRedisplay();
            std::cout << "Camera Angle: " << cangle << "\nForwardX: " << forwardX << "\nForwardZ: " << forwardZ << std::endl;
            break;
        default:
            std::cout << "Key pressed: " << key << " at (" << x << "," << y << ")\n";
            break;
    }
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    /// Getting the Width and height of the window
    double fov = 1.2;
    
    /// Calculating Verticies
    
    Cube cube1;
    
    /// Defining Cube Dimesions
    
    cube1.x = -2.5f;
    cube1.y = 0.0f;
    cube1.z = 2.5f;
    cube1.l = 5.0f;
    cube1.w = 5.0f;
    cube1.h = 5.0f;
    
    /// Defining Cube Offsets to make vertices cleaner
    
    float offsetx = (cube1.x + cube1.l);
    float offsety = (cube1.y + cube1.h);
    float offsetz = (cube1.z + cube1.w);
    
    
    /// Defining Verticies
    
    float vertices[][3] = {{cube1.x, cube1.y, cube1.z},
        {offsetx, cube1.y, cube1.z},
        {cube1.x, offsety, cube1.z},
        {offsetx, offsety, cube1.z},
        {cube1.x, cube1.y, offsetz},
        {offsetx, cube1.y, offsetz},
        {cube1.x, offsety, offsetz},
        {offsetx, offsety, offsetz}};
    
    glBegin(GL_TRIANGLES);

        // Each vertex has an (R,G,B) color and (x,y) position
        glColor3f(1.0f, 0.0f, 0.0f);   // Red
        glVertex2f(tdpx(vertices[0][0] - cx, vertices[0][2] - cz, fov), tdpy(vertices[0][0] - cx ,vertices[0][1] - cy, vertices[0][2] - cz, fov));

        glColor3f(0.0f, 1.0f, 0.0f);   // Green
        glVertex2f(tdpx(vertices[1][0] - cx, vertices[1][2] - cz, fov), tdpy(vertices[1][0] - cx, vertices[1][1] - cy, vertices[1][2] - cz, fov));

        glColor3f(0.0f, 0.0f, 1.0f);   // Blue
        glVertex2f(tdpx(vertices[2][0] - cx, vertices[2][2] - cz, fov), tdpy(vertices[2][0] - cx, vertices[2][1] - cy, vertices[2][2] - cz, fov));

    glEnd();
    
    glutKeyboardFunc(handleKeys);
    
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("ProtonEngine Alpha v0.0");
    
    glutDisplayFunc(display);
    
    /// Getting the Width and Height of the window
    
    int width  = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    /// Defining the aspect ratio and fov for 3d calulations
    
    double aspectratio = (width / height);
    double fov = 1.2;
    double f = (1 / tan(fov/2));
    
    
    glutMainLoop();

    return 0;
}
