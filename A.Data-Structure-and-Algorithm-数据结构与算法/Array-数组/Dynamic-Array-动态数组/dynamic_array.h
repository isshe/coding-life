/**
 * 日期：20181018
 * 作者：isshe
 * 
 * 备注：此实现参考nginx里的ngx_array_t的实现，并添加了一些接口
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

#define DYNAMIC_ARRAY_ERROR 1
#define DYNAMIC_ARRAY_OK    0

typedef struct {
    void            *elts;
    size_t          elt_size;
    unsigned int    nelts;
    unsigned int    nalloc;
} dynamic_array_t;

/**
 * 功能：对一个dynamic_array_t *指向的dynamic_array_t进行初始化
 * 参数：
 *  array：指向要进行初始化的动态数组
 *  n：数量
 *  size：每个元素的大小
 */
int dynamic_array_init(dynamic_array_t *array, unsigned int n, size_t size);

/**
 * 功能：对一个dynamic_array_t *指向的dynamic_array_t进行逆初始化
 * 参数：
 *  array：指向要进行操作的动态数组
 */
void dynamic_array_uninit(dynamic_array_t *array);

/**
 * 功能：创建一个动态数组，并返回此动态数组的指针
 * 参数：
 *  n：数量
 *  size：每个元素的大小
 */
dynamic_array_t *dynamic_array_create(unsigned int n, size_t size);

/**
 * 功能：销毁一个动态数组
 * 参数：
 *  array：指向要进行操作的动态数组
 */
void dynamic_array_destroy(dynamic_array_t *array);

/**
 * 功能：准备添加一个元素（具体用法见test_dynamic_array.c文件）
 * 参数：
 *  array：指向要进行操作的动态数组
 */
void *dynamic_array_push(dynamic_array_t *array);

/**
 * 功能：准备添加n个元素（具体用法见test_dynamic_array.c文件）
 * 参数：
 *  array：指向要进行操作的动态数组
 *  n：个数
 */
void *dynamic_array_push_n(dynamic_array_t *array, unsigned int n);

/**
 * 功能：获取动态数组首元素的地址（供遍历等操作）
 * 参数：
 *  array：指向要进行操作的动态数组
 */
void *dynamic_array_elts(dynamic_array_t *array);

/**
 * 功能：获取动态数组存在的元素个数
 * 参数：
 *  array：指向要进行操作的动态数组
 */
unsigned int dynamic_array_nelts(dynamic_array_t *array);

/**
 * 功能：获取动态数组的总大小
 * 参数：
 *  array：指向要进行操作的动态数组
 */
unsigned int dynamic_array_size(dynamic_array_t *array);

/**
 * 功能：弹出1个元素
 * 参数：
 *  array：指向要进行操作的动态数组
 */
void *dynamic_array_pop(dynamic_array_t *array);

/**
 * 功能：弹出n个元素
 * 参数：
 *  array：指向要进行操作的动态数组
 *  n：个数
 */
void *dynamic_array_pop_n(dynamic_array_t *array, unsigned int n);

#endif