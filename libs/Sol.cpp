#include "Sol.h"

//Classe Sol
Sol::Sol(const char * script,std::vector<std::pair<std::string,GLint>> vec_texture){

    ilum_difusa=1.0;
    ilum_especular=1.0;
    ilum_shine=50.0;
    ilum_uniforme=0.0;
    // Propriedades do material da esfera
    float carryAmb[] = {1.0, 1.0, 1.0, ilum_uniforme};    // cor ambiente : amarela
    float carrySpec[]= { 1.0, 1.0, 1.0,  ilum_especular }; //cor difusa: amarela
    float carryDif[] = {1.0, 1.0, 1.0, ilum_difusa};       // cor especular: amarela
    float carryshine[] = { ilum_shine }; 

    // Propriedades da fonte de luz LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, carryAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, carryDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, carrySpec);
    glLightfv(GL_LIGHT0, GL_SHININESS, carryshine);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   //struct de informações para carregar os atributos do Astro
    inf_astros aux={0};
    int num=0;
    if (!parse_model(&aux,script,&num))
    {
        printf("DEU BOM NO FILE SOL \n");
    }
    for(auto i : vec_texture){

        if(i.first.compare(aux.nome_textura)==0){
            this->codigo_textura =i.second;
            break;
        }

    }
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

    n_callist = glGenLists(1);
    glNewList(n_callist, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, codigo_textura);  
            glLightfv(GL_LIGHT0, GL_POSITION, vec_pos.data());
            solidSphere(raio_Astro, 160, 160);
            glDisable(GL_TEXTURE_2D);   
        glPopMatrix();
    glEndList();

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
    // Propriedades da fonte de luz LIGHT0
    // Propriedades do material da esfera
    float carryAmb[] = {ilum_uniforme,ilum_uniforme,ilum_uniforme,1.0};    // cor ambiente : amarela
    float carrySpec[]= {ilum_especular,ilum_especular,ilum_especular, 1.0 }; //cor difusa: amarela
    float carryDif[] = {ilum_difusa,ilum_difusa,ilum_difusa,1.0};       // cor especular: amarela
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, carryAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, carryDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, carrySpec);

    // Desenha a esfera grande e bem arredondada
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glRotatef(-vec_angulo[1], 1.0, 0.0, 0.0); // Rotação no eixo x
        glRotatef(-90, 0.0, 1.0, 0.0); // Rotação no eixo y
        glTranslatef(vec_pos[0], vec_pos[1], vec_pos[2]);
        glCallList(n_callist);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // desenhando as informaçoes na tela
    
}

//vector com ifnormações da ilum nessa ordem ambiente,difusa,especular,shine
std::vector<float>  Sol::ilum_variables(){

    std::vector<float> carry;
    carry.push_back(ilum_uniforme);
    carry.push_back(ilum_difusa);
    carry.push_back(ilum_especular);
    carry.push_back(ilum_shine);

    return carry;
}

void Sol::set_iluminacao(){
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

    //printf("amb =%0.1f,Spec =%0.1f, Dif=%0.1f,shine =%0.1f \n",ilum_uniforme,ilum_especular,ilum_difusa,ilum_shine);


}