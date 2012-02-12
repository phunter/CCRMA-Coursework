/****************************************************************************
 * CS248 Assignment 2, Winter 2012
 *
 * Filename: main.cpp
 * Author: Ming Jiang (ming.jiang@stanford.edu)
 * Date: Sun Jan 24 21:10:00 PST 2010
 *
 * Edited and Submitted by: Hunter McCurry(hmccurry@stanford.edu)
 *
 * Description: Main function entry and SDL framework.
 ****************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef __APPLE__
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif
#include "minigl.h"
#include "mglmesh.h"
#include <iostream>

using namespace std;

string sceneFile;
int testNum = -1;


#define FB_WIDTH 640
#define FB_HEIGHT 480

MGLpixel buffer[FB_WIDTH * FB_HEIGHT];
MGLpixel flipped[FB_WIDTH * FB_HEIGHT];

float xvel = 0.0, yvel = 0.0, avel = 0.0, svel = 1.0;

bool lightOn0 = false, lightOn1 = false, lightOn2 = false, lightOnGlobal = false;

float lightPos0[3] = { 0.0f, 0.0f, 2.0f };
float lightPos1[3] = { 5.0f, 5.0f, 0.0f };
float lightPos2[3] = { -5.0f, -5.0f, -5.0f };

MGLmesh* mesh = NULL;

void setupLights(float elapsedTime) {
    int numkeys;
    Uint8 *keystate = SDL_GetKeyState(&numkeys);
    if (keystate[SDLK_4]) {
        lightPos0[0] += 5 * xvel * elapsedTime;
        lightPos0[1] += 5 * yvel * elapsedTime;
    }
    if (keystate[SDLK_5]) {
        lightPos1[0] += 5 * xvel * elapsedTime;
        lightPos1[1] += 5 * yvel * elapsedTime;
    }
    if (keystate[SDLK_6]) {
        lightPos2[0] += 5 * xvel * elapsedTime;
        lightPos2[1] += 5 * yvel * elapsedTime;
    }

    // Light 0
    mglLight(MGL_LIGHT0, MGL_LIGHT_POSITION, lightPos0);

    // Light 1
    mglLight(MGL_LIGHT1, MGL_LIGHT_POSITION, lightPos1);

    // Light 2
    mglLight(MGL_LIGHT2, MGL_LIGHT_POSITION, lightPos2);
}


void setupCamera(float elapsedTime) {
    int numkeys;
    Uint8 *keystate = SDL_GetKeyState(&numkeys);

    static float xpos = 0.0f, ypos = 0.0f,
        angle = 0.0f, scale = 0.15f;

    if (!keystate[SDLK_4] && !keystate[SDLK_5] && !keystate[SDLK_6]) {
        xpos += xvel * elapsedTime;
        ypos += yvel * elapsedTime;
    }

    angle += avel * elapsedTime;
    scale *= pow(svel, elapsedTime);

    mglMatrixMode(MGL_PROJECTION);
    mglLoadIdentity();
    mglFrustum(-scale * FB_WIDTH / FB_HEIGHT,
	         scale * FB_WIDTH / FB_HEIGHT,
	         -scale, scale, 1.0, 1000.0);

    mglMatrixMode(MGL_MODELVIEW);
    mglLoadIdentity();

    setupLights(elapsedTime);

    mglTranslate(-xpos, -ypos, -10.0);
    mglRotate(angle, 0.0, 1.0, 0.0);

}


void initScene() {
    mesh = new MGLmesh;
    mesh->load(sceneFile);
}


void renderScene(float elapsedTime) {
    setupCamera(elapsedTime);

    mglColor(255, 255, 255);
    mesh->display();

    // DO NOT modify the following statements

    mglReadPixels(FB_WIDTH, FB_HEIGHT, buffer);

    for (int i = 0; i < FB_HEIGHT; ++i) {
        memcpy(flipped + FB_WIDTH * (FB_HEIGHT - i - 1),
	     buffer + FB_WIDTH * i,
	     FB_WIDTH * sizeof(MGLpixel));
    }

    mglFlush();
}


void destroyScene() {
    delete mesh;
}


void handleKeyboard(SDL_KeyboardEvent key) {
    switch (key.keysym.sym) {
    case SDLK_ESCAPE: 
        if (key.type == SDL_KEYDOWN) {
            SDL_Event* e = new SDL_Event;
            e->type = SDL_QUIT;
            SDL_PushEvent(e);
        }
        break;

        // Panning
    case SDLK_UP:        
        if (key.type == SDL_KEYDOWN) yvel = 5.0;
        else if (yvel > 0.0) yvel = 0.0;
        break;
    case SDLK_DOWN:
        if (key.type == SDL_KEYDOWN) yvel = -5.0;
        else if (yvel < 0.0) yvel = 0.0;
        break;
    case SDLK_RIGHT:
        if (key.type == SDL_KEYDOWN) xvel = 5.0;
        else if (xvel > 0.0) xvel = 0.0;
        break;
    case SDLK_LEFT:
        if (key.type == SDL_KEYDOWN) xvel = -5.0;
        else if (xvel < 0.0) xvel = 0.0;
        break;

        // Zooming
    case SDLK_w:
        if (key.type == SDL_KEYDOWN) svel = 0.5;
        else if (svel < 1.0) svel = 1.0;
        break;
    case SDLK_s:
        if (key.type == SDL_KEYDOWN) svel = 2.0;
        else if (svel > 1.0) svel = 1.0;
        break;

        // Rotating
    case SDLK_d:
        if (key.type == SDL_KEYDOWN) avel = 50.0;
        else if (avel > 0.0) avel = 0.0;
        break;
    case SDLK_a:
        if (key.type == SDL_KEYDOWN) avel = -50.0;
        else if (avel < 0.0) avel = 0.0;
        break;

        // Lighting
    case SDLK_1:
        if (key.type == SDL_KEYDOWN && !(key.keysym.mod & KMOD_SHIFT)) {
            lightOn0 = !lightOn0;
            mglLightEnabled(MGL_LIGHT0, lightOn0);
            printf("Light 0 %s\n", lightOn0 ? "enabled" : "disabled");
        }
        break;
    case SDLK_2:
        if (key.type == SDL_KEYDOWN && !(key.keysym.mod & KMOD_SHIFT)) {
            lightOn1 = !lightOn1;
            mglLightEnabled(MGL_LIGHT1, lightOn1);
            printf("Light 1 %s\n", lightOn1 ? "enabled" : "disabled");
        }
        break;
    case SDLK_3:
        if (key.type == SDL_KEYDOWN && !(key.keysym.mod & KMOD_SHIFT)) {
            lightOn2 = !lightOn2;
            mglLightEnabled(MGL_LIGHT2, lightOn2);
            printf("Light 2 %s\n", lightOn2 ? "enabled" : "disabled");
        }
        break;
    case SDLK_l:
        if (key.type == SDL_KEYDOWN && !(key.keysym.mod & KMOD_SHIFT)) {
            lightOnGlobal = !lightOnGlobal;
            mglLightingEnabled(lightOnGlobal);
            printf("Lighting %s\n", lightOnGlobal ? "enabled" : "disabled");
        }
        break;

        // Shading
    case SDLK_p:    
        if (key.type == SDL_KEYDOWN) {
            printf("Phong shading\n");
            mglShadingMode(MGL_PHONG);
        }
        break;
    case SDLK_t:
        if (key.type == SDL_KEYDOWN) {
            printf("Toon shading\n");
            mglShadingMode(MGL_TOON);
        }
        break;
    default:
        break;
    }

    // Select test
    if (key.type == SDL_KEYDOWN && (key.keysym.mod & KMOD_SHIFT)) {
        switch(key.keysym.sym) {
        case SDLK_1:
	        printf("Test 1: only ambient lighting\n");
	        testNum = 1;
	        break;
        case SDLK_2:
	        printf("Test 2: only diffuse lighting\n");
	        testNum = 2;
	        break;
        case SDLK_3:
	        printf("Test 3: only specular lighting\n");
	        testNum = 3;
	        break;
        case SDLK_4:
	        printf("Test 4: one light with multiple properties set\n");
            testNum = 4;
            break;
        case SDLK_5:
	        printf("Test 5: multiple lights with multiple properties set\n");
	        testNum = 5;
	        break;
        case SDLK_6:
	        printf("Test 6: toon shading\n");
	        testNum = 6;
	        break;
        case SDLK_7:
	        printf("Test 7: diffuse texture with one light\n");
	        testNum = 7;
	        break;
        case SDLK_8:
	        printf("Test 8: diffuse texture with multiple lights\n");
	        testNum = 8;
	        break;
        case SDLK_9:
	        printf("Test 9: specular texture with one light\n");
	        testNum = 9;
	        break;
        default:
	        break;
        }
    }
}


float tickTimer() {
    static unsigned lastTime = 0;
    unsigned curTime = SDL_GetTicks();
    unsigned elapsedTime = curTime - lastTime;
    lastTime = curTime;
    return 0.001f * elapsedTime;
}

void printControls() {
    cout << endl;

    cout << "A|D: Rotate left|right" << endl;
    cout << "W|S: Zoom in|out" << endl;
    cout << "LEFT|RIGHT: Pan left|right" << endl;
    cout << "UP|DOWN: Pan up|down" << endl;
    cout << "1|2|3: Toggle light 1|2|3" << endl;
    cout << "L: Toggle lighting" << endl;
    cout << "P|T: Enable Phong|toon shading" << endl;
    cout << "3|4|5 + LEFT|RIGHT: Pan light 1|2|3" << endl;
    cout << "3|4|5 + UP|DOWN: Pan light 1|2|3" << endl;

    cout << endl;
}

int main(int argc, char** argv) {

    if(argc < 2){
        cout<<"Usage: "<<argv[0]<<" <filename> "<<endl;
        exit(1);;
    }
    sceneFile = string(argv[1]);

    cout << endl;
    cout << "CS 248 Assignment 2" << endl;

    printControls();

    SDL_Surface *screen, *image;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr,
	        "Unable to initialize SDL: %s\n",
	        SDL_GetError());
        exit(1);
    }

    if (!(screen = SDL_SetVideoMode(FB_WIDTH, FB_HEIGHT,
				    0, SDL_SWSURFACE))) {
        fprintf(stderr,
	        "Unable to set video mode: %s\n",
	        SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption("MiniGL Demo", NULL);

    if (!(image = SDL_CreateRGBSurfaceFrom(flipped, FB_WIDTH, FB_HEIGHT,
					 32, FB_WIDTH * 4,
					 0x000000ff, 0x0000ff00,
					 0x00ff0000, 0xff000000))) {
        fprintf(stderr,
	        "Unable to create surface: %s\n",
	        SDL_GetError());
        exit(1);
    }



    initScene();

    bool quit = false;
    SDL_Event event;
    tickTimer();
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKeyboard(event.key);
	              break;
            case SDL_QUIT:
                quit = true;
	              break;
            }
        } else {
            renderScene(tickTimer());
            SDL_BlitSurface(image, NULL, screen, NULL);
            SDL_Flip(screen);
        }
    }

    destroyScene();

    SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
}
