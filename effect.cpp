//////////////////////////////////////////////////////////////////////
///Copyright (C) 2011-2012 Benjamin Quach
//
//This file is part of the "Lost Horizons" video game demo
//
//"Lost Horizons" is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "effect.h"

//Most of these functions are to be replaced by the actual effect functions
//Just sets up the framework
CEffect::CEffect(irr::IrrlichtDevice *graphics ,irrklang::ISoundEngine *sound , core::vector3df& pos)
{
	//End should always start out as false
	setEnd(false);
}
CEffect::~CEffect()
{
}

//Since end is a private variable
void CEffect::setEnd(bool value)
{
	end = value;
}

void CEffect::loop()
{
}

bool CEffect::getEnd()
{
	return end;
}

void CEffect::drop()
{
}