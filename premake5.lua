--premake5 script
workspace("Kengin")
	configurations{"Debug","Release"}
	platforms{"x64"}

project("Kengin")
   kind("ConsoleApp")
   language ("C++")
   targetdir ("%{prj.name}/bin/%{cfg.buildcfg}")
   objdir ("%{prj.name}/binobj/%{cfg.buildcfg}")

   files { "**.h", "**.cpp" }

   filter("configurations:Debug")
      defines { "DEBUG" }
      symbols("On")

   filter("configurations:Release")
      defines { "RELEASE" }
      optimize ("On")
	  
	filter ("platforms:x64")
	  architecture ("x64")
	  
	includedirs { "%{prj.name}/include" }