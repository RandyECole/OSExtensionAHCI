

2021-04-15 01:55                                                          Page 1


00010000 T begtext	   000134c5 T _pcb_dump	      00016a89 T cwrite
00010000 T _start	   000135ed T _context_dump   00016aa8 T swritech
0001006c T __isr_restore   000136e8 T _context_dump_a 00016acd T swrites
00010ccb T __cio_setscroll 0001378e T _active_dump    00016afe T swrite
00010d71 T __cio_moveto	   00013a94 T _que_init	      00016b1d T str2int
00010dd5 T __cio_putchar_a 00013aef T _que_alloc      00016b97 T strlen
00010e51 T __cio_putchar   00013b59 T _que_free	      00016bbd T strcpy
00010f53 T __cio_puts_at   00013bba T _que_length     00016beb T strcat
00010f99 T __cio_puts	   00013c0d T _que_enque      00016c25 T strcmp
00010fcf T __cio_write	   00013da6 T _que_deque      00016c5f T pad
0001100c T __cio_clearscro 00013e60 T _que_peek	      00016c85 T padstr
0001108a T __cio_clearscre 00013ec6 T _que_dump	      00016d1d T sprint
000110ff T __cio_scroll	   00013fd4 T _sched_init     00016f53 T cvt_dec0
000115d2 T __cio_printf_at 0001407b T _schedule	      00016fe0 T cvt_dec
000115f6 T __cio_printf	   00014192 T _dispatch	      00017027 T cvt_hex
000117a1 T __cio_getchar   000145be T _sio_init	      000170ba T cvt_oct
0001183a T __cio_gets	   00014753 T _sio_enable     00017144 T report
0001188f T __cio_input_que 000147ce T _sio_disable    000171ac T exit
000118bd T __cio_init	   00014849 T _sio_inq_length 000171b4 T read
00011b80 T __panic	   00014857 T _sio_readc      000171bc T write
00011ba0 T __init_interrup 000148bc T _sio_reads      000171c4 T getpid
00011bb7 T __install_isr   0001495b T _sio_writec     000171cc T getppid
00011be0 T __delay	   000149cf T _sio_write      000171d4 T gettime
00011d4d T _clk_init	   00014a7a T _sio_puts	      000171dc T getprio
00011e44 T _init	   00014abb T _sio_dump	      000171e4 T setprio
00011fd9 T _shell	   00014c3c T _stk_init	      000171ec T kill
00012288 T __insl	   00014cd0 T _stk_alloc      000171f4 T sleep
000122cb T __put_char_or_c 00014d1f T _stk_free	      000171fc T spawn
00012309 T __bound	   00014d45 T _stk_dump	      00017204 T wait
00012331 T __memset	   000157c4 T _sys_init	      0001720c T bogus
0001235a T __memclr	   0001587e T _force_exit     00017214 T exit_helper
00012380 T __memcpy	   000159a4 T ide_read	      0001b000 D __isr_stub_tabl
000123b2 T __strlen	   00015ad9 T ide_write	      0001b50c D __hexdigits
000123d8 T __strcmp	   00015c19 T ide_read_buffer 0001b52c B __bss_start
00012412 T __strcpy	   00015d64 T ide_polling     0001b52c D _edata
00012446 T __strcat	   00015e08 T ide_print_error 0001c6e0 B ide_buf
00012480 T __pad	   00016086 T ide_initialize  0001cee8 B scroll_max_x
000124a6 T __padstr	   00016503 T pciConfigReadWo 0001ceec B max_x
0001253e T __sprint	   000165aa T getVendorID     0001cef0 B curr_x
00012774 T __cvtdec0	   000165e1 T getHeaderType   0001cef4 B min_y
00012801 T __cvtdec	   00016618 T checkFunction   0001cef8 B scroll_max_y
00012848 T __cvthex	   000166eb T checkDevice     0001cefc B scroll_min_y
000128c7 T __cvtoct	   000167c2 T enumeratePCIDev 0001cf00 B min_x
00012955 T _kpanic	   00016819 T __inb	      0001cf04 B curr_y
00012a99 T _km_init	   00016824 T __inw	      0001cf08 B max_y
00012c99 T _km_dump	   00016830 T __inl	      0001cf0c B scroll_min_x
00012d05 T _km_page_alloc  0001683b T __outb	      0001cf20 B __isr_table
00012e01 T _km_page_free   00016847 T __outw	      0001d320 B _sleeping
00012fbd T _km_slice_alloc 00016854 T __outl	      0001d324 B _system_time
00013085 T _km_slice_free  00016860 T __get_flags     0001d340 B b512
000131aa T _pcb_alloc	   00016863 T __pause	      0001d540 B _system_stack
000131fc T _pcb_free	   0001686a T __get_ra	      0001d560 B b256
00013228 T _pcb_find_pid   0001686e T init	      0001d660 B _system_esp
00013281 T _pcb_cleanup	   0001699b T idle	      0001d680 B _next_pid
000132f2 T _proc_init	   00016a33 T cwritech	      0001d684 B _active_procs
0001336a T _proc_create	   00016a58 T cwrites	      0001d6a0 B _ptable







2021-04-15 01:55                                                          Page 2


0001d704 B _ready	   0001d718 B _reading	      0001d820 B channels
0001d714 B _current	   0001d720 B ide_devices     0001d830 B _end



























































