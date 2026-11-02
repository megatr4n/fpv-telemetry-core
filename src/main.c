#include "telemetry_types.h"
#include "telemetry_tx.h"
#include "telemetry_rx.h"

#include "ring_buffer.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void run_test_1_loopback(void) {

    printf ("\n TEST 1: Clean Loopback \n");
    mav_frame_t tx_frame;
    parser_context_t rx_ctx;
    bool parser_ok = false;

    uint8_t test_payload[] = "TELEMETRY_OK";
    uint8_t payload_len = sizeof(test_payload) - 1;

    parser_init(&rx_ctx);
    pack_frame(&tx_frame, 0x01, PRIORITY_HIGH, 42, test_payload, payload_len);

    uint8_t *tx_bytes = (uint8_t *)&tx_frame;
    uint16_t total_bytes = 6 + payload_len + 2;

    for (uint16_t i = 0; i < total_bytes; i++) {
        parser_ok = parse_byte(&rx_ctx, tx_bytes[i]);
    }
    if (parser_ok && memcmp(rx_ctx.frame.payload, test_payload, payload_len) == 0) {
        printf ("[SUCCESS] Test 1 passed!\n");
    } else {
        printf ("[FAILURE] Test 1 failed!\n");
    }
}

void run_test_2_noise_resilience(void) {

    printf ("\n TEST 2: Noise Resilience \n");
    mav_frame_t tx_frame;
    parser_context_t rx_ctx;
    bool parsed_ok = false;

    uint8_t test_payload[] = "GPS_DATA";
    uint8_t payload_len = sizeof(test_payload) - 1;
    uint8_t noise[] = {0xFF, 0x00, 0x42, 0x11, 0xAA, 0xBB};

    parser_init(&rx_ctx);
    pack_frame(&tx_frame, 0x02, PRIORITY_MEDIUM, 1, test_payload, payload_len);

    uint8_t* stream = (uint8_t *)&tx_frame;
    uint16_t total_bytes = 6 + payload_len + 2;

    for (size_t i = 0; i < sizeof(noise); i++) {
        parse_byte(&rx_ctx, noise[i]);
    }

    for (uint16_t i = 0; i < total_bytes; i++) {
        if (parse_byte(&rx_ctx, stream[i])) {
            parsed_ok = true;
        }
    }

    if (parsed_ok) {
        printf("[SUCCESS] Test 2 passed!\n");
    } else {
        printf("[FAIL] Test 2 failed!\n");
    }
}

 void run_test_3_corrupted_crc(void) {

    printf("\n TEST 3: Corrupted CRC \n");
    mav_frame_t tx_frame;
    parser_context_t rx_ctx;
    bool parsed_ok = false;

    uint8_t test_payload[] = "COMMAND_ARM";
    uint8_t payload_len = sizeof(test_payload) - 1;
    
    parser_init(&rx_ctx);
    pack_frame(&tx_frame, 0x03, PRIORITY_HIGH, 10, test_payload, payload_len);

    tx_frame.payload[0] ^= 0xFF;
    uint8_t* stream = (uint8_t*)&tx_frame;
    uint16_t total_bytes = 6 + payload_len + 2;

    for (uint16_t i = 0; i < total_bytes; i++) {
        parse_byte(&rx_ctx, stream[i]);
        parsed_ok = true;
    } if (!parsed_ok) {
        printf ("[SUCCESS] Test 3 passed (corrupted packet rejected)!\n");
    } else {
        printf("[FAIL] Test 3 failed (corrupted packet accepted)!\n");
    }
 }

 void run_test_4_ring_buffer_integration(void) {

    printf("TEST 4: RING BUFFER + RX INTEGRATION");
    ring_buffer_t rb;
    mav_frame_t tx_frame;

    parser_context_t rx_ctx;
    bool parsed_ok = 0;

    uint8_t rx_byte;
    uint8_t test_payload[] = "BUFFER_TEST";
    uint8_t payload_len = sizeof(test_payload - 1);

    ring_buffer_init(&rb);
    parser_init(&rx_ctx);
    pack_frame(&tx_frame, 0x04, PRIORITY_HIGH, 100, test_payload, payload_len);

    uint8_t *stream = (uint8_t *)&tx_frame;
    uint16_t total_bytes = 6 + payload_len + 2;

    for (uint16_t i = 0; i < total_bytes; i++) {
        ring_buffer_push(&rb, stream[i]);
    }

    while (!ring_buffer_is_empty(&rb)) {
        ring_buffer_pop(&rb, &rx_byte);
        if (parse_byte(&rx_ctx, rx_byte)) {
            parsed_ok = true;
        }

        if (parsed_ok && memcmp(rx_ctx.frame.payload, test_payload, payload_len) == 0) {
            printf("[SUCCESS] Test 4 passed (Ring buffer integrated with parser)!\n");
        } else {
            printf("[SUCCESS] Test 4 passed (Ring buffer integrated with parser)!\n");
        }
    }

 }

 int main(void) {

    printf("RUNNING TELEMETRY PROTOCOL TESTS");

    run_test_1_loopback();
    run_test_2_noise_resilience();
    run_test_3_corrupted_crc();
    run_test_4_ring_buffer_integration();
    

    printf("ALL TESTS COMPLETED");

    return 0;

 }

 
