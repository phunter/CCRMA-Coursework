/****************************************************************************
 * CS248 Assignment 2, Winter 2012
 *
 * Filename: minigl.cpp
 * Authors: Ming Jiang (ming.jiang@stanford.edu), Alexis Haraux (aharaux@stanford.edu)
 *
 * Description: MiniGL implementation starter code for assignment 2.
 ****************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <stack>
#include "minigl.h"

using namespace std;


// Macros and constants definition

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define NOT_YET_IMPLEMENTED printf("Warning: call to unimplimented function!\n")

/**
 * A utility function for error reporting.
 */
inline void MGL_ERROR(const char* description) {
    printf("ERROR: %s\n", description);
    exit(1);
}

// Data structures definition

struct MGLpointLight {
    MGLpointLight() {
        x = 0.0f, y = 0.0f, z = 1.0f; // initial positoin (0, 0, 1)
        ambient[0] = 0.0f, ambient[1] = 0.0f, ambient[2] = 0.0f;
        diffuse[0] = 0.0f, diffuse[1] = 0.0f, diffuse[2] = 0.0f;
        specular[0] = 0.0f, specular[1] = 0.0f, specular[2] = 0.0f;
    }
    
    // Location    
    MGLfloat x, y, z, w;
    
    MGLfloat ambient[3];
    MGLfloat diffuse[3];
    MGLfloat specular[3];
};

struct MGLSceneLights {
    MGLSceneLights() {
        // All lights disabled by default
        enabled[0] = false, enabled[1] = false, enabled[2] = false;
        
        // This initializes light 0 to specified defaults
        light_list[0].diffuse[0] = 1.0f, light_list[0].diffuse[1] = 1.0f, light_list[0].diffuse[2] = 1.0f;
        light_list[0].specular[0] = 1.0f, light_list[0].specular[1] = 1.0f, light_list[0].specular[2] = 1.0f;
        
        // TODO: give some values to other lights for testing
    }
    
    MGLbool enabled[3];
    MGLpointLight light_list[3];
};

struct MGLmaterial {
    MGLmaterial() {
        ambient[0] = 0.0f, ambient[1] = 0.0f, ambient[2] = 0.0f;
        diffuse[0] = 0.0f, diffuse[1] = 0.0f, diffuse[2] = 0.0f;
        specular[0] = 0.0f, specular[1] = 0.0f, specular[2] = 0.0f;
        shininess = 0.0f;
    }
    MGLfloat ambient[3];
    MGLfloat diffuse[3];
    MGLfloat specular[3];
    MGLfloat shininess;
};

struct MGLtexture {
    MGLtexture() {
        tex_width = 0;
        tex_height = 0;
        
        data = NULL;
    }
    MGLtexture(int width, int height) {
        tex_width = width;
        tex_height = height;
        
        data = (MGLpixel*) malloc (width * height * sizeof(MGLpixel));
    }
    MGLint tex_width;
    MGLint tex_height;
    
    MGLpixel * data;
};

struct MGLtextureCoord {
    MGLfloat x, y;
};

struct MGLnormalStruct {
    MGLfloat data[3];
};

struct MGLvertex {
    
    MGLfloat x, y, z, w;
    MGLpixel color;
    
    ///////////////////////////////////////////////////////
    //
    // TODO:
    //
    // Add new per-vertex attributes here.
    //
    ///////////////////////////////////////////////////////
    
    MGLfloat eyeX, eyeY, eyeZ, eyeW;
    
    MGLmaterial mat;
    
    MGLfloat normal[3];
    
    MGLbool texturesEnabled;
    
    MGLtex_id diffuse_tex_id;
    MGLint diffuse_tex_width, diffuse_tex_height;    
    MGLfloat diffuse_u, diffuse_v;
    
    MGLtex_id specular_tex_id;
    MGLint specular_tex_width, specular_tex_height; 
    MGLfloat specular_u, specular_v;
    
    
    ///////////////////////////////////////////////////////
    //
    // END HERE
    //
    ///////////////////////////////////////////////////////
};

struct MGLtriangle {
    MGLvertex v[3];
};

/**
 * A simple wrapper structure for storing 4x4 matrix.
 * Constructed to be identity matrix initially.
 */
struct MGLmatrix {
    MGLmatrix() {
        memset(m, 0, 16 * sizeof(MGLfloat));
        m[0] = m[5] = m[10] = m[15] = 1;
    }
    MGLfloat m[16];
};

/**
 * A structure for you to contain all information about a fragment.
 * Currently it only contains color and depth.
 *
 **********************************************************
 *
 * TODO: you will need to add new attributes here.
 *
 **********************************************************
 */
struct MGLfragment {
    MGLfragment() {
        color = 0xff000000;
        depth = 1.0f;
        
        diffuse_id = 0;
        specular_id = 0;
    }
    MGLpixel color;
    MGLfloat depth;
    
    MGLbool texturesEnabled;
    
    MGLint diffuse_id;
    MGLfloat diffuse_u, diffuse_v;
    
    MGLint specular_id;
    MGLfloat specular_u, specular_v;
    
    MGLfloat eyeX, eyeY, eyeZ;
    
    MGLnormalStruct normal;
    
    MGLmaterial material;
    
};

/**
 * A structure (actually a class) abstracted for a fragment buffer.
 * This is the place where your rasterizer will output.  Final color
 * for the frame buffer will be calculated based on this fragment
 * buffer.
 */
struct MGLfragbuffer {
    MGLfragbuffer(MGLsize w, MGLsize h) : width(w), height(h) {
        data = new MGLfragment[w * h];
    }
    ~MGLfragbuffer() {
        delete [] data;
    }
    
    MGLsize width, height;
    MGLfragment* data;
};


// Global variables definition

MGLpixel curColor = 0xff000000;                // changed when mglColor
                                               // is called
MGLmatrix_mode curMatrixMode = MGL_MODELVIEW;  // current matrix mode,
                                               // changed when mglMatrixMode
                                               // is called
MGLpoly_mode curPolyMode = MGL_TRIANGLES;      // current polygon mode
                                               // specified by mglBegin

MGLshading_mode curShadingMode = MGL_PHONG;


MGLbool hasBegun = false;  // true when we enter mglBegin and
                           // false when we leave mglEnd
MGLint curIndex = 0;       // used to ensure correct number of
                           // calls to mglVertex

MGLbool lightingEnabled = false;

MGLbool texturesEnabled = false;

MGLmaterial curMaterial;

MGLmatrix modelViewMatrix, projectionMatrix;  // store the current modelview
                                              // and projection matrices
MGLmatrix* curMatrix;                         // always point to one of the
                                              // above matrix according to the
                                              // current matrix mode
stack<MGLmatrix> modelViewStack;              // modelview matrix stack
stack<MGLmatrix> projectionStack;             // projection matrix stack

vector<MGLvertex> transformedVertices;  // store all vertices in clipping space
                                        // coordinates without dividing by w

MGLSceneLights lights;

MGLnormalStruct curNormal;

vector<MGLtexture> textures(1);

// MGLtexture* curTexture;

MGLtex_id curDiffuseTextureID = 0;
MGLtex_id curSpecularTextureID = 0;

MGLtextureCoord curDiffuseTextureCoord;
MGLtextureCoord curSpecularTextureCoord;

MGLint curTextureSlot;


// Helper functions

/**
 * Debug utility function to display the homogeneous coordinates of a vertex.
 */
void debugShowVertex(const MGLvertex& ver, const char* msg = NULL)
{
    if (msg) fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "%f\t%f\t%f\t%f\n", ver.x, ver.y, ver.z, ver.w);
    fprintf(stderr, "\n");
}

/**
 * Debug utility function to display the elements of a 4x4 matrix.
 */
void debugShowMatrix(const MGLmatrix& mat, const char* msg = NULL)
{
    if (msg) fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "%f\t%f\t%f\t%f\n", mat.m[0], mat.m[4], mat.m[8], mat.m[12]);
    fprintf(stderr, "%f\t%f\t%f\t%f\n", mat.m[1], mat.m[5], mat.m[9], mat.m[13]);
    fprintf(stderr, "%f\t%f\t%f\t%f\n", mat.m[2], mat.m[6], mat.m[10], mat.m[14]);
    fprintf(stderr, "%f\t%f\t%f\t%f\n", mat.m[3], mat.m[7], mat.m[11], mat.m[15]);
    fprintf(stderr, "\n");
}

/**
 * Matrix vector multiplication.
 *
 * Take vi and vo as a column vector. This function gives vo = mat * vi.
 * Input - mat, vi
 * Output - vo
 */
void mulMatrixVector(const MGLmatrix& mat, const MGLfloat* vi, MGLfloat* vo)
{
    vo[0] = mat.m[0] * vi[0] + mat.m[4] * vi[1] + mat.m[8] * vi[2] + mat.m[12] * vi[3];
    vo[1] = mat.m[1] * vi[0] + mat.m[5] * vi[1] + mat.m[9] * vi[2] + mat.m[13] * vi[3];
    vo[2] = mat.m[2] * vi[0] + mat.m[6] * vi[1] + mat.m[10] * vi[2] + mat.m[14] * vi[3];
    vo[3] = mat.m[3] * vi[0] + mat.m[7] * vi[1] + mat.m[11] * vi[2] + mat.m[15] * vi[3];
}

MGLpixel MGLColorFromFloats(MGLfloat const * float_list)
{
    MGLint pix_R, pix_G, pix_B, pix_A;
    // pix individual channels
    pix_R = min(255, (MGLint) (255.0 * float_list[0]));
    pix_G = min(255, (MGLint) (255.0 * float_list[1]));
    pix_B = min(255, (MGLint) (255.0 * float_list[2]));
    pix_A = 255;
    
    MGLpixel pix;
    
    MGL_SET_RED(pix, (MGLbyte) pix_R);
    MGL_SET_GREEN(pix, (MGLbyte) pix_G);
    MGL_SET_BLUE(pix, (MGLbyte) pix_B);
    MGL_SET_ALPHA(pix, (MGLbyte) pix_A);
    
    return pix;
}


MGLpixel MGLColorAdd(MGLpixel left, MGLpixel right)
{
    // extract individual color channels
    MGLbyte left_R, left_G, left_B, left_A, right_R, right_G, right_B, right_A;
    left_R = MGL_GET_RED(left);
    left_G = MGL_GET_GREEN(left);
    left_B = MGL_GET_BLUE(left);
    left_A = MGL_GET_ALPHA(left);
    
    right_R = MGL_GET_RED(right);
    right_G = MGL_GET_GREEN(right);
    right_B = MGL_GET_BLUE(right);
    right_A = MGL_GET_ALPHA(right);
    
    MGLint add_R, add_G, add_B, add_A;
    // add individual channels
    add_R = min(255, (MGLint) (255.0*((left_R/255.0) + (right_R/255.0))));
    add_G = min(255, (MGLint) (255.0*((left_G/255.0) + (right_G/255.0))));
    add_B = min(255, (MGLint) (255.0*((left_B/255.0) + (right_B/255.0))));
    add_A = min(255, (MGLint) (255.0*((left_A/255.0) + (right_A/255.0))));
    
    MGLpixel add;
    
    MGL_SET_RED(add, (MGLbyte) add_R);
    MGL_SET_GREEN(add, (MGLbyte) add_G);
    MGL_SET_BLUE(add, (MGLbyte) add_B);
    MGL_SET_ALPHA(add, (MGLbyte) add_A);
    
    return add;
}


MGLpixel MGLColorMult(MGLpixel left, MGLpixel right)
{
    // extract individual color channels
    MGLbyte left_R, left_G, left_B, left_A, right_R, right_G, right_B, right_A;
    left_R = MGL_GET_RED(left);
    left_G = MGL_GET_GREEN(left);
    left_B = MGL_GET_BLUE(left);
    left_A = MGL_GET_ALPHA(left);
    
    right_R = MGL_GET_RED(right);
    right_G = MGL_GET_GREEN(right);
    right_B = MGL_GET_BLUE(right);
    right_A = MGL_GET_ALPHA(right);
    
    MGLint mul_R, mul_G, mul_B, mul_A;
    // multiply individual channels
    mul_R = min(255, (MGLint) (255.0*((left_R/255.0) * (right_R/255.0))));
    mul_G = min(255, (MGLint) (255.0*((left_G/255.0) * (right_G/255.0))));
    mul_B = min(255, (MGLint) (255.0*((left_B/255.0) * (right_B/255.0))));
    mul_A = min(255, (MGLint) (255.0*((left_A/255.0) * (right_A/255.0))));
    
    MGLpixel mul;
    
    MGL_SET_RED(mul, (MGLbyte) mul_R);
    MGL_SET_GREEN(mul, (MGLbyte) mul_G);
    MGL_SET_BLUE(mul, (MGLbyte) mul_B);
    MGL_SET_ALPHA(mul, (MGLbyte) mul_A);
    
    return mul;
}


/**
 * Linearly interpolates between two colors;
 * s = 0 returns left, s = 1 returns right
 */
MGLpixel MGLColorLerp(MGLpixel left, MGLpixel right, MGLfloat s)
{
    // extract individual color channels
    MGLfloat left_R, left_G, left_B, left_A, right_R, right_G, right_B, right_A;
    left_R = (MGLfloat) MGL_GET_RED(left);
    left_G = (MGLfloat) MGL_GET_GREEN(left);
    left_B = (MGLfloat) MGL_GET_BLUE(left);
    left_A = (MGLfloat) MGL_GET_ALPHA(left);
    
    right_R = (MGLfloat) MGL_GET_RED(right);
    right_G = (MGLfloat) MGL_GET_GREEN(right);
    right_B = (MGLfloat) MGL_GET_BLUE(right);
    right_A = (MGLfloat) MGL_GET_ALPHA(right);
    
    MGLint lerp_R, lerp_G, lerp_B, lerp_A;
    // linterp individual channels
    lerp_R = (MGLint) (left_R + s * (right_R - left_R));
    lerp_G = (MGLint) (left_G + s * (right_G - left_G));
    lerp_B = (MGLint) (left_B + s * (right_B - left_B));
    lerp_A = (MGLint) (left_A + s * (right_A - left_A));
    
    MGLpixel lerped;
    
    MGL_SET_RED(lerped, (MGLbyte) lerp_R);
    MGL_SET_GREEN(lerped, (MGLbyte) lerp_G);
    MGL_SET_BLUE(lerped, (MGLbyte) lerp_B);
    MGL_SET_ALPHA(lerped, (MGLbyte) lerp_A);
    
    return lerped;
}

/**
 * ColorBilerp takes a pointer to a texture, and normalized x_coord and y_coord
 * Note: This wraps around at the edges based on texture height and width 
 */
MGLpixel MGLColorBilerp(MGLint tex_id, MGLfloat tex_x, MGLfloat tex_y)
{
    MGLtexture * texture = &textures[tex_id];
    int h = texture->tex_height;
    int w = texture->tex_height;
    
    
    // Extract 4 closest points' colors;
    // bottom left, bottom right, top left, top right
    MGLpixel bl = texture->data[ ( ((int)floor(tex_y*h)+h) % h) * w + ( ((int)floor(tex_x*w)+w) % w)];
    MGLpixel br = texture->data[ ( ((int)floor(tex_y*h)+h) % h) * w + ( ((int)ceil(tex_x*w)+w) % w)];
    MGLpixel tl = texture->data[ ( ((int)ceil(tex_y*h)+h) % h) * w + ( ((int)floor(tex_x*w)+w) % w)];
    MGLpixel tr = texture->data[ ( ((int)ceil(tex_y*h)+h) % h) * w + ( ((int)ceil(tex_x*w)+w) % w)];
        
    // Find x,y interpolation amounts
    MGLfloat xfract, yfract;
    xfract = tex_x - floor(tex_x);
    yfract = tex_y - floor(tex_y);
    
    // Interpolate top pixels and bottom pixels based on x position
    MGLpixel top = MGLColorLerp(tl, tr, xfract);
    MGLpixel bot = MGLColorLerp(bl, br, xfract);
    
    // Interpolate between top and bottom based on y position
    MGLpixel final = MGLColorLerp(bot, top, yfract);
    return final;
}


/**
 * Rasterizing a triangle.
 * You will have to update this function to handle 
 * perspective correct interpolation of fragment 
 * attributes (color, texture coordinates, normals,
 * eye space coordinates)
 *
 */
void rasterize(MGLtriangle& tri, MGLfragbuffer& fragbuf)
{
    MGLfloat orig_w[3];
    
    // Viewport transformation
    for (int i = 0; i < 3; ++i) {
        tri.v[i].x = 0.5f * fragbuf.width * (tri.v[i].x / tri.v[i].w + 1.0f);
        tri.v[i].y = 0.5f * fragbuf.height * (tri.v[i].y / tri.v[i].w + 1.0f);
        tri.v[i].z = 0.5f * (tri.v[i].z / tri.v[i].w + 1.0f);
        
        ///////////////////////////////////////////////////
        //
        // TODO:
        //
        // You may want to keep the original w value for
        // perspective correction later.  Also you may want
        // to do some transformation to other attributes
        // you added to MGLvertex as well.
        //
        ///////////////////////////////////////////////////
        
        // recompute normals
        
        // WTF why do neither of these work?
        orig_w[i] = tri.v[i].w;
        //orig_w[i] = tri.v[i].eyeW;
        
        ///////////////////////////////////////////////////
        //
        // END HERE
        //
        ///////////////////////////////////////////////////
    }
    
    // printf("orig_w[0] = %f, orig_w[1] = %f, orig_w[2] = %f\n", orig_w[0], orig_w[1], orig_w[2]);
	
	// Compute the bounding rectangle
	float xll = fmax(0.0f, fmin(tri.v[0].x, fmin(tri.v[1].x, tri.v[2].x)));
	float yll = fmax(0.0f, fmin(tri.v[0].y, fmin(tri.v[1].y, tri.v[2].y)));
	float xur = fmin((float)(fragbuf.width-1), fmax(tri.v[0].x, fmax(tri.v[1].x, tri.v[2].x)));
	float yur = fmin((float)(fragbuf.height-1), fmax(tri.v[0].y, fmax(tri.v[1].y, tri.v[2].y)));
	
	// Compute the common denominator
	float detA = (tri.v[0].x * tri.v[1].y) - (tri.v[0].x * tri.v[2].y) - (tri.v[1].x * tri.v[0].y) 
	+ (tri.v[1].x * tri.v[2].y) + (tri.v[2].x * tri.v[0].y) - (tri.v[2].x * tri.v[1].y);
	
	// Draw the triangle
	
	for(int y = floor(yll); y <= ceil(yur); y+=1) {	
		for(int x = floor(xll); x <= ceil(xur); x+=1) { 
			
			MGLvertex p;
			
			// Use the pixel center
			p.x = x+.5;
			p.y = y+.5;
			
			// Compute barycentric coordinates
			float alpha = (p.x * tri.v[1].y) - (p.x * tri.v[2].y) - (tri.v[1].x * p.y) + (tri.v[1].x * tri.v[2].y) + (tri.v[2].x * p.y) - (tri.v[2].x * tri.v[1].y);
			alpha /= detA;
			float beta = (tri.v[0].x * p.y) - (tri.v[0].x * tri.v[2].y) - (p.x * tri.v[0].y) + (p.x * tri.v[2].y) + (tri.v[2].x * tri.v[0].y) - (tri.v[2].x * p.y);
			beta /= detA;
			
            // float gamma = (tri.v[0].x * tri.v[1].y) - (tri.v[0].x * p.y) - (tri.v[1].x * tri.v[0].y) + (tri.v[1].x * p.y) + (p.x * tri.v[0].y) - (p.x * tri.v[1].y);
			// gamma /= detA;
            
            // possibly this is a better way to get gamma
            float gamma = 1.0f - alpha - beta;
			
			// Point/Triangle test
			if (alpha >= 0. && alpha <= 1. && beta >= 0. && beta <= 1. && gamma >= 0. && gamma <=1.)
			{
				///////////////////////////////////////////////////
				//
				// TODO:
				//
				// Here you will have to interpolate any useful
				// fragment attribute with barycentric interpolation.
				// Be careful that some attributes need perspective
				// correction (unlike z). 
				// You will then save those attributes in the fragment
				// buffer.
				//
				///////////////////////////////////////////////////
				
				
				// Barycentric interpolation of z
				p.z = alpha * tri.v[0].z + beta * tri.v[1].z + gamma * tri.v[2].z;
				
				// The following line needs to be changed to handle
				// perspective correct interpolation of colors.
               
                // TODO: PERSPECTIVE CORRECT COLORS
                p.color = tri.v[0].color;

                // This will be used in all perspective-correct calculations
                
                MGLfloat one_over_w = alpha * (1 / orig_w[0]) + beta * (1 / orig_w[1]) + gamma * (1 / orig_w[2]);

                // Diffuse Texture Interpoloation
                MGLfloat diffuse_bary_u, diffuse_bary_v;
                MGLfloat diff_u_over_w =   alpha * (tri.v[0].diffuse_u / orig_w[0]) + beta * (tri.v[1].diffuse_u / orig_w[1]) + gamma * (tri.v[2].diffuse_u / orig_w[2]);
                MGLfloat diff_v_over_w =   alpha * (tri.v[0].diffuse_v / orig_w[0]) + beta * (tri.v[1].diffuse_v / orig_w[1]) + gamma * (tri.v[2].diffuse_v / orig_w[2]);
                diffuse_bary_u = diff_u_over_w / one_over_w;
                diffuse_bary_v = diff_v_over_w / one_over_w;
                
                // Specular Texture Interpolation
                MGLfloat specular_bary_u, specular_bary_v;
                MGLfloat spec_u_over_w =   alpha * (tri.v[0].specular_u / orig_w[0]) + beta * (tri.v[1].specular_u / orig_w[1]) + gamma * (tri.v[2].specular_u / orig_w[2]);
                MGLfloat spec_v_over_w =   alpha * (tri.v[0].specular_v / orig_w[0]) + beta * (tri.v[1].specular_v / orig_w[1]) + gamma * (tri.v[2].specular_v / orig_w[2]);
                specular_bary_u = spec_u_over_w / one_over_w;
                specular_bary_v = spec_v_over_w / one_over_w;
                
                // eyeX Interpolation
                MGLfloat bary_eyeX;
                MGLfloat eyeX_over_w =  alpha * (tri.v[0].eyeX / orig_w[0]) + beta * (tri.v[1].eyeY / orig_w[1]) + gamma * (tri.v[2].eyeZ / orig_w[2]);
                bary_eyeX = eyeX_over_w / one_over_w;

                // eyeY Interpolation
                MGLfloat bary_eyeY;
                MGLfloat eyeY_over_w =  alpha * (tri.v[0].eyeY / orig_w[0]) + beta * (tri.v[1].eyeY / orig_w[1]) + gamma * (tri.v[2].eyeZ / orig_w[2]);
                bary_eyeY = eyeY_over_w / one_over_w;

                // eyeZ Interpolation
                MGLfloat bary_eyeZ;
                MGLfloat eyeZ_over_w =  alpha * (tri.v[0].eyeZ / orig_w[0]) + beta * (tri.v[1].eyeY / orig_w[1]) + gamma * (tri.v[2].eyeZ / orig_w[2]);
                bary_eyeZ = eyeZ_over_w / one_over_w;

                
				MGLsize idx = x + y * fragbuf.width;
				
				// Z-buffer comparison
				if (p.z < fragbuf.data[idx].depth ) {
					fragbuf.data[idx].color = p.color;
					fragbuf.data[idx].depth = p.z;
                    
                    fragbuf.data[idx].texturesEnabled = tri.v[0].texturesEnabled;
                    
                    fragbuf.data[idx].diffuse_id = tri.v[0].diffuse_tex_id;
                    fragbuf.data[idx].diffuse_u = diffuse_bary_u;
                    fragbuf.data[idx].diffuse_v = diffuse_bary_v;
                    
                    fragbuf.data[idx].specular_id = tri.v[0].specular_tex_id;
//                    fragbuf.data[idx].specular_u = specular_bary_u;
//                    fragbuf.data[idx].specular_v = specular_bary_v;
//                    
//                    fragbuf.data[idx].eyeX = bary_eyeX;
//                    fragbuf.data[idx].eyeY = bary_eyeY;
//                    fragbuf.data[idx].eyeZ = bary_eyeZ;
                    
				}
				
				///////////////////////////////////////////////////
				//
				// END HERE
				//
				///////////////////////////////////////////////////
				
				
			}
		}
	}
}

/**
 * Fragment shader.
 *
 * Input - frag
 * Output - data
 */
void shadeFragment(const MGLfragment& frag, MGLpixel& data)
{
    
    MGLpixel cur_color;
    
    if (frag.depth < 1.0) { // it isn't simply a background fragment
        
        
        
        if (frag.diffuse_id != 0 && frag.texturesEnabled) {
            MGLpixel biLerped = MGLColorBilerp(frag.diffuse_id, frag.diffuse_u, frag.diffuse_v);
            cur_color = biLerped;
            
            printf("test");
        }
        
        if (lightingEnabled) {
            for (int l = 0; l < 3; l++) { // for all 3 lights
                if ( lights.enabled[l] ) {
                    
                    MGLpixel amb_light = MGLColorFromFloats(lights.light_list[l].ambient);
                    MGLpixel amb_mat = MGLColorFromFloats(frag.material.ambient);
                    
                    MGLpixel amb_component = MGLColorMult(amb_light, amb_mat);
                    
                    cur_color = MGLColorAdd(cur_color, amb_component);
                    
                    
                    // add in diffuse
                    
                    // add in specular
                }
            }
        }
        
        else {
            //cur_color = MGLColorMult(cur_color, frag.color);
        }

    }
    
    data = cur_color;
}

/*************************************************************
 * Lighting/shading/texture functions (to be implemented for
 * Assignment 2).
 *************************************************************/

/**
 * Enable or disable lighting for the rendered scene.  Lighting should
 * be disabled by default.
 */
void mglLightingEnabled(bool enabled)
{
    lightingEnabled = enabled;
}

/**
 * Enable or disable the specified individual light.  All lights
 * should be disabled by default.
 */
void mglLightEnabled(MGLlight light, bool enabled)
{
    lights.enabled[light] = enabled;
}

/**
 * Set the type of shading (Phong or toon) to be used when
 * the scene is rendered.  For Phong shading, the per-pixel lighting
 * equations should be used for specular, ambient, and diffuse lighting.
 * Toon shading is similar, except that a discretized (rather than 
 * continuous) value is used for the angle between the light vector and 
 * the normal vector.
 */
void mglShadingMode(MGLshading_mode mode)
{
    curShadingMode = mode;
}

/**
 * Set parameters for a particular light.  light specifies
 * the light whose parameters we should set, pname specifies
 * the parameter to set, and values is a pointer to the floats
 * which specify the value for that parameter.  Parameters
 * are handled as follows:
 *
 * MGL_LIGHT_AMBIENT
 * Set the ambient intensity of the light; values should point
 * to three floats representing the ambient RGB (in that order)
 * intensity of the light in homogeneous coordinates.  The initial
 * ambient intensity should be (0, 0, 0).
 *
 * MGL_LIGHT_DIFFUSE
 * Set the diffuse intensity of the light; values should point to three
 * floats representing the diffuse RGB (in that order) intensity of
 * the light in homogeneous coordinates.  The initial diffuse intenisty
 * for MGL_LIGHT0 should be (1, 1, 1); for all others, it should
 * be (0, 0, 0).
 * 
 * MGL_LIGHT_SPECULAR
 * Set the specular intensity of the light; values should be point to
 * three floats representing the specular RGB (in that order) intensity
 * of the light in homogeneous coordinates.  The initial specular
 * intensity for MGL_LIGHT0 should be (1, 1, 1); for all others,
 * it should be (0, 0, 0).
 *
 * MGL_LIGHT_POSITION
 * Set the position of the light.  This position should be transformed
 * by the modelview matrix upon specification.  values should point to
 * three floats representing the position of the light in homogeneous
 * coordinates.  The initial position should be (0, 0, 1).
 */
void mglLight(MGLlight light,
              MGLlight_param pname,
              MGLfloat *values)
{
    if (light > 2) {
        MGL_ERROR("Trying to access light that doens't exist");
    }
    else {
        if (pname == MGL_LIGHT_AMBIENT) {
            lights.light_list[light].ambient[0] = values[0];
            lights.light_list[light].ambient[1] = values[1];
            lights.light_list[light].ambient[2] = values[2];
        }
        else if (pname == MGL_LIGHT_DIFFUSE) {
            lights.light_list[light].diffuse[0] = values[0];
            lights.light_list[light].diffuse[1] = values[1];
            lights.light_list[light].diffuse[2] = values[2];
        }
        else if (pname == MGL_LIGHT_SPECULAR) {
            lights.light_list[light].specular[0] = values[0];
            lights.light_list[light].specular[1] = values[1];
            lights.light_list[light].specular[2] = values[2];
        }
        else if (pname == MGL_LIGHT_POSITION){
            lights.light_list[light].x = values[0];
            lights.light_list[light].y = values[1];
            lights.light_list[light].z = values[2];
        }
        else {
            MGL_ERROR("Unknown light parameter");
        }
    }
}

/**
 * Set the material properties for vertices that are being specified.
 * pname specifies the property being updated, and values is a
 * pointer to the float or floats which specify the value for that
 * parameter.  Parameters are handled as follows:
 *
 * MGL_MAT_AMBIENT
 * Set the ambient reflectance of the material.  values points to three
 * floats specifying the RGB (in that order) reflectance.  The initial
 * ambient reflectance of all materials should be (0.2, 0.2, 0.2).
 *
 * MGL_MAT_DIFFUSE
 * Set the diffuse reflectance of the material.  values points to three
 * floats specfiying the RGB (in that order) reflectance.  The initial
 * diffuse reflectance of all materials should be (0.8, 0.8, 0.8).
 *
 * MGL_MAT_SPECULAR
 * Set the specular reflectance of the material.  values points to three
 * floats specifying the RGB (in that order) reflectance.  The initial
 * specular reflectance for all materials should be (0.0, 0.0, 0.0).
 *
 * MGL_MAT_SHININESS
 * Set the specular exponent for the material.  values points to a single
 * float representing this exponent.  The initial specular exponent for
 * all materials should be 0.0.
 */
void mglMaterial(MGLmat_param pname,
                 MGLfloat *values)
{
    if (pname == MGL_MAT_AMBIENT) {
        curMaterial.ambient[0] = values[0];
        curMaterial.ambient[1] = values[1];
        curMaterial.ambient[2] = values[2];
    }
    else if (pname == MGL_MAT_DIFFUSE) {
        curMaterial.diffuse[0] = values[0];
        curMaterial.diffuse[1] = values[1];
        curMaterial.diffuse[2] = values[2];
    }
    else if (pname == MGL_MAT_SPECULAR) {
        curMaterial.specular[0] = values[0];
        curMaterial.specular[1] = values[1];
        curMaterial.specular[2] = values[2];
    }
    else if (pname == MGL_MAT_SHININESS) {
        curMaterial.shininess = values[0];
    }
    else {
        MGL_ERROR("Unknown material parameter");
    }
}

/**
 * Set whether textures should be drawn over surfaces which are being
 * specified.  Note that, unlike mglLightingEnabled(), this function
 * does not affect the render-time state of the scene - rather, it
 * affects the state of surfaces as they are specified.  Thus at render
 * time, we might have some surfaces with textures enabled, and some
 * without.  See also: mglTextureSlot.
 *
 * If lighting is disabled, then the specular texture slot is ignored,
 * and the diffuse texture is modulated with the color specified by 
 * mglColor.
 *
 * The initial value of this attribute should be false.
 */
void mglTexturesEnabled(bool enabled)
{
    if (hasBegun) {
        MGL_ERROR("Cannot change texture enabled mode after mglBegin");
    }
    else {
        texturesEnabled = enabled;
    }
}

/**
 * Load a texture into memory.  As long as textures have been enabled
 * with mglSetTexturesEnabled(), draw this texture over specified
 * surfaces until mglLoadTexture() is called again.  width and height
 * specify the dimensions of the image, and imageData points to an
 * array of RGB pixels.  The array begins with the bottom-left pixel
 * of the image, then the bottom-second-to-left, etc. - that is, elements
 * of the array are ordered first horizontally across the image, then
 * vertically up it.
 *
 * When this function is called, the in-use texture should be set
 * to the one that has been loaded - that is, the user doesn't need
 * to call both mglLoadTexture() and mglUseTexture() if she wishes
 * to use a newly loaded texture.
 *
 * Returns an ID (just an unsigned integer) by which the loaded
 * texture can be referenced in the future, e.g. with mglUseTexture().
 *
 * Note that you should make a copy of the data that's passed in,
 * rather than just store the imageData pointer.  The user can
 * use mglFreeTexture() to free the data that has been stored.
 */
MGLtex_id mglLoadTexture(MGLsize width,
                         MGLsize height,
                         MGLpixel *imageData)
{
    
    if (hasBegun) {
        MGL_ERROR("Cannot change texture after mglBegin");
        return 0;
    }
    else {
        
        MGLtexture loaded_tex = MGLtexture(width, height);
        memcpy(loaded_tex.data, imageData, width * height * sizeof(MGLpixel));
        
        textures.push_back(loaded_tex);
        
        MGLtex_id id = textures.size() - 1;
        
        if (curTextureSlot == MGL_TEX_DIFFUSE) {
            curDiffuseTextureID = id;
        }
        else {
            curSpecularTextureID = id;
        }
        
        return id;
    }
}

/**
 * Specify the texture which should be applied to newly specified
 * surfaces, using the ID returned by mglLoadTexture().  When a 
 * texture is applied to the surface, it should be modulated 
 * (multiplied) with the material color.  See also: mglTextureSlot.
 * 
 * By default, the texture ID is set to zero (the NULL texture).
 */
void mglUseTexture(MGLtex_id id)
{
    if (hasBegun) {
        MGL_ERROR("Cannot change texture after mglBegin");
    }
    else {
        
        if (curTextureSlot == MGL_TEX_DIFFUSE) {
            curDiffuseTextureID = id;
        }
        else {
            curSpecularTextureID = id;
        }
    }
}

/**
 * Free any memory associated with the specified texture.  A user
 * should call this once she knows a texture will no longer be used.
 * Note that this invalidates the given ID - that is, an error
 * should be thrown if it is later passed to mglUseTexture().
 */
void mglFreeTexture(MGLtex_id id)
{
    //    MGLint width = textures[id].tex_width;
    //    MGLint height = textures[id].tex_height;
    
    
    // TODO: delete texture data here!
    
    
}

/**
 * Should be called just before a call to mglVertex*(); specifies
 * a texture coordinate on the current texture for the vertex
 * which is about to be specified.
 */
void mglTexCoord(MGLfloat x,
                 MGLfloat y)
{
    
    if (curTextureSlot == MGL_TEX_DIFFUSE) {
        curDiffuseTextureCoord.x = x;
        curDiffuseTextureCoord.y = y;
    }
    else {
        curSpecularTextureCoord.x = x;
        curSpecularTextureCoord.y = y;
    }
}

/**
 * Sets the active texture slot.  There are two texture slots.
 * 
 * If MGL_TEX_DIFFUSE is set, then calls to mglTexturesEnabled
 * and mglUseTexture enable texturing and set the texture 
 * for the diffuse color of the object, respectively.
 *
 * If MGL_TEX_SPECULAR is set, then calls to mglTexturesEnabled
 * and mglUseTexture enable texturing and set the texture
 * for the specular color of the object, respectively.
 *
 * The initial value for this attribute should be MGL_TEX_DIFFUSE.
 */
void mglTextureSlot(MGLtex_slot slot)
{
    curTextureSlot = slot;
}

/**
 * Specify the normal vector for subsequent vertices, to be
 * used in lighting computations.  Note that this normal is
 * persistent - it should be used for all specified vertices until
 * this function has been called again.
 */
void mglNormal(MGLfloat x,
               MGLfloat y,
               MGLfloat z)
{
    curNormal.data[0] = x;
    curNormal.data[1] = y;
    curNormal.data[2] = z;
}

/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data)
{
    if (hasBegun) MGL_ERROR("mglReadPixels executed after mglBegin.");
    
    MGLfragbuffer fragbuf(width, height);
    
    // Rasterize each triangle from the vertices array
    for (size_t i = 0; i < transformedVertices.size(); i += 3) {
        MGLtriangle tri;
        tri.v[0] = transformedVertices[i];
        tri.v[1] = transformedVertices[i + 1];
        tri.v[2] = transformedVertices[i + 2];
        rasterize(tri, fragbuf);
    }
    
    // Here it is actually similar to per-fragment shader, where you output
    // the final color for each pixel from the fragment data it contains.
    // Currently it simply copies fragment color to the pixel, and you will
    // need to modify that to account for other new attributes you added.
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            shadeFragment(fragbuf.data[i * width + j],
                          data[i * width + j]);
        }
    }
}

/**
 * Force execution of minigl commands in finite time.
 *
 * We will use this command differently from what it should be
 * originally. It is always called after mglReadPixels, being
 * the last one among the sequence of minigl calls. So this
 * function is a place where you put all the cleanup codes and
 * reset the states such as global variables.
 */
void mglFlush()
{
    if (hasBegun) MGL_ERROR("mglFlush executed after mglBegin.");
    
    transformedVertices.clear();
}

/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode)
{
    if (hasBegun) MGL_ERROR("mglBegin executed after mglBegin.");
    
    hasBegun = true;
    curPolyMode = mode;
}

/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd()
{
    if (!hasBegun) MGL_ERROR("mglEnd has unmatched mglBegin.");
    
    hasBegun = false;
    if (curIndex != 0) MGL_ERROR("incompleted primitives specified.");
}

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y)
{
    mglVertex3(x, y, 0.0f);
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z)
{
    if (!hasBegun) MGL_ERROR("mglVertex executed outside mglBegin/mglEnd.");
    
    MGLfloat v1[4] = {x, y, z, 1.0f};
    MGLfloat v2[4];
    mulMatrixVector(modelViewMatrix, v1, v2);
    mulMatrixVector(projectionMatrix, v2, v1);
    
    
    MGLvertex v;
    v.x = v1[0];
    v.y = v1[1];
    v.z = v1[2];
    v.w = v1[3];
    
    v.eyeX = v2[0];
    v.eyeY = v2[1];
    v.eyeZ = v2[2];
    v.eyeW = v2[3];
    
    ///////////////////////////////////////////////////////
    //
    // TODO:
    //
    // Assign your new added vertex attributes here.
    //
    // ////////////////////////////////////////////////////
    
    v.color = curColor;
    v.mat = curMaterial;
    
    // assign normal
    
     
    MGLint curSpecTexWidth = textures[curSpecularTextureID].tex_width;
    MGLint curSpecTexHeight = textures[curSpecularTextureID].tex_height;

    v.specular_tex_id = curSpecularTextureID;
    v.specular_tex_width = curSpecTexWidth;
    v.specular_tex_height = curSpecTexHeight;
    v.specular_u = curSpecularTextureCoord.x;
    v.specular_v = curSpecularTextureCoord.y;
    
    ///////////////////////////////////////////////////////
    //
    // END HERE
    //
    ///////////////////////////////////////////////////////
    
    transformedVertices.push_back(v);
    
    if (curPolyMode == MGL_TRIANGLES) curIndex = (curIndex + 1) % 3;
    else if (++curIndex == 4) {
        transformedVertices.push_back(*(transformedVertices.end() - 4));
        transformedVertices.push_back(*(transformedVertices.end() - 3));
        curIndex = 0;
    }
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode)
{
    if (hasBegun) MGL_ERROR("mglMatrixMode executed after mglBegin.");
    
    curMatrixMode = mode;
    if (mode == MGL_MODELVIEW) curMatrix = &modelViewMatrix;
    else curMatrix = &projectionMatrix;
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix()
{
    if (hasBegun) MGL_ERROR("mglPushMatrix executed after mglBegin.");
    
    if (curMatrixMode == MGL_MODELVIEW) modelViewStack.push(modelViewMatrix);
    else projectionStack.push(projectionMatrix);
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
    if (hasBegun) MGL_ERROR("mglPopMatrix executed after mglBegin.");
    
    if (curMatrixMode == MGL_MODELVIEW) {
        if (modelViewStack.empty()) MGL_ERROR("mglPopMatrix underflow the stack.");
        
        modelViewMatrix = modelViewStack.top();
        modelViewStack.pop();
    } else {
        if (projectionStack.empty()) MGL_ERROR("mglPopMatrix underflow the stack.");
        
        projectionMatrix = projectionStack.top();
        projectionStack.pop();
    }
}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity()
{
    if (hasBegun) MGL_ERROR("mglLoadIdentity executed after mglBegin.");
    
    for (int i = 0; i < 16; ++i) {
        curMatrix->m[i] = (i % 5 == 0);
    }
}

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix)
{
    if (hasBegun) MGL_ERROR("mglLoadMatrix executed after mglBegin.");
    
    memcpy(curMatrix->m, matrix, 16 * sizeof(MGLfloat));
}

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix)
{
    if (hasBegun) MGL_ERROR("mglMultMatrix executed after mglBegin.");
    
    MGLmatrix result;
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            MGLfloat sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += curMatrix->m[k * 4 + j] * matrix[i * 4 + k];
            }
            result.m[i * 4 + j] = sum;
        }
    }
    
    *curMatrix = result;
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z)
{
    if (hasBegun) MGL_ERROR("mglTranslate executed after mglBegin.");
    
    MGLmatrix tMat;
    tMat.m[12] = x;
    tMat.m[13] = y;
    tMat.m[14] = z;
    mglMultMatrix(tMat.m);
}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
               MGLfloat x,
               MGLfloat y,
               MGLfloat z)
{
    if (hasBegun) MGL_ERROR("mglRotate executed after mglBegin.");
    
    MGLmatrix rMat;
    angle *= M_PI / 180.0f;
    // normalize vector <x, y, z>
    MGLfloat l = sqrt(x * x + y * y + z * z);
    if (l == 0.0f) MGL_ERROR("mglRotate vector must be non-zero.");
    x /= l;
    y /= l;
    z /= l;
    MGLfloat c = cos(angle);
    MGLfloat s = sin(angle);
    
    rMat.m[0] = x * x * (1.0f - c) + c;
    rMat.m[1] = y * x * (1.0f - c) + z * s;
    rMat.m[2] = x * z * (1.0f - c) - y * s;
    rMat.m[4] = x * y * (1.0f - c) - z * s;
    rMat.m[5] = y * y * (1.0f - c) + c;
    rMat.m[6] = y * z * (1.0f - c) + x * s;
    rMat.m[8] = x * z * (1.0f - c) + y * s;
    rMat.m[9] = y * z * (1.0f - c) - x * s;
    rMat.m[10] = z * z * (1.0f - c) + c;
    mglMultMatrix(rMat.m);
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z)
{
    if (hasBegun) MGL_ERROR("mglScale executed after mglBegin.");
    
    MGLmatrix sMat;
    sMat.m[0] = x;
    sMat.m[5] = y;
    sMat.m[10] = z;
    mglMultMatrix(sMat.m);
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
                MGLfloat right,
                MGLfloat bottom,
                MGLfloat top,
                MGLfloat near,
                MGLfloat far)
{
    if (hasBegun) MGL_ERROR("mglFrustum executed after mglBegin.");
    if (left == right || bottom == top || near == far ||
        near <= 0 || far <= 0) {
        MGL_ERROR("mglFrustum invalid arguments.");
    }
    
    MGLmatrix fMat;
    fMat.m[0] = 2.0f * near / (right - left);
    fMat.m[5] = 2.0f * near / (top - bottom);
    fMat.m[8] = (right + left) / (right - left);
    fMat.m[9] = (top + bottom) / (top - bottom);
    fMat.m[10] = -(far + near) / (far - near);
    fMat.m[11] = -1.0f;
    fMat.m[14] = -2.0f * far * near / (far - near);
    fMat.m[15] = 0.0f;
    mglMultMatrix(fMat.m);
}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left,
              MGLfloat right,
              MGLfloat bottom,
              MGLfloat top,
              MGLfloat near,
              MGLfloat far)
{
    if (hasBegun) MGL_ERROR("mglOrtho executed after mglBegin.");
    if (left == right || bottom == top || near == far) {
        MGL_ERROR("mglOrtho invalid arguments.");
    }
    
    MGLmatrix oMat;
    oMat.m[0] = 2.0f / (right - left);
    oMat.m[5] = 2.0f / (top - bottom);
    oMat.m[10] = -2.0f / (far - near);
    oMat.m[12] = -(right + left) / (right - left);
    oMat.m[13] = -(top + bottom) / (top - bottom);
    oMat.m[14] = -(far + near) / (far - near);
    mglMultMatrix(oMat.m);
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLbyte red,
              MGLbyte green,
              MGLbyte blue)
{
    MGL_SET_RED(curColor, red);
    MGL_SET_GREEN(curColor, green);
    MGL_SET_BLUE(curColor, blue);
}
