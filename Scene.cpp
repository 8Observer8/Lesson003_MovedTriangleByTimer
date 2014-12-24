#include <QMatrix4x4>
#include "Scene.h"

Scene::Scene( QWidget *parent ) :
    QOpenGLWidget( parent )
{
    this->setFocusPolicy( Qt::StrongFocus );

    connect( &m_timer, SIGNAL( timeout() ),
             this, SLOT( slotMove() ) );

    m_timer.start( 50 );
}

Scene::~Scene()
{
    delete m_triangle;
}

void Scene::initializeGL()
{
    glClearColor( 0.1f, 0.1f, 0.2f, 1.0f );

    QOpenGLShader vShader( QOpenGLShader::Vertex );
    vShader.compileSourceFile( ":/Shaders/vShader.glsl" );

    QOpenGLShader fShader( QOpenGLShader::Fragment );
    fShader.compileSourceFile( ":/Shaders/fShader.glsl" );

    m_program.addShader( &vShader );
    m_program.addShader( &fShader );
    if ( !m_program.link() )
    {
        qWarning( "Error: unable to link a shader program." );
        return;
    }

    m_vertexAttr = m_program.attributeLocation( "vertexAttr" );
    m_colorAttr = m_program.attributeLocation( "colorAttr" );
    m_matrixUniform = m_program.uniformLocation( "matrix" );

    m_triangle = new Triangle( &m_program, m_vertexAttr, m_colorAttr );
}

void Scene::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );

    if ( !m_program.bind() )
        return;

    QMatrix4x4 matrix;
    matrix.ortho( -2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f );
    matrix.translate( 0.0f, 0.0f, -1.0f );
    m_program.setUniformValue( m_matrixUniform, matrix );

    m_triangle->draw();

    m_program.release();
}

void Scene::resizeGL( int w, int h )
{
    glViewport( 0, 0, w, h );
}

void Scene::slotMove()
{
   const float step = 0.1f;
   m_triangle->setX0( m_triangle->x0() + step );
   m_triangle->setY0( m_triangle->y0() + step );

   if ( ( m_triangle->x0() >= 2.0f ) ||
        ( m_triangle->y0() >= height() / 2.0f ) )
   {
       m_triangle->setY0( -0.5f );
       m_triangle->setX0( -0.5f );
   }

   update();
}
