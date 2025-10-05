//
//  main.cpp
//  Proton Engine
//
//  Created by VibingCatt on 10/5/25.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#define GL_SILENCE_DEPRECATION

// 1. Display callback function
void display() {
    // Set the background color (R, G, B, Alpha)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Clear the color buffer with the set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Flush the OpenGL commands to the screen
    glFlush();
}

int main(int argc, char** argv) {
    // 2. Initialize GLUT
    glutInit(&argc, argv);

    // 3. Set display mode (single buffer and RGB color)
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // 4. Set initial window position (x=100, y=100)
    glutInitWindowPosition(100, 100);

    // 5. Set initial window size (width=800, height=600)
    glutInitWindowSize(800, 600);

    // 6. Create the window with a title
    glutCreateWindow("My GLUT OpenGL Window");

    // 7. Register the display callback function
    glutDisplayFunc(display);

    // 8. Enter the GLUT event processing loop
    glutMainLoop();

    return 0; // We never actually reach this point
}
