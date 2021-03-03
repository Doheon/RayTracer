---
typora-copy-images-to: photo
---

# RayTracer
Own CPU ray tracer using c++



## 개요

c++을 이용하여 CPU를 기반으로 작동하는 Ray tracer를 구현하였으며 구현한 내용은 아래와 같다.

- Recursive reflection
- Recursive refraction
- Texture mapping
- Soft shadow
- Import obj file
- Uniform cell subdivision

![image-20210303120702423](photo\image-20210303120702423.png)

위와 같은 사진을 얻는 데에 1271초(약 21분) 이 소모된다.

만약 uniform cell subdivison을 하지 않는다면 약 2시간 가량 소모되며 uniform cell subdivision을 통해 약 5.5배 시간을 단축 시킬수 있다.







## 구현 설명

### make_sphere.h

이름은 make_sphere.h이지만 triangle, shpere를 만드는 클래스를 포함하고 있다.

- float intersect: object와 입력 받은 ray와의 intersect되려면 얼만큼 가야 하는지 계산한다. intersect되지 않는다면 매우 큰 값을 리턴 한다.
- vec3 getnormal: object 가 주어진 위치에서 갖고 있는 normal vector를 계산한다.
- vec3 gettexture: object를 주어진 이미지로 mapping하고 주어진 위치에서 갖는 pixel color를 계산한다.
- bool cellintersect: object가 주어진 voxel과 intersect가 있는지 계산한다. Triangle과 voxel의 intersection을 계산할 때는 SAT(Separating Axis Theorem)을 이용하였다.



### Importobject.h

obj파일을 읽어오는 클래스가 포함된 헤더파일이다.

obj파일을 v, f 가 처음에 있는 항목만 읽어온다. 읽어온 정보를 각각 vec3형식의 vector에 저장하고 f에 적혀 있는 순서에 따라 v의 vertex정보를 불러온 후 위에서 구현했던 make_sphere의 triangle class를 이용하여 object를 만들고 objects vector에 push back해준다.





### uniformcell.h

속도 향상 기법중 하나인 Uniform cell subdivision을 구현하였다.

생성자에서는 입력 받은 공간을 입력 받은 분할단위로 분할한 후 각 cell에 어떤 object가 intersect되어있는지 확인하고 cell에 intersect 된 object의 정보들을 넣어 준다.

findobject 함수에서는 입력 받은 ray가 cell에 들어있는 object와 intersection이 있는지 확인하고 없으면 ray의 방향에 따라 적절한 다음 cell로 넘어가 언젠가 intersect가 있거나 정해진 공간을 벗어날 때까지 재귀적으로 시행되는 함수이다. Intersect된 object를 return한다.



### ray_tracer.cpp

실제 작업이 이루어 지는 cpp파일이다.

카메라로부터 스크린의 픽셀로 ray를 쏴서 intersect되는 가장 가까운 object를 찾고 그 지점에서 phong lighting을 해서 색을 정한 후 물체의 성질에 따라 refract ray와 reflect ray를 다시 쏴서 픽셀의 최종 색깔을 재귀적으로 계산한다. 추가로 해당지점에서 light로 향하는 ray가 light에 도달할 때까지 intersection이 있는지 확인후 shadow도 계산해 준다. (shadow ray를 light position의 x, z 방향으로 +-200해준 값으로 9개를 쏴서 soft shadow도 구현하였다.)





### Uniform cell subdivision 효과

처음에 고양이를 넣기 전 약 20개의 object만 있었을 때는 공간을 분할하지 않았을 때 가장 빠른 속도를 보이고 공간을 분할할수록 속도가 오히려 늦어지는 것을 확인했다.

하지만 고양이를 넣고 object가 2200여개가 되자 실행속도가 매우 느려 졌고 subdivision의 효과가 나타나기 시작했다. 모든 이미지를 출력하기엔 시간이 너무 오래 걸려서 1만개의 pixel만 처리하여 가장 빠른 subdivision 정도를 찾아보았고 그 결과 공간을 가로 세로 높이를 4등분한 64개의 voxel로 나누었을 때 가장 빠른 것을 확인할 수 있었다. 모든 이미지를 출력하는데 걸리는 시간은 1271초로 subdivision하기 전 약 2시간 걸리던걸 약 5.5배 단축한 효과가 있었다. 

 위의 결과로 가장 빠른 결과를 보이는 uniform cell의 단위는 unit cell 하나당 들어있는 object의 개수에 따라 달라진다고 유추할 수 있었고 object가 수만, 수십만개의 단위로 늘어난다면 cell하나당 약 30여개의 object가 들어가도록 공간을 분할하는 것이 효율적일 것이라고 예상해 볼 수 있었다.



## 기타 사진들



<img src="photo\4.png" alt="4" style="zoom: 33%;" />

<img src="photo\5.png" alt="5" style="zoom: 50%;" />

<img src="C:\Users\Doheon\Desktop\Doheon\documents\Visualstudio\ray_tracer\photo\2373.png" alt="2373" style="zoom: 50%;" />

