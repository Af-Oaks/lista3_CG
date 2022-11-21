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

//KEKW debug do coutinho
static long font = (long)GLUT_BITMAP_8_BY_13;   // Fonte usada para imprimir na tela
static char theStringBuffer[10];                // String buffer
void informacoesTela(void);
void escreveTextoNaTela(void *font, char *string);
void floatParaString(char * destStr, int precision, float val);


void init_sistemaSolar(){

    planetario = new Sistema_solar();

    planetario->criar_sol("assets/scripts/Sol.txt");
    planetario->add_planeta("assets/scripts/Terra.txt");//ao adicionar os planetas automaticamente será adicionado as luas deles
    //adicionar manualmente os planetas KEKW
}

void drawUpdate()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
    //printf("desenho_sts 01?\n");
    //printf("desenho_sts 02?\n");
    //configura sistema de luz global//ver se da para fazer apenas uma vez
    glEnable(GL_DEPTH_TEST);  // Ativa teste Z

    //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, false);  
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, false); // habilita local view
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glEnable(GL_COLOR_MATERIAL); 
    // Propriedades do material da esfera
    float matAmbAndDif[] = {1.0, 1.0, 1.0, 1.0};    // cor ambiente e difusa: branca
    float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };       // cor especular: branca
    float matShine[] = { 1.0}; 
    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


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

    glewInit();    

    glutInit(&argc, argv);

    configGlut();

    //inicializa o sistema solar
    init_sistemaSolar();

    glutDisplayFunc(drawUpdate);
    
    glutMainLoop();

}
/*
// Escreve uma cadeia de caracteres
void escreveTextoNaTela(void *font, char *string)
{
    char *c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Converte um número decimal em string
void floatParaString(char * destStr, int precision, float val)
{
    sprintf(destStr,"%f",val);
    destStr[precision] = '\0';
}

// Escreve as informações variáveis na tela
void informacoesTela(void)
{
    glDisable(GL_LIGHTING); // Desabilita iluminação
    glColor3f(.85f, .85f, .85f);

    glRasterPos3f(-1.0, 1.10, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Iluminacao (l): ");
    escreveTextoNaTela((void*)font, (char*)(isLightingOn ? "ligada" : "desligada"));

    floatParaString(theStringBuffer, 4, m);
    glRasterPos3f(-1.0, 1.00, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Luz ambiente global: ");
    glRasterPos3f(-1.0, 0.95, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade (m/M): ");
    escreveTextoNaTela((void*)font, theStringBuffer);

    glRasterPos3f(-1.0, 0.85, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Luz branca: ");
    glRasterPos3f(-1.0, 0.80, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade difusa (d/D): ");
    floatParaString(theStringBuffer, 4, d);
    escreveTextoNaTela((void*)font, theStringBuffer);
    glRasterPos3f(-1.0, 0.75, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade especular (e/E): ");
    floatParaString(theStringBuffer, 4, e);
    escreveTextoNaTela((void*)font, theStringBuffer);


    glRasterPos3f(-1.0, 0.65, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Material: ");
    glRasterPos3f(-1.0, 0.60, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Expoente especular (s/S): ");
    floatParaString(theStringBuffer, 5, s);
    escreveTextoNaTela((void*)font, theStringBuffer);

}
*/