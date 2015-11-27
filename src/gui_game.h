#ifndef GUI_H
#define GUI_H

#include "gui_ids.h"
#include <irrlicht.h>

class gui_game
{
    public:
        static void create_menu(irr::gui::IGUIEnvironment *gui);
        void create_window(irr::gui::IGUIEnvironment *gui);
};

#endif // GUI_H
