#ifndef AABB_H
#define AABB_H

class aabb {
public:
	interval x, y, z;
	
	aabb(){}

	aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) 
	{
		pad_to_minimums();
	
	}

	aabb(const point3& a, const point3& b) {
		// Treat the two points a and b as extrema for the bounding box, so we don't require a
		// particular minimum/maximum coordinate order.

		x = interval(std::fmin(a[0], b[0]), std::fmax(a[0], b[0]));
		y = interval(std::fmin(a[1], b[1]), std::fmax(a[1], b[1]));
		z = interval(std::fmin(a[2], b[2]), std::fmax(a[2], b[2]));

		pad_to_minimums();
	}

	aabb(const aabb& box1, const aabb& box2) {
		x = interval(box1.x, box2.x);
		y = interval(box1.y, box2.y);
		z = interval(box1.z, box2.z);
	}

	const interval& axis_interval(int n) const {
		if (n == 1) return y;
		if (n == 2) return z;

		return x;
	}

	bool hit(const ray& r, interval ray_t) const {
		const point3& ray_origin = r.origin();
		const vec3& ray_dir = r.direction();

		for (int axis = 0; axis < 3; axis++) {
			const interval& ax = axis_interval(axis);
			const double adinv = 1.0 / ray_dir[axis];

			auto t0 = (ax.min - ray_origin[axis]) * adinv;
			auto t1 = (ax.max - ray_origin[axis]) * adinv;

			if (t0 < t1) {
				if (t0 > ray_t.min) ray_t.min = t0;
				if (t1 < ray_t.max) ray_t.max = t1;
			}
			else {
				if (t1 > ray_t.min) ray_t.min = t1;
				if (t0 < ray_t.max) ray_t.max = t0;
			}

			if (ray_t.max <= ray_t.min)
				return false;
		}

		return true;
	}

	int longest_axis() const {
		//Returns index of longest axis of bounding box

		if (x.size() > y.size())
			return x.size() > z.size() ? 0 : 2;
		else
			return y.size() > z.size() ? 1 : 2;
	}

	static const aabb empty, universe;

private:
	void pad_to_minimums() {
		//Adjust AABB so that no side is smaller than the delta value. So we don't have something infinitely flat

		double delta = 0.0001;
		if (x.size() < delta) x = x.expand(delta);
		if (y.size() < delta) y = y.expand(delta);
		if (z.size() < delta) z = z.expand(delta);
	}

};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

aabb operator+(const aabb& bbox, const vec3& offset) {
	return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

aabb operator+(const vec3& offset, const aabb& bbox) {
	return bbox + offset;
}
#endif // !AABB_H
