#pragma once

double root(double (*F) (double), double (*G) (double), double (*dF)(double), double (*dG)(double), double a, double b, double eps1, int need_iter);

double integral(double (*F) (double), double a, double b, double eps2);