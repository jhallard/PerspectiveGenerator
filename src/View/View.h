#ifndef VIEW_H_
#define VIEW_H_

#include "../Helper/MathHelp.h"

namespace View
{

	void buildProjectionMatrix(float fov, float ratio, float nearp, float farp);

	void setCamera(float posX, float posY, float posZ,float lookAtX, float lookAtY, float lookAtZ);

	void get_bounding_box_for_node (const aiNode* nd, aiVector3D* min, aiVector3D* max);

	void get_bounding_box (aiVector3D* min, aiVector3D* max);

} // end namespace View


#endif