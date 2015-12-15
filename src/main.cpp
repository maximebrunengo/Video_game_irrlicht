#include <irrlicht.h>
#include "events.h"
#include "gui_game.h"
#include <iostream>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;


is::ITriangleSelector* createTree(is::IAnimatedMesh *mesh, is::ISceneManager *smgr, EventReceiver receiver, iv::IVideoDriver  *driver);
is::ITriangleSelector*  createColumn(int position_x, int position_z, int height, is::IAnimatedMeshSceneNode *node, is::IAnimatedMesh *mesh, is::ISceneManager *smgr, std::vector<iv::ITexture*> textures, EventReceiver receiver);
is::ITriangleSelector* createMountain(int nbMountains, is::IAnimatedMeshSceneNode *node, is::IAnimatedMesh *mesh, is::ISceneManager *smgr, std::vector<iv::ITexture*> textures, EventReceiver receiver);

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


    //Ajout de refliefs sur la scene
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

    //ajout tree à la scene
    is::IAnimatedMeshSceneNode *node_tree;
    node_tree = smgr->addAnimatedMeshSceneNode(mesh_tree);
    node_tree->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node_tree->setPosition(core::vector3df(30, 6, 50));
   node_tree->setScale(core::vector3df(1.5,1.5,1.5));
    receiver.set_node(node_tree);

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

    //caméra qui va suivre notre personnage

    //son parent est donc le noeud qui definit le personnage
    //deuxieme paramètre:position de la camera (look From)
    //troisieme paramètre: look at (ici c'est la position du personnage) mise a jour dans event.cpp
    is::ICameraSceneNode *camera = smgr->addCameraSceneNode(node_santaclaus, ic::vector3df(20,10,0), node_santaclaus->getPosition());
    receiver.set_camera(camera);

    // La barre de menu
    gui_game::create_menu(gui_game);

    //declaration du vecteur position necessaire a la gestion du game over
    ic::vector3df pos;
    iv::ITexture* image_gameover = driver->getTexture("data/gameover.jpg");

    while(device->run())
    {

        driver->beginScene(true, true, iv::SColor(0,50,100,255));

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        gui_game->drawAll();

        //game over "sortie du plateau de jeu"
        //on peut le rendre plus robuste en ragardant les limites x et z du plateau
        //ICI j'utilise l'altitude du personnage

        //mise a jour de la postion du personnage  en continu
        pos = node_santaclaus->getPosition();
        if (pos.Y < -5) //condition de game over
        {
            std::cout<<"GAME oVER"<<std::endl;
            driver->draw2DImage(image_gameover, core::position2d<s32>(80,80),
                         core::rect<s32>(0,0,480,360), 0,
                         video::SColor(255,255,255,255), true);

            if(pos.Y < -100) exit(0); //on quite le jeu
            // Amelioration: a la placve de quiter le jeu on attend un clique souris ou clavier pour se remmetre en position initiale


        }

        driver->endScene();



    }
    device->drop();

    return 0;
}

is::ITriangleSelector* createTree(is::IAnimatedMesh *mesh, is::ISceneManager *smgr, EventReceiver receiver, iv::IVideoDriver  *driver)
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
}

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

