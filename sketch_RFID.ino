//Software : RFID - Controle de Acesso leitor RFID
//Autor : Diego Santos Seabra

// Este software visa controlar a entrada e saída dos alunos da sala de aula, e o tempo que os mesmos permaneceram lá.

// UUID's Exitentes:

// Professor (Administrador do Sistema) -> 62 A6 32 00
// Estudante A -> D2 A5 AA D5
// Estudante B -> B6 AE 1B 2B
 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
// #include <Timer.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define OPAC 6

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria o MFRC522
 
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);  // Cria o LCD

//Timer t;
 
char st[20];
int hora;
int minuto;
int segundo;
bool b_Start;
int b_Stop;
 
void setup() 
{
  pinMode(OPAC,OUTPUT);
  analogWrite(OPAC,20);
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  b_Start = false;
  b_Stop = 0;
  hora = 0;
  minuto = 0;
  segundo = 0;
  //Define o número de colunas e linhas do LCD:  
  lcd.begin(16, 2);  
  mensageminicial();
}
 
void loop() 
{
  if (b_Stop != 2){
    if (b_Start == true){
      unsigned long previousMillis = millis();
      segundo++;
        if(segundo == 60){
            segundo = 0;
            minuto++;
          if(minuto == 60){
            minuto = 0;
            hora++;
          }
      }
      imprimeTempo(hora,minuto,segundo);
      unsigned long currentMillis = millis();
      unsigned long intervalo = currentMillis - previousMillis;
      unsigned long atraso = 1000 - intervalo;
      delay(atraso);
    }
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  String relogio= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "62 A6 32 00") //Professor Heber
  {
    Serial.println("Bem-Vindo Heber !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bem-Vindo Heber!");
    lcd.setCursor(0,1);
    lcd.print("ACESSO LIBERADO!");
    delay(3000);
    mensageminicial();
  }
 
  if (conteudo.substring(1) == "D2 A5 AA D5") //Estudante A
  {
    Serial.println("Ola Estudante A!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Estudante A!");
    lcd.setCursor(0,1);
    lcd.print(" ACESSO NEGADO!");
    delay(3000);
    mensageminicial();
  }

  if (conteudo.substring(1) == "B6 AE 1B 2B") //Estudante B
  {
      Serial.println("Ola Estudante B!");
      Serial.println();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ola Estudante B!");
      lcd.setCursor(0,1);
      lcd.print("  BEM - VINDO!");
      delay(3000);
      mensageminicial();
      b_Start = true;
      b_Stop++;
      if (b_Stop == 2){
        imprimeTempo(hora,minuto,segundo);  
        delay(3000);
      }
      if (b_Stop > 2){
        b_Start = false;
        lcd.setCursor(0,0);
        lcd.print("Ola Estudante B!");
        lcd.setCursor(0,1);
        lcd.print("AULA JA ASSIST.!");
        delay(3000);
        mensageminicial();
      }
  }
} 
 
void mensageminicial()
{
  lcd.clear();
  lcd.print(" Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor");  
}

void imprimeTempo(int h, int m, int s){
  lcd.clear();
  lcd.setCursor(10,0);
  lcd.print("Est.");
  lcd.setCursor(10,1);
  lcd.print("Est.");
  lcd.setCursor(15,0);
  lcd.print("A");
  lcd.setCursor(15,1);
  lcd.print("B");
  lcd.setCursor(0,0);
  imprimeHoraA(h);
  lcd.setCursor(0,1);
  imprimeHoraB(h);
  lcd.setCursor(2,0);
  lcd.print(":");
  lcd.setCursor(2,1);
  lcd.print(":");
  imprimeMinutoA(m);
  imprimeMinutoB(m);
  lcd.setCursor(5,0);
  lcd.print(":");
  lcd.setCursor(5,1);
  lcd.print(":");
  imprimeSegundoA(s);
  imprimeSegundoB(s);
}

void imprimeHoraA(int h){
  lcd.setCursor(0,0);
  if(h<10){
    lcd.print("0");
    lcd.setCursor(1,0);
  }
  lcd.print(h);
}

void imprimeHoraB(int h){
  lcd.setCursor(0,1);
  if(h<10){
    lcd.print("0");
    lcd.setCursor(1,1);
  }
  lcd.print(h);
}

void imprimeMinutoA(int m){
  lcd.setCursor(3,0);
  if(m<10){
    lcd.print("0");
    lcd.setCursor(4,0);
  }
  lcd.print(m);
}

void imprimeMinutoB(int m){
  lcd.setCursor(3,1);
  if(m<10){
    lcd.print("0");
    lcd.setCursor(4,1);
  }
  lcd.print(m);
}

void imprimeSegundoA(int s){
  lcd.setCursor(6,0);
  if(s<10){
    lcd.print("0");
    lcd.setCursor(7,0);
  }
  lcd.print(s);
}

void imprimeSegundoB(int s){
  lcd.setCursor(6,1);
  if(s<10){
    lcd.print("0");
    lcd.setCursor(7,1);
  }
  lcd.print(s);
}

void contagemRegressiva()
{
    unsigned long previousMillis = millis();
    segundo = segundo++;
    if(segundo == 60){
      segundo = 0;
      minuto = minuto++;
      if(minuto == 60){
        minuto = 0;
        hora = hora++;
      }
    }
    imprimeTempo(hora,minuto,segundo);
    unsigned long currentMillis = millis();
    unsigned long intervalo = currentMillis - previousMillis;
    unsigned long atraso = 1000 - intervalo;
    delay(atraso);
    //b_PrimeiraVez = false;
}
  


