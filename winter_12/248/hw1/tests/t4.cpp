mglMatrixMode(MGL_PROJECTION);
mglLoadIdentity();
mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
mglMatrixMode(MGL_MODELVIEW);
mglLoadIdentity();

mglColor(0, 0, 255);
mglBegin(MGL_QUADS);
mglVertex2(0.4, 0.2);
mglVertex2(2.0, 0.2);
mglVertex2(2.0, 0.8);
mglVertex2(0.4, 0.8);
mglEnd();

mglColor(255, 0, 0);
mglBegin(MGL_TRIANGLES);
mglVertex3(0.2, 0.2, -0.5);
mglVertex3(0.8, 0.5, 0.5);
mglVertex3(0.2, 0.8, -0.5);
mglEnd();
