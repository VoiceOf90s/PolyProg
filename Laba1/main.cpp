#include <stdio.h>
#include <math.h>
#include <locale.h>

#define YEARS 30
#define MONTHS_IN_YEAR 12
#define HOUSE_COST 13000000

typedef struct {
    double capital;           // �������
    double salary;            // ��������
    double monthlyExpenses;   // �������� ������� �� ���, ������������ � �.�.
    double rentOrMortgage;    // ������ ��� ������� (�������������)
    double inflationRate;     // ��������
    double depositRate;       // ������� �� ������� (����������)
    double monthlyRent;       // �������� ������ (��� ����)
    double mortgageRate;      // ��������� ������
    int hasHouse;             // ����, ���� ����� ��������
} Person;

// ������� ��� ���������� ����� ����� ���������� ���������
double applyInterest(double amount, double rate) {
    return amount * (1 + rate);
}

// ������������� ������� ������
void simulateMonth(Person* person, int isAlice, int month) {
    // �������� ������� ������������� � ��������� (���������� ���������� ������ �����)
    if (!isAlice || month > 0) {
        person->monthlyExpenses = applyInterest(person->monthlyExpenses, person->inflationRate / 12);
        if (!person->hasHouse) {
            person->monthlyRent = applyInterest(person->monthlyRent, person->inflationRate / 12);
        }
    }

    // ���������� �������� �� ���� ������ ��� ������� � ������ ��������
    if (isAlice) {
        person->capital -= person->rentOrMortgage + person->monthlyExpenses;
    }
    else {
        // ���� � ���� ��� ���� ��������, �� ������ �� ������ ������
        if (person->hasHouse) {
            person->capital -= person->monthlyExpenses;
        }
        else {
            person->capital -= person->monthlyRent + person->monthlyExpenses;
        }
    }

    // ���������� �������� (���������� �������� ���������� ���� ��� � ���)
    if (month%12==0) {
        person->salary = applyInterest(person->salary, person->inflationRate);
    }

    person->capital += person->salary;

    // ���������� ��������� �� ������� ��������
    person->capital = applyInterest(person->capital, person->depositRate / 12);
}

// ������������� �� ��������� ���
void simulateYears(Person* alice, Person* bob, int years) {
    setlocale(LC_ALL, "Rus");
    for (int year = 1; year <= years; year++) {
        for (int month = 0; month < MONTHS_IN_YEAR; month++) {
            simulateMonth(alice, 1, month);  // ��������� ��� �����
            simulateMonth(bob, 0, month);    // ��������� ��� ����

            // ��������, ������� �� ��� �� ��������
            if (!bob->hasHouse && bob->capital >= HOUSE_COST) {
                bob->capital -= HOUSE_COST;  // ��� �������� ��������
                bob->hasHouse = 1;           // ������ � ���� ���� ��������
            }
        }

        // ����� ���������� �� ������ ������� ����
        printf("��� %d-�:\n", year);
        printf("������� ����: %.2f\n", alice->capital);
        if (bob->hasHouse) {
            printf("������� ���� (����� ������� ��������): %.2f\n", bob->capital);
        }
        else {
            printf("������� ����: %.2f\n", bob->capital);
        }
        printf("\n");
    }
}

// ������� ��� ��������� ��������� � ����� ����������� ���������
void compareStrategies(Person alice, Person bob) {
    setlocale(LC_ALL, "Rus");
    // �������� ������� ����� � ��� ������� �� �������� ���� ��������� ��������
    double aliceFinalCapital = alice.capital + HOUSE_COST;

    // �������� ������� ���� � ��� ������� �� �������� ���� ��������� ��������, ���� �� � �����
    double bobFinalCapital = bob.capital;
    if (bob.hasHouse) {
        bobFinalCapital += HOUSE_COST;
    }

    printf("����� %d ���:\n", YEARS);
    printf("�������� ������� ����: %.2f\n", aliceFinalCapital);
    printf("�������� ������� ����: %.2f\n", bobFinalCapital);

    if (aliceFinalCapital > bobFinalCapital) {
        printf("A��������� ���� ����� ����������.\n");
    }
    else if (aliceFinalCapital < bobFinalCapital) {
        printf("��������� ���� ����� ����������.\n");
    }
    else {
        printf("��������� �����������.\n");
    }
}

int main() {
    // ������������� ��������� ������ ��� ����� (������� � �������)
    Person alice = {
        1000000,               // capital (��������� �������)
        200000,                // salary (��������)
        15000,                 // monthlyExpenses (�������� �������)
        13000000 * 0.16 / 12,  // rentOrMortgage (����������� ������ �������)
        0.09,                  // inflationRate (�������� 9%)
        0.20,                  // depositRate (������ �� �������� 20%)
        0,                     // monthlyRent (��� ������ ��� �����)
        0.16,                  // mortgageRate (��������� ������ 16%)
        1                      // hasHouse (����� ����� ������� ����� ����� �������)
    };

    // ������������� ��������� ������ ��� ���� (������� ����� � ����� �� �������)
    Person bob = {
        1000000,   // capital (��������� �������)
        200000,    // salary (��������)
        15000,     // monthlyExpenses (�������� �������)
        0,         // rentOrMortgage (��� ������� ��� ����)
        0.09,      // inflationRate (�������� 9%)
        0.20,      // depositRate (������ �� �������� 20%)
        30000,     // monthlyRent (������ 30,000 � �����)
        0,         // mortgageRate (��� ������� ��� ����)
        0          // hasHouse (���� ��� ������������, ��� �����)
    };

    simulateYears(&alice, &bob, YEARS);
    compareStrategies(alice, bob);

    return 0;
}
