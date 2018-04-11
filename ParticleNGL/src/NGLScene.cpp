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
#include <ngl/Random.h>
#include "ParticleSystem.h"
#include "ParticleSystemAOS.h"
#include "ParticleSystemNormal.h"
#include "ParticleSystemAVX2.h"
#include "ParticleSystemSSERand.h"
#include <chrono>
#include "imgui.h"
#include "QtImGui.h"

NGLScene::NGLScene(size_t _numParticles)
{
  setTitle( "SIMD Particles" );
  m_fpsTimer =startTimer(0);
  m_fps=0;
  m_frames=0;
  m_numParticles=_numParticles;
  m_timer.start();
  m_updateTimes.resize(200);
  m_renderTimes.resize(200);
  connect(this,SIGNAL(changeSystem(int)),this,SLOT(updateSystem(int)));

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

  m_particle.reset(new ParticleSystemSSERAND(m_numParticles,{0,0,0}));
  m_particleUpdateTimer=startTimer(0);
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

  QtImGui::initialize(this);
}


void NGLScene::loadMatricesToShader(const char *_shader)
{
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();
  shader->use(_shader);

  ngl::Mat4 MVP;
  const ngl::Mat4 projection = ngl::perspective(45.0f,float(m_win.width)/m_win.height,0.01f,500.0f);
  const ngl::Mat4 view=ngl::lookAt({0,2,2},{0,0,0},{0,1,0});
  // top
  //  const ngl::Mat4 view=ngl::lookAt({0,4,0},{0,0,0},{0,0,1});

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
    auto begin = std::chrono::steady_clock::now();
    m_particle->render();
    auto end = std::chrono::steady_clock::now();
    m_renderTime=std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
    loadMatricesToShader(ngl::nglColourShader);
    ngl::VAOPrimitives::instance()->draw("grid");
/*  const QString c_systemType[]={
    "Particle System SOA -> SSE",
    "Particle System AOS",
    "Particle System SOA (No SSE)",
    "Particle System AVX2",
    "Particle System SSE-Random"
  };
  QString text=c_systemType[m_systemType];
  m_text->renderText(10,20,text);
  text=QString("Total Particles %1 %2 fps").arg(m_numParticles).arg(m_fps);
  m_text->renderText(10,40,text);
  text=QString("update time %1ms").arg(m_updateTime.count());
  m_text->renderText(10,60,text);
  text=QString("render time %1ms").arg(m_renderTime.count());
  m_text->renderText(10,80,text);
  */
  m_updateTimes.erase(std::begin(m_updateTimes));
  m_updateTimes.push_back(m_updateTime.count());
  m_renderTimes.erase(std::begin(m_renderTimes));
  m_renderTimes.push_back(m_renderTime.count());
  ++m_frames;
  if(m_showUI==true)
    drawUI();

}

void NGLScene::drawUI()
{
  QtImGui::newFrame();

  ImGui::Begin("Particle System");

  const char* items[]={
  "Particle System SOA -> SSE",
  "Particle System AOS",
  "Particle System SOA (No SSE)",
  "Particle System AVX2",
  "Particle System SSE-Random"
  };
  static int whichSystem=4;
  static int lastSystem=4;
  ImGui::Combo("System Type", &whichSystem, items,5);   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.
  if(whichSystem !=lastSystem)
  {
    lastSystem=whichSystem;
    emit updateSystem(lastSystem);
  }

  ImGui::Separator();

  ImGui::Checkbox("Animate", &m_animate);
  ImGui::SameLine();
  ImGui::Checkbox("Circle", &m_circle);
  ImGui::Separator();
  ImGui::SliderFloat("Energy", &m_energyRange,0.1f,40.0f);
  m_particle->setEnergy(m_energyRange);
  ImGui::SliderInt("Point Size",&m_pointSize,0,20);
  ImGui::Separator();
  float ave=float(std::accumulate(std::begin(m_updateTimes),std::end(m_updateTimes),0)) / m_updateTimes.size();
  char aveText[10];
  sprintf(aveText,"ave %0.4f",ave);
  ImGui::PlotLines("Update", &m_updateTimes[0], m_updateTimes.size(), 0, aveText, -1.0f, 1.0f, ImVec2(0,80));
  ave=float(std::accumulate(std::begin(m_renderTimes),std::end(m_renderTimes),0)) / m_renderTimes.size();
  sprintf(aveText,"ave %0.4f",ave);
  ImGui::PlotLines("Render", &m_renderTimes[0], m_renderTimes.size(), 0, aveText, -1.0f, 1.0f, ImVec2(0,80));


  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  ImGui::Render();


}

void NGLScene::updateSystem(int _type)
{
  m_particle.reset(new ParticleSystem(m_numParticles,{0,0,0}));
/*  switch (_type)
  {

  case 0 : m_particle.reset(new ParticleSystem(m_numParticles,{0,0,0}));  break;
  case 1 : m_particle.reset(new ParticleSystemAOS(m_numParticles,{0,0,0})); break;
  case 2:  m_particle.reset(new ParticleSystemNormal(m_numParticles,{0,0,0})); break;
  case 3:  m_particle.reset(new ParticleSystemAVX2(m_numParticles,{0,0,0})); break;
  case 4 : m_particle.reset(new ParticleSystemSSERAND(m_numParticles,{0,0,0})); break;
  }
*/
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
    case Qt::Key_3 : m_particle->updateEnergy(-0.1f);  break;
    case Qt::Key_4 : m_particle->updateEnergy(0.1f);  break;
    case Qt::Key_5 :   m_particle.reset(new ParticleSystem(m_numParticles,{0,0,0})); m_systemType=0; break;
    case Qt::Key_6 :   m_particle.reset(new ParticleSystemAOS(m_numParticles,{0,0,0})); m_systemType=1;break;
    case Qt::Key_7 :   m_particle.reset(new ParticleSystemNormal(m_numParticles,{0,0,0})); m_systemType=2;break;
    case Qt::Key_8 :   m_particle.reset(new ParticleSystemAVX2(m_numParticles,{0,0,0})); m_systemType=3;break;
    case Qt::Key_9 :   m_particle.reset(new ParticleSystemSSERAND(m_numParticles,{0,0,0})); m_systemType=4;break;

    case Qt::Key_0 : m_fma^=true; break;
    case Qt::Key_Left : m_particle->updatePosition(-0.1f,0,0); break;
    case Qt::Key_Right : m_particle->updatePosition(0.1f,0,0); break;
    case Qt::Key_Up : m_particle->updatePosition(0,0.1f,0); break;
    case Qt::Key_Down : m_particle->updatePosition(0,-0.1f,0); break;
    case Qt::Key_I : m_particle->updatePosition(0,0.0,0.1f); break;
    case Qt::Key_O : m_particle->updatePosition(0,0,-0.1f); break;
    case Qt::Key_U : m_showUI^=true; break;
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
      auto begin = std::chrono::steady_clock::now();
      m_particle->update(0.01f);
      auto end = std::chrono::steady_clock::now();
      m_updateTime=std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
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
