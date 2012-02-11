#include <iostream>
#include <fstream>
#include <sstream>
#ifdef __APPLE__
    #include <SDL/SDL.h>  
#else
    #include <SDL.h>
#endif
#include "mglmesh.h"
#include "mgltexture.h"

using namespace std;


MGLmesh::MGLmesh() {
    mNumVerts = 0;
    specular_exponent = 10.0;
    for(int i = 0; i < 3; i++) {
        diffuse_color.push_back(0.6f);
        ambient_color.push_back(0.2f);
        specular_color.push_back(1.0f);
    }
}

MGLmesh::~MGLmesh(){
    //Free Textures, if necessary
    if(diffuse_texture() != "")
        mglFreeTexture(mDiffTexID);

    if(specular_texture() != "")
        mglFreeTexture(mSpecTexID);
}

void MGLmesh::load(const string filename) {
    cout<<"Loading "<<filename <<"..."<<endl;
    vector<float> raw_verts;
    vector<float> raw_norms;
    vector<float> raw_texCoords;

    ifstream stream;
    stream.open(filename.c_str());

    while(!stream.fail()){
        string line;
        getline(stream,line);
        stringstream linestream(line);

        //Figure out what the line is about
        string linetype;
        linestream >>linetype;

        if(linetype == "v"){    
            float v[3];
            linestream >> v[0] >> v[1] >>v[2];
            raw_verts.insert(raw_verts.end(), v,v+3);
        }

        if(linetype == "vn"){
            float v[3];
            linestream >> v[0] >> v[1] >>v[2];
            raw_norms.insert(raw_norms.end(),v,v+3);
        }

        if(linetype == "vt"){
            float v[3];
            linestream >> v[0] >> v[1];
            raw_texCoords.insert(raw_texCoords.end(),v,v+2);
        }

        if(linetype == "f"){
            string pt[3];
            linestream >> pt[0] >> pt[1] >> pt[2];

            for(int i=0; i<3; i++){
                int indices[3];
                parseFace(pt[i],indices);
                if(indices[0] > 0 )
                    mVertices.insert(mVertices.end(), 
                                     raw_verts.begin()+3*(indices[0] -1),
                                     raw_verts.begin()+3*indices[0]) ;
                if(indices[1] > 0)
                    mTexCoords.insert(mTexCoords.end(), 
                                      raw_texCoords.begin()+2*(indices[1] -1),
                                      raw_texCoords.begin()+ 2*indices[1]);
                if(indices[2] > 0 )
                    mNormals.insert(mNormals.end(), 
                                    raw_norms.begin() +3*(indices[2] -1),
                                    raw_norms.begin() +3*indices[2]);
                mNumVerts++;                       
            }
        }

        if(linetype  == "mtllib"){
            string matfile;
            linestream >> matfile;
            parseMaterial(matfile);
        }
    }

    //Set Textures
    MGLpixel* texData = NULL;
    MGLsize w, h;
    if(diffuse_texture()  != ""){
        mglGetTexture(diffuse_texture(), texData, &w, &h);
        mDiffTexID = mglLoadTexture(w, h, texData);
        mglTextureSlot(MGL_TEX_DIFFUSE);
        mglTexturesEnabled(true);
        delete[] texData;
    }
    if(specular_texture()  != ""){
        mglGetTexture(specular_texture(), texData, &w, &h);
        mSpecTexID = mglLoadTexture(w, h, texData);
        mglTextureSlot(MGL_TEX_SPECULAR);
        mglTexturesEnabled(true);
        delete[] texData;
    }
}

void MGLmesh::display(){

    //Set Material Properties
    MGLfloat aColor[3];
    MGLfloat dColor[3];
    MGLfloat sColor[3];
    MGLfloat shininess = this->shininess();
    for (int i = 0; i < 3; i++){
        aColor[i] = ambientColor()[i];
        dColor[i] = diffuseColor()[i];
        sColor[i] = specularColor()[i];
    }
      
    mglMaterial(MGL_MAT_DIFFUSE,dColor);
    mglMaterial(MGL_MAT_AMBIENT,aColor);
    mglMaterial(MGL_MAT_SPECULAR,sColor);
    mglMaterial(MGL_MAT_SHININESS,&shininess);
   
    //Load Textures
    if(diffuse_texture() != ""){
        mglTextureSlot(MGL_TEX_DIFFUSE);
        mglUseTexture(mDiffTexID);
    }
    if(specular_texture() != ""){
        mglTextureSlot(MGL_TEX_SPECULAR);
        mglUseTexture(mSpecTexID);
    }
    
    //Object
    vector<float>& verts = vertices();
    vector<float>& norms = normals();
    vector<float>& tCoords= texCoords();
    mglBegin(MGL_TRIANGLES);
    for(unsigned int i=0 ; i< numVertices(); i++){
        if(norms.size() > 0)
            mglNormal(norms[3*i], norms[3*i+1], norms[3*i+2]);
        if(tCoords.size() > 0)
            mglTexCoord(tCoords[2*i],tCoords[2*i+1]);
        mglVertex3(verts[3*i],verts[3*i+1],verts[3*i+2]);
    }
    mglEnd();
}

vector<MGLfloat>& MGLmesh::vertices()
{
    return this->mVertices;
}

vector<MGLfloat>& MGLmesh::normals()
{
    return this->mNormals;
}

vector<MGLfloat>& MGLmesh::texCoords()
{
    return this->mTexCoords;
}

unsigned int MGLmesh::numVertices(){
    return mNumVerts;
}

vector<MGLfloat>& MGLmesh::diffuseColor(){
    return diffuse_color;
}

vector<MGLfloat>& MGLmesh::specularColor(){
    return specular_color;
}

vector<MGLfloat>& MGLmesh::ambientColor(){
    return ambient_color;
}

MGLfloat MGLmesh::shininess(){
    return specular_exponent;
}

string MGLmesh::diffuse_texture(){
    return diffuse_texture_file;
}

string MGLmesh::specular_texture(){
    return specular_texture_file;
}

void MGLmesh::parseFace(string facestring, int indices[]){
        
    int numFound=0;

    stringstream faceStream; 
    size_t pos = 0;
    size_t slashPos = facestring.find("/", pos);
    while(slashPos != string::npos){
        if(pos == slashPos){
            indices[numFound ++] = 0;
        }
        else{
            sscanf(facestring.substr(pos,slashPos).c_str(),"%d",&(indices[numFound++]));
        }
        pos = slashPos + 1;
        slashPos = facestring.find("/",pos);
    }
    
    if(pos == facestring.size()){
        indices[numFound ++] = 0;
    }
    else{
        sscanf(facestring.substr(pos).c_str(),"%d",&(indices[numFound++]));
    }
}

void MGLmesh::parseMaterial(string materialfn){
    ifstream matStream;
    matStream.open(materialfn.c_str());
    if(matStream.fail()){
        cerr<< "Unable to open file: "<<materialfn<<endl;
        cin.get();
        exit(1);
    }

    while(!matStream.fail()){
        string line;
        string linetype;

        getline(matStream, line);
        stringstream linestream;
        linestream.str(line);
        linestream >> linetype;

        if (linetype == "Kd" ||
            linetype == "Ka" ||
            linetype == "Ks"){

            float c[3];
            linestream >> c[0] >>c[1] >>c[2];
            
            //Normalize in case of file errors
            //for(int i=0; i< 3; i++)
            //  if(c[i] > 1.0f) c[i] = 1.0f;

            vector<MGLfloat> *color = NULL;

            if(linetype == "Kd")
                color = &diffuse_color;
            if(linetype == "Ka")
                color = &ambient_color;
            if(linetype == "Ks")
                color = &specular_color;

            if (color != NULL) {
                (*color)[0] = c[0];
                (*color)[1] = c[1];
                (*color)[2] = c[2];
            }
        }

        if(linetype == "map_Kd")
            linestream >> diffuse_texture_file;
            
        if(linetype == "map_Ks")
            linestream >> specular_texture_file;
        
        if(linetype == "Ns")
            linestream>>specular_exponent;
    }
}

