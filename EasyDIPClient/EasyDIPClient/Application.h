#include "EDClientpch.h"


#include "EasyDIPAPI/EDImage.h"
#include "EasyDIPAPI/EDConvolution.h"
#include "EasyDIPAPI/EDCompositeConv.h"



#ifndef __ED_CLIENT_APPLICATION__
#define __ED_CLIENT_APPLICATION__



class Application
{
	//template <typename TT> using ptr = std::shared_ptr<TT>;
	ImGui::FileBrowser fileDialog;

	EDImage *img;
	EDCompositeConv* composite;
	EDConvolution* bw, grey;

	unsigned int texOGImg = 0;
	int widthConv = 1;
	int heightConv = 1;
	float color[7][7][4];
	int pivotX = 0, pivotY = 0;
public:
	Application();
	~Application();
	void Save(EDImage* img, const std::string& path);

	void Render()
	{



	}
	void ImGui();
	
	static void HelpMarker(const char* desc);

};

#endif // !1


