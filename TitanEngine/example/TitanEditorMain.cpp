#include "../include/TitanEditor.hpp"
#include <iostream>

using namespace Titan;

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    TitanEditor editor;
    if (!editor.Initialize("../")) {
        std::cerr << "Failed to initialize TitanEditor" << std::endl;
        return 1;
    }
    editor.Run();
    editor.Shutdown();
    return 0;
}
