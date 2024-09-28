#include "Main.h"
#include "MainLoop.h"

int main(int argc, char** argv)
{
	std::unique_ptr<Chimp::Scene> entryScene = CreateEntryScene();
	Chimp::MainLoop mainLoop(std::move(entryScene));
    return 0;
}