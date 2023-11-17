# Fix Cmod injection from kernel space
  Cmod has a issue with being Unloaded from the kernel.
  Any time I reject it, I recive 'killed'
The Error:
        [ 1321.749258] BUG: kernel NULL pointer dereference, address: 0000000000000070
        [ 1321.749260] #PF: supervisor read access in kernel mode
        [ 1321.749260] #PF: error_code(0x0000) - not-present page
