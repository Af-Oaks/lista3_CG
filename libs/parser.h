#ifndef __PARSER__
#define __PARSER__

// #define__DEBUG__

#ifdef __DEBUG__
#include <stdio.h>
#endif

#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <GL/freeglut.h>
#include <SOIL.h>

typedef struct informacoes_astros{

    GLfloat raio_Astro;
    GLfloat raio_Astro_ao_sol;
    GLfloat material;
    GLint codigo_textura;
    std::vector<GLfloat> vec_velo;

} inf_astros;


int parse_model(inf_astros *aux,const char *file_name,int *num_luas);

int parse_lua(std::vector<inf_astros> *aux,const char *file_name,int num_luas);



#endif