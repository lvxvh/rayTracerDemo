## Rest

### 1. overview

目标: Monte Carlo rendering

未提及: 

* shadow rays, 
* bidirectional methods, 
* Metropolis methods,
* photon mapping

###  2. Monte Carlo Program

用概率统计的方法近似结果

当精确度要求不高时可用

* 估计 Pi
* 收益递减定律，在Monte Carlo方法中，每一个样本的贡献率都比前一个低
* 通过分层采样可缓解上述现象：将采样区间均分，在每个子区间内各采一个样（quasi)
* 当数据维度上升时，上述方法效果下降（维度诅咒）
* 之前的方法中，小光源制造了太多噪声，是因为均匀采样计算平均值没有很好地采样光源，需要用 概率密度函数(PDF) 来进行重要性采样

### 3. 1维 MC 积分

* 举例:
  $$
  I = \int_0^2 x^2dx
  $$
  => 
  $$
  I = 2 \cdot average(x^2, 0, 2)
  $$
  相当于求同样宽度的矩形的面积，高度为平均值

* 密度函数

  概率密度函数和累计密度函数

* 如何生成服从 PDF 的随机数？

  1. 先随机生成 0~1 的随机数，再用一个映射函数 f 映射到 PDF 区间

     ！！！！！！！！！！！！！！！

     **因为累积概率密度函数(APDF)值域是0~1，所以用取的这个随机数当做 APDF 的一个值，反解对应的采样值**

     ！！！！！！！！！！！！！！

  2. 由概率密度函数 p(x) 积分求累加概率密度函数 P(x)

  3. P(x) 的意义是 服从 p(x) 的随机变量小于等于 x 的概率

  4. **f 是 P 的反函数**

* 在重要性采样过程中，每个样本要乘上对应的权重，以保证最终结果的uniformity， **权重就是 1/pdf(x)**

* 重要性采样：

  使用重要性采样可以快速收敛并减少噪声

  当重要性采样的 pdf p越接近 目标函数 时，效果越好

* 过程:

  ![image-20200727224125760](/Users/xuhui/Library/Application Support/typora-user-images/image-20200727224125760.png)

### 4. 球面 MC 积分

* 同理

  被积函数是 cos^2(θ) 时

  ```c++
  inline double pdf(const vec3& p) {
      return 1 / (4*pi);
  }
  
  
  int main() {
      int N = 1000000;
      auto sum = 0.0;
      for (int i = 0; i < N; i++) {
          vec3 d = random_unit_vector();
          auto cosine_squared = d.z()*d.z();
          sum += cosine_squared / pdf(d);
      }
      std::cout << std::fixed << std::setprecision(12);
      std::cout << "I = " << sum/N << '\n'; // 注意 /N
  }
  ```
  
  $$
  I = \frac{1}{N}\sum_{i=0}^N \frac{f(x_i)}{p(x_i)}
  $$
  
  用p的反函数生成 x 采样点。

### 5. 光线散射

* albedo 反射率（A)

  材质反射光线的能力：

  当光线照射到物体上时，A部分的光线被反射，1-A被吸收

* Scattering

  在PBR中，使用波长而不是RGB来表示光的颜色

  当光线在物体表面散射时，物体颜色表示为
  $$
  Color = \int A \cdot s(\omega)\cdot color(\omega)d\omega
  $$
  s 是描述散射光线分布的pdf，其中 A 和 s 是视角相关的

* Scattering PDF

  相位函数？

  表面上某点的颜色为：
  $$
  Color=\frac{A⋅s(direction)⋅color(direction)}{p(direction)}
  $$
  p为任意一个pdf

  **p是我们采样用的pdf，s是材质反射导致的pdf**

* BRDF
  $$
  BRDF=\frac{A⋅s(direction)}{cos(θ)}
  $$
  Lambertian BRDF 为 A/Pi

### 6. 重要性采样

发送更多光线到光源

* 假设有倾向于光源的 PDF pLight, 也有与表面相关的 PDF pSurface， 可以直接用blend的方法合为一个 PDF

* 要让 PDF 尽可能大？？？

  => s * color 尽可能大

* 对于漫反射材质来说，s 各个方向都相同，因此只要让 c 最大

* 对于镜面反射来说，s 只在某个方向最大，因此s更重要（大部分都特殊处理？）

* recap, Monte Carlo:
  $$
  \int f(x) = \frac{1}{N}\cdot \sum \frac{f(r)}{p(r)}
  $$
  r 为采样点， p(r) 为 PDF

* 对于球面采样， p = cos(θ)/π

* 随机半球采样

### 7. 生成随机方向（以z轴为轴心的方法）(主要是计算采样点)

* 在球面上随机生成点，点的方向由 概率函数 f(θ) 决定，则有

* 对点的坐标 θ, φ: 其 PDF 分别为:
  $$
  a(\phi) = \frac{1}{2\pi}\\
  b(\theta) = 2\pi f(\theta)sin(\theta)
  $$
  对于 b(θ), 2πsin(θ)可以理解为在θ角度上的圆的周长

* 用随机数随机生成 r1(对应φ), r2(对应θ), 根据APDF 计算出对应的采样点:
  $$
  r_1 = \int_0^\phi \frac{1}{2\pi}dt\\
  \phi = 2\pi r_1
  $$

  $$
  r_2 = \int_0^\theta 2\pi f(t)sin(t)dt\\
  cos(\theta) = 1-2r_2
  $$

  其中θ不用求出具体值，有cos值足够之后计算

* 根据计算出的采样点得到xyz坐标

* 对于半圆，可将前面的计算 cos(θ) 的公式改为:
  $$
  cos(\theta) = 1 - r_2
  $$
  从而将角度范围限定在了半圆内

### 8. 正交基（以表面法向量为轴）

* 好处是可在任何表面生成随机方向，而不是限定在z轴上

* 相对坐标系

* 生成正交基

  只需要得到与法向量相垂直的一组单位方向向量，不用得到原点

* 只要给出一个不平行于法向量的向量a，即可用两次叉乘得到正交基

* 常用的找到 a 的方法：

  ```
  if absolute(n.x > 0.9)
      a ← (0, 1, 0)
  else
      a ← (1, 0, 0)
  ```

* 坐标转化，给定坐标（x,y,z）, 转为正交基(s,t,n)坐标:

  v = xs + yt + zn

### 9. 直接光线采样

问题：光线需要更多的投向光源。

方案1：使用 shadow ray， 分理处计算直接光照的过程

方案2：向光源发送更多光线

关键：获取 PDF

* 获取光源的 PDF

  ![image-20200729234514167](/Users/xuhui/Library/Application Support/typora-user-images/image-20200729234514167.png)

  目标是只对光源方向采样，且采样方向要均匀分布在光源面积上，

  对于上图，就应该有，光源表面 PDF 为1/A, 且 采样到 dw 和 dA 的概率相同。

  * 在上图中有:
    $$
    \frac{d\omega}{dA} = \frac{1}{distance^2(p,q)} \cdot cos\alpha
    $$
    其中 α 是 dA 所处平面法向量与 pq 连线的夹角，如下图所示:

    ![image-20200730001550659](/Users/xuhui/Library/Application Support/typora-user-images/image-20200730001550659.png)

  * 采样 dw 和 dA 相同，则有
    $$
    p(direction)\cdot \frac{dA\cdot coa\alpha}{distance^2(p,q)} = p_q(q) \cdot dA
    $$

  * 由于 p(q) = 1/A, 所以
    $$
    p(direction) = \frac{distance^2(p,q)}{coa\alpha \cdot A}
    $$

* 在emitted函数中加上面的判断，让面光源单面发光

### 10. 混合密度

* 现有两个 PDF，一个 是 cosθ 相关，另一个是 光线相关，现在需要 blend

* 建立 pdf 抽象类，pdf 需要做两件事：

  1. 给定一个采样值返回对应的 p 值
  2. 生成随机的采样值（反函数）

* 建立向 hittable 采样的 pdf，并应用于光源

* 混合pdf 过程：

  1. 对于获取p值的函数，直接blend

  2. 对于生成随机采样值的函数，用随机抽取的方法抽一个pdf生成采样值:

     ```c++
     virtual double value(const vec3& direction) const override {
             return 0.5 * p[0]->value(direction) + 0.5 *p[1]->value(direction);
         }
     
         virtual vec3 generate() const override {
             if (random_double() < 0.5)
                 return p[0]->generate();
             else
                 return p[1]->generate();
         }
     
     ```

     

