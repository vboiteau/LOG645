__kernel void dataParallel(__global float* input,
	__global float* output,
	const unsigned int multi)
{
	int id = get_global_id(0);
		output[id] = input[id] * multi;

	

}