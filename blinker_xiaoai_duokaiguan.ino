#define BLINKER_WIFI
#define BLINKER_MIOT_MULTI_OUTLET    //设置为小爱多个插座的模式

#include <Blinker.h>
BlinkerButton Button1("btn-km");     //设置blinkerapp内数据键名
BlinkerButton Button2("btn-kckm");   //这里只用了其中三个
BlinkerButton Button3("btn-gckm");  
BlinkerText Text1("text_1");


char i;

char auth[] = "******";
char ssid[] = "******";
char pswd[] = "******";

bool oState[5] = { false };    //设置5路开关控制

void miotPowerState(const String & state, uint8_t num)     //小爱同学控制指令 
{
    BLINKER_LOG("need set outlet: ", num, ", power state: ", state);   
    if(num==1){
      i=D1;          //指定每一路开关对应在开发板上的通道接口
    }else if(num==2){
      i=D2;
    }else if(num==3){
      i=D5;
    }

    if (state == BLINKER_CMD_ON) {      
        digitalWrite(i, HIGH);      //这里控制模式都是点动模式 
        Blinker.delay(200);         //高电平触发继电器200毫秒后低电平断开继电器
        digitalWrite(i, LOW);

        BlinkerMIOT.powerState("on", num);
        BlinkerMIOT.print();

        oState[num] = true;
        
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(i, HIGH);
        Blinker.delay(200);
        digitalWrite(i, LOW);

        BlinkerMIOT.powerState("off", num);
        BlinkerMIOT.print();

        oState[num] = false;

        if (num == 0)
        {
            for (uint8_t o_num = 0; o_num < 5; o_num++)
            {
                oState[o_num] = false;
            }
        }
    }
}

void miotQuery(int32_t queryCode, uint8_t num)
{
    BLINKER_LOG("AliGenie Query outlet: ", num,", codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
            BlinkerMIOT.print();
            break;
    }
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}


void button1_callback(const String & state)     //点灯app内控制按键触发
{
    BLINKER_LOG("get button state: ", state);
        digitalWrite(D1, HIGH);
        Blinker.delay(200);
        digitalWrite(D1, LOW);
        Blinker.print("开门命令已经处理");
        Text1.print("shuju");
        
}

void button2_callback(const String & state)     //点灯app内控制按键触发
{
    BLINKER_LOG("get button state: ", state);
        digitalWrite(D2, HIGH);
        Blinker.delay(400);
        Blinker.print("开车库门已经处理");
        digitalWrite(D2, LOW);
}

void button3_callback(const String & state)     //点灯app内控制按键触发
{
    BLINKER_LOG("get button state: ", state);
        digitalWrite(D5, HIGH);
        Blinker.delay(300);
        Blinker.print("关车库门已经处理");
        digitalWrite(D5, LOW);
}


void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(D1, OUTPUT);
    digitalWrite(D1, LOW);
    pinMode(D2, OUTPUT);
    digitalWrite(D2, LOW);
    pinMode(D5, OUTPUT);
    digitalWrite(D5, LOW);
    

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    Button1.attach(button1_callback); 
    Button2.attach(button2_callback); 
    Button3.attach(button3_callback); 
}

void loop()
{
    Blinker.run();
}
