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


//the idea behind this object is simple
//generate a random id and save it in a 'pool' to ensure that the id is never reused
//then the id can be used to identify unique objects
#ifndef _IDGENERATOR_H_
#define _IDGENERATOR_H_

#include "vector"

class CIDGenerator
{
public:
	CIDGenerator()
	{
		//create new id
		const wchar_t *id = new const wchar_t;
		
		//16 bit for speed

	}

	void checkPool()
	{
		for(int i=0; i<pool.size(); i++)
		{
		}
	}

	~CIDGenerator()
	{
	}

private:
	std::vector<const wchar_t*> pool;
};


#endif