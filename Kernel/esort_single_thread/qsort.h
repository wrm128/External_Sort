#ifndef QSORT_H
#define QSORT_H


/* 返回枢轴位置 */
int Partition(double *a,int lt,int rt);

/* 快排 */
void QuickSort(double *a,int lt,int rt);

/* 返回枢轴位置 */
int Partition1(double *a,int lt,int rt);

/* 快排 */
void QuickSort1(double *a,int lt,int rt);

#endif