/**
 * minigl.h
 * Hunter McCurry
 * CS248 Assignment 1, Winter 2010
 * -------------------------------
 * This file defines the API to be implemented for MiniGL.
 */

#ifndef __MINIGL_H__
#define __MINIGL_H__

typedef int           MGLint;
typedef unsigned char MGLbyte;
typedef unsigned int  MGLsize;
typedef unsigned int  MGLpixel;
typedef float         MGLfloat;
typedef bool          MGLbool;

typedef enum {
    MGL_MODELVIEW,
    MGL_PROJECTION
} MGLmatrix_mode;

typedef enum {
    MGL_TRIANGLES,
    MGL_QUADS
} MGLpoly_mode;

/**
 * Macros to get and set color values from an individual
 * pixel.
 */
#define MGL_GET_RED(P)   (((P) & 0x000000ff))
#define MGL_GET_GREEN(P) (((P) & 0x0000ff00) >> 8)
#define MGL_GET_BLUE(P)  (((P) & 0x00ff0000) >> 16)

#define MGL_SET_RED(P, C)   (P = (((P) & 0xffffff00) | (C)))
#define MGL_SET_GREEN(P, C) (P = (((P) & 0xffff00ff) | ((C) << 8)))
#define MGL_SET_BLUE(P, C)  (P = (((P) & 0xff00ffff) | ((C) << 16)))

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
                   MGLpixel *data);

/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode);

/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd();

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y);

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z);

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode);

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix();

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix();

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity();

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
void mglLoadMatrix(const MGLfloat *matrix);

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
void mglMultMatrix(const MGLfloat *matrix);

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z);

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
               MGLfloat x,
               MGLfloat y,
               MGLfloat z);

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z);

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
                MGLfloat right,
                MGLfloat bottom,
                MGLfloat top,
                MGLfloat near,
                MGLfloat far);

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left,
              MGLfloat right,
              MGLfloat bottom,
              MGLfloat top,
              MGLfloat near,
              MGLfloat far);

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLbyte red,
              MGLbyte green,
              MGLbyte blue);


#endif
