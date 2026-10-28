def compute_final_aligned_checksum(file_path):
    
    START_ADDR = 0x01010000
    END_ADDR   = 0x0102F496 # C loop uses < END_ADDR (Exclusive)
    
    memory = {}
    extended_linear_address = 0

    # 1. Parse HEX file into a virtual memory map
    with open(file_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line.startswith(':') or len(line) < 11:
                continue
            
            byte_count = int(line[1:3], 16)
            address = int(line[3:7], 16)
            record_type = int(line[7:9], 16)
            data_str = line[9:9 + (byte_count * 2)]
            
            if record_type == 0: # Data Record
                abs_addr = extended_linear_address + address
                for i in range(0, byte_count):
                    val = int(data_str[i*2 : i*2 + 2], 16)
                    memory[abs_addr + i] = val
            elif record_type == 4: # Extended Linear Address (The 0x0101 prefix)
                extended_linear_address = int(data_str, 16) << 16
            elif record_type == 1: # End of File
                break

    # 2. Match the C logic: uint32_t checksum = 0;
    checksum = 0
    
    # 3. Match the C loop: addr = START; addr < END; addr += 2
    # In Python range(start, stop, step), 'stop' is exclusive.
    for addr in range(START_ADDR, END_ADDR, 2):
        # Fetch 2 bytes (defaulting to 0xFF if Flash is empty)
        low_byte = memory.get(addr, 0xFF)
        high_byte = memory.get(addr + 1, 0xFF)
        
        # 4. Assemble 16-bit word (Little Endian: *(uint16_t *)addr)
        word = (high_byte << 8) | low_byte
        
        # 5. XOR into the accumulator
        checksum ^= word

    # 6. Final Result
    print(f"--- Logic-Aligned Audit ---")
    print(f"Start (Inc): 0x{START_ADDR:08X}")
    print(f"End   (Exc): 0x{END_ADDR:08X}")
    print(f"Python Checksum: 0x{checksum:04X}")
    return checksum

# Run on your uploaded file
compute_final_aligned_checksum('SR20.hex')
