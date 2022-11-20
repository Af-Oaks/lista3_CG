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
    raio_sistema = 1500.0;

    // paramentros de inicialização do desenho
    float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
    //printf("desenho_sts 01?\n");
    //printf("desenho_sts 02?\n");
    //configura sistema de luz global//ver se da para fazer apenas uma vez
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);  
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, false); // habilita local view
    glEnable(GL_COLOR_MATERIAL); 

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

    glLoadIdentity();
    //printf("desenho_sts 03?\n");
    // Posiciona a câmera de acordo com posição x,y do mouse na janela
    gluLookAt(1*(xMouse-prev_wh/2)/(prev_ww/16), -1*(yMouse-prev_wh/2)/(prev_ww/16) + 3, 5,
              0, 0, 0,
              0, 1, 0);

    //agora desenhar os pontos de luzes
    //printf("desenho_sts 04?\n");
    glEnable(GL_LIGHT0);
    this->sol->desenhar_sol();
    glEnable(GL_LIGHTING);
    // Desabilita iluminação para desenhar os planetas(nao emitem luz!)
    //printf("desenho_sts 05?\n");


    for(auto  i: planetas){

        i->desenhar_Astro();
        //printf("desenho_sts dentro?\n");
    }
    glDisable(GL_LIGHTING);

    // desenhando o background
    // Desenha a esfera grande e bem arredondada
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glLoadIdentity(); 
        solidSphere_inside(raio_sistema, 160, 160);
        glBindTexture(GL_TEXTURE_2D, space_texture); 
        glColor3f(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
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
    this->vec_pos.push_back(raio_Astro_ao_sol);this->vec_pos.push_back(0.0);this->vec_pos.push_back(0.0);
    printf("num luas %d\n",num_luas);
    if(num_luas!= 0){
        std::vector<informacoes_astros> aux_luas;
        std::string input_caminho ="";
        input_caminho.append(script);
        input_caminho.insert(15,"Luas_");
        //printf("nome para a lua  111= %s \n",input_caminho.c_str());
        if(!parse_lua(&aux_luas,input_caminho.c_str(),num_luas))
        {
        printf("DEU BOM NO FILE lUAS \n");
        }
        vec_angulo.push_back(0.0);vec_angulo.push_back(0.0);
        //printf("raio lua em relação aaa %0.2f \n",aux_luas[0].raio_Astro_ao_sol);
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
    vec_angulo.push_back(0.0);vec_angulo.push_back(0.0);
    //      pos_x,                    pos_y,                    pos_z
    printf("raio da lua em relação ao sol! %0.2f \n",raio_Astro_ao_sol);
    vec_pos.push_back(raio_Astro_ao_sol);vec_pos.push_back(0.0);vec_pos.push_back(0.0);
}

void Astros::atualiza_posicao(){

    if(vec_angulo[1]<360.0)// rotação em torno do seu proprio eixo
        vec_angulo[1]+=0.8;
    else
        vec_angulo[1]=0.0;

    if(vec_angulo[0]<360.0)//rotação em relação ao seu astro
        vec_angulo[0]+=0.05;
    else
        vec_angulo[0]=0.0;

    //atualizar via cordenadas polares

    vec_pos[0]=raio_Astro_ao_sol*cos(vec_angulo[0]);
    vec_pos[2]=raio_Astro_ao_sol*sin(vec_angulo[0]);
    

    //atualizar cada uma de suas luas
    for(auto i: luas){

        i->atualiza_lua(vec_pos);
    }

}

void Astros::desenhar_Astro(){


    // calma 1
    glMaterialfv(GL_FRONT, GL_SHININESS, &material);
    //glColor3f(1, 1, 1);// color dnv coutinho??

    // Desenha a esfera grande e bem arredondada
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, codigo_textura);  
    glPushMatrix();
        glTranslatef(vec_pos[0],vec_pos[1],vec_pos[2]);
        glRotatef(vec_angulo[1], 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(raio_Astro, 160, 160);
        // glutSolidSphere(1.5, esferaLados, esferaLados);
        // glutSolidCube(1.5);
        // glutSolidTeapot(1.5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);   

    //calma 1
    
    for(auto i : luas){

        i->desenhar_Lua();
    }
    
}

//----------------------------------------------------------------------------//

//Classe  Luas
Luas::Luas(inf_astros inf_lua) :Astros(inf_lua){


}

void Luas::desenhar_Lua(){

    glMaterialfv(GL_FRONT, GL_SHININESS, &material);
    //glColor3f(1, 1, 1);// color dnv coutinho??

    // Desenha a esfera grande e bem arredondada
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, codigo_textura);  
    glPushMatrix();
        glTranslatef(vec_pos[0],vec_pos[1],vec_pos[2]);
        glRotatef(vec_angulo[1], 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(raio_Astro, 160, 160);
        // glutSolidSphere(1.5, esferaLados, esferaLados);
        // glutSolidCube(1.5);
        // glutSolidTeapot(1.5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //printf("desenho_sts LUAS?\n");
}

void Luas::atualiza_lua(std::vector<GLfloat> astro_ref){

    if(vec_angulo[1]<360.0)//rotacao no proprio eixo
        vec_angulo[1]+=0.8;
    else
        vec_angulo[1]=0.0;

    if(vec_angulo[0]<360.0)//rotacao em relação ao seu astro
        vec_angulo[0]+=0.06;
    else
        vec_angulo[0]=0.0;

    vec_pos[0]=astro_ref[0] + raio_Astro_ao_sol*cos(vec_angulo[0]);
    vec_pos[2]=astro_ref[2] + raio_Astro_ao_sol*sin(vec_angulo[0]);

    //atualizar via cordenadas polares

}
//----------------------------------------------------------------------------//

//Classe Sol
Sol::Sol(const char * script){

    ilum_difusa=1.0;
    ilum_especular=1.0;
    ilum_shine=30.0;
    ilum_uniforme=1.0;
    // Propriedades do material da esfera
    float carryAmb[] = {1.0, 1.0, 1.0, ilum_uniforme};    // cor ambiente : amarela
    float carrySpec[]= { 1.0, 1.0, 1.0,  ilum_especular }; //cor difusa: amarela
    float carryDif[] = {1.0, 1.0, 1.0, ilum_difusa};       // cor especular: amarela
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
    //printf("amb =%0.1f,Spec =%0.1f, Dif=%0.1f,shine =%0.1f \n",ilum_uniforme,ilum_especular,ilum_difusa,ilum_shine);
}

void Sol::set_ilumincao(){
    //aumenta luz uniforme
    if(keyboard.z && ilum_uniforme<1.0)
        ilum_uniforme+=0.01*(float)keyboard.z;
    //diminui  luz uniforme
    if(keyboard.x && ilum_uniforme>0.0)
        ilum_uniforme-=0.01*(float)keyboard.x;
    //aumenta luz difusa
    if(keyboard.c && ilum_difusa<1.0)
        ilum_difusa+=0.01*(float)keyboard.c;
    //diminui  luz difusa
    if(keyboard.v && ilum_difusa>0.0)
        ilum_difusa-=0.01*(float)keyboard.v;
    //aumenta luz especular
    if(keyboard.b && ilum_especular<1.0)
        ilum_especular+=0.01*(float)keyboard.b;
    //diminui luz especular
    if(keyboard.n && ilum_especular>0.0)
        ilum_especular-=0.01*(float)keyboard.n;

    //aumenta luz shine
    if(keyboard.w && ilum_shine<100.0)
        ilum_shine+=1.0*(float)keyboard.w;
    //diminui luz shine
    if(keyboard.a && ilum_shine>0.0)
        ilum_shine-=1.0*(float)keyboard.a;


    // Propriedades do material da esfera
    float carryAmb[] = {1.0, 1.0, 1.0, ilum_uniforme};    // cor ambiente : amarela
    float carrySpec[]= { 1.0, 1.0, 1.0,  ilum_especular }; //cor difusa: amarela
    float carryDif[] = {1.0, 1.0, 1.0, ilum_difusa};       // cor especular: amarela
    float carryshine[] = { ilum_shine }; 

    matAmb = carryAmb;  // jogando para dentro do array do sol
    matSpec = carrySpec; // jogando para dentro do array do sol
    matDif =  carryDif;  // jogando para dentro do array do sol
    matshine =  carryshine;  // jogando para dentro do array do sol

    printf("amb =%0.1f,Spec =%0.1f, Dif=%0.1f,shine =%0.1f \n",ilum_uniforme,ilum_especular,ilum_difusa,ilum_shine);


}

void Sol::atualiza_sol(){

    if(vec_angulo[1]<360.0)
        vec_angulo[1]+=0.08;
    else
        vec_angulo[1]=0.0;

    if(vec_angulo[0]<360.0)
        vec_angulo[0]+=0.005;
    else
        vec_angulo[0]=0.0;

    //printf("aqui %0.2f , %0.2f \n",vec_angulo[0],vec_angulo[1]);

}

void Sol::desenhar_sol(){

    //botando iluminação
    //printf("errro luza?\n");
    // Propriedades da fonte de luz LIGHT0

    // O ERRRO TA EM UMA DESSAS 3 CORDENADAS
    glLightfv(GL_LIGHT0, GL_AMBIENT, matAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, matDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, matSpec);
    //printf("errro luzb?\n");
    //printf("errro luzc?\n");
    glPushMatrix();
        //glColor3f(1, 1, 1);// color dnv coutinho??
        glRotatef(vec_angulo[1], 1.0, 0.0, 0.0); // Rotação no eixo x
        glRotatef(vec_angulo[0], 0.0, 1.0, 0.0); // Rotação no eixo y
        //printf("errro luz0?\n");
        glLightfv(GL_LIGHT0, GL_POSITION, vec_pos.data());
        //printf("errro luz1?\n");
        glTranslatef(vec_pos[0], vec_pos[1], vec_pos[2]);
        glColor3f(1.0, 1.0, 1.0); // coutinho???? usar color??? para luz???
    glPopMatrix();
    
    //desenhando a esfera
    // Define (atualiza) o valor do expoente de especularidade
    //glMaterialfv(GL_FRONT, GL_SHININESS, matshine);

    // Desenha a esfera grande e bem arredondada
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, codigo_textura);  
    glPushMatrix();
        glRotatef(vec_angulo[1], 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(raio_Astro, 160, 160);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_LIGHT0);
    //printf("errro espehera?2?\n");
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
void solidSphere_inside(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricOrientation(quadObj,GLU_OUTSIDE);
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


