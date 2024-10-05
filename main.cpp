#include <iostream>
#include <cmath>
#include <vector>
#include "Point.h"
#include "Cubic_Interpolation_Spline_1D.h"
#include "Smoothing_Spline_1D.h"
#include <fstream>
#include <iomanip>

std::vector<Com_Methods::Point> Generate_Mesh(double a, double b, int Num_Segment) {
	std::vector<Com_Methods::Point> Mesh;
	double h = 0;
	double Mesh_Node = a;

	h = (b - a) / Num_Segment;

	for (int i = 0; i < Num_Segment + 1; i++) {
		Mesh.push_back(Com_Methods::Point(Mesh_Node, 0.0, 0.0));
		Mesh_Node += h;
	}

	return Mesh;
}

std::vector<double> Get_Func() {
	std::vector<double> Func;
	std::ifstream file("func.txt");
	if (file.is_open())
	{
		double f;

		while (file >> f) {
			Func.push_back(f);
		}
	}
	else
		std::cout << "error" << std::endl;
	file.close();

	return Func;
}

int main() {
	try {
		std::vector<Com_Methods::Point> Mesh = Generate_Mesh(1, 1363, 1362);

		std::vector<double> Func = Get_Func();

		double Res[3];

		Com_Methods::Cubic_Interpolation_Spline_1D Spline;
		Spline.Update_Spline(Mesh, Func);

		Com_Methods::Smoothing_Spline_1D Smoothing_Spline(0.5);
		Smoothing_Spline.Update_Spline(Mesh, Func);

		std::ofstream file("smoothing_spline_1d.txt");
		for (int i = 0; i < Func.size(); i++) {
			Smoothing_Spline.Get_Value(Mesh[i], Res);
			file << std::fixed << std::setprecision(9) << Res[0] << std::endl;
		}
		file.close();
	}
	catch (std::exception& Ex) {
		std::cout << "Error" << Ex.what() << std::endl;
	}
}