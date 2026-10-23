#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int s=50,m=59,h=23,f;

void initADC(){
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
  ADMUX  |= (1<<REFS1)|(1<<REFS0);
  ADCSRA |= (1<<ADEN);
}

uint16_t read_adc(){
  ADMUX &= 0xF0;
  ADCSRA |= (1<<ADSC);
  while (ADCSRA & (1<<ADSC));
  return ADC;
}

void initTimer1(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1<<WGM12);                 
  TCCR1B |= (1<<CS12);                  
  OCR1A = 62499;                       
  TIMSK1 |= (1<<OCIE1A);                
  sei();
}

ISR(TIMER1_COMPA_vect){
  s++;
  f=1;
  if(s>=60){
  	m++;
    s=0;
  }
  if(m>=60){
    h++;
    m=0;
  }
  if(h>=24){
  	h=0;
  }
}

void setup(){
  initADC();
  initTimer1();
  lcd.begin(16,2);
}

void caractere2(int x){
  if(x<10){
    lcd.print('0');
  }
  lcd.print(x);
}

void loop(){
  if(f==1){
    f=0;
    /*
    nu am gasit lm35 in tinkercad
    (tmp36 are offset de 500mv fata de lm35,de aia ii -50)
    */
    int t = read_adc() * 110.0 / 1024.0 - 50.0;
    lcd.setCursor(0,0);
    lcd.print("Temp=");
    lcd.print(t);
    lcd.print("'C");
    lcd.setCursor(0,1);
    lcd.print("Ora ");
    caractere2(h);
    lcd.print(":");
    caractere2(m);
    lcd.print(":");
    caractere2(s);
  }
}