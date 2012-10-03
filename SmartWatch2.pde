#include <LCD5110_Basic.h>
#include <Time.h>
#include <avr/pgmspace.h>
#include "UIManager.h" 
#define TIME_MSG_LEN  11
#define TIME_HEADER  'T'
#define TIME_REQUEST  7 

LCD5110 lcd(3,4,5,6,7);


uint8_t bt_logo[] PROGMEM = { 0x42, 0x24, 0xff, 0x99, 0x66 };
uint8_t playButton[] PROGMEM = { 0x7f, 0x3e, 0x1c, 0x08, 0x00 };
uint8_t stopButton[] PROGMEM = { 0x7c, 0x7c, 0x7c, 0x7c, 0x7c };
uint8_t nextButton[] PROGMEM = { 0x7f, 0x3e, 0x1c, 0x08, 0x7f };
uint8_t prevButton[] PROGMEM = { 0x7f, 0x08, 0x1c, 0x3e, 0x7f };
uint8_t musicLogo[] PROGMEM = { 0xc0, 0xf8, 0x04, 0x62, 0x7f };
uint8_t speakerLogo[] PROGMEM = { 0x3c, 0x3c, 0x7e, 0xff, 0xff };
uint8_t tickLogo[] PROGMEM = { 0x38,0x70,0xE0,0x70,0x38,0x1C,0x0E,0x07 };

Label lbl_Music("Tiesto - Love Comes Again",musicLogo);
Label btn_Play("Play",playButton,"1");
Label btn_Stop("Stop",stopButton,"2");
Label btn_Next("Next",nextButton,"3");
Label btn_Prev("Prev",prevButton,"4");
Label btn_VU("Volume +",speakerLogo,"5");
Label btn_VD("Volume -",speakerLogo,"6");
Screen media("Media",500);


Label btn_FB("0 SMS","7");
Label btn_GM("0 eMail","8");
Label btn_CL("0 Calls","9");
Screen alerts("Alerts",500);

UIManager uiManager(&lcd);

SerialHandler serialHandler(&uiManager);

Screen clock("Clock",1000);
void setup()
{
	Serial.begin(115200);
	lcd.InitLCD();
	lcd.setFont(SmallFont);
	
	lbl_Music.ScrollText=true;
	media.AddLabel(&lbl_Music);
	media.AddLabel(&btn_Play);
	media.AddLabel(&btn_Stop);
	media.AddLabel(&btn_Next);
	media.AddLabel(&btn_Prev);
	media.AddLabel(&btn_VU);
	media.AddLabel(&btn_VD);
	
	alerts.AddLabel(&btn_FB);
	alerts.AddLabel(&btn_GM);
	alerts.AddLabel(&btn_CL);
	clock.WatchScreen=true;
	uiManager.AddScreen(&clock);
	uiManager.AddScreen(&media);
	uiManager.AddScreen(&alerts);
	uiManager.setTickLogo(tickLogo);
	
}
int j = 1; // integer used in scanning the array designating column number
//2-dimensional array for asigning the buttons and there high and low values
int Button[21][3] = {{1, 650, 750}, // button 1
                     {2, 800, 900}, // button 2
                     {3, 250, 350}, // button 3
                     {4, 500, 600}, // button 4
                     {5, 100, 200}}; // button 5 + button 6
int analogpin = 0; // analog pin to read the buttons
int label = 0;  // for reporting the button label
int counter = 0; // how many times we have seen new value
long time = 0;  // the last time the output pin was sampled
int debounce_count = 2; // number of millis/samples to consider before declaring a debounced input
int current_state = 0;  // the debounced input value
int ButtonVal;
int ScreenRefreshRate=500;
int CountRefresh=0;
int RealRefreshRate=50;
void loop()
{
  if (millis() != time)
  {
    ButtonVal = analogRead(analogpin);
    if(ButtonVal == current_state && counter >0)
    { 
      counter--;
    }
    if(ButtonVal != current_state)
    {
      counter++;
    }
    if (counter >= debounce_count)
    {
      counter = 0;
      current_state = ButtonVal;
      if (ButtonVal > 0)
      {
        ButtonCheck();
      }
    }
    time = millis();
  }
	serialHandler.Process();
	if(CountRefresh >= ScreenRefreshRate)
	{
		CountRefresh=0;
		uiManager.Draw();		
	}
	else
	{
		CountRefresh+=RealRefreshRate;
	}
	delay(RealRefreshRate);
}

void ButtonCheck()
{
  // loop for scanning the button array.
  for(int i = 0; i <= 21; i++)
  {
    // checks the ButtonVal against the high and low vales in the array
    if(ButtonVal >= Button[i][j] && ButtonVal <= Button[i][j+1])
    {
      // stores the button number to a variable
      label = Button[i][0];
      Action();      
    }
  }
}

void Action()
{
  if(label == 1)
  {
	uiManager.selectorUp();
  }
  if(label == 2)
  {
	uiManager.selectorDown();
  }
  if(label == 3)
  { 
	uiManager.selectorLeft();
  }
  if(label== 4)
  {
	uiManager.selectorRight();
  }
  if(label == 5)
  {
	uiManager.selectorApply();
  }
  
  
}
