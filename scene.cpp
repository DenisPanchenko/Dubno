#include <QtGui>
#include "scene.h"

Scene::Scene(FilterFactory* ff, QWidget *parent): QGLWidget(parent), factory(ff)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    currentFilter = new QGLShaderProgram(this->context());
    image = new QImage();
    curFilerNum=0;
}

void Scene::initializeGL()
{
    qglClearColor(Qt::white);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
}

void Scene::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -1.0, +1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void Scene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void Scene::mousePressEvent(QMouseEvent *event)
{

}

void Scene::mouseMoveEvent(QMouseEvent *event)
{

}

void Scene::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Scene::draw()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D,textname);

#ifdef Q_WS_X11
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, -1.0f,  0.0f);
        glTexCoord2f(1.0, 1.0);
        glVertex3f( 1.0f, -1.0f,  0.0f);
        glTexCoord2f(1.0, 0.0);
        glVertex3f( 1.0f,  1.0f,  0.0f);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-1.0f,  1.0f,  0.0f);
        glTexCoord2f(0.0, 1.0);
   glEnd();
#endif

#ifdef Q_WS_WIN
   glBegin(GL_QUADS);
       glVertex3f(-1.0f, -1.0f,  0.0f);
       glTexCoord2f(1.0, 0.0);
       glVertex3f( 1.0f, -1.0f,  0.0f);
       glTexCoord2f(1.0, 1.0);
       glVertex3f( 1.0f,  1.0f,  0.0f);
       glTexCoord2f(0.0, 1.0);
       glVertex3f(-1.0f,  1.0f,  0.0f);
       glTexCoord2f(0.0, 0.0);
  glEnd();
#endif

}

void Scene::openImage(QString imageName){

    if(!imageName.isEmpty()){
        textname = bindTexture(imageName,GL_TEXTURE_2D,GL_RGBA);
        delete image;
        image = new QImage(imageName);
        width = image->width();
        height = image->height();
        genTexCoordOffsets(width,height);
        emit resizeMainWindow(imageName);
        changeFilter(curFilerNum);
    }
}

void Scene::changeFilter(int pos){
    curFilerNum=pos;
    if(image->isNull()) return;
    Filter* filterData = factory->getFilter(pos);

    delete currentFilter;
    currentFilter = new QGLShaderProgram(this->context());

    currentFilter->addShaderFromSourceFile(QGLShader::Vertex,filterData->getVertexPath());
    currentFilter->addShaderFromSourceFile(QGLShader::Fragment,filterData->getFragmentPath());
    currentFilter->link();
    currentFilter->bind();

    QVector<QPair<QString,double> > params = filterData->getParam();
    changeParameters(params);
}

void Scene::changeParameters(QVector<QPair<QString, double> > params){
    if(image->isNull()) return;
    QPair<QString,double> pair;

    for(int i=0;i<params.size();i++){
        pair = params[i];
        currentFilter->setUniformValue(pair.first.toStdString().c_str(),(float)pair.second);
    }

    currentFilter->setUniformValue("width",(float)width);
    currentFilter->setUniformValue("height",(float)height);
    currentFilter->setUniformValueArray("tcOffset",texCoordOffsets,25,2);

    updateGL();
}

void Scene::genTexCoordOffsets(GLuint width, GLuint height, GLfloat step){
        float xInc = step / (GLfloat)(width);
        float yInc = step / (GLfloat)(height);

        for (int i = 0; i < 5; i++)
        {
                for (int j = 0; j < 5; j++)
                {
                        texCoordOffsets[(((i*5)+j)*2)+0] = (-2.0f * xInc) + ((GLfloat)i * xInc);
                        texCoordOffsets[(((i*5)+j)*2)+1] = (-2.0f * yInc) + ((GLfloat)j * yInc);
                }
        }
}
