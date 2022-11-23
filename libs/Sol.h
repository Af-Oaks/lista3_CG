#ifndef __SOL__
#define __SOL__

#include <memory>
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include "parser.h"
#include "Astros.h"
#include "callback.h"
class Sol{

    private:
        // atributos do planeta
        GLfloat raio_Astro;
        GLfloat material;
        GLfloat ilum_uniforme;
        GLfloat ilum_difusa;
        GLfloat ilum_especular;
        GLfloat ilum_shine;
        GLint codigo_textura;
        GLuint n_callist;
        //atributos em relação a translacao e rotacao do planeta
        // pos0=x||pos1=y||posz=2
        std::vector<GLfloat> vec_pos;
        //pos0=velocidade translacao||pos1=velocidade rotacao
        std::vector<GLfloat> vec_velo;
        //pos0=theta(x,y)||pos1=fi(z,(xy))
        std::vector<GLfloat> vec_angulo;

    public:
        std::vector<GLfloat> get_posicao();
        //construtor
        Sol(const char * script,std::vector<std::pair<std::string,GLint>> vec_texture);
        //metodos
        void atualiza_sol();
        void desenhar_sol();
        void set_iluminacao();
        std::vector<float> ilum_variables();

};


#endif