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
 
#define SS_PIN 10
#define RST_PIN 9
#define OPAC 6

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria o MFRC522
 
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);  // Cria o LCD
 
char st[20];
int horaA;
int minutoA;
int segundoA;
int horaB;
int minutoB;
int segundoB;
bool b_StartA;
int b_StopA;
bool b_StartB;
int b_StopB;
int c_Prof;
bool b_Aula;
String v_Prof;
 
void setup() 
{
  pinMode(OPAC,OUTPUT);
  analogWrite(OPAC,20);
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  v_Prof = "Heber";
  b_StartA = false;
  b_StopA = 0;
  b_StartB = false;
  b_StopB = 0;
  horaA = 0;
  minutoA = 0;
  segundoA = 0;
  horaB = 0;
  minutoB = 0;
  segundoB = 0;
  c_Prof = 0;
  b_Aula = false;
  //Define o número de colunas e linhas do LCD:  
  lcd.begin(16, 2);  
  mensageminicial();
}
 
void loop() 
{
  if (b_StopA != 2){
    if (b_StartA == true){
      unsigned long previousMillis = millis();
      segundoA++;
      if(segundoA == 60){
            segundoA = 0;
            minutoA++;
          if(minutoA == 60){
            minutoA = 0;
            horaA++;
          }
      }
      imprimeTempoA(horaA,minutoA,segundoA);
      unsigned long currentMillis = millis();
      unsigned long intervalo = currentMillis - previousMillis;
      unsigned long atraso = 1000 - intervalo;
      delay(atraso);
    }
  }
  if (b_StopB != 2){
    if (b_StartB == true){
      unsigned long previousMillis = millis();
      segundoB++;
      if(segundoB == 60){
            segundoB = 0;
            minutoB++;
          if(minutoB == 60){
            minutoB = 0;
            horaB++;
          }
      }
      imprimeTempoB(horaB,minutoB,segundoB);
      unsigned long currentMillis = millis();
      unsigned long intervalo = currentMillis - previousMillis;
      unsigned long atraso = 1000 - intervalo;
      delay(atraso);
    }
  }
  // Procura novos cartões - além dos que estão cadastrados
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartões
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
    lcd.print("Bem-Vindo " + v_Prof + "!");
    lcd.setCursor(0,1);
    lcd.print("ACESSO LIBERADO!");
    delay(3000);
    ++c_Prof;
    if (c_Prof == 1){
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("AULA");
      lcd.setCursor(4,1);
      lcd.print("INICIADA");
      b_Aula = true;
      delay(3000);
    }
    if (c_Prof == 2){
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("AULA");
      lcd.setCursor(3,1);
      lcd.print("FINALIZADA");
      b_Aula = false;
      delay(3000);
    }
    if (c_Prof == 3){
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("AULAS");
      lcd.setCursor(3,1);
      lcd.print("RESETADAS");
      b_StartA = false;
      b_StopA = 0;
      b_StartB = false;
      b_StopB = 0;
      horaA = 0;
      minutoA = 0;
      segundoA = 0;
      horaB = 0;
      minutoB = 0;
      segundoB = 0;
      c_Prof = 0;
      b_Aula = false;
      delay(3000);
      mensageminicial();
    }
    mensageminicial();
  }
 
  if (conteudo.substring(1) == "D2 A5 AA D5") //Estudante A
  {
    if (b_Aula == true){
        Serial.println("Ola Estudante A!");
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ola Estudante A!");
        lcd.setCursor(0,1);
        lcd.print("  BEM - VINDO!");
        delay(3000);
        mensageminicial();
        b_StartA = true;
        ++b_StopA;
        if (b_StopA == 2){
          imprimePresencaA(horaA,minutoA,segundoA);
          delay(5000);
          mensageminicial();
        }
        if (b_StopA > 2){
          b_StartA = false;
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("AULA");
          lcd.setCursor(2,1);
          lcd.print("JA ASSISTIDA!");
          delay(3000);
          mensageminicial();
        }
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ola Estudante A!");
      lcd.setCursor(1,1);
      lcd.print("ACESSO NEGADO!");
      delay(3000); 
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("AULA");
      lcd.setCursor(2,1);
      lcd.print("NAO INICIADA!");
      delay(3000);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("PROCURE O SEU");
      lcd.setCursor(3,1);
      lcd.print("PROFESSOR!");
      delay(3000);
      mensageminicial();
    }
  }

  if (conteudo.substring(1) == "B6 AE 1B 2B") //Estudante B
  {
    if (b_Aula == true){
        Serial.println("Ola Estudante B!");
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ola Estudante B!");
        lcd.setCursor(0,1);
        lcd.print("  BEM - VINDO!");
        delay(3000);
        mensageminicial();
        b_StartB = true;
        ++b_StopB;
        if (b_StopB == 2){
          imprimePresencaB(horaB,minutoB,segundoB);
          delay(5000);
          mensageminicial();
        }
        if (b_StopB > 2){
          b_StartB = false;
          lcd.setCursor(0,0);
          lcd.print("Ola Estudante B!");
          lcd.setCursor(0,1);
          lcd.print("AULA JA ASSIST.!");
          delay(3000);
          mensageminicial();
        }
   }
   else
   {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ola Estudante B!");
      lcd.setCursor(1,1);
      lcd.print("ACESSO NEGADO!");
      delay(3000); 
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("AULA");
      lcd.setCursor(2,1);
      lcd.print("NAO INICIADA!");
      delay(3000);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("PROCURE O SEU");
      lcd.setCursor(3,1);
      lcd.print("PROFESSOR!");
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

void imprimeTempoA(int h, int m, int s){
  lcd.clear();
  
  // Tempo de A
  lcd.setCursor(10,0);
  lcd.print("Est.");
  lcd.setCursor(15,0);
  lcd.print("A");
  lcd.setCursor(0,0);
  imprimeHoraA(h);
  lcd.setCursor(2,0);
  lcd.print(":");
  imprimeMinutoA(m);
  lcd.setCursor(5,0);
  lcd.print(":");
  imprimeSegundoA(s);
}

void imprimeTempoB(int h, int m, int s){
  lcd.clear();
  
  // Tempo de B
  lcd.setCursor(10,1);
  lcd.print("Est.");
  lcd.setCursor(15,1);
  lcd.print("B");
  lcd.setCursor(0,1);
  imprimeHoraB(h);
  lcd.setCursor(2,1);
  lcd.print(":");
  imprimeMinutoB(m);
  lcd.setCursor(5,1);
  lcd.print(":");
  imprimeSegundoB(s);
}

void imprimePresencaA(int h, int m, int s){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("PRESENCA OK!");
  lcd.setCursor(1,1);
  lcd.print("TOTAL");
  lcd.setCursor(7,1);
  imprimeHoraPresencaA(h);
  lcd.setCursor(9,1);
  lcd.print(":");
  imprimeMinutoPresencaA(m);
  lcd.setCursor(12,1);
  lcd.print(":");
  imprimeSegundoPresencaA(s); 
}

void imprimePresencaB(int h, int m, int s){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("PRESENCA OK!");
  lcd.setCursor(1,1);
  lcd.print("TOTAL");
  lcd.setCursor(7,1);
  imprimeHoraPresencaB(h);
  lcd.setCursor(9,1);
  lcd.print(":");
  imprimeMinutoPresencaB(m);
  lcd.setCursor(12,1);
  lcd.print(":");
  imprimeSegundoPresencaB(s); 
}

void imprimeHoraA(int h){
  lcd.setCursor(0,0);
  if(h<10){
    lcd.print("0");
    lcd.setCursor(1,0);
  }
  lcd.print(h);
}

void imprimeHoraPresencaA(int h){
  lcd.setCursor(7,1);
  if(h<10){
    lcd.print("0");
    lcd.setCursor(8,1);
  }
  lcd.print(h);
}

void imprimeHoraPresencaB(int h){
  lcd.setCursor(7,1);
  if(h<10){
    lcd.print("0");
    lcd.setCursor(8,1);
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

void imprimeMinutoPresencaA(int m){
  lcd.setCursor(10,1);
  if(m<10){
    lcd.print("0");
    lcd.setCursor(11,1);
  }
  lcd.print(m);
}

void imprimeMinutoPresencaB(int m){
  lcd.setCursor(10,1);
  if(m<10){
    lcd.print("0");
    lcd.setCursor(11,1);
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

void imprimeSegundoPresencaA(int s){
  lcd.setCursor(13,1);
  if(s<10){
    lcd.print("0");
    lcd.setCursor(14,1);
  }
  lcd.print(s);
}

void imprimeSegundoPresencaB(int s){
  lcd.setCursor(13,1);
  if(s<10){
    lcd.print("0");
    lcd.setCursor(14,1);
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
  


