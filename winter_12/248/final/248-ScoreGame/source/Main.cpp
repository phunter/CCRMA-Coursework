// CS 248 Final Project
//
// Hunter McCurry Winter 2012

#include "Framework.h"
#include "Shader.h"

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

//#define MODEL_PATH "models/teapot.3ds"
#define WIN_WIDTH 880
#define WIN_HEIGHT 880

// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Final Project", sf::Style::Close, settings);
 
// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock graphics_clock, audio_clock;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
//Assimp::Importer importer;
//const aiScene* scene;


// Set up OSC addresses, ports
char *hostName = (char*)"localhost";
int outPortNum = 7001;
int inPortNum = 7000;

#define IP_MTU_SIZE 1536

// Note Things
// VIEWABLE RANGE = MIDI NOTE 47 (LOW) TO 94 (HIGH)
// Range of 47 possible notes
int midi_offset = 47; // map midi note 47 to note 0
int max_notes = 51;
Graph *graph;

vector <Note*> g_notes(max_notes);
Note * curNote;

// clock stuff
float lastGraphicsTime = 0.0;
float lastAudioTime = 0.0;

// Dissonance Matrix
Dissonance * dissonance;

// Message Things
queue <NoteMessage*> g_messages;

Camera *cam;


// function prototypes
void initOpenGL();
void loadAssets();
void handleInput();
void renderFrame();
void triggerAudio();
void updateState();

void ReadMessage();

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
                    
                }else if( strcmp( m.AddressPattern(), "/note" ) == 0 ){
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
		p << osc::BeginMessage( "/midi" )
		<< midiPitch << midiVelocity << osc::EndMessage;
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
    
    graph->AddConnectExcite(note, 0.1 + 1.5 * (vel / 127.0));
    
    graph->SetCurrentNote(note);
    cam->setTargetNote(graph->GetCurrentNote());
    
    //cam->setTargetNote(graph->GetNote(note));
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
    glClearColor(.9f, .9f, .8f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glFrustum (-.5, .5, -.5, .5, 0.5, 30.0);
    glMatrixMode (GL_MODELVIEW);


}

void loadAssets() {
    // Read in an asset file, and do some post-processing.  There is much 
    // more you can do with this asset loader, including load textures.
    // More info is here:
    // http://assimp.sourceforge.net/lib_html/usage.html
    //    scene = importer.ReadFile(MODEL_PATH,  
    //        aiProcess_CalcTangentSpace |
    //        aiProcess_Triangulate |
    //        aiProcess_JoinIdenticalVertices |
    //        aiProcessPreset_TargetRealtime_Quality);
    //
    //    if (!scene || scene->mNumMeshes <= 0) {
    //        std::cerr << importer.GetErrorString() << std::endl;
    //        exit(-1);
    //    }
        
    //////////////////////////////////////////////////////////////////////////
    // TODO: LOAD YOUR SHADERS/TEXTURES
    //////////////////////////////////////////////////////////////////////////
    
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
                        break;
                    case sf::Key::Comma:
                        cam->UpdateDefaultHeight(cam->GetDefaultHeight() + .1);
                        break;
                    case sf::Key::Period:
                        cam->UpdateDefaultHeight(cam->GetDefaultHeight() - .1);
                        break;
                    case sf::Key::Num1:
                        note_message = new NoteMessage;
                        note_message->note_num = 20;
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
                        note_message->note_num = 22;
                        note_message->note_vel = 80;
                        g_messages.push(note_message);
                        break;
                    case sf::Key::Num4:
                        note_message = new NoteMessage;
                        note_message->note_num = 23;
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



void renderFrame() {
    
    //glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    aiVector3D * cam_pos = cam->getPosition();
    aiVector3D * look_pos = cam->getLookAt();
    
    glLoadIdentity();
    
//    gluLookAt (cam_pos->x, cam_pos->y, cam_pos->z, cam_pos->x, cam_pos->y, 0.0, 0.0, 1.0, 0.0);
//    gluLookAt (cam_pos->x, cam_pos->y, cam_pos->z + 2.0, look_pos->x, look_pos->y, look_pos->z, 0.0, 1.0, 0.0);
    gluLookAt (cam_pos->x, cam_pos->y, cam_pos->z, look_pos->x, look_pos->y, look_pos->z, 0.0, 1.0, 0.0);
    

    glBegin(GL_QUADS);
    glColor4f(0.3,0.6,0.3,1.0);
    glVertex3f(-3.0, -3.0, 0.0);
    glVertex3f(-3.0, 3.0, 0.0);
    glVertex3f(3.0, 3.0, 0.0);
    glVertex3f(3.0, -3.0, 0.0);
    
    glColor4f(0.6,0.3,0.3,1.0);
    glVertex3f(-3.0, 3.0, 0.0);
    glVertex3f(-3.0, 6.0, 0.0);
    glVertex3f(3.0, 6.0, 0.0);
    glVertex3f(3.0, 3.0, 0.0);
    
    glColor4f(0.3,0.6,0.6,1.0);
    glVertex3f(6.0, -3.0, 0.0);
    glVertex3f(6.0, 3.0, 0.0);
    glVertex3f(3.0, 3.0, 0.0);
    glVertex3f(3.0, -3.0, 0.0);
    
    glColor4f(0.3,0.3,0.6,1.0);
    glVertex3f(-3.0, -3.0, 0.0);
    glVertex3f(-3.0, -6.0, 0.0);
    glVertex3f(3.0, -6.0, 0.0);
    glVertex3f(3.0, -3.0, 0.0);
    
    glColor4f(0.6,0.3,0.6,1.0);
    glVertex3f(-3.0, -3.0, 0.0);
    glVertex3f(-6.0, -3.0, 0.0);
    glVertex3f(-6.0, 3.0, 0.0);
    glVertex3f(-3.0, 3.0, 0.0);


    glEnd();
    
    graph->Display(cam_pos->z);
    
    graph->FadeColors();
    
}

void triggerAudio() {
    
    static float elapsed = 0.0f;
    elapsed += audio_clock.GetElapsedTime();

    float delta = elapsed - lastAudioTime;
    
    vector<int> * note_list = new vector<int>;
    graph->GetCurConnections(note_list);
    
    if (delta > 5) {
        
        audio_clock.Reset();
        lastAudioTime = elapsed;
        
        for (int i = 0; i < note_list->size(); i++) {
            // play note i's audio via osc message
            printf("note number is: %d\n", note_list->at(i) );
            osc::SendMyNote( note_list->at(i), 15 );
        }

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
    
    cam->updateCam(delta);
    
    if (g_messages.size() > 0) {
        ReadMessage();
    }
}


int main(int argc, char** argv) {
    
    printf("Initialize output\n");
    
    if (argc != 2) {
		exit(0);
    }
    
    // Read Dissonance Matrix from file
    dissonance = new Dissonance(std::string(argv[1]));
        
    initOpenGL();
    loadAssets();
    
    pthread_t osc_thread;
    string osc_message = "Starting OSC Listen Thread";
    int osc_ret;
    osc_ret = pthread_create( &osc_thread, NULL, OscThread, (void*) &osc_message);
    
    srand( (unsigned)time( NULL ) );
    
    graph = new Graph(max_notes, dissonance);
    
    cam = new Camera(0.0, 0.0, 4.0);
    
    // Main Loop
    while (window.IsOpened()) {
        
        printf("what the shit\n");
        
        handleInput();
        updateState();
        renderFrame();
        triggerAudio();
        
        window.Display();
    }
    
    return 0;
}