// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __POINT_H__
#define __POINT_H__

template<class TYPE>
class TPoint
{
public:

	TYPE x, y;

	TPoint()
	{}
	TPoint(TYPE x, TYPE y) : x(x), y(y)
	{}

	// Operators ------------------------------------------------
	TPoint operator -(const TPoint&v) const
	{
		//p2Vector2 r;

		//r.x = x - v.x;
		//r.y = y - v.y;

		return TPoint(x - v.x, y - v.y);
	}

	TPoint operator + (const TPoint&v) const
	{
		//p2Vector2 r;

		//r.x = x + v.x;
		//r.y = y + v.y;

		return TPoint(x + v.x, y + v.y);
	}

	const TPoint& operator -=(const TPoint&v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const TPoint& operator +=(const TPoint&v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const TPoint& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const TPoint& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	TPoint& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	TPoint& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const TPoint& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrt((fx*fx) + (fy*fy));
	}
};

typedef TPoint<int> iPoint;
typedef TPoint<float> fPoint;

#endif // __POINT_H__