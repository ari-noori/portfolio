# 5-stage Pipelined RISC Processor
## Implemented in Verilog
**Project Phases:**
1. Built a single cycle non-pipelined processor with a highly idealized memory.
2. Pipelined into 5 distinct stages but while still using a highly idealized memory.
3. Transitioned to using a more realistic banked memory module that cannot respond to requests in a single cycle.
4. Architected a two-way set associative cache.
5. Incorporated cahce into the full processor.
6. Added optimizations for additional processor performance.
    - Execute -> Execute forwarding
    - Memory -> Execute forwarding
    - Branches "predict-not-taken"
    - Single cycle cache hits
