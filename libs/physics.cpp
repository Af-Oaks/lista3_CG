#include "physics.h"

//Classe sistema solar
//----------------------------------------------------------------------------//
Sistema_solar::Sistema_solar()
{ 
    float time =0.0;


};


void Sistema_solar::atualiza_sistema(){

    sol->atualiza_sol();

    for(auto i: planetas){

        i->atualiza_posicao();
    }

}

//chama todo mundo dos vetores e o sol para ser desenhada na tela
void Sistema_solar::desenhar_sistema(){

    sol->desenhar_sol();

    for(auto i: planetas){

        i->desenhar_Astro();
    }



}

void Sistema_solar::criar_sol(const char * nome){

    sol = std::make_shared<Sol>(nome);

}

void Sistema_solar::add_planeta(const char * nome){

    planetas.push_back(std::make_shared<Astros>(nome));

}

// controla a luz do sol
void Sistema_solar::ctrl_ilum_sol(){

// keys
    sol->set_ilumincao();

}

//----------------------------------------------------------------------------//
//Classe Astros
Astros::Astros(const char * script){

    //struct de informações para carregar os atributos do Astro
    inf_astros aux={0};
    parse_model(&aux,script,&luas);
    this->codigo_textura =aux.codigo_textura;
    this->material = aux.material;
    this->raio_Astro = aux.raio_Astro;
    this->raio_Astro_ao_sol = aux.raio_Astro_ao_sol;
    this->vec_velo =aux.vec_velo;

    std::vector<GLfloat> pos;
    //      pos_x,                    pos_y,                    pos_z
    pos.push_back(0.0);pos.push_back(raio_Astro_ao_sol);pos.push_back(0.0);

}
Astros::Astros(inf_astros info_astro){
    
    this->codigo_textura =info_astro.codigo_textura;
    this->material = info_astro.material;
    this->raio_Astro = info_astro.raio_Astro;
    this->raio_Astro_ao_sol = info_astro.raio_Astro_ao_sol;
    this->vec_velo =info_astro.vec_velo;

    std::vector<GLfloat> pos;
    //      pos_x,                    pos_y,                    pos_z
    pos.push_back(0.0);pos.push_back(raio_Astro_ao_sol);pos.push_back(0.0);
}

void Astros::atualiza_posicao(){

}

void Astros::desenhar_Astro(){


    // calma 1
        

    //calma 1
    
}

//----------------------------------------------------------------------------//

//Classe  Luas
Luas::Luas(inf_astros inf_lua) :Astros(inf_lua){
    

}

//----------------------------------------------------------------------------//

//Classe Sol
Sol::Sol(const char * script){


}

void Sol::set_ilumincao(){
    //aumenta luz uniforme
    if(keyboard.z && ilum_uniforme<=1.0)
        ilum_uniforme+=0.01*(float)keyboard.z;
    //diminui  luz uniforme
    if(keyboard.x && ilum_uniforme>=0.0)
        ilum_uniforme-=0.01*(float)keyboard.z;
   
    //aumenta luz difusa
    if(keyboard.c && ilum_difusa<=1.0)
        ilum_difusa+=0.01*(float)keyboard.c;
    //diminui  luz difusa
    if(keyboard.v && ilum_difusa>=0.0)
        ilum_difusa-=0.01*(float)keyboard.v;

    //aumenta luz especular
    if(keyboard.b && ilum_especular<=1.0)
        ilum_especular+=0.01*(float)keyboard.b;

    //diminui luz especular
    if(keyboard.b && ilum_especular>=0.0)
        ilum_especular-=0.01*(float)keyboard.n;

}

//----------------------------------------------------------------------------//

