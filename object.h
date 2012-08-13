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



#ifndef _COBJECT_H_
#define _COBJECT_H_

#include "irrlicht.h"


using namespace irr;
using namespace core;
using namespace scene;


class CObject
{
public:


	CObject(const wchar_t *newname = L"__TEST_SHIP__");

	~CObject();

	const wchar_t *getName()
	{
		return object_name;
	}
	void setName(const wchar_t *newname)
	{
		object_name = newname;
	}
	const wchar_t *getID()
	{
		return object_id;
	}
	void setID(const wchar_t *newid)
	{
		object_id = newid;
	}

private:
	const wchar_t *object_name;
	//needed to save and load stuff
	//legacy////
	const wchar_t *object_id;
};

#endif