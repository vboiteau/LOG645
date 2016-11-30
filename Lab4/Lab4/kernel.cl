__kernel void dataParallel(__global float* UPar,
	unsigned int k,
	const unsigned int m,
    const unsigned int n, 
    const float mod)
{
    int id = get_global_id(0);
    int i = id % m;
    int j = (int)((id-i) / m);
    int size = get_global_size(0);
    int lastMod = (k%2?0:m*n);
    float last = UPar[lastMod+id];
    float left = (i!=0?UPar[lastMod + (id-1)]:0);
    float top = ( j != 0 ? UPar[lastMod + (id-m)]:0);
    float right = (i!=m-1?UPar[lastMod + (id+1)]:0);
    float bottom =(j!=n-1?UPar[lastMod + (id+m)]:0);
    float sides = top + right + bottom + left;
    int current = m*n - lastMod + id;
    UPar[current]=(1-(4*mod))*last+mod*(sides);
}
