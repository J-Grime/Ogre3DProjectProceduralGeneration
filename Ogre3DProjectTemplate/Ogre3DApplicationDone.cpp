#include "Ogre3DApplication.h"

const float PI = 4.141592654;
int space = 400;
float speed= 150.;
Ogre::Vector3 mDirection = Ogre::Vector3(0,0,0);

void Ogre3DApplication::setupUITray(const Ogre::String & name, Ogre::RenderWindow * window)
{
	//By default the Tray uses the overlay configuration in the file SdkTrays.overlay.
	//You can locate this file in media/packs/SdkTrays.zip
	//Unless you REALLY want to reskin the UI, I would recommend using the default overlay configuration as it has
	//already defined some common UI elements.
	mTrayManager = new OgreBites::TrayManager("UITray", window, this);

	//Add a button
	OgreBites::Button* quitBttn = mTrayManager->createButton(OgreBites::TL_BOTTOMLEFT, "QuitBTN", "Quit");

	//Add a label
	OgreBites::Label* titleLB = mTrayManager->createLabel(OgreBites::TL_TOP, "TitleLB", "CIT2213\nOgre3DProjTemplate");

	//physStats = mTrayManager->createTextBox(OgreBites::TL_BOTTOMRIGHT, "physStats", "physStats", Ogre::Real(500), Ogre::Real(300));

	//show the frame stat box
	//N.B. the frame stat box is created by TrayManager by default.
	mTrayManager->showFrameStats(OgreBites::TL_TOPRIGHT);

	mTrayManager->setTrayPadding(10.0f);

	mTrayManager->showAll();

	rw = window;
}

Ogre3DApplication::Ogre3DApplication()
: OgreBites::ApplicationContext("CIT2213 Ogre3D Template")
{
	
}

Ogre3DApplication::~Ogre3DApplication()
{

}

void Ogre3DApplication::setup()
{

	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();

	//Initialise the resource group labeled Essential
	//N.B. all resource groups are defined in the file resources.cfg or resources_d.cfg
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");

	// register for input events
	addInputListener(this);

	// get a pointer to the already created root
	Ogre::Root* root = getRoot();
	scnMgr = root->createSceneManager();

	scnMgr->addRenderQueueListener(mOverlaySystem);

	// register our scene with the RTSS
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	// without light we would just get a black screen    
	Ogre::Light* light = scnMgr->createLight("MainLight");
	Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 30, 0);
	lightNode->attachObject(light);

	// also need to tell where we are
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	// create the camera
	cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(1); // specific to this sample
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);

	getRenderWindow()->addViewport(cam);

	// finally something to render
	ogre = scnMgr->createEntity("Sinbad.mesh");
	ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogre);
	ogreNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree(0.0f)));
	int a = 0;
	int min = -500;
	int max = +500;

	while (a != max)
	{

		int randomX = min + (rand() % static_cast<int>(max - min + 1));
		int randomZ = min + (rand() % static_cast<int>(max - min + 1));
		int randomRotation = 0 + (rand() % static_cast<int>(359 - 0 + 1));

		objects[a].X = randomX;
		objects[a].Z = randomZ;
		objects[a].rotation = randomRotation;
		objects[a].loaded = false;
		a++;
	}

	
	setupUITray("TemplateUI", this->getRenderWindow());

}

void Ogre3DApplication::shutdown()
{
	//Clean up any resources created by this application
	//e.g. TrayManager was created specifically here
	delete mTrayManager;

	ApplicationContext::shutdown();
}

bool Ogre3DApplication::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	mTrayManager->frameRendered(evt);
	try
	{
		return ApplicationContext::frameRenderingQueued(evt);
	}
	catch (Ogre::Exception& e)   // show error and fall back to menu
	{
		mTrayManager->showOkDialog("Error!", e.getDescription() + "\nSource: " + e.getSource());
	}
	return true;
}






void Ogre3DApplication::frameRendered(const Ogre::FrameEvent & evt,Ogre::SceneNode* camNode)
{
	mTrayManager->frameRendered(evt);	
}


bool Ogre3DApplication::keyPressed(const OgreBites::KeyboardEvent & event)
{
	if (event.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	if (event.keysym.sym == OgreBites::SDLK_UP)
	{
		ogreNode->lookAt(Ogre::Vector3(0, 0, +10000), Ogre::Node::TS_PARENT);
		ogreNode->setPosition(Ogre::Vector3(ogreNode->getPosition().x, ogreNode->getPosition().y, ogreNode->getPosition().z - 10));
	}
	if (event.keysym.sym == OgreBites::SDLK_DOWN)
	{
		ogreNode->lookAt(Ogre::Vector3(0, 0, -10000), Ogre::Node::TS_PARENT);
		ogreNode->setPosition(Ogre::Vector3(ogreNode->getPosition().x, ogreNode->getPosition().y, ogreNode->getPosition().z + 10));
	}
	if (event.keysym.sym == OgreBites::SDLK_LEFT)
	{
		ogreNode->lookAt(Ogre::Vector3(10000, 0, 0), Ogre::Node::TS_PARENT);
		ogreNode->setPosition(Ogre::Vector3(ogreNode->getPosition().x-10, ogreNode->getPosition().y, ogreNode->getPosition().z));
	}
	if (event.keysym.sym == OgreBites::SDLK_RIGHT)
	{
		ogreNode->lookAt(Ogre::Vector3(-10000, 0, 0), Ogre::Node::TS_PARENT);
		ogreNode->setPosition(Ogre::Vector3(ogreNode->getPosition().x + 10, ogreNode->getPosition().y, ogreNode->getPosition().z));
	}
	if (event.keysym.sym == OgreBites::SDLK_PAGEDOWN)
	{
		camNode->setPosition(0, 300,0);
		camNode->lookAt(Ogre::Vector3(0, -1, 0), Ogre::Node::TS_PARENT);
	}
	if (event.keysym.sym == OgreBites::SDLK_SPACE)
	{
		
	}
	camNode->setPosition(ogreNode->getPosition().x, 300, ogreNode->getPosition().z);
	//lightNode->setPosition(ogreNode->getPosition().x, 300, ogreNode->getPosition().z);
	inView();
	return true;
}

bool Ogre3DApplication::keyReleased(const OgreBites::KeyboardEvent & evt)
{
	return true;
}

bool Ogre3DApplication::mouseMoved(const OgreBites::MouseMotionEvent & evt)
{
	if (mTrayManager->mouseMoved(evt))
		return true;

	return true;
}

bool Ogre3DApplication::mouseWheelRolled(const OgreBites::MouseWheelEvent & evt)
{
	if (mTrayManager->mouseWheelRolled(evt))
		return true;

	return true;
}

bool Ogre3DApplication::mousePressed(const OgreBites::MouseButtonEvent & evt)
{
	if (mTrayManager->mousePressed(evt))
		return true;

	return true;
}

bool Ogre3DApplication::mouseReleased(const OgreBites::MouseButtonEvent & evt)
{
	if (mTrayManager->mouseReleased(evt))
		return true;

	return true;
}

void Ogre3DApplication::buttonHit(OgreBites::Button * button)
{
	//Handle button hit event.
	Ogre::String bttnName = button->getName();

	if (bttnName == "QuitBTN")
	{
		getRoot()->queueEndRendering();
	}
}

void Ogre3DApplication::itemSelected(OgreBites::SelectMenu * menu)
{
}

void Ogre3DApplication::labelHit(OgreBites::Label * label)
{
}

void Ogre3DApplication::sliderMoved(OgreBites::Slider * slider)
{
}

void Ogre3DApplication::checkBoxToggled(OgreBites::CheckBox * box)
{
}

void Ogre3DApplication::okDialogClosed(const Ogre::DisplayString & message)
{
}

void Ogre3DApplication::yesNoDialogClosed(const Ogre::DisplayString & question, bool yesHit)
{
}
void Ogre3DApplication::inView()
{
	for (int i = 0; i <= 500; i++)
	{
		if ((objects[i].loaded == false) &&(((ogreNode->getPosition().x - 30<objects[i].X)&& (ogreNode->getPosition().x + 30 > objects[i].X))&& ((ogreNode->getPosition().z - 30 < objects[i].Z) && (ogreNode->getPosition().z + 30 > objects[i].Z))))
		{
			Ogre::Entity* temp = scnMgr->createEntity("robot.mesh");
			Ogre::SceneNode* tempNode = scnMgr->getRootSceneNode()->createChildSceneNode();
			tempNode->attachObject(temp);
			tempNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree(objects[i].rotation)));
			tempNode->setPosition(objects[i].X, 0, objects[i].Z);
			tempNode->scale(0.2f,0.2f,0.2f);
			objects[i].loaded == true;
		}
	}
	
}