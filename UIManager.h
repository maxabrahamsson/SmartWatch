#ifndef UIManager_h
#define UIManager_h
#include <LCD5110_Basic.h>
#include <Time.h>
#define FlashTime 500
#define TIME_MSG_LEN  11
#define TIME_HEADER  'T'
#define TIME_REQUEST  7 
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

#include "WProgram.h"

class Label
{
	public:
		String Text,SerialCode;
		int scrollOffset;	
		uint8_t* logo;
		Label* nextLabel;
		Label* prevLabel;
		bool ScrollText;
		Label(){}
		Label(String text,uint8_t* Logo,String sCode)
		{
			Text=text;
			nextLabel=NULL;
			prevLabel=NULL;
			scrollOffset=0;	
			ScrollText=false;
			logo=Logo;
			SerialCode=sCode;
		}
		Label(String text,uint8_t* Logo)
		{
			Text=text;
			nextLabel=NULL;
			prevLabel=NULL;
			scrollOffset=0;	
			ScrollText=false;
			logo=Logo;
			SerialCode=NULL;
		}
		Label(String text)
		{
			Text=text;
			nextLabel=NULL;
			prevLabel=NULL;
			scrollOffset=0;	
			ScrollText=false;
			logo=NULL;
			SerialCode=NULL;
		}
		Label(String text,String sCode)
		{
			Text=text;
			nextLabel=NULL;
			prevLabel=NULL;
			scrollOffset=0;	
			ScrollText=false;
			logo=NULL;
			SerialCode=sCode;
		}
		char* getChar()
		{
			char charBuf[13];
			if(ScrollText)
			{
				
				Text.substring(scrollOffset,scrollOffset+13).toCharArray(charBuf, 13);
				scrollOffset=scrollOffset<Text.length()?scrollOffset+1:0;
			}
			else
			{
				Text.toCharArray(charBuf,13);
			}
			return charBuf;
		}
};

class Screen
{
	private:
		
		Label* FirstEl;
		Label* LastEl;
		Label* SelectedEl;
		String Title;
		short verticalSelection,horizontalSelection,elCount;
		int RefreshRate;
	public:
		bool WatchScreen;
		Screen* nextScreen;
		Screen* prevScreen;
		Screen(String title,int rate)
		{
			Title=title;
			RefreshRate=rate;
		}
		void selectorDown()
		{
			SelectedEl=SelectedEl->nextLabel!=NULL?SelectedEl->nextLabel:FirstEl;		
		}
		void selectorUp()
		{
			SelectedEl=SelectedEl->prevLabel!=NULL?SelectedEl->prevLabel:LastEl;		
		}
		void selectorApply()
		{
			if(SelectedEl->SerialCode!=NULL)
			{
				Serial.print("<");
				Serial.print(SelectedEl->SerialCode);
				Serial.print(">");
			}
		}
		void Draw(LCD5110* lcd)
		{
			if(WatchScreen)
			{
			  lcd->setFont(BigNumbers);
			  lcd->printNumI(hour(), 0, 24);
			  lcd->printNumI(minute(), 33, 24);
			  lcd->setFont(MediumNumbers);
			  lcd->printNumI(second(), RIGHT, 24);
			  lcd->setFont(SmallFont);
			  lcd->print(":", 28, 34);
			  lcd->print("Clock", CENTER, 0); 
			  lcd->printNumI(day(), 12, 16);
			  lcd->print(":", 24, 16);
			  lcd->printNumI(month(), 30, 16);
			  lcd->print(":", 44, 16);
			  lcd->printNumI(year(), 50, 16);			
			}
			else
			{
				char charBuf[13];
				Title.toCharArray(charBuf,13);
				lcd->print(charBuf, CENTER, 0); 
				Label* CurEl=SelectedEl;
				short y=8;
				do
				{
					if(y>40)
					{
					break;
					}
					if(CurEl->ScrollText)
					{		
						CurEl->Text.substring(CurEl->scrollOffset,CurEl->scrollOffset+13).toCharArray(charBuf, 13);
						CurEl->scrollOffset=CurEl->scrollOffset<CurEl->Text.length()?CurEl->scrollOffset+1:0;
					}
					else
					{
						CurEl->Text.toCharArray(charBuf,13);
					}
					if(CurEl->logo!=NULL)
					{
						lcd->drawBitmap(0, y, CurEl->logo, 5, 8);
					}
					if(SelectedEl==CurEl)
					{
					  lcd->invertText(true);  
					  lcd->print(charBuf, 6,y);
					  lcd->invertText(false);				
					}
					else
					{
					  lcd->print(charBuf, 6,y);					
					}
					y+=8;
					CurEl=CurEl->nextLabel;
				}
				while(CurEl!=NULL);
			}
		}
		void AddLabel(Label* temp)
		{
			if(elCount==0)
			{
				FirstEl=temp;
				SelectedEl=FirstEl;
			}
			else
			{
				LastEl->nextLabel=temp;
				temp->prevLabel=LastEl;
			}
			LastEl=temp;
			elCount++;		
		}
};
class UIManager
{
	public:
		Screen* FirstScreen;
		Screen* LastScreen;
		Screen* CurrentScreen;
		short screenCount;
		LCD5110* lcd;
		String alertTitle;
		String alertContent;
		short alertResponse;
		bool alertDisplaying;
		uint8_t* tickLogo;
		
		void Draw()
		{
			lcd->clrScr();
			if(alertDisplaying)
			{
				char charBuf[13];
				alertTitle.toCharArray(charBuf,13);
				lcd->print(charBuf,0,0);
				char charBufContent[50];
				alertContent.toCharArray(charBufContent,13);
				lcd->print(charBufContent,0,8);
				lcd->drawBitmap(0, 76, tickLogo, 8, 8);
				lcd->print("X",40,76);
			}
			else
			{
				CurrentScreen->Draw(lcd);
			}
		}
		void setTickLogo(uint8_t* Logo)
		{
			tickLogo=Logo;
		}
		UIManager(LCD5110* LCD)
		{
			lcd=LCD;
			FirstScreen=NULL;
			LastScreen=NULL;
			CurrentScreen=NULL;
			screenCount=0;
			alertResponse=0;
			alertDisplaying=false;
		}
		void AddScreen(Screen* temp)
		{
			if(screenCount==0)
			{
				FirstScreen=temp;
				CurrentScreen=temp;
			}
			else
			{
				LastScreen->nextScreen=temp;
				temp->prevScreen=LastScreen;
			}			
			LastScreen=temp;
			screenCount++;
		}
		void prevScreen()
		{
			CurrentScreen = CurrentScreen->prevScreen==NULL?LastScreen:CurrentScreen->prevScreen;
		}
		void nextScreen()
		{
			CurrentScreen = CurrentScreen->nextScreen==NULL?FirstScreen:CurrentScreen->nextScreen;
		}
		void selectorLeft()
		{
			prevScreen();
		}
		void selectorRight()
		{
			nextScreen();
		}
		void selectorUp()
		{
			CurrentScreen->selectorUp();
		}
		void selectorDown()
		{
			CurrentScreen->selectorDown();
		}
		void selectorApply()
		{
			CurrentScreen->selectorApply();
		}
};
class SerialHandler
{
  public:
	UIManager* uiManager;
    SerialHandler(UIManager* uiMan)
	{
		uiManager=uiMan;
	}
    void Process(){
		if(Serial.available()) 
		{
		  handleData();
		} 	
	}
  private:
    void handleData(){
		while(Serial.available() >  0){
			char c = Serial.read();
			switch(c)
			{
				case 'T':
					TimeSyncData();
				break;
				case 'B':
					DebugCommand();
				break;
				case 'A':
					ProcessAlert();
				break;
			}
		}	
	}
	void ProcessAlert()
	{
		char c = Serial.read();
		switch(c)
		{
			case 'C':
			//Call
			break;
			case 'E':
			//Email
			break;
			case 'S':
				String mesaj="";
				while(Serial.available()>0)
				{
					mesaj+=Serial.read();
				}
				uiManager->alertTitle="New SMS";
				uiManager->alertContent=mesaj;
				uiManager->alertDisplaying=true;
			break;
		}
	}
	void TimeSyncData(){
		time_t pctime = 0;
		for(int i=0; i < TIME_MSG_LEN -1; i++)
		{   
		char c = Serial.read();          
		if( c >= '0' && c <= '9')
		{   
		  pctime = (10 * pctime) + (c - '0') ;
		}
		}   
		setTime(pctime);	
	}
	void DebugCommand(){
		char komut=Serial.read();
		switch(komut)
		{
			case 'w':
				uiManager->selectorUp();
			break;
			case 's':
				uiManager->selectorDown();
			break;
			case 'a':
				uiManager->selectorLeft();
			break;
			case 'd':
				uiManager->selectorRight();;
			break;
			case 'q':
				uiManager->selectorApply();;
			break;
		}	
	}
};

#endif