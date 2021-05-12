

2021-05-11 20:28                                                          Page 1


00010000 T begtext	   000134c5 T _pcb_dump	      00016fc5 T cwrite
00010000 T _start	   000135ed T _context_dump   00016fe4 T swritech
0001006c T __isr_restore   000136e8 T _context_dump_a 00017009 T swrites
00010ccb T __cio_setscroll 0001378e T _active_dump    0001703a T swrite
00010d71 T __cio_moveto	   00013a94 T _que_init	      00017059 T str2int
00010dd5 T __cio_putchar_a 00013aef T _que_alloc      000170d3 T strlen
00010e51 T __cio_putchar   00013b59 T _que_free	      000170f9 T strcpy
00010f53 T __cio_puts_at   00013bba T _que_length     00017127 T strcat
00010f99 T __cio_puts	   00013c0d T _que_enque      00017161 T strcmp
00010fcf T __cio_write	   00013da6 T _que_deque      0001719b T pad
0001100c T __cio_clearscro 00013e60 T _que_peek	      000171c1 T padstr
0001108a T __cio_clearscre 00013ec6 T _que_dump	      00017259 T sprint
000110ff T __cio_scroll	   00013fd4 T _sched_init     0001748f T cvt_dec0
000115d2 T __cio_printf_at 0001407b T _schedule	      0001751c T cvt_dec
000115f6 T __cio_printf	   00014192 T _dispatch	      00017563 T cvt_hex
000117a1 T __cio_getchar   000145be T _sio_init	      000175f6 T cvt_oct
0001183a T __cio_gets	   00014753 T _sio_enable     00017680 T report
0001188f T __cio_input_que 000147ce T _sio_disable    000176e8 T exit
000118bd T __cio_init	   00014849 T _sio_inq_length 000176f0 T read
00011b80 T __panic	   00014857 T _sio_readc      000176f8 T write
00011ba0 T __init_interrup 000148bc T _sio_reads      00017700 T getpid
00011bb7 T __install_isr   0001495b T _sio_writec     00017708 T getppid
00011be0 T __delay	   000149cf T _sio_write      00017710 T gettime
00011d4d T _clk_init	   00014a7a T _sio_puts	      00017718 T getprio
00011e44 T _init	   00014abb T _sio_dump	      00017720 T setprio
00011fd9 T _shell	   00014c3c T _stk_init	      00017728 T kill
00012288 T __insl	   00014cd0 T _stk_alloc      00017730 T sleep
000122cb T __put_char_or_c 00014d1f T _stk_free	      00017738 T spawn
00012309 T __bound	   00014d45 T _stk_dump	      00017740 T wait
00012331 T __memset	   000157c4 T _sys_init	      00017748 T bogus
0001235a T __memclr	   0001587e T _force_exit     00017750 T exit_helper
00012380 T __memcpy	   000159fb T get_drive_info  0001b000 D __isr_stub_tabl
000123b2 T __strlen	   00016410 T _get_device_lis 0001b50c D __hexdigits
000123d8 T __strcmp	   0001643b T _write_disk     0001b51d B __bss_start
00012412 T __strcpy	   000164b5 T _read_disk      0001b51d D _edata
00012446 T __strcat	   0001652f T _ahci_init      0001ca94 B scroll_max_x
00012480 T __pad	   000169d0 T pciConfigReadWo 0001ca98 B max_x
000124a6 T __padstr	   00016a78 T getVendorID     0001ca9c B curr_x
0001253e T __sprint	   00016aaf T getHeaderType   0001caa0 B min_y
00012774 T __cvtdec0	   00016ae6 T checkFunction   0001caa4 B scroll_max_y
00012801 T __cvtdec	   00016bfc T checkDevice     0001caa8 B scroll_min_y
00012848 T __cvthex	   00016cd3 T _enumerate_pci_ 0001caac B min_x
000128c7 T __cvtoct	   00016d2a T _get_controller 0001cab0 B curr_y
00012955 T _kpanic	   00016d55 T __inb	      0001cab4 B max_y
00012a99 T _km_init	   00016d60 T __inw	      0001cab8 B scroll_min_x
00012c99 T _km_dump	   00016d6c T __inl	      0001cac0 B __isr_table
00012d05 T _km_page_alloc  00016d77 T __outb	      0001cec0 B _sleeping
00012e01 T _km_page_free   00016d83 T __outw	      0001cec4 B _system_time
00012fbd T _km_slice_alloc 00016d90 T __outl	      0001cee0 B b512
00013085 T _km_slice_free  00016d9c T __get_flags     0001d0e0 B _system_stack
000131aa T _pcb_alloc	   00016d9f T __pause	      0001d100 B b256
000131fc T _pcb_free	   00016da6 T __get_ra	      0001d200 B _system_esp
00013228 T _pcb_find_pid   00016daa T init	      0001d220 B _next_pid
00013281 T _pcb_cleanup	   00016ed7 T idle	      0001d224 B _active_procs
000132f2 T _proc_init	   00016f6f T cwritech	      0001d240 B _ptable
0001336a T _proc_create	   00016f94 T cwrites	      0001d2a4 B _ready







2021-05-11 20:28                                                          Page 2


0001d2b4 B _current	   0001d2b8 B _reading	      0001d2bc B _end




























































