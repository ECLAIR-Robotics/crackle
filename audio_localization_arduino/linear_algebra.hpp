#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <math.h>

struct Vector {
  float x, y, z;
  Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
  Vector operator+(const Vector& other) const;
  Vector operator-(const Vector& other) const;
  Vector operator*(const int& a) const;
  Vector operator/(const int& a) const;
};

struct Matrix {
  Vector row0, row1, row2;
  Matrix(Vector _row0, Vector _row1, Vector _row2) : row0(_row0), row1(_row1), row2(_row2) {};
};

float magnitude(Vector a);

Vector normalize(Vector a);

float dot(Vector a, Vector b);

Vector cross(Vector a, Vector b);

Matrix rotaion_matrix_around_axis(Vector axis, float theta);

Vector matrix_multiplication(Matrix m, Vector a);

#endif 