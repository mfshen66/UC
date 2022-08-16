#include "StdAfx.h"
#include "PArray.h"

////////////////////////////////////////////////////////////////
PARRAY* parrayCreate(int size, double w)
{
	int i;
	PARRAY* parray = (PARRAY*)malloc(sizeof(PARRAY));
	if (parray == NULL)
		return NULL;
	parray->xmin = 0.;
	parray->xmax = 0.;
	parray->ymin = 0.;
	parray->ymax = 0.;
	parray->w = w;
	parray->nx = 0;
	parray->ny = 0;
	parray->size = size;
	if (size > 0)
	{
		parray->pixels = (uchar*)malloc(size * sizeof(uchar));
		for (i = 0; i < size; i++)
			parray->pixels[i] = 0;
	}
	else
		parray->pixels = NULL; // nt add 2017/10/26

	return parray;
}

void parrayFree(PARRAY* parray)
{
	if (parray)
	{
		if (parray->pixels)
			free(parray->pixels);
		free(parray);
	}

	return;
}

int parrayMergeZBZB2(PARRAY* parray, ZB* zb, ZB2* zb2)
{
	if (parray == NULL)
		return 0;
	if (zb == NULL &&
		zb2 == NULL)
		return 0;

	parray->xmin = MAX_DBL;
	parray->ymin = MAX_DBL;
	parray->xmax = -MAX_DBL;
	parray->ymax = -MAX_DBL;
	if (zb)
	{
		parray->xmin = mathMIN(zb->xmin, parray->xmin);
		parray->ymin = mathMIN(zb->ymin, parray->ymin);
		parray->xmax = mathMAX(zb->xmax, parray->xmax);
		parray->ymax = mathMAX(zb->ymax, parray->ymax);
	}
	if (zb2)
	{
		parray->xmin = mathMIN(zb2->xmin, parray->xmin);
		parray->ymin = mathMIN(zb2->ymin, parray->ymin);
		parray->xmax = mathMAX(zb2->xmax, parray->xmax);
		parray->ymax = mathMAX(zb2->ymax, parray->ymax);
	}
	parray->nx = (int)ceil((parray->xmax - parray->xmin) / parray->w);
	parray->ny = (int)ceil((parray->ymax - parray->ymin) / parray->w);
	int size = parray->nx*parray->ny;
	if (size > parray->size)
	{
		if (parray->pixels)
			free(parray->pixels);
		parray->size = size;
		parray->pixels = (uchar*)malloc(size * sizeof(uchar));
		if (parray->pixels == NULL)
			return 0;
	}
	for (int s = 0; s < size; s++)
		parray->pixels[s] = 0;

	uchar c;
	int i, j, k, I, J;
	double x, y;
	if (zb)
	{
		k = 0;
		for (j = 0; j < zb->ny; j++)
		{
			y = zb->ymin + zb->w*j;
			J = (int)((y - parray->ymin) / parray->w + 0.5);
			if (J < 0)
				J = 0;
			else
			if (J >= parray->ny)
				J = parray->ny - 1;
			for (i = 0; i < zb->nx; i++)
			{
				x = zb->xmin + zb->w*i;
				I = (int)((x - parray->xmin) / parray->w + 0.5);
				if (I < 0)
					I = 0;
				else
				if (I >= parray->nx)
					I = parray->nx - 1;
				c = zb->pixels[k]; ;
				if (c > 0)
					parray->pixels[parray->nx*J + I] = 255;
				k++;
			}
		}
	}

	if (zb2)
	{
		k = 0;
		for (j = 0; j < zb2->ny; j++)
		{
			y = zb2->ymin + zb2->w*j;
			J = (int)((y - parray->ymin) / parray->w + 0.5);
			if (J < 0)
				J = 0;
			else
				if (J >= parray->ny)
					J = parray->ny - 1;
			for (i = 0; i < zb2->nx; i++)
			{
				x = zb2->xmin + zb2->w*i;
				I = (int)((x - parray->xmin) / parray->w + 0.5);
				if (I < 0)
					I = 0;
				else
				if (I >= parray->nx)
					I = parray->nx - 1;
				c = zb2->pixels[k]; ;
				if (c > 0)
					parray->pixels[parray->nx*J + I] = 255;
				k++;
			}
		}
	}

	return 1;
}

// nt add 2021/12/29
int parrayMergeZB2ZB2(PARRAY* parray, ZB2* zb1, ZB2* zb2)
{
	if (parray == NULL)
		return 0;
	if (zb1 == NULL &&
		zb2 == NULL)
		return 0;

	parray->xmin = MAX_DBL;
	parray->ymin = MAX_DBL;
	parray->xmax = -MAX_DBL;
	parray->ymax = -MAX_DBL;
	if (zb1)
	{
		parray->xmin = mathMIN(zb1->xmin, parray->xmin);
		parray->ymin = mathMIN(zb1->ymin, parray->ymin);
		parray->xmax = mathMAX(zb1->xmax, parray->xmax);
		parray->ymax = mathMAX(zb1->ymax, parray->ymax);
	}
	if (zb2)
	{
		parray->xmin = mathMIN(zb2->xmin, parray->xmin);
		parray->ymin = mathMIN(zb2->ymin, parray->ymin);
		parray->xmax = mathMAX(zb2->xmax, parray->xmax);
		parray->ymax = mathMAX(zb2->ymax, parray->ymax);
	}
	parray->nx = (int)ceil((parray->xmax - parray->xmin) / parray->w);
	parray->ny = (int)ceil((parray->ymax - parray->ymin) / parray->w);
	int size = parray->nx*parray->ny;
	if (size > parray->size)
	{
		if (parray->pixels)
			free(parray->pixels);
		parray->size = size;
		parray->pixels = (uchar*)malloc(size * sizeof(uchar));
		if (parray->pixels == NULL)
			return 0;
	}
	for (int s = 0; s < size; s++)
		parray->pixels[s] = 0;

	uchar c;
	int i, j, k, I, J;
	double x, y;
	if (zb1)
	{
		k = 0;
		for (j = 0; j < zb1->ny; j++)
		{
			y = zb1->ymin + zb1->w*j;
			J = (int)((y - parray->ymin) / parray->w + 0.5);
			if (J < 0)
				J = 0;
			else
			if (J >= parray->ny)
				J = parray->ny - 1;
			for (i = 0; i < zb1->nx; i++)
			{
				x = zb1->xmin + zb1->w*i;
				I = (int)((x - parray->xmin) / parray->w + 0.5);
				if (I < 0)
					I = 0;
				else
				if (I >= parray->nx)
					I = parray->nx - 1;
				c = zb1->pixels[k]; ;
				if (c > 0)
					parray->pixels[parray->nx*J + I] = 255;
				k++;
			}
		}
	}

	if (zb2)
	{
		k = 0;
		for (j = 0; j < zb2->ny; j++)
		{
			y = zb2->ymin + zb2->w*j;
			J = (int)((y - parray->ymin) / parray->w + 0.5);
			if (J < 0)
				J = 0;
			else
			if (J >= parray->ny)
				J = parray->ny - 1;
			for (i = 0; i < zb2->nx; i++)
			{
				x = zb2->xmin + zb2->w*i;
				I = (int)((x - parray->xmin) / parray->w + 0.5);
				if (I < 0)
					I = 0;
				else
				if (I >= parray->nx)
					I = parray->nx - 1;
				c = zb2->pixels[k]; ;
				if (c > 0)
					parray->pixels[parray->nx*J + I] = 255;
				k++;
			}
		}
	}

	return 1;
}

uchar parrayGetPixel(PARRAY* parray, int i, int j)
{
	return parray->pixels[parray->nx*j + i];
}

void parraySetPixel(PARRAY* parray, int i, int j, uchar pixel)
{
	parray->pixels[parray->nx*j + i] = pixel;

	return;
}

int parrayGray(PARRAY* parray, int grays[4]) // 原有边缘灰度处理办法
{
	uchar p, c; // p:previous, c:current
	int i, j, k;

	if (parray)
	{
		k = 0;
		for (j = 0; j < parray->ny; j++)
		{
			p = 0; // initial value
			c = 0; // initial value
			for (i = 0; i < parray->nx; i++)
			{
				c = parray->pixels[k];
				if (p == 0 && c == 255)
				{
					if (i >= 2)
						parray->pixels[k - 2] = (uchar)(255 * grays[3] / 100);
					if (i >= 1)
						parray->pixels[k - 1] = (uchar)(255 * grays[2] / 100);
					parray->pixels[k] = (uchar)(255 * grays[1] / 100);
					if (i + 1 < parray->nx)
						parray->pixels[k + 1] = (uchar)(255 * grays[0] / 100);
				}
				else
					if (p == 255 && c == 0)
					{
						if (i >= 2)
							parray->pixels[k - 2] = (uchar)(255 * grays[0] / 100);
						if (i >= 1)
							parray->pixels[k - 1] = (uchar)(255 * grays[1] / 100);
						parray->pixels[k] = (uchar)(255 * grays[2] / 100);
						if (i + 1 < parray->nx)
							parray->pixels[k + 1] = (uchar)(255 * grays[3] / 100);
					}
				p = c;
				k++;
			}
		}

		for (i = 0; i < parray->nx; i++)
		{
			p = 0; // initial value
			c = 0; // initial value
			for (j = 0; j < parray->ny; j++)
			{
				k = j * parray->nx + i;
				c = parray->pixels[k];
				if (p == 0 && c == 255)
				{
					if (j >= 2)
						parray->pixels[(j - 2)*parray->nx + i] = (uchar)(255 * grays[3] / 100);
					if (j >= 1)
						parray->pixels[(j - 1)*parray->nx + i] = (uchar)(255 * grays[2] / 100);
					parray->pixels[k] = (uchar)(255 * grays[1] / 100);
					if (j + 1 < parray->ny)
						parray->pixels[(j + 1)*parray->nx + i] = (uchar)(255 * grays[0] / 100);
				}
				else
					if (p == 255 && c == 0)
					{
						if (j >= 2)
							parray->pixels[(j - 2)*parray->nx + i] = (uchar)(255 * grays[0] / 100);
						if (j >= 1)
							parray->pixels[(j - 1)*parray->nx + i] = (uchar)(255 * grays[1] / 100);
						parray->pixels[k] = (uchar)(255 * grays[2] / 100);
						if (j + 1 < parray->ny)
							parray->pixels[(j + 1)*parray->nx + i] = (uchar)(255 * grays[3] / 100);
					}
				p = c;
				k++;
			}
		}
	}

	return 1;
}


int parrayGray1(PARRAY* parray, int grays[4])
// 5*5模板进行卷积
// parray 所有的像素点值均为 255，统计中心像素周围有多少个值为 255 的点，根据点数，划分灰度等级
// ||----0--- ||-----grays[3]----||------grays[2]-------||------grays[1]--------||---------------grays[0]----------||                                    
// ||0  1  2  || 3  4  5  6  7   || 8	9	10	11	12  || 13	14	15	16	17  || 18  19  20  21  22  23  24  25  ||     // 255 像素点个数
// ||0  10 20 || 31 41 51 61 71  || 82  92  102 112 122 || 133  143 153 163 173 || 184 194 204 214 224 235 245 255 ||     // 卷积之后中心元素的像素值
// ||0  0  1  || 1  2  3  3  4   || 5   5   6   7   7   || 8    8   9   10  10  || 11  12  12  13  14  14  15  15  ||     // 放入layer 右移4位之后的结果
//    
{
	// 灰度等级和相邻灰度像素的个数
	int grayLevel[26] = {
		0,        0,        0,
		grays[3], grays[3], grays[3], grays[3], grays[3],
		grays[2], grays[2], grays[2], grays[2], grays[2],
		grays[1], grays[1], grays[1], grays[1], grays[1], 
		grays[0], grays[0], grays[0], grays[0], grays[0], grays[0], grays[0], grays[0]
	};
	 // 根据上面等级 建立待选灰度数组
	uchar gray[26];
	for (int i = 0; i < 26; i++)
		gray[i] = (uchar)(255 * grayLevel[i] / 100);
	// 一些常量
	int nx = parray->nx;
	int ny = parray->ny;
	int maxIndex = ny * nx - 1;
	if (parray)
	{
		// 将 parray->pixels 复制出来，作为旧图，必须要复制，否则计算过的点的值会影响后续点
		uchar* oldPixels = (uchar*)malloc( (nx*ny) * sizeof(uchar) );
		memcpy(oldPixels, parray->pixels, (nx*ny)); // 内存复制
		for (int j = 0; j < ny; j++) 
		{
			for (int i = 0; i < nx; i++)
			{
				int count = 0;  // 设置初始的个数为0
				for (int w = -2; w <= 2; w++)  // 双重循环，遍历窗口覆盖元素
					for (int h = -2; h <= 2; h++)
					{
						if ( (i + w) < 0 || (i + w) >= nx || (j + h) < 0 || (j + h) >= ny) // 超出索引范围，跳出
							continue;
						if (oldPixels[(j + h)*nx + i + w] == 255) // 在旧图中判断像素值
							count++;
					}
				parray->pixels[j * nx + i] = gray[count];
			}
		}
		free(oldPixels); // 释放开辟的空间
	}
	return 1;
}
////////////////////////////////////////////////////////////////