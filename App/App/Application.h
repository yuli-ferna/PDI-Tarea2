#include "EDClientpch.h"
#include "Event.h"

#include <opencv2/core/core.hpp>

#ifndef __ED_CLIENT_APPLICATION__
#define __ED_CLIENT_APPLICATION__

class Application
{

	GLFWwindow *window;
	int windowWidth, windowHeight;
	int canvaWidth, canvaHeight;
	bool show_demo_window = true;

	int lowThreshold = 7, highThreshold = 50;
	unsigned int cols, rows;
	cv::Vec2d mouse;
	cv::Mat img,img2;
	int translateX, translateY;
	Image *image;
	std::string modalNameAct;
	Event event;
	bool p_open;

public:
	std::vector<int> arr;
	int columns_count;
	int lines_count;
	Application();
	~Application();
	
	void MainLoop();
	void Render();
	void UI();
	void ImGui();
	void modal();
	void ThresholdSection();
	void MorphologySection();
	void Histogram();
	void KernellView(std::vector<int>& arr, int& columns_count, int& lines_count);

	void ImageVisor();
	void Init();
	void processKeyboardInput(GLFWwindow*);

	void rotationEvent(double angle);

	void traslateEvent();

	std::string loadPath(bool open);

	static void HelpMarker(const char* desc);

};

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
#endif // !1

