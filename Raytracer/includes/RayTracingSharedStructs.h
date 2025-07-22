

#ifndef RAYTRACING_SHARED_STRUCT_H
#define RAYTRACING_SHARED_STRUCT_H

typedef struct Vec3 {
	float x;
    float y;
    float z;
}	Vec3;

typedef struct Vec2 {
    float x;
    float y;
}   Vec2;

// Material
typedef enum MaterialType {
	LAMBERTIAN	    = 0,
	METAL		    = 1,
	DIELECTRIC	    = 2,
    DIFFUSE_LIGHT   = 3
}	MaterialType;

typedef struct Material {
	MaterialType	type;
	Vec3			albedo;
	float			fuzz;
	float			refractionIndex;
}	Material;


// HitRecord
typedef struct HitRecord {
	Vec3			p;
	Vec3			normal;
	struct Material	mat;
	float			t;
    unsigned int	objectId;
	bool			frontFace;
}	HitRecord;


// Object3D
// only supports sphere for now
typedef struct Object3D {
	Vec3			center;
	float			radius;
	Material		mat;
    unsigned int	id;
}	Object3D;


// Interval
typedef struct Interval {
	float min;
	float max;
}	Interval;


// CameraData
typedef struct RaytracingMetaData {
	Vec3			pixel00Loc;
	Vec3			pixelDeltaU;
	Vec3			pixelDeltaV;
	Vec3			center;
	unsigned int	samplePerPixel;
	unsigned int	lightSampleAmount;
	unsigned int	imageW;
	unsigned int	imageH;
	unsigned int	bounces;
}	RaytracingMetaData;

// Ray
typedef struct Ray {
	Vec3	origin;
	Vec3	direction;
}	Ray;


// BVH
typedef struct ObjectsData {
	unsigned int	objectAmount = 0;
	unsigned int	lightAmount = 0;
	unsigned int	nodesCount = 0;
}	ObjectsData;

typedef struct BoundingBox {
	Vec3	min;
	Vec3	max;
	Vec3	center;
}	BoundingBox;

typedef struct BVHNode {
	struct BoundingBox boundingBox;
	int		triangleIndex = 0;
	int		triangleCount = 0;
	int		childIndex = 0;
	int		depth = 0;
}	BVHNode;

#endif /* RAYTRACING_SHARED_STRUCT_HPP */
