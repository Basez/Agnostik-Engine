#pragma once

#include "ialogger.hpp"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <comdef.h>		// used for error checking, quite a heavy header so don't include in shared headers

namespace AGN
{
	void logHResultData(HRESULT a_hr)
	{
		_com_error error(a_hr);
		g_log.debug("HRESULT Message: %s", error.ErrorMessage());
		g_log.debug("HRESULT Description: %s", error.Description());
	}
}