#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include <stdlib.h>
#include <stdbool.h>

int x = 80;  //ball position
int y = 40;
int y1 = 40;
int state = 0;  //0still 1down 2up
int score = 0;
bool right = true;
bool down = true;
bool notend = true;
int x_speed = 1;
int y_speed = 1;
unsigned int seed = 0;

void Inp_init(void)
{
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

void Adc_init(void) 
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_1);
    RCU_CFG0|=(0b10<<14)|(1<<28);
    rcu_periph_clock_enable(RCU_ADC0);
    ADC_CTL1(ADC0)|=ADC_CTL1_ADCON;
}

void IO_init(void)
{
    Inp_init(); // inport init
    Adc_init(); // A/D init
    Lcd_Init(); // LCD init
}

void checkHandle()
{
    if((y1 <= 9 && Get_Button(1)==1) || (y1 >= 71 && Get_Button(0)==1)) {
        state = 0;
        return;
    }
    if (Get_Button(0)==1 && Get_Button(1)==1) //still
    {
        state = 0;
    }
    else if (Get_Button(0)==1 && Get_Button(1)==0) //down
    {
        state = 1;
        seed = ((seed + 1) + rand()) & 0b11110001;    // set seed
    }
    else if (Get_Button(0)==0 && Get_Button(1)==1) //up
    {
        state = 2;
        seed = ((seed + 2) + rand()) & 0b11110001;    // set seed
    }
    else
    {
        state = 0;
    }
}

void checkCollision()
{
    if(x >= 156) {
        notend = false;
        return;
    }

    srand(seed);
    if(y >= 77 || y <= 3)
        down = !down;
    if(x >= 154 && y>=y1-8 && y<=y1+8) {
        right = !right;
        x_speed = rand() % 2 + 1;
        y_speed = rand() % (score + 2) + 1;
        score = score + 1;
    }
    if(x <= 5) {
        right = !right;
        x_speed = rand() % 2 + 1;
        y_speed = rand() % (score + 3) + 1;
    }
}

int main(void)
{
    IO_init();         // init OLED
    drawUI();

    while(notend && score <= 9)
    {
        int left_y;
        if(y < 9) {
            left_y = 9;
        } else if(y > 70) {
            left_y = 70;
        } else {
            left_y = y;
        }

        printScore(score);
        drawBall(x, y);
        drawLeftBoard(left_y);
        drawRightBoard(y1);
        checkHandle();
        if (state == 1 && y1 < 77)
            y1 = y1 + 2;
        if (state == 2 && y1 > 3)
            y1 = y1 - 2;

        checkCollision();
        if(right)
        {
            x  = (x+x_speed>=156) ? 156 : x+x_speed;
            if(down)
            {
                y = (y+y_speed>=77) ? 77 : y+y_speed;
            }
            else
            {
                y = (y-y_speed<=3) ? 3 : y-y_speed;
            }
        }
        else
        {
            x  = (x-x_speed<=5) ? 5 : x-x_speed;
            if(down)
            {
                y = (y+y_speed>=77) ? 77 : y+y_speed;
            }
            else
            {
                y = (y-y_speed<=3) ? 3 : y-y_speed;
            }
        }
        delay_1ms(15);
        flushTable();
    }
    
    if(score > 9) {
        winUI();
    } else {
        loseUI(score);
    }

    return 0;
}
