#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>  // ��������� ��� ���������� �������� ������
#include "Cubic_Interpolation_Spline_1D.h"
#include "Point.h"
#include <locale>

using namespace Com_Methods;

// ������������� ������� ��� ������������
double f(double x) {
    return 1.0 / x + 0.1;
}

// ������ ����������� ������������� �������
double f_prime(double x) {
    return -1.0 / (x * x);
}

// ������� ��� ���������� ������������
void calculate_and_store_errors(const std::vector<Point>& points, const std::vector<double>& f_values, double h, const Cubic_Interpolation_Spline_1D& spline, const std::string& file_prefix) {
    // ��������� ����� ��� ������
    std::ofstream spline_file(file_prefix + "_spline_data.txt");
    std::ofstream error_file(file_prefix + "_error_data.txt");

    if (!spline_file.is_open() || !error_file.is_open()) {
        std::cerr << "������ �������� ������!" << std::endl;
        return;
    }

    // ��������� ��� ������
    spline_file << std::fixed << std::setprecision(16); // ��������� ������� ������
    spline_file << "x\tg(x)\tg'(x)\tg''(x)\n";

    error_file << std::fixed << std::setprecision(16); // ��������� ������� ������
    error_file << "x\tabs(f(x) - g(x))\tabs(f'(x) - g'(x))\n";

    // ������������ ����� � �������� [0.5, 1] � ����� h
    for (double x = 0.5; x <= 1.0; x += h) {
        double res[3];   // ��� �������� �������� �������: g(x), g'(x), g''(x)
        spline.Get_Value(Point(x, 0.0, 0.0), res);

        // ��������� ���������
        double fx = f(x);
        double fx_prime = f_prime(x);

        // ���������� � ���� ������-�������������
        spline_file << x << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << "\n";

        // ��������� � ���������� �����������
        error_file << x << "\t"
            << std::abs(fx - res[0]) << "\t"
            << std::abs(fx_prime - res[1]) << "\n";
    }

    // ��������� �����
    spline_file.close();
    error_file.close();
}

int main() {
    setlocale(LC_ALL, "ru-RU");

    // ������ ����� (�����) ��� ������������
    std::vector<Point> points;
    std::vector<double> f_values;

    // ������� ����� ����� [0.5, 1] � ����� 0.1
    double h = 0.1;
    for (double x = 0.5; x <= 1.0; x += h) {
        points.push_back(Point(x, 0.0, 0.0));  // ���������� ������ ���������� x, y � z ���� �� �����
        f_values.push_back(f(x));              // �������� ������� � �����
    }

    // ������� ������ �������
    Cubic_Interpolation_Spline_1D spline;

    // ��������� ������, ��������� ����� � �������� �������
    spline.Update_Spline(points, f_values);

    // ��������� ������� ��� ��������� �����
    std::vector<double> steps = { 0.025, 0.0125, 0.00625 };

    for (size_t i = 0; i < steps.size(); ++i) {
        // ��� ������� ���� ������� ���������� ������� ��� ������
        std::string file_prefix = "step_" + std::to_string(i + 1);

        // ��������� �������� � ����������� ��� ������� ����
        calculate_and_store_errors(points, f_values, steps[i], spline, file_prefix);
    }

    std::cout << "������ ������� �������� � �����." << std::endl;
    return 0;
}