#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "ctype.h"

#include "common.h"
#include "string.h"
#include "assert.h"

#define SNAKE_ARRAY_SIZE 310

const char SNAKE_HEAD = 'X';
const char SNAKE_BODY = '#';
const char WALL = '#';
const char FOOD = '*';
const char BLANK = ' ';


int get_game_speed(void)
{
    int speed;
    clrscr();

    do
    {
        gotoxy(10, 5);
        printf("Select The game speed between 1 and 9.");
        speed = wait_4_key() - '0';
    } while(speed < 1 || speed > 9);
    return(speed);
}

void pause_menu(void)
{
    gotoxy(28, 23);
    printf("**Paused**");

    wait_4_key();
    gotoxy(28, 23);
    printf("            ");

}

int check_keys_pressed(int direction)
{
    int pressed;

    if(kbhit())
    {
        pressed = getch();
        if (direction != pressed)
        {
            if(pressed == DOWN_ARROW && direction != UP_ARROW)
            {
                direction = pressed;
            }
            else if (pressed == UP_ARROW && direction != DOWN_ARROW)
            {
                direction = pressed;
            }
            else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
            {
                direction = pressed;
            }
            else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
            {
                direction = pressed;
            }
            else if (pressed == EXIT_BUTTON || pressed == PAUSE_BUTTON)
            {
                pause_menu();
            }
        }
    }
    return direction;
}

int collision_snake(int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect)
{
    int i;
    for (i = detect; i < snakeLength; i++)
    {
        if ( x == snakeXY[0][i] && y == snakeXY[1][i])
        {
            return 1;
        }

    }
    return 0;
}

int generate_food(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
    do
    {
        srand(time(NULL));
        foodXY[0] = rand() % (width - 2) + 2;
        srand(time(NULL));
        foodXY[1] = rand() % (height - 6) + 2;
    } while (collision_snake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0));

    gotoxy(foodXY[0] ,foodXY[1]);
    printf("%c", FOOD);

    // wait_4_key();
    return 0;
}

void move_snake_array(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
    int i;
    for( i = snakeLength-1; i >= 1; i-- )
    {
        snakeXY[0][i] = snakeXY[0][i-1];
        snakeXY[1][i] = snakeXY[1][i-1];
    }

    switch(direction)
    {
        case DOWN_ARROW:
            snakeXY[1][0]++;
            break;
        case RIGHT_ARROW:
            snakeXY[0][0]++;
            break;
        case UP_ARROW:
            snakeXY[1][0]--;
            break;
        case LEFT_ARROW:
            snakeXY[0][0]--;
            break;
    }
}

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
    int x;
    int y;

    x = snakeXY[0][snakeLength-1];
    y = snakeXY[1][snakeLength-1];

    gotoxy(x, y);
    printf("%c", BLANK);

    gotoxy(snakeXY[0][0], snakeXY[1][0]);
    printf("%c", SNAKE_BODY);

    move_snake_array(snakeXY, snakeLength, direction);

    gotoxy(snakeXY[0][0], snakeXY[1][0]);
    printf("%c", SNAKE_HEAD);

    gotoxy(1,1);
}

int eat_food(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[])
{
    if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
    {
        foodXY[0] = 0;
        foodXY[1] = 0;

        printf("\7"); //Beep
        return 1;
    }

    return 0;
}

int collision_detection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth,
    int consoleHeight, int snakeLength )
{
    int colision = 0;
    if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) ||
        (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4))
    {
        colision = 1;
    }
    else if (collision_snake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1))
    {
        colision = 1;
    }

    return colision;
}

void refresh_info_bar(int score, int speed)
{
    gotoxy(5, 23);
    printf("Score: %d", score);

    gotoxy(5, 24);
    printf("Speed: %d", speed);

    gotoxy(52, 23);
    printf("Coder: HZJ");

    gotoxy(52, 24);
    printf("Version: 0.5");

    // wait_4_key();
}

void create_high_scores(void)
{
    FILE *file;
    int i;

    file = fopen("highscores","w+");

    if(file == NULL)
    {
        printf("FAILED TO CREATE HIGHSCORES!!! EXITING!");
        exit(0);
    }

    for(i = 0; i < 5; i++)
    {
        fprintf(file,"%d",i+1);
        fprintf(file,"%s","\t0\t\t\tEMPTY\n");
    }

    fclose(file);
    return;
}

int get_lowest_score(void)
{
    FILE *fp;
    char str[128];
    int lowestScore = 0;
    int i;
    int intLength;

    if ((fp = fopen("highscores", "r")) == NULL)
    {
        create_high_scores();
        if((fp = fopen("highscores", "r")) == NULL)
        {
            exit(1);
        }
    }

    while (!feof(fp))
    {
        fgets(str, 126, fp);
    }
    fclose(fp);

    i = 0;

    while(str[2+i] != '\t')
    {
        i++;
    }

    intLength = i;

    for(i=0;i < intLength; i++)
    {
        lowestScore = lowestScore + ((int)str[2+i] - 48) * pow(10,intLength-i-1);
    }

    return lowestScore;
}

void input_score(int score)
{
    FILE *fp;
    FILE *file;
    char str[20];
    int fScore;
    int i, s, y;
    int intLength;
    int scores[5];
    int x;
    char highScoreName[20];
    char highScoreNames[5][20];

    char name[20];

    int entered = 0;

    clrscr();

    if((fp = fopen("highscores", "r")) == NULL)
    {
        create_high_scores();
        if((fp = fopen("highscores", "r")) == NULL)
        {
            exit(1);
        }
    }
    gotoxy(10,5);
    printf("Your Score made it into the top 5!!!");
    gotoxy(10,6);
    printf("Please enter your name: ");
    fgets(name, 20, stdin);

    x = 0;
    while(!feof(fp))
    {
        fgets(str, 126, fp);

        i=0;

        while(str[2+i] != '\t')
        {
            i++;
        }

        s = i;
        intLength = i;
        i=0;
        while(str[5+s] != '\n')
        {
            highScoreName[i] = str[5+s];
            s++;
            i++;
        }

        fScore = 0;
        for(i=0;i < intLength; i++)
        {
            fScore = fScore + ((int)str[2+i] - 48) * pow(10,intLength-i-1);
        }

        if(score >= fScore && entered != 1)
        {
            scores[x] = score;
            strcpy(highScoreNames[x], name);

            x++;
            entered = 1;
        }

        strcpy(highScoreNames[x], highScoreName);
        scores[x] = fScore;

        for(y=0;y<20;y++)
        {
            highScoreName[y] = '\0';
        }

        x++;
        if(x >= 5)
            break;
    }

    fclose(fp);

    file = fopen("highscores","w+");

    for(i = 0; i < 5; i++)
    {
        fprintf(file, "%d\t%d\t\t\t%s\n", i+1, scores[i], highScoreNames[i]);
    }

    fclose(file);
}

void display_high_scores(void)
{
    FILE *fp;
    char str[128];
    int x = 10;
    int y = 5;

    clrscr();

    if((fp = fopen("highscores", "r")) == NULL)
    {
        create_high_scores();
        if((fp = fopen("highscores", "r")) == NULL)
        {
            exit(1);
        }
    }

    gotoxy(x, y);
    y++;
    printf("High Scores");
    gotoxy(x, y);
    y++;
    printf("Rank\tScore\t\t\tName");
    while(!feof(fp))
    {
        gotoxy(x, y);
        y++;
        if(fgets(str, 126, fp))
        {
            printf("%s", str);
        }

    }

    fclose(fp);
    gotoxy(x, y);
    y++;

    printf("Press any key to continue...");
    wait_4_key();
}

void you_win_screen(void)
{
    int x = 6;
    int y = 7;

    gotoxy(x, y);
    y++;
    printf("'##:::'##::'#######::'##::::'##::::'##:::::'##:'####:'##::: ##:'####:");
    gotoxy(x, y);
    y++;
    printf(". ##:'##::'##.... ##: ##:::: ##:::: ##:'##: ##:. ##:: ###:: ##: ####:");
    gotoxy(x, y);
    y++;
    printf(":. ####::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ####: ##: ####:");
    gotoxy(x, y);
    y++;
    printf("::. ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ## ## ##:: ##::");
    gotoxy(x, y);
    y++;
    printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##. ####::..:::");
    gotoxy(x, y);
    y++;
    printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##:. ###:'####:");
    gotoxy(x, y);
    y++;
    printf("::: ##::::. #######::. #######:::::. ###. ###::'####: ##::. ##: ####:");
    gotoxy(x, y);
    y++;
    printf(":::..::::::.......::::.......:::::::...::...:::....::..::::..::....::");
    gotoxy(x, y);
    y++;

    wait_4_key();
    clrscr();
}

void game_over_screen(void)
{
    int x = 17;
    int y = 3;

    //http://www.network-science.de/ascii/ <- Ascii Art Gen

    gotoxy(x, y);
    y++;
    printf(":'######::::::'###::::'##::::'##:'########:\n");
    gotoxy(x, y);
    y++;
    printf("'##... ##::::'## ##::: ###::'###: ##.....::\n");
    gotoxy(x, y);
    y++;
    printf(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
    gotoxy(x, y);
    y++;
    printf(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
    gotoxy(x, y);
    y++;
    printf(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
    gotoxy(x, y);
    y++;
    printf(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
    gotoxy(x, y);
    y++;
    printf(". ######::: ##:::: ##: ##:::: ##: ########:\n");
    gotoxy(x, y);
    y++;
    printf(":......::::..:::::..::..:::::..::........::\n");
    gotoxy(x, y);
    y++;
    printf(":'#######::'##::::'##:'########:'########::'####:\n");
    gotoxy(x, y);
    y++;
    printf("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
    gotoxy(x, y);
    y++;
    printf(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
    gotoxy(x, y);
    y++;
    printf(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
    gotoxy(x, y);
    y++;
    printf(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
    gotoxy(x, y);
    y++;
    printf(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
    gotoxy(x, y);
    y++;
    printf(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
    gotoxy(x, y);
    y++;
    printf(":.......::::::...:::::........::..:::::..::....::\n");

    wait_4_key();
    clrscr();
}

void start_game(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth,
    int consoleHeight, int snakeLength, int direction, int score, int speed)
{
    int gameOver = 0;
    clock_t endWait;

    //CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
    int waitMili = CLOCKS_PER_SEC - (speed) * (CLOCKS_PER_SEC / 10);
    int tempScore = 10 * speed;
    int oldDirection;
    int canChangeDirection = 1;

    endWait = clock() + waitMili;

    do
    {
        if(canChangeDirection)
        {
            oldDirection = direction;
            direction = check_keys_pressed(direction);
        }

        if(oldDirection != direction)
        {
            canChangeDirection = 0;
        }

        if(clock() >= endWait)
        {
            move(snakeXY, snakeLength, direction);
            canChangeDirection = 1;

            if(eat_food(snakeXY, foodXY))
            {
                generate_food( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
                snakeLength++;
                score += speed;
                if(score >= 10 * speed + tempScore)
                {
                    speed++;
                    tempScore = score;

                    if(speed <= 9)
                    {
                        waitMili = waitMili - (CLOCKS_PER_SEC/10);
                    }

                    else if (waitMili >= 40)
                    {
                        waitMili = waitMili - (CLOCKS_PER_SEC/200);
                    }
                }

                refresh_info_bar(score, speed);
            }

            endWait = clock() + waitMili;
        }

        gameOver = collision_detection(snakeXY, consoleWidth, consoleHeight, snakeLength);

        if(snakeLength >= SNAKE_ARRAY_SIZE - 5)
        {
            gameOver = 2;
            score += 1500;
        }

    } while (!gameOver);

    switch(gameOver)
    {
        case 1:
            printf("\7"); //Beep
            printf("\7"); //Beep

            game_over_screen();

            break;
        case 2:
            you_win_screen();
            break;
    }

    if(score >= get_lowest_score() && score != 0)
    {
        input_score(score);
        display_high_scores();
    }
}

void load_enviroment(int consoleWidth, int consoleHeight)
{
    int i;
    int x = 1;
    int y = 1;
    int rectangleHeight = consoleHeight - 4;

    UNUSED(i);
    clrscr();

    gotoxy(x,y);

    for (; y < rectangleHeight; y++)
    {
        gotoxy(x, y);
        printf("%c", WALL);

        gotoxy(consoleWidth, y);
        printf("%c", WALL);
    }

    y = 1;
    for (; x < consoleWidth + 1; x++)
    {
        gotoxy(x, y);
        printf("%c", WALL);

        gotoxy(x, rectangleHeight);
        printf("%c", WALL);
    }
    // wait_4_key();
}

void load_snake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
    int i;
    for (i = 0; i < snakeLength; i++)
    {
        gotoxy(snakeXY[0][i], snakeXY[1][i]);
        printf("%c", SNAKE_BODY);
    }

    // wait_4_key();
}

void prepair_snake_array(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
    int i;
    int snakeX = snakeXY[0][0];
    int snakeY = snakeXY[1][0];

    for(i = 1; i <= snakeLength; i++)
    {
        snakeXY[0][i] = snakeX + i;
        snakeXY[1][i] = snakeY;
    }
}

void load_game(void)
{
    int snakeXY[2][SNAKE_ARRAY_SIZE];

    int snakeLength = 4;

    int direction = LEFT_ARROW;

    int foodXY[] = {5,5};

    int score = 0;


    int consoleWidth = 80;
    int consoleHeight = 25;

    int speed = get_game_speed();

    snakeXY[0][0] = 40;
    snakeXY[1][0] = 10;

    load_enviroment(consoleWidth, consoleHeight);
    prepair_snake_array(snakeXY, snakeLength);
    load_snake(snakeXY, snakeLength);
    generate_food(foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
    refresh_info_bar(score, speed);
    start_game(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);
}

int menu_selector(int x, int y, int yStart)
{
    char key;
    int i = 0;

    x = x - 2;
    gotoxy(x, yStart);

    printf(">");

    gotoxy(1,1);

    do
    {
        key = wait_4_key();
        if (key == (char)UP_ARROW)
        {
            gotoxy(x, yStart+i);
            printf(" ");

            if (yStart >= yStart+i )
                i = y - yStart - 2;
            else
                i--;
            gotoxy(x, yStart+i);
            printf(">");
        }
        else if (key == (char)DOWN_ARROW)
        {
            gotoxy(x, yStart+i);
            printf(" ");

            if (i+2 >= y - yStart )
                i = 0;
            else
                i++;
            gotoxy(x, yStart+i);
            printf(">");
        }
    } while(key != (char)ENTER_KEY);
    return(i);
}

void welcome_art(void)
{
    clrscr();
    //Ascii art reference: http://www.chris.com/ascii/index.php?art=animals/reptiles/snakes
    printf("\n");
    printf("\t\t    _________         _________             \n");
    printf("\t\t   /         \\       /         \\          \n");
    printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\       \n");
    printf("\t\t  |  |     |  |     |  |     |  |           \n");
    printf("\t\t  |  |     |  |     |  |     |  |           \n");
    printf("\t\t  |  |     |  |     |  |     |  |         /	\n");
    printf("\t\t  |  |     |  |     |  |     |  |       //	\n");
    printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ /   \n");
    printf("\t\t  \\__/      \\         /       \\        /     \n");
    printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~         \n");
    printf("\t\t    ^                                           \n");
    printf("\t      Welcome To The Snake Game!              \n");
    printf("\t      Press Any Key To Continue...            \n");
    printf("\n");

    wait_4_key();
}

void tips(void)
{
    int x = 10;
    int y = 5;

    clrscr();
    gotoxy(x, y);
    y++;
    printf("Tips\n");
    gotoxy(x, y);
    y++;
    printf("Use the following arrow keys to direct the snake to the food: ");
    gotoxy(x, y);
    y++;
    gotoxy(x, y);
    y++;
    printf("Right Arrow");
    gotoxy(x, y);
    y++;
    printf("Left Arrow");
    gotoxy(x, y);
    y++;
    printf("Top Arrow");
    gotoxy(x, y);
    y++;
    printf("Bottom Arrow");
    gotoxy(x, y);
    y++;
    gotoxy(x, y);
    y++;
    printf("P & Esc pauses the game.");
    gotoxy(x, y);
    y++;
    gotoxy(x, y);
    y++;
    printf("Press any key to continue...");
    wait_4_key();
}

void exit_yn(void)
{
    char pressed;

    clrscr();
    gotoxy(9, 8);
    printf("Are you sure you want to exit(Y/N)\n");

    do
    {
        pressed = wait_4_key();
        pressed = tolower(pressed);
    } while(!(pressed == 'y' || pressed == 'n'));

    printf("%c", pressed);

    if (pressed == 'y')
    {
        printf("\t      Press Enter To End...            \n");
        do
        {
            pressed = wait_4_key();
        } while(pressed != ENTER_KEY);
        clrscr();
        exit(1);
    }
}

int main_menu(void)
{
    int x = 10;
    int y = 5;
    int yStart = y;

    int selected;

    clrscr();
    gotoxy(x, y);
    y++;
    printf("New Game\n");
    gotoxy(x, y);
    y++;
    printf("High Scores\n");
    gotoxy(x, y);
    y++;
    printf("Tips\n");
    gotoxy(x, y);
    y++;
    printf("Exit\n");
    gotoxy(x, y);
    y++;

    selected = menu_selector(x, y, yStart);

    return(selected);
}

int snake_test(void)
{

    welcome_art();

    do
    {
        switch(main_menu())
        {
            case 0:
                load_game();
                break;
            case 1:
                display_high_scores();
                break;
            case 2:
                tips();
                break;
            case 3:
                exit_yn();
                break;
        }
    } while(1);

    return(0);
}
