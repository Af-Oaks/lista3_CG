#include "physics.h"

//Classe sistema solar
//----------------------------------------------------------------------------//
Sistema_solar::Sistema_solar()
{ 
    float time =0.0;
    space_texture = SOIL_load_OGL_texture("assets/texture/stars_milky_way.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);
    if (space_texture == 0)
    {
        printf("Erro do SOIL space_texture: '%s'\n", SOIL_last_result());
    }
    raio_sistema = 500.0;

};

//faz atualiza todo mundo de forma recursiva!
//os planetas que tiverem luas, serao chamados por aqui também
void Sistema_solar::atualiza_sistema(){

    this->sol->atualiza_sol();

    for(auto  i: planetas){

        i->atualiza_posicao();
    }

}

//chama todo mundo dos vetores e o sol para ser desenhada na tela
void Sistema_solar::desenhar_sistema(){

    float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
    //printf("desenho_sts 01?\n");
    //printf("desenho_sts 02?\n");
    //configura sistema de luz global//ver se da para fazer apenas uma vez
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, false); // habilita local view

    // Limpa a tela e o z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    //printf("desenho_sts 03?\n");
    // Posiciona a câmera de acordo com posição x,y do mouse na janela
    gluLookAt(1*(xMouse-prev_wh/2)/(prev_ww/16), -1*(yMouse-prev_wh/2)/(prev_ww/16) + 3, 5,
              0, 0, 0,
              0, 1, 0);

    //agora desenhar os pontos de luzes
    //printf("desenho_sts 04?\n");
    this->sol->desenhar_sol();
    // Desabilita iluminação para desenhar os planetas(nao emitem luz!)
    //printf("desenho_sts 05?\n");
    for(auto  i: planetas){

        i->desenhar_Astro();
    }
    //printf("desenho_sts 06?\n");


}

void Sistema_solar::criar_sol(const char * nome){

    this->sol = std::make_shared<Sol>(nome);

}

void Sistema_solar::add_planeta(const char * nome){

    planetas.push_back(std::make_shared<Astros>(nome));

}

// controla a luz do sol
void Sistema_solar::ctrl_ilum_sol(){

// keys
    this->sol->set_ilumincao();
}

//----------------------------------------------------------------------------//
//Classe Astros
Astros::Astros(const char * script){
    //struct de informações para carregar os atributos do Astro
    inf_astros aux={0};
    if (!parse_model(&aux,script,&num_luas))
    {
        printf("DEU BOM NO FILE ASTRO \n");
    }
    this->codigo_textura =aux.codigo_textura;
    this->material = aux.material;
    this->raio_Astro = aux.raio_Astro;
    this->raio_Astro_ao_sol = aux.raio_Astro_ao_sol;
    this->vec_velo =aux.vec_velo;
    //      pos_x,                            pos_y,                                 pos_z
    this->vec_pos.push_back(0.0);this->vec_pos.push_back(raio_Astro_ao_sol);this->vec_pos.push_back(0.0);
    printf("num luas %d\n",num_luas);
    if(num_luas!= 0){
        std::vector<informacoes_astros> aux_luas;
        std::string input_caminho ="Luas_";
        input_caminho.append(script);
        if(!parse_lua(&aux_luas,script,num_luas))
        {
        printf("DEU BOM NO FILE lUAS \n");
        }

        for(auto i:aux_luas){

            luas.push_back(std::make_shared<Luas>(i));
        }
    }

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

    ilum_difusa=1.0;
    ilum_especular=1.0;
    ilum_shine=30.0;
    ilum_uniforme=1.0;
    // Propriedades do material da esfera
    float carryAmb[] = {1.0, 1.0, 0.0, ilum_uniforme};    // cor ambiente : amarela
    float carrySpec[]= { 1.0, 1.0, 0.0,  ilum_especular }; //cor difusa: amarela
    float carryDif[] = {1.0, 1.0, 0.0, ilum_difusa};       // cor especular: amarela
    float carryshine[] = { ilum_shine }; 

    matAmb = carryAmb;  // jogando para dentro do array do sol
    matSpec = carrySpec; // jogando para dentro do array do sol
    matDif =  carryDif;  // jogando para dentro do array do sol
    matshine =  carryshine;  // jogando para dentro do array do sol

    // Propriedades da fonte de luz LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, matAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, matDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, matSpec);

    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matshine);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   //struct de informações para carregar os atributos do Astro
    inf_astros aux={0};
    int num=0;
    if (!parse_model(&aux,script,&num))
    {
        printf("DEU BOM NO FILE SOL \n");
    }
    this->codigo_textura =aux.codigo_textura;
    this->material = aux.material;
    this->raio_Astro = aux.raio_Astro;
    this->vec_velo =aux.vec_velo;
    this->vec_angulo.push_back(0.0);
    this->vec_angulo.push_back(0.0);
    //vetor posicao
    this->vec_pos.push_back(0.0);//pos_x
    this->vec_pos.push_back(0.0);//pos_y
    this->vec_pos.push_back(0.0);//pos_z
    this->vec_pos.push_back(1.0);//luz é posicional
}

void Sol::set_ilumincao(){
    //aumenta luz uniforme
    if(keyboard.z && ilum_uniforme<=1.0)
        ilum_uniforme+=0.0001*(float)keyboard.z;
    //diminui  luz uniforme
    if(keyboard.x && ilum_uniforme>=0.0)
        ilum_uniforme-=0.0001*(float)keyboard.z;
    //aumenta luz difusa
    if(keyboard.c && ilum_difusa<=1.0)
        ilum_difusa+=0.0001*(float)keyboard.c;
    //diminui  luz difusa
    if(keyboard.v && ilum_difusa>=0.0)
        ilum_difusa-=0.0001*(float)keyboard.v;
    //aumenta luz especular
    if(keyboard.b && ilum_especular<=1.0)
        ilum_especular+=0.0001*(float)keyboard.b;
    //diminui luz especular
    if(keyboard.b && ilum_especular>=0.0)
        ilum_especular-=0.0001*(float)keyboard.n;

    //atulizando os vetores agora;
    matAmb[4] = ilum_uniforme;  // jogando para dentro do array do sol
    matSpec[4] = ilum_especular; // jogando para dentro do array do sol
    matDif[4] =  ilum_difusa;  // jogando para dentro do array do sol
    matshine[4] =  ilum_shine;  // jogando para dentro do array do sol   


}

void Sol::atualiza_sol(){


}
void Sol::desenhar_sol(){

    //botando iluminação
    //printf("errro luza?\n");
    // Propriedades da fonte de luz LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, matAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, matDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, matSpec);

    glEnable(GL_LIGHT0);
    //printf("errro luzb?\n");
    //printf("errro luzc?\n");
    glPushMatrix();
        glRotatef(vec_angulo[1], 1.0, 0.0, 0.0); // Rotação no eixo x
        glRotatef(vec_angulo[1], 0.0, 1.0, 0.0); // Rotação no eixo y
        //printf("errro luz0?\n");
        glLightfv(GL_LIGHT0, GL_POSITION, vec_pos.data());
        //printf("errro luz1?\n");
        glTranslatef(vec_pos[0], vec_pos[1], vec_pos[2]);
        glColor3f(1.0, 1.0, 0.0); // coutinho???? usar color??? para luz???
    glPopMatrix();
    
    //desenhando a esfera
    // Define (atualiza) o valor do expoente de especularidade
    glMaterialfv(GL_FRONT, GL_SHININESS, matshine);
    glColor3f(1, 1, 1);// color dnv coutinho??

    // Desenha a esfera grande e bem arredondada
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, codigo_textura);  
    glPushMatrix();
        glRotatef(vec_angulo[1], 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(raio_Astro, 160, 160);
        // glutSolidSphere(1.5, esferaLados, esferaLados);
        // glutSolidCube(1.5);
        // glutSolidTeapot(1.5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    printf("errro espehera?2?\n");
}

float* Sol::get_ilumi_amb(){

    return matAmb;
}
float* Sol::get_ilumi_dif(){

    return matDif;
}
float* Sol::get_ilumi_spec(){

    return matSpec;
}
float* Sol::get_ilumi_shine(){

    return matshine;
}

//----------------------------------------------------------------------------//
// funcoes para ajudar a desenhar e afins
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


