#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <math.h>

struct Vector {
  double x, y, z;
  Vector() : x(0), y(0), z(0) {};
  Vector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
  Vector operator+(const Vector& other) const;
  Vector operator-(const Vector& other) const;
  Vector operator*(const int& a) const;
  Vector operator/(const int& a) const;
};

struct Matrix {
  Vector row0, row1, row2;
  Matrix(Vector _row0, Vector _row1, Vector _row2) : row0(_row0), row1(_row1), row2(_row2) {};
};

double magnitude(Vector a);

Vector normalize(Vector a);

double dot(Vector a, Vector b);

Vector cross(Vector a, Vector b);

Matrix rotaion_matrix_around_axis(Vector axis, double theta);

Vector matrix_multiplication(Matrix m, Vector a);

#endif 