#ifndef  POINT

#define POINT


class point
{
public:
	float x, y, z;

	point(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	point()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	friend point operator*(float lhs, point rhs)
	{
		point p;
		p.x = rhs.x * lhs;
		p.y = rhs.y * lhs;
		p.z = rhs.z * lhs;
		return p;
	}
	friend point operator+(point lhs, point rhs)
	{
		point p;
		p.x = rhs.x + lhs.x;
		p.y = rhs.y + lhs.y;
		p.z = rhs.z + lhs.z;
		return p;
	}
};

#endif // ! POINT
