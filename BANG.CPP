void* bang()
{
void *ptr;
unsigned int size;

setcolor(YELLOW);
rectangle(0,0,39,39);

setfillstyle(1,YELLOW);
floodfill(15,20,YELLOW);

/*setcolor(RED);
rectangle(9,9,30,30);

setfillstyle(1,RED);
floodfill(15,20,RED);
*/

size=imagesize(0,0,39,39);
ptr=malloc(size);
getimage(0,0,39,39,ptr);

putimage(0,0,ptr,XOR_PUT);

return ptr;
}