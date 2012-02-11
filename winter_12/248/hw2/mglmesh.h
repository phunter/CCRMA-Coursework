#ifndef __MGLMESH_H__
#define __MGLMESH_H__

#include <string>
#include <vector>
#include "minigl.h"


/* Class: MGLmesh
 * This class loads a mesh in obj format and stores 
 * the resulting data as internal state.
 * Note: The mesh must be fully triangulated.
 */
class MGLmesh
{
public:

    /**
     * Constructor and destructor
     */
    MGLmesh();
    ~MGLmesh();

    /**
     * Function: load()
     * Loads the .obj file specified by FILENAME
     */
    void load(const std::string filename);

    /**
     * Function: display()
     * This function calls the appropriate miniGL calls to draw the mesh.
     * Don't forget to do the following before displaying:
     *   - Set Lighting to enabled
     *   - Select the shading model
     */
    void display();

    /**
     * Function: numVertices()
     * Returns the number of vertices in the mesh.
     */
    unsigned int numVertices();

    /**
     * Function: vertices()
     * Returns a vector of vertices, laid out sequentially as x,y,z coordinates
     * E.g.: [x1,y1,z1, x2,y2,z2, ..., xn, yn,zn]
     */
    std::vector<MGLfloat>& vertices();

    /**
     * Function: normals()
     * Returns a vector of normal coordinates, laid out sequentially as x,y,z components
     * E.g.:   [nx1,ny1,nz1, nx2,ny2,nz2, ..., nxn, nyn,nzn]
     */
    std::vector<MGLfloat>& normals();

    /**
     * Function: texCoords()
     * Returns a vector of texture coordinates, laid out sequentially as s,t components
     * E.g.:   [s1,t1, s2,t2, ..., sn, tn]
     */
    std::vector<MGLfloat>& texCoords();

    /**
     * Function: diffuseColor()
     * Returns a vector with 3 elements, [R G B], for the object's diffuse color
     * Note: All color values in the obj file must be specified on the interval
     * [0.0 1.0]
     */
    std::vector<MGLfloat>& diffuseColor();

    /**
     * Function: specularColor()
     * Returns a vector with 3 elements, [R G B], for the object's specular color
     * Note: All color values in the obj file must be specified on the interval
     * [0.0 1.0]
     */
    std::vector<MGLfloat>& specularColor();

    /**
     * Function: ambientColor()
     * Returns a vector with 3 elements, [R G B], for the object's ambient color
     * Note: All color values in the obj file must be specified on the interval
     * [0.0 1.0]
     */
    std::vector<MGLfloat>& ambientColor();

    /**
     * Function: shininess()
     * The "shininess" parameter (i.e. exponent) used for calculating the specular
     * component in Phong Shading
     */
    MGLfloat shininess();

    /**
     * Function: diffuse_texture()
     * Returns the filename of the texture map used for the diffuse color
     */
    std::string diffuse_texture();

    /**
     * Function: specular_texture()
     * Returns the filename of the texture map used for the specular color
     */
    std::string specular_texture();

private:

    void parseFace(std::string facestring, int indices[]);
    void parseMaterial(std::string materialfn);

    unsigned int mNumVerts;
    std::vector<MGLfloat> mVertices;
    std::vector<MGLfloat> mNormals;
    std::vector<MGLfloat> mTexCoords;
    std::vector<MGLfloat> diffuse_color;
    std::vector<MGLfloat> specular_color;
    std::vector<MGLfloat> ambient_color;
    MGLfloat specular_exponent;
    std::string diffuse_texture_file;
    std::string specular_texture_file;
    MGLtex_id mDiffTexID;
    MGLtex_id mSpecTexID;
};

#endif
