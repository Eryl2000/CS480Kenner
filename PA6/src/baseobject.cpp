#include "baseobject.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects
#include "object.h"


BaseObject::BaseObject(std::string _name, BaseObject *parent_, std::string objectPath) : parent(NULL)
{
    Setup(_name, parent_);
    if(!LoadObject(objectPath)){
        printf("Error loading model: %s\n", objectPath.c_str());
        exit(1);
    }
    Bind();
}

BaseObject::BaseObject(std::string _name, BaseObject *parent_, const aiScene * scene, unsigned int modelIndex) : parent(NULL)
{
    Setup(_name, parent_);
    if(!LoadObject(scene, modelIndex)){
        printf("Error loading model: %d\n", modelIndex);
        exit(1);
    }
    Bind();
}

void BaseObject::Setup(std::string _name, BaseObject *parent_)
{
    SetParent(parent_);
    model = glm::mat4(1.0);

    position = glm::vec3(0.0);
    eulerAngle = glm::vec3(0.0, 0.0, 0.0);
    scale = glm::vec3(1.0, 1.0, 1.0);
    name = _name;
}

void BaseObject::Bind()
{
    //Bind buffers
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete image;
}


bool BaseObject::LoadObject(std::string objectPath){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objectPath.c_str(), aiProcess_Triangulate);
    if(!scene){
        return false;
    }

    LoadObject(scene, (unsigned int) 0);
    for(unsigned int i = 1; i < scene->mNumMeshes; ++i)
    {
        new Object(name, this, scene, i);
    }

    return true;
}

bool BaseObject::LoadObject(const aiScene * scene, unsigned int modelIndex)
{
    Magick::InitializeMagick(NULL);
    if(!scene){
        return false;
    }

    const aiMaterial* pMaterial = scene->mMaterials[scene->mMeshes[modelIndex]->mMaterialIndex];

    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString Path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            image = new Magick::Image(std::string("../obj/" + std::string(Path.C_Str())));
        }
    }

    if(image == NULL)
    {
        image = new Magick::Image("../obj/granite.jpg");
    }

    image -> write(&blob, "RGBA");

    Indices.reserve(3 * scene->mMeshes[modelIndex]->mNumFaces);
    Vertices.reserve(scene->mMeshes[modelIndex]->mNumVertices);
    for(unsigned int j = 0; j < scene->mMeshes[modelIndex]->mNumVertices; ++j){
        aiVector3D *pos = &(scene->mMeshes[modelIndex]->mVertices[j]);
        glm::vec3 vert(pos->x, pos->y, pos->z);
        glm::vec3 col((rand() % 100) / 100.0, 0.0f, 0.0f);

        aiVector3D * texture = NULL;
        if(scene->mMeshes[modelIndex]->HasTextureCoords(0))
        {
            texture = &(scene->mMeshes[modelIndex]->mTextureCoords[0][j]);
        }
        glm::vec2 text;
        if(texture != NULL)
        {
            text.x = texture->x;
            text.y = texture->y;
        }

        Vertex v(vert, col, text);
        Vertices.push_back(v);
    }
    for(unsigned int j = 0; j < scene->mMeshes[modelIndex]->mNumFaces; j++){
        Indices.push_back(scene->mMeshes[modelIndex]->mFaces[j].mIndices[0]);
        Indices.push_back(scene->mMeshes[modelIndex]->mFaces[j].mIndices[1]);
        Indices.push_back(scene->mMeshes[modelIndex]->mFaces[j].mIndices[2]);
    }

    return true;
}

BaseObject::~BaseObject(){
    Vertices.clear();
    Indices.clear();
}

void BaseObject::Update(float dt){
    DerivedUpdate(dt);
    SetTransform(position, eulerAngle, scale);
}

void BaseObject::printModel() const{
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            std::cout << model[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

glm::mat4 BaseObject::GetModel(){
    return model;
}

void BaseObject::SetParent(BaseObject *parent_){
    if(parent_ == NULL){
        if(parent != NULL){
            parent->children.erase(
                std::remove(parent->children.begin(), parent->children.end(), this),
                parent->children.end());
        }
    } else{
        parent_->children.push_back(this);
    }
    parent = parent_;
}

BaseObject *BaseObject::Getparent(){
    return parent;
}

void BaseObject::Render(){

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void BaseObject::SetTransform(glm::vec3 _position, glm::vec3 _eulerAngle, glm::vec3 _scale){
    model = glm::mat4(1.0f);
    if(parent != NULL){
        for(int i = 0; i < 4; ++i){
            model[3][i] = parent->model[3][i];
        }
    }

    position = _position;
    eulerAngle = _eulerAngle;
    scale = _scale;

    model = glm::translate(model, position);
    model = glm::rotate(model, eulerAngle.y, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, eulerAngle.z, glm::vec3(0.0, 0.0, 1.0));
    model = glm::rotate(model, eulerAngle.x, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, scale);
}

glm::vec3 BaseObject::getPosition() const{
    return position;
}

glm::vec3 BaseObject::getEulerAngle() const{
    return eulerAngle;
}

glm::vec3 BaseObject::getScale() const{
    return scale;
}

void BaseObject::MouseDown(SDL_Event event){

}

void BaseObject::MouseUp(SDL_Event event){

}

void BaseObject::KeyDown(SDL_Event event){

}

void BaseObject::KeyUp(SDL_Event event){

}

void BaseObject::MouseWheel(SDL_Event event){

}
