Resampler Engine2

BY MISATO

There is a Alpha Test of UTAU Reader Bridge on NewRenderMode

First,Copy and Replace Files Into UTAU's Install Folder

Then,Checkon the Tools->Option->Rendering->"Use resampler.dll for rendering". or Checkon "Use resampler.dll" in Project Configurations.

After those,you can got a faster reader motion.

All the code is OpenSource with LGPL in https://github.com/MISATOSUSUMI/UtauBridger.You can crossfade yourselves resampler engine into this.

The Plugin EngineSwitcher will help you to change engine and setting a key.


Except the Bridge.you can alse try to rename the engine DLL to resampler.dll or wavtool2.dll.

In this bridge,I try to translate CodePage.

if you are in Chinese lang area or some bigger CodePage in ACP,system will try to change the path directly and make the resampler and wavtool work.
if you are in Japanese lang area or some smaller CodePage in ACP,system will try to change the path to Unicode and build a temppath for it to make the resampler and wavtool work.

Of course,if your UTAU resampler can find your wavefile and work well,the bridge will not do anything.

There is some log for my try:

1.Windows10CHS,setLocate:CHS,CodePage:936,UTAUDefaultDB
  DirectRun and BridgeTranslatePath . Render work well.
  Run with LocateEmulator,Bridge build a temppath,and make the render work well.

2.Windows10CHS,setLocate:JPN,CodePage:936,UTAUDefaultDB
  Run with LocateEmulator,Bridge build a temppath,and make the render work well.


At the last,there is some file list:

  plugins		Directory	EngineSwitcherPlugin
  engines		Directory	SomeExampleEngine And ExeBridge.
  resampler.dll		File		ResamplerBridgeBody.
  resampler.ini		File		ResamplerBridgeSetting.
  resampler2.list.txt	File		The engine list,EngineSwitcherPlugin can create and manage it.
  wavtool2.dll		File		WaveAppenderBridgeBody
  wavtool2.list.txt	File		The appender list,EngineSwitcherPlugin can create and manage it.
 
In Example Engines directory, VS4U and Tn_Fnds is OpenSource recompiled to dll,you can see it and try to recompile yourselves engine and get a higer speed.you can also use Resample_ExeBridge dll can redirect the argument to the dll's same name exefile.and you will get a faster wav append speed(wavtool is a dll and copy faster/8192buffer).

PS:All the program is compiled by VS2012 you must install a VC11 Runtimer or copy the MSVCR110D.dll into the UTAU directory.

											Alpha Information
											  MISATO_SUSUMI
											