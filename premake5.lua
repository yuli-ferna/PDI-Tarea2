outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir = {}
IncludeDir["glad"] = "dependencies/glad/include"
IncludeDir["glm"] = "dependencies/glm/"
IncludeDir["ImGui"] = "dependencies/imgui/"
IncludeDir["GLFW"] = "dependencies/glfw/include"
IncludeDir["opencv2"] = "dependencies/opencv2"

workspace "App PDI"
disablewarnings { "26495" }
   architecture "x64"
   startproject("App")
   configurations { "Debug", "Release"}

   group "Dependencies"
      include "dependencies/glad"
      include "dependencies/imgui"
      include "dependencies/glfw"
      -- include "dependencies/SDL"	
   group ""

project "App"
   systemversion "latest"
   location("%{prj.name}")
   kind "ConsoleApp"

   language "C++"
   cppdialect "C++17"
   staticruntime "On"
   files { "%{prj.name}/**.h", "%{prj.name}/**.cpp"}

   links {
      "glad",
      "GLFW",
      "ImGui",
      "opencv_world440d.lib"
   }
   libdirs { "dependencies/opencv2/lib" }
   postbuildcommands {
      -- Copy the SDL2 dll to the Bin folder.
      -- '{COPY} "%{path.getabsolute("Contrib/SDL/lib/" .. cfg.architecture:gsub("x86_64", "x64") .. "/SDL2.dll")}" "%{cfg.targetdir}"'
   }

   includedirs{
		"%{prj.name}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.opencv2}",
	}


   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

