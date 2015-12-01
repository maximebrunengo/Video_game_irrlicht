// events.cpp

#include <irrlicht.h>

#include "events.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
  : node(nullptr),camera(nullptr), button_pressed(false), current_texture(0)
{
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                                                *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
  if (event.KeyInput.PressedDown)
  {
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();
    switch (event.KeyInput.Key)
    {
      case KEY_ESCAPE:
	exit(0);
      case KEY_KEY_Z: // Avance
    /* Personnage */
	position.X += 1 * cos(rotation.Y * M_PI / 180.0);
	position.Z += -1 * sin(rotation.Y * M_PI / 180.0);
	break;
      case KEY_KEY_S: // Recule
    /* Personnage */
	position.X += -1 * cos(rotation.Y * M_PI / 180.0);
	position.Z += 1 * sin(rotation.Y * M_PI / 180.0);
	break;
      case KEY_KEY_D: // Tourne à droite
    /* Personnage */
    rotation.Y += 10;
	break;
      case KEY_KEY_Q: // Tourne à gauche
    /* Personnage */
    rotation.Y -= 10;
	break;
      case KEY_KEY_C: // Affichage du stuff
    /* Ouverture de la fenêtre affichant son sac a dos */
    // Une fenêtre pour différents réglages
    //create_window(gui);
    break;
      default:;
    }
    node->setPosition(position);
    node->setRotation(rotation);
    //mise a jour de la target de la camera (suivi du personnage en temps réel)
    camera->setTarget(position);
  }

  return false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse(const SEvent &event)
{
  switch(event.MouseInput.Event)
  {
    case EMIE_LMOUSE_PRESSED_DOWN:
      button_pressed = true;
      old_x = event.MouseInput.X;
      old_y = event.MouseInput.Y;
      break;
    case EMIE_LMOUSE_LEFT_UP:
      button_pressed = false;
      break;
    case EMIE_MOUSE_MOVED:
      if (button_pressed)
      {
	ic::vector3df rotation = node->getRotation();
	rotation.Y -= (event.MouseInput.X - old_x);
	old_x = event.MouseInput.X;
	old_y = event.MouseInput.Y;
	//node->setRotation(rotation);
      }
      break;
    case EMIE_MOUSE_WHEEL:
//      current_texture = (current_texture + 1) % textures.size();
//      node->setMaterialTexture(0, textures[current_texture]);
      break;
    default:
      ;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
  if (!node) return false;
  switch (event.EventType)
  {
    case EET_KEY_INPUT_EVENT:
      return keyboard(event);
    case EET_MOUSE_INPUT_EVENT:
      return mouse(event);
    default:;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::set_node                                                *
\**************************************************************************/
void EventReceiver::set_node(irr::scene::ISceneNode *n)
{
  node = n;
}

/**************************************************************************\
 * EventReceiver::set_gui                                                 *
\**************************************************************************/
void EventReceiver::set_gui(irr::gui::IGUIEnvironment *g)
{
  gui = g;
}

/**************************************************************************\
 * EventReceiver::set_camera                                              *
\**************************************************************************/
void EventReceiver::set_camera(irr::scene::ICameraSceneNode *c)
{
  camera = c;
}
