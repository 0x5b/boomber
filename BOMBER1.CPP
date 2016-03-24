#include <stdio.h>
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

  int get_x()        { return x; }

  int get_y()        { return y; }

  virtual void draw()=0;

  virtual void remove()=0;

  virtual int check_enter()=0;

  virtual int bang()=0;
};

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
 public:
  Bomb()
  {
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

  int bang()
  {

  }

  ~Bomb() { delete texture; }
};

class Aliving : public Tile
{

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
  int check_enter(){}

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
  int check_enter(){}

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

    if((f=fopen("c:\\bc\\source\\bomber\\map.txt","r"))==NULL)
     printf("file not found");

    else
    {
     while(!feof(f))
     {
      ch=fgetc(f);
      if(ch=='x')
      {
       tiles[i][j] = new Block();
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
       continue;
      }

      if(ch=='o')
      {
       tiles[i][j] = new Empty();
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
       continue;
      }

      if(ch=='b')
      {
       tiles[i][j] = new DestrBlock();
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
       continue;
      }

      if(ch=='p')
      {
       tiles[i][j] = new Player();
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
       continue;
      }

      if(ch=='e')
      {
       tiles[i][j] = new Enemy();
       tiles[i][j]->ent_x(x);
       tiles[i][j]->ent_y(y);
       x+=sized;
       j++;
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
 int i=1,j=1,buf=0;
 int ei=7,ej=13,n=1;
 int key=0;
 int time=0;
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
      if(m.tiles[i+1][j]->check_enter())
      {
       m.tiles[i][j]->remove();
       buf=m.tiles[i][j]->get_y();
       m.tiles[i][j]->ent_y(m.tiles[i+1][j]->get_y());
       m.tiles[i+1][j]->ent_y(buf);
       t=m.tiles[i+1][j];
       m.tiles[i+1][j]=m.tiles[i][j];
       m.tiles[i++][j]=t;

       m.tiles[i][j]->draw();
       break;
      }
      break;
     }
    case 119:
    {
     if(m.tiles[i-1][j]->check_enter())
     {
      m.tiles[i][j]->remove();
      buf=m.tiles[i][j]->get_y();
      m.tiles[i][j]->ent_y(m.tiles[i-1][j]->get_y());
      m.tiles[i-1][j]->ent_y(buf);
      t=m.tiles[i-1][j];
      m.tiles[i-1][j]=m.tiles[i][j];
      m.tiles[i--][j]=t;


      m.tiles[i][j]->draw();
      break;
     }
     break;
    }

    case 100:
    {
     if(m.tiles[i][j+1]->check_enter())
     {
      m.tiles[i][j]->remove();
      buf=m.tiles[i][j]->get_x();
      m.tiles[i][j]->ent_x(m.tiles[i][j+1]->get_x());
      m.tiles[i][j+1]->ent_x(buf);
      t=m.tiles[i][j+1];
      m.tiles[i][j+1]=m.tiles[i][j];
      m.tiles[i][j++]=t;

      m.tiles[i][j]->draw();
      break;
     }
     break;
    }
    case 97 :
    {
     if(m.tiles[i][j-1]->check_enter())
     {
      m.tiles[i][j]->remove();
      buf=m.tiles[i][j]->get_x();
      m.tiles[i][j]->ent_x(m.tiles[i][j-1]->get_x());
      m.tiles[i][j-1]->ent_x(buf);
      t=m.tiles[i][j-1];
      m.tiles[i][j-1]=m.tiles[i][j];
      m.tiles[i][j--]=t;

      m.tiles[i][j]->draw();
      break;
     }
     break;
    }
   }
   continue;
  }
  //-----------------------------
 if(time==600)
 {
  if(m.tiles[ei-n][ej]->check_enter())
     {
      m.tiles[ei][ej]->remove();
      buf=m.tiles[ei][ej]->get_y();
      m.tiles[ei][ej]->ent_y(m.tiles[ei-n][ej]->get_y());
      m.tiles[ei-n][ej]->ent_y(buf);
      t=m.tiles[ei-n][j];
      m.tiles[ei-n][ej]=m.tiles[ei][ej];
      m.tiles[ei][ej]=t;
      ei-=n;

      m.tiles[ei][ej]->draw();
     }
  else n=-abs(n);
  time=0;
 }
  delay(50);
  time+=50;
 }while(key!=27);
}

void main()
{
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
 enemyTexture     =enemy(14);
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


