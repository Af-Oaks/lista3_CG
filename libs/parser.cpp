#include "parser.h"
// Parser para modelo de arquivo

int parse_model(inf_astros *aux, const char *file_name,int *num_luas)
{
    char *input_str = (char *)malloc(sizeof(char) * 200);
    char *nome_textura = (char *)malloc(sizeof(char) * 200);
    std::ifstream *file = new std::ifstream();
    std::vector<GLfloat> vec_aux;
    inf_astros luas={0};
    int i = 0;
    float carry1=0,carry2 = 0;
    file->open(file_name, std::ios::in);
    *num_luas=0;

    // Verifica se o arquivo está aberto
    if (!file->is_open()){
        printf("erro ao abrir|| arquivo aberto? \n");
        goto ERROR;
        }

    // Verifica se o arquivo está vazio
    if (!file->eof())
    {
        file->getline(input_str, 100);

        if (strcmp("ROTEIRO", input_str))
        {
            delete input_str;
            delete file;
            return 1;
        }

    }
    else{
        printf("arquivo vazio!\n");
        goto ERROR;}
        
    //printf("%s  M01\n",input_str);
    // Lê o modelo do arquivo
    while (file->peek() != EOF)
    {
        file->getline(input_str, 100);
        //printf("%s  M02\n",input_str);
        sscanf(input_str, "%f,%f,%f,%f,%f", &aux->raio_Astro_ao_sol,&aux->material,&aux->raio_Astro,&carry1,&carry2);
        aux->vec_velo.push_back(carry1);
        aux->vec_velo.push_back(carry2);
        file->getline(input_str, 100);
        //printf("%s  M03\n",input_str);
        sscanf(input_str, "%s", nome_textura);  
        std::string input_caminho("assets/texture/");
        input_caminho.append(input_str);
        //printf("name_t = %s\n",input_caminho.c_str());
        // Carrega a textura usando a soil
        GLuint idTextura = SOIL_load_OGL_texture(input_caminho.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

        // Verifica se ocorreu algum erro na SOIL
        if (idTextura == 0)
        {
            printf("Erro do SOIL: '%s'\n", SOIL_last_result());
            return 1;
        }
        else{
            aux->codigo_textura = idTextura;
        }
        
     //pula para proxima linha que vai ter a quantia de luas
    file->getline(input_str, 100);
    //printf("%s  M04\n",input_str);
    sscanf(input_str,"%d",num_luas);

    //tenta pegar mais uma linha para ter zerteza que chego no EOF!
    file->getline(input_str, 100);
    //printf("%s  M05\n",input_str);
    
        
    }

    #if DEBUG
    for (auto i : *target)
    {
        printf("wave[] id=%d : x=%f, y=%f, time=%f \n", i.id_enemy, i.x, i.y, i.time);
    }
    #endif

    // Label para limpar a memória
ERROR:
    delete input_str;
    delete file;

    return 0;
}


int parse_lua(std::vector<inf_astros> *aux,const char *file_name,int num_luas){

    char *input_str = (char *)malloc(sizeof(char) * 200);
    char *nome_textura = (char *)malloc(sizeof(char) * 200);
    std::ifstream *file = new std::ifstream();
    std::vector<GLfloat> vec_aux;
    inf_astros luas={0};
    int i = 0;
    float carry1=0,carry2 = 0;
    std::string input_caminho("assets/texture/");

    file->open(file_name, std::ios::in);

    // Verifica se o arquivo está aberto
    if (!file->is_open())
        goto ERROR;

    // Verifica se o arquivo está vazio
    if (!file->eof())
    {
        file->getline(input_str, 100);

        if (strcmp("ROTEIRO", input_str))
        {
            delete input_str;
            delete file;
            return 1;
        }
    }
    else
        goto ERROR;

    // Lê o modelo do arquivo
    for(int z=0;z<num_luas;z++)
    {
        luas.vec_velo.clear();
        file->getline(input_str, 100);
        sscanf(input_str, "%f,%f,%f,%f,%f", &luas.raio_Astro_ao_sol,&luas.material,&luas.raio_Astro,&carry1,&carry2);
        luas.vec_velo.push_back(carry1);
        luas.vec_velo.push_back(carry2);
        file->getline(input_str, 100);
        sscanf(input_str, "%s", nome_textura);  
        input_caminho.clear();
        input_caminho.append("assets/texture/");
        input_caminho.append(input_str);
        printf("name_t = %s\n",input_caminho.c_str());

        // Carrega a textura usando a soil
        GLuint idTextura = SOIL_load_OGL_texture(input_caminho.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

        // Verifica se ocorreu algum erro na SOIL
        if (idTextura == 0)
        {
            printf("Erro do SOIL: '%s'\n", SOIL_last_result());
            return 1;
        }
        else{
            luas.codigo_textura = idTextura;
        }

        printf("errro pushback?\n");
        aux->push_back(luas);
        printf("errro pushback2?\n");
    }   

    #if DEBUG
    for (auto i : *target)
    {
        printf("wave[] id=%d : x=%f, y=%f, time=%f \n", i.id_enemy, i.x, i.y, i.time);
    }
    #endif

    // Label para limpar a memória
ERROR:
    delete input_str;
    delete file;

    return 0;
}
