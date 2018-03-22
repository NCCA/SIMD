#include "NGLScene.h"
#include <QGuiApplication>
#include <QMouseEvent>

#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Texture.h>


NGLScene::NGLScene(size_t _numParticles)
{
  setTitle( "SIMD Particles" );
  m_fpsTimer =startTimer(0);
  m_fps=0;
  m_frames=0;
  m_numParticles=_numParticles;
  m_timer.start();

}


NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL( int _w, int _h )
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

const char * ParticleShader="ParticleShader";

void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::instance();
  glClearColor( 0.4f, 0.4f, 0.4f, 1.0f ); // Grey Background
  // enable depth testing for drawing
  glEnable( GL_DEPTH_TEST );
// enable multisampling for smoother drawing
#ifndef USINGIOS_
  glEnable( GL_MULTISAMPLE );
#endif
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();

  m_particle.reset(new ParticleSystem(/*16000*64*/m_numParticles,{0,0,0}));
  m_particleUpdateTimer=startTimer(1);
  ngl::VAOPrimitives::instance()->createLineGrid("grid",20,20,100);
  m_text.reset(new ngl::Text(QFont("Arial",14)));
  m_text->setScreenSize(width(),height());
  m_text->setColour(1,1,0);
  shader->loadShader(ParticleShader,"shaders/ParticleVertex.glsl",
                                    "shaders/ParticleFragment.glsl");

  shader->use(ParticleShader);
  shader->setUniform("Colour",1.0f,1.0f,1.0f,1.f);
  ngl::Texture t("textures/texture.png");
  m_texID=t.setTextureGL();


}


void NGLScene::loadMatricesToShader(const char *_shader)
{
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();
  shader->use(_shader);

  ngl::Mat4 MVP;
  const ngl::Mat4 projection = ngl::perspective(45.0f,float(m_win.width)/m_win.height,0.01f,500.0f);
  const ngl::Mat4 view=ngl::lookAt({0,2,2},{0,0,0},{0,1,0});
  shader->setUniform("MVP",projection*view*m_mouseGlobalTX);

}

void NGLScene::paintGL()
{
  glViewport( 0, 0, m_win.width, m_win.height );

  // clear the screen and depth buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX( m_win.spinXFace );
  rotY.rotateY( m_win.spinYFace );
  // multiply the rotations
  m_mouseGlobalTX = rotX * rotY;
  // add the translations
  m_mouseGlobalTX.m_m[ 3 ][ 0 ] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[ 3 ][ 1 ] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[ 3 ][ 2 ] = m_modelPos.m_z;

  // draw
  loadMatricesToShader(ParticleShader);
  glBindTexture(GL_TEXTURE_2D,m_texID);

  glPointSize(m_pointSize);
  m_particle->render();

  loadMatricesToShader(ngl::nglColourShader);
  ngl::VAOPrimitives::instance()->draw("grid");
  QString text=QString("Total Particles %1 %2 fps").arg(m_numParticles).arg(m_fps);
  m_text->renderText(10,40,text);
  ++m_frames;


}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent( QKeyEvent* _event )
{
  // that method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch ( _event->key() )
  {
    // escape key to quit
    case Qt::Key_Escape:
      QGuiApplication::exit( EXIT_SUCCESS );
      break;
// turn on wirframe rendering
#ifndef USINGIOS_
    case Qt::Key_W:
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      break;
    // turn off wire frame
    case Qt::Key_S:
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      break;
#endif
    // show full screen
    case Qt::Key_F:
      showFullScreen();
      break;
    // show windowed
    case Qt::Key_N:
      showNormal();
      break;
    case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());
    break;
    case Qt::Key_C : m_circle^=true; break;
    case Qt::Key_A : m_animate^=true; break;
    case Qt::Key_1 : m_pointSize-=1; break;
    case Qt::Key_2 : m_pointSize+=1; break;
    case Qt::Key_3 : m_particle->updateEnergy(-0.1f); break;
    case Qt::Key_4 : m_particle->updateEnergy(0.1f); break;
    case Qt::Key_0 : m_fma^=true; break;
    case Qt::Key_Left : m_particle->updatePosition(-0.1f,0,0); break;
    case Qt::Key_Right : m_particle->updatePosition(0.1f,0,0); break;
    case Qt::Key_Up : m_particle->updatePosition(0,0.1f,0); break;
    case Qt::Key_Down : m_particle->updatePosition(0,-0.1f,0); break;
    case Qt::Key_I : m_particle->updatePosition(0,0.0,0.1f); break;
    case Qt::Key_O : m_particle->updatePosition(0,0,-0.1f); break;

    default:
      break;
  }
  update();
}


void NGLScene::timerEvent(QTimerEvent *_event)
{
  if(_event->timerId() ==   m_particleUpdateTimer)
  {
    if(m_circle)
    {
      float rad=0.1f;
      static float time=0.0f;
      float pointOnCircleX= cosf(ngl::radians(time))*rad;
      float pointOnCircleZ= sinf(ngl::radians(time))*rad;
      m_particle->updatePosition(pointOnCircleX,0.0f,pointOnCircleZ);
      time+=5.0f;
    }
    if(m_animate)
    {
      if(!m_fma)
        m_particle->update(0.01f);
      else
        m_particle->updateFMA(0.01f);
    }
  }
  if(_event->timerId() == m_fpsTimer)
    {
      if( m_timer.elapsed() > 1000.0)
      {
        m_fps=m_frames;
        m_frames=0;
        m_timer.restart();
      }
     }

  update();
}