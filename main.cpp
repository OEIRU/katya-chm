#include <iostream>
#include <cmath>
#include <vector>
#include "Point.h"
#include "Cubic_Interpolation_Spline_1D.h"
#include "Smoothing_Spline_1D.h"
#include <fstream>
#include <iomanip>

int main() {
    try {
        // Создаем вектор точек и соответствующих значений функции
        std::vector<Com_Methods::Point> Mesh = {
            Com_Methods::Point(0.25, 0.0, 0.0),
            Com_Methods::Point(0.5, 0.0, 0.0),
            Com_Methods::Point(1.0, 0.0, 0.0)
        };

        std::vector<double> Func = { 4.1, 2.1, 1.1 }; // Значения f(x) для каждой точки

        // Создаем сплайн
        Com_Methods::Cubic_Interpolation_Spline_1D Spline;
        Spline.Update_Spline(Mesh, Func);

        // Вычисляем значения сплайна в заданных точках
        double Res[3];

        // Пример использования сплайна на точках 0.25, 0.5 и 1.0
        for (double x : {0.25, 0.5, 1.0}) {
            Com_Methods::Point P(x, 0.0, 0.0);
            Spline.Get_Value(P, Res);
            std::cout << "f(" << x << ") = " << Res[0] << ", f'(" << x << ") = " << Res[1] << ", f''(" << x << ") = " << Res[2] << std::endl;
        }

        // Также можно вычислить и вывести значения функции f(x) = x^{-1} + 0.1 для проверки
        std::cout << "Function values:" << std::endl;
        for (double x : {0.25, 0.5, 1.0}) {
            std::cout << "f(" << x << ") = " << (1.0 / x + 0.1) << std::endl;
        }
    }
    catch (std::exception& Ex) {
        std::cout << "Error: " << Ex.what() << std::endl;
    }

    return 0;
}
