#include "EDClientpch.h"

#include <opencv2/core/core.hpp>

#ifndef __ED_CLIENT_APPLICATION__
#define __ED_CLIENT_APPLICATION__

class Application
{

	GLFWwindow *window;
	int windowWidth, windowHeight;
	bool show_demo_window = true;

	int lowThreshold = 7, highThreshold = 50;
	unsigned int texture;
	unsigned int cols, rows;
	cv::Mat img;
public:
	Application();
	~Application();
	
	void MainLoop();
	void Render();
	void UI();
	void ImGui();
	void ImageVisor();
	void Init();
	
	static void HelpMarker(const char* desc);

};

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
#endif // !1

