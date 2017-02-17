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
		wig.buildFromFile("cdr_d.csv");

		registerWindow();
		createWindow("AutoStack v.1.0", 800, 600);

		engine = new CEngine(this->getHwndPtr());
	}
private:
	void initialize() override
	{
		const unsigned int NUM_OF_DAYS{ 50 };
		const unsigned int START_AT{ 5540 };

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
		double scope = scopeMax - scopeMin;
		engine->clearMainRenderTarget();
		engine->drawFrame();
		for (int i = 0; i < NUM_OF_DAYS; i++)
		{
			temp = wig.at(START_AT + i);
			double openPos = (temp->open - scopeMin) / scope;
			double closePos = (temp->close - scopeMin) / scope;

			// Draw min-max
			const int LEVEL_1{ 119 };
			const int LEVEL_2{ 118 };
			double maxPos = (temp->max - scopeMin) / scope;
			double minPos = (temp->min - scopeMin) / scope;
			SPoint x{ 15 + (15 * i), maxPos * 600, LEVEL_1 };
			engine->drawQuadC(x, 1, (x.y - (minPos * 600)), 255, 255, 255);

			int close = static_cast<int>(closePos * 600);
			int open = static_cast<int>(openPos * 600);
			if (openPos > closePos)
			{
				SPoint p{ 10 + (15 * i), open, LEVEL_2 };
				engine->drawQuadC(p, 10, (p.y - close), 255, 192, 0);
			}
			else
			{
				int temp = open;
				open = close;
				close = temp;

				SPoint p{ 10 + (15 * i), open, LEVEL_2 };
				engine->drawQuadC(p, 10, (p.y - close), 172, 211, 254);
			}
		}
		//engine->drawTextureQuad({ 200, 300, 0 }, 200, 200);
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