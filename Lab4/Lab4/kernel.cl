__kernel void dataParallel(__global float* UPar,
	unsigned int k,
	const unsigned int m,
    const unsigned int n, 
    const float mod)
{
    int id = get_global_id(0);
    int size = get_global_size(0);
    int lastMod = (m*n*k>2*m*n?m*n:0);
    int left = (id%m!=0?lastMod + (id-1):0);
    int top=id-m;
    if (top <= 0 ) {
        top = 0;
    }
    int right = (id%m!=m-1?lastMod + (id+1):0);
    int bottom =(id+m<size?lastMod + (id+m):0);
    float sides = (left?*(UPar+left):0)+(top?*(UPar+top):0)+(right?*(UPar+right):0)+(bottom?*(UPar+bottom):0);
    printf("%.2f\t%d\n",sides, id);
    float last = UPar[lastMod+id];
    int current = m*n - lastMod + id;
    UPar[current]=(1-(4*mod))*last+mod*(sides);
}
