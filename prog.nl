

2021-05-09 22:24                                                          Page 1


00010000 T begtext	   000134c5 T _pcb_dump	      00016d71 T cwrite
00010000 T _start	   000135ed T _context_dump   00016d90 T swritech
0001006c T __isr_restore   000136e8 T _context_dump_a 00016db5 T swrites
00010ccb T __cio_setscroll 0001378e T _active_dump    00016de6 T swrite
00010d71 T __cio_moveto	   00013a94 T _que_init	      00016e05 T str2int
00010dd5 T __cio_putchar_a 00013aef T _que_alloc      00016e7f T strlen
00010e51 T __cio_putchar   00013b59 T _que_free	      00016ea5 T strcpy
00010f53 T __cio_puts_at   00013bba T _que_length     00016ed3 T strcat
00010f99 T __cio_puts	   00013c0d T _que_enque      00016f0d T strcmp
00010fcf T __cio_write	   00013da6 T _que_deque      00016f47 T pad
0001100c T __cio_clearscro 00013e60 T _que_peek	      00016f6d T padstr
0001108a T __cio_clearscre 00013ec6 T _que_dump	      00017005 T sprint
000110ff T __cio_scroll	   00013fd4 T _sched_init     0001723b T cvt_dec0
000115d2 T __cio_printf_at 0001407b T _schedule	      000172c8 T cvt_dec
000115f6 T __cio_printf	   00014192 T _dispatch	      0001730f T cvt_hex
000117a1 T __cio_getchar   000145be T _sio_init	      000173a2 T cvt_oct
0001183a T __cio_gets	   00014753 T _sio_enable     0001742c T report
0001188f T __cio_input_que 000147ce T _sio_disable    00017494 T exit
000118bd T __cio_init	   00014849 T _sio_inq_length 0001749c T read
00011b80 T __panic	   00014857 T _sio_readc      000174a4 T write
00011ba0 T __init_interrup 000148bc T _sio_reads      000174ac T getpid
00011bb7 T __install_isr   0001495b T _sio_writec     000174b4 T getppid
00011be0 T __delay	   000149cf T _sio_write      000174bc T gettime
00011d4d T _clk_init	   00014a7a T _sio_puts	      000174c4 T getprio
00011e44 T _init	   00014abb T _sio_dump	      000174cc T setprio
00011fd9 T _shell	   00014c3c T _stk_init	      000174d4 T kill
00012288 T __insl	   00014cd0 T _stk_alloc      000174dc T sleep
000122cb T __put_char_or_c 00014d1f T _stk_free	      000174e4 T spawn
00012309 T __bound	   00014d45 T _stk_dump	      000174ec T wait
00012331 T __memset	   000157c4 T _sys_init	      000174f4 T bogus
0001235a T __memclr	   0001587e T _force_exit     000174fc T exit_helper
00012380 T __memcpy	   000159fb T getDrives	      0001b000 D __isr_stub_tabl
000123b2 T __strlen	   00016410 T getDevLst	      0001b50c D __hexdigits
000123d8 T __strcmp	   0001643b T writeDisk	      0001b51d B __bss_start
00012412 T __strcpy	   00016462 T readDisk	      0001b51d D _edata
00012446 T __strcat	   00016489 T _ahci_init      0001c814 B scroll_max_x
00012480 T __pad	   0001677c T pciConfigReadWo 0001c818 B max_x
000124a6 T __padstr	   00016824 T getVendorID     0001c81c B curr_x
0001253e T __sprint	   0001685b T getHeaderType   0001c820 B min_y
00012774 T __cvtdec0	   00016892 T checkFunction   0001c824 B scroll_max_y
00012801 T __cvtdec	   000169a8 T checkDevice     0001c828 B scroll_min_y
00012848 T __cvthex	   00016a7f T enumeratePCIDev 0001c82c B min_x
000128c7 T __cvtoct	   00016ad6 T getController   0001c830 B curr_y
00012955 T _kpanic	   00016b01 T __inb	      0001c834 B max_y
00012a99 T _km_init	   00016b0c T __inw	      0001c838 B scroll_min_x
00012c99 T _km_dump	   00016b18 T __inl	      0001c840 B __isr_table
00012d05 T _km_page_alloc  00016b23 T __outb	      0001cc40 B _sleeping
00012e01 T _km_page_free   00016b2f T __outw	      0001cc44 B _system_time
00012fbd T _km_slice_alloc 00016b3c T __outl	      0001cc60 B b512
00013085 T _km_slice_free  00016b48 T __get_flags     0001ce60 B _system_stack
000131aa T _pcb_alloc	   00016b4b T __pause	      0001ce80 B b256
000131fc T _pcb_free	   00016b52 T __get_ra	      0001cf80 B _system_esp
00013228 T _pcb_find_pid   00016b56 T init	      0001cfa0 B _next_pid
00013281 T _pcb_cleanup	   00016c83 T idle	      0001cfa4 B _active_procs
000132f2 T _proc_init	   00016d1b T cwritech	      0001cfc0 B _ptable
0001336a T _proc_create	   00016d40 T cwrites	      0001d024 B _ready







2021-05-09 22:24                                                          Page 2


0001d034 B _current	   0001d038 B _reading	      0001d03c B _end




























































