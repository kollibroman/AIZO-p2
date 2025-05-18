#ifndef SSPMENU_H
#define SSPMENU_H

#include <iostream>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

inline void SSPMenu()
{
    std::cout << "Najkrotsza droga" << std::endl;

    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    std::cout << "Wybierz problem: " << std::endl;
    std::vector<std::string> entries = {
        "Wczytaj z pliku",
        "Wygeneruj graf losowo",
        "Wyswietl listowo i macierzowo",
        "Dijkstra",
        "Bellman ford"
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

#endif //SSPMENU_H
