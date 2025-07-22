//
//  ShaderStructs.m
//  Raytracer
//
//  Created by Lewis lee on 07/07/2025.
//
#ifndef RAYTRACING_SHADER_STRUCT_H
#define RAYTRACING_SHADER_STRUCT_H

typedef struct BVHResult {
	float				closestDst;
	Object3D			object;
	device BVHNode		*node;
	int					depth = {0};
}	BVHResult;

#endif /* RAYTRACING_SHADER_STRUCT_H */

