mglMatrixMode(MGL_PROJECTION);
mglLoadIdentity();
mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
mglMatrixMode(MGL_MODELVIEW);
mglLoadIdentity();

mglPushMatrix();

mglTranslate(0.25, 0.5, 0.0);
mglRotate(-45, 0.0, 1.0, 0.0);
mglScale(0.25, 0.25, 1.0);

mglColor(0, 0, 255);
mglBegin(MGL_QUADS);
mglVertex2(-1.0, -1.0);
mglVertex2(1.0, -1.0);
mglVertex2(1.0, 1.0);
mglVertex2(-1.0, 1.0);
mglEnd();

mglPopMatrix();

mglColor(255, 0, 0);
mglBegin(MGL_TRIANGLES);
mglVertex3(0.5, 0.25, 0.5);
mglVertex3(0.75, 0.25, -0.5);
mglVertex3(0.75, 0.75, -0.5);
mglEnd();
