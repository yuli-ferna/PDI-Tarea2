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
	window = glfwCreateWindow(1800, 900, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
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
	std::string path = "../momo.jpg";
	image = Image(path);
	event = Event();
	image.createTexture();
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
	ImageVisor();
}

void Application::ImGui()
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "")) {
				std::string newFile = loadPath(true);
				if (newFile != "")
				{
					image = Image(newFile);
					image.createTexture();
				}
			}
			if (ImGui::MenuItem("Save", "")) {
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
	ImGui::Begin("Editor",0, ImGuiWindowFlags_NoCollapse);
	/*if (ImGui::ImageButton(my_tex_id, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f / my_tex_w, 32 / my_tex_h), frame_padding, ImVec4(0.0f, 0.0f, 0.0f, 1.0f))) {

	}*/

	ImGui::SliderFloat("Zoom", &image.zoom, 0.1f, 3.0f);

	ImGui::SliderInt("Panning Left X", &translateX, -image.drawImg.cols, image.drawImg.cols,"%d");
	ImGui::SliderInt("Panning Left Y", &translateY, -image.drawImg.rows, image.drawImg.rows,"%d");

	ImGui::SliderFloat("Rotate", &image.rotation, 0.0f, 360.0f, "%.1f �");
	//ImGui::SliderAngle("slider angle", &image.rotation);
	if (ImGui::IsItemEdited()) {
		rotationEvent(image.rotation);
	}

	if (ImGui::CollapsingHeader("Info"))
	{
		ImGui::Text("size = %d x %d", image.drawImg.cols, image.drawImg.rows);
		
	}
	if (ImGui::CollapsingHeader("Morphology"))
	{
		MorphologySection();
	}
	if (ImGui::CollapsingHeader("Threshold"))
	{
		ThresholdSection();

	}
	ImGui::End();
}

void Application::ThresholdSection() 
{
	const double f64_zero = 0.;
	const double f64_one = 1.;
	const ImU8 u8_min = 0;
	const ImU8 u8_max = 255;
	ImGui::Text("Methods:");
	ImGui::Combo("", &event.typeThresh, "OTSU\0TRIANGLE\0\0");
	ImGui::InputDouble("Thresh", &event.thresh, 1.f, 1.f, "%.0f");
	if (ImGui::IsItemEdited()) {
		if (event.thresh < 0)
		{
			event.thresh = 0;
		}
		if (event.thresh > 255)
		{
			event.thresh = 255;
		}
	}
	ImGui::InputDouble("Max Value", &event.maxValue, 1.f, 1.f, "%.0f");
	if (ImGui::IsItemEdited()) {
		if (event.maxValue < 0)
		{
			event.maxValue = 0;
		}
		if (event.maxValue > 255)
		{
			event.maxValue = 255;
		}
	}
	if (ImGui::Button("Apply")) {
		event.threshold(image);
	}
}

void Application::MorphologySection() {
	
	ImGui::Text("Actions:");

	if (ImGui::Button("Erode")) {
		event.erode(image);
	}
	ImGui::SameLine();
	if (ImGui::Button("Dilate")) {
		event.dilate(image);
	}
	ImGui::SameLine();
	if (ImGui::Button("Open")) {
		event.morphOpen(image);
	}
	ImGui::SameLine();
	if (ImGui::Button("Close")) {
		event.morphClose(image);
	}


	ImGui::Text("Structuring Element Type:");
	ImGui::RadioButton("MORPH_RECT", &event.structElem, 0); ImGui::SameLine();
	ImGui::RadioButton("MORPH_CROSS", &event.structElem, 1);
	ImGui::RadioButton("MORPH_ELLIPSE", &event.structElem, 2); ImGui::SameLine();
	ImGui::RadioButton("Arbitrary kernel", &event.structElem, 3);
	ImGui::Separator();
	if (event.structElem == 3)
	{
		KernellView(event.kernel, event.col, event.row);
	} else {
		ImGui::Text("Kernel size(2n + 1):"); ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
		if (ImGui::InputInt("", &event.structElemSize)) {
			if (event.structElemSize < 1)
			{
				event.structElemSize = 1;
			}
		}
	}
}

void Application::KernellView(std::vector<int> &arr, int &columns_count, int& lines_count) {
	ImGui::SetNextItemOpen(true);
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
			
			//ImGui::Text("%i", arr[i]);
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

void Application::ImageVisor()
{
	int drawCols = image.drawImg.cols;
	int drawRows = image.drawImg.rows;

	ImGui::Begin("Image", 0, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoResize);
	if (image.showUndo && ImGui::ArrowButton("##left", ImGuiDir_Left)) { 
		image.Undo();
		//CreateTexture();

	}
	
	//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	//float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	if (image.showRedo)
	{
		ImGui::SameLine();
		if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
			image.Redo();
			//CreateTexture();

		}

	}
	
	//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	ImGui::Separator();

	ImGui::BeginChildFrame(ImGui::GetID("Image"), ImVec2(1024, 800), ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::Image((void*)(intptr_t)image.texture, ImVec2(drawCols * image.zoom, drawRows * image.zoom));
	ImGui::EndChildFrame();

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

void Application::processKeyboardInput(GLFWwindow* window) {

	// Checks if the escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

		// Tells glfw to close the window as soon as possible
		glfwSetWindowShouldClose(window, true);
	}
}

void Application::rotationEvent(double angle) {
	
	
	//cv::resize(image.cImg, image.drawImg, cv::Size(), image.zoom, image.zoom);
	cv::Point2f center = cv::Point2f((image.cImg.cols - 1.0) / 2.0, (image.cImg.rows - 1.0) / 2.0);

	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// Create bounding box
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), image.cImg.size(), angle).boundingRect2f();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - image.cImg.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - image.cImg.rows / 2.0;

	cv::warpAffine(image.cImg, image.drawImg, rot, bbox.size());
	image.createTexture();
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
