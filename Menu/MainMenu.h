#ifndef MAINMENU_H
#define MAINMENU_H

#include <vector>
#include <string>
#include <memory>
#include "MSTMenu.h"
#include "SSPMenu.h"
#include "MaxFlowMenu.h"
#include "Abstractions/MenuBase.h"

class MainMenu : public MenuBase {
public:
    MainMenu() : MenuBase() {}

    void show() {
        std::vector<std::string> entries = {
            "1. Shortest Path Problem",
            "2. Minimum Spanning Tree",
            "3. Maximum Flow",
            "4. Exit"
        };

        runMenu("Graph Algorithm Solver", entries, [this](int selected) {
            switch (selected) {
                case 0: {
                    auto sspMenu = std::make_unique<SSPMenu>();
                    sspMenu->show();
                    break;
                }
                case 1: {
                    auto mstMenu = std::make_unique<MSTMenu>();
                    mstMenu->show();
                    break;
                }
                case 2: {
                    auto maxFlowMenu = std::make_unique<MaxFlowMenu>();
                    maxFlowMenu->show();
                    break;
                }
                case 3:
                    exit(0);
                default:
                    break;
            }
        });
    }
};

#endif //MAINMENU_H