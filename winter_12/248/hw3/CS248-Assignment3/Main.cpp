#include "Framework.h"
#include "Shader.h"

#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include "STPoint2.h"
#include "STVector2.h"
#include "STPoint3.h"
#include "STVector3.h"

#include <sstream>

#define CATHEDRAL_PATH "models/cathedral.3ds"
#define STATUE_PATH "models/armadillo.3ds"
//#define STATUE_PATH "models/sphere.3ds"

#define MY_PI 3.14159265
#define CUBE_MAP_SIZE 600

#define GL_CHECK(x) {\
(x);\
GLenum error = glGetError();\
if (GL_NO_ERROR != error) {\
printf("%s\n", gluErrorString(error));\
}\
}

// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(800, 600), "Assignment 3", sf::Style::Close, settings);

const sf::Input& Input = window.GetInput();
 
// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock clck;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
Assimp::Importer importer1;
Assimp::Importer importer2;
const aiScene* scene1;
const aiScene* scene2;
const aiScene* cur_scene;

//const aiMesh* mesh;
//std::vector<unsigned> indexBuffer;

GLuint scene_list1 = 0;
GLuint scene_list2 = 0;

GLuint cubeMap;

// current rotation angle
static float h_angle = 0.0;
static float v_angle = MY_PI/2;

bool mouseInit = false;
float lastTime = 0.0;
STPoint2 cur_mouse = STPoint2(0.0,0.0);
STPoint2 last_mouse = STPoint2(0.0,0.0);
STVector2 mouse_move = STVector2(0.0,0.0);

STPoint3 current_location = STPoint3(-10,2,0);
STPoint3 statue_location = STPoint3(0,2.92,0);

STVector3 Up = STVector3(0.0,1.0,0.0);
STVector3 current_forward = STVector3(sin(v_angle)*cos(h_angle),cos(v_angle),sin(v_angle)*sin(h_angle));
STVector3 current_right = STVector3::Cross(current_forward, Up);

// globals for keyboard control
bool go_forward = false;
bool go_backward = false;
bool go_left = false;
bool go_right = false;
bool go_up = false;
bool go_down = false;

struct meshObject {
    const aiMesh* mesh;
    std::vector<unsigned> indexBuffer;
    
    aiString diff_string;
    aiString spec_string;
    aiString norm_string;
};

std::vector<meshObject> meshes_1;
std::vector<meshObject> meshes_2;
std::vector<meshObject> * currentMesh_vec;

std::map<aiMaterial*, sf::Image*> diffuse_textures;
std::map<aiMaterial*, sf::Image*> specular_textures;
std::map<aiMaterial*, sf::Image*> normal_textures;

// shaders
std::vector<Shader*> shaders;

sf::Image white = sf::Image(1,1,sf::Color::White);

void initOpenGL();
void loadAssets();
void generateEnvironmentMap();
void handleInput();
void updatePositions();
void setupLights();

void renderFrame();
void applyMatrixTransform(const struct aiNode* nd);
void setMatrices(const aiScene * scene);
void setMaterial(const aiScene * scene, int meshNum, int shaderNum);
//void setTexturesEnv(int i, int shaderNum);
void setTextures(int meshNum, int shaderNum, bool cube);
void setMeshData(int meshNum, int shaderNum);
void recursive_load_meshes(const struct aiScene *sc, const struct aiNode* nd);
//void recursive_render(const struct aiScene *sc, const struct aiNode* nd);

int main(int argc, char** argv) {

    initOpenGL();
    loadAssets();
    
    // go to a square Viewport
    glViewport(0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE);
    generateEnvironmentMap();
    
    // go to our window Viewport
    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    // Put your game loop here (i.e., render with OpenGL, update animation)
    while (window.IsOpened()) {
        handleInput();
        renderFrame();
        window.Display();
        
        updatePositions();
    }
    
    return 0;
}

void setupLights()
{
    // some sexy values for light source 0
    GLfloat light0_position[] = { 0, 5, 0, 1 };
    GLfloat light0_ambient[] = { 0, 0, 0, 1 };
    GLfloat light0_diffuse[] = { .9, .8, .8, 1 };
    GLfloat light0_specular[] = { .9, 0.7, 0.7, 1 };
    GLfloat shininess = 40;
    glLightfv( GL_LIGHT0, GL_AMBIENT, light0_ambient );
    glLightfv( GL_LIGHT0, GL_POSITION, light0_position );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light0_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light0_specular );
    glLightfv( GL_LIGHT0, GL_SHININESS, &shininess );
}

void initOpenGL() {
    // Initialize GLEW on Windows, to make sure that OpenGL 2.0 is loaded
#ifdef FRAMEWORK_USE_GLEW
    GLint error = glewInit();
    if (GLEW_OK != error) {
        std::cerr << glewGetErrorString(error) << std::endl;
        exit(-1);
    }
    if (!GLEW_VERSION_2_0 || !GL_EXT_framebuffer_object) {
        std::cerr << "This program requires OpenGL 2.0 and FBOs" << std::endl;
        exit(-1);
    }
#endif
    // This initializes OpenGL with some common defaults.  More info here:
    // http://www.sfml-dev.org/tutorials/1.6/window-opengl.php
    glClearDepth(1.0f);
    glClearColor(0.3f, 0.25f, 0.7f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    setupLights();
}

void loadAssets() {
    // Read in an asset file, and do some post-processing.  There is much 
    // more you can do with this asset loader, including load textures.
    // More info is here:
    // http://assimp.sourceforge.net/lib_html/usage.html
    scene1 = importer1.ReadFile(CATHEDRAL_PATH,
                                aiProcess_CalcTangentSpace |
                                aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices |
                                aiProcessPreset_TargetRealtime_Quality);
    if (!scene1 || scene1->mNumMeshes <= 0) {
        std::cerr << importer1.GetErrorString() << std::endl;
        exit(-1);
    }
    
    scene2 = importer2.ReadFile(STATUE_PATH,  
                                aiProcess_CalcTangentSpace |
                                aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices |
                                aiProcessPreset_TargetRealtime_Quality);
    if (!scene2 || scene2->mNumMeshes <= 0) {
        std::cerr << importer2.GetErrorString() << std::endl;
        exit(-1);
    }

    
    currentMesh_vec = &meshes_1;
    // if the display list has not been made yet, create a new one and
    // fill it with scene contents
	if(scene_list1 == 0) {
	    scene_list1 = glGenLists(1);
	    glNewList(scene_list1, GL_COMPILE);
	    recursive_load_meshes(scene1, scene1->mRootNode);
	    glEndList();
	}
    glCallList(scene_list1);
    
    
    currentMesh_vec = &meshes_2;
    // if the display list has not been made yet, create a new one and
    // fill it with scene contents
	if(scene_list2 == 0) {
	    scene_list2 = glGenLists(1);
	    glNewList(scene_list2, GL_COMPILE);
	    recursive_load_meshes(scene2, scene2->mRootNode);
	    glEndList();
	}
    glCallList(scene_list2);

        
    Shader * shader1 = new Shader("shaders/phongNorm");
    shaders.push_back(shader1);
    
    Shader * shader2 = new Shader("shaders/phongEnvMap");
    shaders.push_back(shader2);
}

void environmentMatrixTransform(const aiScene * scene, int face) {
    
    STVector3 facingDir, upDir;
    switch (face) {
        case 0:
            facingDir = STVector3(-1,0,0);
            upDir = STVector3(0,1,0);
            break;
        case 1:
            facingDir = STVector3(1,0,0);
            upDir = STVector3(0,1,0);
            break;
        case 2:
            facingDir = STVector3(0,-1,0);
            upDir = STVector3(0,0,1);
            break;
        case 3:
            facingDir = STVector3(0,1,0);
            upDir = STVector3(0,0,-1);
            break;
        case 4:
            facingDir = STVector3(0,0,1);
            upDir = STVector3(0,1,0);
            break;
        case 5:
            facingDir = STVector3(0,0,-1);
            upDir = STVector3(0,1,0);
            break;
            
        default:
            printf("Your face is invalid!\n");
            break;
    }
    
    GLfloat aspectRatio = 1.0f;
    GLfloat nearClip = 0.1f;
    GLfloat farClip = 500.0f;
    GLfloat fieldOfView = 90.0f;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(statue_location.x, statue_location.y, statue_location.z, 
              statue_location.x + facingDir.x,
              statue_location.y + facingDir.y,
              statue_location.z + facingDir.z,
              upDir.x, upDir.y, upDir.z);
    
    setupLights();
    
    applyMatrixTransform(scene->mRootNode);
}

void renderEnvironmentMap(int face) {
    // Always clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    cur_scene = scene1;
    currentMesh_vec = &meshes_1;
    
    // use the shader associated with the Cathedral (shader 0)
    int shaderNum = 0;
    for (int i = 0; i < currentMesh_vec->size(); i++) {
        
        glUseProgram(shaders[shaderNum]->programID());
        
        environmentMatrixTransform(scene1, face);
        
        setMaterial(scene1, i, shaderNum);
        setTextures(i, shaderNum, true);
        setMeshData(i, shaderNum);
        
        // Draw the mesh
        if ((*currentMesh_vec)[i].mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE) {
            glDrawElements(GL_TRIANGLES, 3*(*currentMesh_vec)[i].mesh->mNumFaces, GL_UNSIGNED_INT, &(*currentMesh_vec)[i].indexBuffer[0]);
        }
    }
}

void generateEnvironmentMap() {
    
    // generate texture
    glGenTextures(1, &cubeMap);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    for (uint face = 0; face < 6; face++) {
        
        renderEnvironmentMap(face);


        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA8, CUBE_MAP_SIZE, CUBE_MAP_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, 0, 0, 0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE);
        
//        // THIS CODE SOLELY FOR TESTING: SAVE TO FILE
//        // for rendering to image (debugging)
//        sf::Uint8 *pixelArray = new sf::Uint8[CUBE_MAP_SIZE*CUBE_MAP_SIZE*4];
//        
//        glReadPixels(0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray); // this gives me nice images
//        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray); /// this gives me black images
//    
//        std::ostringstream out;
//        out << "/Users/phunter/CCRMA-Coursework/winter_12/248/hw3/test/cube_" << face << ".jpg";
//        sf::Image img(CUBE_MAP_SIZE, CUBE_MAP_SIZE, sf::Color::White);
//        img.LoadFromPixels(CUBE_MAP_SIZE, CUBE_MAP_SIZE, pixelArray);
//        img.SaveToFile(out.str());
    }
}


void updatePositions() {
    static float elapsed = 0.0f;
    elapsed += clck.GetElapsedTime();
    clck.Reset();
    
    float delta = elapsed - lastTime;
    lastTime = elapsed;
    
    float speed = 5.0;
    
    if (go_forward) {
        current_location += speed * delta * current_forward;
    }
    if (go_backward) {
        current_location -= speed * delta * current_forward;
    }
    if (go_left) {
        current_location -= speed * delta * current_right;
    }
    if (go_right) {
        current_location += speed * delta * current_right;
    }
    if (go_up) {
        current_location += speed * delta * Up;
    }
    if (go_down) {
        current_location -= speed * delta * Up;
    }
}


void handleInput() {
    
    // this handles setting keyboard booleans for motion
    go_forward = Input.IsKeyDown(sf::Key::W);
    go_backward = Input.IsKeyDown(sf::Key::S);
    go_left = Input.IsKeyDown(sf::Key::A);
    go_right = Input.IsKeyDown(sf::Key::D);
    go_up = Input.IsKeyDown(sf::Key::Space);
    go_down = Input.IsKeyDown(sf::Key::C);
    
    
    // Event loop, for processing user input, etc.  For more info, see:
    // http://www.sfml-dev.org/tutorials/1.6/window-events.php
    sf::Event evt;
    while (window.GetEvent(evt)) {
        
        switch (evt.Type) {
            case sf::Event::Closed: 
                // Close the window.  This will cause the game loop to exit,
                // because the IsOpened() function will no longer return true.
                window.Close(); 
                break;
            case sf::Event::Resized: 
                // If the window is resized, then we need to change the perspective
                // transformation and viewport
                glViewport(0, 0, evt.Size.Width, evt.Size.Height);
                break;
                
                // Mouse Movement
            case sf::Event::MouseMoved:
                if (!mouseInit) {
                    cur_mouse = STPoint2(evt.MouseMove.X, evt.MouseMove.Y);
                    last_mouse = cur_mouse;
                    mouseInit = true;
                }
                else {
                    cur_mouse = STPoint2(evt.MouseMove.X, evt.MouseMove.Y);
                }
                mouse_move = cur_mouse - last_mouse;
                
                h_angle += .01 * mouse_move.x;
                v_angle += .01 * mouse_move.y;
                if (v_angle < 0) {
                    v_angle = 0.01;
                }
                else if (v_angle > MY_PI) {
                    v_angle = MY_PI-.01;
                }
                
                current_forward = STVector3(sin(v_angle)*cos(h_angle),cos(v_angle),sin(v_angle)*sin(h_angle));
                current_forward.Normalize();
                current_right = STVector3::Cross(current_forward, Up);
                current_right.Normalize();
                
                last_mouse = cur_mouse;
                break;
                
                // Key presses
            case sf::Event::KeyPressed:
                switch (evt.Key.Code) {
                    case sf::Key::Escape:
                        window.Close();
                        break;
                        
                    default:
                        break;
                }
                
            default: 
                break;
        }
    }
}




void recursive_load_meshes(const struct aiScene *sc, const struct aiNode* nd) {
 
	unsigned int n = 0;
	struct aiMatrix4x4 m = nd->mTransformation;
    
	// update transform    
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
    
    
	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
        meshObject newMesh;
		newMesh.mesh = sc->mMeshes[nd->mMeshes[n]];
        
        // Set up the index buffer. Each face should have 3 vertices since we
        // specified aiProcess_Triangulate
        newMesh.indexBuffer.reserve(newMesh.mesh->mNumFaces * 3);
        for (unsigned i = 0; i < newMesh.mesh->mNumFaces; i++) {
            for (unsigned j = 0; j < newMesh.mesh->mFaces[i].mNumIndices; j++) {
                newMesh.indexBuffer.push_back(newMesh.mesh->mFaces[i].mIndices[j]);
            }
        }
        
        aiMaterial * mat = sc->mMaterials[newMesh.mesh->mMaterialIndex];
        aiString prefix = aiString("models/");
        aiString root;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &root);
        
        if (root != aiString("")) {
            //printf("%s\n",root.data);
            prefix.Append(root.data);
            
            aiString diff = aiString(prefix);
            diff.Append("_d.jpg");
            aiString spec = aiString(prefix);
            spec.Append("_s.jpg");
            aiString norm = aiString(prefix);
            norm.Append("_n.jpg");

            
            sf::Image * diffuseMap, * specularMap, * normalMap;
            
            std::map<aiMaterial*, sf::Image*>::iterator itr;
            itr = diffuse_textures.find(mat);
            if (itr == diffuse_textures.end())
            {
                // No material loaded yet
                diffuseMap = new sf::Image();
                bool loaded = diffuseMap->LoadFromFile(diff.data);
                if (loaded) {
                    printf("Loaded diffuse texture %s\n", diff.data);
                    diffuse_textures.insert(std::pair<aiMaterial *, sf::Image*>(mat, diffuseMap));
//                    diffuseMap->Bind();
//                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//                    glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
//                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    delete diffuseMap;
                }
            }
            itr = specular_textures.find(mat);
            if (itr == specular_textures.end())
            {
                // No texture loaded yet
                specularMap = new sf::Image();
                bool loaded = specularMap->LoadFromFile(spec.data);
                if (loaded) {
                    printf("Loaded specular texture %s\n", spec.data);
                    specular_textures.insert(std::pair<aiMaterial *, sf::Image *>(mat, specularMap));
//                    specularMap->Bind();
//                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//                    glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
//                    glGenerateMipmapEXT(GL_TEXTURE_2D);
                }
                else {
                    delete specularMap;
                }
            }
            itr = normal_textures.find(mat);
            if (itr == normal_textures.end())
            {
                // No texture loaded yet
                normalMap = new sf::Image();
                bool loaded = normalMap->LoadFromFile(norm.data);
                if (loaded) {
                    printf("Loaded normal texture %s\n", norm.data);
                    normal_textures.insert(std::pair<aiMaterial *, sf::Image *>(mat, normalMap));
//                    normalMap->Bind();
//                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//                    glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
//                    glGenerateMipmapEXT(GL_TEXTURE_2D);

                }
                else {
                    delete normalMap;
                }
            }
            
            
            newMesh.diff_string = diff;
            newMesh.spec_string = spec;
            newMesh.norm_string = norm;
        }
        else {
            newMesh.diff_string = aiString("");
        }
        
        currentMesh_vec->push_back(newMesh);
	}
    
	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_load_meshes(sc, nd->mChildren[n]);
	}
    
	glPopMatrix();
}


void setMeshData(int meshNum, int shaderNum) {
    const aiMesh * mesh = (*currentMesh_vec)[meshNum].mesh;
    
    // Get a handle to the variables for the vertex data inside the shader.
    GLint position = glGetAttribLocation(shaders[shaderNum]->programID(), "positionIn");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mVertices);
    
    // Texture coords.  Note the [0] at the end, very important
    GLint texcoord = glGetAttribLocation(shaders[shaderNum]->programID(), "texcoordIn");
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTextureCoords[0]);
    
    // Normals
    GLint normal = glGetAttribLocation(shaders[shaderNum]->programID(), "normalIn");
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mNormals);
    
    // Tangent Vectors
    GLint tangent = glGetAttribLocation(shaders[shaderNum]->programID(), "tangentIn");
    glEnableVertexAttribArray(tangent);
    glVertexAttribPointer(tangent, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTangents);
}


void applyMatrixTransform(const struct aiNode* nd) {
	struct aiMatrix4x4 m = nd->mTransformation;
    
	// update transform    
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
}


void setMatrices(const aiScene * scene) {
    // Set up the projection and model-view matrices
    GLfloat aspectRatio = (GLfloat)window.GetWidth()/window.GetHeight();
    GLfloat nearClip = 0.1f;
    GLfloat farClip = 500.0f;
    GLfloat fieldOfView = 45.0f; // Degrees
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(current_location.x, current_location.y, current_location.z, 
              current_location.x + current_forward.x,
              current_location.y + current_forward.y,
              current_location.z + current_forward.z, 0.0f, 1.0f, 0.0f);
    
    setupLights();
        
    // Pass in our view matrix for cubemapping purposes 
    // note: this may not be the best place to actually pass it to the shader...
    int shaderNum = 1;
    GLfloat* vMatrix = new GLfloat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, vMatrix);
        
    GLint vM = glGetUniformLocation(shaders[shaderNum]->programID(), "viewMatrix");
    glUniformMatrix4fv(vM,1,true,vMatrix);
    
    applyMatrixTransform(scene->mRootNode);
}


void setMaterial(const aiScene * scene, int meshNum, int shaderNum) {
    
    const aiMesh * mesh = (*currentMesh_vec)[meshNum].mesh;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    
    // Get a handle to the diffuse, specular, and ambient variables
    // inside the shader.  Then set them with the diffuse, specular, and
    // ambient color.
    GLint diffuse = glGetUniformLocation(shaders[shaderNum]->programID(), "Kd");
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glUniform3f(diffuse, color.r, color.g, color.b);
    
    // Specular material
    GLint specular = glGetUniformLocation(shaders[shaderNum]->programID(), "Ks");
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glUniform3f(specular, color.r, color.g, color.b);
    
    // Ambient material
    GLint ambient = glGetUniformLocation(shaders[shaderNum]->programID(), "Ka");
    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glUniform3f(ambient, color.r, color.g, color.b);
    
    // Specular power
    GLint shininess = glGetUniformLocation(shaders[shaderNum]->programID(), "alpha");
    float value;
    if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, value)) {
        printf("Lolz\n");
        glUniform1f(shininess, value);
    } else {
        glUniform1f(shininess, 40);
    }
}

void setTextures(int meshNum, int shaderNum, bool cube) {
    sf::Image * diffMap;
    sf::Image * specMap;
    sf::Image * normMap;    
    
    aiMaterial *mat = cur_scene->mMaterials[(*currentMesh_vec)[meshNum].mesh->mMaterialIndex];

    std::map<aiMaterial*, sf::Image*>::iterator itr;
    itr = diffuse_textures.find(mat);

    aiString root;
    mat->GetTexture(aiTextureType_DIFFUSE, 0, &root);

    if (itr != diffuse_textures.end()) {
        diffMap = itr->second;
    }
    else
    {
        // default for meshes with no texture
        diffMap = &white;
    }
        
    itr = specular_textures.find(mat);
    if (itr != specular_textures.end()) {
        specMap = specular_textures[mat];
    }
    else {
        specMap = diffMap;
    }
    
    itr = normal_textures.find(mat);
    if (itr != normal_textures.end()) {
        normMap = normal_textures[mat];
    }
    else {
        normMap = &white;
    }   
    
    // Get a "handle" to the texture variables inside our shader.  Then 
    // pass textures to the shader
    GLint diffuse = glGetUniformLocation(shaders[shaderNum]->programID(), "diffuseMap");
    glUniform1i(diffuse, 0); // The diffuse map will be GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    diffMap->Bind();
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    GLint specular = glGetUniformLocation(shaders[shaderNum]->programID(), "specularMap");
    glUniform1i(specular, 1); // The specular map will be GL_TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    specMap->Bind();
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    GLint normal = glGetUniformLocation(shaders[shaderNum]->programID(), "normalMap");
    glUniform1i(normal, 2); // The normal map will be GL_TEXTURE2
    glActiveTexture(GL_TEXTURE2);
    normMap->Bind();
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // if environment mapping, pass in the cubeMap texture
    if (cube) {
        GLint environ = glGetUniformLocation(shaders[shaderNum]->programID(), "environMap");
        glUniform1i(environ, 3); // The environment map will be GL_TEXTURE3
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
}

void renderFrame() {
    // Always clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int shaderNum;
    
    // Cathedral scene (scene1, meshes_1, shader 0)
    cur_scene = scene1;
    currentMesh_vec = &meshes_1;
    shaderNum = 0;
    for (int i = 0; i < currentMesh_vec->size(); i++) {

        glUseProgram(shaders[shaderNum]->programID());
        
        setMatrices(cur_scene);
        
        setMaterial(cur_scene, i, shaderNum);
        setTextures(i, shaderNum, false);
        setMeshData(i, shaderNum);
        
        // Draw the mesh
        if ((*currentMesh_vec)[i].mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE) {
            glDrawElements(GL_TRIANGLES, 3*(*currentMesh_vec)[i].mesh->mNumFaces, GL_UNSIGNED_INT, &(*currentMesh_vec)[i].indexBuffer[0]);
        }
    }
    
    // Statue scene (scene2, meshes_2, shader 1)
    cur_scene = scene2;
    currentMesh_vec = &meshes_2;
    shaderNum = 1;
    for (int i = 0; i < currentMesh_vec->size(); i++) {
        
        glUseProgram(shaders[shaderNum]->programID());
        
        setMatrices(cur_scene);
        
        setMaterial(cur_scene, i, shaderNum);
        setTextures(i, shaderNum, false);
        setMeshData(i, shaderNum);
        
        // Draw the mesh
        if ((*currentMesh_vec)[i].mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE) {
            glDrawElements(GL_TRIANGLES, 3*(*currentMesh_vec)[i].mesh->mNumFaces, GL_UNSIGNED_INT, &(*currentMesh_vec)[i].indexBuffer[0]);
        }
    }
}

