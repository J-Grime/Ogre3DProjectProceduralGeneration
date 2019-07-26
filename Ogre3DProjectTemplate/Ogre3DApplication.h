#pragma once

#include <Ogre.h>
#include <Bites/OgreTrays.h>
#include <Bites/OgreApplicationContext.h>
#include <Overlay/OgreFont.h>

class Ogre3DApplication : public OgreBites::ApplicationContext, public OgreBites::InputListener, public OgreBites::TrayListener
{
private:
	OgreBites::TrayManager* mTrayManager; 
	
	void setupUITray(const Ogre::String& name, Ogre::RenderWindow *window);

	
public:
	Ogre3DApplication();
	~Ogre3DApplication();

	//The following methods are virtual functions from ApplicationContext
	void setup() override;
	void shutdown() override;
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;
	Ogre::Camera* cam;
	Ogre::SceneNode* node;
	Ogre::SceneNode* camNode;
	Ogre::Timer* time;
	Ogre::SceneNode* lightNode;
	Ogre::SceneManager* scnMgr;
	Ogre::RenderWindow* rw;
	Ogre::SceneNode* ogreNode;
	Ogre::Entity* ogre;
	Ogre::Entity* temp;
	Ogre::SceneNode* tempNode;
	long start;


	std::vector<Ogre::SceneNode*> nodeArray;
	std::vector<Ogre::Entity*> entArray;

	Ogre::SceneNode *cameraYawNode;
	Ogre::SceneNode *cameraPitchNode;
	Ogre::SceneNode *cameraRollNode;
	struct spawns { int X; int Z; int rotation; bool loaded; } objects[500];

	void inView();

	

	//The following methods are virtual functions from InputListener
	//You can override them to customise how your application respond to various inputs.
	//virtual void frameRendered(const Ogre::FrameEvent& evt) override;
	void frameRendered(const Ogre::FrameEvent & evt, Ogre::SceneNode * camNode);
	virtual bool keyPressed(const OgreBites::KeyboardEvent& event) override;
	virtual bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
	virtual bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
	virtual bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;
	virtual bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
	virtual bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;

	//The following methods are virtual functions from TrayListener
	//You can override them to customise how your application respond to UI items event
	virtual void buttonHit(OgreBites::Button* button) override;
	virtual void itemSelected(OgreBites::SelectMenu* menu) override;
	virtual void labelHit(OgreBites::Label* label) override;
	virtual void sliderMoved(OgreBites::Slider* slider) override;
	virtual void checkBoxToggled(OgreBites::CheckBox* box) override;
	virtual void okDialogClosed(const Ogre::DisplayString& message) override;
	virtual void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit) override;
};

