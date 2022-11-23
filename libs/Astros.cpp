#include "Astros.h"


//----------------------------------------------------------------------------//
//Classe Astros
Astros::Astros(const char * script,std::vector<std::pair<std::string,GLint>> vec_texture){
    //struct de informações para carregar os atributos do Astro
    inf_astros aux={0};
    if (!parse_model(&aux,script,&num_luas))
    {
        printf("DEU BOM NO FILE ASTRO \n");
    }
    for(auto i : vec_texture){

        if(i.first.compare(aux.nome_textura)==0){
            this->codigo_textura =i.second;
            break;
        }

    }

    this->nome_textura = aux.nome_textura;
    this->material = aux.material;
    this->raio_Astro = aux.raio_Astro;
    this->raio_Astro_ao_sol = aux.raio_Astro_ao_sol;
    this->vec_velo =aux.vec_velo;
    //      pos_x,                            pos_y,                                 pos_z
    this->vec_pos.push_back(raio_Astro_ao_sol);this->vec_pos.push_back(0.0);this->vec_pos.push_back(0.0);
    printf("num luas %d\n",num_luas);
    vec_angulo.push_back(0.0);vec_angulo.push_back(0.0);
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
        //printf("raio lua em relação aaa %0.2f \n",aux_luas[0].raio_Astro_ao_sol);
        for(auto i:aux_luas){

            luas.push_back(std::make_shared<Luas>(i,vec_texture));
        }
    }
    n_callist = glGenLists(1);
    glNewList(n_callist, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, codigo_textura);  
            solidSphere(raio_Astro, 160, 160);
            glDisable(GL_TEXTURE_2D);   
        glPopMatrix();
    glEndList();

}

Astros::Astros(inf_astros info_astro,std::vector<std::pair<std::string,GLint>> vec_texture){
    
    for(auto i : vec_texture){

        if(i.first.compare(info_astro.nome_textura)==0){
            this->codigo_textura =i.second;
            break;
        }

    }
    this->nome_textura = info_astro.nome_textura;
    this->material = info_astro.material;
    this->raio_Astro = info_astro.raio_Astro;
    this->raio_Astro_ao_sol = info_astro.raio_Astro_ao_sol;
    this->vec_velo =info_astro.vec_velo;
    vec_angulo.push_back(0.0);vec_angulo.push_back(0.0);
    //      pos_x,                    pos_y,                    pos_z
    printf("raio da lua em relação ao sol! %0.2f \n",raio_Astro_ao_sol);
    vec_pos.push_back(raio_Astro_ao_sol);vec_pos.push_back(0.0);vec_pos.push_back(0.0);

    n_callist = glGenLists(1);
    glNewList(n_callist, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, codigo_textura);  
            solidSphere(raio_Astro, 160, 160);
            glDisable(GL_TEXTURE_2D);   
        glPopMatrix();
    glEndList();
}

void Astros::atualiza_posicao(){

    if(vec_angulo[1]<360.0)// rotação em torno do seu proprio eixo
        vec_angulo[1]+=vec_velo[1];
    else
        vec_angulo[1]=0.0;

    if(vec_angulo[0]<360.0)//rotação em relação ao seu astro
        vec_angulo[0]+=vec_velo[0]/1000.0;
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

void Astros::desenhar_Astro(float shine){


    //glColor3f(1, 1, 1);// color dnv coutinho??
    float mat_aux[] = {shine};
    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_aux);
    //glColor3f(1, 1, 1);// color dnv coutinho??

    // Chama a calllist
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(vec_pos[0],vec_pos[1],vec_pos[2]);
        glRotatef(vec_angulo[1], 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        glCallList(n_callist);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);   

    //calma 1
    
    for(auto i : luas){

        i->desenhar_Lua(shine);
    }
    
}
//----------------------------------------------------------------------------//

//Classe  Luas
Luas::Luas(inf_astros inf_lua,std::vector<std::pair<std::string,GLint>> vec_texture) :Astros(inf_lua,vec_texture){


}

void Luas::desenhar_Lua(float shine){

    //glColor3f(1, 1, 1);// color dnv coutinho??
    float mat_aux[] = {shine};

    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_aux);

    // Chama a callist
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(vec_pos[0],vec_pos[1],vec_pos[2]);
        glRotatef(vec_angulo[0], 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        glCallList(n_callist);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);   
    //printf("desenho_sts LUAS?\n");
}

void Luas::atualiza_lua(std::vector<GLfloat> astro_ref){

    if(vec_angulo[1]<360.0)//rotacao em relação ao seu astro
        vec_angulo[1]+=vec_velo[1]/55;
    else
        vec_angulo[1]=0.0;

    if(vec_angulo[0]<360.0)//rotacao no seu proprio eixo
        vec_angulo[0]+=vec_velo[0]/3.0;
    else
        vec_angulo[0]=0.0;

    vec_pos[0]=astro_ref[0] + raio_Astro_ao_sol*cos(vec_angulo[1]);
    vec_pos[2]=astro_ref[2] + raio_Astro_ao_sol*sin(vec_angulo[1]);

    //atualizar via cordenadas polares

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