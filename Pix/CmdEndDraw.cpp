#include "CmdEndDraw.h"
#include "PrimitivesManager.h"

bool CmdEndDraw::Execute(const std::vector<std::string>& params)
{
	// sned verticles to rawterizer   
	return PrimitivesManager::Get()->EndDraw();
}