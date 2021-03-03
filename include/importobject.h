#ifndef importobject_h

#define importobject_h

#include <fstream>
#include <sstream>
#include <string>



class importobject {
	vector<vec3> vertics;
	vector<vec3> faces;
	vector<triangle> obj;
public:
	importobject(const char* txtpath) {
		vector<float> vertex;
		vector<int> face;
		string filepath = txtpath;
		ifstream openFile(filepath.data());

		if (openFile.is_open()) {
			string line;
			float num;
			char slash;
			while (getline(openFile, line)) {
				stringstream stream;
				if (line[0] == 'v') {
					line[0] = ' ';
					stream.str(line);
					while (stream >> num) {
						vertex.push_back(num);
					}
				}
				if (line[0] == 'f') {
					line[0] = ' ';
					stream.str(line);

					for (int i = 0; i < 3; i++) {
						stream >> num;
						face.push_back(num);

						stream >> slash;

						stream >> num;
					}

				}

			}
			openFile.close();
		}

		vec3 temp;
		for (int i = 0; i < vertex.size() / 3; i++) {
			temp.x = vertex[i * 3] - 620;
			temp.y = vertex[i * 3 + 1] - 257.14f;
			temp.z = vertex[i * 3 + 2] - 850.0f;
			vertics.push_back(temp);
		}


		for (int i = 0; i < face.size() / 3; i++) {
			temp.x = face[i * 3];
			temp.y = face[i * 3 + 1];
			temp.z = face[i * 3 + 2];
			faces.push_back(temp);
		}


		//cout << vertics[322].x<< " "<< vertics[322].y<<" "<< vertics[322].z << endl;
		for (int i = 0; i < faces.size(); i++) {
			triangle f1(vertics[faces[i].x - 1], vertics[faces[i].y - 1], vertics[faces[i].z - 1], 0, "", vec3(1.5f, 1.0f, 0.7f), 0.0f, 0.0f, 1.0f);
			obj.push_back(f1);
		}

	}


	vector<objects*> addobject(vector<objects*> object1) {
		for (int i = 0; i < faces.size(); i++) {
			object1.push_back(&obj[i]);
		}
		return object1;
	}

};


#endif
