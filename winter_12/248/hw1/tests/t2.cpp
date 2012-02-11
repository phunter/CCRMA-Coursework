mglMatrixMode(MGL_PROJECTION);
mglLoadIdentity();
mglFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
mglMatrixMode(MGL_MODELVIEW);
mglLoadIdentity();

mglColor(255, 255, 255);
mglBegin(MGL_QUADS);
mglVertex3(-1.0, -1.0, -5.0);
mglVertex3(1.0, -1.0, -2.0);
mglVertex3(1.0, 1.0, -2.0);
mglVertex3(-1.0, 1.0, -5.0);
mglEnd();
