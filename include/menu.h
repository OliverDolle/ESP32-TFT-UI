#ifndef MENU_H
#define MENU_H

struct MenuItem {
  const char* label;
  void (*callback)();
};

void showMainMenu();
void addMenuItem(const char* label, void (*callback)());
void settingsMenu();
void pinConfigMenu();
void splashConfigMenu();

#endif