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
		engine->clearMainRenderTarget();
		engine->drawQuad(SPoint{ 200, 200, 10}, 200, 100);
		engine->drawQuad(SPoint{ 200, 400, 10 }, 150, 150);
		engine->drawQuad(SPoint{ 400, 450, 10 }, 250, 50);
		engine->drawQuad(SPoint{ 450, 500, 10 }, 250, 50);
		engine->present();
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