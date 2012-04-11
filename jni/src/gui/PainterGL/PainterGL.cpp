/*
Copyright (C) 2005 Matthias Braun <matze@braunis.de>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <config.h>

#include "PainterGL.hpp"

#include <SDL.h>
#include <SDL_opengles.h>
#include <iostream>
#include <typeinfo>

#include "TextureGL.hpp"

#include <android/log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "JNI_DEBUGGING", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   "JNI_DEBUGGING", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,   "JNI_DEBUGGING", __VA_ARGS__)

PainterGL::PainterGL()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

PainterGL::~PainterGL()
{
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void
PainterGL::drawTextureRect(const Texture* texture, const Rect2D& rect)
{

    const TextureGL* textureGL = static_cast<const TextureGL*> (texture);
    const Rect2D& r = textureGL->rect;

    glColor4ub( 0xff, 0xff, 0xff, 0xff );
  /*
    glBindTexture(GL_TEXTURE_2D, textureGL->handle);
    glBegin(GL_QUADS);
    glTexCoord2f(r.p1.x, r.p1.y);
    glVertex3f(rect.p1.x, rect.p1.y, 0);
    glTexCoord2f(r.p1.x, r.p2.y);
    glVertex3f(rect.p1.x, rect.p2.y, 0);
    glTexCoord2f(r.p2.x, r.p2.y);
    glVertex3f(rect.p2.x, rect.p2.y, 0);
    glTexCoord2f(r.p2.x, r.p1.y);
    glVertex3f(rect.p2.x, rect.p1.y, 0);
    glEnd();
    */
    //glScalef(0.001,0.001,0.001);
    //LOGD("[Lincity]PainterGL::drawTextureRect() %d %f %f %f %f | %f %f %f %f\n",textureGL->handle,r.p1.x,r.p1.y,r.p2.x,r.p2.y,
    //rect.p1.x,rect.p1.y,rect.p2.x,rect.p2.y);
/*
    if (int a = glGetError() != 0)
    {
       LOGD("[Lincity]PainterGL::drawTextureRect() - Before draw ERROR OCCURED : %d\n",a);
    }
    */
    //glDisable(GL_TEXTURE_2D);
    //glClientActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, textureGL->handle);

    GLfloat texcoords[] = {r.p1.x,r.p1.y,r.p1.x,r.p2.y, r.p2.x,r.p2.y,r.p2.x,r.p1.y};
    //GLfloat texcoords[] = {r.p1.x,r.p1.y,r.p1.x,0., 0.,0.,0.,r.p1.y};

    GLfloat vertices[] = {rect.p1.x,rect.p1.y,0., rect.p1.x,rect.p2.y,0., rect.p2.x,rect.p2.y,0.,rect.p2.x,rect.p1.y,0.};
    //GLfloat vertices[] = {rect.p1.x,rect.p1.y,0., rect.p1.x,rect.p2.y/100000.,0., rect.p2.x/100000.,rect.p2.y/100000.,0.,rect.p2.x/100000.,rect.p1.y,0.};
//GLfloat vertices[] = {rect.p1.x,rect.p1.y,0., rect.p1.x,rect.p1.y+0.0001,0., rect.p1.x+0.0001,rect.p1.y+0.0001,0.,rect.p1.x+0.0001,rect.p1.y,0.};
    //GLfloat colors[] = {1.,0.,0.,1., 0.,1.,0.,1.,0.,0.,1.,1.};

    //glEnableClientState (GL_COLOR_ARRAY);
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);

    glTexCoordPointer (2,GL_FLOAT,0,texcoords);
    //glColorPointer (4,GL_FLOAT,0,colors);
    glVertexPointer (3,GL_FLOAT,0,vertices);
    glDrawArrays (GL_TRIANGLE_FAN,0,4);

    glDisableClientState (GL_TEXTURE_COORD_ARRAY);
    glDisableClientState (GL_VERTEX_ARRAY);
    //glDisableClientState (GL_COLOR_ARRAY);
    /*
    if (int a = glGetError() != 0)
    {
       LOGD("[Lincity]PainterGL::drawTextureRect() ERROR OCCURED : %d\n",a);
    }
    */
}
void
PainterGL::drawTexture(const Texture* texture, const Vector2& pos)
{
    //LOGD("[Lincity]PainterGL::drawTexture()\n");
    Rect2D rect(pos, pos + Vector2(texture->getWidth(), texture->getHeight()));
    drawTextureRect(texture, rect);
}

void
PainterGL::drawStretchTexture(Texture* texture, const Rect2D& rect)
{
    //LOGD("[Lincity]PainterGL::drawStretchTexture()\n");
    assert(typeid(*texture) == typeid(TextureGL));

    if(texture == 0) {
        std::cerr << "Trying to render 0 texture.";
#ifdef DEBUG
        assert(false);
#endif
        return;
    }

    drawTextureRect(texture, rect);
}

void
PainterGL::drawLine( const Vector2 pointA, const Vector2 pointB )
{
    GLfloat vertices[] = {pointA.x,pointA.y,0., pointB.x,pointB.y,0.};
    //LOGD("[Lincity]PainterGL::drawLine()\n");
    glColor4ub(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glDisable(GL_TEXTURE_2D);
  /*
    glBegin(GL_LINES);
    glVertex2f(pointA.x, pointA.y);
    glVertex2f(pointB.x, pointB.y);
    glEnd();
    */
    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer (3,GL_FLOAT,0,vertices);
    glDrawArrays (GL_LINES,0,2);
    glDisableClientState (GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
}

void
PainterGL::fillRectangle(const Rect2D& rect)
{
    //LOGD("[Lincity]PainterGL::fillRectangle()\n");
    glColor4ub(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    glDisable(GL_TEXTURE_2D);

    //printf("Drawrect: %f %f %f %f.\n", rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y);
      /*
    glBegin(GL_POLYGON);
    glVertex2f(rect.p1.x, rect.p1.y);
    glVertex2f(rect.p1.x, rect.p2.y);
    glVertex2f(rect.p2.x, rect.p2.y);
    glVertex2f(rect.p2.x, rect.p1.y);
    glEnd();
        */

    GLfloat vertices[] = {rect.p1.x,rect.p1.y,0., rect.p1.x,rect.p2.y,0., rect.p2.x,rect.p2.y,0.,rect.p2.x,rect.p1.y,0.};

    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer (3,GL_FLOAT,0,vertices);
    glDrawArrays (GL_TRIANGLE_FAN,0,4);
    glDisableClientState (GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
}

void
PainterGL::drawRectangle(const Rect2D& rect)
{
    //LOGD("[Lincity]PainterGL::drawRectangle()\n");
    GLfloat vertices[] = {rect.p1.x,rect.p1.y,0., rect.p1.x,rect.p2.y,0., rect.p2.x,rect.p2.y,0.,rect.p2.x,rect.p1.y,0.};
    glColor4ub(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glDisable(GL_TEXTURE_2D);

          /*
    glBegin(GL_LINE_LOOP);
    glVertex2f(rect.p1.x, rect.p1.y);
    glVertex2f(rect.p1.x, rect.p2.y);
    glVertex2f(rect.p2.x, rect.p2.y);
    glVertex2f(rect.p2.x, rect.p1.y);
    glEnd();
            */
    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer (3,GL_FLOAT,0,vertices);
    glDrawArrays (GL_LINE_LOOP,0,4);
    glDisableClientState (GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
}

void
PainterGL::fillPolygon(int numberPoints, const Vector2* points)
{
    //LOGD("[Lincity]PainterGL::fillPolygon() %d\n",numberPoints);
    //LOGD("[Lincity]PainterGL::fillPolygon() %f %f\n",points[0].x,points[0].y);
    //LOGD("[Lincity]PainterGL::fillPolygon() %f %f\n",points[1].x,points[1].y);
    //LOGD("[Lincity]PainterGL::fillPolygon() %f %f\n",points[2].x,points[2].y);
    glColor4ub(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    glDisable(GL_TEXTURE_2D);
              /*
    glBegin(GL_POLYGON);
    for( int i = 0; i < numberPoints; i++ )
        glVertex2f(points[i].x, points[i].y);
    glEnd();
                */

    GLfloat vertices[] = {
      points[2].x,points[2].y,0., 
      points[1].x,points[1].y,0., 
      points[0].x,points[0].y,0.,
      points[3].x,points[3].y,0.,
      points[2].x,points[2].y,0.,
      points[0].x,points[0].y,0.};

    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer (3,GL_FLOAT,0,vertices);
    glDrawArrays (GL_TRIANGLES,0,6);
    glDisableClientState (GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
}

void
PainterGL::drawPolygon(int numberPoints, const Vector2* points)
{
    //LOGD("[Lincity]PainterGL::drawPolygon()\n");
    glColor4ub(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glDisable(GL_TEXTURE_2D);
                  /*
    glBegin(GL_LINE_LOOP);
    for( int i = 0; i < numberPoints; i++ )
        glVertex2f(points[i].x, points[i].y);
    glEnd();       */

    GLfloat vertices[] = {points[0].x,points[0].y,0., points[1].x,points[1].y,0.
    , points[2].x,points[2].y,0.,points[3].x,points[3].y,0.};

    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer (3,GL_FLOAT,0,vertices);
    glDrawArrays (GL_LINE_LOOP,0,4);
    glDisableClientState (GL_VERTEX_ARRAY);

    glEnable(GL_TEXTURE_2D);
}

void
PainterGL::setFillColor(Color color)
{
    fillColor = color;
}

void
PainterGL::setLineColor(Color color)
{
    lineColor = color;
}

void
PainterGL::translate(const Vector2& vec)
{
    glTranslatef(vec.x, vec.y, 0);
}

void
PainterGL::pushTransform()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void
PainterGL::popTransform()
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void
PainterGL::setClipRectangle(const Rect2D& rect)
{
    GLfloat matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    int screenHeight = SDL_GetVideoSurface()->h;
    glViewport((GLint) (rect.p1.x + matrix[12]),
               (GLint) (screenHeight - rect.getHeight() - (rect.p1.y + matrix[13])),
               (GLsizei) rect.getWidth(),
               (GLsizei) rect.getHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(rect.p1.x + matrix[12], rect.p1.x + matrix[12] + rect.getWidth(),
            rect.p1.y + matrix[13] + rect.getHeight(),
            rect.p1.y + matrix[13], -1, 1);
}

void
PainterGL::clearClipRectangle()
{
    int width = SDL_GetVideoSurface()->w;
    int height = SDL_GetVideoSurface()->h;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, -1, 1);
}

Painter*
PainterGL::createTexturePainter(Texture* texture)
{
    (void) texture;
    // TODO
    return 0;
}

