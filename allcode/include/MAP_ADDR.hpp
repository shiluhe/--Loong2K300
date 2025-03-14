#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define LS_GPIO_REUSE_ADDR 0x16000490   // GPIO复用配置寄存器基地址
#define PAGE_SIZE          0x10000      // 页大小

#define LS_GPIO_REUSE_OFS  0x04   // 复用配置寄存器偏移量

/************************************宏函数****************************************/
#define BIT(n)               (1 << (n))
#define LS_readl(addr)       (*(volatile uint32_t *)(addr))         // 读寄存器值
#define LS_writel(addr, val) (*(volatile uint32_t *)(addr) = (val)) // 写寄存器值

// 将物理地址映射为虚拟地址
void *AddressMap(uint32_t PA, size_t size);

// 复用 GPIO
void GpioReuse(uint8_t gpio, int mux);