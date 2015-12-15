#include <irrlicht.h>
#include "events.h"
#include "gui_game.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;


is::ITriangleSelector* createTree(is::IAnimatedMesh *mesh, is::ISceneManager *smgr, EventReceiver receiver, iv::IVideoDriver  *driver);
is::ITriangleSelector*  createColumn(int position_x, int position_z, int height, is::IAnimatedMeshSceneNode *node, is::IAnimatedMesh *mesh, is::ISceneManager *smgr, std::vector<iv::ITexture*> textures, EventReceiver receiver);
is::ITriangleSelector* createMountain(int nbMountains, is::IAnimatedMeshSceneNode *node, is::IAnimatedMesh *mesh, is::ISceneManager *smgr, std::vector<iv::ITexture*> textures, EventReceiver receiver);

const int ID = 40;
const int cpt = 0;

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

    // Stockage des textures
    textures.push_back(driver->getTexture("data/TXsQk.png"));

    // Chargement du cube
   is::IAnimatedMesh *mesh = smgr->getMesh("data/cube.obj");

    //chargement du santaclaus
    is::IAnimatedMesh *mesh_santaclaus = smgr->getMesh("data/Steve.obj");

    //chargement du tree
    is::IAnimatedMesh *mesh_tree = smgr->getMesh("data/lowpolytree.obj");

    // Ajout de la scène
    is::IAnimatedMeshSceneNode *node;
    int Ni = 100;
    int Nj = 100;

    is::IMetaTriangleSelector *metaselector = smgr-> createMetaTriangleSelector();

    for (int i = 0 ; i < Ni ; i ++)
    {
        for (int j = 0 ; j < Nj ; j ++)
        {
            node = smgr->addAnimatedMeshSceneNode(mesh,nullptr,i+j);
            node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
            node->setPosition (core::vector3df(i,0,j));

            textures.push_back(driver->getTexture("data/TXsQk.png"));
            node->setMaterialTexture(0, textures[0]);
            receiver.set_node(node);
            receiver.set_textures(textures);

            // Création du triangle selector pour gérer la collision
            is::ITriangleSelector *selector = smgr->createTriangleSelector(node->getMesh(),node);
            node ->setTriangleSelector (selector);

            //meta selector permettant de stocker les selecteurs de tous mes cubes
            metaselector->addTriangleSelector(selector);

        }
    }

    //is::ITriangleSelector* selector1 = createTree(mesh, smgr,receiver,driver);
    //metaselector->addTriangleSelector(selector1);

    //Ajout de reliefs sur la scene
    int nbMountains = 2;
    is::ITriangleSelector* selector2 = createMountain(nbMountains, node, mesh, smgr, textures, receiver);
    metaselector->addTriangleSelector(selector2);

    // Ajout du cube à la scène
    /*is::IAnimatedMeshSceneNode *node_personnage;
    node_personnage = smgr->addAnimatedMeshSceneNode(mesh);
    node_personnage->setPosition(core::vector3df(20, 10, 20));
    textures.push_back(driver->getTexture("data/rouge.jpg"));
    node_personnage->setMaterialTexture(0, textures.back());
    node_personnage->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    receiver.set_node(node_personnage);
    receiver.set_textures(textures);*/

    //ajout tree (ou une forêt)  à la scene
    is::IAnimatedMeshSceneNode *node_tree;
    int Nl = 7;
    int Nk = 7;

    for (int l=0; l<Nl; l++)
    {
	for (int k=0; k<Nk; k++)
	{
	    node_tree = smgr->addAnimatedMeshSceneNode(mesh_tree,nullptr, l+k);
	    node_tree->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	    node_tree->setPosition(core::vector3df(10+4*l, 4.3, 60+4*k));//30+4*l, 4.3, 5+4*l));
	    node_tree->setScale(core::vector3df(1.5,1.5,1.5));
	    receiver.set_node(node_tree);

	    //node_tree->setDebugDataVisible(is::EDS_NORMALS);

	    //selector sur l'arbre
	    is::ITriangleSelector *selector = smgr->createTriangleSelector(mesh_tree,node_tree);
	    node_tree->setTriangleSelector(selector);
	    selector->drop();
	    node_tree->setID(ID);

	    metaselector->addTriangleSelector(selector);
	}
    }


    //ajout santaclaus à la scene
    is::IAnimatedMeshSceneNode *node_santaclaus;
    node_santaclaus = smgr->addAnimatedMeshSceneNode(mesh_santaclaus);
    node_santaclaus->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node_santaclaus->setPosition(core::vector3df(20, 2, 30));
    textures.push_back(driver->getTexture("data/Santa.png"));
    node_santaclaus->setMaterialTexture(0, textures.back());
    node_santaclaus->setScale(core::vector3df(0.5,0.5,0.5));
    receiver.set_node(node_santaclaus);
    receiver.set_textures(textures);

    receiver.set_gui(gui_game);

    //Gestion collision
    is::ISceneNodeAnimator *anim;
    anim = smgr ->createCollisionResponseAnimator(metaselector, node_santaclaus,
                                                  ic::vector3df(1, 1, 1), //Rayon de la cam
                                                  ic::vector3df(0, -10, 0),  //gravité
						  ic::vector3df(0, 0, 0));  // décalage du centre

    node_santaclaus->addAnimator(anim);

    //gestionnaire de collision "Selection"

     is::ISceneCollisionManager *collision_manager = smgr->getSceneCollisionManager();


    //caméra qui va suivre notre personnage

    //son parent est donc le noeud qui definit le personnage
    //deuxieme paramètre:position de la camera (look From)
    //troisieme paramètre: look at (ici c'est la position du personnage) mise a jour dans event.cpp
    is::ICameraSceneNode *camera = smgr->addCameraSceneNode(node_santaclaus, ic::vector3df(20,10,0), node_santaclaus->getPosition());
    receiver.set_camera(camera);

    // La barre de menu
    gui_game::create_menu(gui_game);

    while(device->run())
    {

        driver->beginScene(true, true, iv::SColor(0,50,100,255));

	//Séléction de l'arbre à couper avec la souris
	int mouse_x, mouse_y;
	if (receiver.is_mouse_pressed(mouse_x, mouse_y))
	{
	  ic::line3d<f32> ray;
	  ray = collision_manager->getRayFromScreenCoordinates(ic::position2d<s32>(mouse_x, mouse_y));

	  ic::vector3df intersection;
	  ic::triangle3df hit_triangle;

	  is::ISceneNode *selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(ray,
													intersection, // On récupère ici les coordonnées 3D de l'intersection
													hit_triangle, // et le triangle intersecté
													ID); // On ne veut que des noeuds avec cet identifiant

	  //on supprime les arbres
	  if (selected_scene_node)
	  {
	    selected_scene_node->setVisible(false);
	    cpt ++;
	  }

	}

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        gui_game->drawAll();

        driver->endScene();

    }
    device->drop();

    return 0;
}

/*is::ITriangleSelector* createTree(is::IAnimatedMesh *mesh, is::ISceneManager *smgr, EventReceiver receiver, iv::IVideoDriver  *driver)
{
    std::vector<iv::ITexture*> textures;
    is::IAnimatedMeshSceneNode *node_arbre;
    int i = 10;
    int j = 10;

    is::ITriangleSelector *selector;
    is::IMetaTriangleSelector *

            metaselector = smgr-> createMetaTriangleSelector();;

    for (int k = 0 ; k < 15 ; k ++)
    {
        node_arbre = smgr->addAnimatedMeshSceneNode(mesh,nullptr,i+j+k);
        node_arbre->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node_arbre->setPosition (core::vector3df(i,k,j));
        textures.push_back(driver->getTexture("data/tree.jpg"));
        node_arbre->setMaterialTexture(0, textures[0]);
        //node_arbre->setDebugDataVisible(is::EDS_BBOX | is::EDS_HALF_TRANSPARENCY);
        receiver.set_node(node_arbre);
        receiver.set_textures(textures);

        // Création du triangle selector pour gérer la collision
        selector = smgr->createTriangleSelector(node_arbre->getMesh(),node_arbre);
        node_arbre ->setTriangleSelector (selector);

        metaselector->addTriangleSelector(selector);
    }

    return metaselector;
}*/

//Create a moutain
is::ITriangleSelector* createMountain(int nbMountains, is::IAnimatedMeshSceneNode *node, is::IAnimatedMesh *mesh, is::ISceneManager *smgr, std::vector<iv::ITexture*> textures, EventReceiver receiver)
{
    int Ni=100;
    int Nj=100;

    is::ITriangleSelector *selector;
    is::IMetaTriangleSelector *metaselector = smgr-> createMetaTriangleSelector();

    for(int i = 0; i<nbMountains; ++i)
    {
        int delta_max = 20;
        int delta = delta_max;
        int height = 1;
        // Get a random position for the lowest left point of the mountain
        int position_x = rand()%Ni;
        int position_z = rand()%Nj;
        while(delta>2)
        {
            for (int x=position_x; x<position_x+delta; ++x)
            {
                for(int z=position_z; z<position_z+delta; ++z)
                {
                    selector = createColumn(x, z, height, node, mesh, smgr, textures, receiver);

                    metaselector->addTriangleSelector(selector);
                }
            }
            position_x++;
            position_z++;
            height++;
            delta-=2;
        }
    }
    return metaselector;
}


// Given a position on the scene, add cubes to go to the given height
is::ITriangleSelector* createColumn(int position_x, int position_z, int height, is::IAnimatedMeshSceneNode *node, is::IAnimatedMesh *mesh, is::ISceneManager *smgr, std::vector<iv::ITexture*> textures, EventReceiver receiver)
{
    is::ITriangleSelector *selector;
    is::IMetaTriangleSelector *metaselector = smgr-> createMetaTriangleSelector();

    for(int i=1; i<height; ++i)
    {
        node = smgr->addAnimatedMeshSceneNode(mesh, nullptr, position_x+i+position_z);
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node->setPosition (core::vector3df(position_x, i, position_z));

        node->setMaterialTexture(0, textures[0]);
        receiver.set_node(node);
        receiver.set_textures(textures);

        selector = smgr->createTriangleSelector(node->getMesh(),node);
        node ->setTriangleSelector (selector);

        metaselector->addTriangleSelector(selector);
    }
    return metaselector;
}


