#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <string.h>

const int rs = A5, en = A4, d4 = A3, d5 = A2, d6 = A1, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte Rows= 4;
const byte Cols= 4;

char key[Rows][Cols] = 
{
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};

byte rowPins [Rows] = {9,8,7,6};
byte colPins [Cols] = {5,4,3,2};
Keypad keypad = Keypad( makeKeymap(key), rowPins, colPins, Rows,
Cols );
const char caracteresAceitos[11] = "1234567890";

Servo servo;

char senhaSalva[5];
char senhaDigitada[5];
int cursor ;
bool comSenha = false;
bool travado = false;
                       
void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  servo.attach(13);
  destravar();
}

void travar(){
  servo.write(90);
  travado = true;
  delay(1000);
}

void destravar(){
  servo.write(0);
  travado = false;
  delay(1000);
}

bool caracterAceito(char key){
  for (int i = 0; i < strlen(caracteresAceitos); i++){
    if(key == caracteresAceitos[i]){
      return true;
    }
  }
  return false;
}

void criarSenha(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CRIE UMA SENHA");
  lcd.setCursor(0, 1);
  lcd.print("SENHA: [----]");
  int index = 0;
  while(index < 4){
    char key = keypad.getKey();
    if(caracterAceito(key)){
      senhaSalva[index] = key;
      lcd.setCursor(index+8, 1);
      lcd.print("*");
      index++;
    }
  }
  comSenha = true;
  travar();
  
}

void verificaSenha(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TRAVADO");
  lcd.setCursor(0, 1);
  lcd.print("SENHA: [----]");
  int index = 0;
  while(index < 4){
    char key = keypad.getKey();
    if(caracterAceito(key)){
      senhaDigitada[index] = key;
      lcd.setCursor(index+8, 1);
      lcd.print("*");
      index++;
    }
  }
  delay(1000);
  if(!strcmp(senhaDigitada,senhaSalva)){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SENHA CORRETA");
    lcd.setCursor(0, 1);
    lcd.print("DESTRAVADO");
    destravar();
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SENHA INCORRETA");
    lcd.setCursor(0, 1);
    lcd.print("DIGITE NOVAMENTE");
    delay(1000);
  }
  delay(1000);
}

void resetarSenha(){
  comSenha = false;
}
                       
void loop(){
  char key = keypad.getKey();
  
  if (!comSenha){
    criarSenha();
  }
  else {
    if (travado){
      verificaSenha();
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("# - PARA TRAVAR");
      lcd.setCursor(0, 1);
      lcd.print("* - NOVA SENHA");
      char key = keypad.waitForKey();      
      if (key == '#'){
        travar();
      }
      else if (key == '*'){
        resetarSenha();
      }
    }  
  }
}
  
  
