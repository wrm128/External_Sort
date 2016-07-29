# External_Sort
# an external sort program for massive floating point data

本程序是一个简易的外部排序系统。界面部分主要使用WPF技术编程实现，内核部分使用了C和C++。可实现大数据文件的外部排序，主要分为子文件的划分，内部基数排序，多路归并等几个过程。最终在性能较好的机器上，2.5亿数据可跑进150s左右。
