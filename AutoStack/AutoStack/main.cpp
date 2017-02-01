#include "CBaseWindow.h"
#include "CWindowsApp.h"
#include "CEngine.h"
#include <iostream>

class CWindowsTest : public CWindowsApp
{
	CEngine* engine;
public:
	CWindowsTest() : CWindowsApp(nullptr)
	{
		registerWindow();
		createWindow("AutoStack v.1.0", 800, 600);

		engine = new CEngine(this->getHwndPtr());
	}
private:
	void initialize() override
	{
	}
	void update() override
	{
	}
	void cleanup() override
	{
	}
};

int main(int argc, char* argv[])
{
	CWindowsTest* window = new CWindowsTest();
	window->start();
	delete window;

	return 0;
}