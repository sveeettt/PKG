#ifndef COLORTRANSFORMER_H
#define COLORTRANSFORMER_H

class ColorTransformer
{
public:
    static void hsvToXyz(double h, double s, double v, double &x, double &y, double &z);
    static void xyzToHsv(double x, double y, double z, double &h, double &s, double &v);
    static void xyzToLab(double x, double y, double z, double &l, double &a, double &b);
    static void labToXyz(double l, double a, double b, double &x, double &y, double &z);

private:
    static const double X_REF;// белая точка для LAB
    static const double Y_REF;
    static const double Z_REF;
    static const double EPSILON;
    static const double KAPPA;

    static double labF(double t);//нелинейное преобр
    static double labFInv(double t);//обратная функция преобразования
};

#endif // COLORTRANSFORMER_H
