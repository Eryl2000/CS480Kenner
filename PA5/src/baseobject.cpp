#include "baseobject.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects

BaseObject::BaseObject(BaseObject *parent_, std::string objectPath) : parent(NULL){
    SetParent(parent_);
    model = glm::mat4(1.0);

    position = glm::vec3(0.0);
    angle = 0.0f;

    if(!LoadObject(objectPath)){
        printf("Error loading model: %s\n", objectPath.c_str());
        exit(1);
    }

    //Bind buffers
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}


bool BaseObject::LoadObject(std::string objectPath){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objectPath.c_str(), aiProcess_Triangulate);
    if(!scene){
        return false;
    }
    aiColor3D color(0.0f, 0.0f, 0.0f);
    for(unsigned int i = 0; i < scene->mNumMeshes; ++i){
        Indices.reserve(3 * scene->mMeshes[i]->mNumFaces);
        Vertices.reserve(scene->mMeshes[i]->mNumVertices);
        for(unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j){
            aiVector3D *pos = &(scene->mMeshes[i]->mVertices[j]);
            glm::vec3 vert(pos->x, pos->y, pos->z);
            glm::vec3 col((rand() % 100) / 100.0, 0.0f, 0.0f);
            Vertex v(vert, col);
            Vertices.push_back(v);
        }
        for(unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++){
            Indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
            Indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
            Indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
        }
    }

    return true;
}

BaseObject::~BaseObject()
{
  Vertices.clear();
  Indices.clear();
}

void BaseObject::Update(unsigned int dt)
{
    //Calculate dt in terms of seconds instead of milliseconds
    float dtFloat = dt / 1000.0f;
    if(parent == NULL){
        model = glm::mat4(1.0f);
    } else{
        model = parent->model;
    }
    DerivedUpdate(dtFloat);
}

glm::mat4 BaseObject::GetModel()
{
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

void BaseObject::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


void BaseObject::setPosition(glm::vec3 pos){
    model = glm::translate(model, pos - position);
}

glm::vec3 BaseObject::getPosition() const{
    return position;
}
