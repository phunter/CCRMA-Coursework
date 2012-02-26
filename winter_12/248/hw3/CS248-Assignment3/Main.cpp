#include "Framework.h"
#include "Shader.h"

#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include "STPoint3.h"
#include "STVector3.h"

//#define MODEL_PATH "/Users/phunter/CCRMA-Coursework/winter_12/248/hw3/CS248-Assignment3/models/teapot.3ds"
//#define MODEL_PATH "/Users/phunter/CCRMA-Coursework/winter_12/248/hw3/CS248-Assignment3/models/cathedral.3ds"
//#define MODEL_PATH "models/dragon.dae"
//#define MODEL_PATH "models/teapot.3ds"
#define MODEL_PATH "models/cathedral.3ds"
//#define MODEL_PATH "models/armadillo.3ds"


// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(800, 600), "Hunter McCurry - Homework 3", sf::Style::Close, settings);
 
// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock clck;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
Assimp::Importer importer;
const aiScene* scene;

//const aiMesh* mesh;
//std::vector<unsigned> indexBuffer;

GLuint scene_list = 0;

// current rotation angle
static float h_angle = 3.14159/2;
static float v_angle = 0.0f;
STPoint3 current_location = STPoint3(0.0,2.0,0.0);
STVector3 current_forward = STVector3(sin(h_angle),0.0,cos(h_angle));


struct meshObject {
    const aiMesh* mesh;
    std::vector<unsigned> indexBuffer;
};

std::vector<meshObject> meshes;

// shader
std::auto_ptr<Shader> shader;

// Texture
std::auto_ptr<sf::Image> diffuseMap;
std::auto_ptr<sf::Image> specularMap;
sf::Image white = sf::Image(1,1,sf::Color::White);

void initOpenGL();
void loadAssets();
void handleInput();
void setMeshData(const aiMesh * mesh);
void renderFrame();
void setMatrices();
void setMaterial(const aiMesh * mesh);
void setTextures();
void setMeshData();
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
    scene = importer.ReadFile(MODEL_PATH,  
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcessPreset_TargetRealtime_Quality);

    if (!scene || scene->mNumMeshes <= 0) {
        std::cerr << importer.GetErrorString() << std::endl;
        exit(-1);
    }

    printf("scene->HasLights() = %d\nscene->HasMaterials() = %d\nscene->HasMeshes() = %d\nscene->HasTextures() = %d\n",
           scene->HasLights(), scene->HasMaterials(),scene->HasMeshes(),scene->HasTextures());
    
    printf("scene->mNumMaterials = %d\nscene->mNumMeshes = %d\nscene->mNumTextures = %d\n",
           scene->mNumMaterials,scene->mNumMeshes,scene->mNumTextures);
    

    //////////////////////////////////////////////////////////////////////////
    // TODO: LOAD YOUR SHADERS/TEXTURES
    //////////////////////////////////////////////////////////////////////////
    
    // if the display list has not been made yet, create a new one and
    // fill it with scene contents
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
        // now begin at the root node of the imported data and traverse
        // the scenegraph by multiplying subsequent local transforms
        // together on GL's matrix stack.
	    recursive_load_meshes(scene, scene->mRootNode);
	    glEndList();
	}
    
    glCallList(scene_list);
    
    
    // Load the vertex shader
    shader.reset(new Shader("shaders/phong"));
	if (!shader->loaded()) {
		std::cerr << "Shader failed to load" << std::endl;
		std::cerr << shader->errors() << std::endl;
		exit(-1);
	}
    
    // Load the textures
//    diffuseMap.reset(new sf::Image());
//    diffuseMap->LoadFromFile("models/dragon-diffuse.jpg");
//    specularMap.reset(new sf::Image());
//    specularMap->LoadFromFile("models/dragon-specular.jpg");

    diffuseMap.reset(new sf::Image(white));
    specularMap.reset(new sf::Image(white));
    
}




void handleInput() {
    //////////////////////////////////////////////////////////////////////////
    // TODO: ADD YOUR INPUT HANDLING HERE.
    //////////////////////////////////////////////////////////////////////////

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
                
                // Key presses
            case sf::Event::KeyPressed:
                switch (evt.Key.Code) {
                    case sf::Key::Escape:
                        window.Close();
                        break;
                    case sf::Key::A:
                        current_location.x += .1;
                        break;
                    case sf::Key::D:
                        current_location.x -= .1;
                        break;
                    case sf::Key::W:
                        current_location += .3*current_forward;
                        break;
                    case sf::Key::S:
                        current_location -= .3*current_forward;
                        break;
                        
                    case sf::Key::Left:
                        h_angle += .1;
                        current_forward = STVector3(sin(h_angle),0,cos(h_angle));
                        break;
                    case sf::Key::Right:
                        h_angle -= .1;
                        current_forward = STVector3(sin(h_angle),0,cos(h_angle));
                        break;
                        
                    default:
                        break;
                }
                
//                printf("%d\n",evt.Key.Code);
//                if (evt.Key.Code == sf::Key::Escape) {
//                    window.Close();
//                }
//                if (evt.Key.Code == sf::Key::A) {
//                    current_location.x += .1;
//                }
//                if (evt.Key.Code == sf::Key::Left) {
//                    angle += .1;
//                    printf("left\n");
//                }
//                if (evt.Key.Code == sf::Key::Right) {
//                    printf("right\n");
//                    angle -= .1;
//                }
//                if (evt.Key.Code == sf::Key::Up) {
//                    printf("up\n");
//                }
//                if (evt.Key.Code == sf::Key::Down) {
//                    printf("down\n");
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
		newMesh.mesh = scene->mMeshes[nd->mMeshes[n]];
        
        // Set up the index buffer.  Each face should have 3 vertices since we
        // specified aiProcess_Triangulate
        newMesh.indexBuffer.reserve(newMesh.mesh->mNumFaces * 3);
        for (unsigned i = 0; i < newMesh.mesh->mNumFaces; i++) {
            for (unsigned j = 0; j < newMesh.mesh->mFaces[i].mNumIndices; j++) {
                newMesh.indexBuffer.push_back(newMesh.mesh->mFaces[i].mIndices[j]);
            }
        }
        
        meshes.push_back(newMesh);
	}
    
	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_load_meshes(sc, nd->mChildren[n]);
	}
    
	glPopMatrix();
}

// ----------------------------------------------------------------------------
//void recursive_render(const struct aiScene *sc, const struct aiNode* nd)
//{
//	int i;
//	unsigned int n = 0, t;
//	struct aiMatrix4x4 m = nd->mTransformation;
//    
//	// update transform    
//	aiTransposeMatrix4(&m);
//	glPushMatrix();
//	glMultMatrixf((float*)&m);
//    
//	// draw all meshes assigned to this node
//	for (; n < nd->mNumMeshes; ++n) {
//		mesh = scene->mMeshes[nd->mMeshes[n]];
//        
//        setMaterial();
//        setTextures();
////        setMeshData();
//        
//		if(mesh->mNormals == NULL) {
//			glDisable(GL_LIGHTING);
//		} else {
//			glEnable(GL_LIGHTING);
//		}
//        
//		for (t = 0; t < mesh->mNumFaces; ++t) {
//			const struct aiFace* face = &mesh->mFaces[t];
//			GLenum face_mode;
//            
//			switch(face->mNumIndices) {
//				case 1: face_mode = GL_POINTS; break;
//				case 2: face_mode = GL_LINES; break;
//				case 3: face_mode = GL_TRIANGLES; break;
//				default: face_mode = GL_POLYGON; break;
//			}
//            
//			glBegin(face_mode);
//            
//			for(i = 0; i < face->mNumIndices; i++) {
//				int index = face->mIndices[i];
//				if(mesh->mColors[0] != NULL)
//					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
//				if(mesh->mNormals != NULL) 
//					glNormal3fv(&mesh->mNormals[index].x);
//				glVertex3fv(&mesh->mVertices[index].x);
//			}
//            
//			glEnd();
//		}
//	}
//    
//	// draw all children
//	for (n = 0; n < nd->mNumChildren; ++n) {
//		recursive_render(sc, nd->mChildren[n]);
//	}
//    
//	glPopMatrix();
//}

//////////////////// from DEMO /////////////////////
void setMeshData(const aiMesh * mesh) {
    // Get a handle to the variables for the vertex data inside the shader.
    GLint position = glGetAttribLocation(shader->programID(), "positionIn");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mVertices);
    
//    // Texture coords.  Note the [0] at the end, very important
//    GLint texcoord = glGetAttribLocation(shader->programID(), "texcoordIn");
//    glEnableVertexAttribArray(texcoord);
//    glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTextureCoords[0]);
    
    // Normals
    GLint normal = glGetAttribLocation(shader->programID(), "normalIn");
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
void setMatrices() {
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
    static float elapsed = 0.0f;
    elapsed += clck.GetElapsedTime();
    clck.Reset();

//    glRotatef(20*angle, 0, 1, 0);
//    glTranslatef(current_location.x, current_location.y, current_location.z);

    
    glTranslatef(10, 0, 0);
    applyMatrixTransform(scene->mRootNode);
}


//////////////////// from DEMO //////////////////////////
void setMaterial(const aiMesh * mesh) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    
    // Get a handle to the diffuse, specular, and ambient variables
    // inside the shader.  Then set them with the diffuse, specular, and
    // ambient color.
    GLint diffuse = glGetUniformLocation(shader->programID(), "Kd");
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glUniform3f(diffuse, color.r, color.g, color.b);
    
    // Specular material
    GLint specular = glGetUniformLocation(shader->programID(), "Ks");
    material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glUniform3f(specular, color.r, color.g, color.b);
    
    // Ambient material
    GLint ambient = glGetUniformLocation(shader->programID(), "Ka");
    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glUniform3f(ambient, color.r, color.g, color.b);
    
    // Specular power
    GLint shininess = glGetUniformLocation(shader->programID(), "alpha");
    float value;
    if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, value)) {
        glUniform1f(shininess, value);
    } else {
        glUniform1f(shininess, 1);
    }
}


//////////////////// from DEMO //////////////////////////
void setTextures() {
    // Get a "handle" to the texture variables inside our shader.  Then 
    // pass two textures to the shader: one for diffuse, and the other for
    // transparency.
    GLint diffuse = glGetUniformLocation(shader->programID(), "diffuseMap");
    glUniform1i(diffuse, 0); // The diffuse map will be GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    diffuseMap->Bind();
    
    // Transparency
    GLint specular = glGetUniformLocation(shader->programID(), "specularMap");
    glUniform1i(specular, 1); // The transparency map will be GL_TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    specularMap->Bind();
}



void renderFrame() {
    // Always clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //////////////////////////////////////////////////////////////////////////
    // TODO: ADD YOUR RENDERING CODE HERE.  You may use as many .cpp files 
    // in this assignment as you wish.
    //////////////////////////////////////////////////////////////////////////
    

    //////////////////// from DEMO //////////////////////////
    glUseProgram(shader->programID());
    
    for (int i = 0; i < meshes.size(); i++) {
        setMatrices();
        
        setMaterial(meshes[i].mesh);
        setTextures();
        setMeshData(meshes[i].mesh);
        
        // Draw the mesh
        if (i != 3 && i != 24 && i != 5 && i != 30 && i != 36 && i != 26 && i != 28 && i != 39 && i != 44) {
            glDrawElements(GL_TRIANGLES, 3*meshes[i].mesh->mNumFaces, GL_UNSIGNED_INT, &meshes[i].indexBuffer[0]);
        }
    }
}


