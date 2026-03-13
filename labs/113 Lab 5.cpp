#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

// Struct to hold the result of the 1-bit ALU
struct ALUResult {
    int sum;
    int carry;
};

// 1-bit ALU that performs addition and subtraction
ALUResult one_bit_alu(int a, int b, int cin, bool subtract) {
    ALUResult result{};
    int b_input = subtract ? ~b & 1 : b; // For subtraction, invert B
    result.sum = (a ^ b_input ^ cin) & 1; // XOR for sum
    result.carry = (a & b_input) | ((a ^ b_input) & cin); // Carry out
    return result;
}

// 16-bit ALU using the 1-bit ALU as a subcomponent
vector<int> add_16bit(const vector<int>& A, const vector<int>& B, bool subtract = false) {
    vector<int> result(16);
    int carry = subtract ? 1 : 0; // Start carry = 1 for subtraction (2's complement)

    for (int i = 15; i >= 0; i--) {
        ALUResult alu_res = one_bit_alu(A[i], B[i], carry, subtract);
        result[i] = alu_res.sum;
        carry = alu_res.carry;
    }

    return result;
}

// Booth's multiplier function
void booth_multiplier(bitset<16> multiplicand, bitset<16> multiplier) {
    bitset<16> AC;        // Accumulator initialized to 0
    bitset<16> MD = multiplicand; // Multiplicand
    bitset<16> MQ = multiplier;   // Multiplier
    int cycle_counter = 15;       // Cycle counter initialized to 15 (4 bits)
    int MQ_1 = 0;                 // Initially MQ-1 = 0

    // Print header
    cout << "Cycle-counter | MD               | AC               | MQ               | MQ-1 " << endl;
    cout << "-------------------------------------------------------------------------------" << endl;

    // Print the initial state
    cout << bitset<4>(cycle_counter) << "           | " << MD << " | " << AC << " | " << MQ << " | " << MQ_1 << endl;

    while (cycle_counter >= 0) {
        // Booth's algorithm steps
        if (MQ[0] == 1 && MQ_1 == 0) {
            // Subtract multiplicand (AC = AC - MD)
            vector<int> ac_vector(16), md_vector(16);
            for (int i = 0; i < 16; i++) {
                ac_vector[i] = AC[i];
                md_vector[i] = MD[i];
            }
            vector<int> result = add_16bit(ac_vector, md_vector, true);
            for (int i = 0; i < 16; i++) {
                AC[i] = result[i];
            }
        } else if (MQ[0] == 0 && MQ_1 == 1) {
            // Add multiplicand (AC = AC + MD)
            vector<int> ac_vector(16), md_vector(16);
            for (int i = 0; i < 16; i++) {
                ac_vector[i] = AC[i];
                md_vector[i] = MD[i];
            }
            vector<int> result = add_16bit(ac_vector, md_vector, false);
            for (int i = 0; i < 16; i++) {
                AC[i] = result[i];
            }
        }

        // Arithmetic shift right (AC, MQ, and MQ-1 combined)
        MQ_1 = MQ[0]; // Save MQ0 before the shift
        MQ >>= 1;     // Shift MQ right by 1
        MQ[15] = AC[0]; // MQ[15] gets the previous AC[0]
        AC >>= 1;     // Shift AC right by 1 (arithmetic shift)

        // Decrement the cycle counter
        cycle_counter--;

        // Print the current state
        cout << bitset<4>(cycle_counter) << "           | " << MD << " | " << AC << " | " << MQ << " | " << MQ_1 << endl;
    }

    // Print final product
    cout << "Final Product: " << (AC.to_ulong() << 16 | MQ.to_ulong()) << endl;
}

// Main function to input operands and call Booth's multiplier
int main() {
    // Input for multiplicand and multiplier
    string MD_input, MQ_input;

    cout << "Please Enter MD (multiplicand) --> ";
    cin >> MD_input;
    bitset<16> MD(MD_input);

    cout << "Please Enter MQ (multiplier) --> ";
    cin >> MQ_input;
    bitset<16> MQ(MQ_input);

    // Call the Booth's multiplier
    booth_multiplier(MD, MQ);

    return 0;
}

