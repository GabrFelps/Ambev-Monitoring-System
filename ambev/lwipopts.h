#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// --- CONFIGURAÇÃO CRÍTICA PARA O PICO SDK (SEM OS) ---
// Define que não estamos usando sistema operacional (resolve o erro sys_arch.h)
#define NO_SYS                      1 

// Desativa sockets e netconn (usados apenas com OS)
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0

// --- RESOLVE O ERRO DE HOSTNAME ---
// Ativa a função netif_set_hostname que o driver da cyw43 exige
#define LWIP_NETIF_HOSTNAME         1 

// --- OTIMIZAÇÃO DE MEMÓRIA ---
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    16384
#define MEM_LIBC_MALLOC             0

// --- PROTOCOLOS NECESSÁRIOS PARA MQTT ---
#define LWIP_IPV4                   1
#define LWIP_TCP                    1
#define LWIP_UDP                    1
#define LWIP_ARP                    1
#define LWIP_ETHERNET               1
#define LWIP_ICMP                   1
#define LWIP_RAW                    1
#define LWIP_CHKSUM_ALGORITHM       3

// --- CONFIGURAÇÕES ESPECÍFICAS PARA MQTT ---
#define MQTT_OUTPUT_RINGBUF_SIZE    1024
#define MQTT_VAR_HEADER_BUFFER_LEN  128
#define LWIP_TCP_KEEPALIVE          1

// --- DESATIVAR ESTATÍSTICAS (PARA ECONOMIZAR MEMÓRIA) ---
#define MEM_STATS                   0
#define SYS_STATS                   0
#define MEMP_STATS                  0
#define LINK_STATS                  0

#endif