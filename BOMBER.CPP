#include <stdio.h>
#include <graphics.h>
#include <stdio.h>
#include <alloc.h>

#include "c:\\bc\\source\\bomber\\hero.cpp"   //герой
#include "c:\\bc\\source\\bomber\\enemy.cpp"  //враг
#include "c:\\bc\\source\\bomber\\bomba.cpp"  //бомба
#include "c:\\bc\\source\\bomber\\block1.cpp" //неразбиваемый блок
#include "c:\\bc\\source\\bomber\\block2.cpp" //разбваемый блок

#define sized 40      //стандартный размер всего(блок,герой,враг)
#define step 40       //шаг


void graphON()
{
int gd=DETECT,gm;
initgraph(&gd,&gm,"c:\\bc\\bgi");
}

void noworkfield()
{
 setcolor(15);
 line(79,0,79,479);
 line(0,0,79,0);
 line(0,479,79,479);
 line(0,0,0,479);

 setfillstyle(1,7);
 floodfill(15,20,15);

 setcolor(8);
 line(78,1,78,478);
 line(1,1,78,1);
 line(1,478,78,478);
 line(1,1,1,478);

}

class block
{

};

void main()
{
void *blocktx2,*blocktx1,*herotx,*bombtx,*enemytx;
graphON();

blocktx2=block2();
//putimage(100,100,blocktx2,XOR_PUT);
putimage(140,100,blocktx2,XOR_PUT);


blocktx1=block1();
putimage(220,100,blocktx1,XOR_PUT);

herotx=hero();
putimage(300,100,herotx,XOR_PUT);


enemytx=enemy(14);
putimage(380,100,enemytx,XOR_PUT);


bombtx=bomba();
putimage(460,100,bombtx,XOR_PUT);

//noworkfield();

int i,j;
for(i=0,j=0;j<479;i+=40)
 {
 // putimage(i,j,blocktx1,XOR_PUT);
  if(i>=639)
  {
   i=-40;
   j+=40;
  }
 }

getchar();
closegraph();

free(blocktx1);
free(blocktx2);
free(herotx);
free(enemytx);
free(bombtx);
}

