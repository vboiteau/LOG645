#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "printer.hpp"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

unsigned int TEMPS_ATTENTES=5;

/* #####   FUNCTION DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ##################### */
float generateFirstInstanceCell (int m, int n, int i, int j);
float sumImmediateNeighbours (float top, float right, float bottom, float left);
float processTimeEffectOnCell (float mod, float old, float top, float right, float bottom, float left);
float getMod (float td, float h);

int main(int args, char *argv[])
{
    /*-----------------------------------------------------------------------------
     * args instanciation 
     *-----------------------------------------------------------------------------*/
    if (args != 6) {
        printf("Il faut absolument avoir 6 arguments (m, n, np, td, h)\n");
        return 1;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int np = atoi(argv[3]);
    float td = atof(argv[4]);
    float h = atof(argv[5]);
    float mod = getMod(td, h);
    float USeq[2][n][m];
    float *UPar = (float *)malloc(2*m*n*sizeof(float));
    int debug = 1;
    int sequentielle = 1;
    int size=1;
    struct timeval tseq, tpar;
    double timeStart, timeEnd, TSeqExec, TParExec;
    int i, j, k;
    /*-----------------------------------------------------------------------------
     *  Sequential code
     *-----------------------------------------------------------------------------*/
    if(sequentielle) {
        printf("Arguments Values\nm\tn\tnp\ttd\th\n%d\t%d\t%d\t%.2f\t%.2f\n\n",m,n,np,td,h);
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                USeq[0][j][i] = generateFirstInstanceCell(m,n,i,j); 
            }
        }
        if (debug) {
            printResult(m, n, (float *)USeq[0],0);
        }
        gettimeofday (&tseq, NULL); // Debut du chronometre
        timeStart = (double) (tseq.tv_sec) + (double) (tseq.tv_usec) / 1e6;
        for (k=1;k<=np;k++){
            for (j = 0; j < n; ++j) {
                for (i = 0; i < m; i++) {
                    USeq[k%2][j][i] = processTimeEffectOnCell(
                            mod,
                            USeq[(k+1)%2][j][i],
                            (j>0?USeq[(k+1)%2][j-1][i]:0),
                            (i<m-1?USeq[(k+1)%2][j][i+1]:0),
                            (j<n-1?USeq[(k+1)%2][j+1][i]:0),
                            (i>0?USeq[(k+1)%2][j][i-1]:0)
                            );
                }
            }
        }
        gettimeofday (&tseq, NULL); // Fin du chronometre
        timeEnd = (double) (tseq.tv_sec) + (double) (tseq.tv_usec) / 1e6;
        TSeqExec = timeEnd - timeStart; //Temps d'execution en secondes
        if (debug) {
            printResult(m, n, (float *)USeq[np%2], 0);
        }
    }
    /*-----------------------------------------------------------------------------
     *  Parallel code
     *-----------------------------------------------------------------------------*/
    for (j = 0; j < n; ++j) {
        for (i = 0; i < m; ++i) {
            UPar[j*m+i] = generateFirstInstanceCell(m,n,i,j); 
        }
    }
    if (debug) {
        printResult(m, n,(float *) UPar, 0);
    }
	//Variables
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
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


	//Cr�ation du contexte
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	//Creation de la file de commandes
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

//------------------------Creation des buffers � utiliser---------------------------------
	cl_mem Uparmobj;
	
//------------------------------------------------------------------------------------
//------------------------Creation du programe----------------------------------------
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret);
	
	//Build du programme
	ret = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	gettimeofday(&tpar, NULL);
	timeStart = (double) (tpar.tv_sec) + (double) (tpar.tv_usec)/1e6;
    for (k = 1; k <= np; k++) {
        Uparmobj = clCreateBuffer(context, CL_MEM_READ_ONLY |
                CL_MEM_COPY_HOST_PTR, 2 * m * n * sizeof(float), UPar, &ret);
        //------------------------------------------------------------------------------------
        //------------------------Create kernel----------------------------------------------
        kernel = clCreateKernel(program, "dataParallel", &ret);
        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &Uparmobj);
        ret = clSetKernelArg(kernel, 1, sizeof(int), &k);
        ret = clSetKernelArg(kernel, 2, sizeof(int), &m);
        ret = clSetKernelArg(kernel, 3, sizeof(int), &n);
        ret = clSetKernelArg(kernel, 4, sizeof(float), &mod);

        //-----------------------Execution de commande-----------------------------------------
        size_t global_item_size = m*n;
        size_t local_item_size = 1;
        /* Execute OpenCL Kernel */


        ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                &global_item_size, &local_item_size, 0, NULL, NULL);
        clFinish(command_queue);

        //-----------------------Lecture de reponses-------------------------------------------
        ret = clEnqueueReadBuffer(command_queue, Uparmobj, CL_TRUE, 0, 2 * m * n * sizeof(float), UPar, 0, NULL, NULL);
    }    
	gettimeofday (&tpar, NULL); // Fin du chronometre
	timeEnd = (double) (tpar.tv_sec) + (double) (tpar.tv_usec) / 1e6;
	TParExec = timeEnd - timeStart; //Temps d'execution en secondes
	/* Display Results */
    gettimeofday (&tpar, NULL); // Fin du chronometre
    timeEnd = (double) (tpar.tv_sec) + (double) (tpar.tv_usec) / 1e6;
    if (debug) {
        printResult(m, n, UPar, (np%2)*n*m);
    }

	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(Uparmobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);
	free(UPar);
    /*-----------------------------------------------------------------------------
     *  Print results
     *-----------------------------------------------------------------------------*/
    float S = TSeqExec/TParExec;
    float E = S/size;
    printf("T1\t%.6lf sec\nTP\t%.6lf sec\nS\t%.2f\nE\t%.2f %%\n\n", (double)TSeqExec, (double)TParExec, S,E*100);
    FILE *json = fopen("stats.json","w");
    if(json == NULL) {
        printf("stats file not created!");
    } else {
        fprintf(json, "{\"input\":{\"nbproc\":%d,\"m\":%d,\"n\":%d,\"np\":%d,\"td\":%.4f,\"h\":%.4f},", size, m, n, np, td, h);
        fprintf(json, "\"output\":{\"T1\":%.2lf,\"TP\":%.2lf,\"S\":%.2f,\"E\":%.2f}}", TSeqExec*1000, TParExec*1000, S, E);
        fclose(json);
    }
	return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generateFirstInstanceCell
 *  Description:  Will generate first instance of matrix cell since time 0 can't be
 *	found by dependencies.
 * =====================================================================================
 */
float generateFirstInstanceCell (int m, int n, int i, int j) { 
    float value = (float)(i * (m - i - 1) * j * (n - j -1));
    return value;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sumImmediateNeighbours
 *  Description:  Will sum neighbours of cell {top, right, bottom, left}, from last time
 *	instance.
 * =====================================================================================
 */
float sumImmediateNeighbours(float top, float right, float bottom, float left) {
    return top + right + bottom + left;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  processTimeEffectOnCell
 *  Description:  Calculation relative to the time effect on heat propagation
 * =====================================================================================
 */
float processTimeEffectOnCell (float mod, float old, float top, float right, float bottom, float left) {
    usleep(TEMPS_ATTENTES);
    return (1-(4*mod))*old+mod*(sumImmediateNeighbours(top, right, bottom, left));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getMod
 *  Description:  Return the mod, obtain by operation on decritisized time and cell side
 * =====================================================================================
 */
float getMod (float td, float h) {
    return (td/(h*h));
}
