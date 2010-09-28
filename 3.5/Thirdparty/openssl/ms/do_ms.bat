
perl util\mkfiles.pl %1 >MINFO
perl util\mk1mf.pl %1 no-asm VC-WIN32 >ms\nt.mak
perl util\mk1mf.pl %1 dll no-asm VC-WIN32 >ms\ntdll.mak
perl util\mk1mf.pl %1 no-asm VC-CE >ms\ce.mak
perl util\mk1mf.pl %1 dll no-asm VC-CE >ms\cedll.mak

perl util\mkdef.pl 32 libeay > ms\libeay32.def
perl util\mkdef.pl 32 ssleay > ms\ssleay32.def
