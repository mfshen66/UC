//#pragma once
#ifndef _vector_h_
#define _vector_h_

////////////////////////////////////////////////////////////////

double mathGetDist(PNT3D p1, PNT3D p2);
void mathGetVec2D(PNT2D p1, PNT2D p2, VEC2D v); // ¶þÎ¬ÏòÁ¿
void mathMixPnt(PNT3D p1, PNT3D p2, double w1, double w2, PNT3D mix_p);
double mathMProduct(VEC3D v1, VEC3D v2, VEC3D v3);
void mathGetMidPnt(PNT3D p1, PNT3D p2, PNT3D mid);
int mathGetVecUnit(PNT3D p1, PNT3D p2, VEC3D v);
int mathGetVecUnit2D(PNT2D p1, PNT2D p2, VEC2D v);
double mathGetAngle(VEC3D v1, VEC3D v2, double min_len);

void mathRevVec(VEC3D v);
int mathVProductUnit(VEC3D v1, VEC3D v2, VEC3D rtvec);

#endif // !_vector_h_