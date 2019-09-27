#include "baseobject.h"
#include <algorithm>
#include <sstream>
#include <fstream>

BaseObject::BaseObject(BaseObject *parent_, std::string objectPath){
    parent = parent_;
    if(parent != NULL){
        parent->children.push_back(this);
    }

    LoadObject(objectPath);

    angle = 0.0f;

    //Bind buffers
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}


bool BaseObject::LoadObject(std::string objectPath){
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE *file = fopen(objectPath.c_str(), "r");
    if(file == NULL){
        printf("Impossible to open the file %s\n", objectPath.c_str());
        return false;
    }

    bool oldVersion = false;
    while(true){
        char lineHeader[128];
        //read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if(res == EOF){
            break;
        }
        if(strcmp(lineHeader, "#") == 0){
            fscanf(file, "%s", lineHeader);
            if(strcmp(lineHeader, "Blender") == 0){
                fscanf(file, "%s", lineHeader);
                oldVersion = strcmp(lineHeader, "v2.62") == 0;
            }
        } else if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            //temp_vertices.push_back(vertex);
            glm::vec3 color((rand() % 100) / 100.0, 0.0, 0.0);
            Vertex v(vertex, color);
            Vertices.push_back(v);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            if(oldVersion){
                int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                if (matches != 3){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }
            } else{
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }
            }

            Indices.push_back(vertexIndex[0]);
            Indices.push_back(vertexIndex[1]);
            Indices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    fclose(file);

    // The index works at a 0th index
    for(unsigned int i = 0; i < Indices.size(); i++)
    {
        Indices[i]--;
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
