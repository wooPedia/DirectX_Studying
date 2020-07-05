개인적으로 공부하는 내용이므로 틀린 부분이 있을 수 있습니다. 있다면 알려주세요:)

# 행렬 변환

기하와 벡터 또는 선형대수학에서 배웠다면 행렬을 통한 도형 변환에 대해 배웠을 것이다. 예를 들면 다음 그림과 같이 다양한 동작을 수행하는 행렬을 곱하여 도형을 변환시킬 수 있다.

![image](https://user-images.githubusercontent.com/46551002/86531134-9be79100-bef9-11ea-9cb6-b4c0c644af99.png)
    [출처](https://wikidocs.net/37098)



행렬의 장점은 편리함에 있다. 예를 들어 어떤 도형의 크기를 늘리고 회전시키고 싶다면 따로 따로 적용하는 것이 아니라 크기를 늘리는 행렬과 회전을 시키는 행렬을 서로 곱하여 하나의 행렬을 얻기만 하면 된다. 그리고 얻은 행렬을 원하는 점에 적용하면 크기와 회전 등이 알맞게 적용된다. Direct3D 에서는 이와 같은 행렬 변환을 지원한다. 최종적으로 변환이 완료된 행렬을 정점에 곱하면 변환된 정점을 얻을 수 있다. 

변환 종류에는 크게 다음과 같은 것들이 있다.

1. 이동 행렬(Translation matrix)
2. 크기 행렬(Scale matrix)
3. 회전 행렬(Rotation matrix)


이동 행렬은 말 그대로 이동이므로 위치를 변환해주는 행렬이다. 크기 행렬은 크기를 축소 및 확대시킨다. 회전은 말 그대로 회전시킨다.

공간은 3차원이지만 Direct3D에선 투영과 반사와 같은 기능을 위해 4x4 행렬을 이용한다. 정점(벡터)은 1x3인 (x,y,z) 이지만 4x4 행렬과의 곱 연산을 위해 1x4(x,y,z,1)를 사용한다. 마지막 값은 사용되지 않기 때문에 1로 설정한다. 이는 곱 연산시 항상 행렬 값에만 의존하도록 하기 위해서다. (1이 곱 연산에서 항등원이므로)

<br>

## D3DXMATRIX

Direct3D에선 다음과 같은 행렬 구조체를 지원한다. 아래 구조체는 D3DMATRIX인데, 이 클래스를 상속하여 연산자 오버로딩을 추가한 D3D<b>X</b>MATRIX도 존재한다. 주로 D3DXMATRIX를 사용한다. 아래와 같이 4x4 배열로 4x4 행렬을 표현한다.

![image](https://user-images.githubusercontent.com/46551002/86531286-e1f12480-befa-11ea-8c66-f9282e5cf581.png)

<br>

## 이동 행렬(Translation matrix)

이동 행렬은 다음과 같이 점 또는 도형을 이동시키는 행렬이다.

![image](https://user-images.githubusercontent.com/46551002/86531403-c20e3080-befb-11ea-8b3f-b0b2272b039d.png)

이동 행렬은 위와 같이 단위 행렬(identity matrix)에서 마지막 행의 마지막 열을 제외하고 이동하고 싶은 만큼 값을 넣어준다. 변환 이전의 (x,y)에서 각각 a,b만큼 이동하고 싶다면 결과는 (x+a, x+b)가 된다. 원한다면 (x,y,1) 과 행렬 T를 곱해보면 (x+dx, x+dy, 1)이 나올 것이다. 우리는 3차원이므로 x,y,z을 고려하여 4행의 1,2,3열을 변경해주면 된다. 

이러한 이동 변환을 위해 `D3DXMatrixTranslation()` 함수를 지원한다.  

```cpp
// Build a matrix which translates by (x, y, z)
D3DXMATRIX* WINAPI D3DXMatrixTranslation
    ( D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z );
```

`pOut`에 미리 생성한 행렬 변수를 넣어준다. 그리고 x, y, z에 이동을 원하는 만큼의 값을 각각 넣어준다. 위 그림으로 따지면 dx, dy, dz에 해당한다. 그러면 pOut엔 T와 같은 행렬이 저장된다. 

아래와 같이 x,y,z 축으로 각각 3,3,0만큼 이동시키는 행렬을 출력하면 다음과 같다.

```cpp
D3DXMATRIX mat;
D3DXMatrixTranslation(&mat, 3.f, 3.f, 0.f);
```

![image](https://user-images.githubusercontent.com/46551002/86531559-5331d700-befd-11ea-8133-b336ed65d770.png)

<br>

## 크기 행렬(Scale matrix)

크기 행렬은 다음과 같이 점 또는 도형의 크기를 축소 및 확대시키는 행렬이다.

![image](https://user-images.githubusercontent.com/46551002/86531630-e539df80-befd-11ea-8ad5-6b2603e0fcb2.png)

크기를 변환시키기 위해선 각 좌표값에 실수배를 해주면 된다. 이를 위해선 주대각원소의 각 x,y,z값을 변경해주면 된다.

이러한 크기 변환을 위해 `D3DXMatrixScaling()` 함수를 지원한다.

```cpp
// Build a matrix which scales by (sx, sy, sz)
D3DXMATRIX* WINAPI D3DXMatrixScaling
    ( D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz );
```

<br>

다음은 D3DXMatrixScaling() 함수를 적용한 행렬이다.
```cpp
D3DXMATRIX mat;
D3DXMatrixScaling(&mat, 3.f, 3.f, 0.f);
```

![image](https://user-images.githubusercontent.com/46551002/86531788-38f8f880-beff-11ea-95d9-381ed19e2b42.png)

<br>

아래 사진의 오른쪽 사진은 왼쪽의 DirectX 사진을 1/2로 축소시킨 것이다. 물론 겹치면 보이지 않기 때문에 이동 변환도 포함되어 있다.

![image](https://user-images.githubusercontent.com/46551002/86531938-9ccff100-bf00-11ea-9972-0a804a05ac1e.png)

<br>

## 회전 행렬(Rotation matrix)

회전 행렬은 다음과 같이 점 또는 도형을 회전시켜주는 행렬이다.

![image](https://user-images.githubusercontent.com/46551002/86531984-2bdd0900-bf01-11ea-9031-b909f2ff4cdc.png)

회전 변환의 종류로는 크게 다음과 같은 것들이 존재한다. 여기서는 x,y,z 세 개만 알아본다.

- x축 회전
- y축 회전
- z축 회전
- 임의의 축을 기준으로 한 회전

우선 x축 회전은 다음과 같은 함수를 사용한다. 이때 각도는 라디안 값을 이용해야 한다. 30도 45도와 같은 것은 호도법이고 파이로 나타내는 것이 라디안이다. 우리는 호도법이 편하므로 호도법을 라디안으로 변환해주는 `D3DXToRadian` 라는 매크로를 이용한다. 

```cpp
// Build a matrix which rotates around the X axis
D3DXMATRIX* WINAPI D3DXMatrixRotationX
    ( D3DXMATRIX *pOut, FLOAT Angle );
```

<br>

다음은 x축으로 30도만큼 회전시켜주는 행렬이다. 행렬의 값을 보면 중앙의 4부분이 순서대로 cos,sin,-sin,cos과 같이 되어있는 것을 볼 수 있다.

```cpp
D3DXMatrixRotationX(&mat, D3DXToRadian(30));
```

![image](https://user-images.githubusercontent.com/46551002/86532285-c0486b00-bf03-11ea-9905-edbae73f604f.png)

<br>

다음은 y축으로 30도만큼 회전시켜주는 행렬이다.

![image](https://user-images.githubusercontent.com/46551002/86532363-52e90a00-bf04-11ea-896b-0661495c71ca.png)


<br>

다음은 z축으로 30도만큼 회전시켜주는 행렬이다.

![image](https://user-images.githubusercontent.com/46551002/86532382-71e79c00-bf04-11ea-9bc0-145ebf27a6a0.png)


만약 x, y, z축 회전 중 여러개를 적용하고 싶다면 z->x->y 순으로 곱해야 한다. 이를 위해 `D3DXMatrixRotationYawPitchRoll()` 라는 함수도 존재한다. 이 함수는 x,y,z를 인자로 받아 알아서 연산해준다. 

<br>

다음은 z축으로 30도 만큼 회전한 결과이다. (크기와 이동 변환도 적용되었다.)    


![image](https://user-images.githubusercontent.com/46551002/86532593-39e15880-bf06-11ea-9a40-6b6a26e2a781.png)

<br>

참고로 이동, 크기, 회전 변환을 모두 적용하고 싶을 땐 `크기 -> 회전(z->x->y) -> 이동` 순으로 곱해야 한다.   

![image](https://user-images.githubusercontent.com/46551002/86532769-53cf6b00-bf07-11ea-9097-e0912a6da05f.png)
