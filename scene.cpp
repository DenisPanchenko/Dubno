#include <QtGui>
#include "scene.h"

Scene::Scene(QWidget *parent): QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    currentFilter = new QGLShaderProgram(this->context());
    factory = new FilterFactory(QDir::currentPath() + QDir::separator() + "config.txt");
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

void Scene::openImage(){
    imageName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                             "/home",
                                             tr("Images (*.png *.jpeg *.jpg *.gif)"));
    textname = bindTexture(imageName,GL_TEXTURE_2D,GL_RGBA);
    //updateGL();
    changeFilter("nothingFilter.txt");
}

void Scene::changeFilter(int pos){

    Filter* filterData = factory->getFilter(pos);

    delete currentFilter;
    currentFilter = new QGLShaderProgram(this->context());

    currentFilter->addShaderFromSourceFile(QGLShader::Vertex,filterData->getVertexPath());
    currentFilter->addShaderFromSourceFile(QGLShader::Fragment,filterData->getFragmentPath());
    currentFilter->link();
    currentFilter->bind();
    updateGL();
}

void Scene::changeFilter(QString fragmentShaderName){
    delete currentFilter;
    currentFilter = new QGLShaderProgram(this->context());

    currentFilter->addShaderFromSourceFile(QGLShader::Vertex,QDir::currentPath() + QDir::separator() + "vertex_shader.txt");
    currentFilter->addShaderFromSourceFile(QGLShader::Fragment,QDir::currentPath() + QDir::separator() + fragmentShaderName);
    currentFilter->link();
    currentFilter->bind();
    updateGL();

}
