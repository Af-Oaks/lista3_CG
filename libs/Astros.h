#ifndef __ASTROS__
#define __ASTROS__

#include <memory>
#include <GL/freeglut.h>
#include <vector>
#include "parser.h"
#include <cstring>
#include <cmath>

class Astros;
class Luas;

class Astros{

    protected:
        // atributos do planeta
        GLfloat raio_Astro;
        GLfloat raio_Astro_ao_sol;
        GLfloat material;
        GLint codigo_textura;
        std::string nome_textura;
        int num_luas;
        GLuint n_callist;
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
        Astros(const char * script,std::vector<std::pair<std::string,GLint>> vec_texture);
        Astros(inf_astros info_astro,std::vector<std::pair<std::string,GLint>> vec_texture);
        void atualiza_posicao();
        void desenhar_Astro(float shine);

};

class Luas: public Astros{

    public:
        using Astros::Astros;
    // transladar e rotacionar em reação ao sol
        Luas(inf_astros inf_lua,std::vector<std::pair<std::string,GLint>> vec_texture);
        //depois dar overrite no metodo atualiza posicao ?
        void desenhar_Lua(float shine);
        void atualiza_lua(std::vector<GLfloat> astro_ref);
};


void solidSphere(int radius, int stacks, int columns);

#endif