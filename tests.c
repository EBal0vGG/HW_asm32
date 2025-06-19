#include "stdio.h"
#include "root.h"
#include "test_func.h"
#include <math.h>

typedef double(func)(double);

int main(int argc, char **argv) {
    // Тест 1: корень между f4 и f5 (2x-3 и -2x+3) на [1,2] — должен быть 1.5
    double ans = 1.5;
    double res = root(f4, f5, f4d, f5d, 1, 2, 1e-6, 0);
    printf("test 1: %lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans/res - 1))*100);

    // Тест 2: корень между f6 и f5 (3 и -2x+3) на [-1,1] — должен быть 0
    ans = 0;
    res = root(f6, f5, f6d, f5d, -1, 1, 1e-6, 0);
    printf("test 2: %lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans-res/(res+1e-6)))*100);

    // Тест 3: корень между f6 и f4 (3 и 2x-3) на [2,4] — должен быть 3
    ans = 3;
    res = root(f6, f4, f6d, f4d, 2, 4, 1e-6, 0);
    printf("test 3: %lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans/res - 1))*100);

    // Тест 4: интеграл f6 (3) на [1,2] — должен быть 3
    ans = 3;
    res = integral(f6, 1, 2, 1e-6);
    printf("test 4: %lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans/res - 1))*100);

    // Тест 5: интеграл f4 (2x-3) на [1,3] — должен быть 2
    ans = 2;
    res = integral(f4, 1, 3, 1e-6);
    printf("test 5: %lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans/res - 1))*100);

    // Тест 6: интеграл f5 (-2x+3) на [1,3] — должен быть -2
    ans = -2;
    res = integral(f5, 1, 3, 1e-6);
    printf("test 6: %lf %lf %lf%%\n", ans, fabs(ans - res), (fabs(ans/res - 1))*100);
}
