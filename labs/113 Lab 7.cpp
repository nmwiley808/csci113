#include <iostream>
#include <string>
using namespace std;

// AND gate function
int andGate(int a, int b) {
    return a & b;
}

// OR gate function
int orGate(int a, int b) {
    return a | b;
}

// Full adder function
int fullAdder(int a, int b, int carryIn, int &carryOut) {
    int carryIN = 0;
    int sum = a ^ b ^ carryIN;
    carryOut = (a & b) | (carryIn & (a ^ b));
    return sum;
}

// 2x1 multiplexer function
int mux_2x1(int x1, int x2, int sel) {
    return (x1 & ~sel) | (x2 & sel);
}

// 4x1 multiplexer function
int mux_4x1(int x1, int x2, int x3, const string &sel) {
    if (sel == "00") return x1;
    if (sel == "01") return x2;
    if (sel == "10") return x3;
    return 0; // Default case
}

int mux_4x1(int x1, int x2, int x3, int sel, int * carry_out);

// 1bit ALU
int alu1bit(int a, int b, int bInv, int carryIn, const string &op, int &carryOut) {
    int bModified = b ^ bInv;
    andGate(a, b);
    orGate(a, b);
    int sumResult = fullAdder(a, bModified, carryIn, carryOut);
    return sumResult;
}

char selectOperation (const string& op) {
    if (op == "00") {
        return '&';
    }
    else if (op == "01") {
        return '|';
    }
    else if (op == "10") {
        return '-';
    }
    else return ' ';
}

// Main Function for 1-bit ALU
void aluDriverOutput (int a, int b, int carryIn, const string &op) {
    int bInv = (op == "11") ? 1 : 0;
    int carryOut = 0;
    int result = alu1bit(a, b, bInv, carryIn, op, carryOut);
    const string& opCode = op;
    char operation = selectOperation(opCode);

    cout << a << "\t" << b << "\t" << carryIn << "\t" << bInv << "\t\t" << op << "\t" << result << "\t\t" << carryOut << "\t\t" << "A " << operation << " B" << endl;
}

void equalLine () {
    cout << "==============================================================================" << endl;
}
void dashLine() {
    cout << "------------------------------------------------------------------------------" << endl;
}

void displayResults(const string &opCode) {
    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            for (int k=0; k<2; k++) {
                aluDriverOutput (i, j, k, opCode);
            }
        }
    }
}

int main() {
    equalLine();
    cout << "ALU Test" << endl;
    equalLine();
    cout << "A\tb\tcin\tcout\top\tresult\tcout\tOperation Used" << endl;
    dashLine();

    // AND
    displayResults("00");
    dashLine();

    // OR
    displayResults("01");
    dashLine();

    // +
    displayResults("10");
    dashLine();

    // -
    displayResults("11");
    dashLine();
    return 0;
}