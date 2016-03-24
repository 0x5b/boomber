#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <alloc.h>
#include <dos.h>
#include <math.h>

#include "c:\\bc\\source\\bomber\\hero.cpp"   //герой
#include "c:\\bc\\source\\bomber\\enemy.cpp"  //враг
#include "c:\\bc\\source\\bomber\\bomba.cpp"  //бомба
#include "c:\\bc\\source\\bomber\\block1.cpp" //неразбиваемый блок
#include "c:\\bc\\source\\bomber\\block2.cpp" //разбваемый блок
#include "c:\\bc\\source\\bomber\\bang.cpp"   //взрыв

#define sized 40      //стандартный размер всего(блок,герой,враг)
#define step 40       //шаг
#define maxenemy 3

void *blockTexture;
void *destrBlockTexture;
void *playerTexture;
void *enemyTexture;
void *bombTexture;
void *bangTexture;
unsigned int size;

class Game;
class Map;
class Tile;
class Ablock;
class DestrBlock;
class Block;
class Bomb;
class Aliving;
class Player;
class Enemy;
class empty;

//################################

class Tile
{
 protected:
  int x,y,enter;
  void *texture;

 public:
  Tile()
  {

  }

  virtual ~Tile()
  {
   free(texture);
  }

  void ent_x(int i)  { x=i; }
  void ent_y(int i)  { y=i; }

  int get_x() const  { return x; }
  int get_y() const  { return y; }


  virtual void draw()=0;

  virtual void remove()=0;

  virtual int check_enter()=0;

  virtual int bang()=0;
};

//#########################################

class Ablock : public Tile
{
 public:

  int check_enter() { return 0; }

};

//#########################################

class DestrBlock : public Ablock
{
 public:
  DestrBlock() { texture=destrBlockTexture; }

  void draw()  { putimage(x,y,texture,XOR_PUT); }

  void remove(){ putimage(x,y,texture,XOR_PUT); }

  int bang()
  {
   remove();
   putimage(x,y,bangTexture,XOR_PUT);
   delay(75);
   putimage(x,y,bangTexture,XOR_PUT);
   return 1;
  }

  ~DestrBlock(){ free(texture); }
};

//#####################################################

class Block : public Ablock
{

 public:
  Block()      { texture=blockTexture; }

  void draw()  { putimage(x,y,texture,XOR_PUT); }

  void remove(){ putimage(x,y,texture,XOR_PUT); }

  int bang()   { return 0; }

  ~Block() { free(texture); }
};

//#####################################################

class Bomb : public Tile
{
 private:
  int timer,i,j;

 public:
  Bomb() { texture=bombTexture; }

  int check_enter() { return 0; }

  void draw()   { putimage(x,y,texture,XOR_PUT); }

  void remove() { putimage(x,y,texture,XOR_PUT); }

  void timedawn() { timer-=25; }

  int get_time() const { return timer; }

  int start_time() { timer=1000; }

  int get_i() const  { return i; }
  int get_j() const  { return j; }

  void ent_i(int x) { i=x; }
  void ent_j(int y) { j=y; }

  int bang()
  {
   remove();
   putimage(x,y,bangTexture,XOR_PUT);
   delay(75);
   putimage(x,y,bangTexture,XOR_PUT);
   return 1;
  }

  ~Bomb() { free(texture); }
};

//######################################################

class Aliving : public Tile
{
 protected:
  int i,j,live;

 public:

  int get_i() const  { return i; }
  int get_j() const  { return j; }

  void ent_i(int x) { i=x; }
  void ent_j(int y) { j=y; }

  void kill() { live = 0; remove(); }

};

//#######################################################

class Player : public Aliving
{
 public:
  Player() { live=1; texture=playerTexture; }

  void draw() { putimage(x,y,texture,XOR_PUT); }

  void remove() { putimage(x,y,texture,XOR_PUT); }

  int check_enter()
  {
   kill();
   putimage(x,y,bangTexture,XOR_PUT);
   delay(75);
   putimage(x,y,bangTexture,XOR_PUT);
   printf("DEFEAT...");
   getchar();
   exit(1);
  }

  int bang(){ check_enter(); }

  ~Player() { free(texture); }
};

//###################################################

class Enemy : public Aliving
{
 private:
  int timer,v;

 public:
  Enemy() { v=1; live=1; texture=enemyTexture; }

  void start_time() { timer = 250; }

  int get_time()    { return timer;}

  void time_dawn()  { timer-=50;   }

  int get_status()  { return live; }

  void draw()       { putimage(x,y,texture,XOR_PUT); }

  int check_enter() { return 0; }

  int get_v()      { return v; }

  void change_vx()
  {
   if(v==1) v=-1;
   else v=1;
  }

  void remove() { putimage(x,y,texture,XOR_PUT); }

  int bang()
  {
   kill();
   putimage(x,y,bangTexture,XOR_PUT);
   delay(75);
   putimage(x,y,bangTexture,XOR_PUT);
   return 1;
  }

  ~Enemy() { free(texture); }
};

//######################################################

class Empty : public Tile
{
 public:
  check_enter() { return 1; }

  void draw()
  {
   setcolor(0);
   rectangle(x,y,x+sized,y+sized);
   setfillstyle(1,0);
   floodfill(x+10,y+10,0);

  }
  int bang()
  {
   putimage(x,y,bangTexture,XOR_PUT);
   delay(75);
   putimage(x,y,bangTexture,XOR_PUT);
   return 0;
  }

  void remove() { }
};

//#######################################################

class Map
 {
  private:
   FILE *f;
   char ch;


   public:

   Map()
   {

   }

   ~Map()
   {
    int i,j;
    for(i=0;i<11;i++)
     for(j=0;j<15;j++)
      free(tiles[i][j]);

    free(f);
   }

   Player *p;

   Enemy *e[maxenemy];

   Tile *tiles[11][15];

   void CreateMap();

   void Draw()
   {
    int i,j;
    for(i=0;i<11;i++)
     for(j=0;j<15;j++)
     {
      tiles[i][j]->draw();
     }
   }
 };

void Map::CreateMap()
{
    int x=0,y=39;
    int i=0,j=0;
    int n=0;
    if((f=fopen("c:\\bc\\source\\bomber\\map.txt","r"))==NULL)
     printf("file not found");

    else
    {
     while(!feof(f))
     {
      ch=fgetc(f);
      if(ch=='x')
      {

       tiles[i][j] = new Block;
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
       continue;
      }

      if(ch=='o')
      {
       tiles[i][j] = new Empty;
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
       continue;
      }

      if(ch=='b')
      {
       tiles[i][j] = new DestrBlock;
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
       continue;
      }

      if(ch=='p')
      {
       p = new Player;
       p->ent_x(x);
       p->ent_y(y);
       p->ent_i(i);
       p->ent_j(j);
       tiles[i][j]=p;
       x+=sized;
       j++;
       continue;
      }

      if(ch=='e')
      {
       e[n] = new Enemy;
       e[n]->ent_x(x);
       e[n]->ent_y(y);
       e[n]->ent_i(i);
       e[n]->ent_j(j);
       tiles[i][j]=e[n];
       x+=sized;
       j++;
       n++;
       continue;
      }

      if(ch=='\n')
      {
       x=0;
       y+=sized;
       j=0;
       i++;
       continue;
      }
     }
    }
}

//#############################################

class Game
{
 private:
  Map m;

 public:

  Game()
  {
   m.CreateMap();
   m.Draw();
  }
  ~Game() {}

  void upgrade();

 };

void Game::upgrade()
{
 int i,j,buf,k,n1;
 int f=0;

 Bomb*b = new Bomb;

 int key=0;

 Tile*t;

 for(k=0;k<maxenemy;k++)
 {
  m.e[k]->start_time();
 }

 do
 {
  if(kbhit())                            //управление героем
  {
   key=getche();
   switch(key)
   {
    case 115:
     {
      i=m.p->get_i();
      j=m.p->get_j();

      if(m.tiles[i+1][j]->check_enter())
      {
       if(f && m.p->get_i()==b->get_i()
	  && m.p->get_j()==b->get_j())  //если бомба существует,то стераем героя
       {                             //записываем в него новые координаты
	m.tiles[i][j]->remove();     //указатель новой позиции направляем на героя
				     //указатель старой позиции направляем на адрес бомбы
	m.tiles[i][j]->ent_y(m.tiles[i+1][j]->get_y());

	m.tiles[i+1][j]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i++][j]->draw();

	m.p->ent_i(i);

	m.p->draw();

	continue;
       }
       else
       {
       m.tiles[i][j]->remove();      //если бомбы нет то просто меняем
				     //на что указывают указатели
       buf=m.tiles[i][j]->get_y();
       m.tiles[i][j]->ent_y(m.tiles[i+1][j]->get_y());
       m.tiles[i+1][j]->ent_y(buf);

       t=m.tiles[i+1][j];
       m.tiles[i+1][j]=m.tiles[i][j];
       m.tiles[i++][j]=t;

       m.p->ent_i(i);

       m.p->draw();
       }
       break;
      }
      break;
     }
    case 119:
    {
     i=m.p->get_i();
     j=m.p->get_j();
     if(m.tiles[i-1][j]->check_enter())
     {
      if(f && m.p->get_i()==b->get_i()
	 && m.p->get_j()==b->get_j())
       {
	m.tiles[i][j]->remove();

	m.tiles[i][j]->ent_y(m.tiles[i-1][j]->get_y());

	m.tiles[i-1][j]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i--][j]->draw();

	m.p->ent_i(i);

	m.p->draw();

	continue;
       }

      m.tiles[i][j]->remove();

      buf=m.tiles[i][j]->get_y();
      m.tiles[i][j]->ent_y(m.tiles[i-1][j]->get_y());
      m.tiles[i-1][j]->ent_y(buf);

      t=m.tiles[i-1][j];
      m.tiles[i-1][j]=m.tiles[i][j];
      m.tiles[i--][j]=t;
      m.p->ent_i(i);

      m.p->draw();
      break;

     }
     break;
    }

    case 100:
    {
     i=m.p->get_i();
     j=m.p->get_j();
     if(m.tiles[i][j+1]->check_enter())
     {
      if(f && m.p->get_i()==b->get_i()
	 && m.p->get_j()==b->get_j())
       {
	m.tiles[i][j]->remove();

	m.tiles[i][j]->ent_x(m.tiles[i][j+1]->get_x());

	m.tiles[i][j+1]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i][j++]->draw();

	m.p->ent_j(j);

	m.p->draw();

	continue;
       }
      m.tiles[i][j]->remove();

      buf=m.tiles[i][j]->get_x();
      m.tiles[i][j]->ent_x(m.tiles[i][j+1]->get_x());
      m.tiles[i][j+1]->ent_x(buf);

      t=m.tiles[i][j+1];
      m.tiles[i][j+1]=m.tiles[i][j];
      m.tiles[i][j++]=t;

      m.p->ent_j(j);

      m.p->draw();
      break;
     }
     break;
    }
    case 97 :
    {
     i=m.p->get_i();
     j=m.p->get_j();
     if(m.tiles[i][j-1]->check_enter())
     {
      if(f && m.p->get_i()==b->get_i()
	 && m.p->get_j()==b->get_j())
       {
	m.tiles[i][j]->remove();

	m.tiles[i][j]->ent_x(m.tiles[i][j-1]->get_x());

	m.tiles[i][j-1]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i][j--]->draw();

	m.p->ent_j(j);

	m.p->draw();

	continue;
       }
      m.tiles[i][j]->remove();

      buf=m.tiles[i][j]->get_x();
      m.tiles[i][j]->ent_x(m.tiles[i][j-1]->get_x());
      m.tiles[i][j-1]->ent_x(buf);

      t=m.tiles[i][j-1];
      m.tiles[i][j-1]=m.tiles[i][j];
      m.tiles[i][j--]=t;

      m.p->ent_j(j);

      m.p->draw();
      break;
     }
     break;
    }
    case 32:         //создание бомбы,она просто создается с теми же
    {                //параметрами что и герой
     if(!f)          //если бомба еще не стоит
     {               //можно ставить только 1 бомбу
      b->start_time();
      b->ent_x(m.p->get_x());
      b->ent_y(m.p->get_y());
      b->ent_i(m.p->get_i());
      b->ent_j(m.p->get_j());
      f=1;
      break;
     }
     else break;
    }
   }
   continue;
  }
  //------AI-------------------

 for(k=0;k<maxenemy;k++)
 {
  if(m.e[k]->get_status())
  {
   n1=m.e[k]->get_v();
   if(m.e[k]->get_time()==0)
   {
    i=m.e[k]->get_i();
    j=m.e[k]->get_j();
    if(m.tiles[i][j+n1]->check_enter())
     {
      m.tiles[i][j]->remove();

      buf=m.tiles[i][j]->get_x();
      m.tiles[i][j]->ent_x(m.tiles[i][j+n1]->get_x());
      m.tiles[i][j+n1]->ent_x(buf);

      t=m.tiles[i][j+n1];
      m.tiles[i][j+n1]=m.tiles[i][j];
      m.tiles[i][j]=t;

      j+=n1;
      m.e[k]->ent_j(j);

      m.tiles[i][j]->draw();
     }
    else
    {
     if(m.tiles[i-n1][j]->check_enter())
     {
      m.tiles[i][j]->remove();

      buf=m.tiles[i][j]->get_y();
      m.tiles[i][j]->ent_y(m.tiles[i-n1][j]->get_y());
      m.tiles[i-n1][j]->ent_y(buf);

      t=m.tiles[i-n1][j];
      m.tiles[i-n1][j]=m.tiles[i][j];
      m.tiles[i][j]=t;

      i-=n1;
      m.e[k]->ent_i(i);

      m.tiles[i][j]->draw();
     }
    else
    {
     m.e[k]->change_vx();
    }
   }
    m.e[k]->start_time();
  }
 }
  m.e[k]->time_dawn();
}

//--------------------------------------------------------

 if(f)               //если бомба существует
 {
  if(b->get_time()==0)//дошел ли таймер до 0?
  {
   int i,j;
   i=b->get_i();
   j=b->get_j();
   if(m.tiles[i][j]->bang())
   {
    //free(m.tiles[i][j]);
    m.tiles[i][j] = new Empty;       //создаем новую пустую клетку
    m.tiles[i][j]->ent_x(b->get_x());//на нее указывает то что указывало на бомбу
    m.tiles[i][j]->ent_y(b->get_y());//рисовать не нужно
   }
   if(m.tiles[i+1][j]->bang())
   {
    //free(m.tiles[i+1][j]);
    m.tiles[i+1][j] = new Empty;
    m.tiles[i+1][j]->ent_x(sized*j);
    m.tiles[i+1][j]->ent_y(39+sized*(i+1));
   }
   if(m.tiles[i][j-1]->bang())
   {
    //free(m.tiles[i][j-1]);
    m.tiles[i][j-1] = new Empty;
    m.tiles[i][j-1]->ent_x(sized*(j-1));
    m.tiles[i][j-1]->ent_y(39+sized*i);
   }
   if(m.tiles[i-1][j]->bang())
   {
    //free(m.tiles[i-1][j]);
    m.tiles[i-1][j] = new Empty;
    m.tiles[i-1][j]->ent_x(sized*j);
    m.tiles[i-1][j]->ent_y(39+sized*(i-1));
   }
   if(m.tiles[i][j+1]->bang())
   {
    //free(m.tiles[i][j+1]);
    m.tiles[i][j+1] = new Empty;
    m.tiles[i][j+1]->ent_x(sized*(j+1));
    m.tiles[i][j+1]->ent_y(39+sized*i);
   }

   f=0;
  }
  b->timedawn();
 }
 for(k=0;k<maxenemy;k++)
 {
  if(!m.e[k]->get_status()) continue;
  else break;
 }
 if(k==maxenemy)
 {
  printf("VICTORY");
  getchar();
  exit(0);
 }
  delay(50);

 }while(key!=27);

 free(b);
 free(t);
}

//#################################################

void main()
{
 randomize();
 int gd=DETECT,gm;
 initgraph(&gd,&gm,"c:\\bc\\bgi");

 size=imagesize(0,0,39,39);

 blockTexture     =malloc(size);
 destrBlockTexture=malloc(size);
 playerTexture    =malloc(size);
 enemyTexture     =malloc(size);
 bombTexture      =malloc(size);
 bangTexture      =malloc(size);

 blockTexture	  =block1();
 destrBlockTexture=block2();
 playerTexture	  =hero();
 enemyTexture     =enemy(random(16));
 bombTexture      =bomba();
 bangTexture      =bang();

 Game g;
 g.upgrade();
 closegraph();

 free(blockTexture);
 free(destrBlockTexture);
 free(playerTexture);
 free(enemyTexture);
 free(bombTexture);
 free(bangTexture);

}