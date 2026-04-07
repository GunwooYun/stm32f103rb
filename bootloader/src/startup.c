#include <stdint.h>

/* 링커 스크립트에서 정의한 외부 심볼 선언 */
extern uint32_t _estack;    /* 스택 시작 주소 (SRAM 끝) */
extern uint32_t _sdata;     /* RAM 내 .data 섹션 시작 */
extern uint32_t _edata;     /* RAM 내 .data 섹션 끝 */
extern uint32_t _sidata;    /* Flash 내 .data 섹션의 LMA 시작점 */
extern uint32_t _sbss;      /* RAM 내 .bss 섹션 시작 */
extern uint32_t _ebss;      /* RAM 내 .bss 섹션 끝 */

/* main 함수 선언 */
extern int main(void);

/* Reset Handler: CPU가 리셋 직후 실행하는 함수 */
void Reset_Handler(void) {

    /* 1. Flash에 있는 초기화 데이터(.data)를 RAM으로 복사 */
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint8_t *pDst = (uint8_t *)&_sdata;
    uint8_t *pSrc = (uint8_t *)&_sidata;

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = *pSrc++;
    }

    /* 2. 미초기화 데이터 영역(.bss)을 0으로 초기화 */
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint8_t *)&_sbss;

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = 0;
    }

    /* 3. 시스템 초기화 호출 (필요 시 추가) 및 main 실행 */
    main();

    /* main이 종료되지 않도록 무한 루프 */
    while (1);
}

/* 기본 예외 핸들러 (디버깅용 무한 루프) */
void Default_Handler(void) {
    while (1);
}

/* 인터럽트 벡터 테이블 정의 */
/* .isr_vector 섹션에 배치되어 Flash의 0x08000000 위치에 저장됨 */
__attribute__((section(".isr_vector")))
uint32_t vectors[] = {
    (uint32_t)&_estack,         /* 0x00: Stack Pointer 초기값 */
    (uint32_t)Reset_Handler,    /* 0x04: Reset Vector */
    (uint32_t)Default_Handler,  /* 0x08: NMI Handler */
    (uint32_t)Default_Handler,  /* 0x0C: HardFault Handler */
    /* 필요에 따라 추가 인터럽트 핸들러 배치 가능 */
};
