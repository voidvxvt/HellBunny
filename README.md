# HellBunny 🐇🔥

>hopping to the system service call stubs in ntdll.dll or win32u.dll

HellBunny is a malleable shellcode loader written in C and Assembly utilizing direct and indirect syscalls for evading EDR hooks. 
It can be built as EXE, DLL, or XLL and offers a variety of QoL features that make it more adaptable.

<p align="center"> <img src="hellbunny_icon.webp" width="400"/> </p>

The purpose of this research project was to develop a comprehensive understanding of the architecture and internals of the Windows operating system, including the Native API, the Process and Thread Environment Block, and the PE file format, while also exploring the techniques used by EDR software to collect telemetry and detect malware at runtime.

## Features
- **System Calls**
    - call the NtAPI directly
    - perform direct system calls
    - perform indirect system calls in ntdll.dll
    - perform indirect system calls in win32u.dll
    - System Service Number (SSN) resolving through Hell's Gate and Halo's Gate + an early hook detection (JMP opcode) 
- **Compile as**
    - EXE
    - DLL
    - XLL
- **CRT Independence**
- **Shellcode Encryption**:
    - AES-256-CTR 
    - XChaCha20
- **Custom DLL Entrypoint Name** ( for sideloading attacks )
- **IAT Camouflage** 
- **Custom Module and Function Resolving from PEB (for hiding function imports)**
- **supports local and remote injection. design is open for implementation of additional injection methods** 
- **String Hashing Algorithms:**
    - CRC32B
    - FNV1A32B
    - DJB2
- **String Encryption**: Strings are XOR encrypted using a single byte key to obfuscate them within the binary
- shellcode is downloaded from a remote webserver
- commandline arguments are erased if executed by rundll32


## Credits
- [@bakki](https://x.com/shubakki)
- [@C5pider](https://x.com/c5pider) -> [Havoc C2 Implant](https://github.com/HavocFramework/Havoc/tree/main/payloads/Demon)
- [MalDevAcademy](https://maldevacademy.com/)
- [https://github.com/realoriginal/titanldr-ng](https://github.com/realoriginal/titanldr-ng)
- [https://github.com/am0nsec/HellsGate](https://github.com/am0nsec/HellsGate)
- [https://blog.sektor7.net/#!res/2021/halosgate.md](https://blog.sektor7.net/#!res/2021/halosgate.md)
- [https://github.com/mrexodia/phnt-single-header](https://github.com/mrexodia/phnt-single-header)
- [https://github.com/kokke/tiny-AES-c](https://github.com/kokke/tiny-AES-c)
- [https://github.com/spcnvdr/xchacha20](https://github.com/spcnvdr/xchacha20)
