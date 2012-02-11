/**
 * main.cpp
 * CS248 Assignment 1, Winter 2010
 * -------------------------------
 * This is simple testbed for your miniGL implementation. 
 * Modify the function render() to issue commands to your
 * miniGL pipeline. 
 *
 * When this program is run, it saves the resulting 
 * framebuffer to a TGA file. 
 *
 * Note: the name of the output file can be specified via
 * a command line argument.  The default is "render.tga".
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "minigl.h"

using namespace std;


// Change these to test MiniGL at different resolutions
#define FB_WIDTH  640
#define FB_HEIGHT 480

void render();
void writeToTGA(char* filename);


/**
 * Make all your MiniGL calls here. 
 *
 * To use a test script, you can copy and paste 
 * the contents into the render(), or you 
 * can #include file inside the function.
 *
 * Example:
 * void render()
 * {
 *   #include "tests/t1.cpp"
 * }
 */
void render()
{
  #include "tests/t4.cpp"
}

/**
 * DO NOT MODIFY BELOW
 * ----------------------------------
 * Writes the MiniGL framebuffer to a picture
 * file in .tga format (viewable with quicktime)
 */
void writeToTGA(char* filename)
{
    
    MGLpixel* buffer= new MGLpixel[FB_WIDTH * FB_HEIGHT];
    mglReadPixels(FB_WIDTH, FB_HEIGHT, buffer);
    
    // Open filestream
    ofstream out;
    out.open(filename, ios::binary);
    if(out.fail()) {
        cout << "There was an error opening file " << filename << endl;
        exit(1);
    }

    // Write header
    const int HEADER_SIZE = 18;
    char header[HEADER_SIZE];
    memset(header, 0, HEADER_SIZE);
    header[2] = 2;                         // data type = unmapped RGB
    *((short*)(header + 12)) = FB_WIDTH;   // width of image in pixels
    *((short*)(header + 14)) = FB_HEIGHT;  // height of image in pixels
    header[16] = 24;                       // number of bits per pixel (for now, 3bytes)
    header[17] = header[17] & (0 << 5) ;   // picture origin in top left
    out.write((char*)header, HEADER_SIZE);

    // Write image data
    cout << "Writing..." << endl;
    for (MGLpixel* ptr = buffer; ptr-buffer < FB_WIDTH * FB_HEIGHT; ptr++) {
        //Write each value indiviually to avoid endianness issues
        out.put(MGL_GET_BLUE(*ptr));
        out.put(MGL_GET_GREEN(*ptr));
        out.put(MGL_GET_RED(*ptr));
    }

    out.close();
    delete buffer;
}


int main(int argc, char** argv)
{
    render();

    if (argc > 1){
        writeToTGA(argv[1]);
    } else {
        writeToTGA((char *)"render.tga");
    }

    return 0;
}
