#include "Application.h"


extern Shader* bwShader;

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
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	if (EDImage::TryLoad("../chikis.jpg", img))
	{
		std::cout << "img loaded successfully\n";
	}
	else
	{
		std::cout << "img load fail\n";
		return;
	}

	//std::string pathNegative = "negative.png";
	//EDImage::TrySave(negative.get(), pathNegative, img->GetWidth(), img->GetHeight(), img->GetNChannels());
	//
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

	//delete img;
	//delete composite;
	//delete bw;
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
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui();

		// Rendering
		ImGui::Render();
		Render();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

void Application::Render()
{

	Quad *quad = Quad::Instance();
	if (bwShader) {
		bwShader->use();
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, texId);
		bwShader->setInt("tex", 0);
		quad->Bind();
		quad->Draw();

	}
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

	if (ImGui::Button("recompile"))
	{
		delete bwShader;



		std::string vert = Shader::GetSrcFromFile("bw.vert");
		//std::string frag = init + end;
		std::string init("#version 330 core\n"
			"in vec2 fragPos;\n"
			"uniform sampler2D tex;\n"
			"uniform int mode = 1;\n"
			"uniform float imgWidth;\n"
			"uniform float imgHeight;\n"
			"out vec4 fragColor;\n"

			"void main(){\n"
			"vec2 actPos = (fragPos.xy+1)/2.f;\n"
			"vec3 texColor = texture(tex, actPos).rgb;\n"
			"vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);\n"

			"	\n"
			"int mode = 2;\n"
			"if(mode == 0){\n"
			"texColor = 1.f - texColor;\n"
			"}else if(mode == 1){\n"
			"vec3 grey = vec3(0.2125f, 0.7154f, 0.0721f);\n"
			"float g = dot(texColor, grey);\n"
			"texColor = vec3(g);\n"
			"}\n"
			"else if (mode == 2){\n"
			"vec3 avg = vec3(0);\n"
			"\n"
			"\n"

			"vec2 uAcum = vec2(0);\n"

			"int width = 3;\n"
			"int height = 3;\n"
			"		 \n"
			"float conv[9] = float[](\n"
			"0.1f, 0.1f, 0.1f,\n"
			"0.1f, 0.1f, 0.1f,\n"
			"0.1f, 0.1f, 0.1f\n"
			");\n"
			"\n"

			"//		float conv[9] = float[](\n"
			"//			-1.f, 0, 1.f,\n"
			"//			-1.f, 0, 1.f,\n"
			"//			-1.f, 0, 1.f\n"
			"//		);\n"
			"\n"
		);
std::string end(
		"int convI = 0;\n"
		"for(int yy = 0; yy < height; yy++, uAcum.y += d.y){\n"
			"for(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){\n"
				"vec2 nUv = actPos + uAcum;\n"
				"avg+= texture(tex, nUv).rgb * conv[convI];\n"
			"}\n"
		"}\n"

"//		avg = texture(tex, actPos ).rgb;\n"
		"texColor = avg;\n"
	"}\n"
"\n"
"\n"
"\n"

	"fragColor = vec4(texColor,1);\n"
			"}\n");

		std::string frag = init + end;
		bwShader = Shader::FromString(vert.c_str(), frag.c_str());
		std::cout << "recompiled" << std::endl;

		std::unique_ptr<RawData> negative{ EDNegativeHA(img->data, img->GetWidth(), img->GetHeight()) };
		texId = GetTexture(negative.get(), img->GetWidth(), img->GetHeight());


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
