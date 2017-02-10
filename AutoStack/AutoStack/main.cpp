#include "CBaseWindow.h"
#include "CWindowsApp.h"
#include "CEngine.h"
#include "CDataBase.h"
#include <iostream>

class CWindowsTest : public CWindowsApp
{
	CEngine* engine;
	CDataBase wig;
public:
	CWindowsTest() : CWindowsApp(nullptr)
	{
		wig.buildFromFile("wig_d.csv");

		registerWindow();
		createWindow("AutoStack v.1.0", 800, 600);

		engine = new CEngine(this->getHwndPtr());
	}
private:
	void initialize() override
	{
		const unsigned int NUM_OF_DAYS{ 40 };
		const unsigned int START_AT{ 5500 };

		SStock* temp = wig.at(START_AT);
		double scopeMax = temp->max;
		double scopeMin = temp->min;

		// Find max value in the scope
		for (int i = 0; i < NUM_OF_DAYS; i++)
		{
			temp = wig.at(START_AT + i);
			if (temp->max > scopeMax)
			{
				scopeMax = temp->max;
			}
			if (temp->min < scopeMin)
			{
				scopeMin = temp->min;
			}
		}

		// Draw chart
		temp = wig.at(START_AT);
		engine->clearMainRenderTarget();
		engine->drawFrame();
		for (int i = 0; i < NUM_OF_DAYS; i++)
		{
			temp = wig.at(START_AT + i);
			int close = static_cast<int>((temp->min * 600.0) / scopeMax);
			int open = static_cast<int>((temp->max * 600.0) / scopeMax);
			SPoint p{ 100 + (15 * i), open, 10 };
			if (open > close)
			{
				engine->drawQuad(p, 10, (p.y - close) * 15);
			}
			else
			{
				engine->drawQuad(p, 10, (p.y - close) * 15);
			}
		}
		engine->present();
	}
	void update() override
	{
		//initialize();
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