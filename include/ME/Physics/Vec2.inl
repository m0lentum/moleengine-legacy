
inline float Vec2::getLength() const
{
	return std::sqrtf(x * x + y * y);
}

inline Vec2 Vec2::normalized() const
{
	float length = getLength();
	return Vec2(x / length, y / length);
}

inline float Vec2::dot(const Vec2 &other) const
{
	return x * other.x + y * other.y;
}

inline Vec2 Vec2::projectToAxis(const Vec2 &axis) const
{
	float dp = dot(axis);
	return Vec2(axis.x * dp, axis.y * dp);
}

inline Vec2 Vec2::projectToVector(const Vec2 &vector) const
{
	return Vec2::projectToAxis(vector.normalized());
}

inline void Vec2::rotateRad(float radians)
{
	float oldX = x;
	x = (std::cos(radians) * oldX) - (std::sin(radians) * y);
	y = (std::sin(radians) * oldX) + (std::cos(radians) * y);
}

inline void Vec2::rotateDeg(float degrees)
{
	rotateRad(degrees * PI / 180);
}

inline Vec2 Vec2::leftNormal() const
{
	return Vec2(-y, x);
}

inline Vec2 Vec2::rightNormal() const
{
	return Vec2(y, -x);
}



inline Vec2::Vec2(float x, float y) :
	x(x), y(y)
{
}

inline Vec2::~Vec2()
{
}


// Operators
inline Vec2 operator -(const Vec2 &right)
{
	return Vec2(-right.x, -right.y);
}

inline Vec2 operator +(const Vec2 &left, const Vec2 &right)
{
	return Vec2(left.x + right.x, left.y + right.y);
}

inline Vec2 operator -(const Vec2 &left, const Vec2 &right)
{
	return Vec2(left.x - right.x, left.y - right.y);
}

inline Vec2 operator *(const Vec2 &left, float right)
{
	return Vec2(left.x * right, left.y * right);
}

inline Vec2 operator *(float left, const Vec2 &right)
{
	return Vec2(right.x * left, right.y * left);
}

inline Vec2 operator /(const Vec2 &left, float right)
{
	return Vec2(left.x / right, left.y / right);
}

inline Vec2& operator +=(Vec2 &left, const Vec2 &right)
{
	left.x += right.x;
	left.y += right.y;
	return left;
}

inline Vec2& operator -=(Vec2 &left, const Vec2 &right)
{
	left.x -= right.x;
	left.y -= right.y;
	return left;
}

inline Vec2& operator *=(Vec2 &left, float right)
{
	left.x *= right;
	left.y *= right;
	return left;
}

inline Vec2& operator /=(Vec2 &left, float right)
{
	left.x /= right;
	left.y /= right;
	return left;
}