#include "main.h"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " file_a.stl file_b.stl" << endl;
		return 0;
	}

	vector<triangle> ta, tb;

	read_triangles_from_binary_stereo_lithography_file(argv[1], ta);
	read_triangles_from_binary_stereo_lithography_file(argv[2], tb);

	if (ta.size() != tb.size())
	{
		cout << "Triangle count mismatch" << endl;
		return 0;
	}

	set<float> diffs;

	for (size_t i = 0; i < ta.size(); i++)
	{
		if (ta[i].vertex[0] != tb[i].vertex[0])
		{
			float x_diff = fabs(ta[i].vertex[0].x - tb[i].vertex[0].x);
			float y_diff = fabs(ta[i].vertex[0].y - tb[i].vertex[0].y);
			float z_diff = fabs(ta[i].vertex[0].z - tb[i].vertex[0].z);

			diffs.insert(x_diff);
			diffs.insert(y_diff);
			diffs.insert(z_diff);

			//cout << "vertex 0 mismatch " << i << " of " << ta.size() << endl;
			//cout << "diff: " << x_diff << " " << y_diff << " " << z_diff << endl;
			//cout << endl;
		}

		if (ta[i].vertex[1] != tb[i].vertex[1])
		{
			float x_diff = fabs(ta[i].vertex[1].x - tb[i].vertex[1].x);
			float y_diff = fabs(ta[i].vertex[1].y - tb[i].vertex[1].y);
			float z_diff = fabs(ta[i].vertex[1].z - tb[i].vertex[1].z);

			diffs.insert(x_diff);
			diffs.insert(y_diff);
			diffs.insert(z_diff);

			//cout << "vertex 1 mismatch " << i << " of " << ta.size() << endl;
			//cout << "diff: " << x_diff << " " << y_diff << " " << z_diff << endl;
			//cout << endl;
		}

		if (ta[i].vertex[2] != tb[i].vertex[2])
		{
			float x_diff = fabs(ta[i].vertex[2].x - tb[i].vertex[2].x);
			float y_diff = fabs(ta[i].vertex[2].y - tb[i].vertex[2].y);
			float z_diff = fabs(ta[i].vertex[2].z - tb[i].vertex[2].z);

			diffs.insert(x_diff);
			diffs.insert(y_diff);
			diffs.insert(z_diff);

			//cout << "vertex 2 mismatch " << i << " of " << ta.size() << endl;
			//cout << "diff: " << x_diff << " " << y_diff << " " << z_diff << endl;
			//cout << endl;
		}
	}

	if (diffs.size() != 0)
	{
		cout << "Min diff: " << *diffs.begin() << endl;
		cout << "Max diff: " << *diffs.rbegin() << endl;
	}

	return 0;

}