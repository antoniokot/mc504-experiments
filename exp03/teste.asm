BITS 16
org 0x7c00

start:
  mov ah, 0x0e
  mov si, message

print:
  lodsb
  cmp al, 0
  je done
  int 0x10
  jmp print

done:
  hlt

message db 'Hello, World!', 0

times 510-($-$$) db 0
dw 0xaa55