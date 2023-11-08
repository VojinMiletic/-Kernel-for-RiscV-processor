.global trap
.align 4

trap:
    j syscall
    j timer
    addi a0, a0, 0
    addi a0, a0, 0
    addi a0, a0, 0
    addi a0, a0, 0
    addi a0, a0, 0
    addi a0, a0, 0
    addi a0, a0, 0
    j console

syscall:
    addi sp, sp, -256
        #Ne cuvamo registar x10 jer je to a0 a kroz njega vracamo povratnu vrednost
        .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
         sd x\index, \index * 8(sp)
         .endr


        call _ZN5Riscv13handleSyscallEv

        #Ne restauiramo registar x10 jer je to a0 a kroz njega vracamo povratnu vrednost
        .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, \index * 8(sp)
        .endr

        addi sp, sp, 256
        sret

timer:
       addi sp, sp, -256
       #Cuvamo registar x10 jer je to a0 a on nam je bitan u smislu tajmera 
       .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       sd x\index, \index * 8(sp)
       .endr


       call _ZN5Riscv11handleTimerEv

       #Restauiramo registar x10 jer je to a0 a on nam je bitan u smislu tajmera 
       .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       ld x\index, \index * 8(sp)
       .endr

       addi sp, sp, 256
       sret

console:
       addi sp, sp, -256
       #Cuvamo registar x10 jer je to a0  a on nam je bitan u smislu konzole 
       .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       sd x\index, \index * 8(sp)
       .endr


       call _ZN5Riscv13handleConsoleEv

       #Restauiramo registar x10 jer je to a0  a on nam je bitan u smislu konzole 
       .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
       ld x\index, \index * 8(sp)
       .endr

       addi sp, sp, 256
       sret