/*   Copyright 2009 T. Phillip Johnson

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "CSceneNodeAnimatorScale.h"

namespace irr
{
namespace scene
{

CSceneNodeAnimatorScale::CSceneNodeAnimatorScale(const core::vector3df& startScale, 
									  const core::vector3df& endScale, u32 timeForScaling,
									  bool loop, u32 now)
  : StartScale(startScale),
  EndScale(endScale),
  TimeForScaling(timeForScaling),
  Loop(loop),
  StartTime(now),
  TimeFactor(0, 0, 0),
  ScaleDiff(0, 0, 0)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorScale");
	#endif

	recalculateImmediateValues();
}


void CSceneNodeAnimatorScale::recalculateImmediateValues()
{
	ScaleDiff = EndScale - StartScale;

	TimeFactor = ScaleDiff / static_cast<f32>(TimeForScaling);
}


CSceneNodeAnimatorScale::~CSceneNodeAnimatorScale(void)
{
}


void CSceneNodeAnimatorScale::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node)
		return;

	u32 t = (timeMs-StartTime);

	core::vector3df scale = StartScale;

	if (!Loop && t >= TimeForScaling)
		scale = EndScale;
	else
		scale += static_cast<f32>(fmod(static_cast<f32>(t), static_cast<f32>(TimeForScaling) ) ) * TimeFactor;

	node->setScale(scale);
}

void CSceneNodeAnimatorScale::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* /*options*/)
{
	out->addVector3d("StartScale", StartScale);
	out->addVector3d("EndScale", EndScale);
	out->addInt("TimeForScaling", TimeForScaling);
	out->addBool("Loop", Loop);
}


void CSceneNodeAnimatorScale::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* /*options*/)
{
	StartScale = in->getAttributeAsVector3d("StartScale");
	EndScale = in->getAttributeAsVector3d("EndScale");
	TimeForScaling = in->getAttributeAsInt("TimeForScaling");
	Loop = in->getAttributeAsBool("Loop");

	recalculateImmediateValues();
}


}	//end namespace scene
}	//end namespace irr

