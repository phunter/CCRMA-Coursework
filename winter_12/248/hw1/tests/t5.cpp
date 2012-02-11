static MGLfloat trans[6][3] = {
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0},
    {1.0, 0.0, 0.0},
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, -1.0, 0.0}
};
static MGLfloat rots[6][4] = {
    {0.0, 0.0, 1.0, 0.0},
    {180.0, 0.0, 1.0, 0.0},
    {90.0, 0.0, 1.0, 0.0},
    {-90.0, 0.0, 1.0, 0.0},
    {-90.0, 1.0, 0.0, 0.0},
    {90.0, 1.0, 0.0, 0.0}
};
static MGLbyte cols[6][3] = {
    {255, 0, 0},
    {0, 255, 255},
    {0, 255, 0},
    {255, 0, 255},
    {0, 0, 255},
    {255, 255, 0}
};

mglMatrixMode(MGL_PROJECTION);
mglLoadIdentity();
mglFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
mglMatrixMode(MGL_MODELVIEW);
mglLoadIdentity();

mglTranslate(0.0, 0.0, -5.0);
mglRotate(-60, -1.0, 1.0, -1.0);

for (int i = 0; i < 6; ++i) {
    MGLfloat* tran = trans[i];
    MGLfloat* rot = rots[i];
    MGLbyte* col = cols[i];

    mglPushMatrix();

    mglTranslate(tran[0], tran[1], tran[2]);
    mglRotate(rot[0], rot[1], rot[2], rot[3]);

    mglColor(col[0], col[1], col[2]);
    mglBegin(MGL_QUADS);
    mglVertex2(-1.0, -1.0);
    mglVertex2(1.0, -1.0);
    mglVertex2(1.0, 1.0);
    mglVertex2(-1.0, 1.0);
    mglEnd();

    mglPopMatrix();
}
