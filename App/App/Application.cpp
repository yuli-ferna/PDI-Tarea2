#include "Application.h"


Application::Application() {

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		__debugbreak();
		return;
	}

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	window = glfwCreateWindow(800, 600, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL) {
		__debugbreak();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	bool err = gladLoadGL() == 0;

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		__debugbreak();
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	//glfwSetScrollCallback(window, callBackMouseWhell);

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	Init();
}

Application::~Application() {

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	//delete img;
	//delete composite;
	//delete bw;
}


void Application::MainLoop()
{	
	zoom = 1.0;
	std::string path = "../momo.jpg";
	image = Image(path);
	pOpen = new bool;
	arr = std::vector(64, 1);
	columns_count = 4;
	lines_count = 1;
	CreateTexture();

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		processKeyboardInput(window);

		glViewport(0, 0, windowWidth, windowHeight);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// Your code here

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// ImGui
		UI();

		// Rendering
		ImGui::Render();
		Render();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

void Application::Render()
{
}

void Application::UI() {
	ImGui();
	ImageVisor(pOpen);
}

void Application::ImGui()
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				std::string newFile = loadPath(true);
				if (newFile != "")
				{
					image = Image(newFile);
					//image.Load();
					CreateTexture();

				}
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				std::string newFile = loadPath(false);
				if (newFile != "")
				{
					image.Save(newFile);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	// Or here
	ImGui::Begin("Editor");
	if (ImGui::CollapsingHeader("Info"))
	{
		ImGui::Text("size = %d x %d", cols, rows);

	}
	if(ImGui::SliderFloat("Zoom", &image.zoom, 0.1f, 3.0f)){

		//zoomEvent(image.zoom);
	}

	ImGui::SliderInt("Panning Left X", &leftPanningX, 0, image.drawImg.cols,"%d");
	ImGui::SliderInt("Panning Left Y", &leftPanningY, 0, image.drawImg.rows,"%d");
	ImGui::SliderInt("Panning Right X", &rightPanningX, 0, image.drawImg.cols, "%d");
	ImGui::SliderInt("Panning Right Y", &rightPanningY, 0, image.drawImg.rows, "%d");


	ImGui::SliderFloat("Rotate", &image.rotation, 0.0f, 360.0f, "%.1f �");
	//ImGui::SliderAngle("slider angle", &image.rotation);
	if (ImGui::IsItemEdited()) {
		rotationEvent(image.rotation);
	}
	if (ImGui::CollapsingHeader("Morphology"))
	{
		KernellView(arr, columns_count, lines_count);
	}
		
	
	ImGui::End();

}

void Application::KernellView(std::vector<int> &arr, int &columns_count, int& lines_count) {
	if (ImGui::TreeNode("Kernel"))
	{
		
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
		ImGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 7, "%d columns");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
		ImGui::DragInt("##lines_count", &lines_count, 0.1f, 2, 7, "%d rows");
		if (columns_count < 2)
			columns_count = 2;
		if (lines_count < 1)
			lines_count = 1;

		ImGui::Columns(columns_count, NULL, true);
		for (int i = 0; i < columns_count * lines_count; i++)
		{
			ImGui::PushID(i);

			if (ImGui::GetColumnIndex() == 0)
				ImGui::Separator();
			
			ImGui::Text("%i", arr[i]);
			ImGui::InputInt("", &arr[i]);
			ImGui::PopID();

			ImGui::NextColumn();
		}
		
		ImGui::Columns(1);
		//if (h_borders)
		ImGui::Separator();
		ImGui::TreePop();
	}
}

void Application::ImageVisor(bool *pOpen)
{
	// Or here
	float auxLeftPaddingX = float(leftPanningX)/image.drawImg.cols;
	float auxLeftPaddingY = float(leftPanningY)/image.drawImg.rows;
	float auxRightPaddingX = float(rightPanningX) / image.drawImg.cols;
	float auxRightPaddingY = float(rightPanningY) / image.drawImg.rows;

	int drawCols = abs(image.drawImg.cols - leftPanningX - (image.drawImg.cols - rightPanningX));
	int drawRows = abs(image.drawImg.rows - leftPanningY - (image.drawImg.rows - rightPanningY));
	
	ImGui::Begin("Image", pOpen, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Image((void*)(intptr_t)texture, ImVec2(drawCols  * image.zoom, drawRows * image.zoom),ImVec2(auxLeftPaddingX, auxLeftPaddingY), ImVec2(auxRightPaddingX, auxRightPaddingY));

	ImGui::End();

}

void Application::Init() {}

void Application::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

}

void Application::CreateTexture() {

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.drawImg.cols, image.drawImg.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.drawImg.data);

	cols = image.drawImg.cols;
	rows = image.drawImg.rows;
	leftPanningX = leftPanningY = 0;
	rightPanningX = image.drawImg.cols;
	rightPanningY = image.drawImg.rows;

}

void Application::processKeyboardInput(GLFWwindow* window) {

	// Checks if the escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

		// Tells glfw to close the window as soon as possible
		glfwSetWindowShouldClose(window, true);
	}
}

void Application::erode() {
	int erosion_type = 0;
	int erosion_elem = 0;
	int erosion_size = 4;
	if (erosion_elem == 0) { erosion_type = cv::MorphShapes::MORPH_RECT; }
	else if (erosion_elem == 1) { erosion_type = cv::MorphShapes::MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = cv::MorphShapes::MORPH_ELLIPSE; }
	cv::Mat element = cv::getStructuringElement(erosion_type,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	cv::erode(image.cImg, image.drawImg, element);
}

void Application::zoomEvent(float zoom)
{
	cv::resize(image.cImg, image.drawImg, cv::Size(), zoom, zoom);
	
	CreateTexture();

}

void Application::rotationEvent(double angle) {
	
	
	cv::resize(image.cImg, image.drawImg, cv::Size(), image.zoom, image.zoom);
	cv::Point2f center = cv::Point2f((image.drawImg.cols - 1.0) / 2.0, (image.drawImg.rows - 1.0) / 2.0);

	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// Create bounding box
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), image.drawImg.size(), angle).boundingRect2f();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - image.drawImg.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - image.drawImg.rows / 2.0;

	cv::warpAffine(image.drawImg, image.drawImg, rot, bbox.size());
	CreateTexture();
}

std::string Application::loadPath(bool open)
{
	
	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;

	ofn.lpstrFilter = L"JPG Files(.jpg)\0*.jpg\0PNG Files(.png)\0*.png\0BMP Files(.bmp)\0*.bmp";

	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";
	std::string fileNameStr;
	if ((open && GetOpenFileName(&ofn)) || (!open && GetSaveFileName(&ofn))) {
		std::wstring wsfileName(fileName);
		std::string str(wsfileName.begin(), wsfileName.end());
		fileNameStr = str;
		//std::cout << fileNameStr << std::endl;
	}
	return fileNameStr;
}
