#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstddef> // g++ chokes on size_t without this
#include <set>

using namespace std;


class indexed_vertex_3
{
public:

	inline indexed_vertex_3(void) : x(0.0f), y(0.0f), z(0.0f) { /*default constructor*/ }
	inline indexed_vertex_3(const float src_x, const float src_y, const float src_z) : x(src_x), y(src_y), z(src_z) { /* custom constructor */ }

	inline const void normalize(void)
	{
		float len = length();

		if (0.0f != len)
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

	inline const float self_dot(void)
	{
		return x * x + y * y + z * z;
	}

	inline const float length(void)
	{
		return std::sqrtf(self_dot());
	}

	inline const indexed_vertex_3& cross(const indexed_vertex_3& right) const
	{
		static indexed_vertex_3 temp;

		temp.x = y * right.z - z * right.y;
		temp.y = z * right.x - x * right.z;
		temp.z = x * right.y - y * right.x;

		return temp;
	}

	inline const indexed_vertex_3& operator-(const indexed_vertex_3& right) const
	{
		static indexed_vertex_3 temp;

		temp.x = this->x - right.x;
		temp.y = this->y - right.y;
		temp.z = this->z - right.z;

		return temp;
	}

	inline const indexed_vertex_3& operator+(const indexed_vertex_3& right) const
	{
		static indexed_vertex_3 temp;

		temp.x = this->x + right.x;
		temp.y = this->y + right.y;
		temp.z = this->z + right.z;

		return temp;
	}

	inline bool operator<(const indexed_vertex_3& right) const
	{
		if (right.x > x)
			return true;
		else if (right.x < x)
			return false;

		if (right.y > y)
			return true;
		else if (right.y < y)
			return false;

		if (right.z > z)
			return true;
		else if (right.z < z)
			return false;

		return false;
	}

	inline void zero(void)
	{
		x = y = z = 0;
	}

	inline void rotate_x(const float& radians)
	{
		float t_y = y;

		y = t_y * cos(radians) + z * sin(radians);
		z = t_y * -sin(radians) + z * cos(radians);
	}

	inline void rotate_y(const float& radians)
	{
		float t_x = x;

		x = t_x * cos(radians) + z * -sin(radians);
		z = t_x * sin(radians) + z * cos(radians);
	}

	inline void rotate_z(const float& radians)
	{
		float t_x = x;

		x = t_x * cos(radians) + y * sin(radians);
		y = t_x * -sin(radians) + y * cos(radians);
	}

	bool operator==(const indexed_vertex_3& rhs)
	{
		if (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z)
			return true;

		return false;
	}

	bool operator!=(const indexed_vertex_3& rhs)
	{
		return !(*this == rhs);
	}

	float x, y, z;
};

class triangle
{
public:
	indexed_vertex_3 vertex[3];
};

bool read_triangles_from_binary_stereo_lithography_file(const char* const file_name, vector<triangle> &triangles)
{
	triangles.clear();

	// Write to file.
	ifstream in(file_name, ios_base::binary);

	if (in.fail())
		return false;

	const size_t header_size = 80;
	vector<char> buffer(header_size, 0);
	unsigned int num_triangles = 0; // Must be 4-byte unsigned int.
	indexed_vertex_3 normal;

	// Read header.
	in.read(reinterpret_cast<char*>(&(buffer[0])), header_size);

	if (header_size != in.gcount())
		return false;

	if (tolower(buffer[0]) == 's' &&
		tolower(buffer[1]) == 'o' &&
		tolower(buffer[2]) == 'l' &&
		tolower(buffer[3]) == 'i' &&
		tolower(buffer[4]) == 'd')
	{
		return false;
	}


	// Read number of triangles.
	in.read(reinterpret_cast<char*>(&num_triangles), sizeof(unsigned int));

	if (sizeof(unsigned int) != in.gcount())
		return false;

	triangles.resize(num_triangles);

	// Enough bytes for twelve 4-byte floats plus one 2-byte integer, per triangle.
	const size_t data_size = (12 * sizeof(float) + sizeof(short unsigned int)) * num_triangles;
	buffer.resize(data_size, 0);

	in.read(reinterpret_cast<char*>(&buffer[0]), data_size);

	if (data_size != in.gcount())
		return false;

	// Use a pointer to assist with the copying.
	// Should probably use std::copy() instead, but memcpy() does the trick, so whatever...
	char* cp = &buffer[0];

	for (vector<triangle>::iterator i = triangles.begin(); i != triangles.end(); i++)
	{
		// Skip face normal.
		cp += 3 * sizeof(float);

		// Get vertices.
		memcpy(&i->vertex[0].x, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[0].y, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[0].z, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[1].x, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[1].y, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[1].z, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[2].x, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[2].y, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[2].z, cp, sizeof(float)); cp += sizeof(float);

		// Skip attribute.
		cp += sizeof(short unsigned int);
	}

	in.close();

	return true;
}