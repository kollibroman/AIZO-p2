#ifndef MSTMENU_H
#define MSTMENU_H
#include <iostream>

#include "MainMenu.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

inline void MSTMenu()
{
    std::cout << "MST" << std::endl;

    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    std::cout << "Wybierz problem: " << std::endl;
    std::vector<std::string> entries = {
        "Wczytaj z pliku",
        "Wygeneruj graf losowo",
        "Wyswietl listowo i macierzowo",
        "Kruskal",
        "Prim"
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

            break;
        case 1:

            break;
        case 2:

            break;
        default:
            std::cout << "Wybierz poprawny problem" << std::endl;
    }
}

#endif //MSTMENU_H
