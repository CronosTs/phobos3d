/*
Phobos3d
March 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef RENDER_H
#define RENDER_H

#include <Phobos/Shell/Command.h>
#include <Phobos/Shell/Variable.h>
#include <Phobos/Engine/Module.h>
#include <Phobos/Singleton.h>
#include <Phobos/System/Window.h>

PH_DECLARE_SINGLETON_PTR(Render);

class Render: public Phobos::Engine::Module
{
	PH_DECLARE_SINGLETON_METHODS(Render);

	public:		

	protected:		
		Render();

		void OnPreInit() override;
		void OnInit() override;		

	private:
		void CmdScreenshot(const Phobos::Shell::StringVector_t &args, Phobos::Shell::Context &);

	private:
		// =====================================================
		// PRIVATE ATTRIBUTES
		// =====================================================		
		Phobos::System::WindowPtr_t m_ipWindow;

		Phobos::Shell::Variable m_varRScreenX;
		Phobos::Shell::Variable m_varRScreenY;
		Phobos::Shell::Variable m_varRVSync;
		Phobos::Shell::Variable m_varRFullScreen;

		Phobos::Shell::Command m_cmdScreenshot;
};

#endif
