mglMatrixMode(MGL_PROJECTION);
mglLoadIdentity();
mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
mglMatrixMode(MGL_MODELVIEW);
mglLoadIdentity();

mglTranslate(0.1667, 0.5, 0.0);
mglBegin(MGL_TRIANGLES);
mglColor(255, 0, 0);
for( int i = 0; i < 3; i++ )
{
  mglVertex2(0 + 0.33*i, 0.25);
  mglVertex2(-0.1667 + 0.33*i, -0.25);
  if( i == 0 ) mglColor(0, 255, 0);
  else if( i == 1) mglColor(0, 0, 255);
  mglVertex2(0.1667 + 0.33*i, -0.25);
}
mglEnd();
