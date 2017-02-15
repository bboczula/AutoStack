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
			double maxPos = (temp->max - scopeMin) / scope;
			double minPos = (temp->min - scopeMin) / scope;
			SPoint x{ 14 + (15 * i), maxPos * 600, 0.5 };
			engine->drawQuadC(x, 2, (x.y - (minPos * 600)), 0, 0, 0);

			int close = static_cast<int>(closePos * 600);
			int open = static_cast<int>(openPos * 600);
			if (openPos > closePos)
			{
				SPoint p{ 10 + (15 * i), open, 0 };
				engine->drawQuadC(p, 10, (p.y - close), 255, 0, 0);
			}
			else
			{
				int temp = open;
				open = close;
				close = temp;

				SPoint p{ 10 + (15 * i), open, -0.5 };
				engine->drawQuadC(p, 10, (p.y - close), 0, 255, 0);
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