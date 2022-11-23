#ifndef __PHYSICS__
#define __PHYSICS__
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <cstring>
#include <GL/freeglut.h>
#include "callback.h"
#include "parser.h"
#include "Astros.h"
#include "Sol.h"


typedef struct movee{

    float x;
    float y;
    float z;

} move;

class Sistema_solar
{ 
    private:
        //não contem o sol dentro da listas dos astros
        std::vector<std::shared_ptr<Astros>> planetas;
        std::shared_ptr<Sol> sol;
        float time;
        GLint space_texture;
        GLfloat raio_sistema;
        GLuint n_callist;
        std::vector<std::pair<std::string,GLint>> vec_texture;

    public:
        //construtor
        Sistema_solar();

        //metodos
        void atualiza_sistema();
        void desenhar_sistema();
        void ctrl_ilum_sol();
        void add_planeta(const char * nome);
        void criar_sol(const char * nome);
        void carregar_texture();

};


//inspirado no codigo do coutinho KEKW
void solidSphere(int radius, int stacks, int columns);
void solidSphere_inside(int radius, int stacks, int columns);
//KEKW debug do coutinho
// Escreve as informações variáveis na tela nessa ordem:ambiente,difusa,especular,shine
void informacoesTela(float m, float d, float e,float s);
void escreveTextoNaTela(void *font, char *string);
void floatParaString(char * destStr, int precision, float val);
void setupCamera(move focus,move midPoint);
void movet(move focus,move midPoint);

#endif