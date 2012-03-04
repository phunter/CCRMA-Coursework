// main.cpp
//
//  Created by phunter on 11/1/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#include "st.h"
#include "stgl.h"
#include "stglut.h"
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <pthread.h>

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

#define WIN_WIDTH 880
#define WIN_HEIGHT 880

using namespace std;

// Window size so we can properly update the UI.
int gWindowSizeX = WIN_WIDTH;
int gWindowSizeY = WIN_HEIGHT;

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

// Dissonance Matrix
Dissonance * dissonance;

// Message Things
queue <NoteMessage*> g_messages;

Camera *cam;

// function prototypes
//void repelAll();
//void fadeColors();
//void triggerNote(int node, int excitement);
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

void tick(int value)
{    
    graph->UpdateGraph();
    
    cam->updateCam();
    
    if (g_messages.size() > 0) {
        ReadMessage();
    }
    
    // reschedule
    glutTimerFunc(16.0f, tick, 0);
    // redisplay
    glutPostRedisplay();
}

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
    
    graph->AddConnectExcite(note, 0.0 + 1.5 * (vel / 127.0));
    cam->setTargetNote(graph->GetNote(note));

    
//    if (note == 69) {
//        triggerNote(1, vel);
//    }
//    else if (note == 60) {
//        triggerNote(2, vel);
//    }

//    float default_dist = .4;
//    graph->AddConnectExcite(note, default_dist);
    
    //printf("New size of message queue is  %lu'\n", g_messages.size());
}

void display(void)
{
    glClearColor(.9f, .9f, .8f, 1.0f);
    //glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    STPoint3 * cam_pos = cam->getPosition();
    //STPoint3 * look_pos = cam->getTargetNotePosition();
    
    glLoadIdentity();
    
    gluLookAt (cam_pos->x, cam_pos->y, cam_pos->z, cam_pos->x, cam_pos->y, 0.0, 0.0, 1.0, 0.0);
    //gluLookAt (cam_pos->x, cam_pos->y, cam_pos->z, look_pos->x, look_pos->y, look_pos->z, 0.0, 1.0, 0.0);
    
    
    graph->Display(cam_pos->z);
    
    graph->FadeColors();
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glFrustum (-.5, .5, -.5, .5, .5, 300.0);
    glMatrixMode (GL_MODELVIEW);
}

//void triggerNote(int node, int excitement) {
//    
//    float add_val = (float)excitement / 127.0;
//    
//    g_notes[node]->setExcite(fmin(g_notes[node]->getExcite() + add_val, 1.0));
//
//    curNote = g_notes[node];
//    cam->setTargetNote(g_notes[node]->getLocation());
//}


void keyboard(unsigned char key, int x, int y)
{    
    int midi;
    printf("key is %hhu\n", key);
    
    switch (key) {
        case '1': 
            
            midi = 20;
            graph->AddConnectExcite(midi, .5);
            cam->setTargetNote(graph->GetNote(midi));
            
            break;
		case '2':
            midi = 21;
            graph->AddConnectExcite(midi, .5);
            cam->setTargetNote(graph->GetNote(midi));
            
            break;
        case '3':
            midi = 22;
            graph->AddConnectExcite(midi, .5);
            cam->setTargetNote(graph->GetNote(midi));
            
            break;
        case '4':
            midi = 23;
            graph->AddConnectExcite(midi, .5);
            cam->setTargetNote(graph->GetNote(midi));
            
            break;
        case '5':
            midi = 24;
            graph->AddConnectExcite(midi, .5);
            cam->setTargetNote(graph->GetNote(midi));
            
            break;
        case '6':
            midi = 25;
            graph->AddConnectExcite(midi, .5);
            cam->setTargetNote(graph->GetNote(midi));
            
            break;
        case 'w':
            graph->Clear();
            break;
            
        case 61: // '='
            cam->UpdateDefaultHeight(cam->GetDefaultHeight() - .1);
            break;
            
        case 45: // '-'
            cam->UpdateDefaultHeight(cam->GetDefaultHeight() + .1);
            
            break;
        case 27:
            exit(0);
    }
}


void MouseCallback(int button, int state, int x, int y)
{
}

void PassiveMotionCalback(int x, int y)
{
}

void MotionCallback(int x, int y)
{
}


static void init(void)
{
    glEnable( GL_BLEND );
    //glEnable( GL_DEPTH_TEST );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


int main(int argc, char** argv)
{
    
    if (argc != 2)
		exit(0);
    
    // Read Dissonance Matrix from file
    dissonance = new Dissonance(std::string(argv[1]));

    //
    // Initialize GLUT.
    //
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowPosition(0, 20);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("GraphScore");

    //
    // Initialize the UI.
    //
    init();

    //
    // Register GLUT callbacks and enter main loop.
    //
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    
//    glutMouseFunc( MouseCallback );
//    glutPassiveMotionFunc( PassiveMotionCalback );
//    glutMotionFunc( MotionCallback );
    
    pthread_t osc_thread;
    string osc_message = "Starting OSC Listen Thread";
    int osc_ret;
    osc_ret = pthread_create( &osc_thread, NULL, OscThread, (void*) &osc_message);
    
    srand( (unsigned)time( NULL ) );
    
    graph = new Graph(max_notes, dissonance);
    
    //camera = STVector3(0.0, 0.0, default_height);
    //curNote = g_notes[0];
    //computeTravelDist();
    
    cam = new Camera(0.0, 0.0, 1.0, 2.0);
    
//    AddNotes();

    //cam->computeTravelDist();
    
    glutTimerFunc(16.0f, tick, 0);
    
    glutMainLoop();

    return 0;
}