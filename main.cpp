#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SOIL.h>
#include <GL/freeglut.h>
#include "libs/callback.h"
#include "libs/physics.h"
#include "libs/parser.h"

//variaveis globais
Sistema_solar *planetario;

void init_sistemaSolar(){

    planetario = new Sistema_solar();

    planetario->criar_sol("sol.txt");
    planetario->add_planeta("terra.txt");
    //adicionar manualmente os planetas KEKW
}

void drawUpdate()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glEnable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void onTimeUpdate(int time)
{
    // Draws everything <3
    glutPostRedisplay();

    // Restarts the timerFunc
    glutTimerFunc(time, onTimeUpdate, time);
}

void configGlut()
{
    // Configura a janela da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600 * 1/razaoAspecto);

    glutInitWindowPosition(0, 0);
    glutCreateWindow("Solar System");

    // Configura os callbacks da GLUT
    glutDisplayFunc(drawUpdate);
    glutReshapeFunc(reshapeFct);
    glutKeyboardFunc(keyboardFct);
    glutKeyboardUpFunc(keyboardFct);
    glutSpecialFunc(keyboardSpecial);
    glutSpecialUpFunc(keyboardSpecial);

    glutTimerFunc(16, onTimeUpdate, 16);

    glLineWidth(8);
    glClearColor(1, 1, 1, 1);

    // Ignora repetições de teclas e verifica apenas o pressionamento e qnd soltar
    glutIgnoreKeyRepeat(1);
}

//usar a taxa de variacao de posicao dos pontos do mouse para rotacionar o plauyer lookat
//w a s d para mexer as posicoes da glFrustum ou glperspective(melhor perspective)
//



int main(int argc, char **argv)
{
    

    glutInit(&argc, argv);

    configGlut();

    glewInit();

    init_sistemaSolar();

    glutDisplayFunc(drawUpdate);
    glutMainLoop();

}