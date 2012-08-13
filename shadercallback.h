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

#ifndef SHADERCALLBACK_H_
#define SHADERCALLBACK_H_

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace video;

class ShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

	float fLightStrength[4];
	vector3df fvLightPosition[4]; //the light positions
	SColorf fvLightColor[4];
	SColorf fvAmbient;     
	float fSpecularPower;   // S=pow(fDiffuseIntensity,fSpecularPower)*fSpecularStrength;
	float fSpecularStrength;// will result in less glossing surfaces
	float fBumpStrength;    //strength of the bumpmapping.. higher values will result in more "bumpy" surfaces
	bool shader_enabled;	//if shader is enabled


	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{ 
		core::matrix4 invWorld = services->getVideoDriver()->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();
		services->setVertexShaderConstant("matWorldInverse", (float*)(&invWorld), 16);

		matrix4 ModelViewProjectionMatrix;
		ModelViewProjectionMatrix = services->getVideoDriver()->getTransform(ETS_PROJECTION);
		ModelViewProjectionMatrix *= services->getVideoDriver()->getTransform(ETS_VIEW);
		ModelViewProjectionMatrix *= services->getVideoDriver()->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("ModelViewProjectionMatrix", ModelViewProjectionMatrix.pointer(), 16);

		matrix4 ModelViewMatrix;
		ModelViewMatrix = services->getVideoDriver()->getTransform(ETS_VIEW);
		ModelViewMatrix *= services->getVideoDriver()->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("ModelViewMatrix", ModelViewMatrix.pointer(), 16);

		services->setVertexShaderConstant("fLightStrength1", (float*)(&fLightStrength[0]), 1);
		services->setVertexShaderConstant("fLightStrength2", (float*)(&fLightStrength[1]), 1);
		services->setVertexShaderConstant("fLightStrength3", (float*)(&fLightStrength[2]), 1);
		services->setVertexShaderConstant("fLightStrength4", (float*)(&fLightStrength[3]), 1);

		services->setVertexShaderConstant("fvLightPosition1", (float*)(&fvLightPosition[0]), 3);
		services->setVertexShaderConstant("fvLightPosition2", (float*)(&fvLightPosition[1]), 3);
		services->setVertexShaderConstant("fvLightPosition3", (float*)(&fvLightPosition[2]), 3);
		services->setVertexShaderConstant("fvLightPosition4", (float*)(&fvLightPosition[3]), 3);

		services->setPixelShaderConstant("fvAmbient", (float*)(&fvAmbient), 4);
		services->setPixelShaderConstant("fvLight1Color", (float*)(&fvLightColor[0]), 4);
		services->setPixelShaderConstant("fvLight2Color", (float*)(&fvLightColor[1]), 4);
		services->setPixelShaderConstant("fvLight3Color", (float*)(&fvLightColor[2]), 4);
		services->setPixelShaderConstant("fvLight4Color", (float*)(&fvLightColor[3]), 4);

		services->setPixelShaderConstant("fSpecularPower", (float*)(&fSpecularPower), 1);
		services->setPixelShaderConstant("fSpecularStrength", (float*)(&fSpecularStrength), 1);
		services->setPixelShaderConstant("fBumpStrength", (float*)(&fBumpStrength), 1);
		
		
	}

	virtual void enableHighQuality(bool quality)
	{
		shader_enabled = quality;
	}
};


#endif