//
//  Vec3Shader.h
//  test
//
//  Created by Lewis lee on 02/05/2025.
//

#ifndef VECSHADER_HPP
# define VECSHADER_HPP

inline Vec3 operator+(const Vec3 u, const Vec3 v) {
    return (Vec3){u.x + v.x, u.y + v.y, u.z + v.z};
}

inline Vec3 operator-(const Vec3 u, const Vec3 v) {
    return (Vec3){u.x - v.x, u.y - v.y, u.z - v.z};
}

inline Vec3 operator*(const Vec3 u, const Vec3 v) {
    return (Vec3){u.x * v.x, u.y * v.y, u.z * v.z};
}

inline Vec3 operator+(const Vec3 v, const float n) {
    return (Vec3){n * v.x, n * v.y, n * v.z};
}

inline Vec3 operator*(float t, const Vec3 v) {
    return (Vec3){t*v.x, t*v.y, t*v.z};
}

inline Vec3 operator*(const Vec3 v, float t) {
    return t * v;
}

inline Vec3 operator/(const Vec3 v, float t) {
    return (1/t) * v;
}

inline float dot(const Vec3 u, const Vec3 v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline float dot(const Vec2 u, const Vec2 v) {
    return u.x * v.x
         + u.y * v.y;
}

inline Vec3 at(Vec3 o, Vec3 direction, float t) {
    return o + t * direction;
}

inline float lengthSquared(const Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline float length(const Vec3 v) {
    return metal::sqrt(lengthSquared(v));
}

inline Vec3 unitVector(const Vec3 v) {
    return v / length(v);
}

inline unsigned TausStep(const unsigned z, const int s1, const int s2, const int s3, const unsigned M) {
    unsigned b=(((z << s1) ^ z) >> s2);
    return (((z & M) << s3) ^ b);
}

inline Vec3 normalizeVec(Vec3 v) {
    float len = length(v);
    return (Vec3){v.x / len, v.y / len, v.z / len};
}

inline float randomFloat(float min, float max, Vec3 v) {
    unsigned seed = v.x * 1099087573UL;
    unsigned seedb = (v.y) * 1099087573UL;
    unsigned seedc = (v.z) * 1099087573UL;
    unsigned seedd = (v.x + v.y) * 1099087573UL;
    unsigned seede = (v.y + v.z) * 1099087573UL;
    unsigned z1 = TausStep(seed,13,19,12,429496729UL);
    unsigned z2 = TausStep(seed,2,25,4,4294967288UL);
    unsigned z3 = TausStep(seed,3,11,17,429496280UL);
    unsigned z4 = (1664525*seed + 1013904223UL);

    unsigned r1 = (z1^z2^z3^z4^seedb);
	z1 = TausStep(r1,13,19,12,429496729UL);
	z2 = TausStep(r1,2,25,4,4294967288UL);
	z3 = TausStep(r1,3,11,17,429496280UL);
	z4 = (1664525*r1 + 1013904223UL);

    unsigned r2 = (z1^z2^z3^z4^seedc);
	z1 = TausStep(r2,13,19,12,429496729UL);
	z2 = TausStep(r2,2,25,4,4294967288UL);
	z3 = TausStep(r2,3,11,17,429496280UL);
	z4 = (1664525*r2 + 1013904223UL);

    unsigned r3 = (z1^z2^z3^z4^seedd);
    z1 = TausStep(r2,13,19,12,429496729UL);
	z2 = TausStep(r2,2,25,4,4294967288UL);
	z3 = TausStep(r2,3,11,17,429496280UL);
	z4 = (1664525*r2 + 1013904223UL);
	
	unsigned r4 = (z1^z2^z3^z4^seede);
    
    return min + (max - min) * ((r1^r2^r3^r4) * 2.3283064365387e-10);
}

inline Vec3 randomVector(float min, float max, Vec3 v) {
    
    if (v.x < 0)
        v.x *= -1;
    if (v.y < 0)
        v.y *= -1;
    if (v.z < 0)
        v.z *= -1;
	
	float minVal = metal::min(v.x, metal::min(v.z, v.y));
	float maxVal = metal::max(v.x, metal::max(v.z, v.y));

    unsigned seeda = (v.x) * 1099087573UL;
    unsigned seedb = (v.y) * 1099087573UL;
    unsigned seedc = (v.z) * 1099087573UL;
    unsigned seedd = (maxVal - minVal) * 1099087573UL;
    
    unsigned z1 = TausStep(seeda,13,19,12,429496729UL);
    unsigned z2 = TausStep(seeda,2,25,4,4294967288UL);
    unsigned z3 = TausStep(seeda,3,11,17,429496280UL);
    unsigned z4 = (1664525*seeda + 1013904223UL);

    unsigned r1 = (z1^z2^z3^z4^seedb);
    z1 = TausStep(r1,13,19,12,429496729UL);
    z2 = TausStep(r1,2,25,4,4294967288UL);
    z3 = TausStep(r1,3,11,17,429496280UL);
    z4 = (1664525*r1 + 1013904223UL);

    unsigned r2 = (z1^z2^z3^z4^seedc);
    z1 = TausStep(r2,13,19,12,429496729UL);
    z2 = TausStep(r2,2,25,4,4294967288UL);
    z3 = TausStep(r2,3,11,17,429496280UL);
    z4 = (1664525*r2 + 1013904223UL);
    
    unsigned r3 = (z1^z2^z3^z4^seedd);

	float fr1 = min + ((max - min) * (r1 * 2.3283064365387e-10));
    float fr2 = min + ((max - min) * (r3 * 2.3283064365387e-10));
    float fr3 = min + ((max - min) * (r2 * 2.3283064365387e-10));

    return (Vec3){fr1, fr2, fr3};
}

inline float randomValueNormalDistribution(Vec3 seed) {
    Vec3 newSeed = randomVector(0, 1, seed);
    Vec3 newSeed1 = randomVector(0, 1, newSeed);


	float theta = 2.0f * M_PI_F * randomFloat(0, 1, newSeed1);
    float rho = metal::sqrt(-2 * metal::log(randomFloat(0, 1, newSeed)));
    return rho * metal::cos(theta);
}

inline Vec3 randomDirection(Vec3 seed) {
    Vec3 newSeed1 = randomVector(0, 1, seed);
    Vec3 newSeed2 = randomVector(0, 1, newSeed1);
	Vec3 newSeed3 = randomVector(0, 1, newSeed2);
    
    float x = randomValueNormalDistribution(newSeed1);
    float y = randomValueNormalDistribution(newSeed2);
    float z = randomValueNormalDistribution(newSeed3);
    return normalizeVec((Vec3){x, y, z});
}

inline Vec3 randomUnitVector(Vec3 v) {
    if (v.x < 0)
        v.x *= -1;
    if (v.y < 0)
        v.y *= -1;
    if (v.z < 0)
        v.z *= -1;
    
    Vec3 seedVec = v;
    while (true) {
        seedVec = randomVector(-1, 1, seedVec);
        float lensq = lengthSquared(seedVec);
        if (0 < lensq && lensq <= 1) {
            return seedVec / metal::sqrt(lensq);
        }
        if (seedVec.x < 0)
            seedVec.x *= -1;
        if (seedVec.y < 0)
            seedVec.y *= -1;
        if (seedVec.z < 0)
            seedVec.z *= -1;
    }
}

inline Vec3 randomOnHemisphere(const Vec3 normal, const Vec3 seed) {
    Vec3 onUnitSphere = randomDirection(seed);
    if (dot(onUnitSphere, normal) > 0.0)
        return onUnitSphere;
    return onUnitSphere * -1;
}

inline bool nearZero(Vec3 v) {
    float s = 1e-8;
    return (metal::fabs(v.x) < s) && (metal::fabs(v.y) < s) && (metal::fabs(v.z) < s);
}

inline Vec3 reflect(const Vec3 v, const Vec3 n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3 uv, const Vec3 n, float etaIOverEtat) {
    float cosTheta = metal::fmin(dot(uv * -1, n), 1.0);

    Vec3 rOutPerp = etaIOverEtat * (uv + cosTheta * n);
    Vec3 rOutPara = -metal::sqrt(metal::fabs(1.0 - lengthSquared(rOutPerp))) * n;
    return rOutPerp + rOutPara;
}

/* Generates a 2d square bounding box
 Note:
 do not pass in a zero value in the seed where it could cause numbers
 generated numbers to be zero
*/
Vec3 sampleSquare(Vec3 seed) {

	Vec3 seed1 = randomVector(0, 1, seed);
//	Vec3 seed2 = randomUnitVector(seed1);
    
    seed1.x -= 0.5f;
    seed1.y -= 0.5f;
    seed1.z = 0;
    return seed1;
}

inline float linearToGamma(float linearComponent) {
    if (linearComponent > 0)
        return metal::sqrt(linearComponent);
    return 0;
}

inline Vec3 randomInUnitDisk(const Vec3 seed) {
    while (1) {
        Vec3 p = randomVector(-1, 1, seed);
        p.z = 0;
        if (length(p) < 1)
            return p;
    }
}

inline Interval intervalExpand(Interval interval, float delta) {
    float padding = delta / 2;
    return (Interval){interval.min - padding, interval.max + padding};
}

inline float intervalSize(float max, float min) {
    return max - min;
}




#endif
