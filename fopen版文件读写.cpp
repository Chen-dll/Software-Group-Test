#include <iostream>
#include <cstdio>
using namespace std;
int main()
{
    FILE *fin, *fout;
    fin = fopen("in.txt","rb");
    fout = fopen("out.txt","wb");
    int temp, sum = 0;
    while(fscanf(fin, "%d",&temp) == 1)
    {
        sum += temp;
    }
    fprintf(fout, "%d\n",sum);
    fclose(fin);fclose(fout);
    system("pause");
    return 0;
}