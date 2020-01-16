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
	 window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
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
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	if (EDImage::TryLoad("C:/Users/adrianjmejias/Desktop/CG3-P1/chikis.jpg", img))
	{
		std::cout << "img loaded successfully\n";

		//if (EDImage::TrySave(img, "saved.png"))
		//{
		//	std::cout << "success img save \n";
		//}
		//else 
		//{
		//	std::cout << "failed img save \n";
		//}
	}
	else
	{
		std::cout << "img load fail\n";

	}



	std::unique_ptr<RawData> negative{ EDNegativeHA(img->data, img->GetWidth(), img->GetHeight()) };




	std::string pathNegative = "negative.png";
	EDImage::TrySave(negative.get(), pathNegative, img->GetWidth(), img->GetHeight(), img->GetNChannels());
	
	/*NegativeHA(img->data, imgWidth, imgHeight, nChannels, strideX, strideY, conv, convWidth, convHeight);*/












	//{
	//	std::unique_ptr<EDBaseConvolution> mv10{ EDConvolution::CreateCustom({ 0,0,0,0,0,0,0,0,0,1 }, 10, 1, 9, 0) };
	//	std::unique_ptr<EDImage> mvImg{ mv10->ApplyConvolution(*img) };
	//	Save(mvImg.get(), "mv.png");
	//}




	//bw = EDConvolution::CreateNegative();
	//composite = EDCompositeConv::Create();

	//std::unique_ptr<EDConvolution> blur{ EDConvolution::CreateCustom(
	//	EDConvolution::Normalize({
	//		1,2,1,
	//		2,3,2,
	//		1,2,1
	//	}),
	//	{ 0,0,0,0 }, 3, 3, 1, 1) };

	//std::unique_ptr<EDConvolution> blurPower{ EDConvolution::CreateCustom(
	//EDConvolution::Normalize({
	//	1,2,3,2,1,
	//	2,4,6,4,2,
	//	3,6,8,6,3,
	//	2,4,6,4,2,
	//	1,2,3,2,1
	//}),
	//{ 0,0,0,0 }, 5, 5, 2, 2) };


	//std::unique_ptr<EDConvolution> sobelX{ EDConvolution::CreateCustom(
	//{
	//	1, 2, 1,
	//	0, 0, 0,
	//	-1, -2, 1,
	//},
	//{ 0,0,0,0 }, 3, 3, 1, 1) };

	//std::unique_ptr<EDConvolution> sobelY{ EDConvolution::CreateCustom(
	//{
	//	-1 , 0, 1,
	//	-2 , 0, 2,
	//	-1 , 0, 1,
	//},
	//{ 0,0,0,0 }, 3, 3, 1, 1) };

	//std::unique_ptr<EDConvolution> average{ EDConvolution::CreateCustom(
	//EDConvolution::Normalize({
	//	1,1,1,
	//	1,1,1,
	//	1,1,1,
	//}),
	//{ 0,0,0,0 }, 3, 3, 1, 1) };

	//std::unique_ptr<EDConvolution> laplacian{ EDConvolution::CreateCustom(
	//{
	//	-1,-1,-1,
	//	-1,8,-1,
	//	-1,-1,-1,
	//},
	//{ 0,0,0,0 }, 3, 3, 1, 1) };


	//composite->push_back(bw);
	//composite->push_back(bw);
	
	{
		//std::unique_ptr<EDImage> bwImg{bw->ApplyConvolution(*img)};
		//Save(bwImg.get(), "bw.png");

		//std::unique_ptr<EDImage> srcDest{ bw->ApplyConvolution(*img, img) };
		//Save(srcDest.get(), "srcDest.png");


		//std::unique_ptr<EDImage> compositeImg{ composite->ApplyConvolution(*img) };
		//Save(compositeImg.get(), "composite.png");


		//std::unique_ptr<EDImage> blurImg{ blur->ApplyConvolution(*img) };
		//Save(blurImg.get(), "blurImg.png");

		//std::unique_ptr<EDImage> blurPowerImg { blurPower->ApplyConvolution(*img) };
		//Save(blurPowerImg.get(), "blurPowerImg.png");

		//std::unique_ptr<EDImage> sobelXImg{ sobelX->ApplyConvolution(*img) };
		//Save(sobelXImg.get(), "sobelXImg.png");


		//std::unique_ptr<EDImage> sobelYImg{ sobelY->ApplyConvolution(*img) };
		//Save(sobelYImg.get(), "sobelYImg.png");

		//std::unique_ptr<EDImage> averageImg{ average->ApplyConvolution(*img) };
		//Save(averageImg.get(), "averageImg.png");

		//std::unique_ptr<EDImage> laplacianImg{ laplacian->ApplyConvolution(*img) };
		//Save(laplacianImg.get(), "laplacianImg.png");
		
	/*

		std::unique_ptr<EDImage> bwbwImage{ bw->ApplyConvolution(*bwImg) };

		Save(bwbwImage.get(), "bwbw.png");

	*/



		//std::unique_ptr<EDConvolution> copyPassConv{ EDConvolution::CreateCopyPass() };
		//std::unique_ptr<EDImage> copyPass{ copyPassConv->ApplyConvolution(*img) };

		//Save(copyPass.get(), "copyPass.png");
}


	// (optional) set browser properties
	//fileDialog.SetTitle("title");
	//fileDialog.SetTypeFilters({ ".jpg", ".png", ".jpeg" });

}

Application::~Application() {

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	delete img;
	delete composite;
	delete bw;
}

void Application::Save(EDImage* img, const std::string& path)
{
	if (EDImage::TrySave(*img, path))
	{
		std::cout << "success " << path << " save \n";
	}
	else
	{
		std::cout << "failed " << path << " save \n";
	}
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(window) && false)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();




		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui();
		Render();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

void Application::Render()
{



}

void Application::ImGui()
{
	ImGui::Begin("Convolution Editor");

	if (ImGui::InputInt("Convolution Height", &heightConv))
	{
		heightConv = clamp(7, 1, heightConv);
	}

	if (ImGui::InputInt("Convolution Width", &widthConv))
	{
		widthConv = clamp(7, 1, widthConv);
	}

	if (ImGui::InputInt("Pivot X", &pivotX))
	{
		pivotX = clamp(widthConv-1, 0, pivotX);
	}

	if (ImGui::InputInt("Pivot Y", &pivotY))
	{
		pivotY = clamp(heightConv - 1, 1, pivotY);
	}



	ImGui::Text("Color button with Picker:");
	ImGui::SameLine(); HelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\nWith the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only be used for the tooltip and picker popup.");


	static std::vector<const char *> names = {
		"0,0",
		"1,0",
		"2,0",
		"3,0",
		"4,0",
		"5,0",
		"6,0",
		"0,1",
		"1,1",
		"2,1",
		"3,1",
		"4,1",
		"5,1",
		"6,1",
		"0,2",
		"1,2",
		"2,2",
		"3,2",
		"4,2",
		"5,2",
		"6,2",
		"0,3",
		"1,3",
		"2,3",
		"3,3",
		"4,3",
		"5,3",
		"6,3",
		"0,4",
		"1,4",
		"2,4",
		"3,4",
		"4,4",
		"5,4",
		"6,4",
		"0,5",
		"1,5",
		"2,5",
		"3,5",
		"4,5",
		"5,5",
		"6,5",
		"0,6",
		"1,6",
		"2,6",
		"3,6",
		"4,6",
		"5,6",
		"6,6",
	};

	for (size_t yy = 0, nn = 0; yy < heightConv; yy++)
	{
		for (size_t xx = 0; xx < widthConv; xx++, nn++)
		{
			ImGui::ColorEdit4(names[nn], (float*)&color[yy][xx], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}

	if (texOGImg)
	{
		//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	//if (ImGui::Button("Save Image"))
	//{
	//	ImGui::SameLine();
	//	//ImGui::InputText("imgFile")
	//}

	//if (ImGui::Button("Load Image"))
	//{
	//	fileDialog.Open();
	//}
	//	fileDialog.Display();


	//if (fileDialog.HasSelected())
	//{
	//	std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
	//	
	//	fileDialog.Close();
	//	fileDialog.ClearSelected();
	//}


	ImGui::End();

}

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
