//
//  BVH.h
//  Raytracer
//
//  Created by Lewis lee on 08/07/2025.
//

#ifndef BVH_H
#define BVH_H

#include "RayTracingSharedStructs.h"
#include <vector>

void	Split(std::vector<BVHNode> &allNodes, std::vector<Object3D> &allObjects, BVHNode *node, int depth = 0);

/* HELPER FUNCTIONS */
void	growBoundingBoxSphere(BoundingBox &bb, const Object3D &obj);

#endif /* BHV_H */
