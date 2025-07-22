//
//  test.metal
//  HybridRendering
//
//  Created by Lewis lee on 20/04/2025.
//  Copyright © 2025 Apple. All rights reserved.
//

#include <simd/simd.h>
#include <metal_stdlib>
#include "../includes/RayTracingSharedStructs.h"
#include "../includes/Vec3ShaderHeader.metal"

using namespace metal;

Vec3 emitted(const Material mat) {
    if (mat.type == MaterialType::DIFFUSE_LIGHT)
        return mat.albedo;
    return (Vec3){0.0f, 0.0f, 0.0f};
}

// Lambertian
bool scatterLambertian(
	const Material	mat,
	const Ray		rIn,
	const HitRecord	rec,
	thread Vec3*	attenuation,
	thread Ray*		scattered,
	thread Loki&	loki
) {
	Vec3 scatterDirection = rec.normal + randomUnitVector(loki); // works now
    if (nearZero(scatterDirection))
        scatterDirection = rec.normal;

    *scattered		= (Ray){rec.p, scatterDirection};
    *attenuation	= mat.albedo;
    return true;
}

// Metal
bool scatterMetal(
	const Material	mat,
	const Ray		rIn,
	const HitRecord	rec,
	thread Vec3*	attenuation,
	thread Ray*		scattered,
	thread Loki&	loki
) {
    Vec3 reflected	= reflect(rIn.direction, rec.normal);
	reflected		= unitVector(reflected) + (mat.fuzz * randomDirection(loki)); // works now
    *scattered		= (Ray){rec.p, reflected};
    *attenuation	= mat.albedo;

    return (dot(scattered->direction, rec.normal) > 0);
}

// Dielectric
float reflectance(float cosine, float refractionIndex) {
    float r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * metal::pow((1 - cosine), 5);
}

bool scatterDielectric(
	const Material	mat,
	const Ray		rIn,
	const HitRecord	rec,
	thread Vec3*	attenuation,
	thread Ray*		scattered,
	thread Loki&	loki
) {
    *attenuation = (Vec3){1.0f, 1.0f, 1.0f};
    float ri;
    if (rec.frontFace)
        ri = 1.0f / mat.refractionIndex;
    else
        ri = mat.refractionIndex;

    Vec3 unitDirection	= unitVector(rIn.direction);
    float cosTheta		= metal::fmin(dot(unitDirection * -1, rec.normal), 1.0f);
    float sinTheta		= metal::sqrt(1.0f - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0f;
    Vec3 direction;

    if (cannotRefract || reflectance(cosTheta, ri) > randomFloat(0.0f, 1.0f, loki))
        direction = reflect(unitDirection, rec.normal);
    else
        direction = refract(unitDirection, rec.normal, ri);
        

    *scattered = (Ray){rec.p, direction};
    return true;
}

bool scatter(
	const Material	mat,
	const Ray		rIn,
	const HitRecord	rec,
	thread Vec3*	attenuation,
	thread Ray*		scattered,
	thread Loki&	loki
) {
    switch(mat.type) {
        case MaterialType::LAMBERTIAN:
            return scatterLambertian(mat, rIn, rec, attenuation, scattered, loki);
        case MaterialType::METAL:
            return scatterMetal(mat, rIn, rec, attenuation, scattered, loki);
        case MaterialType::DIELECTRIC:
            return scatterDielectric(mat, rIn, rec, attenuation, scattered, loki);
        case MaterialType::DIFFUSE_LIGHT:
            return false;
        default:
            return true;
    }
}


// Raytracing part
bool hit(const Object3D object, const Ray r, Interval rayT, thread HitRecord* rec) {
    Vec3 oc				= object.center - r.origin;
    float a				= lengthSquared(r.direction);
    float b				= dot(r.direction, oc);
    float c				= lengthSquared(oc) - object.radius * object.radius;
    float descriminant	= b * b - a * c;

    if (descriminant < 0)
        return false;

    float sqrtD	= metal::sqrt(descriminant);
    float root	= (b - sqrtD) / a;
    if (!(rayT.min < root && root < rayT.max)) {
        root	= (b + sqrtD) / a;
        if (!(rayT.min < root && root < rayT.max)) {
            return false;
        }
    }

    rec->t		= root;
    rec->p		= at(r.origin, r.direction, rec->t);
    rec->normal	= (rec->p - object.center) / object.radius;

    Vec3 outwardNormal = (rec->p - object.center) / object.radius;
    
    rec->frontFace = dot(r.direction, outwardNormal) < 0;
    if (rec->frontFace)
        rec->normal	= outwardNormal;
    else
        rec->normal	= outwardNormal * -1;

    rec->mat		= object.mat;
    rec->objectId	= object.id;
    
    return true;
}

bool rayBoxIntersection(const BoundingBox bbox, const Ray ray, thread Interval& rayT) {
	float invD	= 0.0f;
	float t0	= 0.0f;
	float t1	= 0.0f;
	
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			invD = 1.0f / ray.direction.x;
			t0 = (bbox.min.x - ray.origin.x) * invD;
			t1 = (bbox.max.x - ray.origin.x) * invD;
		}
		else if (i == 1) {
			invD = 1.0f / ray.direction.y;
			t0 = (bbox.min.y - ray.origin.y) * invD;
			t1 = (bbox.max.y - ray.origin.y) * invD;
		}
		else {
			invD = 1.0f / ray.direction.z;
			t0 = (bbox.min.z - ray.origin.z) * invD;
			t1 = (bbox.max.z - ray.origin.z) * invD;
		}
		
		if (invD < 0.0f) {
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}
		
		rayT.min = metal::fmax(rayT.min, t0);
		rayT.max = metal::fmin(rayT.max, t1);
		
		if (rayT.max < rayT.min)
			return false;
	}
	return true;
}

// currently only works for sphere
bool gotHit(
	const Ray				r,
	const Interval			rayT,
	thread HitRecord*		rec,
	const ObjectsData		objectsData,
	device const Object3D*	objects,
	device const BVHNode*	nodes
) {
//    HitRecord    tempRec;
//    bool         hitAnything    = false;
//    float        closestSoFar   = rayT.max;


//    for (uint i = 0; i < objectsData.objectAmount; i++) {
//        Interval tempInterval = (Interval){rayT.min, closestSoFar};
//        if (hit(objects[i], r, tempInterval, &tempRec)) {
//            hitAnything = true;
//            closestSoFar = tempRec.t;
//            *rec = tempRec;
//        }
//    }
	
	
	int nodeStack[32];
	HitRecord tempRec;
	int stackIndex		= 0;
	bool hitAnything	= false;
	float closestDist	= rayT.max;
	
	nodeStack[stackIndex++] = 0;
	
	while (stackIndex > 0) {
		int						nodeIndex	= nodeStack[stackIndex--];
		const device BVHNode&	currentNode	= nodes[nodeIndex];
		
		if (currentNode.childIndex == 0) {
			for (int i = currentNode.triangleIndex; i < currentNode.triangleIndex + currentNode.triangleCount; i++) {
				Interval tempInterval = (Interval){rayT.min, closestDist};
				if (hit(objects[i], r, tempInterval, &tempRec)) {
					hitAnything	= true;
					closestDist	= tempRec.t;
					*rec		= tempRec;
				}
			}
		}
		else {
			int childAIndex = currentNode.childIndex;
			int childBIndex = currentNode.childIndex + 1;
			
			const device BoundingBox& childABbox = nodes[childAIndex].boundingBox;
			const device BoundingBox& childBBbox = nodes[childBIndex].boundingBox;
			
			Interval childADistance = (Interval){rayT.min, closestDist};
			Interval childBDistance = (Interval){rayT.min, closestDist};
			rayBoxIntersection(childABbox, r, childADistance);
			rayBoxIntersection(childBBbox, r, childBDistance);
			
			if (childADistance.min < childBDistance.min) {
				if (childBDistance.min < closestDist)
					nodeStack[stackIndex++] = childBIndex;
				if (childADistance.min < closestDist)
					nodeStack[stackIndex++] = childAIndex;
			}
			else {
				if (childADistance.min < closestDist)
					nodeStack[stackIndex++] = childAIndex;
				if (childBDistance.min < closestDist)
					nodeStack[stackIndex++] = childBIndex;
			}
		}
	}

    return hitAnything;
}

Ray getRay(int x, int y, const RaytracingMetaData metaData, thread Loki& loki) {
    Vec3 offset			= sampleSquare(loki);
    Vec3 pixelSample	= metaData.pixel00Loc + ((x + offset.x) * metaData.pixelDeltaU) + ((y + offset.y) * metaData.pixelDeltaV);
    Vec3 rayOrigin		= metaData.center;
    Vec3 rayDirection	= pixelSample - rayOrigin;

    return (Ray){rayOrigin, rayDirection};
}

Vec3 directLighting(
	const HitRecord			currentRecord,
	const unsigned int		lightSampleAmount,
	const ObjectsData		objectsData,
	device const Object3D*	objects,
	device const BVHNode*	nodes,
	thread Loki&			loki
) {
    uint lightFound			= 0;
    Vec3 currentColor;
    Vec3 color				= (Vec3){0.0f, 0.0f, 0.0f};
	Interval interval		= (Interval){0.1f, INFINITY};
	float lightSampleWeight = 1.0f / lightSampleAmount;

    
    // if it is light source just return its color
    if (currentRecord.mat.type == MaterialType::DIFFUSE_LIGHT)
        return currentRecord.mat.albedo;
    
	for (unsigned int i = 0; i < objectsData.objectAmount && lightFound <= objectsData.lightAmount; i++) {
		
		if (objects[i].mat.type != MaterialType::DIFFUSE_LIGHT)
			continue;
	
		lightFound++;
		currentColor = (Vec3){0.0f, 0.0f, 0.0f};
		
		for (unsigned int j = 0; j < lightSampleAmount; j++) {

			// gets the direction from the light to currentObject
			Vec3 lightCenterToObject	= currentRecord.p - objects[i].center;
			Vec3 pointOnLightSphere		= objects[i].center + randomOnHemisphere(lightCenterToObject, loki) * objects[i].radius;
			Vec3 directionToLight		= normalizeVec(pointOnLightSphere - currentRecord.p);
			
			if (dot(currentRecord.normal, directionToLight) < 0.0f)
				break;
			
			// this checks if there is any other objects in between the current object & the light
			Ray r				= (Ray){currentRecord.p, directionToLight};
			HitRecord hitR;
			
			
			if (gotHit(r, interval, &hitR, objectsData, objects, nodes) == true && hitR.objectId == objects[i].id && hitR.mat.type != MaterialType::DIELECTRIC) {
				if (!(currentRecord.mat.type == MaterialType::METAL)) {
					float dotProd			= dot(currentRecord.normal, directionToLight);
					float distanceSquared	= lengthSquared(pointOnLightSphere - currentRecord.p);
					float attenuation		= 1.0f / distanceSquared;
		
					currentColor = currentColor + (objects[i].mat.albedo * max(0.0f, dotProd) * attenuation);
				}
			}
		}
		color = color + ((currentColor * lightSampleWeight) * currentRecord.mat.albedo);
    }
    return color;
}


Vec3 rayColor(
	const Ray						r,
	constant RaytracingMetaData&	metaData,
	constant ObjectsData&			objectsData,
	device const Object3D*			objects,
	device const BVHNode*			nodes,
	thread Loki&					loki
) {
    HitRecord   rec;
    Vec3        color               = {1.0f, 1.0f, 1.0f};
    Vec3        incomingLight       = {0.0f, 0.0f, 0.0f};
    Ray         currentRay          = r;
    
    for (uint i = 0; i < metaData.bounces; i++) {
        if (!gotHit(currentRay, (Interval){0.001f, INFINITY}, &rec, objectsData, objects, nodes)) // if it hits nothing then return the sky color
            break;
        
        Ray scattered;
        Vec3 attenuation = (Vec3){0.0f, 0.0f, 0.0f};
//		Vec3 colorFromEmission = emitted(rec.mat); // indirect lighting
		Vec3 colorFromEmission = directLighting(rec, metaData.lightSampleAmount, objectsData, objects, nodes, loki); // direct lighting
        
        if (!scatter(rec.mat, currentRay, rec, &attenuation, &scattered, loki)) {
			return colorFromEmission * color;
        }
        
        incomingLight	= incomingLight + (colorFromEmission * color);
		color			= color * attenuation;
        currentRay		= scattered;
    }
    return incomingLight;
}



// NOTE: always check if the input is correct or not
kernel void computeMain(
    constant RaytracingMetaData&	metaData							[[buffer(0)]],
	device const Object3D*			objects								[[buffer(1)]],
	device const BVHNode*			bvhNodes							[[buffer(2)]],
	constant ObjectsData&  			objectsData							[[buffer(3)]],
    texture2d<float, access::write> outputTexture						[[texture(0)]],
    uint2							tid									[[thread_position_in_grid]]
) {
    if (tid.x > outputTexture.get_width() || tid.y > outputTexture.get_height())	// stop if the tid is out of range
        return;
        
    Vec3    color           = {0.0f, 0.0f, 0.0f};
	Loki	loki			= Loki(
		static_cast<unsigned int>(tid.x * 0.4f) + static_cast<unsigned int>(tid.y * 0.71f) + static_cast<unsigned int>(1.53f),
		tid.x,
		tid.y
	);
	
    for (uint i = 0; i < metaData.samplePerPixel; i++) {
		Ray ray = getRay(tid.x, tid.y, metaData, loki);

		color = color + rayColor(ray, metaData, objectsData, objects, bvhNodes, loki);
    }
    
	float weight = 1.0f / metaData.samplePerPixel;
    color = color * weight;
                             
    color.x = linearToGamma(color.x);
    color.y = linearToGamma(color.y);
    color.z = linearToGamma(color.z);
    
    color.x = clamp(color.x, 0.0f, 1.0f);
    color.y = clamp(color.y, 0.0f, 1.0f);
    color.z = clamp(color.z, 0.0f, 1.0f);

    float4 outColor = float4(color.x, color.y, color.z, 1.0f);
    outputTexture.write(outColor, tid);
    return ;
}

