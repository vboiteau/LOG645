#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main()
{
	//Variables
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


	FILE *fp;
	char fileName[] = "./kernel.cl";
	char *source_str;
	size_t source_size;

	/* Load the source code containing the kernel*/
#ifdef __APPLE__
	fp = fopen(fileName, "r");
#else
	fopen_s(&fp, fileName, "r");
#endif
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	//Required methods for OpenCL
	//Creation de la plateforme et du deviceid
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);


	//Création du contexte
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	//Creation de la file de commandes
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

//------------------------Creation des buffers à utiliser---------------------------------
	int i, j;
	float *A;
	float *B;
	
	A = (float *)malloc(4 * 4 * sizeof(float));
	B = (float *)malloc(4 * 4 * sizeof(float));
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			A[i * 4 + j] = i * 4 + j + 1;
		}
	}
	cl_mem Amobj;
	cl_mem Bmobj = NULL;
	Amobj = clCreateBuffer(context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, 4 * 4 * sizeof(float), A, &ret);
	Bmobj = clCreateBuffer(context, CL_MEM_WRITE_ONLY|
		CL_MEM_COPY_HOST_PTR, 4 * 4 * sizeof(float), B, &ret);
	
//------------------------------------------------------------------------------------
//------------------------Creation du programe----------------------------------------
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret);
	
	//Build du programme
	ret = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	
//------------------------------------------------------------------------------------
//------------------------Create kernel----------------------------------------------
	int multi = 3;
	kernel = clCreateKernel(program, "dataParallel", &ret);

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &Amobj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &Bmobj);
	ret = clSetKernelArg(kernel, 2, sizeof(int), &multi);
	
//-----------------------Execution de commande-----------------------------------------
	size_t global_item_size = 16;
	size_t local_item_size = 1;
	/* Execute OpenCL Kernel */
	

	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	clFinish(command_queue);
	
//-----------------------Lecture de reponses-------------------------------------------
	ret = clEnqueueReadBuffer(command_queue, Bmobj, CL_TRUE, 0, 4 * 4 * sizeof(float), B, 0, NULL, NULL);
	/* Display Results */
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%7.2f ", B[i * 4 + j]);
		}
		printf("\n");
	}


	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseMemObject(Amobj);
	ret = clReleaseMemObject(Bmobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);
	free(A);
	free(B);
	return 0;
}
