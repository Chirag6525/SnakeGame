#include<bits/stdc++.h>
#include<conio.h> //keypress hit
#include<windows.h> //coordinates
using namespace std;


//Maximum length of snake
#define MAX_LENGTH 1000


// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';


//Width of Board
int consoleWidth, consoleHeight;


//Initialize the Screen
void initScreen(){
    //information about console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight =csbi.srWindow.Bottom - csbi.srWindow.Top +1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}


// Point Structure
struct Point{
    int xCoord;
    int yCoord;
    Point(){

    }
    Point(int x, int y){
        xCoord = x;
        yCoord = y;
    }
};


//Snake Class
class Snake{
    int length; //lenght of snake
    char direction; //direction of snake
    public:
        Point body[MAX_LENGTH];//length of snake not more than maximum length
        //Constructor
        Snake(int x,int y){
            length = 1; //initially snake length will be 1
            body[0] = Point(x,y); //initially the position of snake
            direction = DIR_RIGHT;//initially snake will move in right direction
        }
        //destructor
        // ~Snake(){
        //     delete[] body; 
        // }

        //return private length of snake
        int getLength(){
            return length;
        }
        //Change direction
        void changeDirection(char newDirection){
            if(newDirection == DIR_UP && direction != DIR_DOWN){
                direction = newDirection;
            }
            else if(newDirection == DIR_DOWN && direction != DIR_UP){
                direction = newDirection;
            }
            else if(newDirection == DIR_LEFT && direction != DIR_RIGHT){
                direction = newDirection;
            }
            else if(newDirection == DIR_RIGHT && direction != DIR_LEFT){
                direction = newDirection;
            }
        }
        //game over condition and movement of snake
        bool move(Point food){
            for(int i = length-1;i>0;i--){
                body[i]=  body[i-1];
            }
            //head movement
            switch(direction){
                int val;
                case DIR_UP:
                    val = body[0].yCoord;
                    body[0].yCoord = val-1;
                    break;
                case DIR_DOWN:
                    val = body[0].yCoord;
                    body[0].yCoord = val+1;
                    break;
                case DIR_RIGHT:
                    val = body[0].xCoord;
                    body[0].xCoord = val+1;
                    break;
                case DIR_LEFT:
                    val = body[0].xCoord;
                    body[0].xCoord = val-1;
                    break;
            }
            //snake bites itself
            for(int i = 1;i<length;i++){
                if(body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord){
                    return false;
                }
            }

            //snake eats food
            if(food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord){
                body[length] = Point(body[length-1].xCoord, body[length-1].yCoord);
                length++;
            }
            return true;
        }
};



//Board Class
class Board{
    Snake *snake; //dynammically
    const char SNAKE_BODY = 'O'; //representation of snake
    Point food; //Point of food
    const char FOOD = 'o'; //representation of food
    int score; //Score of game
    public:
        //Constructor
        Board(){
            spawnFood();
            snake = new Snake(10,10);
            score =0;
        }
        //Destructor
        ~Board(){
            delete snake;
        }
        //return the score
        int getScore(){
            return score;
        }
        void displayCurrentScore(){
            gotoxy(consoleWidth/2,0);
            cout << "Current Score: " << score;
        }
        //food will be initialize randomly
        void spawnFood(){
            int x = rand()%consoleWidth;
            int y = rand()%consoleHeight;
            //specify the food coordinates
            food = Point(x,y);
        }
        //fix the cursor position from window class 
        void gotoxy(int x,int y){
            COORD coord;
            coord.X = x;
            coord.Y = y;
            //draw a point on sonsole screen
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
        }
        void draw(){
            system("cls");
            //print the snake body
            for(int i= 0;i<snake->getLength();i++){
                gotoxy(snake->body[i].xCoord,snake->body[i].yCoord);
                cout << SNAKE_BODY;
            }
            //print the food
            gotoxy(food.xCoord,food.yCoord);
            cout << FOOD;
            displayCurrentScore();
        }
        //position update of snake
        bool update(){
            bool isalive = snake->move(food);
            if(isalive==false){
                return false;
            }
            if(food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord){
                score++;
                spawnFood();
            }
            return true;
        }
        //Input taken
        void getInput(){
            if(kbhit()){
                int key = getch();
                if(key == 'w' || key == 'W' || key == 72){
                    snake->changeDirection(DIR_UP);
                }
                else if(key == 'a' || key == 'A' || key == 75){
                    snake->changeDirection(DIR_LEFT);
                }
                else if(key == 's' || key == 'S' || key == 80){
                    snake->changeDirection(DIR_DOWN);
                }
                else if(key == 'd' || key == 'D' || key == 77){
                    snake->changeDirection(DIR_RIGHT);
                }
            }
        }
};
int main(){
    srand(time(0));
    initScreen();
    Board *board = new Board();
    while(board->update()){
        board->getInput();
        board->draw();
        Sleep(100);
    }
    cout << endl << "Game Over" << endl;
    cout << "Final Score is : " << board->getScore();
    return 0;
}