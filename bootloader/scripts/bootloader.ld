/* 1. 메모리 영역 정의 (우리가 설계한 16KB 기준) */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 16K
    SRAM (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* 2. 섹션 배치 */
SECTIONS
{
    /* 코드 영역: 벡터 테이블이 가장 앞에 와야 함 */
    .text :
    {
        KEEP(*(.isr_vector))    /* 스타트업 코드의 벡터 테이블 */
        *(.text)                /* 프로그램 코드 */
        *(.rodata)              /* 읽기 전용 데이터 (상수 등) */
        . = ALIGN(4);
        _etext = .;             /* 코드의 끝 지점 심볼 */
    } > FLASH

    /* 초기화 데이터 영역 (Flash에 저장되었다가 실행 시 RAM으로 복사됨) */
    _sidata = LOADADDR(.data);
    .data :
    {
        _sdata = .;             /* RAM 내 데이터 시작 지점 */
        *(.data)
        . = ALIGN(4);
        _edata = .;             /* RAM 내 데이터 끝 지점 */
    } > SRAM AT > FLASH

    /* 미초기화 데이터 영역 (실행 시 0으로 초기화됨) */
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM

    /* 기본 스택 위치 (SRAM의 끝 지점) */
    _estack = ORIGIN(SRAM) + LENGTH(SRAM);
}
