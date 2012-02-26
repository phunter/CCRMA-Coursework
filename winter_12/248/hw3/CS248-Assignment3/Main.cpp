#include "Framework.h"
#include "Shader.h"

#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"


//#define MODEL_PATH "/Users/phunter/CCRMA-Coursework/winter_12/248/hw3/CS248-Assignment3/models/teapot.3ds"
//#define MODEL_PATH "/Users/phunter/CCRMA-Coursework/winter_12/248/hw3/CS248-Assignment3/models/cathedral.3ds"
//#define MODEL_PATH "models/dragon.dae"
#define MODEL_PATH "models/cathedral.3ds"
//#define MODEL_PATH "models/teapot.3ds"


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
const aiMesh* mesh;
std::vector<unsigned> indexBuffer;

GLuint scene_list = 0;

// current rotation angle
static float angle = 0.f;



// vertex shader
std::auto_ptr<Shader> shader;

// Texture
std::auto_ptr<sf::Image> diffuseMap;
std::auto_ptr<sf::Image> specularMap;

void initOpenGL();
void loadAssets();
void handleInput();
void setMeshData();
void renderFrame();
void setMatrices();
void setMaterial();
void setTextures();
void setMeshData();


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
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
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

    //////////////////////////////////////////////////////////////////////////
    // TODO: LOAD YOUR SHADERS/TEXTURES
    //////////////////////////////////////////////////////////////////////////
  
    printf("scene->HasLights() = %d\nscene->HasMaterials() = %d\nscene->HasMeshes() = %d\nscene->HasTextures() = %d\n",
           scene->HasLights(), scene->HasMaterials(),scene->HasMeshes(),scene->HasTextures());
    
    printf("scene->mNumMaterials = %d\nscene->mNumMeshes = %d\nscene->mNumTextures = %d\n",
           scene->mNumMaterials,scene->mNumMeshes,scene->mNumTextures);

    
    // Just render the first mesh in the imported scene file
    mesh = scene->mMeshes[0];
    
    // Set up the index buffer.  Each face should have 3 vertices since we
    // specified aiProcess_Triangulate
    indexBuffer.reserve(mesh->mNumFaces * 3);
    for (unsigned i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            //printf("indexBuffer.push_back(mesh->mFaces[%d].mIndices[%d] to position %lu\n", i,j, indexBuffer.size());
            indexBuffer.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }
	
    // Load the vertex shader
    shader.reset(new Shader("shaders/phongDemo"));
	if (!shader->loaded()) {
		std::cerr << "Shader failed to load" << std::endl;
		std::cerr << shader->errors() << std::endl;
		exit(-1);
	}
    
    // Load the textures
    diffuseMap.reset(new sf::Image());
    diffuseMap->LoadFromFile("models/dragon-diffuse.jpg");
    specularMap.reset(new sf::Image());
    specularMap->LoadFromFile("models/dragon-specular.jpg");
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
                printf("%d\n",evt.Key.Code);
                if (evt.Key.Code == sf::Key::Escape) {
                    window.Close();
                }
                if (evt.Key.Code == sf::Key::A) {
                    printf("LOLZ it's an A\n");
                }
                if (evt.Key.Code == sf::Key::Left) {
                    angle += .1;
                    printf("left\n");
                }
                if (evt.Key.Code == sf::Key::Right) {
                    printf("right\n");
                    angle -= .1;
                }
                if (evt.Key.Code == sf::Key::Up) {
                    printf("up\n");
                }
                if (evt.Key.Code == sf::Key::Down) {
                    printf("down\n");
                }
            default: 
                break;
        }
    }
}


void apply_material(const struct aiMaterial *mtl)
{
//	float c[4];
//    
//	GLenum fill_mode;
//	int ret1, ret2;
//	struct aiColor4D diffuse;
//	struct aiColor4D specular;
//	struct aiColor4D ambient;
//	struct aiColor4D emission;
//	float shininess, strength;
//	int two_sided;
//	int wireframe;
//	int max;
//    
//	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
//		color4_to_float4(&diffuse, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
//    
//	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
//		color4_to_float4(&specular, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
//    
//	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
//		color4_to_float4(&ambient, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
//    
//	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
//		color4_to_float4(&emission, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
//    
//	max = 1;
//	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
//	if(ret1 == AI_SUCCESS) {
//    	max = 1;
//    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
//		if(ret2 == AI_SUCCESS)
//			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
//        else
//        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
//    }
//	else {
//		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
//		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
//		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
//	}
//    
//	max = 1;
//	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
//		fill_mode = wireframe ? GL_LINE : GL_FILL;
//	else
//		fill_mode = GL_FILL;
//	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
//    
//	max = 1;
//	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
//		glDisable(GL_CULL_FACE);
//	else 
//		glEnable(GL_CULL_FACE);
}


// ----------------------------------------------------------------------------
void recursive_render (const struct aiScene *sc, const struct aiNode* nd)
{
	int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;
    
	// update transform    
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
    
	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		mesh = scene->mMeshes[nd->mMeshes[n]];
        
		//apply_material(sc->mMaterials[mesh->mMaterialIndex]);
//        setMaterial();
//        setTextures();
//        setMeshData();
        
		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}
        
		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;
            
			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}
            
			glBegin(face_mode);
            
			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}
            
			glEnd();
		}
	}
    
	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}
    
	glPopMatrix();
}

//////////////////// from DEMO //////////////////////////
void setMeshData() {
    // Get a handle to the variables for the vertex data inside the shader.
    GLint position = glGetAttribLocation(shader->programID(), "positionIn");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mVertices);
    
    // Texture coords.  Note the [0] at the end, very important
    GLint texcoord = glGetAttribLocation(shader->programID(), "texcoordIn");
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTextureCoords[0]);
    
    // Normals
    GLint normal = glGetAttribLocation(shader->programID(), "normalIn");
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mNormals);
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
    gluLookAt(0.0f, 2.0f, -12.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // Add a little rotation, using the elapsed time for smooth animation
    static float elapsed = 0.0f;
    elapsed += clck.GetElapsedTime();
    clck.Reset();
    glRotatef(20*angle, 0, 1, 0);
}


//////////////////// from DEMO //////////////////////////
void setMaterial() {
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
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glUniform3f(specular, color.r, color.g, color.b);
    
    // Ambient material
    GLint ambient = glGetUniformLocation(shader->programID(), "Ka");
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
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
    //glUseProgram(shader->programID());
    
    setMatrices();
//    setTextures();
    //setMaterial();
    
    // if the display list has not been made yet, create a new one and
    // fill it with scene contents
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
        // now begin at the root node of the imported data and traverse
        // the scenegraph by multiplying subsequent local transforms
        // together on GL's matrix stack.
	    recursive_render(scene, scene->mRootNode);
	    glEndList();
	}
    
    glCallList(scene_list);
    
    //setMeshData();
    
    // Draw the mesh
    
    //printf("index buffer is length %lu", indexBuffer.size());
    glDrawElements(GL_TRIANGLES, 3*mesh->mNumFaces, GL_UNSIGNED_INT, &indexBuffer[0]);
}







