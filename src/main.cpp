#include <irrlicht.h>
#include "events.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


int main()
{
  // Le gestionnaire d'événements
  EventReceiver receiver;

  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
					ic::dimension2d<u32>(640, 480),
					16, false, false, false, &receiver);

  is::ISceneManager *smgr = device->getSceneManager();
  iv::IVideoDriver  *driver = device->getVideoDriver();

  smgr->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -40), ic::vector3df(0, 5, 0));
  //is::ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya();
  //camera->setTarget(node->getPosition());

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(0,50,100,255));

    // Dessin de la scène :
    smgr->drawAll();
    //
    driver->endScene();
  }
  device->drop();

  return 0;
}

struct MyEventReceiver : IEventReceiver
{
  bool OnEvent(const SEvent &event)
  {
    // Si l'événement est de type clavier (KEY_INPUT)
    // et du genre pressage de touche
    // et que la touche est escape, on sort du programme
    if (event.EventType == EET_KEY_INPUT_EVENT &&
	event.KeyInput.PressedDown)
    {
      switch (event.KeyInput.Key)
      {
	case KEY_ESCAPE:
	  exit(0);
	default:;
      }
    }
    return false;
  }
};

