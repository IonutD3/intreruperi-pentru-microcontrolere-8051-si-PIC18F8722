# Sistem de întreruperi pentru microcontrolere 8051 și PIC18F8722

---

# 🇷🇴 Română

## Descriere

Acest proiect este o colecție de implementări practice în **Embedded C** pentru gestionarea **întreruperilor hardware** pe microcontrolere 8051 și PIC18F8722.

Implementările sunt organizate progresiv, de la gestionarea unei singure întreruperi externe până la o configurație PIC18F8722 care combină:

- patru întreruperi externe (`INT0`–`INT3`);
- trei timere (`Timer0`, `Timer1`, `Timer2`);
- priorități HIGH și LOW;
- rutine de tratare separate;
- contoare software pentru evenimente generate de timere.

Ieșirile sunt conectate la LED-uri pe portul `D` al PIC18F8722, respectiv portul `P1` al 8051, iar comportamentul poate fi urmărit în simulările Proteus incluse în proiect.

---

## Structura proiectului

```text
intreruperi-pentru-microcontrolere-8051-si-PIC18F8722/
│
├── README.md
│
├── 8051/
│   └── 01-timer-and-external-interrupts/
│       ├── proteus/
│       │   └── project.pdsprj
│       └── src/
│           └── main.c
│
└── pic18f8722/
    ├── 01-external-int0/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    ├── 02-multiple-external-interrupts/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    ├── 03-interrupt-priority/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    ├── 04-interrupt-handlers/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    └── 05-advanced-interrupts/
        ├── proteus/
        │   └── project.pdsprj
        └── src/
            └── main.c
```

---

# 8051

## `01-timer-and-external-interrupts`

Implementarea utilizează simultan două timere și o întrerupere externă.

### Configurație

```c
TMOD = 0x11;
TR0 = 1;
TR1 = 1;
IT0 = 1;
PT0 = 0;
PT1 = 1;
IE = 0x8b;
```

Sunt configurate:

- **Timer 0** → întrerupere `interrupt 1`;
- **Timer 1** → întrerupere `interrupt 3`;
- **INT0** → întrerupere externă `interrupt 0`.

Timer 0 și Timer 1 sunt configurate cu aceeași valoare inițială:

```c
TH0 = 0xFC;
TL0 = 0x18;

TH1 = 0xFC;
TL1 = 0x18;
```

Frecvența cu care sunt comutate LED-urile este controlată prin contoare software:

```c
counter++;
if (counter == 1000) {
    LED = ~LED;
    counter = 0;
}
```

și:

```c
counter1++;
if (counter1 == 500) {
    LED1 = ~LED1;
    counter1 = 0;
}
```

`INT0` declanșează o secvență separată care comută `LED2` de 20 de ori, folosind o funcție de delay.

### Maparea ieșirilor

| Ieșire | Pin |
|---|---|
| `LED` | `P1^0` |
| `LED1` | `P1^1` |
| `LED2` | `P1^2` |

---

# PIC18F8722

## `01-external-int0`

Prima implementare PIC18F8722 utilizează o singură sursă externă de întrerupere: **INT0**.

Configurarea principală este:

```c
GIE = 1;
INT0IE = 1;
INT0IF = 0;
INTEDG0 = 1;
```

La setarea flag-ului `INT0IF`, rutina:

```c
void interrupt ext0()
```

comută `RD0` de 20 de ori.

Fiecare schimbare de stare este urmată de o întârziere realizată cu:

```c
__delay_ms(5);
```

Bucla principală nu efectuează polling:

```c
while (1) {}
```

Evenimentul este procesat exclusiv prin rutina de întrerupere.

### Maparea ieșirii

```text
INT0 → RD0
```

---

## `02-multiple-external-interrupts`

Implementarea extinde configurația la două surse externe:

```text
INT1
INT2
```

Ambele sunt activate și configurate pentru detecția frontului crescător:

```c
INT2IE = 1;
INT1IE = 1;

INT2IF = 0;
INT1IF = 0;

INTEDG2 = 1;
INTEDG1 = 1;
```

Rutina:

```c
void interrupt ext2()
```

verifică separat cele două flag-uri.

### Comportament

| Interrupt | Ieșire |
|---|---|
| `INT1IF` | `RD0` |
| `INT2IF` | `RD1` |

Fiecare eveniment comută LED-ul asociat de două ori.

Flag-urile sunt resetate la finalul rutinei:

```c
INT1IF = 0;
INT2IF = 0;
```

---

## `03-interrupt-priority`

Această implementare introduce mecanismul de **interrupt priority** al PIC18F8722.

Sunt utilizate două niveluri:

```text
HIGH priority
LOW priority
```

Configurația este:

```c
IPEN = 1;

INT1IP = 1;
INT2IP = 0;
```

Astfel:

```text
INT1 → HIGH priority
INT2 → LOW priority
```

Sunt utilizate două rutine ISR distincte:

```c
void interrupt high_priority ext1()
```

și:

```c
void interrupt low_priority ext2()
```

### Comportament

| Interrupt | Prioritate | Ieșire |
|---|---|---|
| `INT1` | HIGH | `RD0` |
| `INT2` | LOW | `RD1` |

Ambele rutine comută LED-ul asociat de șase ori.

Activarea globală a celor două niveluri se realizează prin:

```c
GIEH = 1;
GIEL = 1;
```

---

## `04-interrupt-handlers`

Implementarea separă logica fiecărei surse de întrerupere în **handler-e dedicate**.

Sunt definite:

```c
void ext0();
void ext1();
void ext2();
void ext3();
```

Rutina HIGH priority gestionează:

```text
INT0 → ext0()
INT1 → ext1()
```

iar rutina LOW priority:

```text
INT2 → ext2()
INT3 → ext3()
```

Structura ISR este:

```c
void interrupt high_priority ex1()
{
    if (INT0IF) {
        ext0();
    }

    if (INT1IF) {
        ext1();
    }
}
```

și:

```c
void interrupt low_priority ex2()
{
    if (INT2IF) {
        ext2();
    }

    if (INT3IF) {
        ext3();
    }
}
```

### Maparea întreruperilor

| Interrupt | Prioritate | Handler | Ieșire |
|---|---|---|---|
| `INT0` | HIGH | `ext0()` | `RD0` |
| `INT1` | HIGH | `ext1()` | `RD1` |
| `INT2` | LOW | `ext2()` | `RD2` |
| `INT3` | LOW | `ext3()` | `RD3` |

Fiecare handler comută ieșirea asociată de șase ori și resetează flag-ul întreruperii.

---

## `05-advanced-interrupts`

Ultima implementare combină întreruperi externe și întreruperi generate de timere în aceeași arhitectură.

Sunt utilizate șapte surse de întrerupere:

### HIGH priority

```text
INT0
INT1
Timer0
Timer2
```

### LOW priority

```text
INT2
INT3
Timer1
```

Distribuția este implementată prin două ISR-uri:

```c
void interrupt high_priority ex1()
```

și:

```c
void interrupt low_priority ex2()
```

Rutina HIGH verifică:

```c
INT0IF
INT1IF
TMR0IF
TMR2IF
```

iar rutina LOW verifică:

```c
INT2IF
INT3IF
TMR1IF
```

---

### Handlere pentru întreruperi externe

Handler-ele `ext0()`–`ext3()` controlează primele patru ieșiri:

| Sursă | Handler | Ieșire |
|---|---|---|
| `INT0` | `ext0()` | `RD0` |
| `INT1` | `ext1()` | `RD1` |
| `INT2` | `ext2()` | `RD2` |
| `INT3` | `ext3()` | `RD3` |

Fiecare handler execută șase comutări ale LED-ului și apoi resetează flag-ul corespunzător.

---

### Timer0

Timer0 este configurat ca timer intern, pe 8 biți:

```c
T08BIT = 1;
T0CS = 0;
PSA = 0;

T0PS2 = 1;
T0PS1 = 0;
T0PS0 = 1;
```

Întreruperea este activată și asignată nivelului HIGH:

```c
TMR0IE = 1;
TMR0IF = 0;
TMR0IP = 1;
```

Handler-ul utilizează un contor software:

```c
counter++;
if (counter == 30) {
    RD4 = ~RD4;
    counter = 0;
}
```

Astfel, `RD4` este comutat după 30 de evenimente Timer0.

---

### Timer1

Timer1 este configurat ca timer intern:

```c
T1CONbits.RD16 = 0;
T1CKPS1 = 0;
T1CKPS0 = 1;
TMR1CS = 0;
```

Întreruperea este configurată ca LOW priority:

```c
TMR1IE = 1;
TMR1IF = 0;
TMR1IP = 0;
```

Handler-ul folosește `counter1`:

```c
counter1++;

if (counter1 == 2) {
    RD5 = ~RD5;
    counter1 = 0;
}
```

---

### Timer2

Timer2 utilizează prescaler-ul configurat prin:

```c
T2CKPS1 = 0;
T2CKPS0 = 0;
```

Perioada este stabilită prin:

```c
PR2 = 255;
```

Întreruperea Timer2 este HIGH priority:

```c
TMR2IE = 1;
TMR2IF = 0;
TMR2IP = 1;
```

Handler-ul utilizează un al treilea contor software:

```c
counter2++;

if (counter2 == 3921) {
    RD6 = ~RD6;
    counter2 = 0;
}
```

---

## Maparea finală a sistemului

În configurația avansată, ieșirile sunt împărțite între evenimente externe și evenimente generate de timere:

| Sursă | Prioritate | Handler | Ieșire |
|---|---|---|---|
| `INT0` | HIGH | `ext0()` | `RD0` |
| `INT1` | HIGH | `ext1()` | `RD1` |
| `INT2` | LOW | `ext2()` | `RD2` |
| `INT3` | LOW | `ext3()` | `RD3` |
| `Timer0` | HIGH | `ext4()` | `RD4` |
| `Timer1` | LOW | `ext5()` | `RD5` |
| `Timer2` | HIGH | `ext6()` | `RD6` |

---

# Arhitectura de întreruperi

Implementarea PIC18F8722 folosește o structură bazată pe două niveluri de întrerupere:

```text
                    ┌──────────────────┐
                    │   PIC18F8722     │
                    └────────┬─────────┘
                             │
              ┌──────────────┴──────────────┐
              │                             │
       HIGH PRIORITY                  LOW PRIORITY
              │                             │
       ┌──────┼──────┐               ┌──────┼──────┐
       │      │      │               │      │      │
      INT0   INT1   TMR0            INT2   INT3   TMR1
                     │
                    TMR2
       │      │      │               │      │      │
      RD0    RD1    RD4             RD2    RD3    RD5
                             │
                            RD6
```

Bucla principală nu conține logică de polling:

```c
while (1) {}
```

Controlul ieșirilor este realizat prin rutinele de întrerupere și handler-ele asociate fiecărei surse.

---

# Instrumente și tehnologii

- **C**
- **8051**
- **PIC18F8722**
- **MPLAB XC8**
- **Keil**
- **Proteus**
- **Hardware Interrupts**
- **Timer Interrupts**
- **Interrupt Priority**
- **Embedded Systems**

---

# 🇬🇧 English

## Overview

This project is a collection of practical **Embedded C** implementations for **hardware interrupt handling** on 8051 and PIC18F8722 microcontrollers.

The implementations progress from a single external interrupt to a PIC18F8722 configuration combining:

- four external interrupts (`INT0`–`INT3`);
- three timers (`Timer0`, `Timer1`, `Timer2`);
- HIGH and LOW interrupt priorities;
- dedicated interrupt handlers;
- software counters for timer-driven events.

The outputs are connected to LEDs on the `D` port of the PIC18F8722 and the `P1` port of the 8051. Each implementation includes a corresponding Proteus simulation project.

---

## Project structure

```text
intreruperi-pentru-microcontrolere-8051-si-PIC18F8722/
│
├── README.md
│
├── 8051/
│   └── 01-timer-and-external-interrupts/
│       ├── proteus/
│       │   └── project.pdsprj
│       └── src/
│           └── main.c
│
└── pic18f8722/
    ├── 01-external-int0/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    ├── 02-multiple-external-interrupts/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    ├── 03-interrupt-priority/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    ├── 04-interrupt-handlers/
    │   ├── proteus/
    │   │   └── project.pdsprj
    │   └── src/
    │       └── main.c
    │
    └── 05-advanced-interrupts/
        ├── proteus/
        │   └── project.pdsprj
        └── src/
            └── main.c
```

---

## 8051 Implementation

### `01-timer-and-external-interrupts`

The 8051 implementation combines two timer interrupts with an external interrupt.

The configuration enables:

- **Timer 0** → `interrupt 1`;
- **Timer 1** → `interrupt 3`;
- **INT0** → `interrupt 0`.

Timer 0 and Timer 1 reload the same initial value:

```c
TH0 = 0xFC;
TL0 = 0x18;

TH1 = 0xFC;
TL1 = 0x18;
```

Software counters determine when the corresponding LEDs are toggled.

```text
Timer 0 → counter → P1.0
Timer 1 → counter1 → P1.1
INT0    → external event → P1.2
```

The external interrupt triggers a separate LED sequence implemented using a delay function.

---

# PIC18F8722 Implementations

## `01-external-int0`

The first PIC18F8722 implementation handles a single external interrupt:

```text
INT0 → RD0
```

The interrupt is enabled with:

```c
GIE = 1;
INT0IE = 1;
INT0IF = 0;
INTEDG0 = 1;
```

When `INT0IF` is set, the ISR toggles `RD0` 20 times.

The main loop contains no polling logic:

```c
while (1) {}
```

---

## `02-multiple-external-interrupts`

This implementation introduces two external interrupt sources:

```text
INT1 → RD0
INT2 → RD1
```

Both interrupts are enabled and configured for rising-edge detection.

A single ISR checks both interrupt flags and executes the corresponding output operation.

---

## `03-interrupt-priority`

This implementation introduces the PIC18F8722 interrupt priority mechanism.

```text
INT1 → HIGH priority → RD0
INT2 → LOW priority  → RD1
```

Interrupt priority is enabled through:

```c
IPEN = 1;
GIEH = 1;
GIEL = 1;
```

The two sources are handled by separate high- and low-priority ISRs.

---

## `04-interrupt-handlers`

The interrupt logic is separated into dedicated handler functions:

```text
INT0 → ext0() → RD0
INT1 → ext1() → RD1
INT2 → ext2() → RD2
INT3 → ext3() → RD3
```

The high-priority ISR dispatches `INT0` and `INT1`, while the low-priority ISR dispatches `INT2` and `INT3`.

This separates interrupt dispatching from event-specific output handling.

---

## `05-advanced-interrupts`

The final implementation combines external interrupts and timer interrupts using the PIC18F8722 high/low priority system.

### HIGH priority

```text
INT0
INT1
Timer0
Timer2
```

### LOW priority

```text
INT2
INT3
Timer1
```

The corresponding outputs are:

```text
INT0  → RD0
INT1  → RD1
INT2  → RD2
INT3  → RD3
Timer0 → RD4
Timer1 → RD5
Timer2 → RD6
```

### Timer0

Timer0 operates as an 8-bit internal timer with a software counter:

```c
counter++;

if (counter == 30) {
    RD4 = ~RD4;
    counter = 0;
}
```

Timer0 is assigned to HIGH priority.

### Timer1

Timer1 operates as an internal timer and is assigned to LOW priority.

Its software counter controls `RD5`:

```c
counter1++;

if (counter1 == 2) {
    RD5 = ~RD5;
    counter1 = 0;
}
```

### Timer2

Timer2 uses `PR2 = 255` as its period register and is assigned to HIGH priority.

Its software counter controls `RD6`:

```c
counter2++;

if (counter2 == 3921) {
    RD6 = ~RD6;
    counter2 = 0;
}
```

---

## Interrupt Mapping

| Source | Priority | Handler | Output |
|---|---|---|---|
| `INT0` | HIGH | `ext0()` | `RD0` |
| `INT1` | HIGH | `ext1()` | `RD1` |
| `INT2` | LOW | `ext2()` | `RD2` |
| `INT3` | LOW | `ext3()` | `RD3` |
| `Timer0` | HIGH | `ext4()` | `RD4` |
| `Timer1` | LOW | `ext5()` | `RD5` |
| `Timer2` | HIGH | `ext6()` | `RD6` |

---

# Interrupt Architecture

The PIC18F8722 implementation uses a two-level interrupt structure:

```text
                    ┌──────────────────┐
                    │   PIC18F8722     │
                    └────────┬─────────┘
                             │
              ┌──────────────┴──────────────┐
              │                             │
       HIGH PRIORITY                  LOW PRIORITY
              │                             │
       ┌──────┼──────┐               ┌──────┼──────┐
       │      │      │               │      │      │
      INT0   INT1   TMR0            INT2   INT3   TMR1
                     │
                    TMR2
       │      │      │               │      │      │
      RD0    RD1    RD4             RD2    RD3    RD5
                             │
                            RD6
```

The main loop remains intentionally empty:

```c
while (1) {}
```

Event handling is performed by the interrupt subsystem rather than by polling input states.

---

## Tools & Technologies

- **C**
- **8051**
- **PIC18F8722**
- **MPLAB XC8**
- **Keil**
- **Proteus**
- **External Hardware Interrupts**
- **Timer Interrupts**
- **Interrupt Priority Handling**
- **Interrupt Service Routines (ISRs)**
- **Embedded Systems**

---

## 👤 Autor / Author

**IonutD**
