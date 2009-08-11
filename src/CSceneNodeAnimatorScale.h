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


#pragma once

#pragma warning(push, 3)
#include "ISceneNode.h"
#pragma warning(pop)


namespace irr
{
namespace scene
{

class CSceneNodeAnimatorScale :
	public ISceneNodeAnimator
{
	public:

		/// constructor
		CSceneNodeAnimatorScale(const core::vector3df& startScale, 
									  const core::vector3df& endScale, u32 timeForScaling,
									  bool loop, u32 now);

		/// destructor
		virtual ~CSceneNodeAnimatorScale();

		/// animates a scene node
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		/// Writes attributes of the scene node animator.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);

		/// Reads attributes of the scene node animator.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		/// Creates a clone of the scene node animator
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

		/// Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType()	{ return ESNAT_UNKNOWN; }

	private:

		void recalculateImmediateValues();

		core::vector3df StartScale;
		core::vector3df EndScale;
		core::vector3df ScaleDiff;
		core::vector3df TimeFactor;
		u32 StartTime;
		u32 TimeForScaling;
		bool Loop;
};

}	//end namespace scene
}	//end namespace irr
