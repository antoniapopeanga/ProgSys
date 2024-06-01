#include <stdio.h>
#include <setjmp.h>

jmp_buf jump_buffer;

int f(){
return setjmp(jump_buffer);
}
void g()
{
longjmp(jump_buffer,1);
}

int main() {
    volatile int i=0;
   int r=f();
   if(i<10)
   {
    printf("%d\n",i);
    i++;
    g();
   }
    return 0;
}
//si on mets les appels setjmp et longjmp dans des fonctions, le program ne marche plus