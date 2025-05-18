#ifndef MAINMENU_H
#define MAINMENU_H

#include <vector>
#include <string>

#include "MaxFlowMenu.h"
#include "MSTMenu.h"
#include "SSPMenu.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

inline void MainMenu()
{
     auto screen = ftxui::ScreenInteractive::TerminalOutput();
     std::cout << "Wybierz problem: " << std::endl;
     std::vector<std::string> entries = {
         "MST",
         "Najkrotsza droga",
         "Maksymalny przeplyw"
     };
     int selected = 0;
     bool quit = false;


    auto menu = ftxui::Menu(
      {
         .entries = &entries,
         .selected = &selected,
         .on_enter = [&screen, &quit]() {
             quit = true;
             screen.Exit();
         },
    });

    screen.Loop(menu);

    switch (selected)
    {
        case 0:
            MSTMenu();
            break;
        case 1:
            SSPMenu();
            break;
        case 2:
            MaxFlowMenu();
            break;
        default:
            std::cout << "Wybierz poprawny problem" << std::endl;
            MainMenu();
    }
}

#endif //MAINMENU_H
