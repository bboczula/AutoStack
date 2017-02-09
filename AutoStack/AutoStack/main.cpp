#include "CBaseWindow.h"
#include "CWindowsApp.h"
#include "CEngine.h"
#include "CDataBase.h"
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
	int pos = 200;
	void initialize() override
	{
	}
	void update() override
	{
		pos += 1;
		engine->clearMainRenderTarget();
		engine->drawQuad(SPoint{ 200, 600, 10}, 10, 100);
		engine->drawQuad(SPoint{ 200, 400, 10 }, 10, 150);
		engine->drawQuad(SPoint{ 400, 450, 10 }, 10, 50);
		engine->drawQuad(SPoint{ 450, 500, 10 }, 10, 50);
		engine->present();
	}
	void cleanup() override
	{
	}
};

int main(int argc, char* argv[])
{
	CDataBase wig;
	wig.buildFromFile("wig_d.csv");

	CWindowsTest* window = new CWindowsTest();
	window->start();
	delete window;

	return 0;
}