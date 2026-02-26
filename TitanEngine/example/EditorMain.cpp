#include "../include/HammerEditor.hpp"
#include <iostream>

using namespace Titan;

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    HammerEditor editor;
    if (!editor.Initialize("../")) {
        std::cerr << "Failed to initialize HammerEditor" << std::endl;
        return 1;
    }
    editor.Run();
    editor.Shutdown();
    return 0;
}
