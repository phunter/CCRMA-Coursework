// CS 248 Final Project
//
// Hunter McCurry Winter 2012

#include "Framework.h"
#include "Shader.h"
#include "MultiSampleRenderTarget.h"
#include "DepthRenderTarget.h"

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "ip/IpEndpointName.h"
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"

#include "Note.h"
#include "Graph.h"
#include "Camera.h"
#include "NoteMessage.h"
#include "Dissonance.h"
#include "CustomMaterial.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
//#define WIN_WIDTH 740
//#define WIN_HEIGHT 480
//#define FS_WIN_WIDTH 1920
//#define FS_WIN_HEIGHT 1200
#define FS_WIN_WIDTH 1024
#define FS_WIN_HEIGHT 768

// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);

// fullscreen or no?
//sf::Window window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Final Project", sf::Style::Close, settings);
sf::Window window(sf::VideoMode(FS_WIN_WIDTH, FS_WIN_HEIGHT, 32), "Final Project", sf::Style::Fullscreen, settings);

// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock graphics_clock, audio_clock;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
//Assimp::Importer importer;
//const aiScene* scene;


// Set up OSC addresses, ports
char *hostName = (char*)"192.168.1.5";
int outPortNum = 7001;
int inPortNum = 7000;

#define IP_MTU_SIZE 1536

// Note Things
// VIEWABLE RANGE = MIDI NOTE 47 (LOW) TO 94 (HIGH)
// Range of 48 possible notes
int midi_offset = 47; // map midi note 47 to note 0
int max_notes = 48;   // number of representable notes
Graph *graph;

// window dimensions
int window_width;
int window_height;

vector <Note*> g_notes(max_notes);
Note * curNote;

int curMidiNote = -1;

//bool fixedPipeline = true;
bool fixedPipeline = false;

bool test_quad = false;
bool toFile = false;

// MultiSample stuff
aiVector2D randomVar = aiVector2D(.00001, .51234);

int multiSampleAmount = 3;
MultiSampleRenderTarget *multiSampleRenderTarget;

// end MultiSample stuff

GLuint scene_list = 0;
sf::Image * paperTexture;

// shader things
std::vector<Shader*> shaders;

typedef vector<int> IntContainer;
typedef IntContainer::iterator IntIterator;
IntContainer connection_list;

// clock stuff
float lastGraphicsTime = 0.0;
float lastAudioTime = 0.0;

// Dissonance Matrix
Dissonance * dissonance;

// Message Things
queue <NoteMessage*> g_messages;

Camera *cam;

struct testVertex
{
	aiVector3D pos;
	aiVector3D norm;
    aiVector2D texcoord;
};


// function prototypes
void initOpenGL();
void loadAssets();
void loadTexture();
void handleInput();

// temporary
void Display_FixedPipeline();

void renderFrame();
void updateAudioConnections();
void triggerAudio(int note_number, int on);
void updateState();

void ReadMessage();

void setupLights();


////////////////////////////////////////////////////////////////
namespace osc{
	
	class PacketListener : public OscPacketListener{
	protected:
		
		virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                    const IpEndpointName& remoteEndpoint )
        {
            try{
                // example of parsing single messages. osc::OsckPacketListener
                // handles the bundle traversal.
                
                if( strcmp( m.AddressPattern(), "/test" ) == 0 ){
                    // example #1 -- argument stream interface
                    osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                    float a3;
                    args >> a3 >> osc::EndMessage;
                    
                    std::cout << "received '/test' message with arguments: "
                    << a3 << "\n";
                    
                }
				else if( strcmp( m.AddressPattern(), "/note" ) == 0 ){
                    // example #2 -- argument iterator interface, supports
                    // reflection for overloaded messages (eg you can call 
                    // (*arg)->IsBool() to check if a bool was passed etc).
                    osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                    int a1 = (arg++)->AsInt32();
                    int a2 = (arg++)->AsInt32();
                    if( arg != m.ArgumentsEnd() )
                        throw osc::ExcessArgumentException();
                    
                    NoteMessage *note_message = new NoteMessage;
                    note_message->note_num = a1;
                    note_message->note_vel = a2;
                    
                    g_messages.push(note_message);
                    
                    std::cout << "received '/note' message with arguments: "
                    << a1 << " " << a2 << "\n";
                }
				else if( strcmp( m.AddressPattern(), "/new" ) == 0 ){
                    graph->Reset(); // start anew
					curMidiNote = -1;
                }
            }catch( osc::Exception& e ){
                // any parsing errors such as unexpected argument types, or 
                // missing arguments get thrown as exceptions.
                printf("error while parsing message: unknown");
            }
        }
	};
    
	void SendMyNote( int midiPitch, int midiVelocity )
	{
		IpEndpointName host( hostName, outPortNum );
		
		char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
		host.AddressAsString( hostIpAddress );
		
		char buffer[IP_MTU_SIZE];
		osc::OutboundPacketStream p( buffer, IP_MTU_SIZE );
		UdpTransmitSocket socket( host );
		
		p.Clear();
		p << osc::BeginMessage( "/midi" ) << midiPitch << midiVelocity << osc::EndMessage;
		socket.Send( p.Data(), p.Size() );
	}
    
    void SendClearBuffers()
	{
		IpEndpointName host( hostName, outPortNum );
		
		char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
		host.AddressAsString( hostIpAddress );
		
		char buffer[IP_MTU_SIZE];
		osc::OutboundPacketStream p( buffer, IP_MTU_SIZE );
		UdpTransmitSocket socket( host );
		
		p.Clear();
		p << osc::BeginMessage( "/wipe" ) << osc::EndMessage;
		socket.Send( p.Data(), p.Size() );
	}

	
} // namespace osc
////////////////////////////////////////////////////////////////

void *OscThread(void *ptr)
{
    // Set up OSC Reciever
    osc::PacketListener listener;
    UdpListeningReceiveSocket s(IpEndpointName( IpEndpointName::ANY_ADDRESS, inPortNum ),
								&listener );
    
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);
    
    s.Run();
    
    pthread_exit(NULL);
}

void ReadMessage() {
    NoteMessage *note_message = g_messages.front();
    
    int note = note_message->note_num;
    int vel = note_message->note_vel;
    
    //printf("message received is 'note %d, vel %d'\n", note, vel);
	
    g_messages.pop();
	
	if (note >= 0) {
	
	    graph->AddConnectExcite(note, 0.1 + .2 * (vel / 127.0));
	    
	    if (note != curMidiNote) {
	        //triggerAudio(curMidiNote, 0);
	        curMidiNote = note;
	        //triggerAudio(curMidiNote, 1);
	    }
	    
	    graph->SetCurrentNote(note);
	    cam->setTargetNote(graph->GetCurrentNote());
	    
	    //cam->setTargetNote(graph->GetNote(note));
	}
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

	window_width = window.GetWidth();
	window_height = window.GetHeight();
	
    // This initializes OpenGL with some common defaults.  More info here:
    // http://www.sfml-dev.org/tutorials/1.6/window-opengl.php
    glClearDepth(1.0f);
    glClearColor(.4f, .4f, .424f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    glViewport(0, 0, window_width, window_height);
    
    // fixed pipeline
    if (fixedPipeline) {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }
        
    setupLights();
    
    multiSampleRenderTarget = new MultiSampleRenderTarget(window_width*multiSampleAmount, window_height*multiSampleAmount);
}

void loadAssets() {
    // Read in an asset file, and do some post-processing.  There is much 
    // more you can do with this asset loader, including load textures.
    // More info is here:
    // http://assimp.sourceforge.net/lib_html/usage.html
        
    //////////////////////////////////////////////////////////////////////////
    // TODO: LOAD YOUR SHADERS/TEXTURES
    //////////////////////////////////////////////////////////////////////////
 	loadTexture();
 
    Shader * shader1 = new Shader("shaders/outlineToon");
    shaders.push_back(shader1); // 0
    
    Shader * shader2 = new Shader("shaders/specToon");
    shaders.push_back(shader2); // 1
    
    Shader * shader3 = new Shader("shaders/downsample");
    shaders.push_back(shader3); // 2
	
	Shader * shader4 = new Shader("shaders/phongTexture");
    shaders.push_back(shader4); // 3
	
	Shader * shader5 = new Shader("shaders/backgroundFuzz");
    shaders.push_back(shader5); // 4
}

void setupLights()
{
    // some sexy values for light source 0
    GLfloat light0_position[] = { 0.0, 5.0, 0.0, 1.0 };
    GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shininess = 40;
    glLightfv( GL_LIGHT0, GL_AMBIENT, light0_ambient );
    glLightfv( GL_LIGHT0, GL_POSITION, light0_position );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light0_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light0_specular );
    glLightfv( GL_LIGHT0, GL_SHININESS, &shininess );
}

void loadTexture( void )	
{
    
    //aiString parchment = aiString("models/parchment.jpg");
	//aiString parchment = aiString("/Users/phunter8/CCRMA-Coursework/winter_12/248/final/248-ScoreGame/models/paper/smoothish_paper.jpg");
	aiString parchment = aiString("/Users/phunter8/CCRMA-Coursework/winter_12/248/final/248-ScoreGame/models/paper/parchment_png.png");
	//aiString parchment = aiString("/Users/phunter8/CCRMA-Coursework/winter_12/248/final/248-ScoreGame/models/parchment_png.png");
	//aiString parchment = aiString("/Users/phunter8/CCRMA-Coursework/winter_12/248/final/248-ScoreGame/models/red_noise.jpg");
	//aiString parchment = aiString("/Users/phunter8/CCRMA-Coursework/winter_12/248/final/248-ScoreGame/models/tanFuzz2.jpg");
	//aiString parchment = aiString("/Users/phunter8/CCRMA-Coursework/winter_12/248/final/248-ScoreGame/models/dirty_parchment_by_swimmingintheether.jpg");

    paperTexture = new sf::Image();
    bool loaded = paperTexture->LoadFromFile(parchment.data);
    
    if( loaded )
	{
	
		//glGenTextures( 1, &paperTextureID );
        
		//glBindTexture(GL_TEXTURE_2D, paperTextureID);
		paperTexture->Bind();
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        
		//glTexImage2D( GL_TEXTURE_2D, 0, 3, background->GetWidth(),background->GetHeight(), 0,
        //             GL_RGB, GL_UNSIGNED_BYTE,background-> );
	}
    
}


void handleInput() {
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
            case sf::Event::KeyPressed:
                NoteMessage *note_message;
                switch (evt.Key.Code) {
                    case sf::Key::Escape:
                        window.Close();
                        break;
                    case sf::Key::W:
                        graph->Clear();
                        graph->SetCurrentNote(-1);
                        osc::SendMyNote( -1, 0 ); // all notes off
                        osc::SendClearBuffers();
                        break;
                    case sf::Key::T:
                        test_quad = !test_quad;
                        break;
                    case sf::Key::F:
                        toFile = true;
                        break;
                        
//                    case sf::Key::P:
//                        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//                        break;
//                                            
                    case sf::Key::Comma:
                        cam->UpdateDefaultHeight(cam->GetDefaultHeight() + .1);
                        break;
                    case sf::Key::Period:
                        cam->UpdateDefaultHeight(cam->GetDefaultHeight() - .1);
                        break;
                    case sf::Key::Num1:
                        note_message = new NoteMessage;
                        note_message->note_num = 8;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num2:
                        note_message = new NoteMessage;
                        note_message->note_num = 21;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num3:
                        note_message = new NoteMessage;
                        note_message->note_num = 36;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num4:
                        note_message = new NoteMessage;
                        note_message->note_num = 31;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num5:
                        note_message = new NoteMessage;
                        note_message->note_num = 24;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num6:
                        note_message = new NoteMessage;
                        note_message->note_num = 25;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num7:
                        note_message = new NoteMessage;
                        note_message->note_num = 26;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num8:
                        note_message = new NoteMessage;
                        note_message->note_num = 27;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num9:
                        note_message = new NoteMessage;
                        note_message->note_num = 28;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num0:
                        note_message = new NoteMessage;
                        note_message->note_num = 29;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
						break;
                    case sf::Key::A:
                        osc::SendMyNote( 22, 15 );
                        break;
                    default:
                        break;
                }                
                
            default: 
                break;
        }
    }
}

void setMaterial(int shaderNum) {
    
    GLfloat diff_color[] = { .0, .6, .6, 1.0 };
    GLfloat spec_color[] = { .25, .25, .25, 1.0 };
    GLfloat amb_color[] = { .1, .1, .1, 1.0 };
    GLfloat shiny = 40.0;

    
    // Get a handle to the diffuse, specular, and ambient variables
    // inside the shader.  Then set them with the diffuse, specular, and
    // ambient color.
    
    GLint diffuse = glGetUniformLocation(shaders[shaderNum]->programID(), "Kd");
    glUniform3f(diffuse, diff_color[0], diff_color[1], diff_color[2]);
    
    // Specular material
    GLint specular = glGetUniformLocation(shaders[shaderNum]->programID(), "Ks");
    glUniform3f(specular, spec_color[0], spec_color[1], spec_color[2]);
    
    // Ambient material
    GLint ambient = glGetUniformLocation(shaders[shaderNum]->programID(), "Ka");
    glUniform3f(ambient, amb_color[0], amb_color[1], amb_color[2]);
    
    // Specular power
    GLint shininess = glGetUniformLocation(shaders[shaderNum]->programID(), "alpha");
    glUniform1f(shininess, shiny);
}

void DrawRects() {
    glBegin(GL_QUADS);
    
    glColor4f(0.3,0.6,0.3,0.6);
    glVertex3f(-3.0, -3.0, 0.0);
    glVertex3f(-3.0, 3.0, 0.0);
    glVertex3f(3.0, 3.0, 0.0);
    glVertex3f(3.0, -3.0, 0.0);

    glColor4f(0.6,0.3,0.3,0.6);
    glVertex3f(-3.0, 3.0, 0.0);
    glVertex3f(-3.0, 6.0, 0.0);
    glVertex3f(3.0, 6.0, 0.0);
    glVertex3f(3.0, 3.0, 0.0);
    
    glColor4f(0.3,0.6,0.6,0.6);
    glVertex3f(6.0, -3.0, 0.0);
    glVertex3f(6.0, 3.0, 0.0);
    glVertex3f(3.0, 3.0, 0.0);
    glVertex3f(3.0, -3.0, 0.0);
    
    glColor4f(0.3,0.3,0.6,0.6);
    glVertex3f(-3.0, -3.0, 0.0);
    glVertex3f(-3.0, -6.0, 0.0);
    glVertex3f(3.0, -6.0, 0.0);
    glVertex3f(3.0, -3.0, 0.0);
    
    glColor4f(0.6,0.3,0.6,0.6);
    glVertex3f(-3.0, -3.0, 0.0);
    glVertex3f(-6.0, -3.0, 0.0);
    glVertex3f(-6.0, 3.0, 0.0);
    glVertex3f(-3.0, 3.0, 0.0);

    glEnd();
}

void FuzzRects() {
	
	// set up shader
    int shaderNum = 4; // downsample shader
    glUseProgram(shaders[shaderNum]->programID());

	// set up vertices
    int num_vertices = 6;
    testVertex * my_vertices;
    my_vertices = new testVertex[num_vertices];

    // specify vertex locations
    my_vertices[0].pos = aiVector3D(-100.0, -100.0, 0.0);
    my_vertices[1].pos = aiVector3D(-100.0, 100.0, 0.0);
    my_vertices[2].pos = aiVector3D(100.0, 100.0, 0.0);
	
    my_vertices[3].pos = aiVector3D(100.0, 100.0, 0.0);
    my_vertices[4].pos = aiVector3D(100.0, -100.0, 0.0);
    my_vertices[5].pos = aiVector3D(-100.0, -100.0, 0.0);
	
    // specify normal directions
    my_vertices[0].norm = aiVector3D(-1.0, -1.0, 0.1);
    my_vertices[1].norm = aiVector3D(-1.0, 1.0, 0.1);
    my_vertices[2].norm = aiVector3D(1.0, 1.0, 0.1);
    
    my_vertices[3].norm = aiVector3D(1.0, 1.0, 0.1);
    my_vertices[4].norm = aiVector3D(1.0, -1.0, 0.1);
    my_vertices[5].norm = aiVector3D(-1.0, -1.0, 0.1);

    // specify texcoord locations
    my_vertices[0].texcoord = aiVector2D(0.0, 0.0);
    my_vertices[1].texcoord = aiVector2D(0.0, 100.0);
    my_vertices[2].texcoord = aiVector2D(10.0, 10.0);
    
    my_vertices[3].texcoord = aiVector2D(10.0, 10.0);
    my_vertices[4].texcoord = aiVector2D(10.0, 0.0);
    my_vertices[5].texcoord = aiVector2D(0.0, 0.0);

	paperTexture->Bind();
	// attach texture
	GLint texture = glGetUniformLocation(shaders[shaderNum]->programID(), "diffuseMap");
    glUniform1i(texture, 3); // The normal map will be GL_TEXTURE2
    glActiveTexture(GL_TEXTURE3);
	
	paperTexture->Bind();
	//glBindTexture(GL_TEXTURE_2D, paperTextureID);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

//	GLint texture = glGetUniformLocation(shaders[shaderNum]->programID(), "diffuseMap");
//    glUniform1i(texture, 4); // The normal map will be GL_TEXTURE2
//    glActiveTexture(GL_TEXTURE4);
//	paperTexture->Bind();
//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // send downsample amount 
    GLint downAmnt = glGetUniformLocation(shaders[shaderNum]->programID(), "downAmount");
    glUniform1f(downAmnt, 1);
    
    // randomizer
    //help += .0123456789;
    randomVar.x += sin(randomVar.y);
    randomVar.y += sin(randomVar.x);
    
    GLint randInc = glGetUniformLocation(shaders[shaderNum]->programID(), "randomInc");
    glUniform2f(randInc, randomVar.x, randomVar.y);
    
    // send window size 
    GLint winSize = glGetUniformLocation(shaders[shaderNum]->programID(), "targetRes");
    glUniform2f(winSize, window_width, window_height);
    
    
    GLint position = glGetAttribLocation(shaders[shaderNum]->programID(), "positionIn");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->pos);
    
    GLint texcoord = glGetAttribLocation(shaders[shaderNum]->programID(), "texcoordIn");
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->texcoord); // use norm holder as hack for now
    
    glDrawArrays(GL_TRIANGLES,0,num_vertices); 
	
	///////////////////////////
	    
//    CustomMaterial *mat = new CustomMaterial;
//    
//    mat->amb_color[0] = .3 + .4 ;
//    mat->amb_color[1] = .3 + .4 ;
//    mat->amb_color[2] = .3 + .4 ;
//    mat->diff_color[0] = .3 ;
//    mat->diff_color[1] = .3 ;
//    mat->diff_color[2] = .3 ;
//    mat->spec_color[0] = mat->spec_color[1] = mat->spec_color[2] = .1;
//    mat->shiny = 1;
//    
//    GLint diffuse = glGetUniformLocation(shaders[shaderNum]->programID(), "Kd");
//    glUniform3f(diffuse, mat->diff_color[0], mat->diff_color[1], mat->diff_color[2]);
//    
//    // Specular material
//    GLint specular = glGetUniformLocation(shaders[shaderNum]->programID(), "Ks");
//    glUniform3f(specular, mat->spec_color[0], mat->spec_color[1], mat->spec_color[2]);
//    
//    // Ambient material
//    GLint ambient = glGetUniformLocation(shaders[shaderNum]->programID(), "Ka");
//    glUniform3f(ambient, mat->amb_color[0], mat->amb_color[1], mat->amb_color[2]);
//    
//    // Specular power
//    GLint shininess = glGetUniformLocation(shaders[shaderNum]->programID(), "alpha");
//    glUniform1f(shininess, mat->shiny);
//
//    //setMaterial(shaderNum);
//    
//	GLint texcoord = glGetAttribLocation(shaders[shaderNum]->programID(), "texcoordIn");
//    glEnableVertexAttribArray(texcoord);
//    glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->texcoord); // use norm holder as hack for now
//	
//    GLint position = glGetAttribLocation(shaders[shaderNum]->programID(), "positionIn");
//    glEnableVertexAttribArray(position);
//    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->pos);
//    
//    GLint normal = glGetAttribLocation(shaders[shaderNum]->programID(), "normalIn");
//    glEnableVertexAttribArray(normal);
//    glVertexAttribPointer(normal, 3, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->norm);
		  
    glDrawArrays(GL_TRIANGLES,0,num_vertices); 
}

void TestRects() {

    int num_vertices = 6;
    testVertex * my_vertices;
    my_vertices = new testVertex[num_vertices];

    // specify vertex locations
    my_vertices[0].pos = aiVector3D(-100.0, -100.0, 0.0);
    my_vertices[1].pos = aiVector3D(-100.0, 100.0, 0.0);
    my_vertices[2].pos = aiVector3D(100.0, 100.0, 0.0);
	
    my_vertices[3].pos = aiVector3D(100.0, 100.0, 0.0);
    my_vertices[4].pos = aiVector3D(100.0, -100.0, 0.0);
    my_vertices[5].pos = aiVector3D(-100.0, -100.0, 0.0);
	
    // specify normal directions
    my_vertices[0].norm = aiVector3D(-1.0, -1.0, 0.1);
    my_vertices[1].norm = aiVector3D(-1.0, 1.0, 0.1);
    my_vertices[2].norm = aiVector3D(1.0, 1.0, 0.1);
    
    my_vertices[3].norm = aiVector3D(1.0, 1.0, 0.1);
    my_vertices[4].norm = aiVector3D(1.0, -1.0, 0.1);
    my_vertices[5].norm = aiVector3D(-1.0, -1.0, 0.1);

    // specify texcoord locations
    my_vertices[0].texcoord = aiVector2D(0.0, 0.0);
    my_vertices[1].texcoord = aiVector2D(0.0, 70.0);
    my_vertices[2].texcoord = aiVector2D(70.0, 70.0);
    
    my_vertices[3].texcoord = aiVector2D(70.0, 70.0);
    my_vertices[4].texcoord = aiVector2D(70.0, 0.0);
    my_vertices[5].texcoord = aiVector2D(0.0, 0.0);

    int shaderNum = 3;
	glUseProgram(shaders[shaderNum]->programID());
	    
    CustomMaterial *mat = new CustomMaterial;
    
    mat->amb_color[0] = .4;
    mat->amb_color[1] = .4;
    mat->amb_color[2] = .35;
    mat->diff_color[0] = .4;
    mat->diff_color[1] = .3;
    mat->diff_color[2] = .3;
    mat->spec_color[0] = mat->spec_color[1] = mat->spec_color[2] = .1;
    mat->shiny = 1;
    
    GLint diffuse = glGetUniformLocation(shaders[shaderNum]->programID(), "Kd");
    glUniform3f(diffuse, mat->diff_color[0], mat->diff_color[1], mat->diff_color[2]);
    
    // Specular material
    GLint specular = glGetUniformLocation(shaders[shaderNum]->programID(), "Ks");
    glUniform3f(specular, mat->spec_color[0], mat->spec_color[1], mat->spec_color[2]);
    
    // Ambient material
    GLint ambient = glGetUniformLocation(shaders[shaderNum]->programID(), "Ka");
    glUniform3f(ambient, mat->amb_color[0], mat->amb_color[1], mat->amb_color[2]);
    
    // Specular power
    GLint shininess = glGetUniformLocation(shaders[shaderNum]->programID(), "alpha");
    glUniform1f(shininess, mat->shiny);

    //setMaterial(shaderNum);
    
	GLint texcoord = glGetAttribLocation(shaders[shaderNum]->programID(), "texcoordIn");
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->texcoord); // use norm holder as hack for now
	
    GLint position = glGetAttribLocation(shaders[shaderNum]->programID(), "positionIn");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->pos);
    
    GLint normal = glGetAttribLocation(shaders[shaderNum]->programID(), "normalIn");
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->norm);
	
	GLint texture = glGetUniformLocation(shaders[shaderNum]->programID(), "diffuseMap");
    glUniform1i(texture, 3); // The normal map will be GL_TEXTURE2
    glActiveTexture(GL_TEXTURE3);
	
	paperTexture->Bind();
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	  
    glDrawArrays(GL_TRIANGLES,0,num_vertices); 
}

void SaveImgToFile() {
    // Copy the back buffer into the current face of the cube map
    glBindTexture(GL_TEXTURE_2D, multiSampleRenderTarget->textureID());
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, multiSampleRenderTarget->fboID());
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width*multiSampleAmount, window_height*multiSampleAmount, 0, GL_RGBA, GL_FLOAT, NULL);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, window_width*multiSampleAmount, window_height*multiSampleAmount);
    
    // for rendering to image (debugging)
    sf::Uint8 *pixelArray = new sf::Uint8[window_width*multiSampleAmount*window_height*multiSampleAmount*4];
    
    // use ONE of the two following lines:
    // This line will copy images from the framebuffer
    //glReadPixels(0, 0, window_width, window_height, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
    
    // This line will copy images stored in the currently bound texture
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
    
    // FOR SAVING TO FILE
    std::ostringstream out;
    out << "/Users/phunter8/test_img2.jpg";
    sf::Image img(window_width*multiSampleAmount, window_height*multiSampleAmount, sf::Color::White);
    img.LoadFromPixels(window_width*multiSampleAmount, window_height*multiSampleAmount, pixelArray);
    img.SaveToFile(out.str());
    
    toFile = false;
    
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}



void Display_FixedPipeline() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	cam->CallLookAt(window_width / window_height);
    
    aiVector3D * cam_pos = cam->getPosition();
    
    //DrawRects();
    graph->Display(cam_pos->z);
}

void renderFrame() {
    
	
	float aspect = (float)window_width / (float)window_height;
	
	cam->CallLookAt(aspect);    
    
    ////////////////////////////// first pass //////////////////
    // set rendering destination to FBO
    glViewport(0, 0, window.GetWidth()*multiSampleAmount, window.GetHeight()*multiSampleAmount);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, multiSampleRenderTarget->fboID());
    multiSampleRenderTarget->bind();
    
    // clear buffers
    //glClearColor(.9f, .9f, .8f, 1.0f);
	//glClearColor(.972f, .972f, .972f, 1.0f);
	glClearColor(.4f, .4f, .424f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw a scene to a texture directly
    graph->Render();
    
	//FuzzRects();
	TestRects();
    
    if (toFile) {
        SaveImgToFile();
    }
    
    /////////////////////////////////////////second pass
    glViewport(0, 0, window_width, window_height);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // default framebuffer (window)
    
    // clear buffers
    glClearColor(.1f, .2f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // set up shader
    int shaderNum = 2; // downsample shader
    glUseProgram(shaders[shaderNum]->programID());

    //attach texture 
    GLint bigTex = glGetUniformLocation(shaders[shaderNum]->programID(), "bigTexture");
    glUniform1i(bigTex, 0); // The environment map will be GL_TEXTURE3
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, multiSampleRenderTarget->textureID());
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // send downsample amount 
    GLint downAmnt = glGetUniformLocation(shaders[shaderNum]->programID(), "downAmount");
    glUniform1f(downAmnt, multiSampleAmount);
    
    // randomizer
    //help += .0123456789;
    randomVar.x += sin(randomVar.y);
    randomVar.y += sin(randomVar.x);
    
    GLint randInc = glGetUniformLocation(shaders[shaderNum]->programID(), "randomInc");
    glUniform2f(randInc, randomVar.x, randomVar.y);
    
    // send window size 
    GLint winSize = glGetUniformLocation(shaders[shaderNum]->programID(), "targetRes");
    glUniform2f(winSize, window_width, window_height);
    
    int num_vertices = 6;
    testVertex * my_vertices;
    my_vertices = new testVertex[num_vertices];
    
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
    //glOrtho(-window_width,window_width,-window_height,window_height,1,20);
	
	
	//GLfloat nearClip = .1f;
	//GLfloat farClip = 50.f;
	//GLfloat fieldOfView = 90.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( -1, 1, -1, 1, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    glLoadIdentity();
    gluLookAt(0., 0., 2., 0., 0., 0., 0., 1., 0.);
	
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
    
    // specify vertex locations
    my_vertices[0].pos = aiVector3D(-1.0, -1.0, 0.0);
    my_vertices[1].pos = aiVector3D(-1.0, 1.0, 0.0);
    my_vertices[2].pos = aiVector3D(1.0, 1.0, 0.0);
    
    // specify texcoord locations
    my_vertices[0].texcoord = aiVector2D(0.0, 0.0);
    my_vertices[1].texcoord = aiVector2D(0.0, 1.0);
    my_vertices[2].texcoord = aiVector2D(1.0, 1.0);
    
    // specify vertex locations
    my_vertices[3].pos = aiVector3D(1.0, 1.0, 0.0);
    my_vertices[4].pos = aiVector3D(1.0, -1.0, 0.0);
    my_vertices[5].pos = aiVector3D(-1.0, -1.0, 0.0);
    
    // specify texcoord locations
    my_vertices[3].texcoord = aiVector2D(1.0, 1.0);
    my_vertices[4].texcoord = aiVector2D(1.0, 0.0);
    my_vertices[5].texcoord = aiVector2D(0.0, 0.0);
    
    GLint position = glGetAttribLocation(shaders[shaderNum]->programID(), "positionIn");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->pos);
    
    GLint texcoord = glGetAttribLocation(shaders[shaderNum]->programID(), "texcoordIn");
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(testVertex), &my_vertices->texcoord); // use norm holder as hack for now
    
    glDrawArrays(GL_TRIANGLES,0,num_vertices); 
    
    
    ////////////////////////////////////////////// end second pass
    
    // Display a test quad on screen (press t key to toggle)
    if (test_quad)
    {
        // Render test quad
        glDisable(GL_LIGHTING);
        glUseProgramObjectARB(0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-window_width/2,window_width/2,-window_height/2,window_height/2,1,20);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor4f(1,1,1,1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, multiSampleRenderTarget->textureID());
        glEnable(GL_TEXTURE_2D);
        glTranslated(0,-window_height/2,-1);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);glVertex3f(0,0,0);
        glTexCoord2d(1,0);glVertex3f(window_width/2,0,0);
        glTexCoord2d(1,1);glVertex3f(window_width/2,window_height/2,0);
        glTexCoord2d(0,1);glVertex3f(0,window_height/2,0);
        glEnd();
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        multiSampleRenderTarget->unbind();
    }
}


void updateAudioConnections() {
    
    vector<int> * updated_connection_list = new vector<int>;
    graph->GetConnections(curMidiNote, updated_connection_list);
    
    for (int i = 0; i < connection_list.size(); i++) {
        
        IntIterator itr = find(updated_connection_list->begin(), updated_connection_list->end(), i);
        
        if (connection_list[i] == 1 && itr == updated_connection_list->end()) { // currently active but not found
            connection_list[i] = 0;
            osc::SendMyNote( i, 0 );
        }
        else if (connection_list[i] == 0 && itr != updated_connection_list->end()) { // currently inactive but found
            connection_list[i] = 1;
            osc::SendMyNote( i, 1 );
        }
    }
}

void triggerAudio(int note_number, int on) {
    
    vector<int> * connection_list = new vector<int>;
    //graph->GetCurConnections(note_list);
    graph->GetConnections(note_number, connection_list);
    
    for (int i = 0; i < connection_list->size(); i++) {
        // play note i's audio via osc message
        // printf("note number is: %d\n", note_list->at(i) );
        osc::SendMyNote( connection_list->at(i), on );
    }    
}

void updateState() {
    
    static float elapsed = 0.0f;
    elapsed += graphics_clock.GetElapsedTime();
    graphics_clock.Reset();
    
    float delta = elapsed - lastGraphicsTime;
    lastGraphicsTime = elapsed;
    
    //printf("delta is %f\n", delta);
    
    delta = .002;
    
    graph->UpdateGraph(delta);
    //graph->FadeColors();
    
    cam->updateCam(delta);
    
    if (g_messages.size() > 0) {
        ReadMessage();
    }
}

int main(int argc, char** argv) {
    

    printf("Initialize output\n");
       
    // Read Dissonance Matrix from file
    dissonance = new Dissonance(std::string("diss_matrix.txt"));

    initOpenGL();
    loadAssets();
    
    pthread_t osc_thread;
    string osc_message = "Starting OSC Listen Thread";
    int osc_ret;
    osc_ret = pthread_create( &osc_thread, NULL, OscThread, (void*) &osc_message);
    
    srand( (unsigned)time( NULL ) );
    
    graph = new Graph(max_notes, dissonance, &shaders);
    
    cam = new Camera(0.0, 0.0, 1.9);
    
    connection_list.resize(max_notes);
    
    // initialize connection_list to zeros
    for (int i = 0; i < connection_list.size(); i++) {
        connection_list[i] = 0;
    }

    
    // Main Loop
    while (window.IsOpened()) {
        
        handleInput();
        updateState();
        updateAudioConnections();
        
        if (fixedPipeline) {
            // Fixed Pipeline
            Display_FixedPipeline();
        }
        else {
            // Custom Pipeline
            renderFrame();
        }
        
        window.Display();
    }
    return 0;
}
