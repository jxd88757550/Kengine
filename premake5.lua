--premake5 script
require ("premake-qt/qt")

local qt = premake.extensions.qt

local qt_path = "C:/Qt/5.12.3/"
--change local qt path
local compilerversion = "msvc2017_64"
--set this to the folder name that contains the include and lib folders in the qt directory

workspace("Kengine")
	configurations{"Debug","Release"}
	platforms{"x64"}
	startproject ("Gui")

project("Kengine")
   kind("StaticLib")
   location("Kengine")
   language ("C++")
   cppdialect ("C++17")
   staticruntime ("on")
   
   targetdir ("%{prj.name}/bin/%{cfg.buildcfg}")
   objdir ("%{prj.name}/binobj/%{cfg.buildcfg}")

   files { "%{prj.name}/**.h", "%{prj.name}/**.cpp" }
   includedirs { "%{prj.name}/include"}

   filter("configurations:Debug")
      defines { "DEBUG" }
	  runtime("Debug")
      symbols("On")

   filter("configurations:Release")
	  runtime("Release")
      defines { "RELEASE" }
      optimize ("On")
	
	filter ("platforms:x64")
	  architecture ("x64")
	  	
	filter("system:windows")
		systemversion("latest")
		
		filter ({})

project ("Gui")
   kind("ConsoleApp")
   location("Gui")
   language ("C++")
   cppdialect ("C++17")
   staticruntime ("on")
   
   targetdir ("%{prj.name}/bin/%{cfg.buildcfg}")
   objdir ("%{prj.name}/binobj/%{cfg.buildcfg}")
   libdirs{"$(SolutionDir)Kengine/bin/%{cfg.buildcfg}"}
			
   includedirs { "%{prj.name}/include",
   "$(SolutionDir)Kengine/include" }

   files { "%{prj.name}/**.h", "%{prj.name}/**.cpp", 
			"%{prj.name}/**.ui", "%{prj.name}/**.qrc"}

   qt.enable()
   
   qtpath (qt_path .. compilerversion)
   qtmodules { "core", "gui", "widgets" }
   qtprefix ("Qt5")
   qtgenerateddir("Gui/")

   links{"Kengine"}
   
   local cmd = "{COPY} " .. qtpath .. compilerversion .. "/bin/"
   
   filter("configurations:Debug")   
	  postbuildcommands{ 
		(cmd .. "Qt5Cored.dll" .. " bin/%{cfg.buildcfg}"),
		(cmd .. "Qt5Guid.dll" .. " bin/%{cfg.buildcfg}"),
		(cmd .. "Qt5Widgetsd.dll" .. " bin/%{cfg.buildcfg}")
	  }
	  
	  qtsuffix("d")
      defines { "DEBUG" }
	  runtime("Debug")
      symbols("On")

   filter("configurations:Release")
      postbuildcommands{ 
		(cmd .. "Qt5Core.dll" .. " bin/%{cfg.buildcfg}"),
		(cmd .. "Qt5Gui.dll" .. " bin/%{cfg.buildcfg}"),
		(cmd .. "Qt5Widgets.dll" .. " bin/%{cfg.buildcfg}")
	  }

      defines { "RELEASE" }
	  runtime("Release")
      optimize ("On")
	  
	filter ("platforms:x64")
	  architecture ("x64")
	  
	filter("system:windows")
		systemversion("latest")