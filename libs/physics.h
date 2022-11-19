#ifndef __PHYSICS__
#define __PHYSICS__
#include <iostream>
#include <vector>
#include <memory>
#include <GL/freeglut.h>
#include "callback.h"
#include "parser.h"


class Sistema_solar
{ 
    private:
        //não contem o sol dentro da listas dos astros
        std::vector<std::shared_ptr<Astros>> planetas;
        std::shared_ptr<Sol> sol;
        float time;

    public:
        //construtor
        Sistema_solar();

        //metodos
        void atualiza_sistema();
        void desenhar_sistema();
        void ctrl_ilum_sol();
        void add_planeta(const char * nome);
        void criar_sol(const char * nome);

};

class Astros{

    private:
        // atributos do planeta
        GLfloat raio_Astro;
        GLfloat raio_Astro_ao_sol;
        GLfloat material;
        GLint codigo_textura;
        //atributos em relação a translacao e rotacao do planeta
        // pos0=x||pos1=y||posz=2
        std::vector<GLfloat> vec_pos;
        //pos0=velocidade translacao||pos1=velocidade rotacao
        std::vector<GLfloat> vec_velo;
        //pos0=theta(x,y)||pos1=fi(z,(xy))
        std::vector<GLfloat> vec_angulo;
        std::vector<std::shared_ptr<Luas>> luas;


    public:
        std::vector<GLfloat> get_posicao();
        //construtor
        Astros(const char * script);
        Astros(inf_astros info_astro);
        void atualiza_posicao();
        void desenhar_Astro();
    // transladar e rotacionar em reação ao sol
    //atualiza(pos_sol)
        //atualizar a posicao nova em relação ao sol

    //atualiza luas(pos_do_planeta)
        // ORDEM DE FAZER ATUALIZAÇÃO
        // transladar o planeta em relação ao sol
        // rotacionar o planeta;
        //passar posição nova do planeta para as luas;
        //transladar a lua para a nova posição do planeta(corrigir a posição)
        // agora transladar em relação ao planeta
        //rotacionar a lua 



};


class Sol{

    private:
        // atributos do planeta
        GLfloat raio_Astro;
        GLfloat material;
        GLfloat ilum_uniforme;
        GLfloat ilum_difusa;
        GLfloat ilum_especular;
        GLint codigo_textura;
        //atributos em relação a translacao e rotacao do planeta
        // pos0=x||pos1=y||posz=2
        std::vector<GLfloat> vec_pos;
        //pos0=velocidade translacao||pos1=velocidade rotacao
        std::vector<GLfloat> vec_velo;
        //pos0=theta(x,y)||pos1=fi(z,(xy))
        std::vector<GLfloat> vec_angulo;

    public:
        std::vector<GLfloat> get_posicao();
        void set_ilumincao();
        //construtor
        Sol(const char * script);
        void atualiza_sol();
        void desenhar_sol();

};

class Luas: Astros{

    public:
    // transladar e rotacionar em reação ao sol
        Luas(inf_astros inf_lua);

        //depois dar overrite no metodo atualiza posicao
        //
};


//inspirado no codigo do coutinho KEKW
void solidSphere(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
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





#endif