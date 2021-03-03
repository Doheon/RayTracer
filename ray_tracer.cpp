#include <iostream>
#include "FreeImage.h"
#include <make_sphere.h>
#include <uniformcell.h>
#include <importobject.h>
#include <time.h>

#define width 800
#define height 600
#define bpp 24


vec3 lightpos1(00.0f, 400.0f, -100.0f);
vec3 lightcolor1(1.0f);

vec3 lightpos2(-500.0f, 2300.0f, -1200.0f);
vec3 lightcolor2(1.0f, 1.0f, 0.9f);


vector<objects*> object;


vec3 phong(vec3 start, vec3 direction, vec3 lightpos, vec3 lightcolor, subcell c1);
objects& findnear(vec3 start, vec3 direction, subcell c1);
float shadow(vec3 start, vec3 lightpos, subcell c1);

int phongrepeat;
vec3 camera(0.0f, 0.0f, 400.0f);
vec3 background(0.2f);

time_t time1, time2;
double result;

int main() {

	time1 = time(NULL);


	//sphere sphere1(vec3(-1000.0f, -100.0f, -700.0f), 150.0f, "", vec3(0.1f), 0.3f, 0.9f, 1.0f); //쇠구슬
	sphere sphere4(vec3(-50.0f, -50.0f, -600.0f), 200.0f, "basketball1.png", vec3(0.0f), 0.3f, 0.0f, 1.0f);//농구공
	sphere sphere2(vec3(100.0f, -150.0f, -300.0f), 100.0f, "", vec3(0.1f), 1.0f, 0.4f, 0.2f); // 유리구슬

	sphere light(vec3(-400.0f, 1600.0f, -2060.0f), 100.0f, "", vec3(3.0f, 3.0f, 2.0f), 1.0f, 0.0f, 1.0f); //전등인척 하는 구

	//.floor
	triangle floor1(vec3(-2500.0f, -250.0f, -2000.0f), vec3(-2500.0f, -250.0f, 500.0f), vec3(2500.0f, -250.0f, 500.0f), 0, "floor.png", vec3(0.0f), 0.3f, 0.0f, 1.0f);
	triangle floor2(vec3(-2500.0f, -250.0f, -2000.0f), vec3(2500.0f, -250.0f, 500.0f), vec3(2500.0f, -250.0f, -2000.0f), 1, "floor.png", vec3(0.0f), 0.3f, 0.0f, 1.0f);

	//wall
	triangle wall1(vec3(-2500.0f, 2200.0f, -2000.0f), vec3(-2500.0f, -250.0f, -2000.0f), vec3(2500.0f, -250.0f, -2000.0f), 0,"wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);
	triangle wall2(vec3(-2500.0f, 2200.0f, -2000.0f), vec3(2500.0f, -250.0f, -2000.0f), vec3(2500.0f, 2200.0f, -2000.0f), 1, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);

	triangle wall3(vec3(-2500.0f, 2200.0f, 500.0f), vec3(-2500.0f, -250.0f, 500.0f), vec3(-2500.0f, -250.0f, -2000.0f), 0, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);
	triangle wall4(vec3(-2500.0f, 2200.0f, 500.0f), vec3(-2500.0f, -250.0f, -2000.0f), vec3(-2500.0f, 2200.0f, -2000.0f), 1, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);

	triangle wall5(vec3(-2500.0f, 2200.0f, 500.0f), vec3(-2500.0f, -250.0f, 500.0f), vec3(2500.0f, -250.0f, 500.0f), 0, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);
	triangle wall6(vec3(-2500.0f, 2200.0f, 500.0f), vec3(2500.0f, -250.0f, 500.0f), vec3(2500.0f, 2200.0f, 500.0f), 1, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);

	triangle wall7(vec3(2500.0f, 2200.0f, -2000.0f), vec3(2500.0f, -250.0f, -2000.0f), vec3(2500.0f, -250.0f, 500.0f), 0, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);
	triangle wall8(vec3(2500.0f, 2200.0f, -2000.0f), vec3(2500.0f, -250.0f, 500.0f), vec3(2500.0f, 2200.0f, 500.0f), 1, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);

	triangle wall9(vec3(-2500.0f, 2200.0f, 500.0f), vec3(-2500.0f, 2200.0f, -2000.0f), vec3(2500.0f, 2200.0f, -2000.0f), 0, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);
	triangle wall10(vec3(-2500.0f, 2200.0f, 500.0f), vec3(2500.0f, 2200.0f, -2000.0f), vec3(2500.0f, 2200.0f, 500.0f), 1, "wall3.png", vec3(0.0f), 0.0f, 0.0f, 1.0f);


	//mirror
	triangle mirror1(vec3(400.0f, 350.0f, -600.0f), vec3(400.0f, -250.0f, -600.0f), vec3(530.0f, -250.0f, -200.0f), 0, "", vec3(0.0f, 0.0f, 0.0f), 0.3f, 0.7f, 1.0f);
	triangle mirror2(vec3(400.0f, 350.0f, -600.0f), vec3(530.0f, -250.0f, -200.0f), vec3(530.0f, 350.0f, -200.0f), 1, "", vec3(0.0f, 0.0f, 0.0f), 0.3f, 0.7f, 1.0f);

	triangle mirror3(vec3(530.0f, 350.0f, -200.0f), vec3(530.0f, -250.0f, -200.0f), vec3(560.0f, -250.0f, -207.0f), 0, "", vec3(0.0f, 0.0f, 0.0f), 0.3f, 0.3f, 1.0f);
	triangle mirror4(vec3(530.0f, 350.0f, -200.0f), vec3(560.0f, -250.0f, -207.0f), vec3(560.0f, 350.0f, -207.0f), 1, "", vec3(0.0f, 0.0f, 0.0f), 0.3f, 0.3f, 1.0f);


	//glass
	triangle glass1(vec3(-530.0f, 350.0f, -200.0f), vec3(-530.0f, -250.0f, -200.0f), vec3(-400.0f, -250.0f, -600.0f), 0, "", vec3(0.1f), 0.3f, 0.6f, 0.2f);
	triangle glass2(vec3(-530.0f, 350.0f, -200.0f), vec3(-400.0f, -250.0f, -600.0f), vec3(-400.0f, 350.0f, -600.0f), 0, "", vec3(0.1f), 0.3f, 0.6f, 0.2f);

	triangle glass3(vec3(-560.0f, 350.0f, -207.0f), vec3(-560.0f, -250.0f, -207.0f), vec3(-530.0f, -250.0f, -200.0f), 0, "", vec3(0.1f), 0.3f, 0.0f, 0.5f);
	triangle glass4(vec3(-560.0f, 350.0f, -207.0f), vec3(-530.0f, -250.0f, -200.0f), vec3(-530.0f, 350.0f, -200.0f), 0, "", vec3(0.1f), 0.3f, 0.0f, 0.5f);

	triangle glass5(vec3(-430.0f, 350.0f, -607.0f), vec3(-430.0f, -250.0f, -607.0f), vec3(-400.0f, -250.0f, -600.0f), 0, "", vec3(0.1f), 0.3f, 0.0f, 0.1f);
	triangle glass6(vec3(-430.0f, 350.0f, -607.0f), vec3(-400.0f, -250.0f, -600.0f), vec3(-400.0f, 350.0f, -600.0f), 0, "", vec3(0.1f), 0.3f, 0.0f, 0.1f);

	triangle glass7(vec3(-560.0f, 350.0f, -207.0f), vec3(-560.0f, -250.0f, -207.0f), vec3(-430.0f, -250.0f, -607.0f), 0, "", vec3(0.1f), 0.3f, 0.0f, 0.1f);
	triangle glass8(vec3(-560.0f, 350.0f, -207.0f), vec3(-430.0f, -250.0f, -607.0f), vec3(-430.0f, 350.0f, -607.0f), 0, "", vec3(0.1f), 0.3f, 0.0f, 0.1f);



	//cube
	triangle cube1(vec3(-1060.2f, -50.0f, -700.0f), vec3(-1060.2f, -250.0f, -700.0f), vec3(-887.0f, -250.0f, -600.0f), 0, "four.png", vec3(0.0f), 0.3f, 0.3f, 1.0f);
	triangle cube2(vec3(-1060.2f, -50.0f, -700.0f), vec3(-887.0f, -250.0f, -600.0f), vec3(-887.0f, -50.0f, -600.0f), 1, "four.png", vec3(0.0f), 0.3f, 0.3f, 1.0f);

	triangle cube3(vec3(-887.0f, -50.0f, -600.0f), vec3(-887.0f, -250.0f, -600.0f), vec3(-787.0f, -250.0f, -773.2f), 0, "six2.png", vec3(0.0f), 0.3f, 0.3f, 1.0f);
	triangle cube4(vec3(-887.0f, -50.0f, -600.0f), vec3(-787.0f, -250.0f, -773.2f), vec3(-787.0f, -50.0f, -773.2f), 1, "six2.png", vec3(0.0f), 0.3f, 0.3f, 1.0f);

	triangle cube5(vec3(-960.2f, -50.0f, -873.2f), vec3(-1060.2f, -50.0f, -700.0f), vec3(-887.0f, -50.0f, -600.0f), 0, "", vec3(1.0f), 0.3f, 0.3f, 1.0f);
	triangle cube6(vec3(-960.2f, -50.0f, -873.2f), vec3(-887.0f, -50.0f, -600.0f), vec3(-787.0f, -50.0f, -773.2f), 1, "", vec3(1.0f), 0.3f, 0.3f, 1.0f);

	triangle cube7(vec3(-960.2f, -50.0f, -873.2f), vec3(-960.2f, -250.0f, -873.2f), vec3(-1060.2f, -250.0f, -700.0f), 0, "one.png", vec3(0.0f), 0.3f, 0.5f, 1.0f);
	triangle cube8(vec3(-960.2f, -50.0f, -873.2f), vec3(-1060.2f, -250.0f, -700.0f), vec3(-1060.2f, -50.0f, -700.0f), 1, "one.png", vec3(0.0f), 0.3f, 0.5f, 1.0f);

	triangle cube9(vec3(-787.0f, -50.0f, -773.2f), vec3(-787.0f, -250.0f, -773.2f), vec3(-960.2f, -250.0f, -873.2f), 0, "six2.png", vec3(0.0f), 0.3f, 0.5f, 1.0f);
	triangle cube10(vec3(-787.0f, -50.0f, -773.2f), vec3(-960.2f, -250.0f, -873.2f), vec3(-960.2f, -50.0f, -873.2f), 1, "six2.png", vec3(0.0f), 0.3f, 0.5f, 1.0f);






	//object.push_back(&sphere1);
	object.push_back(&sphere2);
	object.push_back(&sphere4);
	object.push_back(&light);

	object.push_back(&floor1);
	object.push_back(&floor2);

	object.push_back(&wall1);
	object.push_back(&wall2);
	object.push_back(&wall3);
	object.push_back(&wall4);

	//object.push_back(&wall5);
	//object.push_back(&wall6);
	//object.push_back(&wall7);
	//object.push_back(&wall8);
	//object.push_back(&wall9);
	//object.push_back(&wall10);

	object.push_back(&mirror1);
	object.push_back(&mirror2);
	object.push_back(&mirror3);
	object.push_back(&mirror4);

	object.push_back(&glass1);
	object.push_back(&glass2);
	object.push_back(&glass3);
	object.push_back(&glass4);


	
	object.push_back(&cube1);
	object.push_back(&cube2);
	object.push_back(&cube3);
	object.push_back(&cube4);
	object.push_back(&cube5);
	object.push_back(&cube6);

	//object.push_back(&cube7);
	//object.push_back(&cube8);
	//object.push_back(&cube9);
	//object.push_back(&cube10);
	
	importobject a("cat2.obj");
	object = a.addobject(object);


	subcell c(vec3(-2600, -350, 600), vec3(2600, 2300, -2100), 4, object); //가로 세로 높이를 4칸으로 나눈 voxel로 subdivision


	//cout << c1.cells[1].startpoint.z<<endl;

	//cout << triangle3.getnormal(vec3(0)).x << triangle3.getnormal(vec3(0)).y << triangle3.getnormal(vec3(0)).z << endl;


	
	
	//이미지 생성-----------------------------------------------
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, bpp);
	RGBQUAD color;

	if (!bitmap)
		exit(1);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
		//	int x = i - 322;
			int x = i - width / 2;
		//	int y = j - 80;
			int y = j - height / 2;

			phongrepeat = 0;
			vec3 ray = normalize(vec3(x, y, 0) - camera);
			vec3 phongcolor = phong(camera, ray, lightpos2, lightcolor2, c);

			color.rgbRed = min(phongcolor.x * 255, (float) 255);
			color.rgbGreen = min(phongcolor.y * 255, (float) 255);
			color.rgbBlue = min(phongcolor.z * 255, (float)255);	

			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}
	if (FreeImage_Save(FIF_PNG, bitmap, "figure.png", 0))
		cout << "저장성공!" << endl;

	FreeImage_DeInitialise();



	time2 = time(NULL);
	result = (double)(time2 - time1);
	cout << result << "sec" << endl;

	

}



vec3 phong(vec3 start, vec3 direction, vec3 lightpos, vec3 lightcolor, subcell c1) {


	objects& nearobject = findnear(start, direction, c1);


	if (&nearobject == nullptr) {
		return background;
	}

	float intersect = nearobject.intersect(direction, start);

	vec3 pos = start + intersect * direction;
	if (length(pos) < 10000) {
		vec3 n = nearobject.getnormal(pos);

		float diffuselight = max(dot(normalize(lightpos - pos), n), 0.0f);
		float speclight = pow(max(dot(normalize(start - pos), reflect(normalize(pos - lightpos), n)), 0.0f), 64);

		float distance = length(lightpos - pos) / 1000;
		float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);
		//float attenuation = 1.0f;

		vec3 phongcolor;
		phongcolor = (lightcolor * nearobject.gettexture(pos) * 0.35f + lightcolor * nearobject.gettexture(pos) * diffuselight + lightcolor * nearobject.gettexture(pos) * nearobject.specrate * speclight) * attenuation;
		
		float sh = shadow(pos, lightpos, c1);

		if (sh) {
			phongcolor *= sh;
		}
		else {
			return phongcolor * vec3(0.2f);
		}

		

		start += intersect * direction;
		vec3 direction1 = reflect(direction, n);
		vec3 direction2 = refract(direction, n, 0.9f);

		vec3 pixel;
		phongrepeat += 1;

		if (phongrepeat > 10) { //무한 반사, 굴절을 방지
			return phongcolor;
		}
		else if (nearobject.refractrate == 1.0f && nearobject.reflectrate == 0.0f) {
			pixel =phongcolor;
		}
		else if (nearobject.refractrate == 1.0f) {
			pixel = (phongcolor + nearobject.reflectrate * phong(start, direction1, lightpos, lightcolor, c1));
		}
		else if (nearobject.reflectrate == 0.0f) {
			pixel = nearobject.refractrate * (phongcolor) + (1.0f - nearobject.refractrate) * phong(start, direction2, lightpos, lightcolor, c1);
		}
		else {
			pixel = nearobject.refractrate * (phongcolor + nearobject.reflectrate * phong(start, direction1, lightpos, lightcolor, c1)) + (1.0f - nearobject.refractrate) * phong(start, direction2, lightpos, lightcolor, c1);
		}
		return pixel;
	}
	else {
		return background;
	}
}


/* 모든 object와 intersect 계산
objects& findnear(vec3 start, vec3 direction, subcell c1) {
	float min = 100000;
	objects* nearest = nullptr;
	for (objects* object1 : object) {
		if (object1->intersect(direction, start) < min) {
			min = object1->intersect(direction, start);
			nearest = object1;
		}
	}
	return *nearest;
}
*/


//uniform cell acceleration 적용
objects& findnear(vec3 start, vec3 direction, subcell c1) {
	int index = c1.findfirstindex(start, direction);
	vec3 point = start;
	return c1.findobject(index, point, direction);
}



float shadow(vec3 start, vec3 lightpos, subcell c1) {
	vec3 samplepos;
	float shadow = 0;
	for (int i = -1; i < 2; i++) { //soft shadow : 기존의 position +-200위치에 가상의 light 생성(9개)
		for (int j = -1; j < 2; j++) {
			samplepos = vec3(lightpos.x + i * 200, lightpos.y, lightpos.z + j * 200);
			vec3 direction = normalize(samplepos - start);
			objects& nearobject = findnear(start, direction, c1);
			if (&nearobject == nullptr) {
				shadow += 1;
			}
			else if (length(direction * nearobject.intersect(direction, start)) > length(lightpos - start) || length(direction * nearobject.intersect(direction, start)) < 1) {
				shadow += 1;
			}
			/*
			else if (nearobject.refractrate < 1) {
				start += direction * nearobject.intersect(direction, start);
				return shadow(start, direction, lightpos, c1) * (1.0f - nearobject.refractrate);
			}*/
		}
	}
	return shadow / 9;

}
