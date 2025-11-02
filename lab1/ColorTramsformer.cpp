#include "ColorTransformer.h"
#include <cmath>
#include <algorithm>

const double ColorTransformer::X_REF = 95.047;
const double ColorTransformer::Y_REF = 100.000;
const double ColorTransformer::Z_REF = 108.883;
const double ColorTransformer::EPSILON = 0.008856;
const double ColorTransformer::KAPPA = 903.3;

double ColorTransformer::labF(double t)//прямое
{
    if (t > EPSILON) {
        return std::cbrt(t);
    } else {
        return (KAPPA * t + 16.0) / 116.0;
    }
}

double ColorTransformer::labFInv(double t)//обратное
{
    double t3 = t * t * t;
    if (t3 > EPSILON) {
        return t3;
    } else {
        return (116.0 * t - 16.0) / KAPPA;
    }
}

void ColorTransformer::hsvToXyz(double h, double s, double v, double &x, double &y, double &z)
{
    s /= 100.0;
    v /= 100.0;

  //HSV to RGB
    double c = v * s;
    double h_prime = h / 60.0;
    double x_val = c * (1 - std::abs(std::fmod(h_prime, 2.0) - 1));

    double r, g, b;
    if (h_prime >= 0 && h_prime < 1) {
        r = c; g = x_val; b = 0;
    } else if (h_prime < 2) {
        r = x_val; g = c; b = 0;
    } else if (h_prime < 3) {
        r = 0; g = c; b = x_val;
    } else if (h_prime < 4) {
        r = 0; g = x_val; b = c;
    } else if (h_prime < 5) {
        r = x_val; g = 0; b = c;
    } else {
        r = c; g = 0; b = x_val;
    }

    double m = v - c;
    r += m;
    g += m;
    b += m;

    //RGB to linear
    r = (r > 0.04045) ? std::pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
    g = (g > 0.04045) ? std::pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
    b = (b > 0.04045) ? std::pow((b + 0.055) / 1.055, 2.4) : b / 12.92;

    r *= 100.0;
    g *= 100.0;
    b *= 100.0;

    // Convert to XYZ
    x = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
    y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
    z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
}

void ColorTransformer::xyzToHsv(double x, double y, double z, double &h, double &s, double &v)
{
    //XYZ to RGB
    double r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314;
    double g = x * -0.9692660 + y * 1.8760108 + z * 0.0415560;
    double b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252;

    r /= 100.0;
    g /= 100.0;
    b /= 100.0;

  // обратная гамма-коррекция
    r = (r > 0.0031308) ? (1.055 * std::pow(r, 1.0/2.4) - 0.055) : 12.92 * r;
    g = (g > 0.0031308) ? (1.055 * std::pow(g, 1.0/2.4) - 0.055) : 12.92 * g;
    b = (b > 0.0031308) ? (1.055 * std::pow(b, 1.0/2.4) - 0.055) : 12.92 * b;


    r = std::max(0.0, std::min(1.0, r));
    g = std::max(0.0, std::min(1.0, g));
    b = std::max(0.0, std::min(1.0, b));

    //RGB to HSV
    double max = std::max(std::max(r, g), b);
    double min = std::min(std::min(r, g), b);
    double delta = max - min;

    v = max * 100.0;

    if (max == 0 || delta == 0) {
        h = 0;
        s = 0;
    } else {
        s = (delta / max) * 100.0;

        if (max == r) {
            h = 60.0 * std::fmod((g - b) / delta, 6.0);
        } else if (max == g) {
            h = 60.0 * (((b - r) / delta) + 2.0);
        } else {
            h = 60.0 * (((r - g) / delta) + 4.0);
        }

        if (h < 0) h += 360.0;
    }
}

void ColorTransformer::xyzToLab(double x, double y, double z, double &l, double &a, double &b)
{ //нормализация к белой точке
    double x_n = x / X_REF;
    double y_n = y / Y_REF;
    double z_n = z / Z_REF;

    // Применение нелинейной функции
    double fx = labF(x_n);
    double fy = labF(y_n);
    double fz = labF(z_n);

    l = 116.0 * fy - 16.0;
    a = 500.0 * (fx - fy);
    b = 200.0 * (fy - fz);
}

void ColorTransformer::labToXyz(double l, double a, double b_val, double &x, double &y, double &z)
{
    double fy = (l + 16.0) / 116.0;
    double fx = fy + (a / 500.0);
    double fz = fy - (b_val / 200.0);

    double x_n = labFInv(fx);
    double y_n = labFInv(fy);
    double z_n = labFInv(fz);

    x = x_n * X_REF;
    y = y_n * Y_REF;
    z = z_n * Z_REF;
}
