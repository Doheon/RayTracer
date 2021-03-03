#ifndef uniformcell_h

#define uniformcell_h


class cell {
public:
	vec3 startpoint;
	vec3 unitvec;
	vector<objects*> object;
	cell(vec3 startpoint1, vec3 unitvec1) : startpoint(startpoint1), unitvec(unitvec1){}

	void addobject(objects* object1) {
		object.push_back(object1);
	}

};


class subcell {
public:
	vector<cell> cells;
	int subnum;
	subcell(vec3 volumestart, vec3 volumeend, int subnum1, vector<objects*> object) :subnum(subnum1){ //각각의 voxel에 intersection이 있는 object들을 저장
		vec3 unitvec = (volumeend - volumestart) * (float(1) / subnum);
		for (int i = 0; i < subnum; i++) {
			for (int j = 0; j < subnum; j++) {
				for (int k = 0; k < subnum; k++) {
					cell a(volumestart + vec3(unitvec.x * k, unitvec.y * j, unitvec.z * i), unitvec);

					for (objects* object1 : object) {
						if (object1->cellintersect(a.startpoint, a.unitvec)) {
							a.addobject(object1);
							
						}
					}

					//cout << a.object.size() << endl;
					cells.push_back(a);
				}
			}
		}
	}



	int findfirstindex(vec3 start, vec3 direction) {
		vec3 o = cells[0].startpoint;
		//cout << o.x << endl;
		vec3 unit = cells[0].unitvec;
		vec3 v = start - o;
		int i = v.x / unit.x;
		int j = v.y / unit.y;
		int k = v.z / unit.z;
		int cellindex = i + subnum * j + subnum * subnum * k;
		//cout << cellindex << endl;
		return cellindex;
	}



	objects& findobject(int cellindex, vec3 start, vec3 direction) { //주어진 index의 voxel에 있는 object들과 ray가 intersection이 있는지 계산, 없다면 다음 voxel로 넘어가서 반복실행
		float m = 100000;
		objects* nearest = nullptr;

		for (objects* object1 : cells[cellindex].object) {
			if (object1->intersect(direction, start) < m) {
				m = object1->intersect(direction, start);
				nearest = object1;
			}
		}

		if (nearest != nullptr) {
			vec3 inter = start + m * direction;
			vec3 s1 = cells[cellindex].startpoint;
			vec3 s2 = cells[cellindex].startpoint + cells[cellindex].unitvec;
			if ((inter.x >= s1.x && inter.x <= s2.x) && (inter.y >= s1.y && inter.y <= s2.y) && (inter.z <= s1.z && inter.z >= s2.z)) {
				return *nearest;
			}
		}

		vec3 o1 = cells[cellindex].startpoint;
		vec3 o2 = o1 + cells[cellindex].unitvec;
		float tx0 = 1000000, tx1 = 1000000, tx = 1000000, ty0 = 1000000, ty1 = 1000000, ty = 1000000, tz0 = 1000000, tz1 = 1000000, tz = 1000000;

		if (direction.x != 0) {
			tx0 = (o1.x - start.x) / direction.x;
			tx1 = (o2.x - start.x) / direction.x;
			tx = max(tx0, tx1);
		}
		else tx = 1000000;

		if (direction.y != 0) {
			ty0 = (o1.y - start.y) / direction.y;
			ty1 = (o2.y - start.y) / direction.y;
			ty = max(ty0, ty1);
		}
		else ty = 1000000;
		if (direction.z != 0) {
			tz0 = (o1.z - start.z) / direction.z;
			tz1 = (o2.z - start.z) / direction.z;
			tz = max(tz0, tz1);
		}
		else tz = 1000000;

		float t = min(min(tx, ty), tz);

		

		if (t < 0 || t == 1000000) {
			return *nearest;
		}


		if (t == tx) {
			if (t == tx0) {
				if (cellindex % subnum == 0) {
					return *nearest;
				}
				cellindex -= 1;
				start += direction * t;
			}

			else if (t == tx1) {
				if (cellindex % subnum == subnum - 1) {
					return *nearest;
				}
				cellindex += 1;
				start += direction * t;
			}
		}

		else if (t == ty) {
			if (t == ty0) {
				if (cellindex % (subnum * subnum) < subnum) {
					return *nearest;
				}
				cellindex -= subnum;
				start += direction * t;
			}

			else if (t == ty1) {
				if (cellindex % (subnum * subnum) > subnum* (subnum - 1) - 1) {
					return *nearest;
				}
				cellindex += subnum;
				start += direction * t;
			}
		}

		else if (t == tz) {
			if (t == tz0) {
				if (cellindex < subnum * subnum) {
					return *nearest;
				}
				cellindex -= subnum * subnum;
				start += direction * t;
			}

			else if (t == tz1) {
				if (cellindex > subnum* subnum* (subnum - 1) - 1) {
					return *nearest;
				}
				cellindex += subnum * subnum;
				start += direction * t;
			}
		}

		return findobject(cellindex, start, direction);
	
	}

};

#endif