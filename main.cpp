#include "windows_painter.h"
#include "renderer.h"

int main(void)
{
	WindowsPainter painter;
	Renderer renderer;
	auto func = [&renderer] { renderer.Update(); };
	if (!painter.screen_init(800, 600))
	{
		return 0;
	}
	renderer.Init(800, 600, painter.screen_fb);
	painter.update(func);

	return 0;
}
