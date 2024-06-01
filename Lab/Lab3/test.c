#include <unistd.h>
int main()
{
    for(char i=48;i<58;i++)
    {
        write(1,&i,sizeof(char));

    }
    return 0;
    

}