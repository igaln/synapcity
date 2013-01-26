#include "ParticleSystem.h"

#include "cinder/app/AppBasic.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "clusterSystem.h"
#include "cluster.h"

using namespace ci;
using namespace ci::app;

class synapcityApp : public AppBasic {
 public:	
	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
	
	void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event);
	void mouseUp( MouseEvent event);
	void mouseDrag( MouseEvent event );
	
	Vec2i mouse;
	float timeStep;
	bool isMousePressed, slowMotion;
	ParticleSystem* particleSystem;
	clusterSystem* clusterSys;
	gl::Texture logo;
	gl::Texture icons;
	
};

void synapcityApp::prepareSettings( Settings *settings )
{
	//settings->setFullScreen( true );
	settings->setWindowSize(1024, 768);
	//settings->setFrameRate(160);
	
}

void synapcityApp::setup(){
	
	int binPower = 6;
	slowMotion = isMousePressed = false;
	particleSystem = new ParticleSystem();
	particleSystem->setup(getWindowWidth(), getWindowHeight(), binPower);
	clusterSys = new clusterSystem(particleSystem,10, 800);
	
	logo = gl::Texture(loadImage(ci::app::App::get()->getResourcePath("logo.png")));
	icons = gl::Texture(loadImage(ci::app::App::get()->getResourcePath("raw_wave_title.png")));

}

void synapcityApp::update(){
	
	clusterSys->update();
	
}

void synapcityApp::draw()
{
	
	gl::clear();
	
	
	gl::enableAlphaBlending();
	
	//gl::drawString( toString((int) getAverageFps()) + " fps", Vec2f(32.0f, 52.0f));
	glPushMatrix();
	clusterSys->draw();
	glPopMatrix();

	gl::disableAlphaBlending();
	//gl::color( Color( 0, 0, 0 ) );
	//logo.enableAndBind();
	//gl::clear();
	//logo.disable();
	gl::enableAlphaBlending();
	glPushMatrix();
	glEnable( GL_BLEND );
	glColor4f( 1.0f, 1.0f, 1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		gl::draw( icons, Vec2f(getWindowWidth()- 640,12) );
		gl::draw( logo, Vec2f(20,20) );
		glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	
	

}
void synapcityApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 's' ) {
		slowMotion = !slowMotion;
		if(slowMotion)
			ParticleSystem::timeStep = .02;
		else
			ParticleSystem::timeStep = 1;
	}
	if (event.getChar() == 'f' ) {
	setFullScreen( ! isFullScreen() );
	}
	if (event.getChar() == 'i'){
		cluster::drawLabel = !cluster::drawLabel;
	}
}

void synapcityApp::mouseDown( MouseEvent event )
{
	isMousePressed = true;
	clusterSys->mousePos = Vec2i(event.getPos());	
}

void synapcityApp::mouseUp( MouseEvent event )
{
	isMousePressed = false;
}

void synapcityApp::mouseDrag( MouseEvent event )
{
	clusterSys->mousePos = Vec2i(event.getPos());
}



// This line tells Cinder to actually create the application
CINDER_APP_BASIC( synapcityApp, RendererGl )
