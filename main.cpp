#include <iostream>
#include <math.h>
#include <stdio.h>

typedef float scalar_t;

scalar_t ssqrt(const scalar_t& x)
{
	return sqrt(x);
}

scalar_t ssin(const scalar_t& x)
{
	return sin(x);
}

scalar_t scos(const scalar_t& x)
{
	return cos(x);
}

scalar_t srad(const scalar_t& x)
{
	return x * (M_PI / 180.f);
}

class vec2 {
public:
	vec2()
	{
		m_x[0] = 0;
		m_x[1] = 0;
	}
	vec2(const scalar_t& x, const scalar_t& y)
	{
		m_x[0] = x;
		m_x[1] = y;
	}
	vec2(const scalar_t& t)
	{
		m_x[0] = scos(t);
		m_x[1] = ssin(t);
	}
	vec2(const vec2& o)
	{
		m_x[0] = o.m_x[0];
		m_x[1] = o.m_x[1];
	}
	~vec2()
	{
	}
	vec2& operator=(const vec2& o) {
		m_x[0] = o.m_x[0];
		m_x[1] = o.m_x[1];
		return *this;
	}
	vec2& operator*=(const scalar_t& x)
	{
		m_x[0] *= x;
		m_x[1] *= x;
		return *this;
	}
	vec2 operator*(const scalar_t& x) const
	{
		vec2 v;
		v.m_x[0] = m_x[0] * x;
		v.m_x[1] = m_x[1] * x;
		return v;
	}
	scalar_t operator*(const vec2& o) const
	{
		return m_x[0] * o.m_x[0] + m_x[1] * o.m_x[1];
	}
	vec2 operator+(const vec2& o) const
	{
		vec2 a;
		a.m_x[0] = m_x[0] + o.m_x[0];
		a.m_x[1] = m_x[1] + o.m_x[1];
		return a;
	}
	vec2& operator+=(const vec2& o)
	{
		m_x[0] += o.m_x[0];
		m_x[1] += o.m_x[1];
		return *this;
	}
	vec2 operator-(const vec2& o) const
	{
		vec2 a;
		a.m_x[0] = m_x[0] - o.m_x[0];
		a.m_x[1] = m_x[1] - o.m_x[1];
		return a;
	}
	vec2& operator-=(const vec2& o)
	{
		m_x[0] -= o.m_x[0];
		m_x[1] -= o.m_x[1];
		return *this;
	}
	vec2& operator/=(const scalar_t& x)
	{
		m_x[0] /= x;
		m_x[1] /= x;
		return *this;
	}
	vec2 operator/(const scalar_t& x) const
	{
		vec2 v;
		v.m_x[0] = m_x[0] / x;
		v.m_x[1] = m_x[1] / x;
		return v;
	}
	const scalar_t& operator[](int n) const {
		return m_x[n];
	}
	scalar_t& operator[](int n) {
		return m_x[n];
	}
	vec2& normalize()
	{
		return *this /= length();
	}
	vec2 normalized() const
	{
		return *this / length();
	}
	scalar_t length() const
	{
		return ssqrt(length2());
	}
	scalar_t length2() const
	{
		return *this * *this;
	}
private:
	scalar_t m_x[2];
};

class mat2x2 {
public:
	mat2x2()
	{
		m_x[0] = 1;
		m_x[1] = 0;
		m_x[2] = 0;
		m_x[3] = 1;
	}
	mat2x2(const scalar_t& x1, const scalar_t& x2, const scalar_t& y1, const scalar_t& y2)
	{
		set(x1, x2, y1, y2);
	}
	mat2x2(const scalar_t& t)
	{
		setangle(t);
	}
	mat2x2(const mat2x2& o)
	{
		*this = o;
	}
	~mat2x2()
	{
	}
	mat2x2 operator*(const scalar_t& a) const
	{
		return mat2x2(m_x[0] * a, m_x[1] * a, m_x[2] * a, m_x[3] * a);
	}
	mat2x2& operator*=(const scalar_t& a)
	{
		m_x[0] *= a;
		m_x[1] *= a;
		m_x[2] *= a;
		m_x[3] *= a;
		return *this;
	}
	vec2 operator*(const vec2& o)
	{
		return vec2(m_x[0] * o[0] + m_x[1] * o[1], m_x[2] * o[0] + m_x[3] * o[1]);
	}
	mat2x2 operator*(const mat2x2& o) const
	{
		mat2x2 t;
		t.m_x[0] = m_x[0] * o.m_x[0] + m_x[1] * o.m_x[2];
		t.m_x[1] = m_x[0] * o.m_x[1] + m_x[1] * o.m_x[3];
		t.m_x[2] = m_x[2] * o.m_x[0] + m_x[3] * o.m_x[2];
		t.m_x[3] = m_x[2] * o.m_x[1] + m_x[3] * o.m_x[3];
		return t;
	}
	mat2x2& operator*=(const mat2x2& o)
	{
		return *this = *this * o;
	}
	mat2x2& operator=(const mat2x2& o)
	{
		m_x[0] = o.m_x[0];
		m_x[1] = o.m_x[1];
		m_x[2] = o.m_x[2];
		m_x[3] = o.m_x[3];
		return *this;
	}
	mat2x2& setangle(const scalar_t& t)
	{
		m_x[0] = scos(t);
		m_x[1] = -ssin(t);
		m_x[2] = ssin(t);
		m_x[3] = scos(t);
		return *this;
	}
	mat2x2& set(const scalar_t& x1, const scalar_t& x2, const scalar_t& y1, const scalar_t& y2)
	{
		m_x[0] = x1;
		m_x[1] = x2;
		m_x[2] = y1;
		m_x[3] = y2;
		return *this;
	}
	scalar_t determinant() const {
		return m_x[0] * m_x[3] - m_x[2] * m_x[1];
	}
	mat2x2 inverse() const
	{
		mat2x2 m(m_x[3], -m_x[1], -m_x[2], m_x[0]);
		scalar_t d = determinant();
		m *= 1 / d;
		return m;
	}
	mat2x2& invert()
	{
		return *this = inverse();
	}
	const scalar_t* operator[](int n) const
	{
		return m_x + n * 2;
	}
	scalar_t* operator[](int n)
	{
		return m_x + n * 2;
	}
private:
	scalar_t m_x[4];
};

void printmat(const mat2x2& m)
{
	printf("[%.2f %.2f]\n[%.2f %.2f]\n", m[0][0], m[0][1], m[1][0], m[1][1]);
}

void printvec(const vec2& x)
{
	printf("(%.2f, %.2f)\n", x[0], x[1]);
}

class motor
{
};

class polardrive
{
public:
	polardrive()
	{
	}
	~polardrive()
	{
	}
	void drive(const vec2 &v, const scalar_t& t)
	{
		vec2 w;
		/*calculate m_world from fusion object*/
		w = m_world * v;
		/*motor speed = ((w * motor) + t)*k;*/
	}
private:
	mat2x2 m_world;
};

int main(int argc, char **argv)
{
	polardrive d;
	return 0;
}