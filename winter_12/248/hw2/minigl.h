/**
 * minigl.h
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
typedef unsigned int  MGLtex_id;
typedef enum {
    MGL_MODELVIEW,
    MGL_PROJECTION
} MGLmatrix_mode;

typedef enum {
    MGL_TRIANGLES,
    MGL_QUADS
} MGLpoly_mode;

typedef enum {
  MGL_PHONG,
  MGL_TOON
} MGLshading_mode;

typedef enum {
  MGL_LIGHT0 = 0,
  MGL_LIGHT1,
  MGL_LIGHT2,
  MGL_LIGHT_NUM
} MGLlight;

typedef enum {
  MGL_LIGHT_AMBIENT,
  MGL_LIGHT_DIFFUSE,
  MGL_LIGHT_SPECULAR,
  MGL_LIGHT_POSITION
} MGLlight_param;

typedef enum {
  MGL_MAT_AMBIENT,
  MGL_MAT_DIFFUSE,
  MGL_MAT_SPECULAR,
  MGL_MAT_SHININESS
} MGLmat_param;

typedef enum {
  MGL_TEX_DIFFUSE,
  MGL_TEX_SPECULAR,
  MGL_TEX_SLOT_NUM
} MGLtex_slot;

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
 * Enable or disable lighting for the rendered scene.  Lighting should
 * be disabled by default.
 */
void mglLightingEnabled(bool enabled);

/**
 * Enable or disable the specified individual light.  All lights
 * should be disabled by default.
 */
void mglLightEnabled(MGLlight light, bool enabled);

/**
 * Set the type of shading (Phong or toon) to be used when
 * the scene is rendered.  For Phong shading, the per-pixel lighting
 * equations should be used for specular, ambient, and diffuse lighting.
 * Toon shading is similar, except that a discretized (rather than 
 * continuous) value is used for the angle between the light vector and 
 * the normal vector.
 */
void mglShadingMode(MGLshading_mode mode);

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
	      MGLfloat *values);

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
		 MGLfloat *values);

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
 * The initial value this attribute should be false.
 */
void mglTexturesEnabled(bool enabled);

 MGLtex_id mglLoadTexture(MGLsize width,
			 MGLsize height,
			 MGLpixel *imageData);

/**
 * Specify the texture which should be applied to newly specified
 * surfaces, using the ID returned by mglLoadTexture().  When a 
 * texture is applied to the surface, it should be modulated 
 * (multiplied) with the material color.  See also: mglTextureSlot.
 * 
 * By default, the texture ID is set to zero (the NULL texture).
 */
void mglUseTexture(MGLtex_id id);

/**
 * Free any memory associated with the specified texture.  A user
 * should call this once she knows a texture will no longer be used.
 * Note that this invalidates the given ID - that is, an error
 * should be thrown if it is later passed to mglUseTexture().
 */
void mglFreeTexture(MGLtex_id id);

/**
 * Should be called just before a call to mglVertex*(); specifies
 * a texture coordinate on the current texture for the vertex
 * which is about to be specified.
 */
void mglTexCoord(MGLfloat x,
		 MGLfloat y);

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
void mglTextureSlot(MGLtex_slot slot);

/**
 * Specify the normal vector for subsequent vertices, to be
 * used in lighting computations.  Note that this normal is
 * persistent - it should be used for all specified vertices until
 * this function has been called again.
 */
void mglNormal(MGLfloat x,
	       MGLfloat y,
	       MGLfloat z);

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
 * Force execution of minigl commands in finite time.
 *
 * We will use this command differently from what it should be
 * originally. It is always called after mglReadPixels, being
 * the last one among the sequence of minigl calls. So this
 * function is a place where you put all the cleanup codes and
 * reset the states such as global variables.
 */
void mglFlush();

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
