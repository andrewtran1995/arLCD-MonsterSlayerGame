//
// Slay the monster holding EarthLCD under siege!
//
#include <ezLCDLib.h>
#include <cstdio>
#include <cstring>

ezLCD3 lcd;

// global variables
int state = 0, i, acc, scurvyTime = 0, bindTime = 0, divine = 0;
boolean crit = false, scurvy = false, bind = false;

// player's statistics
int pHealth = 100, pMana = 100, pDamage, hPots = 3, mPots = 3;

// monster's statistics
int mHealth = 200, mDamage;

void setup(){
  lcd.begin();
  lcd.cls(BLACK,WHITE);
  
  // text for buttons (choices)
  lcd.string(1,"Physical");
  lcd.string(2,"Magic");
  lcd.string(3,"Items");
  lcd.string(4,"Phone-a-Friend");
  
  // set up buttons
  lcd.theme(1, DARKVIOLET, LAVENDER, WHITE, VIOLET, WHITE, PURPLE, PURPLE, PURPLE, BLACK, 0);
  lcd.button(1,  0,170,160,35,1,0,10,1,1); // top-left
  lcd.button(2,160,170,160,35,1,0,10,1,2); // top-right
  lcd.button(3,  0,205,160,35,1,0,10,1,3); // bottom-left
  lcd.button(4,160,205,160,35,1,0,10,1,4); // bottom-right
  
  // set up status bars
  lcd.theme(2, DARKRED, TOMATO, WHITE, WHITE, WHITE, BLACK, RED, RED, BLACK, 0);
  lcd.theme(3, DARKBLUE, LIGHTBLUE, WHITE, WHITE, WHITE, BLACK, BLUE, BLUE, BLACK, 0);
  lcd.string(51," Health (Monster)");
  lcd.string(52," Health");
  lcd.string(53," Mana");
  lcd.gauge(11,  0,  0,320,20,1,mHealth,0,200,2,51); // monster's health
  lcd.gauge(12,  0,155,160,17,1,pHealth,0,100,2,52); // player's health
  lcd.gauge(13,160,155,160,17,1,  pMana,0,100,3,53); // player's mana
  
  // set up monster
  lcd.rect(170,25,140,125,1);
  
  // set up text box
  lcd.theme(4, GOLDENROD, LIGHTYELLOW, YELLOW, YELLOW, YELLOW, DARKGRAY, WHITE, GRAY, GRAY, 0);
  lcd.string(5,"Slay the monster \\nbesieging \\nEarthLCD!");
  lcd.staticText(5,10,25,140,125,5,4,5);

  lcd.color(YELLOW);
}

void loop(){
  lcd.wstack(0); // revise button selection
  /*if(lcd.currentInfo != 0){
    i = lcd.currentWidget;
    if(state == 0)
      buttonUpdate(i);
    else if (i == 4)
      buttonUpdate(0);
    else if(state != 0 && i < 4){
      update(playerChoice(i));
      delay(2000);
      update(monsterChoice());
      delay(2000);
      if(scurvy){
        update(91);
        delay(2000);
      }
      if(bind){
        update(92);
        delay(2000);
      }
      buttonUpdate(0);
    }    
  } while(lcd.wvalue(i) != 0);*/
  if(state == 0){
    for(i=1; i<=4; i++){
      if(lcd.wvalue(i) == 1){
        while(lcd.wvalue(i) == 1)
          lcd.wvalue(5,"Button press ...\\nstate==0");
        buttonUpdate(i);
        break;
      }
    }
  }
  else{
    for(i=1; i<4; i++){
      if(lcd.wvalue(i) == 1){
        while(lcd.wvalue(i) == 1)
          lcd.wvalue(5,"Button press ...\\nstate!=0");
        update(playerChoice(i));
        delay(2000);
        update(monsterChoice());
        delay(2000);
        if(scurvy){
          update(91);
          delay(2000);
        }
        if(bind){
          update(92);
          delay(2000);
        }
        buttonUpdate(0);
        lcd.wstack(2);
      }
    }
    if(lcd.wvalue(4) == 1){
      while(lcd.wvalue(4) == 1);
      buttonUpdate(0);
    }
  }
    
  if(mHealth <= 0){
    lcd.wvalue(5,"YOU WIN!");
  } while(mHealth <= 0);
  if(pHealth <= 0){
    lcd.wvalue(5,"GAME OVER");
  } while(pHealth <= 0);
}

void buttonUpdate(int newState){
  switch(newState){
    case 0:
      lcd.string(1,"Physical");      lcd.string(2,"Magic");
      lcd.string(3,"Items");         lcd.string(4,"Phone-A-Friend");
      state = 0;
      break;
    case 1:
      lcd.string(1,"Strike");        lcd.string(2,"Lunge");
      lcd.string(3,"Slap");          lcd.string(4,"Back");
      state = 1;
      break;
    case 2:
      lcd.string(1,"C Extraction");  lcd.string(2,"Bind");
      lcd.string(3,"???");           lcd.string(4,"Back");
      state = 2;
      break;
    case 3:
      lcd.string(1,"Health Potion"); lcd.string(2,"Mana Potion");
      lcd.string(3,"Random Item");   lcd.string(4,"Back");
      state = 3;
      break;
    case 4:
      update(40);
  }
  lcd.wstate(1,3);
  lcd.wstate(2,3);
  lcd.wstate(3,3);
  lcd.wstate(4,3);
}

int playerChoice(int button){
  // determine accuracy
  lcd.wvalue(5,"playerChoice");
  randomSeed(analogRead(0));
  if(bind)
    acc = random(101) + 20;
  else
    acc = random(101);
  if(divine && acc > (100 - divine)){
    pDamage = random(100,201);
    return 93;
  }
  // determine outcome
  switch(state){
    case 1:
      if(button == 1){ // strike
        if(acc >= 20)
          pDamage = random(20,36);
        else
          pDamage = 0;
        if(acc >= 85){
          pDamage = 2*pDamage;
          crit = true;
        } return 11;
      }
      else if(button == 2){ // lunge
        if(acc >= 45)
          pDamage = random(10,71);
        else
          pDamage = 0;
        if(acc >= 90){
          pDamage = 2*pDamage;
          crit = true;
        } return 12;
      }
      else if(button == 3){ // slap
        if(acc >= 10)
          pDamage = random(10);
        else
          pDamage = 0;
        if(acc >= 90)
          crit = true;
        return 13;
      }
      else
        buttonUpdate(0);
      break;
    case 2:
      if(button == 1){ // c extraction
        if((pMana - 40) > 0){
          scurvy = true;
          scurvyTime = 5;
        }
        return 21;
      }
      else if(button == 2){ // bind
        if((pMana - 20) > 0){
          bind = true;
          bindTime = 3;
        }
        return 22;
      }
      else if(button == 3){ // ???
        if((pMana - 5) > 0)
          divine += 5;
        return 23;  
      }
      else
        buttonUpdate(0);
      break;
    case 3:
      if(button == 1){
        pDamage = random(10,40);
        return 31;
      }
      else if(button == 2){
        pDamage = random(10,40);
        return 32;
      }
      else if(button == 3){
      }
      else
        buttonUpdate(0);
      break;
  }
}

int monsterChoice(){
  lcd.wvalue(5,"monsterChoice");
  randomSeed(analogRead(0));
  acc = random(101);
  if(acc > 20)
    mDamage = random(5,31);
  else
    mDamage = 0;
  return 51;
}

void update(int choice){
  lcd.wvalue(5,"update");
  switch(choice){
    case 11:
    case 12:
    case 13:
      if(pDamage > 0){
        if(crit){
          textString(F("You hit the \\nmonster for \\n"),pDamage,F("\\ncritical damage!"));
          crit = false;
        } else
          textString(F("You hit the \\nmonster for \\n"),pDamage,F("\\ndamage!"));
        mHealth = mHealth - pDamage;
        lcd.wvalue(11,mHealth);
      }
      else
        lcd.wvalue(5,"You missed!");
      break;
    case 21:
      if(scurvy){
        lcd.wvalue(5,(char*)F("You extract\\nthe monster's\\nvitamin C!\\n(-40 mana)"));
        pMana -= 40;
        lcd.wvalue(13,pMana);
      } break;
    case 22:
      if(bind){
        lcd.wvalue(5,(char*)F("You bind the\\nmonster,\\nreducing\\nit's mobility!\\n(-20 mana)"));
        pMana -= 20;
        lcd.wvalue(13,pMana);
      } break;
    case 23:
      lcd.wvalue(5,(char*)F("... what are\\nyou doing?\\n(-5 mana)"));
      pMana -= 5;
      lcd.wvalue(13,pMana);
      break;
    case 31:
      hPots--;
      if(hPots > 0){
        textString(F("You heal\\nyourself for\\n"),pDamage,F("\\nhealth points!"));
        pHealth = pHealth + pDamage;
        lcd.wvalue(12,pHealth);
        delay(2000);
        textString(F("You have\\n"),hPots,F("\\npotions left."));
      } else
        lcd.wvalue(5,(char*)F("You have no\\nmore potions!"));
      break;
    case 32:
      mPots--;
      if(mPots > 0){
        textString(F("You recover\\n"),pDamage,F("\\nmana points!"));
        pMana = pMana + pDamage;
        lcd.wvalue(13,pMana);
        delay(2000);
        textString(F("You have\\n"),mPots,F("\\npotions left."));
      } else
        lcd.wvalue(5,(char*)F("You have no\\nmore potions!"));
      break;
    case 40:
      lcd.wvalue(5,(char*)F("You have\\nno friends ..."));
      break;
    case 51:
      if(mDamage > 0)
        textString(F("The monster \\nhit for \\n"),mDamage,F("\\ndamage!"));
      else
        lcd.wvalue(5,(char*)F("The monster \\nmissed!"));
      pHealth = pHealth - mDamage;
      lcd.wvalue(12,pHealth);
      break;
    case 91:
      if(--scurvyTime != 0){
        textString(F("The monster\\nsuffers\\n"),pDamage = random(5,15),F("\\nscurvy damage!"));
        mHealth = mHealth - pDamage;
        lcd.wvalue(11,mHealth);
      }
      else
        lcd.wvalue(5,(char*)F("The monster\\nate an\\norange!"));
      lcd.wvalue(11,mHealth);
      break;
    case 92:
      if(--bindTime != 0)
        textString(F("The monster is\\nbound for\\n"),bindTime,F("\\nmore turns!"));
      else{
        lcd.wvalue(5,(char*)F("The monster\\nbroke from\\nthe bind!"));
        bind = false;
      }
      break;
    case 93:
      textString(F("Meteors rain \\non the \\nmonster for\\n"),pDamage,F("\\ndamage!"));
      mHealth = mHealth - pDamage;
      lcd.wvalue(11,mHealth);
      break;
  }
  lcd.wstate(11,3);
  lcd.wstate(12,3);
  lcd.wstate(13,3);
}

void textString(char str1[30], int x, char str2[30]){
  lcd.wvalue(5,"");
  char number[4];
  sprintf(number,"%d",x);
  strcat(str1,number);
  strcat(str1,str2);
  lcd.wvalue(5,str1);
}
void textString(const __FlashStringHelper* str1, int x, const __FlashStringHelper* str2){
  lcd.wvalue(5,"");
  char number[4];
  sprintf(number,"%d",x);
  strcat((char*)str1,number);
  strcat((char*)str1,(char*)str2);
  lcd.wvalue(5,(char*)str1);
}
