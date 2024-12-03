#include "linear_algebra.hpp"

Vector Vector::operator+(const Vector& other) const {
  return Vector(
    x + other.x,
    y + other.y,
    z + other.z
  );
}

Vector Vector::operator-(const Vector& other) const {
  return Vector(
    x - other.x,
    y - other.y,
    z - other.z
  );
}

Vector Vector::operator*(const int& a) const {
  return Vector(
    x * a,
    y * a,
    z * a
  );
}

Vector Vector::operator/(const int& a) const {
  return Vector(
    x / a,
    y / a,
    z / a
  );
}

float magnitude(Vector a) {
  return sqrt(pow(a.x,2) + pow(a.y,2) + pow(a.z,2));
}

Vector normalize(Vector a) {
  float m = magnitude(a);
  return Vector(
    a.x/m,
    a.y/m,
    a.z/m
  );
}

float dot(Vector a, Vector b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector cross(Vector a, Vector b) {
  return Vector(
    a.y*b.z - a.z*b.y,
    a.z*b.x - a.x*b.z,
    a.x*b.y - a.y*b.x
  );
}

Matrix rotaion_matrix_around_axis(Vector axis, float theta) {
  // https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle

  float sin_theta = sin(theta);
  float cos_theta = cos(theta);

  return Matrix(
    Vector(
      pow(axis.x,2)*(1-cos_theta) + cos_theta,
      axis.x*axis.y*(1-cos_theta) - axis.z*sin_theta,
      axis.x*axis.z*(1-cos_theta) + axis.y*sin_theta
    ),
    Vector(
      axis.x*axis.y*(1-cos_theta) + axis.z*sin_theta,
      pow(axis.y,2)*(1-cos_theta) + cos_theta,
      axis.y*axis.z*(1-cos_theta) - axis.x*sin_theta
    ),
    Vector(
      axis.x*axis.z*(1-cos_theta) - axis.y*sin_theta,
      axis.y*axis.z*(1-cos_theta) + axis.x*sin_theta,
      pow(axis.z,2)*(1-cos_theta) + cos_theta
    )
  );
}

Vector matrix_multiplication(Matrix m, Vector a) {
  return Vector(
    m.row0.x*a.x + m.row0.y*a.y + m.row0.z*a.z,
    m.row1.x*a.x + m.row1.y*a.y + m.row1.z*a.z,
    m.row2.x*a.x + m.row2.y*a.y + m.row2.z*a.z
  );
}