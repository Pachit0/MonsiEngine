#pragma once

namespace Monsi {

	class MeshInvalidationTracker
	{
	public:
		static void MarkDirty()
		{
			s_Dirty = true;
		}

		static void ReleaseDirty()
		{
			s_Dirty = false;
		}

		static bool GetState()
		{
			return s_Dirty;
		}

	private:
		static bool s_Dirty;
	};

}