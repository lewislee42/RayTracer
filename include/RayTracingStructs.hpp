

#ifndef RAYTRACINGSTRUCT_HPP
# define RAYTRACINGSTRUCT_HPP


typedef struct Vec3 {
	float x;
    float y;
    float z;
}	Vec3;

// Material
typedef enum MaterialType {
	LAMBERTIAN	= 0,
	METAL		= 1,
	DIELECTRIC	= 2
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
	bool			frontFace;
}	HitRecord;


// Object3D
// only supports sphere for now
typedef struct Object3D {
	Vec3			center;
	float			radius;
	Material	mat;
}	Object3D;


// Interval
typedef struct Interval {
	float min;
	float max;
}	Interval;


// CameraData
typedef struct CameraData {
	Vec3	pixel00Loc;
	Vec3	pixelDeltaU;
	Vec3	pixelDeltaV;
	Vec3	center;
	int		imageW;
	int		imageH;
}	CameraData;


// Ray
typedef struct Ray {
	Vec3	origin;
	Vec3	direction;
}	Ray;


#endif /* RAYTRACINGSTRUCT_HPP */
