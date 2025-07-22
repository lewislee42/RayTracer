//
//  BVH.cpp
//  Raytracer
//
//  Created by Lewis lee on 08/07/2025.
//

#include "BVH.h"
#include <iostream>

#define MAX_DEPTH 10

inline Vec3	boundingBoxSize(const BoundingBox &bb) {
	return Vec3{bb.max.x - bb.min.x, bb.max.y - bb.min.y, bb.max.z - bb.min.z};
}

void growBoundingBoxSphere(BoundingBox &bb, const Object3D &obj) {
	bb.min.x = fmin(bb.min.x, obj.center.x - obj.radius);
	bb.min.y = fmin(bb.min.y, obj.center.y - obj.radius);
	bb.min.z = fmin(bb.min.z, obj.center.z - obj.radius);
	
	bb.max.x = fmax(bb.min.x, obj.center.x + obj.radius);
	bb.max.y = fmax(bb.min.y, obj.center.y + obj.radius);
	bb.max.z = fmax(bb.min.z, obj.center.z + obj.radius);
}



void	Split(std::vector<BVHNode> &allNodes, std::vector<Object3D> &allObjects, BVHNode *node, int depth) {
	if (depth == MAX_DEPTH || node->triangleCount < 1) {
		return;
	}

	node->childIndex = static_cast<int>(allNodes.size());
//	std::cout << "node childIndex: " << node->childIndex << std::endl;
	BVHNode childA;
	BVHNode childB;
	childA.triangleIndex = node->triangleIndex;
	childB.triangleIndex = node->triangleIndex;
	childA.depth = depth + 1;
	childB.depth = depth + 1;

	
	for (int i = node->triangleIndex; i < node->triangleIndex + node->triangleCount; i++) {
		BVHNode *child;
		float parentCenterAxis = 1.0f;
		float triangleCenterAxis = 1.0f;
		Vec3 boundsSize = boundingBoxSize(node->boundingBox);
		
		if (boundsSize.x < boundsSize.y) {
			parentCenterAxis = node->boundingBox.center.y;	
			triangleCenterAxis = allObjects[i].center.y;
		}
		if (boundsSize.y < boundsSize.z) {
			parentCenterAxis = node->boundingBox.center.z;	
			triangleCenterAxis = allObjects[i].center.z;	
		}
		if (boundsSize.z < boundsSize.x) {
			parentCenterAxis = node->boundingBox.center.x;	
			triangleCenterAxis = allObjects[i].center.x;
		}

		if (triangleCenterAxis < parentCenterAxis)
			child = &childA;
		else
			child = &childB;

		growBoundingBoxSphere(child->boundingBox, allObjects[i]);
		child->triangleCount++;
		if (triangleCenterAxis < parentCenterAxis) {
			int swap = child->triangleIndex + child->triangleCount - 1;
			Object3D temp = allObjects[i];
			allObjects[i] = allObjects[swap];
			allObjects[swap] = temp;
			childB.triangleIndex++;
		}
	}
	
	allNodes.push_back(childA);
	BVHNode& childARef = allNodes.back();
	allNodes.push_back(childB);
	BVHNode& childBRef = allNodes.back();
	
	Split(allNodes, allObjects, &childARef, depth + 1);

	Split(allNodes, allObjects, &childBRef, depth + 1);
	
}
