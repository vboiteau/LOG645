/*
 * =====================================================================================
 *
 *       Filename:  Lab4.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/23/2016 15:23:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef __APPLE__
#include <OpenCl/opencl.h>
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.h>
#endif
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

std::vector<cl::Platform> all_platforms;
std::vector<cl::Device> all_devices;
cl::Platform platform;
cl::Device device;
std::string kernel_code;

int main(){
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_mem memobj = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;

    char string [MEM_SIZE];
    FILE *fp;
    char fileName[] = "./Lab4.cl";
    char *source_str;
    size_t source_size;
    #ifdef __APPLE__
    fp = fopen(fileName, "r");
    #else
    fopen_S(&fp, fileName, "r");
    #endif
    if(!fp){
        fprintf(stderr, "kernel.\n");
    }

    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE,fp);
    fclose(fp);

    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, &ret);
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "hello", &ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
    ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(char), string, 0, NULL, NULL);

    puts(string);

    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(source_str);
    return 0;
}
