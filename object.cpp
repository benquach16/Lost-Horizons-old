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
#include "object.h"

CObject::CObject(const wchar_t *newname)
{
	object_name = newname;
	//ghetto id generation
	//right now it just creates a new num
	stringw id = L"";
	const wchar_t *letters[7];
	letters[0]=L"A";
	letters[1]=L"B";
	letters[2]=L"C";
	letters[3]=L"D";
	letters[4]=L"E";
	letters[5]=L"F";
	letters[6]=L"G";
	for(int i=0; i<6; i++)
	{
		id+=letters[rand()%6];
	}
	id+=L"x";
	id+=rand()%9;

	//have to use a buffer whenever saving aloading cause strings suck
	int size = id.size();
	wchar_t *buffer_id;
	buffer_id = new wchar_t[size+1];
	//re encode the string onto the new space
	for(int n=0;n<size;n++)
	{
		buffer_id[n]=id[n];
	}
	//termination character
	buffer_id[size]='\0';
	object_id = buffer_id;
	//generate unique id
}

CObject::~CObject()
{
}