; Sistemas Operacionais - MC504A_2024S2
; Desenvolvido por: Antônio Hideto Borges Kotsubo (236041) & Pedro Sanchez Bitencourt (231133)

BITS 16
org 0x7c00

start:
  cli                            ; desabilita interrupções
  xor ax, ax                     ; zera o registrador ax
  mov ds, ax                     ; define o segmento de dados
  mov es, ax                     ; define o segmento extra
  mov ss, ax                     ; define o segmento de pilha
  mov sp, 0x7c00                 ; define o ponteiro de pilha

  call main                     ; chama a função main

  hlt                           ; para a CPU

main:
  push ax                       ; salva o registrador ax na pilha
  call write                    ; chama a função write
  pop ax                        ; restaura o registrador ax
  ret                           ; retorna da função main

write:
  mov si, message               ; carrega o endereço da mensagem em si
  print_char:
    lodsb                       ; carrega o próximo byte da mensagem em al
    cmp al, 0                   ; compara al com 0 (fim da string)
    je done                     ; se al for 0, pula para done
    mov ah, 0x0e                ; função de teletipo da BIOS
    int 0x10                    ; chama a interrupção da BIOS
    jmp print_char              ; repete para o próximo caractere
  done:
    ret                         ; retorna da função write

message db 'Hello, World!', 0

times 510-($-$$) db 0           ; preenche o restante do setor de boot com zeros
dw 0xaa55                       ; assinatura do setor de boot