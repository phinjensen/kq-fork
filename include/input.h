#pragma once

class KPlayerInput {
public:
    KPlayerInput();
    ~KPlayerInput() = default;

    void readcontrols();

public:
    // Flags for determining keypresses and player movement.

    // Moves the cursor or player horizontally (right).
    bool right;
    // Moves the cursor or player horizontally (left).
    bool left;
    // Moves the cursor or player vertically (up).
    bool up;
    // Moves the cursor or player vertically (down).
    bool down;
    // Exits menus, or opens the game menu.
    bool besc;
    // Usually the action or "accept" button.
    bool balt;
    // Usually the run or "cancel" button.
    bool bctrl;
    // Usually the menu button.
    bool benter;
    // Displays the (not-yet implemented) help menu.
    bool bhelp;
    // Activates cheats (calls cheat.lua) and runs whatever commands are found there.
    bool bcheat;

    // Scan codes for the keys (help is always F1)
    //int kright, kleft, kup, kdown;
    //int kesc, kenter, kalt, kctrl;

    // Joystick buttons
    //int jbalt, jbctrl, jbenter, jbesc;
};

extern KPlayerInput playerInput;
