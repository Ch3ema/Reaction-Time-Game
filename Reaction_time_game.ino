#include <LiquidCrystal.h>

// Create LCD object: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(3, 5, A5, A4, A3, A2);

int led_1 = 2;
int button_1 = 4;

int led_2 = 6;
int button_2 = 7;

int led_3 = 9;
int power = 13;
int buzzer = 8;

unsigned long wait_time;
int previous_state_1 = HIGH;
int previous_state_2 = HIGH;

unsigned long reaction_time_1;
unsigned long reaction_time_2;

unsigned long press_time_1;
unsigned long press_time_2;

bool button_pressed_1 = false;
bool button_pressed_2 = false;

int total_rounds = 5;
int current_round = 1;

bool start_led_flash = false;
bool press_permission = false;

int score_player_1 = 0;
int score_player_2 = 0;

const int MIN_REACTION_TIME = 30;  // Ignore reaction times below this

void setup() {
  Serial.begin(9600);

  pinMode(button_1, INPUT_PULLUP);
  pinMode(led_1, OUTPUT);

  pinMode(button_2, INPUT_PULLUP);
  pinMode(led_2, OUTPUT);

  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);

  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  lcd.begin(16, 2);
}

void loop() {
  if (current_round <= total_rounds) {
    int current_state_1 = digitalRead(button_1);
    int current_state_2 = digitalRead(button_2);

    if (!start_led_flash) {
      delay(random(100, 150));

      pinMode(led_3, OUTPUT);
      pinMode(buzzer, OUTPUT);

      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzer, HIGH);
        digitalWrite(led_3, HIGH);
        delay(25);
        digitalWrite(led_3, LOW);
        digitalWrite(buzzer, LOW);
        delay(25);
      }

      wait_time = millis();
      press_permission = true;
      start_led_flash = true;
    }

    // Register player 1
    if (press_permission && !button_pressed_1 && previous_state_1 == HIGH && current_state_1 == LOW) {
      press_time_1 = millis();
      reaction_time_1 = press_time_1 - wait_time;
      if (reaction_time_1 >= MIN_REACTION_TIME) {
        lcd.setCursor(0, 0);
        lcd.print("P1:");
        lcd.print(reaction_time_1);
        button_pressed_1 = true;
      }
    }

    // Register player 2
    if (press_permission && !button_pressed_2 && previous_state_2 == HIGH && current_state_2 == LOW) {
      press_time_2 = millis();
      reaction_time_2 = press_time_2 - wait_time;
      if (reaction_time_2 >= MIN_REACTION_TIME) {
        lcd.setCursor(8, 0);
        lcd.print("P2:");
        lcd.print(reaction_time_2);
        button_pressed_2 = true;
      }
    }

    if (button_pressed_1 && button_pressed_2) {
      digitalWrite(led_1, LOW);
      digitalWrite(led_2, LOW);

      if (reaction_time_1 < reaction_time_2) {
        score_player_1++;
        lcd.setCursor(0, 1);
        lcd.print("Victory: P1");
        lcd.setCursor(12, 1);
        lcd.print(score_player_1);
        lcd.print("-");
        lcd.print(score_player_2);

        for (int i = 0; i < 6; i++) {
          digitalWrite(led_1, HIGH);
          digitalWrite(buzzer, HIGH);
          delay(75);
          digitalWrite(led_1, LOW);
          digitalWrite(buzzer, LOW);
          delay(75);
        }
      } else if (reaction_time_2 < reaction_time_1) {
        score_player_2++;
        lcd.setCursor(0, 1);
        lcd.print("Victory: P2");
        lcd.setCursor(12, 1);
        lcd.print(score_player_1);
        lcd.print("-");
        lcd.print(score_player_2);

        for (int i = 0; i < 6; i++) {
          digitalWrite(led_2, HIGH);
          digitalWrite(buzzer, HIGH);
          delay(75);
          digitalWrite(led_2, LOW);
          digitalWrite(buzzer, LOW);
          delay(75);
        }
      }

      if (current_round == total_rounds) {
        lcd.clear();
        lcd.setCursor(0, 0);

        if (score_player_1 > score_player_2) {
          lcd.print("Winner: P1");
          lcd.setCursor(0, 1);
          lcd.print("Score: ");
          lcd.print(score_player_1);
          lcd.print("-");
          lcd.print(score_player_2);
          delay(5000);
        } else if (score_player_2 > score_player_1) {
          lcd.print("Winner: P2");
          lcd.setCursor(0, 1);
          lcd.print("Score: ");
          lcd.print(score_player_2);
          lcd.print("-");
          lcd.print(score_player_1);
          delay(5000);
        } else {
          lcd.print("Draw");
        }
      }

      // Reset for next round
      reaction_time_1 = 0;
      reaction_time_2 = 0;
      press_time_1 = 0;
      press_time_2 = 0;
      button_pressed_1 = false;
      button_pressed_2 = false;
      previous_state_1 = HIGH;
      previous_state_2 = HIGH;
      press_permission = false;
      start_led_flash = false;
      delay(2100);
      lcd.clear();
      current_round++;
    }

    previous_state_1 = current_state_1;
    previous_state_2 = current_state_2;
  }
}
