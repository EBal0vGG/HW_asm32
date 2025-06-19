#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef double afunc(double);

double root(afunc *f, afunc *g, afunc *df, afunc *dg, double a, double b, double eps, int need_iter) {
    double fa = f(a) - g(a);
    double fb = f(b) - g(b);

    if (fa * fb > 0) {
        fprintf(stderr, "Нет смены знака на отрезке [%f, %f]\n", a, b);
        exit(1);
    }

    double x0 = a, x1 = b;
    int is_increasing = (fa < 0);

    // Проверка кривизны графика функции F(x) = f(x) - g(x)
    double mid = (a + b) / 2;
    double fmid = f(mid) - g(mid);
    double chord_mid = (fa + fb) / 2;
    int above_chord = (fmid > chord_mid);

    // Определяем знак f'(x) * f''(x)
    int d2_sign = (is_increasing && !above_chord) || (!is_increasing && above_chord);

    int max_iter = 100000;
    for (int i = 0; i < max_iter; ++i) {
        double fa_now = f(x0) - g(x0);
        double fb_now = f(x1) - g(x1);
        double denom = fb_now - fa_now;

        if (fabs(denom) < 1e-14) {
            fprintf(stderr, "Деление на слишком маленькое число!\n");
            exit(3);
        }

        // Метод хорд
        double chord = (x0 * fb_now - x1 * fa_now) / denom;

        // Метод касательных
        double d = d2_sign ? x1 : x0;
        double dfd = df(d);
        double dgd = dg(d);
        if (isnan(dfd) || isnan(dgd)) {
            fprintf(stderr, "df(d) or dg(d) is NaN at iter %d: d=%.10lf, df(d)=%.10lf, dg(d)=%.10lf\n", i, d, dfd, dgd);
            exit(8);
        }
        double deriv = dfd - dgd;

        if (isnan(deriv)) {
            fprintf(stderr, "deriv is NaN at iter %d: d=%.10lf, f(d)=%.10lf, g(d)=%.10lf\n", i, d, f(d), g(d));
            exit(6);
        }
        if (fabs(deriv) < 1e-14) {
            fprintf(stderr, "Производная слишком мала! deriv=%.10le, d=%.10lf, f(d)=%.10lf, g(d)=%.10lf\n", deriv, d, f(d), g(d));
            exit(4);
        }
        double fdgd = f(d) - g(d);
        if (isnan(fdgd)) {
            fprintf(stderr, "f(d)-g(d) is NaN at iter %d: d=%.10lf, f(d)=%.10lf, g(d)=%.10lf\n", i, d, f(d), g(d));
            exit(7);
        }

        double tangent = d - fdgd / deriv;

        // Проверка на NaN
        if (isnan(chord) || isnan(tangent) || isnan(x0) || isnan(x1)) {
            fprintf(stderr, "NaN detected at iter %d: chord=%.10lf, tangent=%.10lf, x0=%.10lf, x1=%.10lf\n", i, chord, tangent, x0, x1);
            exit(5);
        }

        // Комбинированное приближение
        double c0 = chord;
        double c1 = tangent;

        // Выбор нового отрезка
        double fc0 = f(c0) - g(c0);
        double fc1 = f(c1) - g(c1);
        double fx0 = f(x0) - g(x0);

        if (fc0 * fc1 <= 0) {
            x0 = c0;
            x1 = c1;
        } else if (fc0 * fx0 <= 0) {
            x1 = c0;
        } else {
            x0 = c1;
        }

        if (fabs(x1 - x0) < eps) {
            if (need_iter) {
                return i + 1;
            }
            return (x0 + x1) / 2;
        }
    }

    fprintf(stderr, "Не сошлось за %d итераций\n", max_iter);
    exit(2);
}

double integral(afunc *f, double a, double b, double eps) {
    int n = 10;
    double h = (b - a) / n;
    double I_prev = 0.0;
    double I = 0.0;

    // Начальное приближение
    I = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        I += f(a + i * h);
    }
    I *= h;

    // Уточнение результата
    while (1) {
        n *= 2;
        h = (b - a) / n;
        I_prev = I;
        double sum = 0.0;

        // Считаем только новые точки (в серединках старых отрезков)
        for (int i = 1; i < n; i += 2) {
            sum += f(a + i * h);
        }

        I = 0.5 * I + h * sum;

        // Рунге: погрешность < eps?
        if (fabs(I - I_prev) / 3.0 < eps)
            break;

        if (n > (1 << 20)) {
            fprintf(stderr, "integral: слишком много итераций\n");
            break;
        }
    }

    return I;
}

