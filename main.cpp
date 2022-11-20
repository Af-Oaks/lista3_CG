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

    planetario->criar_sol("assets/scripts/Sol.txt");
    planetario->add_planeta("assets/scripts/Terra.txt");//ao adicionar os planetas automaticamente será adicionado as luas deles
    //adicionar manualmente os planetas KEKW
}

void drawUpdate()
{

    glClear(GL_COLOR_BUFFER_BIT);

    //o que tive de textura será habilitado aqui
    //desenha tudo em sistema de recurssao
    planetario->desenhar_sistema();
    //desabilita as texturas para não interferir nos proximos ciclos

    glutSwapBuffers();
}

void onTimeUpdate(int time)
{

    //sistema sera atualizado
    planetario->atualiza_sistema();

    // Draws everything <3
    glutPostRedisplay();

    // Restarts the timerFunc
    glutTimerFunc(time, onTimeUpdate, time);
}

void configGlut()
{
    // Configura a janela da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600 * 1/razaoAspecto);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Solar System");

    //atulizando o a cord do background
    glClearColor(0,0,0, 0.0);
    glEnable(GL_DEPTH_TEST);  // Ativa teste Z



    // Não mostrar faces do lado de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Esconder o ponteiro do mouse quando dentro da janela
    glutSetCursor(GLUT_CURSOR_NONE);

    // Configura os callbacks da GLUT
    glutDisplayFunc(drawUpdate);
    glutReshapeFunc(reshapeFct);
    glutKeyboardFunc(keyboardFct);
    glutKeyboardUpFunc(keyboardFct);
    glutSpecialFunc(keyboardSpecial);
    glutSpecialUpFunc(keyboardSpecial);
    glutPassiveMotionFunc(posicionaCamera);
    // Ignora repetições de teclas e verifica apenas o pressionamento e qnd soltar
    glutIgnoreKeyRepeat(1);
    //configura para ser chamada a cada 16ms=60frames/sec
    glutTimerFunc(16, onTimeUpdate, 16);
}

//usar a taxa de variacao de posicao dos pontos do mouse para rotacionar o plauyer lookat
//w a s d para mexer as posicoes da glFrustum ou glperspective(melhor perspective)

int main(int argc, char **argv)
{
    

    glutInit(&argc, argv);

    configGlut();

    glewInit();
    //inicializa o sistema solar
    init_sistemaSolar();

    glutDisplayFunc(drawUpdate);
    glutMainLoop();

}