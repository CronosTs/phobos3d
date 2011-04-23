/*
Phobos 3d
  September 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_ENTITY_H
#define PH_ENTITY_H

#include <PH_Node.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class Dictionary_c;

	PH_DECLARE_NODE_PTR(Entity);

	class PH_GAME_ENGINE_API Entity_c: public Node_c
	{
		public:
			static EntityPtr_t Create(const String_c &name);

			void Load(const Dictionary_c &dictionary);
			void LoadFinished();

			inline const String_c &GetEntityClassName() const;

		protected:
			Entity_c(const String_c &name);

			virtual void OnLoad(const Dictionary_c &dictionary) {};

			virtual void OnLoadFinished() {};

		private:
			String_c strClassName;
	};

	inline const String_c &Entity_c::GetEntityClassName() const
	{
		return strClassName;
	}
}

#endif
