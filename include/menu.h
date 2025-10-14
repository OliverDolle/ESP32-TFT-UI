#ifndef MENU_H
#define MENU_H

void showMainMenu();
void addMenuItem(const char* label, void (*callback)());

#endif