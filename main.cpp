#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>  // Добавлено для управления форматом вывода
#include "Cubic_Interpolation_Spline_1D.h"
#include "Point.h"
#include <locale>

using namespace Com_Methods;

// Аналитическая функция для тестирования
double f(double x) {
    return 1.0 / x + 0.1;
}

// Первая производная аналитической функции
double f_prime(double x) {
    return -1.0 / (x * x);
}

// Функция для вычисления погрешностей
void calculate_and_store_errors(const std::vector<Point>& points, const std::vector<double>& f_values, double h, const Cubic_Interpolation_Spline_1D& spline, const std::string& file_prefix) {
    // Открываем файлы для записи
    std::ofstream spline_file(file_prefix + "_spline_data.txt");
    std::ofstream error_file(file_prefix + "_error_data.txt");

    if (!spline_file.is_open() || !error_file.is_open()) {
        std::cerr << "Ошибка открытия файлов!" << std::endl;
        return;
    }

    // Заголовки для файлов
    spline_file << std::fixed << std::setprecision(16); // Установка формата вывода
    spline_file << "x\tg(x)\tg'(x)\tg''(x)\n";

    error_file << std::fixed << std::setprecision(16); // Установка формата вывода
    error_file << "x\tabs(f(x) - g(x))\tabs(f'(x) - g'(x))\n";

    // Обрабатываем точки в пределах [0.5, 1] с шагом h
    for (double x = 0.5; x <= 1.0; x += h) {
        double res[3];   // Для хранения значений сплайна: g(x), g'(x), g''(x)
        spline.Get_Value(Point(x, 0.0, 0.0), res);

        // Вычисляем аналитику
        double fx = f(x);
        double fx_prime = f_prime(x);

        // Записываем в файл сплайн-аппроксимацию
        spline_file << x << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << "\n";

        // Вычисляем и записываем погрешности
        error_file << x << "\t"
            << std::abs(fx - res[0]) << "\t"
            << std::abs(fx_prime - res[1]) << "\n";
    }

    // Закрываем файлы
    spline_file.close();
    error_file.close();
}

int main() {
    setlocale(LC_ALL, "ru-RU");

    // Массив точек (узлов) для интерполяции
    std::vector<Point> points;
    std::vector<double> f_values;

    // Создаем сетку узлов [0.5, 1] с шагом 0.1
    double h = 0.1;
    for (double x = 0.5; x <= 1.0; x += h) {
        points.push_back(Point(x, 0.0, 0.0));  // Используем только координату x, y и z пока не нужны
        f_values.push_back(f(x));              // Значения функции в узлах
    }

    // Создаем объект сплайна
    Cubic_Interpolation_Spline_1D spline;

    // Обновляем сплайн, используя точки и значения функции
    spline.Update_Spline(points, f_values);

    // Выполняем расчеты для различных шагов
    std::vector<double> steps = { 0.025, 0.0125, 0.00625 };

    for (size_t i = 0; i < steps.size(); ++i) {
        // Для каждого шага создаем уникальный префикс для файлов
        std::string file_prefix = "step_" + std::to_string(i + 1);

        // Вычисляем значения и погрешности для каждого шага
        calculate_and_store_errors(points, f_values, steps[i], spline, file_prefix);
    }

    std::cout << "Данные успешно записаны в файлы." << std::endl;
    return 0;
}