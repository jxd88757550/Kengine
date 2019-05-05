--premake5 script
workspace("Kengine")
	configurations{"Debug","Release"}
	platforms{"x64"}

project("Kengine")
   kind("ConsoleApp")
   location("Kengine")
   language ("C++")
   targetdir ("%{prj.name}/bin/%{cfg.buildcfg}")
   objdir ("%{prj.name}/binobj/%{cfg.buildcfg}")

   files { "%{prj.name}/**.h", "%{prj.name}/**.cpp" }

   filter("configurations:Debug")
      defines { "DEBUG" }
      symbols("On")

   filter("configurations:Release")
      defines { "RELEASE" }
      optimize ("On")
	
	filter ("platforms:x64")
	  architecture ("x64")
	  
	includedirs { "%{prj.name}/include" }
	
	filter("system:windows")
		systemversion("latest")
		
		filter ({})

project("Gui")
   kind("ConsoleApp")
   location("Gui")
   language ("C++")
   targetdir ("%{prj.name}/bin/%{cfg.buildcfg}")
   objdir ("%{prj.name}/binobj/%{cfg.buildcfg}")

   files { "%{prj.name}/**.h", "%{prj.name}/**.cpp" }

   filter("configurations:Debug")
      defines { "DEBUG" }
      symbols("On")

   filter("configurations:Release")
      defines { "RELEASE" }
      optimize ("On")
	  
	filter ("platforms:x64")
	  architecture ("x64")
	  
	includedirs { "%{prj.name}/include" }
	
	filter("system:windows")
		systemversion("latest")
		