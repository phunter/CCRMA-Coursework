mglMatrixMode(MGL_PROJECTION);
mglLoadIdentity();
mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
mglMatrixMode(MGL_MODELVIEW);
mglLoadIdentity();

mglColor(255, 255, 255);
mglBegin(MGL_TRIANGLES);
mglVertex2(0.25, 0.25);
mglVertex2(0.75, 0.25);
mglVertex2(0.75, 0.75);
mglEnd();
