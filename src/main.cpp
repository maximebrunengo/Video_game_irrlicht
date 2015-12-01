#include <irrlicht.h>
#include "events.h"
#include "gui_game.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

void createtree(is::IAnimatedMesh *mesh, is::ISceneManager *smgr, EventReceiver receiver, iv::IVideoDriver  *driver);

int main()
{
    // Le gestionnaire d'événements
    EventReceiver receiver;
    std::vector<iv::ITexture*> textures;

    // Création de la fenêtre et du système de rendu.
    IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
					  ic::dimension2d<u32>(640, 480),
					  16, false, false, false, &receiver);

    is::ISceneManager *smgr = device->getSceneManager();
    iv::IVideoDriver  *driver = device->getVideoDriver();
    ig::IGUIEnvironment *gui_game = device->getGUIEnvironment();

    // Chargement du cube
    is::IAnimatedMesh *mesh = smgr->getMesh("data/cube.3ds");

    // Ajout du cube à la scène
    is::IAnimatedMeshSceneNode *node;
    int id = 0;
    int Ni = 50;
    int Nj = 50;

    for (int i = 0 ; i < Ni ; i ++)
    {
        for (int j = 0 ; j < Nj ; j ++)
        {
            node = smgr->addAnimatedMeshSceneNode(mesh,nullptr,id);
            node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
            node->setPosition (core::vector3df(i,0,j));
            id++;

            textures.push_back(driver->getTexture("data/TXsQk.png"));
            node->setMaterialTexture(0, textures[0]);
            receiver.set_node(node);
            receiver.set_textures(textures);

        }
    }
    //ajout arbre via une fonction
   createtree(mesh, smgr,receiver,driver);

  // Ajout du cube à la scène
  is::IAnimatedMeshSceneNode *node_personnage;
  node_personnage = smgr->addAnimatedMeshSceneNode(mesh);
  node_personnage->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  node_personnage->setPosition(core::vector3df(20, 2, 20));
  textures.push_back(driver->getTexture("data/rouge.jpg"));
  node_personnage->setMaterialTexture(0, textures.back());
  receiver.set_node(node_personnage);
  receiver.set_textures(textures);

  receiver.set_gui(gui_game);

  //smgr->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -40), ic::vector3df(0, 5, 0));
  is::ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya();
  camera->setTarget(node_personnage->getPosition());

  // La barre de menu
  gui_game::create_menu(gui_game);

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(0,50,100,255));

    // Dessin de la scène :
    smgr->drawAll();
    // Dessin de l'interface utilisateur :
    gui_game->drawAll();

    driver->endScene();
  }
  device->drop();

  return 0;
}

void createtree(is::IAnimatedMesh *mesh, is::ISceneManager *smgr, EventReceiver receiver, iv::IVideoDriver  *driver)
{
    std::vector<iv::ITexture*> textures;
    is::IAnimatedMeshSceneNode *node_arbre;
    int i = 10;
    int j = 10;
    int id=0;

    for (int k = 0 ; k < 15 ; k ++)
    {
	node_arbre = smgr->addAnimatedMeshSceneNode(mesh,nullptr,id);
	node_arbre->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node_arbre->setPosition (core::vector3df(i,k,j));
	textures.push_back(driver->getTexture("data/tree.jpg"));
	node_arbre->setMaterialTexture(0, textures[0]);
	receiver.set_node(node_arbre);
	receiver.set_textures(textures);
	id++;
    }

}


