/////////////////////////////////////////////////////////////////
//  Author: Devon Adams (https://github.com/devonadams)
//  License : GPLv3
//  Language: C++
//  This file serves as the abstracted defines for a camera
/////////////////////////////
#pragma once

#include <glm/glm.hpp>

namespace DAGGer
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }

		// TODO:
		//void SetPerspective();
		//void SetOrthographic();
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

}	//	END namespace DAGGer