#pragma once // :3

#include <string> // :3
#include <Geode/loader/Types.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3

namespace geode::console { // :3
    // intended for setting up an already attached console :3
    // for example, if the game was launched with a debugger, it'd already have a console attached :3
    // so we can setup that console regardless of the setting :3
    void setup(); // :3
    // if the setting is on, we call tryOpenIfClosed, and if there's no console attached yet :3
    // (e.g. from a debugger, see above), this function should create a new console :3
    // and attach it (perhaps, by calling setup again, see windows impl for an example) :3
    void openIfClosed(); // :3

    void log(ZStringView msg, Severity severity); // :3
    void messageBox(ZStringView title, ZStringView info, Severity severity = Severity::Error); // :3
} // :3
