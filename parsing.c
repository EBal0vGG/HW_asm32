#include "getopt.h"
#include "stdio.h"
#include "root.h"
#include "func.h"
#include <string.h>
#include <stdlib.h>
#include "test_func.h"
#include <math.h>

typedef double(func)(double);

func* func_by_name(const char* s) {
    char num = (s[0] == 'f') ? s[1] : s[0];
    switch (num) {
        case '1':
            return f1;
        case '2':
            return f2;
        case '3':
            return f3;
        case '4':
            return f4;
        case '5':
            return f5;
        case '6':
            return f6;
        default:
            printf("Met unknown function: %s\n", s);
            exit(0);
    }
}

func* dfunc_by_name(const char* s) {
    char num = (s[0] == 'f') ? s[1] : s[0];
    switch (num) {
        case '1':
            return f1d;
        case '2':
            return f2d;
        case '3':
            return f3d;
        case '4':
            return f4d;
        case '5':
            return f5d;
        case '6':
            return f6d;
        default:
            printf("Met unknown function (derivative): %s\n", s);
            exit(0);
    }
}

void launch_root(char * s) {
    char* cur = strtok(s, ":");
    func* first = func_by_name(cur);
    func* dfirst = dfunc_by_name(cur);
    cur = strtok(0, ":");
    func* second = func_by_name(cur);
    func* dsecond = dfunc_by_name(cur);
    cur = strtok(0, ":");
    double a = atof(cur);
    cur = strtok(0, ":");
    double b = atof(cur);
    cur = strtok(0, ":");
    double eps = atof(cur);
    cur = strtok(0, ":");
    double ans = atof(cur);

    double fa = first(a) - second(a);
    double fb = first(b) - second(b);
    if (fa * fb > 0) {
        printf("Error: no sign change on the interval [%.6lf, %.6lf]\n", a, b);
        return;
    }

    double res = root(first, second, dfirst, dsecond, a, b, eps, 0);
    printf("%lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans/res - 1))*100);
}

void launch_integral(char * s) {
    char* cur = strtok(s, ":");
    func* first = func_by_name(cur);
    cur = strtok(0, ":");
    double a = atof(cur);
    cur = strtok(0, ":");
    double b = atof(cur);
    cur = strtok(0, ":");
    double eps = atof(cur);
    cur = strtok(0, ":");
    double ans = atof(cur);
    double res = integral(first, a, b, eps);
    printf("%lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans/res - 1))*100);
}

double diff_f1_f3(double x) { return f1(x) - f3(x); }
double diff_f2_f3(double x) { return f2(x) - f3(x); }

void parsing(int argc, char** argv) {
    // Восстановленная логика парсинга команд
    if (argc > 1) {
        double root12, root13, root23;
        double eps = 1e-5;
        static struct option long_options[] = {
            {"help", 0, 0, 0},
            {"root", 0, 0, 1},
            {"iterations", 0, 0, 2},
            {"test-root", 1, 0, 3},
            {"test-integral", 1, 0, 4},
        };
        int c;
        while (1) {
            c = getopt_long(argc, argv, "rihR:I:", long_options, 0);
            if (c == -1)
                break;
            switch (c) {
                case 0:
                case 'h':
                    printf("1. Опции --help и -h, которые выводят на печать все допустимые ключи\n"
                           "командной строки.\n"
                           "2. Опции --root и -r, которые печатают абсциссы точек пересечения кривых.\n"
                           "3. Опции --iterations и -i, которые печатают число итераций, потребовавшихся на приближенное решение уравнений при поиске точек пересечения.\n"
                           "4. Опции --test-root и -R, которые позволяют протестировать функцию\n"
                           "root. Фактические параметры вызова root задаются единственным параметром этой опции в виде F1:F2:A:B:E:R, где F1, F2 — номера используемых функций, A, B, E — значения параметров a, b, eps1 функции root,\n"
                           "R — правильный ответ (вычисленный аналитически). Программа должна\n"
                           "вызывать функцию root с указанными параметрами, сравнивать результат с правильным ответом и выводить на экран полученный результат,\n"
                           "абсолютную и относительную ошибку.\n"
                           "5. Опции --test-integral и -I, которые позволяют протестировать функцию integral. Фактические параметры вызова integral задаются единственным параметром этой опции в виде F:A:B:E:R, где F — номера используемой функций, A, B, E — значения параметров a, b, eps2 функции\n"
                           "integral, R — правильный ответ (вычисленный аналитически). Программа должна вызывать функцию integral с указанными параметрами, сравнивать результат с правильным ответом и выводить на экран полученный\n"
                           "результат, абсолютную и относительную ошибку.\n"
                           "6. Программа, запущенная без входных параметров, должна выводить ответ\n"
                           "на поставленную задачу (площадь фигуры).\n");
                    return;
                case 1:
                case 'r':
                    root12 = root(f1, f2, f1d, f2d, 1.5, 2.5, eps, 0);
                    root13 = root(f1, f3, f1d, f3d, -0.4, 0.0, eps, 0);
                    root23 = root(f2, f3, f2d, f3d, 0.0, 0.5, eps, 0);
                    printf("f1 intersects f2 at the point %lf\n", root12);
                    printf("f1 intersects f3 at the point %lf\n", root13);
                    printf("f2 intersects f3 at the point %lf\n", root23);
                    break;
                case 2:
                case 'i':
                    int it1 = root(f1, f2, f1d, f2d, 1.5, 2.5, eps, 1);
                    int it2 = root(f1, f3, f1d, f3d, -0.4, 0.0, eps, 1);
                    int it3 = root(f2, f3, f2d, f3d, 0.0, 0.5, eps, 1);
                    printf("Iterations for root f1=f2: %d\n", it1);
                    printf("Iterations for root f1=f3: %d\n", it2);
                    printf("Iterations for root f2=f3: %d\n", it3);
                    break;
                case 3:
                case 'R':
                    launch_root(optarg);
                    return;
                case 4:
                case 'I':
                    launch_integral(optarg);
                    return;
                default:
                    printf("Meet unknown parameter\n");
            }
        }
        return;
    }

    // Основной режим: вычисление площади фигуры
    const double eps_root = 1e-5;
    const double eps_int = 1e-4;
    double x12 = root(f1, f2, f1d, f2d, 1.5, 2.5, eps_root, 0);
    double x13 = root(f1, f3, f1d, f3d, -0.4, 0.0, eps_root, 0);
    double x23 = root(f2, f3, f2d, f3d, 0.0, 0.5, eps_root, 0);
    // Упорядочим корни по возрастанию
    double x[3] = { x12, x13, x23 };
    for (int i = 0; i < 2; ++i)
        for (int j = i + 1; j < 3; ++j)
            if (x[i] > x[j]) {
                double tmp = x[i]; x[i] = x[j]; x[j] = tmp;
            }
    double x1 = x[0], x2 = x[1], x3 = x[2];
    // Интегралы разностей (площадь между кривыми)
    double s1 = integral(diff_f1_f3, x1, x2, eps_int);
    double s2 = integral(diff_f2_f3, x2, x3, eps_int);
    double total_area = s1 + s2;
    // Финальный вывод
    printf("Площадь фигуры: %.6f\n", total_area);
}
