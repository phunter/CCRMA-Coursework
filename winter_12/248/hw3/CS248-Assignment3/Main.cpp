#include "Framework.h"
#include "Shader.h"

#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include "STPoint2.h"
#include "STVector2.h"
#include "STPoint3.h"
#include "STVector3.h"

//#define MODEL_PATH "models/dragon.dae"
//#define MODEL_PATH "models/teapot.3ds"
//#define CATHEDRAL_PATH "models/dragon.dae"
#define CATHEDRAL_PATH "models/cathedral.3ds"
#define STATUE_PATH "models/armadillo.3ds"
//#define STATUE_PATH "models/sphere.3ds"


#define MY_PI 3.14159265


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

//const aiMesh* mesh;
//std::vector<unsigned> indexBuffer;

GLuint scene_list1 = 0;
GLuint scene_list2 = 0;

// current rotation angle
static float h_angle = 0.0;
static float v_angle = MY_PI/2;

bool mouseInit = false;
float lastTime = 0.0;
STPoint2 cur_mouse = STPoint2(0.0,0.0);
STPoint2 last_mouse = STPoint2(0.0,0.0);
STVector2 mouse_move = STVector2(0.0,0.0);

STPoint3 current_location = STPoint3(0.0,2.0,0.0);

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
//    int diff_index;
//    int spec_index;
};

std::vector<meshObject> meshes_1;
std::vector<meshObject> meshes_2;
std::vector<meshObject> * currentMesh_vec;

std::map<std::string, sf::Image> TextureMap;

// shader
std::vector<Shader*> shaders;
//std::auto_ptr<Shader> shader1;
//std::auto_ptr<Shader> shader2;

// Texture
//std::auto_ptr<sf::Image> diffuseMap;
//std::auto_ptr<sf::Image> specularMap;


sf::Image white = sf::Image(1,1,sf::Color::White);

void initOpenGL();
void loadAssets();
void handleInput();
void updatePositions();

void renderFrame();
void setMatrices(const aiScene * scene);
void setMaterial(const aiScene * scene, int meshNum, int shaderNum);
void setTextures(int meshNum, int shaderNum);
void setMeshData(int meshNum, int shaderNum);
void recursive_load_meshes(const struct aiScene *sc, const struct aiNode* nd);
//void recursive_render(const struct aiScene *sc, const struct aiNode* nd);

int main(int argc, char** argv) {

    initOpenGL();
    loadAssets();

    // Put your game loop here (i.e., render with OpenGL, update animation)
    while (window.IsOpened()) {
        handleInput();
        renderFrame();
        window.Display();
        
        updatePositions();
    }
    
    return 0;
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
    glClearColor(0.45f, 0.45f, 0.45f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
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


//    printf("scene->HasLights() = %d\nscene->HasMaterials() = %d\nscene->HasMeshes() = %d\nscene->HasTextures() = %d\n",
//           scene->HasLights(), scene->HasMaterials(),scene->HasMeshes(),scene->HasTextures());
//    
//    printf("scene->mNumMaterials = %d\nscene->mNumMeshes = %d\nscene->mNumTextures = %d\n",
//           scene->mNumMaterials,scene->mNumMeshes,scene->mNumTextures);
    

    //////////////////////////////////////////////////////////////////////////
    // TODO: LOAD YOUR SHADERS/TEXTURES
    //////////////////////////////////////////////////////////////////////////
    
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

    
//    // Load the vertex shader
//    shader1.reset(new Shader("shaders/phong"));
//	if (!shader1->loaded()) {
//		std::cerr << "Shader failed to load" << std::endl;
//		std::cerr << shader1->errors() << std::endl;
//		exit(-1);
//	}
//    
//    // Load the vertex shader
//    shader2.reset(new Shader("shaders/phongDemo"));
//	if (!shader2->loaded()) {
//		std::cerr << "Shader failed to load" << std::endl;
//		std::cerr << shader2->errors() << std::endl;
//		exit(-1);
//	}
    
    Shader * shader1 = new Shader("shaders/phong");
    shaders.push_back(shader1);
    
    Shader * shader2 = new Shader("shaders/phongDemo");
    shaders.push_back(shader2);
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
    //////////////////////////////////////////////////////////////////////////
    // TODO: ADD YOUR INPUT HANDLING HERE.
    //////////////////////////////////////////////////////////////////////////

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
//                    case sf::Key::W:
//                        //                        current_location += .3*current_forward;
//                        printf("W down\n");
//                        go_forward = true;
//                        break;
//                    case sf::Key::S:
//                        //                        current_location -= .3*current_forward;
//                        go_backward = true;
//                        break;
//                        
//                    case sf::Key::A:
//                        current_location -= .3*current_right;
//                        break;
//                    case sf::Key::D:
//                        current_location += .3*current_right;
//                        break;
//                        
//                    case sf::Key::Space:
//                        current_location += .3*Up;
//                        break;
//                    case sf::Key::C:
//                        current_location -= .3*Up;
//                        break;
//                        
//                    case sf::Key::Left:
//                        printf("Left Arrow\n");
//                        break;
//                    case sf::Key::Right:                        
//                        printf("Left Arrow\n");
//                        break;
                        
                    default:
                        break;
                }
//                
//            case sf::Event::KeyReleased:
//                switch (evt.Key.Code) {
//                    case sf::Key::W:
//                        printf("W up\n");
//                        go_forward = false;
//                        break;
//                    case sf::Key::S:
//                        go_backward = false;
//                        break;
//                        
//                    case sf::Key::A:
//                        
//                        break;
//                    case sf::Key::D:
//                        
//                        break;
//                        
//                    case sf::Key::Space:
//                        
//                        break;
//                    case sf::Key::C:
//                        
//                        break;
//                        
//                    default:
//                        break;
//                }

                
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
            
            
            sf::Image * tmp_diff, * tmp_spec, * tmp_norm;
            
//            if (diffuseMap->GetWidth() == 0) {
//                diffuseMap->LoadFromFile(diff.data);
//            }
            
            bool d, s, n;
            d = tmp_diff->LoadFromFile(diff.data);
            s = tmp_spec->LoadFromFile(spec.data);
            n = tmp_norm->LoadFromFile(norm.data);
            
            printf("Prefix: %s, d = %d, s = %d, n = %d\n",prefix.data, d, s, n);
            
//            sf::Image * diffuseMap = &TextureMap[diff.data];
//            
//            // don't load it if it's already been loaded!
//            if (diffuseMap->GetWidth() == 0) {
//                diffuseMap->LoadFromFile(diff.data);
//            }
//            
//            newMesh.diff_string = diff;
        }
        else {
            newMesh.diff_string = aiString("");
        }
        
        
            
            
//        aiString* spec = new aiString(*prefix);
//        spec->Append("_s.jpg");
//
//        
//        std::auto_ptr<sf::Image> diffuseMap, specularMap;
//        
//        diffuseMap.reset(new sf::Image());
//        diffuseMap->LoadFromFile(diff->data);
//        
//        specularMap.reset(new sf::Image());
//        specularMap->LoadFromFile(spec->data);
        
//        diffuseMap.reset(new sf::Image(white));
//        specularMap.reset(new sf::Image(white));
//        
        currentMesh_vec->push_back(newMesh);
	}
    
	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_load_meshes(sc, nd->mChildren[n]);
	}
    
	glPopMatrix();
}


//////////////////// from DEMO /////////////////////
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
}


void applyMatrixTransform(const struct aiNode* nd) {
	struct aiMatrix4x4 m = nd->mTransformation;
    
	// update transform    
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
}

//////////////////// from DEMO //////////////////////////
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
    
    // Add a little rotation, using the elapsed time for smooth animation
//    static float elapsed = 0.0f;
//    elapsed += clck.GetElapsedTime();
//    clck.Reset();
    
    glTranslatef(10, 0, 0);
    applyMatrixTransform(scene->mRootNode);
}


//////////////////// from DEMO //////////////////////////
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
        glUniform1f(shininess, 20);
    }
}


//////////////////// from DEMO //////////////////////////
void setTextures(int meshNum, int shaderNum) {
    sf::Image * diffMap;
    
    if ((*currentMesh_vec)[meshNum].diff_string != aiString("")) {
        diffMap = &TextureMap[(*currentMesh_vec)[meshNum].diff_string.data];
//        printf("meshes[meshNum].diff_string = %s\n", meshes[meshNum].diff_string.data);
//        printf("GetWidth() = %d, GetHeight() = %d\n", diffMap->GetWidth(), diffMap->GetHeight());
    }
    else {
        diffMap = &white;
//        printf("whiteness!\n");
    }
        
    // Get a "handle" to the texture variables inside our shader.  Then 
    // pass two textures to the shader: one for diffuse, and the other for
    // specular.
    GLint diffuse = glGetUniformLocation(shaders[shaderNum]->programID(), "diffuseMap");
    glUniform1i(diffuse, 0); // The diffuse map will be GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    diffMap->Bind();
//    diffuseMap->Bind();
    
    GLint specular = glGetUniformLocation(shaders[shaderNum]->programID(), "specularMap");
    glUniform1i(specular, 1); // The transparency map will be GL_TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    diffMap->Bind();
//    specularMap->Bind();
}


void renderFrame() {
    // Always clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //////////////////////////////////////////////////////////////////////////
    // TODO: ADD YOUR RENDERING CODE HERE.  You may use as many .cpp files 
    // in this assignment as you wish.
    //////////////////////////////////////////////////////////////////////////
    

    //////////////////// from DEMO //////////////////////////
    int shaderNum;
    
    currentMesh_vec = &meshes_1;
    shaderNum = 0;
    for (int i = 0; i < currentMesh_vec->size(); i++) {

        glUseProgram(shaders[shaderNum]->programID());
        
        setMatrices(scene1);
        
        setMaterial(scene1, i, shaderNum);
        setTextures(i, shaderNum);
        setMeshData(i, shaderNum);
        
        // Draw the mesh
        if (i != 3 && i != 24 && i != 5 && i != 30 && i != 36 && i != 26 && i != 28 && i != 39 && i != 44) {
            glDrawElements(GL_TRIANGLES, 3*(*currentMesh_vec)[i].mesh->mNumFaces, GL_UNSIGNED_INT, &(*currentMesh_vec)[i].indexBuffer[0]);
        }
    }
    
    currentMesh_vec = &meshes_2;
    shaderNum = 1;
    for (int i = 0; i < currentMesh_vec->size(); i++) {
        
        glUseProgram(shaders[shaderNum]->programID());
        
        setMatrices(scene2);
        
        setMaterial(scene2, i, shaderNum);
        setTextures(i, shaderNum);
        setMeshData(i, shaderNum);
        
        // Draw the mesh
        if (i != 3 && i != 24 && i != 5 && i != 30 && i != 36 && i != 26 && i != 28 && i != 39 && i != 44) {
            glDrawElements(GL_TRIANGLES, 3*(*currentMesh_vec)[i].mesh->mNumFaces, GL_UNSIGNED_INT, &(*currentMesh_vec)[i].indexBuffer[0]);
        }
    }

    
    
}


