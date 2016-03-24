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

#define sized 40      //стандартный размер всего(блок,герой,враг)
#define step 40       //шаг

void *blockTexture;
void *destrBlockTexture;
void *playerTexture;
void *enemyTexture;
void *bombTexture;
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
   delete texture;
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

  int check_enter()
  {
   return 0;
  }

};

class DestrBlock : public Ablock
{
 public:
  DestrBlock()
  {
   texture=destrBlockTexture;
  }

  void draw()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  void remove()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  int bang()
  {
   return 1;
  }

  ~DestrBlock() { delete texture; }
};


class Block : public Ablock
{

 public:
  Block()
  {
   texture=blockTexture;
  }

  void draw()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  void remove()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  int bang()
  {
   return 0;
  }

  ~Block() { delete texture; }
};

class Bomb : public Tile
{
 private:
  int timer,i,j;
 public:
  Bomb()
  {
   timer=3
   00;
   texture=bombTexture;
  }

  int check_enter()
  {
   return 0;
  }

  void draw()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  void remove()
  {
   putimage(x,y,texture,XOR_PUT);
  }
  void timedawn() { timer-=25; }

  int get_time() { return timer; }

  int get_i() const  { return i; }
  int get_j() const  { return j; }

  void ent_i(int x) { i=x; }
  void ent_j(int y) { j=y; }

  int bang()
  {}

  ~Bomb() { delete texture; }
};

class Aliving : public Tile
{
 private:
  int i,j;

 public:
  int check_enter(){ return 0; }

  int get_i() const  { return i; }
  int get_j() const  { return j; }

  void ent_i(int x) { i=x; }
  void ent_j(int y) { j=y; }

};

class Player : public Aliving
{
 public:
  Player()
  {
   texture=playerTexture;
  }

  void draw()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  void remove()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  int bang()
  {
   return 3;
  }

  ~Player() { delete texture; }
};

class Enemy : public Aliving
{
 public:
  Enemy()
  {
   texture=enemyTexture;
  }

  void draw()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  void remove()
  {
   putimage(x,y,texture,XOR_PUT);
  }

  int bang()
  {
   return 3;
  }


  ~Enemy() { delete texture; }
};

class Empty : public Tile
{
 public:
  check_enter()
  {
   return 1;
  }
  void draw()
  {
   setcolor(0);
   rectangle(x,y,x+sized,y+sized);
   setfillstyle(1,0);
   floodfill(x+10,y+10,0);

  }
  int bang(){}
  void remove(){}
};


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

   Enemy *e[5];

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
 int i,j,buf;
 int f=0;
 int n=1,n1=1;
 Bomb*b;

 int key=0;
 int time=0,time1=0;

 Tile*t;

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
       if(f)                         //если бомба существует,то стераем героя
       {                             //записываем в него новые координаты
	m.tiles[i][j]->remove();     //указатель новой позиции направляем на героя
				     //указатель старой позиции направляем на адрес бомбы
	m.tiles[i][j]->ent_y(m.tiles[i+1][j]->get_y());

	m.tiles[i+1][j]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i][j]->draw();
	i++;

	m.p->ent_i(i);

	m.p->draw();

	continue;
       }
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
      if(f)
       {
	m.tiles[i][j]->remove();

	m.tiles[i][j]->ent_y(m.tiles[i-1][j]->get_y());

	m.tiles[i-1][j]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i][j]->draw();
	i--;

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
      if(f)
       {
	m.tiles[i][j]->remove();

	m.tiles[i][j]->ent_x(m.tiles[i][j+1]->get_x());

	m.tiles[i][j+1]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i][j]->draw();
	j++;

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
      if(f)
       {
	m.tiles[i][j]->remove();

	m.tiles[i][j]->ent_x(m.tiles[i][j-1]->get_x());

	m.tiles[i][j-1]=m.tiles[i][j];
	m.tiles[i][j]=b;

	m.tiles[i][j]->draw();
	j--;

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
      b =new Bomb;
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
  //-----------------------------
 if(time==400)                 //движение врагов
 {                             //двигаются по одной линии от препятствия до
  i=m.e[1]->get_i();           //до препятствия
  j=m.e[1]->get_j();
  if(m.tiles[i-n][j]->check_enter())
     {
      m.tiles[i][j]->remove();

      buf=m.tiles[i][j]->get_y();
      m.tiles[i][j]->ent_y(m.tiles[i-n][j]->get_y());
      m.tiles[i-n][j]->ent_y(buf);

      t=m.tiles[i-n][j];
      m.tiles[i-n][j]=m.tiles[i][j];
      m.tiles[i][j]=t;

      i-=n;
      m.e[1]->ent_i(i);

      m.tiles[i][j]->draw();
     }
  else
  {
   if(n==1) n=-1;
   else n=1;
  }
  time=0;
 }

 if(time1==600)
 {
  i=m.e[0]->get_i();
  j=m.e[0]->get_j();
  if(m.tiles[i][j+n]->check_enter())
     {
      m.tiles[i][j]->remove();

      buf=m.tiles[i][j]->get_x();
      m.tiles[i][j]->ent_x(m.tiles[i][j+n]->get_x());
      m.tiles[i][j+n]->ent_x(buf);

      t=m.tiles[i][j+n];
      m.tiles[i][j+n]=m.tiles[i][j];
      m.tiles[i][j]=t;

      j+=n;
      m.e[0]->ent_j(j);

      m.tiles[i][j]->draw();
     }
   else
  {
   if(n1==1) n1=-1;
   else n1=1;
  }
  time1=0;
 }
//--------------------------------------------------------

 if(f)               //если бомба существует
 {
  if(b->get_time()==0)//дошел ли таймер до 0?
  {
   int i,j;
   i=b->get_i();
   j=b->get_j();
   b->remove();                  //стираем бомбу
   m.tiles[i][j] = new Empty;    //создаем новую пустую клетку
   m.tiles[i][j]->ent_x(b->get_x());//на нее указывает то что указывало на бомбу
   m.tiles[i][j]->ent_y(b->get_y());//рисовать не нужно

   f=0;                             //f=0 бомбы не существует,можно поставить новую
  }
  b->timedawn();                    //уменьшение таймера бомбы
 }                                  //ошибки в логике быть не может
  delay(50);                        //только если в адресах
  time+=50;
  time1+=50;

 }while(key!=27);
 delete t;
}

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

 blockTexture	  =block1();
 destrBlockTexture=block2();
 playerTexture	  =hero();
 enemyTexture     =enemy(random(16));
 bombTexture      =bomba();

 Game g;
 g.upgrade();
 closegraph();

 delete blockTexture;
 delete destrBlockTexture;
 delete playerTexture;
 delete enemyTexture;
 delete bombTexture;

}


