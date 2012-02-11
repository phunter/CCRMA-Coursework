/**
 * minigl.cpp
 * Hunter McCurry
 * CS248 Assignment 1, Winter 2010
 * -------------------------------
 * Implement miniGL here.
 * Do not use any additional files
 */

#include <cstdio>
#include <cstdlib>
#include <math.h>
#include "minigl.h"

#include <stack>
#include <vector>
#include <assert.h>

using namespace std;

class MGLvector4 {
    vector <MGLfloat> data;

public:
    
    MGLvector4() {
        data.resize(4);
        for (int i = 0; i < 4; i++) {
            data[i] = 0.0;
        }
        //printf("data in initial MGLvector4 is [%f,%f,%f,%f]\n",data[0],data[1],data[2],data[3]);
    }
    
    MGLvector4(MGLfloat x, MGLfloat y, MGLfloat z, MGLfloat w) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }
    
    float Get(int i) {
        return data[i];
    }
    
    void Set(int i, MGLfloat val) {
        data[i] = val;
    }
    
    void AddVector(MGLvector4 vec) {
        for (int i = 0; i < 4; i++) {
            data[i] += vec.Get(i);
        }
    }
    
    void SetData(MGLfloat x, MGLfloat y, MGLfloat z, MGLfloat w) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }
    
    void Normalize() {
        MGLfloat mag = sqrt(pow(data[0],2) + pow(data[1],2) + pow(data[2],2) + pow(data[3],2));
        data[0] /= mag;
        data[1] /= mag;
        data[2] /= mag;
        data[3] /= mag;
    }
};


class MGLvertex {
private:
    MGLvector4 *position;
    MGLpixel color;
    
public:
    MGLvertex() {
        position = new MGLvector4();
    }
    
    MGLvector4 * GetPosition() {
        return position;
    }
    
    MGLpixel GetColor() {
        return color;
    }
    
    MGLfloat GetCoord(int i) {
        return position->Get(i);
    }
    
    void SetCoord(int i, MGLfloat val) {
        position->Set(i, val);
    }
    
    void SetPosition(MGLvector4 *pos) {
        position = pos;
    }
    void SetColor(MGLpixel col) {
        color = col;
    }
};

struct Line {
    MGLfloat A, B, C;
};

struct BBox {
	MGLint xmin, xmax, ymin, ymax;
};


class MGLmatrix {
    
private:
    //vector <float> data;
    MGLfloat matrix[16];
    
public:
    MGLmatrix() {
        //data.resize(16);
        LoadIdentity();
    }
    
    void LoadIdentity() {
        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                if (c == r) {
                    matrix[4*c + r] = 1;
                }
                else {
                    matrix[4*c + r] = 0;
                }
            }
        }
    }
    
    MGLfloat * GetMatrix() {
        return matrix;
    }
    
    MGLfloat GetData(int col, int row) {
        return matrix[4*col + row];
    }
    
    void SetData(int col, int row, float value) {
        matrix[4*col + row] = value;
    }
    
    void CopyMatrix(MGLmatrix * mat) {
        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                SetData(c,r,mat->GetData(c,r));
            }
        }
    }
    
    void MultMatrix(MGLmatrix transform_mat) {
        MGLmatrix new_mat = MGLmatrix();
        
        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                new_mat.SetData( c, r, (GetData(0,r) * transform_mat.GetData(c,0) +
                                        GetData(1,r) * transform_mat.GetData(c,1) +
                                        GetData(2,r) * transform_mat.GetData(c,2) +
                                        GetData(3,r) * transform_mat.GetData(c,3)));
            }
        }
        
        CopyMatrix(&new_mat);
    }
    
    MGLvector4 * MultVector(MGLvector4 in_vector) {
        MGLvector4 *new_vector = new MGLvector4();
        
        for (int r = 0; r < 4; r++) {
            new_vector->Set(r, matrix[0*4+r] * in_vector.Get(0) +
                            matrix[1*4+r] * in_vector.Get(1) +
                            matrix[2*4+r] * in_vector.Get(2) +
                            matrix[3*4+r] * in_vector.Get(3));
        }
        return new_vector;
    }
    
    void Print() {
        printf("[%f, %f, %f, %f,]\n[%f, %f, %f, %f,]\n[%f, %f, %f, %f,]\n[%f, %f, %f, %f,]\n",
               matrix[0],matrix[4],matrix[8],matrix[12],
               matrix[1],matrix[5],matrix[9],matrix[13],
               matrix[2],matrix[6],matrix[10],matrix[14],
               matrix[3],matrix[7],matrix[11],matrix[15]);
    }
        
};

class MGLmat_stack {
private:
    stack <MGLmatrix*> mat_stack;
    
public:
    
    MGLmat_stack() {
        MGLmatrix *ident = new MGLmatrix();
        mat_stack.push(ident);
    }
    
    void Push(MGLmatrix *mat) {
        mat_stack.push(mat);
    }
    
    MGLmatrix * Pop() {
        MGLmatrix *mat = mat_stack.top();
        mat_stack.pop();
        return mat;
    }
    
    MGLmatrix * Top() {
        return mat_stack.top();
    }
    
    long unsigned int Size() {
        return mat_stack.size();
    }
};


// State Variables
bool drawing = false;
MGLmatrix_mode g_matrix_mode;
MGLpoly_mode g_poly_mode;
MGLpixel cur_color;
MGLint cur_num_verts = 0;


// render dimensions
MGLint g_width;
MGLint g_height;

// write image here
MGLpixel *g_data;


// minigl stl global data structures
vector <MGLvertex> g_vertices;

MGLmat_stack g_modelview;
MGLmat_stack g_projection;

MGLmat_stack *g_cur_stack;



//////////////////////////////
// Helper Functions (User doesn't call these)

void rasterize( int t );
void fragment(int x, int y);

void ConvertToNDC() {
    for (unsigned int i = 0; i < g_vertices.size(); i++) {
        for (int j = 0; j < 4; j++) {
            g_vertices[i].SetCoord(j,g_vertices[i].GetCoord(j)/g_vertices[i].GetCoord(3));
        }
    }
}

void ConvertToWindowCoords() {
    
    printf("size is %d by %d\n",g_width, g_height);
    
    MGLfloat fwidth = ((MGLfloat)g_width)/2.0;
    MGLfloat fheight = ((MGLfloat)g_height)/2.0;
    
    MGLmatrix mult_mat;
    mult_mat.SetData(0,0,fwidth);
    mult_mat.SetData(1,1,fheight);
    mult_mat.SetData(2,2,1.0/2.0);
    
    for (unsigned int i = 0; i < g_vertices.size(); i++) {
        MGLvector4 *new_vec = new MGLvector4();
        new_vec = mult_mat.MultVector(*g_vertices[i].GetPosition());
        
        MGLvector4 add_vec = MGLvector4();
        add_vec.SetData(fwidth, fheight, 1.0/2.0, 0.0);
        
        new_vec->AddVector(add_vec);;
        new_vec->Set(3, 0.0);
        
        g_vertices[i].SetPosition(new_vec);
    } 
}

void makeline( MGLvertex& v0, MGLvertex& v1, Line& l ) {
    l.A = v1.GetCoord(1) - v0.GetCoord(1);
    l.B = v0.GetCoord(0) - v1.GetCoord(0);
    l.C = -(l.A * v0.GetCoord(0) + l.B * v0.GetCoord(1));
}

void bound3( int n, BBox & b ) {
    
    MGLvector4 *first = g_vertices[n].GetPosition();
    MGLvector4 *second = g_vertices[n+1].GetPosition();
    MGLvector4 *third = g_vertices[n+2].GetPosition();
    
    // semi-unreadable: double mins, double maxs
    b.xmin = ceil(min(min( first->Get(0) , second->Get(0) ), third->Get(0) ));
    b.xmax = ceil(max(max( first->Get(0) , second->Get(0) ), third->Get(0) ));
    b.ymin = ceil(min(min( first->Get(1) , second->Get(1) ), third->Get(1) ));
    b.ymax = ceil(max(max( first->Get(1) , second->Get(1) ), third->Get(1) ));
}

int shadow( Line l ) {
    return (l.A>0) || (l.A == 0 && l.B > 0);
}

int inside( float e, Line l) {
    return (e == 0) ? !shadow(l) : (e < 0);
}

void rasterize(int t) {
    BBox bounds;
    bound3( t, bounds );

    printf("bounding box - xmin: %d, xmax, %d, ymin: %d, ymax, %d \n", bounds.xmin, bounds.xmax, bounds.ymin, bounds.ymax);
    
    Line l0, l1, l2;
    MGLfloat e0, e1, e2;    
    
    // switch order of vertices on every odd triangle
    MGLvertex v0, v1, v2, p;
   
    // location
    v0.SetPosition(g_vertices[t].GetPosition());
    v1.SetPosition(g_vertices[t+1].GetPosition());
    v2.SetPosition(g_vertices[t+2].GetPosition());
    // color
    v0.SetColor(g_vertices[t].GetColor());
    v1.SetColor(g_vertices[t+1].GetColor());
    v2.SetColor(g_vertices[t+2].GetColor());
    
    makeline(v0, v1, l2);
    makeline(v1, v2, l0);
    makeline(v2, v0, l1);
    
    cur_color = v0.GetColor();
    printf("cur color is %d\n", cur_color);
           
    for (int y = bounds.ymin; y < bounds.ymax; y++) {
        for (int x = bounds.xmin; x < bounds.xmax; x++) {
            e0 = l0.A * x + l0.B * y + l0.C;
            e1 = l1.A * x + l1.B * y + l1.C;
            e2 = l2.A * x + l2.B * y + l2.C;
            if ( inside(e0,l0) && inside(e1,l1) && inside(e2,l2)) {
                fragment(x,y);
            }
        }
    }
}

void fragment(int x, int y) {
    
    g_data[g_width*y + x] = cur_color;

}

// End Helper Functions
//////////////////////////////


/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
    printf("%s\n", description);
    exit(1);
}


/**
 * read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  the boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data)
{
    g_width = width;
    g_height = height;
    g_data = data;
    
    // if there are actually triangles to draw
    if (g_vertices.size() >= 3) {
        
        ConvertToNDC();
        
        ConvertToWindowCoords();
        
        for (unsigned int i = 0; i < g_vertices.size(); i+=3) {
            
            // send index of first vertex for given triangle
            rasterize(i);
        }
        
        for (unsigned int i = 0; i < g_vertices.size(); i++) {

            printf("Read position of point %d is (%f,%f,%f,%f)\n",
                   i,
                   g_vertices[i].GetCoord(0),
                   g_vertices[i].GetCoord(1),
                   g_vertices[i].GetCoord(2),
                   g_vertices[i].GetCoord(3));
        }
    }
}



/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode)
{
    g_poly_mode = mode;
    
    drawing = true;
    cur_num_verts = 0;
}

/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd()
{
    drawing = false;
    
    printf("numverts = %d\n", cur_num_verts);
    
    // delete incomplete vertices
    for (int i = cur_num_verts; i > 0; i--) {
        g_vertices.pop_back();
    }
    cur_num_verts = 0;

}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z)
{
    if (drawing) {
        MGLvertex new_vert = MGLvertex();
        
        // untransformed point        
        new_vert.SetCoord(0,x);
        new_vert.SetCoord(1,y);
        new_vert.SetCoord(2,z);
        new_vert.SetCoord(3,1.0);
        
        // transformed point
        MGLmatrix *top_model_mat = g_modelview.Top();
        MGLmatrix *top_project_mat = g_projection.Top();
        
        new_vert.SetPosition(top_model_mat->MultVector(*new_vert.GetPosition()));
        new_vert.SetPosition(top_project_mat->MultVector(*new_vert.GetPosition()));

        new_vert.SetColor(cur_color);
        
        if (g_poly_mode == 0) { // triangles
                        
            g_vertices.push_back(new_vert);
            cur_num_verts++;
            cur_num_verts = cur_num_verts % 3;
            
        }
        else { // quads
            printf("cur_num_verts is %d\n", cur_num_verts);
            
            if (cur_num_verts < 3) {
                g_vertices.push_back(new_vert);
                cur_num_verts++;
            }
            else {
                g_vertices.push_back(g_vertices[g_vertices.size()-3]); // first vert
                g_vertices.push_back(g_vertices[g_vertices.size()-2]); // previous vert
                g_vertices.push_back(new_vert);                       // current vert
                cur_num_verts++;
                cur_num_verts = cur_num_verts % 4;
            }    
        }
    }
    else {
        // raise error
        char* err = "Cannot specify vertex outside mglBegin() and mglEnd() block";
        MGL_ERROR(err);
    }
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
    MGLfloat z_val = 0.0;
    mglVertex3(x, y, z_val);
}


/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode)
{
    g_matrix_mode = mode;
    if (g_matrix_mode == 0) { // modelview
        g_cur_stack = &g_modelview;
    }
    else { // projection
        g_cur_stack = &g_projection;
    }
    
    //printf("cur stack size is %lu", g_cur_stack->Size());
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix()
{
    MGLmatrix *new_mat = new MGLmatrix();
    
    new_mat->CopyMatrix(g_cur_stack->Top());
    g_cur_stack->Push(new_mat);

}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
    if (g_cur_stack->Size() > 1) {
        g_cur_stack->Pop();
    }
    else {
        char* err = "Cannot Pop last matrix off stack";
        MGL_ERROR(err);
    }

}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity()
{
    if (!drawing) {
        g_cur_stack->Top()->LoadIdentity();
    }
    else {
        char* err = "Cannot transform matrix within mglBegin() and mglEnd() block";
        MGL_ERROR(err);
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
    if (!drawing) {
        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                g_cur_stack->Top()->SetData(c,r, matrix[4*c+r]);
                
                //g_modelview.top()->SetData(c,r, matrix[4*c+r]);
            }
        }
    }
    else {
        char* err = "Cannot transform matrix within mglBegin() and mglEnd() block";
        MGL_ERROR(err);
    }
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
    if (!drawing) {
        
        MGLmatrix mult_by_this = MGLmatrix();
        
        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                mult_by_this.SetData(c,r, matrix[4*c+r]);
            }
        }
        g_cur_stack->Top()->MultMatrix(mult_by_this);
        
    }
    else {
        char* err = "Cannot transform matrix within mglBegin() and mglEnd() block";
        MGL_ERROR(err);
    }
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z)
{
    MGLmatrix mult_by_this = MGLmatrix();
    
    mult_by_this.SetData(3,0,x);
    mult_by_this.SetData(3,1,y);
    mult_by_this.SetData(3,2,z);
    
    MGLfloat *mult_mat = mult_by_this.GetMatrix();
    mglMultMatrix(mult_mat);
    
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
    MGLmatrix mult_by_this = MGLmatrix();
    
    MGLvector4 vec = MGLvector4();
    vec.SetData(x,y,z,0.0);
    vec.Normalize();
    
    MGLfloat ux = vec.Get(0);
    MGLfloat uy = vec.Get(1);
    MGLfloat uz = vec.Get(2);
    
    mult_by_this.SetData(0,0, cos(angle) + ux * ux * (1 - cos(angle)));
    mult_by_this.SetData(0,1, uy * ux * (1 - cos(angle)) + uz * sin(angle));
    mult_by_this.SetData(0,2, uz * ux * (1 - cos(angle)) + uy * sin(angle));
    
    mult_by_this.SetData(1,0, ux * uy * (1 - cos(angle)) + uz * sin(angle));
    mult_by_this.SetData(1,1, cos(angle) + uy * uy * (1 - cos(angle)));
    mult_by_this.SetData(1,2, uz * uy * (1 - cos(angle)) + ux * sin(angle));
    
    mult_by_this.SetData(2,0, ux * uz * (1 - cos(angle)) + uy * sin(angle));
    mult_by_this.SetData(2,2, uy * uz * (1 - cos(angle)) + ux * sin(angle));
    mult_by_this.SetData(2,1, cos(angle) + uz * uz * (1 - cos(angle)));
    
    MGLfloat *mult_mat = mult_by_this.GetMatrix();
    mglMultMatrix(mult_mat);
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z)
{
    MGLmatrix mult_by_this = MGLmatrix();
    
    mult_by_this.SetData(0,0,x);
    mult_by_this.SetData(1,1,y);
    mult_by_this.SetData(2,2,z);
    
    MGLfloat *mult_mat = mult_by_this.GetMatrix();
    mglMultMatrix(mult_mat);
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
    MGLmatrix mult_by_this = MGLmatrix();
    
    mult_by_this.SetData(0,0,near/right);
    mult_by_this.SetData(1,1,near/top);
        
    mult_by_this.SetData(2,2,-(far + near)/(far - near));
    mult_by_this.SetData(3,2,-(2*far*near)/(far - near));
    
    mult_by_this.SetData(2,3,-1.0);
    mult_by_this.SetData(3,3,0.0);
    
    MGLfloat *mult_mat = mult_by_this.GetMatrix();
    mglMultMatrix(mult_mat);
    
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
    MGLmatrix mult_by_this = MGLmatrix();
    
    mult_by_this.SetData(0,0,2/(right - left));
    mult_by_this.SetData(1,1,2/(top - bottom));
    mult_by_this.SetData(2,2,-2/(far - near));
    
    mult_by_this.SetData(3,0,-(right + left)/(right - left));
    mult_by_this.SetData(3,1,-(top + bottom)/(top - bottom));
    mult_by_this.SetData(3,2,-(far + near)/(far - near));
    mult_by_this.SetData(3,3,1.0);
        
    MGLfloat *mult_mat = mult_by_this.GetMatrix();
    mglMultMatrix(mult_mat);

}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLbyte red,
              MGLbyte green,
              MGLbyte blue) {
    MGL_SET_RED(cur_color, red);
    MGL_SET_GREEN(cur_color, green);
    MGL_SET_BLUE(cur_color, blue);
    printf("cur color is %d\n", cur_color);
}
