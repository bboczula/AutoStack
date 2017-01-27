#include "CBaseWindow.h"
#include "CWindowsApp.h"

class CWindowsTest : public CWindowsApp
{
public:
	CWindowsTest() : CWindowsApp(nullptr)
	{
		registerWindow();
		createWindow("AutoStack v.1.0", 800, 600);
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