#ifndef make_h

#define make_h

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <FreeImage.h>

using namespace glm;
using namespace std;



bool test(vec3 v0, vec3 v1, vec3 v2, vec3 axis, vec3 u0, vec3 u1, vec3 u2, vec3 e);

class objects {
public:
	vec3 color;
	float specrate;
	float reflectrate;
	float refractrate;
	virtual float intersect(vec3 ray, vec3 start) = 0;
	virtual vec3 getnormal(vec3 p) = 0;
	virtual vec3 gettexture(vec3 pos) = 0;
	virtual bool cellintersect(vec3 starpoint, vec3 unitvec) = 0;
	objects(vec3 color1,float specrate1, float reflectrate1, float refractrate1) : color(color1), specrate(specrate1), reflectrate(reflectrate1), refractrate(refractrate1){}

};

class sphere :public objects{
	
public:
	vec3 center;
	float radius;
	vec3 color;
	FIBITMAP* texture;
	sphere(vec3 center1, float radius1, const char* filename1, vec3 color1, float specrate1, float reflectrate1, float refractrate1) : center(center1), radius(radius1), color(color1), texture(FreeImage_Load(FIF_PNG, filename1, PNG_DEFAULT)), objects(color1, specrate1, reflectrate1, refractrate1) {

	}

	float intersect(vec3 ray, vec3 start) {
		vec3 p = start - center;
		float b = 2 * dot(p, ray);
		float c = dot(p, p) - radius * radius;
		float d = b * b - 4*c;
		if (d < 0.001) {
			return 100000;
		}
		float t1 = (-b - sqrt(d)) / 2;
		float t2 = (-b + sqrt(d)) / 2;

		if (min(t1, t2) < 0.01) {
			return 100000;
		}

		return (t1 < t2) ? t1 : t2;
	}

	vec3 getnormal(vec3 p) {
		return normalize(p - center);
	}

	vec3 gettexture(vec3 pos) {
		if (texture == nullptr) {
			return color;
		}

		float r, g, b;
		vec3 v1 = pos - center;
		vec3 x = vec3(-1.0f, 0.0f, 0.0f);
		vec3 y = vec3(0.0f, 1.0f, 0.0f);
		vec3 z = vec3(0.0f, 0.0f, -1.0f);
		float phi = atan2(length(cross(v1, y)), dot(v1, y));


		float theta = atan2(dot(cross(vec3(0.0f, v1.y,v1.z), z), x), dot(vec3(0.0f, v1.y, v1.z), z));
		theta += 3.141592;
		//cout << theta << endl;
		
		float u = theta / (2 * 3.14159);
		float v = phi / (3.14159);

		//cout << phi << " " << theta << endl;
		//cout << u << " " << v << endl;

		
		u += 0.5;
		if (u > 1)
			u -= 1;

			/*
		v += 0.5;
		if (v > 1)
			v -= 1;
			*/
		FreeImage_Initialise();
		int width = FreeImage_GetWidth(texture);
		int height = FreeImage_GetHeight(texture);
		RGBQUAD ballcolor;
		FreeImage_GetPixelColor(texture, u * width, v * height, &ballcolor);
		r = ballcolor.rgbRed;
		g = ballcolor.rgbGreen;
		b = ballcolor.rgbBlue;
		FreeImage_DeInitialise();
		
		return vec3(r / 255 + color.x, g / 255 + color.y, b / 255 + color.z);
	}

	bool cellintersect(vec3 startpoint, vec3 unitvec) {
		vec3 endpoint = startpoint + unitvec;
		float r = radius * radius;
		if (center.x < startpoint.x) r -= (center.x - startpoint.x) * (center.x - startpoint.x);
		else if (center.x > endpoint.x) r -= (center.x - endpoint.x) * (center.x - endpoint.x);
		if (center.y < startpoint.y) r -= (center.y - startpoint.y) * (center.y - startpoint.y);
		else if (center.y > endpoint.y) r -= (center.y - endpoint.y) * (center.y - endpoint.y);
		if (center.z > startpoint.z) r -= (center.z - startpoint.z) * (center.z - startpoint.z);
		else if (center.z < endpoint.z) r -= (center.z - endpoint.z) * (center.z - endpoint.z);
		return r >= 0;
	}

};

class triangle : public objects {
public:
	vec3 pt1, pt2, pt3;
	vec3 color;
	FIBITMAP* texture;
	int updown;
	triangle(vec3 point1, vec3 point2, vec3 point3,int updown1, const char* filename1, vec3 color1, float specrate1, float reflectrate1, float refractrate1) : pt1(point1), pt2(point2), pt3(point3), updown(updown1), color(color1), texture(FreeImage_Load(FIF_PNG, filename1, PNG_DEFAULT)),  objects(color1, specrate1, reflectrate1, refractrate1) {
	}

	float intersect(vec3 ray, vec3 start) {

		vec3 v1 = pt2 - pt1;
		vec3 v2 = pt3 - pt1;
		vec3 n = normalize(cross(v1, v2));

		float d = -dot(n, pt1);

		float t = -(d + dot(n, start)) / (dot(n, ray));

		if (t < 0.01) {
			return 100000;
		}

		vec3 pt4 = start + t * ray;

		vec3 v3 = pt4 - pt1;
		
		//삼각형 내부외부 확인
		if (dot(cross(v1, v3), cross(v3, v2)) >= 0) {
			v1 = pt1 - pt2;
			v2 = pt3 - pt2;
			v3 = pt4 - pt2;
			if (dot(cross(v1, v3), cross(v3, v2)) >= 0) {
				v1 = pt1 - pt3;
				v2 = pt2 - pt3;
				v3 = pt4 - pt3;
				if (dot(cross(v1, v3), cross(v3, v2)) >= 0) {
					
					return t;
				}
			}
		}

		return 100000;
	}

	vec3 getnormal(vec3 p) {
		vec3 v1 = pt2 - pt1;
		vec3 v2 = pt3 - pt1;
		vec3 n = normalize(cross(v1, v2));
		return n;
	}

	vec3 gettexture(vec3 pos) {

		if (texture == nullptr) {
			return color;
		}

		float r, g, b;

		if (!updown) {//아래부분의 texture (texture coordinate를 대체)
			vec3 p = pos - pt2;
			vec3 v1 = pt1 - pt2;
			vec3 v2 = pt3 - pt2;

			//solve lilnear system
			mat3 m1 = mat3(v1, v2, vec3(1));
			vec2 uv = inverse(m1) * p;
			float x = uv.x;
			float y = uv.y;

			FreeImage_Initialise();
			int width = FreeImage_GetWidth(texture);
			int height = FreeImage_GetHeight(texture);

			RGBQUAD wallcolor;
			FreeImage_GetPixelColor(texture, y * width, x * height, &wallcolor);
			r = wallcolor.rgbRed;
			g = wallcolor.rgbGreen;
			b = wallcolor.rgbBlue;

			FreeImage_DeInitialise();

		}
		else {//위부분의 texture
			vec3 p = pos - pt3;
			vec3 v1 = pt1 - pt3;
			vec3 v2 = pt2 - pt3;


			mat3 m1 = mat3(v1, v2, vec3(1));
			vec2 uv = inverse(m1) * p;
			float x = 1-uv.x;
			float y = 1-uv.y;

			FreeImage_Initialise();

			int width = FreeImage_GetWidth(texture);
			int height = FreeImage_GetHeight(texture);

			RGBQUAD wallcolor;
			FreeImage_GetPixelColor(texture, x * width, y * height, &wallcolor);
			r = wallcolor.rgbRed;
			g = wallcolor.rgbGreen;
			b = wallcolor.rgbBlue;

			FreeImage_DeInitialise();
		}
		return vec3(r / 255 + color.x, g / 255 + color.y, b / 255 + color.z);
	}

	bool cellintersect(vec3 startpoint, vec3 unitvec) {// separating axis theorem
		vec3 boxcenter = startpoint + unitvec * 0.5f;
		vec3 extent = unitvec * 0.5f;

		vec3 v0 = pt1 - boxcenter;
		vec3 v1 = pt2 - boxcenter;
		vec3 v2 = pt3 - boxcenter;
		
		vec3 f0 = v1 - v0;
		vec3 f1 = v2 - v1;
		vec3 f2 = v0 - v2;

		vec3 u0(1.0f, 0.0f, 0.0f);
		vec3 u1(0.0f, 1.0f, 0.0f);
		vec3 u2(0.0f, 0.0f, 1.0f);

		vec3 axis1 = cross(u0, f0);
		vec3 axis2 = cross(u0, f1);
		vec3 axis3 = cross(u0, f2);

		vec3 axis4 = cross(u1, f0);
		vec3 axis5 = cross(u1, f1);
		vec3 axis6 = cross(u1, f2);

		vec3 axis7 = cross(u2, f0);
		vec3 axis8 = cross(u2, f1);
		vec3 axis9 = cross(u2, f2);

		vec3 axis10 = u0;
		vec3 axis11 = u1;
		vec3 axis12 = u2;

		vec3 axis13 = cross(f0, f1);

		if (test(v0, v1, v2, axis1, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis2, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis3, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis4, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis5, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis6, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis7, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis8, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis9, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis10, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis11, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis12, u0, u1, u2, extent)) {
			return false;
		}
		if (test(v0, v1, v2, axis13, u0, u1, u2, extent)) {
			return false;
		}

		return true;
	}
};


bool test(vec3 v0, vec3 v1, vec3 v2, vec3 axis, vec3 u0, vec3 u1, vec3 u2, vec3 e) {
	float p0 = dot(v0, axis);
	float p1 = dot(v1, axis);
	float p2 = dot(v2, axis);

	float r = e.x * abs(dot(u0, axis)) + e.y * abs(dot(u1, axis)) - e.z * abs(dot(u2, axis));

	if (max(-max(max(p0, p1), max(p1, p2)), min(min(p0, p1), min(p1, p2))) > r) {
		return true;
	}

	return false;
}

#endif