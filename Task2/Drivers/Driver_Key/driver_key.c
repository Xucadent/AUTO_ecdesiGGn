#include "driver_key.h"

keysta_t xdata keysta[3] = {0,0,0};
keysta_t xdata keylast[3] = {0,0,0};
int16_t xdata keybuf[3] = {0,0,0};
keyactqueue_t xdata keyact_q[3];

void KeyInit(void)
{
    uint8_t keynum;
    for(keynum=0; keynum<3; keynum++)
    {
        keyact_q[keynum].head = 0;
        keyact_q[keynum].tail = 0;
    }
}

void KeyActEnqueue(uint8_t keynum, uint8_t keysta)
{
    uint8_t tail = keyact_q[keynum].tail;
    keyact_q[keynum].keyact[tail] = keysta;
    keyact_q[keynum].tail = (tail + 1) % KAQLEN;
    if(keyact_q[keynum].head == keyact_q[keynum].tail)
    {
        keyact_q[keynum].head = (keyact_q[keynum].head + 1) % KAQLEN;
    }
}

uint8_t KeyActDequeue(uint8_t keynum)
{
    uint8_t head = keyact_q[keynum].head;
	if(keyact_q[keynum].head == keyact_q[keynum].tail)
    {
        return 0;
    }
    keyact_q[keynum].head = (head + 1) % KAQLEN;
    return keyact_q[keynum].keyact[head];
}

void KeyDetect(void)
{
    uint8_t keynum = 0;

    for(keynum=0; keynum<3; keynum++)
    {
        KEYRESET(keynum);
        if(KEYCOMM == 0)
            keybuf[keynum] = keybuf[keynum] >= 2000 ? 2000 : (keybuf[keynum]+1);   
        else
        {
            keybuf[keynum] = keybuf[keynum] <= 10 ? 0 : (keybuf[keynum]-10);
            if(keybuf[keynum] >= 800 && keybuf[keynum] < 1000)
                keybuf[keynum] = 0;
        }
            
        KEYSET(keynum);

        keylast[keynum] = keysta[keynum];
        if(keybuf[keynum] < 50)
            keysta[keynum] = KeyUp;
        else if(keybuf[keynum] < 800 && keybuf[keynum] >= 50)
            keysta[keynum] = ShortPress;
        else if(keybuf[keynum] >= 1000)
            keysta[keynum] = LongPress;
        // else if(keylast[keynum] == LongPress)
        //     keybuf[keynum] = 0; // 从长按直接跳转到抬起
        
        if(keylast[keynum] == ShortPress && keysta[keynum] == KeyUp)
                KeyActEnqueue(keynum, ShortPress);
        // else if (keylast[keynum] != LongPress && keysta[keynum] == LongPress)
        // else if(keysta[keynum] == LongPress && keylast[keynum] != LongPress)
        //         KeyActEnqueue(keynum, LongPressDown);
        else if(keysta[keynum] == LongPress)
                KeyActEnqueue(keynum, LongPress);
    }
}

uint8_t GetKeyStatus(uint8_t keynum)
{
    return KeyActDequeue(keynum);
}
