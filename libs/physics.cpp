#include "physics.h"

static long font = (long)GLUT_BITMAP_8_BY_13;   // Fonte usada para imprimir na tela
static char theStringBuffer[10];                // String buffer

//Classe sistema solar
//----------------------------------------------------------------------------//
Sistema_solar::Sistema_solar()
{ 
    float time =0.0;
    space_texture = SOIL_load_OGL_texture("assets/texture/stars_milky_way.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (space_texture == 0)
    {
        printf("Erro do SOIL space_texture: '%s'\n", SOIL_last_result());
    }
    raio_sistema = 1500.0;

    n_callist = glGenLists(1);
    glNewList(n_callist, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, space_texture);  
            solidSphere(raio_sistema, 160, 160);
            glDisable(GL_TEXTURE_2D);   
        glPopMatrix();
    glEndList();

    // paramentros de inicialização do desenho

};

//faz atualiza todo mundo de forma recursiva!
//os planetas que tiverem luas, serao chamados por aqui também
void Sistema_solar::atualiza_sistema(){

    this->sol->atualiza_sol();
    this->ctrl_ilum_sol();
    for(auto  i: planetas){

        i->atualiza_posicao();
    }

}

//chama todo mundo dos vetores e o sol para ser desenhada na tela
void Sistema_solar::desenhar_sistema(){

    // Limpa a tela e o z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHT0);
    glLoadIdentity();
    //printf("desenho_sts 03?\n");
    //desenha informações na tela!

    // desenhando o background
    // Desenha a esfera grande e bem arredondada
    // Posiciona a câmera de acordo com posição x,y do mouse na janela  
    //informacoesTela(sol->ilum_variables()[0],sol->ilum_variables()[1], sol->ilum_variables()[2],sol->ilum_variables()[3]);   
    gluLookAt(500, 500, 5,
              0, 0, 0,
              0, 1, 0);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
        glCallList(n_callist);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    informacoesTela(sol->ilum_variables()[0],sol->ilum_variables()[1], sol->ilum_variables()[2],sol->ilum_variables()[3]);   

    //agora desenhar os pontos de luzes
    //printf("desenho_sts 04?\n");
    this->sol->desenhar_sol();

    // Desabilita iluminação para desenhar os planetas(nao emitem luz!)
    //printf("desenho_sts 05?\n");
    glEnable(GL_LIGHTING);
    for(auto  i: planetas){

        i->desenhar_Astro(sol->ilum_variables()[3]);
        //printf("desenho_sts dentro?\n");
    }

    //printf("desenho_sts 06?\n");

    glDisable(GL_LIGHTING);


}

void Sistema_solar::criar_sol(const char * nome){

    this->sol = std::make_shared<Sol>(nome,vec_texture);

}

void Sistema_solar::add_planeta(const char * nome){

    planetas.push_back(std::make_shared<Astros>(nome,vec_texture));

}

void Sistema_solar::carregar_texture(){

    //0=sol,1=mercu,2=venus,3=terra,4=mart,5=jupit,6=saturn,7=uranus,8=neutuno,9=moon,10=lua1,11=lua2;
    parse_texture(&vec_texture,"assets/scripts/Texturas.txt");

}
void Sistema_solar::ctrl_ilum_sol(){

// keys
    this->sol->set_iluminacao();
}

//----------------------------------------------------------------------------//
// funcoes para ajudar a desenhar e afins

void solidSphere_inside(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    //gluQuadricOrientation(quadObj,GLU_INSIDE);
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);

}

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

// Escreve as informações variáveis na tela nessa ordem:ambiente,difusa,especular,shine
void informacoesTela(float m, float d, float e,float s)
{
    glDisable(GL_LIGHTING); // Desabilita iluminação

    floatParaString(theStringBuffer, 4, m);
    glRasterPos3f(-300*razaoAspecto, 475, 850.0);
    escreveTextoNaTela((void*)font, (char*)"Luz ambiente global: ");
    glRasterPos3f(-275*razaoAspecto, 450, 825.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade (Z/X): ");
    escreveTextoNaTela((void*)font, theStringBuffer);

    glRasterPos3f(-250*razaoAspecto, 425, 800.0);
    escreveTextoNaTela((void*)font, (char*)"Luz branca: ");
    glRasterPos3f(-225*razaoAspecto, 400, 750.0);

    escreveTextoNaTela((void*)font, (char*)"  - Intensidade difusa (C/V): ");
    floatParaString(theStringBuffer, 4, d);
    escreveTextoNaTela((void*)font, theStringBuffer);

    glRasterPos3f(-200*razaoAspecto, 375, 750.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade especular (B/N): ");
    floatParaString(theStringBuffer, 4, e);
    escreveTextoNaTela((void*)font, theStringBuffer);

    glRasterPos3f(-175*razaoAspecto, 350, 750.0);
    escreveTextoNaTela((void*)font, (char*)"Material: ");
    glRasterPos3f(-150*razaoAspecto, 325, 750.0);
    escreveTextoNaTela((void*)font, (char*)"  - Expoente shineness (W/A): ");
    floatParaString(theStringBuffer, 5, s);
    escreveTextoNaTela((void*)font, theStringBuffer);

}
