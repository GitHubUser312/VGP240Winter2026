//====================================================================================================
// Filename:	WinMain.h
// Created by:	Peter Chan
//====================================================================================================

#include "PixEditor.h"
#include "Viewport.h"
#include "Clipper.h"

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	PixEditor editor;

	X::Start("xconfig.json");
	editor.Initialize();
	
	// Set viewport here
	Viewport::Get()->SetViewport(100, 100, 400, 300);
	Viewport::Get()->DrawViewport();

	Clipper::Get()->SetClipping(true);


	X::Run([&](float deltaTime)
	{
		return editor.Run(deltaTime);
	});

	editor.Terminate();
	X::Stop();
	return 0;
}