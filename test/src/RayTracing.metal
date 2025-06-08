//
//  test.metal
//  HybridRendering
//
//  Created by Lewis lee on 20/04/2025.
//  Copyright © 2025 Apple. All rights reserved.
//

#include <simd/simd.h>
#include <metal_stdlib>
#include "../include/RayTracingStructs.hpp"
#include "../include/Vec3Shader.h"

using namespace metal;

Vec3 emitted(const Material mat) {
    if (mat.type == MaterialType::DIFFUSE_LIGHT)
        return mat.albedo;
    return (Vec3){0.0f, 0.0f, 0.0f};
}

// Lambertian
bool scatterLambertian(const Material mat, const Ray rIn, const HitRecord rec, thread Vec3* attenuation, thread Ray* scattered) {
	Vec3 scatterDirection = rec.normal + randomUnitVector(rIn.direction * scattered->direction); // works now
    if (nearZero(scatterDirection))
        scatterDirection = rec.normal;

    *scattered = (Ray){rec.p, scatterDirection};
    *attenuation = mat.albedo;
    return true;
}

// Metal
bool scatterMetal(const Material mat, const Ray rIn, const HitRecord rec, thread Vec3* attenuation, thread Ray* scattered) {
    Vec3 reflected = reflect(rIn.direction, rec.normal);
	reflected = unitVector(reflected) + (mat.fuzz * randomUnitVector(scattered->direction * scattered->direction)); // works now
    *scattered = (Ray){rec.p, reflected};
    *attenuation = mat.albedo;

    return (dot(scattered->direction, rec.normal) > 0);
}

// Dielectric
float reflectance(float cosine, float refractionIndex) {
    float r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * metal::pow((1 - cosine), 5);
}

bool scatterDielectric(const Material mat, const Ray rIn, const HitRecord rec, thread Vec3* attenuation, thread Ray* scattered) {
    *attenuation = (Vec3){1.0f, 1.0f, 1.0f};
    float ri;
    if (rec.frontFace)
        ri = 1.0f / mat.refractionIndex;
    else
        ri = mat.refractionIndex;

    Vec3 unitDirection = unitVector(rIn.direction);
    float cosTheta = metal::fmin(dot(unitDirection * -1, rec.normal), 1.0f);
    float sinTheta = metal::sqrt(1.0f - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0f;
    Vec3 direction;

    if (cannotRefract || reflectance(cosTheta, ri) > randomFloat(0.0001f, 1.0f, randomVector(0.0f, 1.0f, rec.normal * rIn.origin)))
        direction = reflect(unitDirection, rec.normal);
    else
        direction = refract(unitDirection, rec.normal, ri);
        

    *scattered = (Ray){rec.p, direction};
    return true;
}

bool scatter(const Material mat, const Ray rIn, const HitRecord rec, thread Vec3* attenuation, thread Ray* scattered) {
    switch(mat.type) {
        case MaterialType::LAMBERTIAN:
            return scatterLambertian(mat, rIn, rec, attenuation, scattered);
        case MaterialType::METAL:
            return scatterMetal(mat, rIn, rec, attenuation, scattered);
        case MaterialType::DIELECTRIC:
            return scatterDielectric(mat, rIn, rec, attenuation, scattered);
        case MaterialType::DIFFUSE_LIGHT:
            return false;
        default:
            return true;
    }
}


// Raytracing part
bool hit(const Object3D object, const Ray r, Interval rayT, thread HitRecord* rec) {
    Vec3 oc = object.center - r.origin;
    float a = lengthSquared(r.direction);
    float b = dot(r.direction, oc);
    float c = lengthSquared(oc) - object.radius * object.radius;
    float descriminant = b * b - a * c;

    if (descriminant < 0)
        return false;

    float sqrtD = metal::sqrt(descriminant);
    float root = (b - sqrtD) / a;
    if (!(rayT.min < root && root < rayT.max)) {
        root = (b + sqrtD) / a;
        if (!(rayT.min < root && root < rayT.max)) {
            return false;
        }
    }

    rec->t = root;
    rec->p = at(r.origin, r.direction, rec->t);
    rec->normal = (rec->p - object.center) / object.radius;

    Vec3 outwardNormal = (rec->p - object.center) / object.radius;
    
    rec->frontFace = dot(r.direction, outwardNormal) < 0;
    if (rec->frontFace)
        rec->normal = outwardNormal;
    else
        rec->normal = outwardNormal * -1;

    rec->mat = object.mat;
    rec->objectId = object.id;
    
    return true;
}

// currently only works for sphere
bool gotHit(const Ray r, const Interval rayT, thread HitRecord* rec, device const Object3D* objects, const uint objectAmount) {
    HitRecord    tempRec;
    bool         hitAnything    = false;
    float        closestSoFar   = rayT.max;


    for (uint i = 0; i < objectAmount; i++) {
        Interval tempInterval = (Interval){rayT.min, closestSoFar};
        if (hit(objects[i], r, tempInterval, &tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            *rec = tempRec;
        }
    }

    return hitAnything;
}

Ray getRay(int x, int y, const struct CameraData camData, Vec3 seed) {
    Vec3 offset = sampleSquare(seed);
    Vec3 pixelSample = camData.pixel00Loc
        + ((x + offset.x) * camData.pixelDeltaU)
        + ((y + offset.y) * camData.pixelDeltaV);
    Vec3 rayOrigin = camData.center;
    Vec3 rayDirection = pixelSample - rayOrigin;

    return (Ray){rayOrigin, rayDirection};
}

bool isSameVec(const Vec3 v1, const Vec3 v2) {
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
        return true;
    return false;
}

Vec3 directLighting(const HitRecord currentRecord, int lightSampleAmount, device const Object3D* objects, const uint objectAmount) {
    
    uint i = 0;
    uint j = 0;
    uint sampleAmount = lightSampleAmount;
    Vec3 currentColor;
    Vec3 color = (Vec3){0.0f, 0.0f, 0.0f};
    
    // if it is light source just return its color
    if (currentRecord.mat.type == MaterialType::DIFFUSE_LIGHT)
        return currentRecord.mat.albedo;
    
    Vec3 seed = (Vec3){
				 ((0.62f) * 0.41) * currentRecord.normal.x * currentRecord.t,
				 ((2.15f) * 0.83) * currentRecord.normal.y * currentRecord.t,
				 ((9.18f) * 0.01) * currentRecord.normal.z * currentRecord.t
			 };
    
	// will have to change this later when implementing bvh
	while (objects[i].mat.type == MaterialType::DIFFUSE_LIGHT && i < objectAmount) {
	
		j = 0;
		currentColor = (Vec3){0.0f, 0.0f, 0.0f};
		
		while (j < sampleAmount) {

			 // gets the direction from the light to currentObject
			Vec3 lightCenterToObject = currentRecord.p - objects[i].center;
			seed = randomVector(0, 1, seed);
			
			Vec3 pointOnLightSphere = objects[i].center + randomOnHemisphere(lightCenterToObject, seed) * objects[i].radius;
			Vec3 directionToLight = normalizeVec(pointOnLightSphere - currentRecord.p);
			
			// this checks if there is any other objects in between the current object & the light
			Ray r = (Ray){currentRecord.p, directionToLight};
			Interval interval = (Interval){0.1, INFINITY};
			HitRecord hitR;
			gotHit(r, interval, &hitR, &objects[i], 1); // it shoots a light first to the light to get the t which is more or less the distance from the point to the light surface
			
			float lightDist = hitR.t;
			bool hitsLight = true;
			
			for (uint j = 0; j < objectAmount; j++) {
				if (i != j && objects[j].id != currentRecord.objectId && // so that the current object does not check itself
					objects[j].mat.type != MaterialType::DIELECTRIC && gotHit(r, interval, &hitR, &objects[j], 1)) {
					if (hitR.t < lightDist) {
						hitsLight = false;
						break;  // did not hit the light
					}
				}
			}
			
			if (hitsLight == true) {
				if (!(currentRecord.mat.type == MaterialType::METAL)) {
					float dotProd = dot(currentRecord.normal, directionToLight);
					float distanceSquared = lengthSquared(pointOnLightSphere - currentRecord.p);
					float attenuation = 1.0f / distanceSquared;
		
					currentColor = currentColor + (objects[i].mat.albedo * max(0.0f, dotProd) * attenuation);
				}
			}
			j++;
		}
		color = color + ((currentColor / sampleAmount) * currentRecord.mat.albedo);
		i++;
    }
    return color;
}

Vec3 rayColor(const Ray r, uint currentBounces, int lightSampleAmount, device const Object3D* objects, const uint objectCount) {
    HitRecord   rec;
    Vec3        color               = {1.0f, 1.0f, 1.0f};
    Vec3        incomingLight       = {0.0f, 0.0f, 0.0f};
    Ray         currentRay          = r;
    
    for (uint i = 0; i < currentBounces; i++) {
        if (currentBounces <= 0) { // returns if bounce too many times
            return (Vec3){0.0f, 0.0f, 0.0f};
        }
        
        if (!gotHit(currentRay, (Interval){0.001f, INFINITY}, &rec, objects, objectCount)) { // if it hits nothing then return the sky color
            break; // backgound color for now
        }
        Ray scattered;
        Vec3 attenuation = (Vec3){0.0f, 0.0f, 0.0f};
//		Vec3 colorFromEmission = directLighting(rec, lightSampleAmount, objects, objectCount); // direct lighting
		Vec3 colorFromEmission = emitted(rec.mat);  // indirection lighting
        
        if (!scatter(rec.mat, currentRay, rec, &attenuation, &scattered)) {
            incomingLight = colorFromEmission;
            break ;
        }
        incomingLight = incomingLight + (colorFromEmission * color);
        color = color * attenuation;
        currentRay = scattered;
    }
    return incomingLight;
}


// NOTE: always check if the input is correct or not
kernel void computeMain(
    constant CameraData&    camData                             [[buffer(0)]],
    device const Object3D*  objects                             [[buffer(1)]],
    constant uint&          objectCount                         [[buffer(2)]],
    constant uint&          bounces                             [[buffer(3)]],
    texture2d<float, access::write> outputTexture               [[texture(0)]],
    uint2                   tid                                 [[thread_position_in_grid]]
    ) {
    if (tid.x > outputTexture.get_width() || tid.y > outputTexture.get_height()) {
        return;
    }
        
    Vec3    color           = {0.0f, 0.0f, 0.0f};
	Vec3	seed			= (Vec3){
				(1.72f + tid.x) * (0.1f * camData.frameCount),
				(1.94f + tid.y) * (0.6f * camData.frameCount),
				(1.13f + tid.y + tid.x) * (0.1f * camData.frameCount)
			};

    for (uint i = 0; i < camData.samplePerPixel; i++) {
		seed = randomDirection(seed * (0.001f * (camData.frameCount * i)));
        Ray ray = getRay(
            tid.x,
            tid.y,
            camData,
			seed
        ); // just needed someway to randomize the ray direction

        color = color + rayColor(ray, bounces, camData.lightSampleAmount, objects, objectCount);
    }
    
    color = color / camData.samplePerPixel;
                             
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

