#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <modbus.h>
#include <errno.h>
// gcc -o modfuzz modfuzz.c -lmodbus. make sure to insall the modbus library 
void usageExit() {
    printf("FuzzyModbus v0.4\nModbusTCP Fuzzing Tool\n");
    printf("-h or --help\tHelp Menu\n");
    printf("-f or --func-code <Code>\tModbus Function Code\n");
    printf("-F or --flood\tOPTIONAL Modbus Flood/DoS Attack\n");
    printf("-t or --target <IP_Addr>\tTarget Modbus Server IP Address\n");
    printf("-p or --port <Port>\tDEFAULT=502 Target Modbus Server Port\n");
    printf("-u or --uid <UID>\tDEFAULT=1 Unit ID\n");
    printf("-a or --address <Reg_Addr>\tDEFAULT=0 First Register Address\n");
    printf("-c or --count <Count>\tDEFAULT=1 Count of Registers to read/write\n");
    printf("-i or --input <value>\tDEFAULT_BIT=0/DEFAULT_REG=666 Input Value to write\n\n");
    printf("Supported Function Codes:\n");
    printf("  Read Coils (1), Read Discrete Inputs (2), Read Holding Registers (3), Read Input Registers (4),\n");
    printf("  Write Single Coil (5), Write Single Register (6), Write Multiple Coils (15), Write Multiple Registers (16)\n");
    exit(1);
}

// Function prototypes
void readCoils(char *rq_type, modbus_t *ctx, int bit_addr, int bit_nb);
void readDiscreteInputs(char *rq_type, modbus_t *ctx, int bit_addr, int bit_nb);
void readHoldingRegisters(char *rq_type, modbus_t *ctx, int reg_addr, int reg_nb);
void readInputRegisters(char *rq_type, modbus_t *ctx, int reg_addr, int reg_nb);
void writeSingleCoil(char *rq_type, modbus_t *ctx, int bit_addr, int input_bit);
void writeSingleRegister(char *rq_type, modbus_t *ctx, int reg_addr, int input_value);
void writeMultipleCoils(char *rq_type, modbus_t *ctx, int bit_addr, int bit_nb, int input_value);
void writeMultipleRegisters(char *rq_type, modbus_t *ctx, int reg_addr, int reg_nb, int input_value);

// Main function
int main(int argc, char *argv[]) {
    int opt;
    int func_code = -1;
    char *target_ip = NULL;
    int port = 502;
    int uid = 1;
    int address = 0;
    int count = 1;
    int input_value = 666;
    char *rq_type = "single";

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"func-code", required_argument, 0, 'f'},
        {"flood", no_argument, 0, 'F'},
        {"target", required_argument, 0, 't'},
        {"port", required_argument, 0, 'p'},
        {"uid", required_argument, 0, 'u'},
        {"address", required_argument, 0, 'a'},
        {"count", required_argument, 0, 'c'},
        {"input", required_argument, 0, 'i'},
        {0, 0, 0, 0}
    };

    // Parse command-line arguments
    while ((opt = getopt_long(argc, argv, "hf:Ft:p:u:a:c:i:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                usageExit();
                break;
            case 'f':
                func_code = atoi(optarg);
                break;
            case 'F':
                rq_type = "flood";
                break;
            case 't':
                target_ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'u':
                uid = atoi(optarg);
                break;
            case 'a':
                address = atoi(optarg);
                break;
            case 'c':
                count = atoi(optarg);
                break;
            case 'i':
                input_value = atoi(optarg);
                break;
            default:
                usageExit();
        }
    }

    // Check for required options
    if (func_code == -1 || target_ip == NULL) {
        fprintf(stderr, "Error: Function code and target IP are required.\n");
        usageExit();
    }

    // Set up Modbus context
    modbus_t *ctx = modbus_new_tcp(target_ip, port);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the Modbus context: %s\n", modbus_strerror(errno));
        exit(1);
    }
    modbus_set_slave(ctx, uid);

    // Connect to Modbus server
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        exit(1);
    }

    // Execute the requested Modbus function
    switch (func_code) {
        case 1:
            readCoils(rq_type, ctx, address, count);
            break;
        case 2:
            readDiscreteInputs(rq_type, ctx, address, count);
            break;
        case 3:
            readHoldingRegisters(rq_type, ctx, address, count);
            break;
        case 4:
            readInputRegisters(rq_type, ctx, address, count);
            break;
        case 5:
            writeSingleCoil(rq_type, ctx, address, input_value);
            break;
        case 6:
            writeSingleRegister(rq_type, ctx, address, input_value);
            break;
        case 15:
            writeMultipleCoils(rq_type, ctx, address, count, input_value);
            break;
        case 16:
            writeMultipleRegisters(rq_type, ctx, address, count, input_value);
            break;
        default:
            fprintf(stderr, "Error: Unsupported function code.\n");
            modbus_close(ctx);
            modbus_free(ctx);
            usageExit();
    }

    // Clean up
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}

// Implement the Modbus functions here
void readCoils(char *rq_type, modbus_t *ctx, int bit_addr, int bit_nb) {
    // Implementation here
}

void readDiscreteInputs(char *rq_type, modbus_t *ctx, int bit_addr, int bit_nb) {
    // Implementation here
}

void readHoldingRegisters(char *rq_type, modbus_t *ctx, int reg_addr, int reg_nb) {
    // Implementation here
}

void readInputRegisters(char *rq_type, modbus_t *ctx, int reg_addr, int reg_nb) {
    // Implementation here
}

void writeSingleCoil(char *rq_type, modbus_t *ctx, int bit_addr, int input_bit) {
    // Implementation here
}

void writeSingleRegister(char *rq_type, modbus_t *ctx, int reg_addr, int input_value) {
    // Implementation here
}

void writeMultipleCoils(char *rq_type, modbus_t *ctx, int bit_addr, int bit_nb, int input_value) {
    // Implementation here
}

void writeMultipleRegisters(char *rq_type, modbus_t *ctx, int reg_addr, int reg_nb, int input_value) {
    // Implementation here
}
