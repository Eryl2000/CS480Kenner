#include "baseobject.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects
#include "object.h"


BaseObject::BaseObject(std::string _name, BaseObject *parent_, std::string objectPath, bool _onlyCopyParentPos)
    : parent(NULL),
    name(_name),
    doNotRender(objectPath == std::string("")),
    onlyCopyParentPos(_onlyCopyParentPos),
    rigidbody(NULL)
{
    Setup(parent_);
    if(!doNotRender){
        if(!LoadObject(objectPath)){
            printf("Error loading model: %s\n", objectPath.c_str());
            exit(1);
        }
        Bind();
    }
}

BaseObject::BaseObject(std::string _name, BaseObject *parent_, const aiScene * scene, unsigned int modelIndex, bool _onlyCopyParentPos)
    : parent(NULL),
    name(_name),
    doNotRender(scene == NULL),
    onlyCopyParentPos(_onlyCopyParentPos),
    rigidbody(NULL)
{
    Setup(parent_);
    if(!doNotRender){
        if(!LoadObject(scene, modelIndex)){
            printf("Error loading model: %d\n", modelIndex);
            exit(1);
        }
        Bind();
    }
}

void BaseObject::Setup(BaseObject *parent_)
{
    SetParent(parent_);
    model = glm::mat4(1.0);

    position = glm::vec3(0.0);
    eulerAngle = glm::vec3(0.0, 0.0, 0.0);
    scale = glm::vec3(1.0, 1.0, 1.0);

    ka = glm::vec4(0, 0, 0, 1);
    kd = glm::vec4(0.8, 0.8, 0.8, 1);
    ks = glm::vec4(0.75, 0.75, 0.75, 1);
    shininess = 2;
}

void BaseObject::Bind()
{
    //Bind buffers and textures
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
    image = NULL;
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

    if(image == NULL){
        image = new Magick::Image("../obj/" + name + ".jpg");
    }

    if(image == NULL)
    {
        image = new Magick::Image("../obj/planetTextures/" + name + ".jpg");
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
            text.y = -texture->y;
        }

        glm::vec3 normal;
        if(scene->mMeshes[modelIndex]->HasNormals())
        {
            aiVector3D *norm = &(scene->mMeshes[modelIndex]->mNormals[j]);
            normal.x = norm->x;
            normal.y = norm->y;
            normal.z = norm->z;
        }

        Vertex v(vert, col, text, normal);
        Vertices.push_back(v);
    }
    for(unsigned int j = 0; j < scene->mMeshes[modelIndex]->mNumFaces; j++){
        Indices.push_back(scene->mMeshes[modelIndex]->mFaces[j].mIndices[0]);
        Indices.push_back(scene->mMeshes[modelIndex]->mFaces[j].mIndices[1]);
        Indices.push_back(scene->mMeshes[modelIndex]->mFaces[j].mIndices[2]);
    }

    objTriMesh = new btTriangleMesh();
    for(unsigned int j = 0; j < scene->mMeshes[modelIndex]->mNumFaces; j++){
        btVector3 triArray[3];
        for(unsigned int index = 0; index < 3; ++index){
            aiFace face = scene->mMeshes[modelIndex]->mFaces[j];
            int mIndex = face.mIndices[index];
            aiVector3D position = scene->mMeshes[modelIndex]->mVertices[mIndex];
            triArray[index] = btVector3(position.x, position.y, position.z);
        }
        objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
    }

    return true;
}

BaseObject::~BaseObject(){
    Vertices.clear();
    Indices.clear();
}

/*
 * HARRIS BUTTON! Changes all textures to a picture of Dr. Harris
 */
void BaseObject::HarrisButton(bool _harrisButton){
    if(harrisButton != _harrisButton){
        harrisButton = _harrisButton;
        if(harrisButton)
        {
            if(image != NULL){
                delete image;
            }
            image = new Magick::Image(std::string("../obj/planetTextures/harris.jpg"));
        } else{
            if(image != NULL){
                delete image;
            }
                if(image == NULL){
                    image = new Magick::Image("../obj/" + name + ".jpg");
                }

                if(image == NULL){
                    image = new Magick::Image("../obj/planetTextures/" + name + ".jpg");
                }

        }
        if(image != NULL){
            image->write(&blob, "RGBA");
            Bind();
        }
    }
    std::cout << (harrisButton ? "HARRIS!" : "NOT HARRIS") << std::endl;
}

/*
 * The base update function which is called by graphics class. It calls DerivedUpdate().
 * Derived classes only override DerivedUpdate(), not the normal Update() function
 */
void BaseObject::Update(float dt){
    DerivedUpdate(dt);
    SyncBullet();
    //SetTransform(position, eulerAngle, scale);
}

void BaseObject::SyncBullet()
{
    if(rigidbody != NULL)
    {
        btTransform trans;
        btScalar m[16];

        trans = rigidbody->getWorldTransform();
        trans.getOpenGLMatrix(m);
        /*for(int i = 0; i < 16; ++i){
            std::cout << m[i] << " ";
        }
        std::cout << std::endl;
        */
        model = glm::make_mat4(m);
    } else
    {
        std::cerr << "Object \"" << name << "\" does not have a rigidbody (BaseObject::SyncBullet)" << std::endl;
        exit(1);
    }
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

glm::vec4 BaseObject::GetAmbient() const{
    return ka;
}

glm::vec4 BaseObject::GetDiffuse() const{
    return kd;
}

glm::vec4 BaseObject::GetSpecular() const{
    return ks;
}

float BaseObject::GetShininess() const{
    return shininess;
}

void BaseObject::incrementAmbient(){
    ka = ka + glm::vec4(.05,.05,.05,0);
}

void BaseObject::decrementAmbient(){
    ka = ka - glm::vec4(.05,.05,.05,0);
}

void BaseObject::incrementSpecular(){
    ks = ks + glm::vec4(.05,.05,.05,0);
}

void BaseObject::decrementSpecular(){
    ks = ks - glm::vec4(.05,.05,.05,0);
}

void BaseObject::incrementDiffuse(){
    kd = kd + glm::vec4(.05,.05,.05,0);
}

void BaseObject::decrementDiffuse(){
    kd = kd - glm::vec4(.05,.05,.05,0);
}

/*
 * Sets an object as this object's parent which means its position and rotation
 * will be based on the parent's position and rotation.
 */
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

/*
 * Renders the object to the screen at the location, orientation, and size
 * specified by the model matrix.
 */
void BaseObject::Render(){
    if(doNotRender){
        return;
    }
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBindTexture(GL_TEXTURE_2D, texture);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glActiveTexture(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

/*
 * Sets the position, orientation, and scale of the object based on the member
 * variables for each. This allows these member variables to be updated as many
 * times as wanted within DerivedUpdate() while the calculations to update
 * the model matrix correspondingly will only be done once, since this function
 * is called after DerivedUpdate().
 */
void BaseObject::SetTransform(glm::vec3 _position, glm::vec3 _eulerAngle, glm::vec3 _scale){
    model = glm::mat4(1.0f);
    if(parent != NULL){
        if(onlyCopyParentPos){
            //Only use the position of the parent, not the orientation
            for(int i = 0; i < 4; ++i){
                model[3][i] = parent->model[3][i];
            }
        } else{
            model = parent->model;
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
    glm::vec3 pos;
    for(int i = 0; i < 3; ++i){
        //std::cout << model[3][i] << " ";
        pos[i] = model[3][i];
    }
    //std::cout << std::endl;
    return pos;
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

void BaseObject::MouseMotion(SDL_Event event){

}
