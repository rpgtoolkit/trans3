Engine
======
* Available Build Configurations
  * Debug
  * Optimised
  
* Microsoft DirectX Feb2010 SDK
  * Rewriting the Gfx engine isn't a priority right now
  * http://www.microsoft.com/en-us/download/details.aspx?id=10084
  * You will have a system variable called "DXSDK_DIR". Project already references this path

* Windows Driver Kit 7.1.0
  * For ATL and MFC. Porting everything to CLR is not a priority.
  * http://www.microsoft.com/en-us/download/details.aspx?id=11800
  * Follow the installation procedure. However, you will have to create the system variable yourself. Name the variable "WINDDK_DIR".
  * Check the projects and resource files and make sure the dependencies are properly set:
  
  * Includes
    * $(WINDDK_DIR)inc\atl71
    * $(WINDDK_DIR)inc\mfc42
    
  * Libraries
    * $(WINDDK_DIR)lib\ATL\i386
    * $(WINDDK_DIR)inc\win7\i386 --> instead of "win7" use "wxp" for Windows XP
    * $(WINDDK_DIR)lib\Mfc\i386
    
* Testing/ Debugging
  * FreeImage has been recompiled for VS2008. Get the DLL at "(trunk)\vc\tkCommon\images" and overwrite the one that came in with the TK3 package.
  * A VS2008 version of Audiere was linked by the project. Get the DLL at "(trunk)\vc\trans3\audio" and overwrite the one that came in with the TK3 package.
  * Configure "Working Directory" to a proper RPGToolkit install path, complete with all dependencies

For any questions contact me:
- "swordmaster" at rpgtoolkit.net